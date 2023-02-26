//=============================================================================
// Ssao.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// SSAO ���� ����ϴ� fx.
//=============================================================================

cbuffer cbPerFrame
{
	float4x4 gViewToTexSpace; // Proj * Texture, ���� ��� ���ϱ� �ؽ��� ���
	float4   gOffsetVectors[14];
	float4   gFrustumCorners[4];

	// ���� ������ ���̴� �����.
	float    gOcclusionRadius    = 0.5f;
	float    gOcclusionFadeStart = 0.2f;
	float    gOcclusionFadeEnd   = 2.0f;
	float    gSurfaceEpsilon     = 0.05f;
};
 
// ���ġ ������ ��� ���ۿ� �߰��� �� ����.
Texture2D gNormalDepthMap; // xyz�� �������� �� �ȼ��� ����, ���İ�(w)�� ����
Texture2D gRandomVecMap; // ���� ���� �ؽ��� �迭
 
SamplerState samNormalDepth
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;

	// ���� ���� �� �ٱ��� ǥ���� �����ϴ� ��� �߸���
	// ���󵵰� ������ �ʵ��� ���� �� ���� ���� �����Ѵ�.
	AddressU = BORDER;
	AddressV = BORDER;
	BorderColor = float4(0.0f, 0.0f, 0.0f, 1e5f); // �� ������ ���İ��� ����
};

SamplerState samRandomVec
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

struct VertexIn
{
	float3 PosL            : POSITION;
	float3 ToFarPlaneIndex : NORMAL;
	float2 Tex             : TEXCOORD;
};

struct VertexOut
{
    float4 PosH       : SV_POSITION;
    float3 ToFarPlane : TEXCOORD0;
	float2 Tex        : TEXCOORD1;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// �̹� NDC ������ �ִ�. 
	// �ֳĸ� FullScreen ���� �����ϰ� ȭ�鿡 ���� ���� ������ ���� 4���� ������ ����
	vout.PosH = float4(vin.PosL, 1.0f);

	// ������ x ���п��� ����ü �� ��� �������� ������ ����Ǿ� �ִ�.
	// ToFarPlaneIndex �̸� ���� ȥ���� �� �� �ִµ� ���� Normal�� ���� ������ ������.
	vout.ToFarPlane = gFrustumCorners[vin.ToFarPlaneIndex.x].xyz;

	// �ؽ��� ��ǥ�� �״�� �ȼ� ���̴��� �Ѱ��ش�.
	vout.Tex = vin.Tex;
	
    return vout;
}

// ǥ���� q�� �� p�� �󸶳� ���������� distZ (���� ����)�� �Լ��μ� ����Ѵ�.
// distZ = | depth(p) - depth(q) |
float OcclusionFunction(float distZ)
{
	//
	// ���� depth(q) ��  depth(p) �ڿ� �ִٸ� q�� p�� ���� �� ����.
	// ���ʿ� AO ��� ��ü�� �ش� ������ ���� ���������� �ݿ��� �׸�������
	// ������ �������� ���� ���󵵸� ���ϴ� ���ε� �ݿ��� �Ѿ q(�״ϱ� �ڿ� �ִ� q)�� p�� ���� �� ����.
	// �� depth(q) �� depth(p) �� �ʹ� ������� q�� p�� ������ �ʴ� ������ �Ѵ�.
	// ���� ������ ����� ��鿡�� p, q���� �Ÿ��� ��û ����� ���� �ִµ� �̸� ���󵵿� �ݿ��ϴ� �� �� �̻����� �� �ִ�.
	// ���� Epsilon �����ٴ� p, q �Ÿ� ���̰� Ŀ���Ѵ�. (�� �׸�ŭ q�� p �տ� ��ġ�ؾ� ���󵵿� �ݿ��� �ȴ�.)
	//
	// ���� ��꿡�� ������ ���� �Լ��� ����Ѵ�.
	// 
	//
	//       1.0     -------------\
	//               |           |  \
	//               |           |    \
	//               |           |      \ 
	//               |           |        \
	//               |           |          \
	//               |           |            \
	//  ------|------|-----------|-------------|---------|--> zv
	//        0     Eps          z0            z1        
	//
	
	float occlusion = 0.0f;
	if(distZ > gSurfaceEpsilon)
	{
		float fadeLength = gOcclusionFadeEnd - gOcclusionFadeStart;
		
		// distZ�� gOcclusionFadeStart���� gOcclusionFadeEnd�� �����Կ�
		// ���� ���󵵸� 1���� 0���� ���� ���ҽ�Ų��.
		// gOcclusionFadeEnd�� �Ѿ������ �Ÿ����̰� ���� AO�� ���� ����� �ű�Ⱑ
		// �������ϴٰ� �Ǵ��ϴ� ��.
        occlusion = saturate((gOcclusionFadeEnd - distZ) / fadeLength);
    }
	
	return occlusion;	
}

