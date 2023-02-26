//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

// cbuffer --> ��� ���۸� �ǹ�
cbuffer cbPerObject
{
	// ���Ұ������� ���� ���� ��ǥ�� �Ű��ִ� S,R,T,�þ�,���� �̷��� ��ĺ�ȯ�� ��� ����Ǿ� ����Ǵ� ��� ���
	float4x4 gWorldViewProj;
};

struct VertexIn
{
	float3 Pos   : POSITION;
	float4 Color : COLOR;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
    float4 Color : COLOR;
};

// VS --> ���� ���̴�
VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// ���� ���� �������� ��ȯ
	// float4(vin.Pos, 1.0f) --> 4���� ���� ����, mul�� HSHL ���� ��� ���� ���ִ� �Լ�
	vout.PosH = mul(float4(vin.Pos, 1.0f), gWorldViewProj);
	
	// ���� ������ �״�� �ȼ� ���̴��� ����
    vout.Color = vin.Color;
    
    return vout;
}

// PS --> �ȼ� ���̴�, VS�� ���������� ó���� �Ǿ��ٸ� ������ �ȼ������� ���� ���̰� ������ ������ �����Ѵ�.
// SV_Target --> ��ȯ�� ������ ���� ��� ���İ� ��ġ�ؾ� �Ѵ�.
float4 PS(VertexOut pin) : SV_Target
{
    return pin.Color; // ������ ���� ��ȯ
}

technique11 ColorTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}

