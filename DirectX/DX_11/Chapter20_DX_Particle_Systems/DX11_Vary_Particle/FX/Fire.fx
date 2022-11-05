// ����Ⱑ �ٸ� ����⸦ �����ϴ� ȿ�� ������ �����ߴ�.
// �����غ��� ���ڵ��� ���� ������ ���� �п��ϸ鼭 �ö� ���̴�.
// ���⼭ �ָ� ���� �κ��� ������ �ٷ�� ��쿴�µ�
// ó���� ����⿡�� ���ڸ� �����ϸ� �������� �پ���
// ���� �ӵ��� ���� ���ڵ��� ������ ���̴�.
// �ٵ� �� ���ڵ��� �ٽ� ����� ������ �ϰ� �� ��
// �� �ʹݿ� �ٷ� ������ ��â�� �ι�° �������� ���� ���� ����� ��� ���� �ϸ�
// ������ ���� �������� �߻��ؼ� ������ �ӵ��� ��� �п��Ǵ� ������ �� �� �ִ�.
// �̷��� �����ϸ� ��������� �ʰ� �ǵ��� �ٰ� �ƴϴ�.
// �̸� �ذ��ϱ� ���� �� ���ڵ��� ���� �ٸ� �������� �����غô��� ���� �ӵ��� ���� �ٸ� ���¿���.
// ���� ���� �ӵ��� ���������� �߰��Ͽ� �� ���ڵ��� ���� �ٸ� ������ ������ �� �ְ� �ߴ�.
// �׸��� �� �Ǽ��� �κ��� �ִµ� [maxvertexcount(64)] ���Ͻ��̴����� �� �κ��� ���� ������ �� Ȯ������...
// ���� �����ÿ� ������ 5�� ���Ҵµ� 2���ۿ� ������ �ȵǾ ��� ã�ٰ� ���� �� ���� 2�� �Ǿ��ִ���...

//***********************************************
// ���� ������
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
    float3 gAccelW = { 0.0f, 7.8f, 0.0f };
    float N = 10;
    float M = 3;
	
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

float3 RandVec3(float offset)
{
	// Use game time plus offset to sample random texture.
    float u = (gGameTime + offset);
	
	// coordinates in [-1,1]
    float3 v = gRandomTex.SampleLevel(samLinear, u, 0).xyz;
	
    return v;
}
 
//***********************************************
// ��Ʈ�� ��� ���� ��� (������ X)
//***********************************************

#define PT_EMITTER 0 // ���� ����� 0
#define PT_EMITTER2 1 // ���ο� ����� 1
 
struct Particle
{
    float3 InitialPosW : POSITION; // ���� ���� ��ġ
    float3 InitialVelW : VELOCITY; // ���� ���� �ӵ�
    float2 SizeW : SIZE; // ���� ũ��
    float Age : AGE; // ������ ���� (���ڰ� �ð��� ���� �̵��ϱ⿡ �ʿ�)
    uint Type : TYPE; // ������ ��������� ������ �� �������� ����
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
[maxvertexcount(64)]
void StreamOutGS(point Particle gin[1],
                 inout PointStream<Particle> ptStream)
{
    gin[0].Age += gTimeStep; // ������ ���� �߰� (gTimeStep == deltaTime)
	
    if (gin[0].Type == PT_EMITTER)
    {
		// �� ���ڸ� ������ �ð��� �Ǿ��°�?
        if (gin[0].Age > 0.05f)
        {
            for (int i = 0; i < N; i++)
            {
                float3 vRandom = RandUnitVec3((float) i / N); // �ʹݿ� ó�� �����ϴ� ������ �׳� �̷��� �ص� �ȴ�.
                vRandom.x *= 2.0f;
                vRandom.z *= 2.0f;
                vRandom.y = abs(vRandom.y);
			
                Particle p;
                p.InitialPosW = gEmitPosW.xyz; // ����Ǵ� ���ڴ� ������� ��ġ
                p.InitialVelW = vRandom; // ������ �ӵ��� ������ ���� 4��
                p.SizeW = float2(2.0f, 2.0f); // ������ ũ�� ����
                p.Age = 0.0f; // ���� ���ڴ� �ʱ� ���̰� 0
                p.Type = PT_EMITTER2; // ����Ⱑ �ƴ� ���ڶ�� ���
			
                ptStream.Append(p); // ���ڸ� ������ ����
            }
        }
        else
        {
            ptStream.Append(gin[0]); // ���� ���������� ����
        }
    }
    else // PT_EMITTER2
    {
        // ���� �����ÿ� �ð��� ������ ���� �ʰ� ���� �ʹ� �ӵ����� ������ �޾� ���� ����
        // �� ������ ���ڸ� �����ϴ� �ñ⸦ ����
        float randTime = abs(RandVec3(gin[0].InitialVelW.y - gGameTime).x);
        if (gin[0].Age > min(10 * randTime, 2))
        {
            for (int i = 0; i < M; i++)
            {
                // ���⼭ �׳� RandUnitVec3((float) i / M); �̷��� �ع�����
                // ���� �ð��� ������ ���ڵ��� ��� ���� ���� �ӵ��� ������ ���� ���ڵ��� �����Ѵ�.
                // ���� �ش� ������ �ӵ�(���ڸ��� ��� �ٸ�)�� ������ �Ͽ� �߰����� ������ �ش�.
                float3 vRandom = RandUnitVec3((float) i / M + gin[0].InitialVelW.y);
                vRandom.x *= 0.5f;
                vRandom.z *= 0.5f;
                vRandom.y = -abs(vRandom.y);
			
                float t = gin[0].Age;
                Particle p;
                // ���� ����Ǵ� ������ ���� ��ġ�� ���� ����Ѵ�.
                p.InitialPosW = 0.5f * t * t * gAccelW + t * gin[0].InitialVelW + gin[0].InitialPosW; // ����⿡�� ����� �Ǵٸ� ����� ���� ��ġ
                p.InitialVelW = 4.0f * vRandom; // ������ �ӵ��� ������ ���� 4��
                p.SizeW = float2(2.0f, 2.0f); // ������ ũ�� ����
                p.Age = 0.0f; // ���� ���ڴ� �ʱ� ���̰� 0
                p.Type = PT_EMITTER2; // ������ �� �ٸ� ����� ����
			
                ptStream.Append(p); // ���ڸ� ������ ����
            }
        }
        else
        {
            ptStream.Append(gin[0]); // ���� ���������� ����
        }
    }
}

GeometryShader gsStreamOut = ConstructGSWithSO(
	CompileShader(gs_5_0, StreamOutGS()),
	"POSITION.xyz; VELOCITY.xyz; SIZE.xy; AGE.x; TYPE.x");
	
technique11 StreamOutTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, StreamOutVS()));
        SetGeometryShader(gsStreamOut);
        
        // �ȼ� ���̴� ��Ȱ��ȭ
        SetPixelShader(NULL);
        
        // ���� ���� ��Ȱ��ȭ
		// ������ ������ ���� ���̶� ���� ���۸� ���� ����� �Ǵ��� ������ ����.
		// ó�� ���� �����ϰ� CreateBuffer �ϴ� ��� ������ ���� ��ǥ�� ���ϰ� �ִ� ���� �����غ��� �ȴ�.
        SetDepthStencilState(DisableDepth, 0);
    }
}

