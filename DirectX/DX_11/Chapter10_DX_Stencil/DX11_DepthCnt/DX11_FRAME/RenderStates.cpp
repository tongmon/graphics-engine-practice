//***************************************************************************************
// RenderStates.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "RenderStates.h"

ID3D11RasterizerState* RenderStates::WireframeRS = 0;
ID3D11RasterizerState* RenderStates::NoCullRS = 0;

ID3D11BlendState* RenderStates::AlphaToCoverageBS = 0;
ID3D11BlendState* RenderStates::TransparentBS = 0;

ID3D11DepthStencilState* RenderStates::NormalDSS = 0;
ID3D11DepthStencilState* RenderStates::IncStencilDSS = 0;
ID3D11DepthStencilState* RenderStates::DistStencilDSS = 0;

void RenderStates::InitAll(ID3D11Device* device)
{
	//
	// ������ȭ�� �ܰ�, ���̾� ������, �ĸ� ����
	//
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&wireframeDesc, &WireframeRS));

	//
	// �ָ���, �ĸ� ���� ����, ���� �׸� �� ������ �κ��� �ֱ⿡ �̷��� ó��
	//
	D3D11_RASTERIZER_DESC noCullDesc;
	ZeroMemory(&noCullDesc, sizeof(D3D11_RASTERIZER_DESC));
	noCullDesc.FillMode = D3D11_FILL_SOLID;
	noCullDesc.CullMode = D3D11_CULL_NONE;
	noCullDesc.FrontCounterClockwise = false;
	noCullDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&noCullDesc, &NoCullRS));

	//
	// AlphaToCoverageBS
	//

	D3D11_BLEND_DESC alphaToCoverageDesc = { 0 };
	alphaToCoverageDesc.AlphaToCoverageEnable = true; // �Ĺ��̳� â�칮 �ؽ�ó �������� ������ ����ǥ��ȭ ����� ų�� ���� ����
	alphaToCoverageDesc.IndependentBlendEnable = false; // �� ���� ��󸶴� �������� ȥ���� ������ ������ �ƴ��� ����

	// RenderTarget�� ���� 8��¥�� �迭�ε� �ε����� ���� ��� ������ �ǹ��Ѵ�.
	// ���� IndependentBlendEnable�� �״ٸ� �� RenderTarget�ε����� ���� �ɼ��� �ٸ��� ����ǰ�����
	// ���� 0��° �ε��� ������ ������ ���� ��󿡰� ��� ����ȴ�.
	alphaToCoverageDesc.RenderTarget[0].BlendEnable = false; // ȥ���� Ȱ��ȭ�Ϸ��� true �ƴϸ� false
	// ���� ��� ����ũ ���� ����
	// ����ũ�� ������ �����Ͽ� RGB ä�ο� ����� �ݿ����� �ʰ� �ϸ缭 ���İ����� �ݿ��� �ǰ� �Ѵٴ���
	// �� �ݴ븦 ���� �Ѵٴ���, �ذ� ���� ���������� �Ѵٴ����� �����ϴ�.
	alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	// ������ �ɼǵ�
	// SrcBlend = RGB ���� ȥ���� ���� ȥ�� ��� F(src)�� �ǹ��Ѵ�.
	// DestBlend = RGB ���� ȥ���� ��� ȥ�� ��� F(dst)�� �ǹ��Ѵ�.
	// BlendOp = RGB ���� ȥ�� �����ڸ� �ǹ��Ѵ�.
	// SrcBlendAlpha = ���� ���� ȥ���� ���� ȥ�� ��� F(src)�� �ǹ��Ѵ�.
	// DestBlendAlpha = ���� ���� ȥ���� ��� ȥ�� ��� F(dst)�� �ǹ��Ѵ�.
	// BlendOpAlpha = ���� ���� ȥ���� ȥ�� �����ڸ� �ǹ��Ѵ�.

	// ȥ�� ���� ���� �Լ�
	// ������ ȥ�� ���¸� �����ϴ� ����ü, ������ ȥ�� ���� �������̽��� ���� ������
	// ���� ���� ���ڵ�� ������ �ȴ�.
	HR(device->CreateBlendState(&alphaToCoverageDesc, &AlphaToCoverageBS));

	//
	// ���� ȥ�� ����
	// �Ϲ����� ȥ�� �� --> F(src) * C(src) + F(dst) * C(dst)
	// ���̿� ���� �����ڵ��� ���� �ɼ��� ���� �ٲ��� �� �ִ�.
	//

	D3D11_BLEND_DESC transparentDesc = { 0 };
	transparentDesc.AlphaToCoverageEnable = false;
	transparentDesc.IndependentBlendEnable = false;

	transparentDesc.RenderTarget[0].BlendEnable = true;
	transparentDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // ���� �ȼ��� ������
	transparentDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // ���� �ȼ��� ����, �� ���� ��ŭ �ڿ� �������� ��� �ȼ��� ���� ǥ����
	transparentDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	transparentDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; // D3D11_BLEND_ZERO, D3D11_BLEND_ONE
	transparentDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(device->CreateBlendState(&transparentDesc, &TransparentBS));

	D3D11_DEPTH_STENCIL_DESC IncDesc;
	IncDesc.DepthEnable = true;
	IncDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	IncDesc.DepthFunc = D3D11_COMPARISON_LESS;
	IncDesc.StencilEnable = true;
	IncDesc.StencilReadMask = 0xff;
	IncDesc.StencilWriteMask = 0xff;

	IncDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	IncDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	IncDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR; // ���ٽǰ� ���� ���� ������ ��� ���������� ���ٽ� ���� ������Ŵ
	IncDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS; // 0���� ������ �ȼ��� �׸��� ������ ȥ��

	// �ĸ� �ﰢ���� ���� ���ٽ� ���� ���� ��� ����, ���� �ĸ� ���Ÿ� �ϱ⿡ �Ⱦ��δ�.
	IncDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	IncDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	IncDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	IncDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	HR(device->CreateDepthStencilState(&IncDesc, &IncStencilDSS));

	D3D11_DEPTH_STENCIL_DESC distDesc;
	distDesc.DepthEnable = true;
	distDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	distDesc.DepthFunc = D3D11_COMPARISON_LESS;
	distDesc.StencilEnable = true;
	distDesc.StencilReadMask = 0xff;
	distDesc.StencilWriteMask = 0xff;

	distDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	distDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	distDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP; // ���ٽǰ� ���� ���� ������ ��� ���������� ���ٽ� ���� ������Ŵ
	distDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL; // 0���� ������ �ȼ��� �׸��� ������ ȥ��

	// �ĸ� �ﰢ���� ���� ���ٽ� ���� ���� ��� ����, ���� �ĸ� ���Ÿ� �ϱ⿡ �Ⱦ��δ�.
	distDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	distDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	distDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	distDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	HR(device->CreateDepthStencilState(&distDesc, &DistStencilDSS));
}

void RenderStates::DestroyAll()
{
	ReleaseCOM(WireframeRS);
	ReleaseCOM(NoCullRS);
	ReleaseCOM(AlphaToCoverageBS);
	ReleaseCOM(TransparentBS);
	ReleaseCOM(IncStencilDSS);
	ReleaseCOM(DistStencilDSS);
}