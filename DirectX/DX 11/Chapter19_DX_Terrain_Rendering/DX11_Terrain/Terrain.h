//***************************************************************************************
// Terrain.h by Frank Luna (C) 2011 All Rights Reserved.
//   
// Class that renders a terrain using hardware tessellation and multitexturing.
//***************************************************************************************

#ifndef TERRAIN_H
#define TERRAIN_H

#include "d3dUtil.h"

class Camera;
struct DirectionalLight;

class Terrain
{
public:
	struct InitInfo
	{
		std::wstring HeightMapFilename; // RAW ���� �̸�

		// �������� �ؽ��� ���� �̸���
		std::wstring LayerMapFilename0;
		std::wstring LayerMapFilename1;
		std::wstring LayerMapFilename2;
		std::wstring LayerMapFilename3;
		std::wstring LayerMapFilename4;
		std::wstring BlendMapFilename;

		// ���� ��� ���
		float HeightScale;
		// ���̸� ũ��
		UINT HeightmapWidth;
		UINT HeightmapHeight;
		// ���� �� ĭ�� ũ��, Grid �����ÿ� ����ϴ� ux, uz�� �ǹ�
		float CellSpacing;
	};

public:
	Terrain();
	~Terrain();

	float GetWidth()const;
	float GetDepth()const;
	float GetHeight(float x, float z)const;

	XMMATRIX GetWorld()const;
	void SetWorld(CXMMATRIX M);

	void Init(ID3D11Device* device, ID3D11DeviceContext* dc, const InitInfo& initInfo);

	void Draw(ID3D11DeviceContext* dc, const Camera& cam, DirectionalLight lights[3]);

private:
	void LoadHeightmap();
	void Smooth();
	bool InBounds(int i, int j);
	float Average(int i, int j);
	void CalcAllPatchBoundsY();
	void CalcPatchBoundsY(UINT i, UINT j);
	void BuildQuadPatchVB(ID3D11Device* device);
	void BuildQuadPatchIB(ID3D11Device* device);
	void BuildHeightmapSRV(ID3D11Device* device);

private:

	// �ִ� �׼����̼� ����� 64�̴�.
	static const int CellsPerPatch = 64;

	ID3D11Buffer* mQuadPatchVB;
	ID3D11Buffer* mQuadPatchIB;

	ID3D11ShaderResourceView* mLayerMapArraySRV;
	ID3D11ShaderResourceView* mBlendMapSRV;
	ID3D11ShaderResourceView* mHeightMapSRV;

	InitInfo mInfo;

	UINT mNumPatchVertices;
	UINT mNumPatchQuadFaces;

	UINT mNumPatchVertRows;
	UINT mNumPatchVertCols;

	XMFLOAT4X4 mWorld;

	Material mMat;

	std::vector<XMFLOAT2> mPatchBoundsY; // x���п� y�� �ּڰ�, y���п� y�� �ִ��� �����Ѵ�.
	std::vector<float> mHeightmap;
};

#endif // TERRAIN_H