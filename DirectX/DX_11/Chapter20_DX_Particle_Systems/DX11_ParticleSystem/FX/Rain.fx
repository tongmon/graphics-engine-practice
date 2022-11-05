//=============================================================================
// Rain.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Rain particle system.  Particles are emitted directly in world space.
//=============================================================================


//***********************************************
// ���� ����
//***********************************************

cbuffer cbPerFrame
{
	float3 gEyePosW;
	
	// ���� ��ġ�� ������ �ٸ� ��츦 ���� ������.
	float3 gEmitPosW; // ���� ��ġ�� ī�޶� ��ġ
	float3 gEmitDirW;
	
	float gGameTime;
	float gTimeStep;
	float4x4 gViewProj; 
};

cbuffer cbFixed
{
	// ������ ������ ���� ��¥ ��� ���ӵ�.
	// �� �缱���� ������.
	float3 gAccelW = {-1.0f, -9.8f, 0.0f};
};
 
// ���� �ؽ�ó ������ ���� �ؽ�ó �迭
Texture2DArray gTexArray;

// ���̴����� ������ ��µ� ���̴� ���� �ؽ���
Texture1D gRandomTex;
 
SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

DepthStencilState DisableDepth
{
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;
};

DepthStencilState NoDepthWrites
{
    DepthEnable = TRUE;
    DepthWriteMask = ZERO;
};


//***********************************************
// ���� �Լ�
//***********************************************
float3 RandUnitVec3(float offset)
{
	// ���� �ð� ���ϱ� �������� ������ �ؽ�ó ������ ���� �ؽ�ó ��ǥ�� ����Ѵ�.
	float u = (gGameTime + offset);
	
	// ���е��� ������ [-1,1]
    float3 v = gRandomTex.SampleLevel(samLinear, u, 0).xyz; // gRandomTex ��� �Ӹ��� �� �ϳ��� ���� 0�� ����
	
	// ���� ���� ����
	return normalize(v);
}

float3 RandVec3(float offset)
{
	// Use game time plus offset to sample random texture.
	float u = (gGameTime + offset);
	
	// coordinates in [-1,1]
	float3 v = gRandomTex.SampleLevel(samLinear, u, 0).xyz;
	
	return v;
}
 
//***********************************************
// ��Ʈ�� ���
//***********************************************

#define PT_EMITTER 0
#define PT_FLARE 1
 
struct Particle
{
	float3 InitialPosW : POSITION;
	float3 InitialVelW : VELOCITY;
	float2 SizeW       : SIZE;
	float Age          : AGE;
	uint Type          : TYPE;
};
  
Particle StreamOutVS(Particle vin)
{
	return vin;
}

// ��Ʈ�� ��� ���� ���� ���̴��� �� ������ ����� ���� ������
// �ı��� ����Ѵ�. ���� �ý��۸��� ������ ����, �ı� ��Ģ�� �ٸ� ���̹Ƿ�
// �� �κ��� ���� ���� ���� �ý��۸��� �ٸ� �ʿ䰡 �ִ�.
[maxvertexcount(6)]
void StreamOutGS(point Particle gin[1], 
                 inout PointStream<Particle> ptStream)
{	
	gin[0].Age += gTimeStep;

	if( gin[0].Type == PT_EMITTER )
	{	
		// �� ���ڸ� ������ �ð��� �Ǿ��°�?
		if( gin[0].Age > 0.002f )
		{
			for(int i = 0; i < 5; ++i) // ����� 5�� ����
			{
				// ī�޶� ���ʿ� �������� ������Ų��.
                float3 vRandom = 35.0f * RandVec3((float) i / 5.0f);
				vRandom.y = 20.0f; // ī�޶� ��ġ���� ���� �׻� 20 ��
			
				Particle p;
                p.InitialPosW = gEmitPosW.xyz + vRandom; // gEmitPosW�� ��ǻ� ī�޶� ��ġ�� Main.cpp���� �����ȴ�.
				p.InitialVelW = float3(0.0f, 0.0f, 0.0f); // ������� ó�� �ӵ��� 0
				p.SizeW       = float2(1.0f, 1.0f); // ����� ũ��� 1
				p.Age         = 0.0f;
				p.Type        = PT_FLARE;
			
				ptStream.Append(p);
			}
			
			// ���� �ð� �ʱ�ȭ
			gin[0].Age = 0.0f;
		}

		// ���� ���� �ٽ� ����
		ptStream.Append(gin[0]);
	}
	else
	{
		// �ð��� 3�� ���� ����� ����
		if( gin[0].Age <= 3.0f )
			ptStream.Append(gin[0]);
	}
}

// ��Ʈ�� ���
GeometryShader gsStreamOut = ConstructGSWithSO( 
	CompileShader( gs_5_0, StreamOutGS() ), 
	"POSITION.xyz; VELOCITY.xyz; SIZE.xy; AGE.x; TYPE.x" );
	
technique11 StreamOutTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, StreamOutVS() ) );
        SetGeometryShader( gsStreamOut );
        
        // ��Ʈ�� ��� ������ ���� �ȼ� ���̴��� ��Ȱ��ȭ�Ѵ�.
        SetPixelShader(NULL);
        
        // ��Ʈ�� ��� ������ ���ؼ��� ���� ���۵� ��Ȱ��ȭ�ؾ� �Ѵ�.
        SetDepthStencilState( DisableDepth, 0 );
    }
}

//***********************************************
// ������ ���
//***********************************************

struct VertexOut
{
	float3 PosW  : POSITION;
	uint   Type  : TYPE;
};

VertexOut DrawVS(Particle vin)
{
	VertexOut vout;
	
	float t = vin.Age;
	
	// �ð��� ���� ��ӵ� � �Ÿ� ����
    vout.PosW = 0.5f * t * t * gAccelW + t * vin.InitialVelW + vin.InitialPosW;
	
	vout.Type  = vin.Type;
	
	return vout;
}

struct GeoOut
{
	float4 PosH  : SV_Position;
	float2 Tex   : TEXCOORD;
};

// ������� �׳� ���� ������ Ȯ��
[maxvertexcount(2)]
void DrawGS(point VertexOut gin[1], 
            inout LineStream<GeoOut> lineStream)
{
	// ������ �׸��� �ʰ� ����� �ƴ� ���� �׸���.
    if (gin[0].Type != PT_EMITTER)
    {
		// ������ ���ӵ� �������� �������� �Ѵ�.
        float3 p0 = gin[0].PosW;
        float3 p1 = gin[0].PosW + 0.07f * gAccelW;
		
        GeoOut v0;
        v0.PosH = mul(float4(p0, 1.0f), gViewProj);
        v0.Tex = float2(0.0f, 0.0f);
        lineStream.Append(v0);
		
        GeoOut v1;
        v1.PosH = mul(float4(p1, 1.0f), gViewProj);
        v1.Tex = float2(1.0f, 1.0f);
        lineStream.Append(v1);
    }
}

float4 DrawPS(GeoOut pin) : SV_TARGET
{
	return gTexArray.Sample(samLinear, float3(pin.Tex, 0));
}

technique11 DrawTech
{
    pass P0
    {
        SetVertexShader(   CompileShader( vs_5_0, DrawVS() ) );
        SetGeometryShader( CompileShader( gs_5_0, DrawGS() ) );
        SetPixelShader(    CompileShader( ps_5_0, DrawPS() ) );
        
        SetDepthStencilState( NoDepthWrites, 0 );
    }
}