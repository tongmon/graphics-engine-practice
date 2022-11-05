//***************************************************************************************
// LightHelper.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Structures and functions for lighting calculations.
//***************************************************************************************

struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float pad;
};

struct PointLight
{ 
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Att;
	float pad;
};

struct SpotLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Direction;
	float Spot;

	float3 Att;
	float pad;
};

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular; // w = SpecPower
	float4 Reflect;
};

//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a directional light.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputeDirectionalLight(Material mat, DirectionalLight L, 
                             float3 normal, float3 toEye,
					         out float4 ambient,
						     out float4 diffuse,
						     out float4 spec)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The light vector aims opposite the direction the light rays travel.
	float3 lightVec = -L.Direction;

	// Add ambient term.
	ambient = mat.Ambient * L.Ambient;	

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.
	
	float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if( diffuseFactor > 0.0f )
	{
		float3 v         = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
					
		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec    = specFactor * mat.Specular * L.Specular;
	}
}

//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a point light.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputePointLight(Material mat, PointLight L, float3 pos, float3 normal, float3 toEye,
				   out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	float3 lightVec = L.Position - pos;
		
	// The distance from surface to light.
	float d = length(lightVec);
	
	// Range test.
	if( d > L.Range )
		return;
		
	// Normalize the light vector.
	lightVec /= d; 
	
	// Ambient term.
	ambient = mat.Ambient * L.Ambient;	

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if( diffuseFactor > 0.0f )
	{
		float3 v         = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
					
		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec    = specFactor * mat.Specular * L.Specular;
	}

	// Attenuate
	float att = 1.0f / dot(L.Att, float3(1.0f, d, d*d));

	diffuse *= att;
	spec    *= att;
}

//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a spotlight.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputeSpotLight(Material mat, SpotLight L, float3 pos, float3 normal, float3 toEye,
				  out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	float3 lightVec = L.Position - pos;
		
	// The distance from surface to light.
	float d = length(lightVec);
	
	// Range test.
	if( d > L.Range )
		return;
		
	// Normalize the light vector.
	lightVec /= d; 
	
	// Ambient term.
	ambient = mat.Ambient * L.Ambient;	

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if( diffuseFactor > 0.0f )
	{
		float3 v         = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
					
		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec    = specFactor * mat.Specular * L.Specular;
	}
	
	// Scale by spotlight factor and attenuate.
	float spot = pow(max(dot(-lightVec, L.Direction), 0.0f), L.Spot);

	// Scale by spotlight factor and attenuate.
	float att = spot / dot(L.Att, float3(1.0f, d, d*d));

	ambient *= spot;
	diffuse *= att;
	spec    *= att;
}

//---------------------------------------------------------------------------------------
// ���� �� ǥ���� ���� �������� ��ȯ�Ѵ�.
// normalMapSample �ؽ��Ŀ����� ���� ��
// unitNormalW ������ ����
// tangentW ������ ������
// ���࿡ �ؽ��Ŀ� ȸ�� ��ȯ ����� ���ߴٸ�
// ���⼭ �߰������� �������� ��ȯ�ؾ� �Ѵ�. (�� �Լ��� ��ȯ ��� ���ڸ� �߰��ؾ��Ѵ�.)
// ����� ���� ���� �������� ���ǵǾ� �ִ� unitNormalW, tangentW ��׸�
// ���������� ����;��Ѵ�.
// ���� ���� --> �������� �ؿ� ���� TBN ������ ��ġ����̴�. (TBN ������ ��ġ ��� == TBN ������ �����)
// ����� �������� N, T ���Ϳ� �ؽ��� ȸ�� ��ȯ�� �����ϰ� �ٽ� ������
// TBN ���͸� ���ؼ� ������ --> ���� �������� �������´�.
// �׷��� ���� ȸ���� ����� N, T, B ���Ͱ� ���Ӱ� �������� ��װ� ���ο� TBN �����
// �����ϰ� �ȴ�.
//---------------------------------------------------------------------------------------
float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
	// �� ������ 0 ~ 1(�ؽ��� ��ǥ�� ����) ���� -1 ~ 1�� ��ȯ�Ѵ�.
	float3 normalT = 2.0f*normalMapSample - 1.0f;

	// �������� ������ �����Ѵ�.
	float3 N = unitNormalW;
    float3 T = normalize(tangentW - dot(tangentW, N) * N); // tangentW, unitNormalW ������ ���� 90���� ����� ���� ����
	float3 B = cross(N, T);

	// T(x), T(y), T(z)
	// B(x), B(y), B(z)
	// N(x), N(y), N(z)
	// TBN ���͵��� ���� ���̴� �ܰ迡�� �̹� ���� ����� ��� ������ ������
	// unitNormalW, tangentW�� �̿��Ͽ� �������⿡
	// TBN�� ������� ���ؿ����� 3���� �� �����̴�.
	// �� TBN�� ���� ���� ���� ��ǥ�� �������� ������ �Ǿ� �ִ� �����̴�.
	float3x3 TBN = float3x3(T, B, N);

	// ���������� ����������� ��ȯ�Ѵ�.
	// ���� TBN ����� ���� ���� ��ǥ�� �������� ������ �Ǿ� �ְ�
	// �ؽ��� ���� ��ǥ�� �� ��İ� �����ָ� ���� ��ǥ�谡
	// ���� ���� ��ǥ���� TBN ������ ������ �ȴ�.
	// ���� �� ���� �Ŀ� bumpedNormalW�� ���� ���� ���� ��ǥ��� ������ �� �����̴�.
	float3 bumpedNormalW = mul(normalT, TBN);

	return bumpedNormalW;
}