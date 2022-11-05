#ifndef BASICMODEL_H
#define BASICMODEL_H

#include "MeshGeometry.h"
#include "TextureMgr.h"
#include "Vertex.h"

class BasicModel
{
public:
	BasicModel(ID3D11Device* device, TextureMgr& texMgr, const std::string& modelFilename, const std::wstring& texturePath);
	~BasicModel();

	UINT SubsetCount;

	std::vector<Material> Mat;
	std::vector<ID3D11ShaderResourceView*> DiffuseMapSRV;
	std::vector<ID3D11ShaderResourceView*> NormalMapSRV;

	// CPU���� �޽� �ڷḦ ���� �� �ֵ��� �ý��� �޸𸮿� ���纻�� ���� �д�.
	// �����ü ���, ����, �浹 ������ ��� CPU�� �����ؼ� �ʿ��ϴ�.
	std::vector<Vertex::PosNormalTexTan> Vertices;
	std::vector<USHORT> Indices;
	std::vector<MeshGeometry::Subset> Subsets;

	MeshGeometry ModelMesh;
};

struct BasicModelInstance
{
	BasicModel* Model;
	XMFLOAT4X4 World;
};

#endif // SKINNEDMODEL_H