//=============================================================================
// Sky.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// �ϴ� �� �������� ���̴� ȿ��
//=============================================================================

// �ڿ������� �ϴ��� ǥ���ϱ� ���� ��ȸ ���͸� Ÿ������ �����Ѵ�.
// �� �÷��̾ �ϴÿ� ������ �� ���� �ϴ��� �Թ�ü �� �߽���
// ī�޶�(����)�� �߽����� �����Ѵ�.
// �׷��� �÷��̾ ��� ��� ���� �ϴ��� ��� �÷��̾�� ���� �Ÿ��� ������ ���̴�.

cbuffer cbPerFrame
{
	float4x4 gWorldViewProj;
};
 
// �Թ�ü �� ������ TextureCube
TextureCube gCubeMap;

SamplerState samTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VertexIn
{
	float3 PosL : POSITION;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
    float3 PosL : POSITION;
};
 
VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// z / w = 1 �� �ǵ��� (�ϴ� ���� �׻� �� ��鿡 �ֵ���) z = w�� �����Ѵ�.
	// z�� �ٰ��� ��Ÿ���� ô����. �̰� ���� ���̳ĸ�
	// gWorldViewProj �� ����� ���ƴٴ� ���� �߽���ǥ�� ������ ī�޶� ��ǥ�� ������ �ǰ�
	// �� �Ŀ� ������ ���� w�� ������ �Ǹ� ����ȭ�� NDC ��ǥ��� ��ȭ�ȴ�.
	// ī�޶� ��ǥ��� z ���� �ٶ󺸴� ������ �������� ��ԵǴ� ���ε�(ȭ���� �߽�, CrossHair)
	// z �� ���� �־��� ���� �ش� ��ü�� �ְ� ���� ���̴�.
	// �ٵ� ����ȭ�� NDC ��ǥ�迡�� z�� ������ 0 ~ 1 �̰� ���� �� ��찡 1�̴�.
	// ���� ����ȭ�� NDC ��ǥ�迡�� �ϴ� �ؽ��ĸ� ���� �� ���� ������ �� ������
	// �÷��̾�� �ֱ� ���� z�� w�� ������ NDC ��ǥ�迡�� �������� z���� 1�� ������ִ� ���̴�. 
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj).xyww;
	
	// ���� ���� ��ġ�� �Թ�ü �� ��ȸ ���ͷ� ����Ѵ�.
	// �ϴ� ���� ���� �߿� �ϳ��� ��� ���� ������ �������� �� ���������� ���Ͱ�
	// ����Ű�� ���� �ؽ��İ� ��µ� ���̴�.
	vout.PosL = vin.PosL;
	
	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	return gCubeMap.Sample(samTriLinearSam, pin.PosL);
}

RasterizerState NoCull
{
    CullMode = None;
};

DepthStencilState LessEqualDSS
{
	// ���� �Լ��� �׳� < �� �ƴ϶� <=�� �ؾ� �Ѵ�. 
	// �׷��� ���� ������, ���� ���۸� 1�� �����ٰ� �� �� z = 1 (NDC)��
	// ����ȭ�� ���� ������ ���� ������ �����Ѵ�.
    DepthFunc = LESS_EQUAL;
};

technique11 SkyTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
        
        SetRasterizerState(NoCull);
        SetDepthStencilState(LessEqualDSS, 0);
    }
}
