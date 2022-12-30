#pragma once

#include <goh.h>

#include "ModelLoader.h"
#include "d3dUtil.h"

using namespace DirectX;

// �������� ������, �ε������� ���� �� ��𼭺��� �������� � �޽�����
// �˷��ִ� �ε����� ��� �ִ� �༮
class ModelMesh
{
public:
	ModelMesh();
	~ModelMesh();

	template <typename VertexType>
	void SetVertices(ID3D11Device* device, const VertexType* vertices, UINT count);
	void SetSubsetTable(std::vector<ModelLoader::Subset>& subsetTable);
	void SetIndices(ID3D11Device* device, const UINT* indices, UINT count);
	void Draw(ID3D11DeviceContext* dc, UINT subsetId);

private:
	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	DXGI_FORMAT mIndexBufferFormat; // Always 16-bit
	UINT mVertexStride;

	std::vector<ModelLoader::Subset> mSubsetTable;
};

template <typename VertexType>
void ModelMesh::SetVertices(ID3D11Device* device, const VertexType* vertices, UINT count)
{
	ReleaseCOM(mVB);

	mVertexStride = sizeof(VertexType);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexType) * count;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;

	HR(device->CreateBuffer(&vbd, &vinitData, &mVB));
}
