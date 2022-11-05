//=============================================================================
// SsaoBlur.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// SSAO �ʿ� ���� ����� �����ڸ� ���� �帮�⸦ ������.
// ��� ��带 ������ ���� ��ȯ���� �ʱ� ���� ��� ���̴� ��� �ȼ� ���̴��� ����Ѵ�.
// ��� ���̴� �������� �޸� ���� �޸𸮸� ����� �� �����Ƿ�, �ؽ��ĸ� ������ ĳ�÷� ����Ѵ�.
// SSAO ���� 16��Ʈ �ؽ��� ������ ����ϹǷ� ũ�Ⱑ �۴�.
// ���� ĳ�ÿ� ���� ���� �ؼ��� ���� �� �ִ�.
//=============================================================================

cbuffer cbPerFrame
{
	float gTexelWidth;
	float gTexelHeight;
};

cbuffer cbSettings
{
	float gWeights[11] = 
	{
		0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f
	};
};

cbuffer cbFixed
{
	static const int gBlurRadius = 5;
};
 
// Nonnumeric values cannot be added to a cbuffer.
Texture2D gNormalDepthMap; // �þ߿����� ����, ���� ��
Texture2D gInputImage; // AO ó�� �ϼ��� �ؽ���
 
SamplerState samNormalDepth
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

SamplerState samInputImage
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;

	AddressU  = CLAMP;
    AddressV  = CLAMP;
};

struct VertexIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex     : TEXCOORD;
};

struct VertexOut
{
    float4 PosH  : SV_POSITION;
	float2 Tex   : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// NDC ��ǥ�迡 �̹� ����, FullScreenQuad
	vout.PosH = float4(vin.PosL, 1.0f);

	// �ؽ��� �׳� �ѱ�
	vout.Tex = vin.Tex;
	
    return vout;
}


float4 PS(VertexOut pin, uniform bool gHorizontalBlur) : SV_Target
{
	// ����, ���� �帮�� ����
	float2 texOffset;
	if(gHorizontalBlur)
	{
		texOffset = float2(gTexelWidth, 0.0f);
	}
	else
	{
		texOffset = float2(0.0f, gTexelHeight);
	}

	// ���� �� �߾��� ǥ���� �׻� ���տ� �⿩�Ѵ�.
    float4 color = gWeights[5] * gInputImage.SampleLevel(samInputImage, pin.Tex, 0.0);
	float totalWeight = gWeights[5];
	 
	float4 centerNormalDepth = gNormalDepthMap.SampleLevel(samNormalDepth, pin.Tex, 0.0f);

	for(float i = -gBlurRadius; i <=gBlurRadius; ++i)
	{
		// �߾� ����ġ�� �̹� �ջ���
		if( i == 0 )
			continue;

		float2 tex = pin.Tex + i*texOffset;

		float4 neighborNormalDepth = gNormalDepthMap.SampleLevel(
			samNormalDepth, tex, 0.0f);

		//
		// �߾Ӱ��� �� �̿� ���� ���̰� �ʹ� ũ��(�����̵�, �����̵�)
		// ���� ���� �ҿ��Ӽ���(�� �����ڸ���) ���� �ִ� ������ �����ؼ�,
		// �ش� ǥ������ �帮�⿡�� ���ܽ�Ų��.
		//
	
		if( dot(neighborNormalDepth.xyz, centerNormalDepth.xyz) >= 0.8f &&
		    abs(neighborNormalDepth.a - centerNormalDepth.a) <= 0.2f )
		{
			float weight = gWeights[i+gBlurRadius];

			// �帱 �̿� �ȼ����� �߰��Ѵ�.
			color += weight*gInputImage.SampleLevel(
				samInputImage, tex, 0.0);
		
			totalWeight += weight;
		}
	}

	// ��꿡�� ���ܵ� ǥ���� ���� �� �����Ƿ�, ������ ����� ����ġ����
	// ������ ������ �ش�.
	return color / totalWeight;
}

technique11 HorzBlur
{
    pass P0
    {
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(true) ) );
    }
}

technique11 VertBlur
{
    pass P0
    {
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(false) ) );
    }
}
 