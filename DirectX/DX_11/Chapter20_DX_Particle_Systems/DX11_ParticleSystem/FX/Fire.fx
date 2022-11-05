//=============================================================================
// Fire.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Fire particle system.  Particles are emitted directly in world space.
//=============================================================================


//***********************************************
// GLOBALS                                      *
//***********************************************

cbuffer cbPerFrame
{
	float3 gEyePosW;
	
	// ���� ��ġ�� ������ �ٸ� ��츦 ���� ������.
	float3 gEmitPosW;
	float3 gEmitDirW;
	
	float gGameTime;
	float gTimeStep;
	float4x4 gViewProj; 
};

cbuffer cbFixed
{
	// ������ ������ ���� ��¥ ��� ���ӵ�.
	// y������ ����
	float3 gAccelW = {0.0f, 7.8f, 0.0f};
	
	// �ؽ��ĸ� �簢�� ��ü�� ������ �ؽ��� ��ǥ��.
	// �� ���ڸ� �簢������ Ȯ���� �� ����. (������ ���� ���� ���� ����)
	float2 gQuadTexC[4] = 
	{
		float2(0.0f, 1.0f),
		float2(1.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 0.0f)
	};
};
 
// ���� �ؽ��� ������ ���� �ؽ��� �迭
Texture2DArray gTexArray;

// ���̴����� ������ ��� �� ���̴� ������ �ؽ���.
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

BlendState AdditiveBlending // ���� ȥ��
{
    AlphaToCoverageEnable = FALSE;
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = ONE;
    BlendOp = ADD;
    SrcBlendAlpha = ZERO;
    DestBlendAlpha = ZERO;
    BlendOpAlpha = ADD;
    RenderTargetWriteMask[0] = 0x0F;
};

//***********************************************
// ���� �Լ���                           
//***********************************************
float3 RandUnitVec3(float offset)
{
	// Use game time plus offset to sample random texture.
	float u = (gGameTime + offset);
	
	// coordinates in [-1,1]
	float3 v = gRandomTex.SampleLevel(samLinear, u, 0).xyz;
	
	// project onto unit sphere
	return normalize(v);
}
 
//***********************************************
// ��Ʈ�� ��� ���� ��� (������ X)
//***********************************************

#define PT_EMITTER 0 // ������ 0
#define PT_FLARE 1 // ���ڸ� 1
 
struct Particle
{
	float3 InitialPosW : POSITION; // ���� ���� ��ġ
	float3 InitialVelW : VELOCITY; // ���� ���� �ӵ�
	float2 SizeW       : SIZE; // ���� ũ��
	float Age          : AGE; // ������ ���� (���ڰ� �ð��� ���� �̵��ϱ⿡ �ʿ�)
	uint Type          : TYPE; // ������ ��������� ������ �� �������� ����
};

// ��Ʈ�� ��� �����̱⿡ ���� ���̴��� �� ���� ����.
// �ϴ� �־��� ������ ���� ���� ���̴����� ����� ���� �ϱ� �����̴�.
Particle StreamOutVS(Particle vin)
{
	return vin;
}

// ��Ʈ�� ��� ���� ���� ���̴��� �� ������ ����� ���� ������
// �ı��� ����Ѵ�. ���� �ý��۸��� ������ ����, �ı� ��Ģ�� �ٸ� ���̹Ƿ�
// �� �κ��� ���� ���� ���� �ý��۸��� �ٸ� �ʿ䰡 �ִ�.
[maxvertexcount(2)]
void StreamOutGS(point Particle gin[1], 
                 inout PointStream<Particle> ptStream)
{	
	gin[0].Age += gTimeStep; // ������ ���� �߰� (gTimeStep == deltaTime)
	
	if ( gin[0].Type == PT_EMITTER )
	{	
		// �� ���ڸ� ������ �ð��� �Ǿ��°�?
		if( gin[0].Age > 0.005f )
		{
			float3 vRandom = RandUnitVec3(0.0f);
			vRandom.x *= 0.5f;
			vRandom.z *= 0.5f;
			
			Particle p;
			p.InitialPosW = gEmitPosW.xyz; // ����Ǵ� ���ڴ� ������� ��ġ
            p.InitialVelW = 4.0f * vRandom; // ������ �ӵ��� ������ ���� 4��
			p.SizeW       = float2(3.0f, 3.0f); // ������ ũ�� ����
			p.Age         = 0.0f; // ���� ���ڴ� �ʱ� ���̰� 0
			p.Type        = PT_FLARE; // ����Ⱑ �ƴ� ���ڶ�� ���
			
			ptStream.Append(p); // ���ڸ� ������ ����
			
			// �����ϳ� ������ �ٽ� �ʱ�ȭ�ϰ� 0.005���� ���
			gin[0].Age = 0.0f;
		}
		
		// ����� ���ڴ� ����
		ptStream.Append(gin[0]);
	}
	else
	{
		// ���⼭ ���ڸ� ������ ���ǵ��� �����Ѵ�.
		// ��ü���� ������ ���� �ý��۸��� �ٸ� �� �ִ�.
		// �����ڴ� ���̰� 1�� �Ѿ�� �������.
		if( gin[0].Age <= 1.0f )
			ptStream.Append(gin[0]);
	}
}