//***********************************************
// ������ ���
//***********************************************

struct VertexOut
{
    float3 PosW : POSITION;
    float2 SizeW : SIZE;
    float4 Color : COLOR;
    uint Type : TYPE;
};

VertexOut DrawVS(Particle vin)
{
    VertexOut vout;
	
    float t = vin.Age;
	
	// �ð��� ���� �Ÿ�(��ġ) ��ӵ� ���
    vout.PosW = 0.5f * t * t * gAccelW + t * vin.InitialVelW + vin.InitialPosW;
	
    // ���� �������� ����, ���� �ð��� ���� ������ �ƴ� �ش� ������ �ӵ��� ���� ������ ȹ��
    float3 randCol = RandUnitVec3(vin.InitialVelW.y - gGameTime);
    randCol = abs(randCol);
    vout.Color = float4(randCol, 1.0);
	
    vout.SizeW = vin.SizeW;
    vout.Type = vin.Type;
	
    return vout;
}

struct GeoOut
{
    float4 PosH : SV_Position;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
};

// �������� ���� ���̴��� �׳� ���� ī�޶� ���� �簢������ Ȯ���Ѵ�.
[maxvertexcount(4)]
void DrawGS(point VertexOut gin[1],
            inout TriangleStream<GeoOut> triStream)
{
	// ����� ���ڴ� �׸��� �ʴ´�.
	//
	// �����尡 ī�޶� ���ϰ� �ϴ� ���� ����� ����Ѵ�.
	//
    float3 look = normalize(gEyePosW.xyz - gin[0].PosW); // ī�޶� ���� ������ �ݴ�
    float3 right = normalize(cross(float3(0, 1, 0), look)); // ���� ���� ��
    float3 up = cross(look, right); // �������� �� ����� ���Ѵ�.
		
	//
	// �簢���� �����ϴ� �ﰢ�� �� ��������
	// ����Ѵ�. (������� ���� ����)
	//
    float halfWidth = 0.5f * gin[0].SizeW.x;
    float halfHeight = 0.5f * gin[0].SizeW.y;
	
    float4 v[4];
    v[0] = float4(gin[0].PosW + halfWidth * right - halfHeight * up, 1.0f);
    v[1] = float4(gin[0].PosW + halfWidth * right + halfHeight * up, 1.0f);
    v[2] = float4(gin[0].PosW - halfWidth * right - halfHeight * up, 1.0f);
    v[3] = float4(gin[0].PosW - halfWidth * right + halfHeight * up, 1.0f);
		
	//
	// �� �������� ���� �������� ��ȯ�ϰ�, �ϳ��� �ﰢ�� ��μ�
	// ����Ѵ�.
	//
    GeoOut gout;
	[unroll]
    for (int i = 0; i < 4; ++i)
    {
        gout.PosH = mul(v[i], gViewProj);
        gout.Tex = gQuadTexC[i];
        gout.Color = gin[0].Color;
        triStream.Append(gout);
    }
}

float4 DrawPS(GeoOut pin) : SV_TARGET
{
    return gTexArray.Sample(samLinear, float3(pin.Tex, 0)) * pin.Color; // �ؽ��� �迭���� ù��° �ؽ���
}

technique11 DrawTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, DrawVS()));
        SetGeometryShader(CompileShader(gs_5_0, DrawGS()));
        SetPixelShader(CompileShader(ps_5_0, DrawPS()));
        
        SetBlendState(AdditiveBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff); // ���� ȥ�� ����
        SetDepthStencilState(NoDepthWrites, 0);
    }
}