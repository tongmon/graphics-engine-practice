//***************************************************************************************
// GeometryGenerator.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "GeometryGenerator.h"
#include "MathHelper.h"

void GeometryGenerator::CreateBox(float width, float height, float depth, MeshData& meshData)
{
	//
	// Create the vertices.
	//

	Vertex v[24];

	float w2 = 0.5f*width;
	float h2 = 0.5f*height;
	float d2 = 0.5f*depth;
    
	// �ո� ���� �ڷ� ä���
	// �ؽ��� ��ǥ ���� 0 ~ 1 �� ������ ��ġ
	v[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// �޸� ���� �ڷ� ä���
	v[4] = Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[5] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[6] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[7] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// ���� ���� �ڷ� ä���
	v[8]  = Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[9]  = Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[10] = Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[11] = Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// �ظ� ���� �ڷ� ä���
	v[12] = Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[13] = Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[14] = Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[15] = Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// �޸� ���� �ڷ� ä���
	v[16] = Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[17] = Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[18] = Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[19] = Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// ������ ���� �ڷ� ä���
	v[20] = Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[21] = Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[22] = Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[23] = Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	meshData.Vertices.assign(&v[0], &v[24]);
 
	//
	// Create the indices.
	//

	UINT i[36];

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7]  = 5; i[8]  = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] =  9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	meshData.Indices.assign(&i[0], &i[36]);
}

