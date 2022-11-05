 
#include "LightHelper.fx"
 
cbuffer cbPerFrame
{
	DirectionalLight gDirLights[3];
	float3 gEyePosW;

	float  gFogStart;
	float  gFogRange;
	float4 gFogColor;
	
	// �Ÿ��� �ּ��̸� �׼����̼� ����� �ִ��̴�.
	// �Ÿ��� �ִ��̸� �׼����̼� ����� �ּ��̴�.
	float gMinDist;
	float gMaxDist;

	// 2�� ������ ������ �׼����̼� ����� ���� ����. �׼����̼� �����
	// ������ 2 ^ MinTess, 2 ^ MaxTess�����̰� MinTess�� ��� 0 �̻�
	// MaxTess�� ���ƾ� 6���Ͽ��� �Ѵ�.
	float gMinTess;
	float gMaxTess;
	
	// ����ȭ�� uv ���� [0, 1] ������ ���� ���� ������ ����.
	float gTexelCellSpaceU;
	float gTexelCellSpaceV;
	// ���� �������� ��ĭ�� ������ ����.
	float gWorldCellSpace;
	float2 gTexScale = 50.0f;
	
	// ���� �������� ����ü ����� ��� ��� 6�� ����
	float4 gWorldFrustumPlanes[6];
};