GeometryShader gsStreamOut = ConstructGSWithSO( 
	CompileShader( gs_5_0, StreamOutGS() ), 
	"POSITION.xyz; VELOCITY.xyz; SIZE.xy; AGE.x; TYPE.x" );
	
technique11 StreamOutTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, StreamOutVS() ) );
        SetGeometryShader( gsStreamOut );
        
        // �ȼ� ���̴� ��Ȱ��ȭ
        SetPixelShader(NULL);
        
        // ���� ���� ��Ȱ��ȭ
		// ������ ������ ���� ���̶� ���� ���۸� ���� ����� �Ǵ��� ������ ����.
		// ó�� ���� �����ϰ� CreateBuffer �ϴ� ��� ������ ���� ��ǥ�� ���ϰ� �ִ� ���� �����غ��� �ȴ�.
        SetDepthStencilState( DisableDepth, 0 );
    }
}

//***********************************************
// ������ ���
//***********************************************

struct VertexOut
{
	float3 PosW  : POSITION;
	float2 SizeW : SIZE;
	float4 Color : COLOR;
	uint   Type  : TYPE;
};

VertexOut DrawVS(Particle vin)
{
	VertexOut vout;
	
	float t = vin.Age;
	
	// �ð��� ���� �Ÿ�(��ġ) ��ӵ� ���
    vout.PosW = 0.5f * t * t * gAccelW + t * vin.InitialVelW + vin.InitialPosW;
	
	// �ð��� ���� ���� ����.
	// smoothstep�� ���� �Լ��� http://egloos.zum.com/chulin28ho/v/5324893
	// ��ũ�� �׸��� �� ������ �Ǿ��ִ�. 
	// �뷫 �������ϰ� Ư�� �������� ���� ����ִ� �Լ���� ���� �ȴ�.
	// opacity ���� ���� �ð��� ���� �������Ƿ� �� ���ڴ� 
	// ���� ����ϸ鼭 ���� ��������� ȿ���� �����.
	float opacity = 1.0f - smoothstep(0.0f, 1.0f, t/1.0f);
	vout.Color = float4(1.0f, 1.0f, 1.0f, opacity);
	
	vout.SizeW = vin.SizeW;
	vout.Type  = vin.Type;
	
	return vout;
}

struct GeoOut
{
	float4 PosH  : SV_Position;
	float4 Color : COLOR;
	float2 Tex   : TEXCOORD;
};

// �������� ���� ���̴��� �׳� ���� ī�޶� ���� �簢������ Ȯ���Ѵ�.
[maxvertexcount(4)]
void DrawGS(point VertexOut gin[1], 
            inout TriangleStream<GeoOut> triStream)
{	
	// ����� ���ڴ� �׸��� �ʴ´�.
	if( gin[0].Type != PT_EMITTER )
	{
		//
		// �����尡 ī�޶� ���ϰ� �ϴ� ���� ����� ����Ѵ�.
		//
		float3 look  = normalize(gEyePosW.xyz - gin[0].PosW); // ī�޶� ���� ������ �ݴ�
		float3 right = normalize(cross(float3(0,1,0), look)); // ���� ���� ��
		float3 up    = cross(look, right); // �������� �� ����� ���Ѵ�.
		
		//
		// �簢���� �����ϴ� �ﰢ�� �� ��������
		// ����Ѵ�. (������� ���� ����)
		//
		float halfWidth  = 0.5f*gin[0].SizeW.x;
		float halfHeight = 0.5f*gin[0].SizeW.y;
	
		float4 v[4];
		v[0] = float4(gin[0].PosW + halfWidth*right - halfHeight*up, 1.0f);
		v[1] = float4(gin[0].PosW + halfWidth*right + halfHeight*up, 1.0f);
		v[2] = float4(gin[0].PosW - halfWidth*right - halfHeight*up, 1.0f);
		v[3] = float4(gin[0].PosW - halfWidth*right + halfHeight*up, 1.0f);
		
		//
		// �� �������� ���� �������� ��ȯ�ϰ�, �ϳ��� �ﰢ�� ��μ�
		// ����Ѵ�.
		//
		GeoOut gout;
		[unroll]
		for(int i = 0; i < 4; ++i)
		{
			gout.PosH  = mul(v[i], gViewProj);
			gout.Tex   = gQuadTexC[i];
			gout.Color = gin[0].Color;
			triStream.Append(gout);
		}
	}
}

float4 DrawPS(GeoOut pin) : SV_TARGET
{
	// ���� �� ���� pin.Color ���⿡ �ؽ��� ������ ������ ���̴�. 
	// �� ���󿡴� ���İ��� �����ϰ� �Ű��� ���� ���̰� �� ���İ��� ���� ȥ���� �Ǿ� ������
	// ���� ������ �ȴ�.
	return gTexArray.Sample(samLinear, float3(pin.Tex, 0)) * pin.Color; // �ؽ��� �迭���� ù��° �ؽ���
}

technique11 DrawTech
{
    pass P0
    {
        SetVertexShader(   CompileShader( vs_5_0, DrawVS() ) );
        SetGeometryShader( CompileShader( gs_5_0, DrawGS() ) );
        SetPixelShader(    CompileShader( ps_5_0, DrawPS() ) );
        
        SetBlendState(AdditiveBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff); // ���� ȥ�� ����
        SetDepthStencilState( NoDepthWrites, 0 );
    }
}