float4 PS(VertexOut pin, uniform int gSampleCount) : SV_Target
{
	// p -- ���� �ֺ��� ���� ����ϰ��� �ϴ� �ȼ��� �ش��ϴ� ��
	// n -- p������ ���� ����
	// q -- p �ֺ��� �� ������ ��
	// q�� �ǹ̰� �߿��ѵ� q�� �׳� ���̴�. 
	// p���� ����� ������ �������� �ٸ� ��ü�� �΋H���� ���� ������ �ƴ϶� �׳� ���̴�.
	// SSAO������ �� ���� ���� AO ��ġ�� �����ؾ� �Ѵ�.
	// ������ AO ����� ������ �����ؼ� ��������� �ε� �ð��� ��ģ���� �������
	// SSAO�� ����ϴµ� SSAO�� AO �� ȹ���� �������� �������̶�� ���� �����̴�.
	// r -- p�� ���� ���ɼ��� �ִ� ������ ������.

	// �� �ȼ��� �þ� ���� ������ z ������ �����´�. ���� �������Ǵ�
	// ȭ�� ��ü �簢���� �ؽ��� ��ǥ���� �̹� uv ������ �ִ�.
	// normalDepth ���⿡ ����ִ� �༮���� World * View �ۿ� �� ������ �ִ�.
	// ���� z ������ 0 ~ 1 �� �ƴ϶� ���� �����̴�.
	// �ڵ忡���� 0.1 ~ 10000 �̷��� ®�� ���̴�.
	float4 normalDepth = gNormalDepthMap.SampleLevel(samNormalDepth, pin.Tex, 0.0f);
 
	float3 n = normalDepth.xyz; // �ش� �ȼ��� ���� ȹ��
	float pz = normalDepth.w; // ���� ȹ��

	//
	// ������ �þ� ���� ��ġ (x, y, z) �� �籸���Ѵ�.
	// �켱 p = t * pin.ToFarPlane �� �����ϴ� t�� ���Ѵ�.
	// p.z = t * pin.ToFarPlane.z
	// t = p.z / pin.ToFarPlane.z
	// ToFarPlane �� �༮�� ������ normalDepth ��� ����.
	//
    float3 p = (pz / pin.ToFarPlane.z) * pin.ToFarPlane;
	
	// ������ ���͸� �����ؼ� [0,1] --> [-1,1]�� ����Ѵ�.
    float3 randVec = 2.0f * gRandomVecMap.SampleLevel(samRandomVec, 4.0f * pin.Tex, 0.0f).rgb - 1.0f;

	float occlusionSum = 0.0f;
	
	// p �ֺ��� �̿� ǥ�������� n ������ �ݱ����� �����Ѵ�. AO �˰���
	[unroll]
	for(int i = 0; i < gSampleCount; ++i)
	{
		// �̸� ����� �� ��� ������ ���͵��� ���� �����Ǿ� �ִ�.
		// ������ ���͵��� ���� �������� ���� ���� �ʴ�
		// �̵��� �ϳ��� ������ ���͸� �������� �ݻ��Ű�� ���� ������ ������ ���͵���
		// ���������.
		float3 offset = reflect(gOffsetVectors[i].xyz, randVec);
	
		// ������ ���Ͱ� (p, n) ���� ���ǵ� ����� ������ ���ϰ� ������
		// ������ �ݴ�� �����´�.
		// �״ϱ� �ݱ����� �ƴ϶� �ݴ��� �ݱ� �����̸� �� ������ �ùٸ��� ������.
		// �а��̸� ���� ���� �������� �̿�
		float flip = sign( dot(offset, n) );
		
		// p �ֺ����� ���� ������ �̳��� ������ �� q�� �����Ѵ�. (������)
		float3 q = p + flip * gOcclusionRadius * offset;
		
		// q�� �����ؼ� ���� �ؽ��� ��ǥ�� ���Ѵ�.
		// q�� �̹� �þ� ��ǥ�迡�� ���ǵǾ� ������ ���� ��ĸ� �����ش�. (���� �ؽ��� ���� ��ȯ ��ĵ� ���Ѵ�.)
		float4 projQ = mul(float4(q, 1.0f), gViewToTexSpace);
		projQ /= projQ.w; // ���� ������

		// �������� q���� ���������� ������ ���� ����� �ȼ��� ���̸� ���Ѵ�.
		// �̰��� q�� ���̴� �ƴϰ� q�� �׳� p ��ó�� ������ ���̸�,
		// ����� ��ü�� �ƴ� �� ������ �ִ� ���� ���� �ִ�.
		// ���� ����� ���̴� ���� �ʿ��� �����Ѵ�.

		// projQ ���� xy ��ǥ�� �ؽ��� ��ȯ���� �Ϸ�� ���¶�
		// samNormalDepth ��� 1:1�� ��Ī�ȴ�.
		float rz = gNormalDepthMap.SampleLevel(samNormalDepth, projQ.xy, 0.0f).a;

		// ������ �þ� ���� ��ġ r = (rx, ry, rz)�� �籸���Ѵ�.
		// r�� q�� ������ �������� �����Ƿ�, r = t * q�� �����ϴ� t�� �����Ѵ�.
		// r.z = t*q.z ==> t = r.z / q.z
		// ������ ���� ��� ���̴�. (�ﰢ�� �� ���)

		float3 r = (rz / q.z) * q;
		
		//
		// r�� p�� �������� ����.
		//   * ���� dot(n, normalize(r - p)) �� ������ ������ r�� ���
		//     plane(p,n) �� ������ �󸶳� �տ� �ִ����� ��Ÿ����.
		//     �� �տ� ��������(��鿡 ����� ����) ���� ����ġ�� �� ũ�� ��´�.
		//     �̷��� �ϸ� r�� �ü��� ������ ��� (p, n)�� ���� �� ���� ������ ���� �� ���� ������
		//	   r�� p�� �����ٰ� �߸� �����ϴ� ��Ȳ�� �����ȴ�.
		//   * ���󵵴� ���� �� p�� ������ r ������ �Ÿ��� �����Ѵ�.
		//     r �� p���� �ʹ� �ָ� ������ p�� ������ �ʴ� ������ ���ֵȴ�.
		// 
		
		float distZ = p.z - r.z;
		float dp = max(dot(n, normalize(r - p)), 0.0f); // ����ġ�̰� ������ p->r ������ ���� ������ ���� ���� Ŀ����.
		float occlusion = dp * OcclusionFunction(distZ); // �Ÿ����Ǳ��� ���� ���� ���󵵸� ȹ��
		
		occlusionSum += occlusion; // �� ������ �������� ����� ���� �ջ�
	}
	
	occlusionSum /= gSampleCount; // ���� ����ŭ ����� ��� ����
	
	float access = 1.0f - occlusionSum; // ������ �ݴ�� ���� �޴� ��ŭ�� ���Ѵ�.

	// SSAO�� �ظ��� ȿ���� ������, ������ �ŵ������� ���ش�.
	// SSAO ���� ��� ������ ��ӵ� ���� Ŀ����.
	// �̷��� �ؼ� ���� �������� ��� �ȼ��� ���� ������ ����� �Ϸ�Ǿ���.
	return saturate(pow(access, 4.0f));
}

technique11 Ssao
{
    pass P0
    {
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(14) ) );
    }
}
 