cbuffer cbPerObject
{
	// Terrain coordinate specified directly 
	// at center of world space.
	
	float4x4 gViewProj;
	Material gMaterial;
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2DArray gLayerMapArray;
Texture2D gBlendMap;
Texture2D gHeightMap;

SamplerState samLinear // ���� �ؽ��Ŀ� ���� ����
{
	Filter = MIN_MAG_MIP_LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};

SamplerState samHeightmap // ���̸ʿ� ���� �ؽ��� ����
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

struct VertexIn
{
	float3 PosL     : POSITION;
	float2 Tex      : TEXCOORD0;
	float2 BoundsY  : TEXCOORD1;
};

struct VertexOut
{
	float3 PosW     : POSITION;
	float2 Tex      : TEXCOORD0;
	float2 BoundsY  : TEXCOORD1;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// �� �������� �Էµ� ���� ��ġ �������� �̹� ���� ������ �ִ�.
	// �̹� 0,0�� �߽��̰� ����, ���� ũ�Ⱑ 2049x2049�̸� 0.5 �������� ���ڰ� �����ȴ�.
	// ���� ���� ��ȯ�� ���� �ʿ䰡 ����.
	vout.PosW = vin.PosL;

	// ��ġ ������(��������) �������� ������ ���̷� �����Ѵ�.
	// �̷��� �ϸ� ���� �������� ��ġ������ �Ÿ� ����� �� �� ��Ȯ������.
	vout.PosW.y = gHeightMap.SampleLevel( samHeightmap, vin.Tex, 0 ).r;

	// ���� Ư������ ���� �ܰ�� ����Ѵ�.
	vout.Tex      = vin.Tex;
	vout.BoundsY  = vin.BoundsY;
	
	return vout;
}
 
float CalcTessFactor(float3 p)
{
	float d = distance(p, gEyePosW);

	// ���⼭�� ���� ���νÿ� ����ߴ� �Լ��� �޸�
	// ���μ����� 2�� ����� ����Ǿ� �Ÿ��� ���� ���̰� �� �� �ߺ��δ�.
	// �״ϱ� 2�� s �¿��� s���� �����Ǹ鼭 TessFactor�� 2 ^ s�� �����ȴ�.
	
	float s = saturate( (d - gMinDist) / (gMaxDist - gMinDist) );
	
	return pow(2, (lerp(gMaxTess, gMinTess, s)) );
}

// ���� ��ü�� ����� ����(���� �ݰ���)�� �ִٸ� true�� �����ش�.
// ��� ������� ax + by + cz + w = 0 ���� (a, b, c)�� ����� ���� ���͸� �ǹ��Ѵ�.
// ����ü ������ ������ �׻� ���� ���θ� ����Ű�� �ִ�.
// ���� ����ü ������ �ո��� ������ �׻� �ٶ󺸰� �ִ�.
bool AabbBehindPlaneTest(float3 center, float3 extents, float4 plane)
{
    float3 n = abs(plane.xyz); // |n.x|, |n.y|, |n.z|
	
	// �������� �׻� ���
	// extents�� x,y,z �� ���������� �������̰� �̴� ������ �밢�������� �簢�� �������� �ȴ�.
	// �̸� ���� n�� �����ϴ� ������ �ذ� ����. (n�� ũ�� 1�̿��� ��������)
	// ��������� �� ������ ������
	// |x �������� n�� ������ ����| + |y �������� n�� ������ ����| + |z �������� n�� ������ ����|
	// �� AABB�� �������� ���� n�� ������ ���̸� ��Ÿ����.
	// ������ n ���͸� ���� �� �̹� �ٿ��� �������� �̹� ����� �� ����ȭ�� �� �ʿ䰡 ����.
	// OBB �浹���� ��������� �ٽ� ���ϸ�
	// �� �濡 r = dot(extents, plane.xyz)�� ���� �ʴ� ������ �ִ�.
	// ������ ���� ���ͷ� ������ ���� �� ���� �� ���̸� ����� ������(�밢�� ������) ���ͷ� r�� ������ �ϴµ�
	// �׳� �־��� �����̷� ���� ����� ���� ���Ϳ� ������Ű�� �̰� ���� �� ���̸� ������ ���� �� �ֱ⿡
	// dot(extents, abs(plane.xyz)) �� ����� �ϴ� ���̴�.
	float r = dot(extents, n);
	
	// ��� �������� ��ȣ �ִ� �Ÿ�.
	// ���� �� ������ �Ÿ��� ���ϴ� �����ε� �� ����� ������ ũ��� ������ �ʴ��ĸ�
	// �̹� �� �Լ��� ������ ������ ���� ���ʹ� ����ȭ�� �Ǿ ũ�Ⱑ 1�̱� �����̴�.
	// �Ÿ��� ������ ���� ���� �ְ� ������ ������ ����
	float s = dot( float4(center, 1.0f), plane );
	
	// ������ ������ ��� �������� �Ÿ� r�̻� ������ ������
	// (��� �����̹Ƿ� s�� ����) ���� ��ü�� ����� ���� �ݰ����� �ִ� ��
	return (s + r) < 0.0f;
}

// ���ڰ� ����ü �ٱ��̸� true�� �����ش�.
bool AabbOutsideFrustumTest(float3 center, float3 extents, float4 frustumPlanes[6])
{
	for(int i = 0; i < 6; ++i)
	{
		// ���� ��ü�� ����ü�� ��� �� ����� ���ʿ� �ִٸ�
		// ���ڴ� ����ü �ٱ��� �ִ� ���̴�.
		if( AabbBehindPlaneTest(center, extents, frustumPlanes[i]) )
		{
			return true;
		}
	}
	
	return false;
}

struct PatchTess
{
	float EdgeTess[4]   : SV_TessFactor;
	float InsideTess[2] : SV_InsideTessFactor;
};

PatchTess ConstantHS(InputPatch<VertexOut, 4> patch, uint patchID : SV_PrimitiveID)
{
	PatchTess pt;
	
	//
	// ����ü ����
	//
	
	// ��ġ�� BoundY�� ù ��° �������� ������ �ξ��� �̸� ������ ���.
	float minY = patch[0].BoundsY.x;
	float maxY = patch[0].BoundsY.y;
	
	// �� ���� �����ڸ� �����.
	/*
	0 --- 1
	|	  |
	|	  |
	2 --- 3
	1�� x,z�� 4�� ���� �߿� ���� ũ�� 2�� x,z�� ���� �۴�.
	*/
	float3 vMin = float3(patch[2].PosW.x, minY, patch[2].PosW.z);
	float3 vMax = float3(patch[1].PosW.x, maxY, patch[1].PosW.z);
	
	// AABB�� ����, AABB�� ������
	float3 boxCenter  = 0.5f*(vMin + vMax);
	float3 boxExtents = 0.5f*(vMax - vMin);
	if( AabbOutsideFrustumTest(boxCenter, boxExtents, gWorldFrustumPlanes) )
	{
		// ����ü�� ��� ��ġ�� �׼����̼� ����� 0���� ����� �����Ѵ�.
		pt.EdgeTess[0] = 0.0f;
		pt.EdgeTess[1] = 0.0f;
		pt.EdgeTess[2] = 0.0f;
		pt.EdgeTess[3] = 0.0f;
		
		pt.InsideTess[0] = 0.0f;
		pt.InsideTess[1] = 0.0f;
		
		return pt;
	}
	//
	// �Ÿ��� �����ؼ� �׼����̼� ����� ����Ѵ�.
	//
	else 
	{
		// �׼����̼� ������� ���� �Ӽ��� �ٰ��ؼ� ����ϴ� ���� �߿��ϴ�.
		// �׷��� ���� ��ġ�� �����ϴ� ���� �׼����̼� ����� �׷��� ��� ��ġ���� ����������.
		// �׷��� ������ �׼����̼� �� �޽ÿ� ƴ�� �����.
		
		// �� ���� ������ ��ġ ��ü�� ������ ����Ѵ�.
		float3 e0 = 0.5f*(patch[0].PosW + patch[2].PosW);
		float3 e1 = 0.5f*(patch[0].PosW + patch[1].PosW);
		float3 e2 = 0.5f*(patch[1].PosW + patch[3].PosW);
		float3 e3 = 0.5f*(patch[2].PosW + patch[3].PosW);
		float3  c = 0.25f*(patch[0].PosW + patch[1].PosW + patch[2].PosW + patch[3].PosW); // �簢�� �߽�
		
		// ī�޶�� �� ���� �Ÿ��� ���� �׼����̼� ��� ����
		pt.EdgeTess[0] = CalcTessFactor(e0);
		pt.EdgeTess[1] = CalcTessFactor(e1);
		pt.EdgeTess[2] = CalcTessFactor(e2);
		pt.EdgeTess[3] = CalcTessFactor(e3);
		// �簢�� �߽ɰ� ī�޶� �Ÿ��� ���� �׼����̼� ��� ����
		pt.InsideTess[0] = CalcTessFactor(c);
		pt.InsideTess[1] = pt.InsideTess[0];
	
		return pt;
	}
}

struct HullOut
{
	float3 PosW     : POSITION;
	float2 Tex      : TEXCOORD0;
};

[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HullOut HS(InputPatch<VertexOut, 4> p, 
           uint i : SV_OutputControlPointID,
           uint patchId : SV_PrimitiveID)
{
	HullOut hout;
	
	// Pass through shader.
	hout.PosW     = p[i].PosW;
	hout.Tex      = p[i].Tex;
	
	return hout;
}

struct DomainOut
{
	float4 PosH     : SV_POSITION;
    float3 PosW     : POSITION;
	float2 Tex      : TEXCOORD0;
	float2 TiledTex : TEXCOORD1;
};

// �׼����̼ǿ��� ���� ���̴��� �׼����̼� �� �������� ���� ���̴� ������ ����Ѵ�.
[domain("quad")]
DomainOut DS(PatchTess patchTess, 
             float2 uv : SV_DomainLocation, 
             const OutputPatch<HullOut, 4> quad)
{
	DomainOut dout;
	
	// �簢�� ��ġ�� �׼����̼� ����
	
	// ���ο� ���� ���� ���� ������ ������ ���� �����.
	dout.PosW = lerp(
		lerp(quad[0].PosW, quad[1].PosW, uv.x),
		lerp(quad[2].PosW, quad[3].PosW, uv.x),
		uv.y); 
	
	// �ؽ��� ��ǥ�� ������ ���� ��´�.
	dout.Tex = lerp(
		lerp(quad[0].Tex, quad[1].Tex, uv.x),
		lerp(quad[2].Tex, quad[3].Tex, uv.x),
		uv.y); 
		
	// ������ �ؽ��� �������� Ÿ�ϸ��Ѵ�.
    dout.TiledTex = dout.Tex * gTexScale;
	
	// ������ uv�� ���� ȹ��� Tex ��ǥ�� ������ ���� ����.
	dout.PosW.y = gHeightMap.SampleLevel( samHeightmap, dout.Tex, 0 ).r;
	
	// ���� ���̴����� �������й��� �̿��ؼ� ������ ����� ��������
	// fractional_even�� ��� �������� ���Ӿ��� �����̱� ������ ������
	// ��ȭ�� ���� ���� �����Ÿ��� ������ �ε巯����. �׷��� �ᱹ�� ����
	// ����� �ȼ� ���̴��� �Ű�ٰ� �Ѵ�.
	
	// ���� ���� �������� ����
	dout.PosH    = mul(float4(dout.PosW, 1.0f), gViewProj);
	
	return dout;
}

float4 PS(DomainOut pin, 
          uniform int gLightCount, 
		  uniform bool gFogEnabled) : SV_Target
{
	//
	// �߽����й��� �̿��ؼ� �����͵�� ���� ���͸� ��û�Ѵ�.
	// �ش� �������� x�� ����, z�� ���⸦ ���ϰ�
	// y�� ���� �̺� x --> f(x, y, z) => f(1, dy/dx, 0), 
	// y�� ���� �̺� z --> f(x, y, -z) => f(0, dy/dz, -1)�� �ؼ�
	// ���� ���� �������� ������ TBN�� ȹ���Ѵ�.
	// z�� �ؽ��� ��ǥ ���� �Ʒ������� �������̶� -z�� �´�.
	//
	
	// �ؽ��� x��ǥ �������� �� ���� ��, ���� ��ǥ ȹ��
	float2 leftTex   = pin.Tex + float2(-gTexelCellSpaceU, 0.0f);
	float2 rightTex  = pin.Tex + float2(gTexelCellSpaceU, 0.0f);
	
	// �ؽ��� z��ǥ �������� �� ���� ��, ���� ��ǥ ȹ��
	float2 bottomTex = pin.Tex + float2(0.0f, gTexelCellSpaceV);
	float2 topTex    = pin.Tex + float2(0.0f, -gTexelCellSpaceV);
	
	// ���� ���� ���� ȹ��
	float leftY   = gHeightMap.SampleLevel( samHeightmap, leftTex, 0 ).r;
	float rightY  = gHeightMap.SampleLevel( samHeightmap, rightTex, 0 ).r;
	float bottomY = gHeightMap.SampleLevel( samHeightmap, bottomTex, 0 ).r;
	float topY    = gHeightMap.SampleLevel( samHeightmap, topTex, 0 ).r;
	
	// ���� ���ؼ� ���� ��ǥ���� TBN ȹ��
	float3 tangent = normalize(float3(2.0f*gWorldCellSpace, rightY - leftY, 0.0f));
	// �ؽ��� ��ǥ�� ���� ��ǥ�� z�� ������ �ݴ�
	// ���� ��� ���̸� �ؽ��� ��ǥ���� (0,0.1)�� ���� ��ǥ���� (0, y��, 500)�� �����Ǹ�
	// (0,0.2) �� �༮�� ���� ��ǥ���� (0, y��, 250)�� ������ ���̴�.
	// �ֳĸ� �ؽ��� ��ǥ���� ������ ������ ���� �ؽ��� ��ǥ v�� Ŀ������ ������ǥ������ z ���� �۾�����.
	// ���⼭ topTex�� (0,0.1) �갡 �� ���̰� bottomTex�� (0,0.2) �갡 �� ���̴�.
	// �׷��� ���������� �ؽ��� �� v ������ ���ʹ� (0,0.2) - (0,0.1) �̷��� ���ϰ� �� ���̰�
	// �̸� ������ǥ�� �Űܼ� �����ϸ� (0, y��, 250) - (0, y��, 500) �̷���
	// y���� ��������� bottomY - topY �̰� �Ǿ��� z���� ������ �Ǵ� �ذ� ����.
	float3 bitan   = normalize(float3(0.0f, bottomY - topY, -2.0f*gWorldCellSpace));
	float3 normalW = cross(tangent, bitan);


	// The toEye vector is used in lighting.
	float3 toEye = gEyePosW - pin.PosW;

	// Cache the distance to the eye from this surface point.
	float distToEye = length(toEye);

	// Normalize.
	toEye /= distToEye;
	
	//
	// Texturing
	//
	
	// �ؽ��� �迭�� ����� �ؽ��� �����鿡�� ǥ������ �����Ѵ�.
	// �� �༮���� ���İ��� �ǹ̰� ����. 
	// ���뼺�� �ø��� ���� ȥ�� �� �ؽ��ĸ� ���� ����Ѵ�.
	float4 c0 = gLayerMapArray.Sample( samLinear, float3(pin.TiledTex, 0.0f) );
	float4 c1 = gLayerMapArray.Sample( samLinear, float3(pin.TiledTex, 1.0f) );
	float4 c2 = gLayerMapArray.Sample( samLinear, float3(pin.TiledTex, 2.0f) );
	float4 c3 = gLayerMapArray.Sample( samLinear, float3(pin.TiledTex, 3.0f) );
	float4 c4 = gLayerMapArray.Sample( samLinear, float3(pin.TiledTex, 4.0f) ); 
	
	// ȥ�� �ʿ��� ���� ������ �����Ѵ�.
	// �� r,g,b,a�� ���� �ٸ� ȸ���� ������ ����ִ�.
	// �� ȥ�ոʿ��� pin.Tex�� ����ϴ� ������ ���̸ʰ� ȥ�� ������ �����Ǳ� �����̴�.
	// ������ ������ ���� ���� ���� �����̶���� �̷� ���� ����µ� ȿ�����̴�.
	float4 t  = gBlendMap.Sample( samLinear, pin.Tex ); 
    
    // �������� ������ ȥ���Ѵ�.
	// ���� �ٸ� ������ ���� �ٸ� ������ ���� ���� �ؽ��ĸ� �̾Ƴ���.
    float4 texColor = c0;
    texColor = lerp(texColor, c1, t.r);
    texColor = lerp(texColor, c2, t.g);
    texColor = lerp(texColor, c3, t.b);
    texColor = lerp(texColor, c4, t.a);
 
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
			ComputeDirectionalLight(gMaterial, gDirLights[i], normalW, toEye, 
				A, D, S);

			ambient += A;
			diffuse += D;
			spec    += S;
		}

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

    return litColor;
}

technique11 Light1
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( CompileShader( hs_5_0, HS() ) );
        SetDomainShader( CompileShader( ds_5_0, DS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(1, false) ) );
    }
}

technique11 Light2
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( CompileShader( hs_5_0, HS() ) );
        SetDomainShader( CompileShader( ds_5_0, DS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(2, false) ) );
    }
}

technique11 Light3
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( CompileShader( hs_5_0, HS() ) );
        SetDomainShader( CompileShader( ds_5_0, DS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(3, false) ) );
    }
}

technique11 Light1Fog
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( CompileShader( hs_5_0, HS() ) );
        SetDomainShader( CompileShader( ds_5_0, DS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(1, true) ) );
    }
}

technique11 Light2Fog
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( CompileShader( hs_5_0, HS() ) );
        SetDomainShader( CompileShader( ds_5_0, DS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(2, true) ) );
    }
}

technique11 Light3Fog
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( CompileShader( hs_5_0, HS() ) );
        SetDomainShader( CompileShader( ds_5_0, DS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(3, true) ) );
    }
}
