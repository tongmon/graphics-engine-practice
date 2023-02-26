//***************************************************************************************
// BlurFilter.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "BlurFilter.h"
#include "Effects.h"

BlurFilter::BlurFilter()
  : mBlurredOutputTexSRV(0), mBlurredOutputTexUAV(0)
{
}

BlurFilter::~BlurFilter()
{
	ReleaseCOM(mBlurredOutputTexSRV);
	ReleaseCOM(mBlurredOutputTexUAV);
}

ID3D11ShaderResourceView* BlurFilter::GetBlurredOutput()
{
	return mBlurredOutputTexSRV;
}

void BlurFilter::SetGaussianWeights(float sigma)
{
	float d = 2.0f*sigma*sigma;

	float weights[9];
	float sum = 0.0f;
	for(int i = 0; i < 8; ++i)
	{
		float x = (float)i;
		weights[i] = expf(-x*x/d);

		sum += weights[i];
	}

	// Divide by the sum so all the weights add up to 1.0.
	for(int i = 0; i < 8; ++i)
	{
		weights[i] /= sum;
	}

	Effects::BlurFX->SetWeights(weights);
}

void BlurFilter::SetWeights(const float weights[9])
{
	Effects::BlurFX->SetWeights(weights);
}

void BlurFilter::Init(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format)
{
	// Start fresh.
	ReleaseCOM(mBlurredOutputTexSRV);
	ReleaseCOM(mBlurredOutputTexUAV);

	mWidth = width;
	mHeight = height;
	mFormat = format;

	// UAV������ ���� �ؽ��� ������ ����� �� ����. ������ ����ϸ� �ذ� ���� ������ ��´�.
	// ERROR: ID3D11Device::CreateTexture2D: The format (0x4d, BC3_UNORM) 
	// cannot be bound as an UnorderedAccessView, or cast to a format that
	// could be bound as an UnorderedAccessView.  Therefore this format 
	// does not support D3D11_BIND_UNORDERED_ACCESS.

	D3D11_TEXTURE2D_DESC blurredTexDesc;
	blurredTexDesc.Width     = width;
	blurredTexDesc.Height    = height;
    blurredTexDesc.MipLevels = 1;
    blurredTexDesc.ArraySize = 1;
	blurredTexDesc.Format    = format;
	blurredTexDesc.SampleDesc.Count   = 1;
	blurredTexDesc.SampleDesc.Quality = 0;
    blurredTexDesc.Usage     = D3D11_USAGE_DEFAULT;
    blurredTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
    blurredTexDesc.CPUAccessFlags = 0;
    blurredTexDesc.MiscFlags      = 0;

	// mBlurredOutputTex �� SRV, UAV �뵵�� ���� �ǰ� �̵��� �ᱹ 
	// blurredTex �ڿ��� �ǵ帮���� �뵵�� �ٸ� ���̴�.
	// ��������� ���� �ڿ��� �ǵ帮�� �����Ͷ�� ���� �ȴ�.
	ID3D11Texture2D* blurredTex = 0;
	HR(device->CreateTexture2D(&blurredTexDesc, 0, &blurredTex));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = format; // ����, �Ǽ���, ������ 3�� �� �̷���...
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D; // 2���� �ؽ��� �ڿ����� ����
	srvDesc.Texture2D.MostDetailedMip = 0; // ���� ������ �Ӹ� ����
	srvDesc.Texture2D.MipLevels = 1; // ��� �Ӹ� ����
	HR(device->CreateShaderResourceView(blurredTex, &srvDesc, &mBlurredOutputTexSRV));

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = format;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = 0;
	HR(device->CreateUnorderedAccessView(blurredTex, &uavDesc, &mBlurredOutputTexUAV));

	// Views save a reference to the texture so we can release our reference.
	ReleaseCOM(blurredTex);
}

void BlurFilter::BlurInPlace(ID3D11DeviceContext* dc, 
							 ID3D11ShaderResourceView* inputSRV, 
	                         ID3D11UnorderedAccessView* inputUAV,
							 int blurCount)
{
	//
	// ��� ���̴��� �����ؼ� ȭ�� �� �ؽ��ĸ� �帰��.
	// 

	for(int i = 0; i < blurCount; ++i)
	{
		// ���� �帮�� �н�
		D3DX11_TECHNIQUE_DESC techDesc;
		Effects::BlurFX->HorzBlurTech->GetDesc( &techDesc );
		for(UINT p = 0; p < techDesc.Passes; ++p)
		{
			Effects::BlurFX->SetInputMap(inputSRV);
			Effects::BlurFX->SetOutputMap(mBlurredOutputTexUAV);
			Effects::BlurFX->HorzBlurTech->GetPassByIndex(p)->Apply(0, dc);

			// numGroupsX�� �̹����� �ȼ� �� �ϳ��� ó���ϴ� �� �ʿ��� ������ �׷��� �����̴�.
			// �� ������ �׷��� 256���� �ȼ��� ó���ϰ� �ȴ�.
			// 256���� ��� ���̴��� ���ǵǾ� �ִ�.
			UINT numGroupsX = (UINT)ceilf(mWidth / 256.0f);
			dc->Dispatch(numGroupsX, mHeight, 1); // ������ ��� (x, y, z)
		}
	
		// �ڿ��� ȿ������ ������ ���� �Է� �ؽ��ĸ� ��� ���̴����� �����.
		// CS(��� ���̴� �ܰ迡��)Set(�����Ѵ�.)ShaderResources(���̴� �ڿ���)
		// ���⼭�� nullSRV�� �����ߴ�.
		ID3D11ShaderResourceView* nullSRV[1] = { 0 };
		dc->CSSetShaderResources( 0, 1, nullSRV );

		// ��� �ؽ��ĵ� ��� ���̴����� �����.
		// �� ����� ���� �н��� �Է����� ����Ѵ�. A --> B
		// �ϳ��� �ڿ��� ���ÿ� �Է°� ��¿� ��� ����� ���� ����.
		ID3D11UnorderedAccessView* nullUAV[1] = { 0 };
		dc->CSSetUnorderedAccessViews( 0, 1, nullUAV, 0 );
	
		// ���� �帮�� �н�
		Effects::BlurFX->VertBlurTech->GetDesc( &techDesc );
		for(UINT p = 0; p < techDesc.Passes; ++p)
		{
			Effects::BlurFX->SetInputMap(mBlurredOutputTexSRV);
			Effects::BlurFX->SetOutputMap(inputUAV);
			Effects::BlurFX->VertBlurTech->GetPassByIndex(p)->Apply(0, dc);

			// numGroupsY�� �̹����� �ȼ� �� �ϳ��� ó���ϴ� �� �ʿ��� ������ �׷��� �����̴�.
			// �� ������ �׷��� 256���� �ȼ��� ó���ϰ� �ȴ�.
			// 256���� ��� ���̴��� ���ǵǾ� �ִ�.
			UINT numGroupsY = (UINT)ceilf(mHeight / 256.0f);
			dc->Dispatch(mWidth, numGroupsY, 1);
		}
	
		dc->CSSetShaderResources( 0, 1, nullSRV );
		dc->CSSetUnorderedAccessViews( 0, 1, nullUAV, 0 );
	}

	// ��� ���̴��� ��Ȱ��ȭ�Ѵ�.
	dc->CSSetShader(0, 0, 0);
}
