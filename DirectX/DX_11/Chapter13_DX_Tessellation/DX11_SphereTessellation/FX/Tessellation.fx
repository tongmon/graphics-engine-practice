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
	float4x4 gTexTransform;
	Material gMaterial;
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap;

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};

struct VertexIn
{
	float3 PosL    : POSITION;
};

struct VertexOut
{
	float3 PosL    : POSITION;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	vout.PosL = vin.PosL;

	return vout;
}
 
struct PatchTess
{
	float EdgeTess[3]   : SV_TessFactor;
	float InsideTess[1] : SV_InsideTessFactor;
};

PatchTess ConstantHS(InputPatch<VertexOut, 3> patch, uint patchID : SV_PrimitiveID)
{
	PatchTess pt;
	
	float3 centerW = mul(float4(0,0,0, 1.0f), gWorld).xyz; // ���� �߽��� ���� ��ǥ�� �̵��� ��
	
	float d = distance(centerW, gEyePosW); // ī�޶�� �Ÿ��� ����Ͽ� �׼����̼� ��� ����

	// �Ÿ��� d1���� ũ��(ī�޶󿡼� ���� �ָ�) �׼����̼� ����� 0���� �ϰ�
	// �Ÿ��� d0���� ������(ī�޶�� ���� �����ٸ�) �׼����̼� ����� 64�� �Ѵ�.
	// �� ���̴� d0, d1�� ������ ������ �����Ѵ�.
	
	const float d0 = 20.0f;
	const float d1 = 100.0f;
	float tess = 64.0f*saturate( (d1-d)/(d1-d0) );

    pt.EdgeTess[0] = tess;
    pt.EdgeTess[1] = tess;
    pt.EdgeTess[2] = tess;
	
	// ���� ���е�
    pt.InsideTess[0] = tess;
	
	return pt;
}

struct HullOut
{
	float3 PosL : POSITION;
};

// ������ ���� ���̴��� �״�� ��� ��Ű�� �����̴�.
[domain("tri")]
[partitioning("integer")] // fractional_even, fractional_odd, integer
[outputtopology("triangle_cw")]
// �Է� ��ġ�� ���� ������ ���� ���̴��� ���� Ƚ����� �� ���� �ִ�.
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HullOut HS(InputPatch<VertexOut, 3> p, 
           uint i : SV_OutputControlPointID, // ���� ����� �������� ����
           uint patchId : SV_PrimitiveID)
{
	HullOut hout;
	
	hout.PosL = p[i].PosL; // � ó���� ���ϰ� �׳� �����Ų��.
	
	return hout;
}

struct DomainOut
{
	float4 PosH : SV_POSITION;
};

// ���� ���̴��� �׼������Ͱ� ������ �� �������� ȣ��ȴ�. 
// �׼����̼� ������ ���� ���̴���� �� �� �ִ�.
[domain("tri")]
DomainOut DS(PatchTess patchTess, 
             float3 uvw : SV_DomainLocation, // �׼����̼� ���� ��ǥ
             const OutputPatch<HullOut, 3> quad)
{
	DomainOut dout;
	
    float3 p = uvw.x * quad[0].PosL + uvw.y * quad[1].PosL + uvw.z * quad[2].PosL;
    p = normalize(p);
	
	// Hill �������� ����ߴ� ����� �����ϴ� ������ �Լ�
	// �������� �������־�� �׼����̼� ���ε��� ���������� ������°� �� ���´�.
	
	dout.PosH = mul(float4(p, 1.0f), gWorldViewProj); // ���� ��µ� ��ǥ
	
	return dout;
}

float4 PS(DomainOut pin) : SV_Target
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f); // �׳� ȹ���� �ȼ� ��ǥ�� ������� ĥ�Ѵ�.
}

technique11 Tess
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( CompileShader( hs_5_0, HS() ) );
        SetDomainShader( CompileShader( ds_5_0, DS() ) );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}
