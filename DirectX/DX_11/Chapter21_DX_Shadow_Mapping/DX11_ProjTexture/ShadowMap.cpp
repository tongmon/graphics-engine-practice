//***************************************************************************************
// ShadowMap.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "ShadowMap.h"

ShadowMap::ShadowMap(ID3D11Device* device, UINT width, UINT height)
: mWidth(width), mHeight(height), mDepthMapSRV(0), mDepthMapDSV(0)
{
    // 그림자 맵의 크기와 일치하는 뷰포트
    mViewport.TopLeftX = 0.0f;
    mViewport.TopLeftY = 0.0f;
    mViewport.Width    = static_cast<float>(width);
    mViewport.Height   = static_cast<float>(height);
    mViewport.MinDepth = 0.0f;
    mViewport.MaxDepth = 1.0f;

	// 깊이 * 스텐실 뷰는 비트들을 DXGI_FORMAT_D24_UNORM_S8_UINT로 해석하는 반면
    // 쉐이더 자원 뷰는 비트들을 DXGI_FORMAT_R24_UNORM_X8_TYPELESS로 해석할 것이므로
    // 무형식 텍스쳐를 사용한다.
	D3D11_TEXTURE2D_DESC texDesc;
    texDesc.Width     = mWidth;
    texDesc.Height    = mHeight;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format    = DXGI_FORMAT_R24G8_TYPELESS;
    texDesc.SampleDesc.Count   = 1;  
    texDesc.SampleDesc.Quality = 0;  
    texDesc.Usage          = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0; 
    texDesc.MiscFlags      = 0;

    ID3D11Texture2D* depthMap = 0;
    HR(device->CreateTexture2D(&texDesc, 0, &depthMap));

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = 0;
    dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;
    HR(device->CreateDepthStencilView(depthMap, &dsvDesc, &mDepthMapDSV));

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    HR(device->CreateShaderResourceView(depthMap, &srvDesc, &mDepthMapSRV));

    // 자원 뷰들을 다 뽑았으니 텍스쳐 해제
    ReleaseCOM(depthMap);

    // 영사로 뿌릴 프로젝터 역할을 하는 텍스쳐 자원

    D3D11_TEXTURE2D_DESC texDesc_2;
    texDesc_2.Width = mWidth;
    texDesc_2.Height = mHeight;
    texDesc_2.MipLevels = 1;
    texDesc_2.ArraySize = 1;
    texDesc_2.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc_2.SampleDesc.Count = 1;
    texDesc_2.SampleDesc.Quality = 0;
    texDesc_2.Usage = D3D11_USAGE_DEFAULT;
    texDesc_2.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    texDesc_2.CPUAccessFlags = 0;
    texDesc_2.MiscFlags = 0;

    ID3D11Texture2D* projMap = 0;
    HR(device->CreateTexture2D(&texDesc_2, 0, &projMap));

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc_2;
    srvDesc_2.Format = texDesc_2.Format;
    srvDesc_2.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc_2.Texture2D.MipLevels = texDesc_2.MipLevels;
    srvDesc_2.Texture2D.MostDetailedMip = 0;
    HR(device->CreateShaderResourceView(projMap, &srvDesc_2, &mProjSRV));

    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
    rtvDesc.Format = texDesc_2.Format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Texture2D.MipSlice = 0;
    HR(device->CreateRenderTargetView(projMap, &rtvDesc, &mProjRTV));

    // 자원 뷰들을 다 뽑았으니 텍스쳐 해제
    ReleaseCOM(projMap);
}

ShadowMap::~ShadowMap()
{
    ReleaseCOM(mDepthMapSRV);
	ReleaseCOM(mDepthMapDSV);

    ReleaseCOM(mProjSRV);
    ReleaseCOM(mProjRTV);
}

ID3D11ShaderResourceView* ShadowMap::DepthMapSRV()
{
    return mDepthMapSRV;
}

ID3D11ShaderResourceView* ShadowMap::ProjMapSRV()
{
    return mProjSRV;
}

void ShadowMap::BindDsvAndSetNullRenderTarget(ID3D11DeviceContext* dc)
{
    dc->RSSetViewports(1, &mViewport);

	// 깊이 버퍼에만 장면을 그릴 것이므로 렌더 대상을 널 값으로 설정한다.
	// 널 렌더 대상을 설정하면 색상 쓰기가 비활성화된다. (그리기 속도도 훨씬 빨라진다.)
    ID3D11RenderTargetView* renderTargets[1] = {0};
    dc->OMSetRenderTargets(1, renderTargets, mDepthMapDSV);
    
    dc->ClearDepthStencilView(mDepthMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void ShadowMap::BindProjAndSetProjRenderTarget(ID3D11DeviceContext* dc)
{
    dc->RSSetViewports(1, &mViewport);

    // 깊이 버퍼에만 장면을 그릴 것이므로 렌더 대상을 널 값으로 설정한다.
    // 널 렌더 대상을 설정하면 색상 쓰기가 비활성화된다. (그리기 속도도 훨씬 빨라진다.)
    ID3D11RenderTargetView* renderTargets[1] = { mProjRTV };
    dc->OMSetRenderTargets(1, renderTargets, NULL);

    dc->ClearRenderTargetView(mProjRTV, reinterpret_cast<const float*>(&Colors::Black));
}

