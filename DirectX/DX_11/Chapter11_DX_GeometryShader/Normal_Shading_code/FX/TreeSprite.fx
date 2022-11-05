//***************************************************************************************
// TreeSprite.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// ���� ���̴��� �̿��ؼ� �� ��������Ʈ�� y�࿡ ���ĵ� ī�޶� ���� ������ �簢������ Ȯ���Ѵ�.
//***************************************************************************************

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
	float4x4 gViewProj;
	Material gMaterial;
};

cbuffer cbFixed
{
	//
	// �ؽ��İ� �簢�� ��ü�� ���������� �ϴ� �ؽ��� ��ǥ�� ���
	//

	float2 gTexC[4] = 
	{
		float2(0.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 1.0f),
		float2(1.0f, 0.0f)
	};
};

// ���ġ ������ cbuffer�� �߰��� �� ����
Texture2DArray gTreeMapArray;

SamplerState samLinear
{
	Filter   = MIN_MAG_MIP_LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

struct VertexIn
{
	float3 PosW  : POSITION;
	float2 SizeW : SIZE;
};

struct VertexOut
{
	float3 CenterW : POSITION;
	float2 SizeW   : SIZE;
};

struct GeoOut
{
	float4 PosH    : SV_POSITION;
    float3 PosW    : POSITION;
    float3 NormalW : NORMAL;
    float2 Tex     : TEXCOORD;
	// �� fx�� �ѹ� ����ؼ� �ѹ��� �������� ��ü�� �׸��� �� ��ü �� ��ŭ
	// fx ������ �ڵ����� �ε����� ���� ��Ű�鼭 �ش� �׷�ġ�� ��ü�� ���� ID�� �����Ѵ�.
    uint   PrimID  : SV_PrimitiveID; // ���� ��ü ID
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	// ���� ���̴����� �� ���� �ʰ� �״�� ���� ���̴��� �ѱ��.
	vout.CenterW = vin.PosW;
	vout.SizeW   = vin.SizeW;

	return vout;
}
 
// �� ���� �簢��(���� 4��)���� Ȯ���ϹǷ�, ���� ���̴� ���� ��
// �ִ� ��� ���� ������ 4�̴�.
[maxvertexcount(4)]
void GS(point VertexOut gin[1], // ���� �ϳ� �ְ�
        uint primID : SV_PrimitiveID, 
        inout TriangleStream<GeoOut> triStream) // �ϼ��� ������ �ﰢ�� ��Ʈ���� ��´�.
{	
	//
	// �����尡 y�࿡ ���ĵǾ ������ ���ϵ��� �ϴ�,
	// ���� ������ �������� �� �������� ���� ��ǥ�踦 ����Ѵ�.
	//

	float3 up = float3(0.0f, 1.0f, 0.0f); // y �࿡ ����, ������ ���� �ڶ�ϱ�.
	float3 look = gEyePosW - gin[0].CenterW; // ���� �߽ɿ��� ī�޶���� ������ ���� ȹ��
	look.y = 0.0f; // y�� �����̶� ������ y�� 0���� �����Ѵ�.
	look = normalize(look); // ����ȭ�Ͽ� ũ�� 1�� �����.
	// ������ ���� ���� �߽ɿ��� �������� ���Ϳ� y�� ��ο��� ������ ������ ���͸� ȹ���Ѵ�.
	float3 right = cross(up, look);

	//
	// �簢���� �̷�� �ﰢ�� �� �������� ���� ���� ��ġ�� ����Ѵ�.
	//
	float halfWidth  = 0.5f*gin[0].SizeW.x; // �ʺ��� ����
	float halfHeight = 0.5f*gin[0].SizeW.y; // ������ ����
	
	/*
	v[3] ------- v[1]
	 |             |
	 |             |
	 |   CenterW   |
	 |             |
	 |             |
	v[2] ------- v[0]
	�̷��� �簢���� ī�޶� �������� �����ϰ� ����� ���̴�.
	*/
	float4 v[4]; 
	v[0] = float4(gin[0].CenterW + halfWidth*right - halfHeight*up, 1.0f);
	v[1] = float4(gin[0].CenterW + halfWidth*right + halfHeight*up, 1.0f);
	v[2] = float4(gin[0].CenterW - halfWidth*right - halfHeight*up, 1.0f);
	v[3] = float4(gin[0].CenterW - halfWidth*right + halfHeight*up, 1.0f);

	//
	// �簢�� �������� ���� ���� �������� ��ȯ�ϰ�
	// �ϳ��� �ﰢ���� ��μ� ����Ѵ�.
	//
	GeoOut gout;
	[unroll]
	for(int i = 0; i < 4; ++i)
	{
		gout.PosH     = mul(v[i], gViewProj); // ����, ���� ����� �����ش�.
		gout.PosW     = v[i].xyz;
		gout.NormalW  = look;
		gout.Tex      = gTexC[i];
		gout.PrimID   = primID;
		
		triStream.Append(gout);
	}
}

float4 PS(GeoOut pin, uniform int gLightCount, uniform bool gUseTexure, uniform bool gAlphaClip, uniform bool gFogEnabled) : SV_Target
{
	// Interpolating normal can unnormalize it, so normalize it.
    pin.NormalW = normalize(pin.NormalW);

	// The toEye vector is used in lighting.
	float3 toEye = gEyePosW - pin.PosW;

	// Cache the distance to the eye from this surface point.
	float distToEye = length(toEye);

	// Normalize.
	toEye /= distToEye;
   
    // Default to multiplicative identity.
    float4 texColor = float4(1, 1, 1, 1);
    if(gUseTexure)
	{
		// �ؽ��Ŀ��� ǥ�� ����
		float3 uvw = float3(pin.Tex, pin.PrimID%4); // PrimID���� ���� �ٸ� ���� �ؽ��� ���
		// uvw --> (�ؽ��� x, �ؽ��� y, �ؽ��� �迭 �ε��� ��ȣ)
		texColor = gTreeMapArray.Sample( samLinear, uvw );

		if(gAlphaClip)
		{
			// Discard pixel if texture alpha < 0.05.  Note that we do this
			// test as soon as possible so that we can potentially exit the shader 
			// early, thereby skipping the rest of the shader code.
			clip(texColor.a - 0.05f);
		}
	}

	//
	// Lighting.
	//

	float4 litColor = texColor;
	if( gLightCount > 0  )
	{
		// Start with a sum of zero.
		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

		// Sum the light contribution from each light source.  
		[unroll]
		for(int i = 0; i < gLightCount; ++i)
		{
			float4 A, D, S;
			ComputeDirectionalLight(gMaterial, gDirLights[i], pin.NormalW, toEye, 
				A, D, S);

			ambient += A;
			diffuse += D;
			spec    += S;
		}

		// Modulate with late add.
		litColor = texColor*(ambient + diffuse) + spec;
	}

	//
	// Fogging
	//

	if( gFogEnabled )
	{
		float fogLerp = saturate( (distToEye - gFogStart) / gFogRange ); 

		// Blend the fog color and the lit color.
		litColor = lerp(litColor, gFogColor, fogLerp);
	}

	// Common to take alpha from diffuse material and texture.
	litColor.a = gMaterial.Diffuse.a * texColor.a;

    return litColor;
}

//---------------------------------------------------------------------------------------
// Techniques-- �ʿ��ϸ� �˾Ƽ� �˸°� �߰�, �����ϸ� �ȴ�.
//---------------------------------------------------------------------------------------
technique11 Light3
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader( gs_5_0, GS() ) );
        SetPixelShader( CompileShader( ps_5_0, PS(3, false, false, false) ) );
    }
}

technique11 Light3TexAlphaClip
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader( gs_5_0, GS() ) );
        SetPixelShader( CompileShader( ps_5_0, PS(3, true, true, false) ) );
    }
}
            
technique11 Light3TexAlphaClipFog
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader( gs_5_0, GS() ) );
        SetPixelShader( CompileShader( ps_5_0, PS(3, true, true, true) ) );
    }
}