void GeometryGenerator::CreateSphere(float radius, UINT sliceCount, UINT stackCount, MeshData& meshData)
{
	meshData.Vertices.clear();
	meshData.Indices.clear();

	//
	// Compute the vertices stating at the top pole and moving down the stacks.
	//

	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.
	Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	meshData.Vertices.push_back( topVertex );

	float phiStep   = XM_PI/stackCount;
	float thetaStep = 2.0f*XM_PI/sliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for(UINT i = 1; i <= stackCount-1; ++i)
	{
		float phi = i*phiStep;

		// Vertices of ring.
		for(UINT j = 0; j <= sliceCount; ++j)
		{
			float theta = j*thetaStep;

			Vertex v;

			// spherical to cartesian
			v.Position.x = radius*sinf(phi)*cosf(theta);
			v.Position.y = radius*cosf(phi);
			v.Position.z = radius*sinf(phi)*sinf(theta);

			// Partial derivative of P with respect to theta
			v.TangentU.x = -radius*sinf(phi)*sinf(theta);
			v.TangentU.y = 0.0f;
			v.TangentU.z = +radius*sinf(phi)*cosf(theta);

			XMVECTOR T = XMLoadFloat3(&v.TangentU);
			XMStoreFloat3(&v.TangentU, XMVector3Normalize(T));

			XMVECTOR p = XMLoadFloat3(&v.Position);
			XMStoreFloat3(&v.Normal, XMVector3Normalize(p));

			v.TexC.x = theta / XM_2PI;
			v.TexC.y = phi / XM_PI;

			meshData.Vertices.push_back( v );
		}
	}

	meshData.Vertices.push_back( bottomVertex );

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	for(UINT i = 1; i <= sliceCount; ++i)
	{
		meshData.Indices.push_back(0);
		meshData.Indices.push_back(i+1);
		meshData.Indices.push_back(i);
	}
	
	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = 1;
	UINT ringVertexCount = sliceCount+1;
	for(UINT i = 0; i < stackCount-2; ++i)
	{
		for(UINT j = 0; j < sliceCount; ++j)
		{
			meshData.Indices.push_back(baseIndex + i*ringVertexCount + j);
			meshData.Indices.push_back(baseIndex + i*ringVertexCount + j+1);
			meshData.Indices.push_back(baseIndex + (i+1)*ringVertexCount + j);

			meshData.Indices.push_back(baseIndex + (i+1)*ringVertexCount + j);
			meshData.Indices.push_back(baseIndex + i*ringVertexCount + j+1);
			meshData.Indices.push_back(baseIndex + (i+1)*ringVertexCount + j+1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = (UINT)meshData.Vertices.size()-1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;
	
	for(UINT i = 0; i < sliceCount; ++i)
	{
		meshData.Indices.push_back(southPoleIndex);
		meshData.Indices.push_back(baseIndex+i);
		meshData.Indices.push_back(baseIndex+i+1);
	}
}
 
// �ﰢ���� �� ���� 4���� �ﰢ������ �ɰ��� �Լ�
void GeometryGenerator::Subdivide(MeshData& meshData)
{
	// meshData �����ؼ� ����
	MeshData inputCopy = meshData;


	meshData.Vertices.resize(0);
	meshData.Indices.resize(0);

	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2

	UINT numTris = inputCopy.Indices.size() / 3;
	for (UINT i = 0; i < numTris; ++i)
	{
		Vertex v0 = inputCopy.Vertices[inputCopy.Indices[i * 3 + 0]];
		Vertex v1 = inputCopy.Vertices[inputCopy.Indices[i * 3 + 1]];
		Vertex v2 = inputCopy.Vertices[inputCopy.Indices[i * 3 + 2]];

		//
		// �������� ��´�.
		//

		Vertex m0, m1, m2;

		// For subdivision, we just care about the position component.  We derive the other
		// vertex components in CreateGeosphere.

		m0.Position = XMFLOAT3(
			0.5f * (v0.Position.x + v1.Position.x),
			0.5f * (v0.Position.y + v1.Position.y),
			0.5f * (v0.Position.z + v1.Position.z));

		m1.Position = XMFLOAT3(
			0.5f * (v1.Position.x + v2.Position.x),
			0.5f * (v1.Position.y + v2.Position.y),
			0.5f * (v1.Position.z + v2.Position.z));

		m2.Position = XMFLOAT3(
			0.5f * (v0.Position.x + v2.Position.x),
			0.5f * (v0.Position.y + v2.Position.y),
			0.5f * (v0.Position.z + v2.Position.z));

		//
		// Add new geometry.
		//

		meshData.Vertices.push_back(v0); // 0
		meshData.Vertices.push_back(v1); // 1
		meshData.Vertices.push_back(v2); // 2
		meshData.Vertices.push_back(m0); // 3
		meshData.Vertices.push_back(m1); // 4
		meshData.Vertices.push_back(m2); // 5

		meshData.Indices.push_back(i * 6 + 0);
		meshData.Indices.push_back(i * 6 + 3);
		meshData.Indices.push_back(i * 6 + 5);

		meshData.Indices.push_back(i * 6 + 3);
		meshData.Indices.push_back(i * 6 + 4);
		meshData.Indices.push_back(i * 6 + 5);

		meshData.Indices.push_back(i * 6 + 5);
		meshData.Indices.push_back(i * 6 + 4);
		meshData.Indices.push_back(i * 6 + 2);

		meshData.Indices.push_back(i * 6 + 3);
		meshData.Indices.push_back(i * 6 + 1);
		meshData.Indices.push_back(i * 6 + 4);
	}
}

void GeometryGenerator::CreateGeosphere(float radius, UINT numSubdivisions, MeshData& meshData)
{
	// ���� Ƚ���� ������ �д�.
	numSubdivisions = MathHelper::Min(numSubdivisions, 5u);

	// ó���� �����̸�ü�� �����Ͽ� ��� ����ȭ�ذ��� ���� �ٻ��� ������.(�׼����̼�)

	const float X = 0.525731f;
	const float Z = 0.850651f;

	XMFLOAT3 pos[12] = // ���� �迭
	{
		XMFLOAT3(-X, 0.0f, Z),  XMFLOAT3(X, 0.0f, Z),
		XMFLOAT3(-X, 0.0f, -Z), XMFLOAT3(X, 0.0f, -Z),
		XMFLOAT3(0.0f, Z, X),   XMFLOAT3(0.0f, Z, -X),
		XMFLOAT3(0.0f, -Z, X),  XMFLOAT3(0.0f, -Z, -X),
		XMFLOAT3(Z, X, 0.0f),   XMFLOAT3(-Z, X, 0.0f),
		XMFLOAT3(Z, -X, 0.0f),  XMFLOAT3(-Z, -X, 0.0f)
	};

	DWORD k[60] = // �ε��� �迭
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
	};

	// ����, �ε��� �迭 ���� ��ŭ Ȯ��
	meshData.Vertices.resize(12);
	meshData.Indices.resize(60);

	for (UINT i = 0; i < 12; ++i)
		meshData.Vertices[i].Position = pos[i];

	for (UINT i = 0; i < 60; ++i)
		meshData.Indices[i] = k[i];

	// �ﰢ���� �ɰ��� ���� ������ �ݺ� ����(numSubdivisions ��ŭ)
	for (UINT i = 0; i < numSubdivisions; ++i)
		Subdivide(meshData);

	// �������� ���� �����ϰ� ����Ѵ�.
	for (UINT i = 0; i < meshData.Vertices.size(); ++i)
	{
		// ���� ���� �����Ѵ�.
		XMVECTOR n = XMVector3Normalize(XMLoadFloat3(&meshData.Vertices[i].Position));

		// �־��� ���������� ��ʽ�Ų��.
		XMVECTOR p = radius * n;

		XMStoreFloat3(&meshData.Vertices[i].Position, p);
		XMStoreFloat3(&meshData.Vertices[i].Normal, n);

		// ���� ��ǥ�κ��� �ؽ��� ��ǥ�� ���Ѵ�.
		float theta = MathHelper::AngleFromXY(
			meshData.Vertices[i].Position.x,
			meshData.Vertices[i].Position.z);

		float phi = acosf(meshData.Vertices[i].Position.y / radius);

		meshData.Vertices[i].TexC.x = theta / XM_2PI;
		meshData.Vertices[i].TexC.y = phi / XM_PI;

		// tetha�� ���� P�� ��̺��� ���Ѵ�.
		meshData.Vertices[i].TangentU.x = -radius * sinf(phi) * sinf(theta);
		meshData.Vertices[i].TangentU.y = 0.0f;
		meshData.Vertices[i].TangentU.z = +radius * sinf(phi) * cosf(theta);

		XMVECTOR T = XMLoadFloat3(&meshData.Vertices[i].TangentU);
		XMStoreFloat3(&meshData.Vertices[i].TangentU, XMVector3Normalize(T));
	}
}

void GeometryGenerator::CreateCylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData& meshData)
{
	// �Ʒ� ������, �� ������, ����, sliceCount(���� �ڸ����� �ڸ� ���� ����)
	// , stackCount(��� ����� �� ������ �ڸ� ���� ����), ����� ��� �޽� ������

	// ������ ����� ����, �ε��� ������ ����
	meshData.Vertices.clear();
	meshData.Indices.clear();

	//
	// ������ �׾� �ø���.
	// 

	float stackHeight = height / stackCount; // ���� �� ���� ���̸� ȹ��

	// ������ ���� �� ���� ������ ũ�Ⱑ ���� �� �ִµ�(���� �������ų� Ŀ���� ���) �̸� ���
	float radiusStep = (topRadius - bottomRadius) / stackCount; // ������ �Ǹ鼭 �� ��ŭ�� �������� ������ ������

	UINT ringCount = stackCount + 1; // �� ������ ���ú��� �� �� ����

	// ���ϴ� ������ �ֻ�� ������ �����鼭 ������ ���
	for (UINT i = 0; i < ringCount; ++i)
	{
		float y = -0.5f * height + i * stackHeight; // ����
		float r = bottomRadius + i * radiusStep; // ������

		// ���� ���� ������
		float dTheta = 2.0f * XM_PI / sliceCount; // �� �������� �����̽� ���� ��ŭ �����Ͽ� �������ش�.
		for (UINT j = 0; j <= sliceCount; ++j)
		{
			Vertex vertex;

			float c = cosf(j * dTheta);
			float s = sinf(j * dTheta);

			vertex.Position = XMFLOAT3(r * c, y, r * s); // �� �Լ��� ���� ������ r�� �� �׸���(���� sliceCount�� ����)

			// �ؽ��� ������ ��ǥ ����
			vertex.TexC.x = (float)j / sliceCount;
			vertex.TexC.y = 1.0f - (float)i / stackCount;

			// ������� ������ ���� �Ű�����ȭ �� �� �ִ�. �Ű����� v�� ����
			// ���ԵǾ��µ�, �� �Ű������� ���� ��ǥ v�� ������ ��������
			// ���ư���. (������[bitangent]�� v���� ��ǥ�� ������ ��������)
			// �����ϵ��� �ϱ� ����)
			//  y(v) = h - hv for v in [0,1].
			//  r(v) = r1 + (r0-r1)v
			//
			//  x(t, v) = r(v)*cos(t)
			//  y(t, v) = h - hv
			//  z(t, v) = r(v)*sin(t)
			// 
			//  dx/dt = -r(v)*sin(t)
			//  dy/dt = 0
			//  dz/dt = +r(v)*cos(t)
			//
			//  dx/dv = (r0-r1)*cos(t)
			//  dy/dv = -h
			//  dz/dv = (r0-r1)*sin(t)

			// TangentU�� ���� ����
			vertex.TangentU = XMFLOAT3(-s, 0.0f, c);

			float dr = bottomRadius - topRadius;
			XMFLOAT3 bitangent(dr * c, -height, dr * s);

			XMVECTOR T = XMLoadFloat3(&vertex.TangentU);
			XMVECTOR B = XMLoadFloat3(&bitangent);
			XMVECTOR N = XMVector3Normalize(XMVector3Cross(T, B));
			XMStoreFloat3(&vertex.Normal, N);

			meshData.Vertices.push_back(vertex);
		}
	}

	// ����տ� �ؽ��ĸ� ����� �������� ù ������ ������ ������ �ߺ��Ǿ�� �Ѵ�.
	// �̸� ���� ���� ������ ���� �������� �ϳ� �� ���� �Ѵ�.
	// �����ϰ� ���� �ᱸ�� �Ͱ� ����� ����
	UINT ringVertexCount = sliceCount + 1;

	// �� ���ø��� �������� ���
	// ringCount�� �ƴ� stackCount�� �����ؾ� �ȴ�. 
	// �翬�� ���� �� �ε��� ���۸� ä��� ���̴ϱ�.
	for (UINT i = 0; i < stackCount; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			// ��Ģ�� �ִµ� �׸��� �׷����� �̰͵� ���� �ε��� �迭�� �� �ٸ� ���� ����.
			meshData.Indices.push_back(i * ringVertexCount + j);
			meshData.Indices.push_back((i + 1) * ringVertexCount + j);
			meshData.Indices.push_back((i + 1) * ringVertexCount + j + 1);

			meshData.Indices.push_back(i * ringVertexCount + j);
			meshData.Indices.push_back((i + 1) * ringVertexCount + j + 1);
			meshData.Indices.push_back(i * ringVertexCount + j + 1);
		}
	}

	// ������� ������ �� ä������ �ٴڰ� �Ѳ��� ä���� �Ѵ�.
	BuildCylinderTopCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData); // �Ѳ�
	BuildCylinderBottomCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData); // �ٴ�
}

