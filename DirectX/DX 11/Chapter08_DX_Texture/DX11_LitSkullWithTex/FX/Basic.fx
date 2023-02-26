//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "LightHelper.fx"

cbuffer cbPerFrame
{
	DirectionalLight gDirLights[3];
	float3 gEyePosW;

	float  gFogStart;
	float  gFogRange;
	float4 gFogColor;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gWorldViewProj;
	float4x4 gTexTransform; // �ؽ��� ����
	Material gMaterial;
};

// ���ġ ���� cbuffer�� �߰��� �� ����.
Texture2D gDiffuseMap;

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC; // ���� ���͸� �ɼ�
	MaxAnisotropy = 4;

	// ��ǥ �������
	// WRAP --> Ÿ�ϸ�, 0 ~ 1�̿��� �ؽ��� ��ǥ�� Ÿ�ϸ� ����� ����
	// BORDER --> 0 ~ 1 �̿��� �ؽ��� ��ǥ�� ���α׷��Ӱ� ������ ���� ��ȯ
	// CLAMP --> ���� ���, 0 ~ 1 �̿��� �ؽ��� ��ǥ�� ���� ����� �ؽ��� ���� ������ ����
	// MIRROR --> �ſ� ���, Ÿ�ϸ��� ����ѵ� �ſ� ���� ������ ����
	AddressU = WRAP;
	AddressV = WRAP;

	// BorderColor = float4(0.0f, 0.0f, 1.0f, 1.0f) // BORDER�� ��� ���� ����
};

struct VertexIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex     : TEXCOORD; // �ؽ��� ��ǥ
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
	float3 NormalW : NORMAL;
	float2 Tex     : TEXCOORD; // �ؽ��� ��ǥ
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	// ���� �������� ��ȯ
	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);

	// ���� ���� �������� ��ȯ
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	// ���� Ư������ ����Ѵ�. (���� �ﰢ�� ǥ���� ���� �����ȴ�.)
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

	return vout;
}

// gUseTexure�� ���� �ؽ��� ���� ���� �Ǵ�
float4 PS(VertexOut pin, uniform int gLightCount, uniform bool gUseTexure) : SV_Target
{
	// Interpolating normal can unnormalize it, so normalize it.
	pin.NormalW = normalize(pin.NormalW);

	// The toEye vector is used in lighting.
	float3 toEye = gEyePosW - pin.PosW;

	// Cache the distance to the eye from this surface point.
	float distToEye = length(toEye);

	// Normalize.
	toEye /= distToEye;

	// �⺻ �ؽ�ó ������ ������ �׵��
	float4 texColor = float4(1, 1, 1, 1);
	if (gUseTexure)
	{
		// �ؽ��Ŀ��� ǥ���� �����Ѵ�.
		texColor = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
	}

	//
	// Lighting.
	//

	float4 litColor = texColor;
	if (gLightCount > 0)
	{
		// Start with a sum of zero. 
		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

		// Sum the light contribution from each light source.  
		[unroll]
		for (int i = 0; i < gLightCount; ++i)
		{
			float4 A, D, S;
			ComputeDirectionalLight(gMaterial, gDirLights[i], pin.NormalW, toEye,
				A, D, S);

			ambient += A;
			diffuse += D;
			spec += S;
		}

		// ���� �� ����, �̷��� ����� �Ϲ���
		litColor = texColor * (ambient + diffuse) + spec;
	}

	// �Ϲ������� �л걤 ������ ���� ������ ���� ���� ������ ����Ѵ�.
	litColor.a = gMaterial.Diffuse.a * texColor.a;

	return litColor;
}

technique11 Light1
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(1, false)));
	}
}

technique11 Light2
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(2, false)));
	}
}

technique11 Light3
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(3, false)));
	}
}

technique11 Light0Tex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(0, true)));
	}
}

technique11 Light1Tex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(1, true)));
	}
}

technique11 Light2Tex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(2, true)));
	}
}

technique11 Light3Tex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(3, true)));
	}
}