void GeometryGenerator::BuildCylinderTopCap(float bottomRadius, float topRadius, float height,
	UINT sliceCount, UINT stackCount, MeshData& meshData)
{
	UINT baseIndex = (UINT)meshData.Vertices.size();

	float y = 0.5f * height; // �Ѳ� y ��ǥ, ���� ���� ��ŭ ����
	float dTheta = 2.0f * XM_PI / sliceCount;

	// �� �������� �����ؼ� ���� �������� �����.
	// (�ؽ��� ��ǥ�� ������ �ٸ��Ƿ� ��ó�� �ߺ��� �ʿ��ϴ�.)
	for (UINT i = 0; i <= sliceCount; ++i)
	{
		float x = topRadius * cosf(i * dTheta);
		float z = topRadius * sinf(i * dTheta);

		// ���� ������ �ؽ��� ��ǥ ������ �ظ鿡 ����ϵ���
		// �ؽ��� ��ǥ�� ���̿� ���� ������ ����Ѵ�.
		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		// ������ ���� ������ ����
		meshData.Vertices.push_back(Vertex(x, y, z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
	}

	// �Ѳ� �߽� ��ǥ
	meshData.Vertices.push_back(Vertex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f));

	// �Ѳ��� �ε��� �迭, �׷��� Ȯ���غ��� ������ �������� �ﰢ�� ������ 1�� �� ����.
	UINT centerIndex = (UINT)meshData.Vertices.size() - 1;

	// �Ѳ��� �������� �� ���� ����ϰ� ������ �ȴ�.
	for (UINT i = 0; i < sliceCount; ++i)
	{
		meshData.Indices.push_back(centerIndex);
		meshData.Indices.push_back(baseIndex + i + 1);
		meshData.Indices.push_back(baseIndex + i);
	}
}

void GeometryGenerator::BuildCylinderBottomCap(float bottomRadius, float topRadius, float height,
	UINT sliceCount, UINT stackCount, MeshData& meshData)
{
	// �ٴ� �κ�
	// �Ѳ��� ���� �Ȱ���.

	UINT baseIndex = (UINT)meshData.Vertices.size();
	float y = -0.5f * height; 

	// ���� ������
	float dTheta = 2.0f * XM_PI / sliceCount;
	for (UINT i = 0; i <= sliceCount; ++i)
	{
		float x = bottomRadius * cosf(i * dTheta);
		float z = bottomRadius * sinf(i * dTheta);

		// Scale down by the height to try and make top cap texture coord area
		// proportional to base.
		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		meshData.Vertices.push_back(Vertex(x, y, z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
	}

	// Cap center vertex.
	meshData.Vertices.push_back(Vertex(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f));

	// Cache the index of center vertex.
	UINT centerIndex = (UINT)meshData.Vertices.size() - 1;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		meshData.Indices.push_back(centerIndex);
		meshData.Indices.push_back(baseIndex + i);
		meshData.Indices.push_back(baseIndex + i + 1);
	}
}

// ���� ����� �Լ�, �� ���� ���鶧 �ַ� ���ȴ�.
void GeometryGenerator::CreateGrid(float width, float depth, UINT m, UINT n, MeshData& meshData)
{
	UINT vertexCount = m*n; // ������ ���� ����
	UINT faceCount   = (m-1)*(n-1)*2; // ���ڿ� ���ԵǴ� �ﰢ�� ����

	//
	// �������� �����.
	//

	float halfWidth = 0.5f*width; // �ʺ��� ����, x / 2
	float halfDepth = 0.5f*depth; // ������ ����, z / 2

	float dx = width / (n-1);  // ���� �ʺ� ��ĭ
	float dz = depth / (m-1); // ���� ���� ��ĭ

	float du = 1.0f / (n-1); // �ؽ��� �ʺ� ��ĭ
	float dv = 1.0f / (m-1); // �ؽ��� ���� ��ĭ

	meshData.Vertices.resize(vertexCount);
	for(UINT i = 0; i < m; ++i)
	{
		float z = halfDepth - i*dz; // z�� ������ �� ĭ�� �������鼭 
		for(UINT j = 0; j < n; ++j)
		{
			float x = -halfWidth + j*dx; // x�� ���ʿ��� �� ĭ�� ���������� �̵��ϸ鼭
			// i*n+j �� ������ ���ڸ� ������ ���� �迭�� �־�� �ؼ� ������ --> ������ ��ȯ
			meshData.Vertices[i*n+j].Position = XMFLOAT3(x, 0.0f, z);
			meshData.Vertices[i*n+j].Normal   = XMFLOAT3(0.0f, 1.0f, 0.0f); // ���� ����
			meshData.Vertices[i*n+j].TangentU = XMFLOAT3(1.0f, 0.0f, 0.0f);

			// ���ڿ� ���� �ؽ��� ��ġ ����
			// �ϴ� ���⼭�� 0 ~ 1 ���� ���� ���ڷ� ���߾� ������
			// �Ŀ� ���� �ؽ��� ��ȯ ���ķ� Ȯ���Ͽ� Ÿ�ϸ� ��Ų��.
			meshData.Vertices[i*n+j].TexC.x = j*du;
			meshData.Vertices[i*n+j].TexC.y = i*dv;
		}
	}
 
    //
	// ������ ���� �ε��� ����
	//

	meshData.Indices.resize(faceCount*3); // ���鿡 �����Ǵ� �ﰢ�� ������ 3��, �ﰢ���� ������ 3���ϱ�.

	// ���� �� ĭ�� �����鼭 �ε��� ���, �׸��� �׷����� ���ذ� �� ����.
	UINT k = 0;
	for(UINT i = 0; i < m-1; ++i)
	{
		for(UINT j = 0; j < n-1; ++j)
		{
			meshData.Indices[k]   = i*n+j;
			meshData.Indices[k+1] = i*n+j+1;
			meshData.Indices[k+2] = (i+1)*n+j;

			meshData.Indices[k+3] = (i+1)*n+j;
			meshData.Indices[k+4] = i*n+j+1;
			meshData.Indices[k+5] = (i+1)*n+j+1;

			k += 6; // ���� ĭ���� �̵�
		}
	}
}

void GeometryGenerator::CreateFullscreenQuad(MeshData& meshData)
{
	meshData.Vertices.resize(4);
	meshData.Indices.resize(6);

	// Position coordinates specified in NDC space.
	meshData.Vertices[0] = Vertex(
		-1.0f, -1.0f, 0.0f, 
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f);

	meshData.Vertices[1] = Vertex(
		-1.0f, +1.0f, 0.0f, 
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f);

	meshData.Vertices[2] = Vertex(
		+1.0f, +1.0f, 0.0f, 
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f);

	meshData.Vertices[3] = Vertex(
		+1.0f, -1.0f, 0.0f, 
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f);

	meshData.Indices[0] = 0;
	meshData.Indices[1] = 1;
	meshData.Indices[2] = 2;

	meshData.Indices[3] = 0;
	meshData.Indices[4] = 2;
	meshData.Indices[5] = 3;
}
