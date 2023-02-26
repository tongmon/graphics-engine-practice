//***************************************************************************************
// Terrain.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "Terrain.h"
#include "Camera.h"
#include "LightHelper.h"
#include "Effects.h"
#include "Vertex.h"
#include <fstream>
#include <sstream>

Terrain::Terrain() : 
	mQuadPatchVB(0), 
	mQuadPatchIB(0), 
	mLayerMapArraySRV(0), 
	mBlendMapSRV(0), 
	mHeightMapSRV(0),
	mNumPatchVertices(0),
	mNumPatchQuadFaces(0),
	mNumPatchVertRows(0),
	mNumPatchVertCols(0)
{
	XMStoreFloat4x4(&mWorld, XMMatrixIdentity());

	mMat.Ambient  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mMat.Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mMat.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 64.0f);
	mMat.Reflect  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
}

Terrain::~Terrain()
{
	ReleaseCOM(mQuadPatchVB);
	ReleaseCOM(mQuadPatchIB);
	ReleaseCOM(mLayerMapArraySRV);
	ReleaseCOM(mBlendMapSRV);
	ReleaseCOM(mHeightMapSRV);
}

float Terrain::GetWidth()const
{
	// ��ü ���� �ʺ�(����)
	return (mInfo.HeightmapWidth-1)*mInfo.CellSpacing;
}

float Terrain::GetDepth()const
{
	// ��ü ���� ����(����)
	return (mInfo.HeightmapHeight-1)*mInfo.CellSpacing;
}

// �ش� ���� ������ ��ġ���� ������ ���� ���� ȹ���ϰ� ���ִ� �Լ�
// �ϴ� �־��� ��ġ�� ��� ���ڿ� ���ϴ��� Ȯ���� �ϰ�
// �� ���ڿ��� ������ ���� ���̸� ȹ���Ѵ�.
float Terrain::GetHeight(float x, float z)const
{
	// ���� ���� ������ "��ĭ" �������� ��ȯ�Ѵ�. (������ ������ ����, ���� �̵��Ѵ�.)
	// ���� ���� ������ ������ ������ �̹� ���� ������ ���� ��ǥ���
	// ���� ��ǥ�谡 ����. �߽��� �� ��ǥ�� ��� (0,0,0)�̴�.
	// x,z ��ǥ�� ������ ��� ������ (0,0)�� (-0.5f*GetWidth(), 0.5f*GetDepth())�� �ٲ�
	// ��ǥ��� ��ȯ �Ѵٰ� �����غ��� ������ (0,0) �̿��� ��� ��ǥ�迡�� (1,1)��
	// �� �״�� �������� x�� 1, z�� 1 ��ŭ �������־ (1,1)�̴�.
	// �ٵ� ������ (-0.5f*GetWidth(), 0.5f*GetDepth()) �̰� �ǰ� z���� �ݴ� ������ �Ǹ�
	// ������ (0,0)�� (0.5f*GetWidth(), 0.5f*GetDepth()) �̿� ���� ǥ���� �� ���̰�
	// ������ (1,1)�� �������� ���� 1,1 ������ �ִٰ� �ߴµ� z���� ������ �Ǿ����� z�� 1�� �ƴ� -1�� ������ ���� �ǰ�
	// ������ǥ�� (0.5f*GetWidth() + 1, 0.5f*GetDepth() - 1) �̰� �� ���̴�.
	// mInfo.CellSpacing �̸� ������ ������ ��ǥ�� ������ �̵��� ���� �ƴ϶�
	// �� ������ ���̵� ���� 0.5���� CellSpacing�� 1�� �ٲٱ� ���ؼ� ���� ���̴�. (���� ����غ��� �� ���´�.)
	float c = (x + 0.5f*GetWidth()) /  mInfo.CellSpacing;
	float d = (z - 0.5f*GetDepth()) / -mInfo.CellSpacing;

	// �־��� ��ġ�� ���� ��� ���� ���Ѵ�. (�����Լ� ���)
	int row = (int)floorf(d);
	int col = (int)floorf(c);

	// ���� ��ĭ�� �� ���̵��� ���Ѵ�.
	// row*mInfo.HeightmapWidth + col �� �༮�� �������� ���� �� �ִ� ����
	// ���� ���ڸ� ���鶧 ������ (-0.5f*GetWidth(), 0.5f*GetDepth()) ���⼭���� �����̱� �����̴�.
	// A*--*B
	//  | /|
	//  |/ |
	// C*--*D
	float A = mHeightmap[row*mInfo.HeightmapWidth + col];
	float B = mHeightmap[row*mInfo.HeightmapWidth + col + 1];
	float C = mHeightmap[(row+1)*mInfo.HeightmapWidth + col];
	float D = mHeightmap[(row+1)*mInfo.HeightmapWidth + col + 1];

	// 1x1���� s, t�� 1�� �Ѵ��� �ƴ��� �Ǵ�
	// �̸� �ϴ� ������ �簢���� �ΰ��� �ﰢ������ �̷�� ���⿡
	// ABC�� ���ϴ���, BCD�� ���ϴ��� �Ǵ��ؾ� �ùٸ� ���̸� ���� �� �ִ�.
	float s = c - (float)col;
	float t = d - (float)row;

	// ABC�� ���ϸ� s + t�� 1���� �۰ų� ����.
	if( s + t <= 1.0f)
	{
		float uy = B - A; // ���� AB���� ���̿� �����ϴ� ����
		float vy = C - A; // ���� AC���� ���̿� �����ϴ� ����
		return A + s*uy + t*vy;
	}
	else // DCB�� ���ϴ� ���
	{
		float uy = C - D; // ���� DC���� ���̿� �����ϴ� ����
		float vy = B - D; // ���� DB���� ���̿� �����ϴ� ����
		// D���� C,B�� ������� ���Ͷ� ������ ����� 1 - s, 1 - t�� ����Ѵ�.
		return D + (1.0f - s) * uy + (1.0f - t) * vy;
	}
}

XMMATRIX Terrain::GetWorld()const
{
	return XMLoadFloat4x4(&mWorld);
}

void Terrain::SetWorld(CXMMATRIX M)
{
	XMStoreFloat4x4(&mWorld, M);
}

void Terrain::Init(ID3D11Device* device, ID3D11DeviceContext* dc, const InitInfo& initInfo)
{
	mInfo = initInfo;

	// ���̸��� ��ġ���� ���ڷ� �����ϵ�, ������ �� ��ġ�� ���� ��ĭ ������
	// CellsPerPatch�� �ǰ� �Ѵ�. (�����̶� +1)
	// �ο������� �ϸ� mInfo.HeightmapHeight ��� ���� ������ ���̰� �Ű��� �ִ� ���̰��̴�.
	// ���⼭ 1�� ���� ��ĭ ������ �ȴ�.
	// CellsPerPatch ��� ��ĭ ������ 64�� �Ű��� �ִµ� ��ġ �ϳ��� �ִ� �׼����̼��� �Ǹ�
	// �ϳ��� �簢���� 64���� ��ĭ�� ���� �簢���� �Ǳ⿡ 64�� ������ �Ǿ� �ִ� ���̴�.
	// �׷��� (mInfo.HeightmapHeight-1) / CellsPerPatch �� �༮�� ���̸��� 64 ���� ������
	// ������ ���� ���̰� ���⿡ + 1�� �Ѵٴ� ���� ��ĭ �������� ���� ������ �ٲٱ� �����̴�.
	// ���� ��� 1920�� mInfo.HeightmapHeight ��� 1919 / 64 = 29 ���� 29���� ���� ������ ��ĭ�� �����
	// �� ��ĭ�� �����ϱ� ���ؼ� 30���� ���� ������ ������ �ʿ��ϴ�.
	mNumPatchVertRows = ((mInfo.HeightmapHeight-1) / CellsPerPatch) + 1;
	mNumPatchVertCols = ((mInfo.HeightmapWidth-1) / CellsPerPatch) + 1;

	mNumPatchVertices  = mNumPatchVertRows*mNumPatchVertCols; // ���̸� ���� ����
	mNumPatchQuadFaces = (mNumPatchVertRows-1)*(mNumPatchVertCols-1); // ���̸� ����(��ĭ) ����

	LoadHeightmap();
	Smooth();
	CalcAllPatchBoundsY();

	BuildQuadPatchVB(device);
	BuildQuadPatchIB(device);
	BuildHeightmapSRV(device);

	std::vector<std::wstring> layerFilenames;
	layerFilenames.push_back(mInfo.LayerMapFilename0);
	layerFilenames.push_back(mInfo.LayerMapFilename1);
	layerFilenames.push_back(mInfo.LayerMapFilename2);
	layerFilenames.push_back(mInfo.LayerMapFilename3);
	layerFilenames.push_back(mInfo.LayerMapFilename4);
	mLayerMapArraySRV = d3dHelper::CreateTexture2DArraySRV(device, dc, layerFilenames);

	HR(D3DX11CreateShaderResourceViewFromFile(device, 
		mInfo.BlendMapFilename.c_str(), 0, 0, &mBlendMapSRV, 0));
}

void Terrain::Draw(ID3D11DeviceContext* dc, const Camera& cam, DirectionalLight lights[3])
{
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	dc->IASetInputLayout(InputLayouts::Terrain);

	UINT stride = sizeof(Vertex::Terrain);
    UINT offset = 0;
    dc->IASetVertexBuffers(0, 1, &mQuadPatchVB, &stride, &offset);
	dc->IASetIndexBuffer(mQuadPatchIB, DXGI_FORMAT_R16_UINT, 0);

	XMMATRIX viewProj = cam.ViewProj();
	XMMATRIX world  = XMLoadFloat4x4(&mWorld);
	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
	XMMATRIX worldViewProj = world*viewProj;

	XMFLOAT4 worldPlanes[6];
	ExtractFrustumPlanes(worldPlanes, viewProj);

	// Set per frame constants.
	Effects::TerrainFX->SetViewProj(viewProj);
	Effects::TerrainFX->SetEyePosW(cam.GetPosition());
	Effects::TerrainFX->SetDirLights(lights);
	Effects::TerrainFX->SetFogColor(Colors::Silver);
	Effects::TerrainFX->SetFogStart(15.0f);
	Effects::TerrainFX->SetFogRange(175.0f);
	Effects::TerrainFX->SetMinDist(20.0f);
	Effects::TerrainFX->SetMaxDist(500.0f);
	Effects::TerrainFX->SetMinTess(0.0f);
	Effects::TerrainFX->SetMaxTess(6.0f);
	Effects::TerrainFX->SetTexelCellSpaceU(1.0f / mInfo.HeightmapWidth);
	Effects::TerrainFX->SetTexelCellSpaceV(1.0f / mInfo.HeightmapHeight);
	Effects::TerrainFX->SetWorldCellSpace(mInfo.CellSpacing);
	Effects::TerrainFX->SetWorldFrustumPlanes(worldPlanes);
	
	Effects::TerrainFX->SetLayerMapArray(mLayerMapArraySRV);
	Effects::TerrainFX->SetBlendMap(mBlendMapSRV);
	Effects::TerrainFX->SetHeightMap(mHeightMapSRV);

	Effects::TerrainFX->SetMaterial(mMat);

	ID3DX11EffectTechnique* tech = Effects::TerrainFX->Light1Tech;
    D3DX11_TECHNIQUE_DESC techDesc;
    tech->GetDesc( &techDesc );

    for(UINT i = 0; i < techDesc.Passes; ++i)
    {
        ID3DX11EffectPass* pass = tech->GetPassByIndex(i);
		pass->Apply(0, dc);

		dc->DrawIndexed(mNumPatchQuadFaces*4, 0, 0);
	}	

	// FX sets tessellation stages, but it does not disable them.  So do that here
	// to turn off tessellation.
	dc->HSSetShader(0, 0, 0);
	dc->DSSetShader(0, 0, 0);
}

void Terrain::LoadHeightmap()
{
	// �� ������ ���̸� ��� �迭(����).
	std::vector<unsigned char> in(mInfo.HeightmapWidth * mInfo.HeightmapHeight);

	// ���̸� ������ ����.
	// �� Raw ������ 2049 x 2049 ���� ��ŭ�� ���� ������ ���ִ�.
	std::ifstream inFile;
	inFile.open(mInfo.HeightMapFilename.c_str(), std::ios_base::binary);

	if (inFile)
	{
		// RAW ������ ����Ʈ���� �о� ���δ�.
		inFile.read((char*)&in[0], (std::streamsize)in.size());

		// ������ �� �������.
		inFile.close();
	}

	// �迭 �ڷḦ �ε� �Ҽ��� �迭�� ����ؼ� �����Ѵ�.
	// �̷��� �����ؾ� ���߿� 255 ������ �� ū ���̸� �׸� �� �ִ�.
	// �ֳ��ϸ� �ʹ� ���̸��� ������ 0 ~ 255�����ϱ�....
	mHeightmap.resize(mInfo.HeightmapHeight * mInfo.HeightmapWidth, 0);
	for (UINT i = 0; i < mInfo.HeightmapHeight * mInfo.HeightmapWidth; ++i)
	{
		mHeightmap[i] = (in[i] / 255.0f) * mInfo.HeightScale;
	}
}

// �� ���̸� �ȼ��� Average�� �����ϴ� �Լ�
void Terrain::Smooth()
{
	std::vector<float> dest( mHeightmap.size() );

	for(UINT i = 0; i < mInfo.HeightmapHeight; ++i)
	{
		for(UINT j = 0; j < mInfo.HeightmapWidth; ++j)
		{
			dest[i*mInfo.HeightmapWidth+j] = Average(i,j);
		}
	}

	// Replace the old heightmap with the filtered one.
	mHeightmap = dest;
}

bool Terrain::InBounds(int i, int j)
{
	// i�� j�� ��ȿ�� �����̸� true �ƴϸ� false.
	return 
		i >= 0 && i < (int)mInfo.HeightmapHeight && 
		j >= 0 && j < (int)mInfo.HeightmapWidth;
}

float Terrain::Average(int i, int j)
{
	// ij��° �ȼ��� ��� ���̸� ����ϴ� �Լ�.
	// ij��° �ȼ� �� �� �̿��� ���� �ȼ��� ����� ���Ѵ�.
	// �����ڸ� �ȼ����� ���, �ش� ������ �̿� �ȼ��� ���ڸ��ٸ�
	// �׳� ��տ� ���Խ�Ű�� �ʰ� �Ѿ.
	//
	// ----------
	// | 1| 2| 3|
	// ----------
	// |4 |ij| 6|
	// ----------
	// | 7| 8| 9|
	// ----------

	float avg = 0.0f;
	float num = 0.0f;

	// ���� ���� ����ϱ� ���� int�� ����Ѵ�.
	// i�� j�� 0�� ��쵵 ����ؾ� �Ѵ�.
	for(int m = i-1; m <= i+1; ++m)
	{
		for(int n = j-1; n <= j+1; ++n)
		{
			if( InBounds(m,n) )
			{
				avg += mHeightmap[m*mInfo.HeightmapWidth + n];
				num += 1.0f;
			}
		}
	}

	return avg / num;
}

void Terrain::CalcAllPatchBoundsY()
{
	mPatchBoundsY.resize(mNumPatchQuadFaces);

	// For each patch
	for(UINT i = 0; i < mNumPatchVertRows-1; ++i)
	{
		for(UINT j = 0; j < mNumPatchVertCols-1; ++j)
		{
			CalcPatchBoundsY(i, j);
		}
	}
}

void Terrain::CalcPatchBoundsY(UINT i, UINT j)
{
	// �� ��ġ�� �����ϴ� ���̸� ���ҵ��� �����鼭
	// �ִ�, �ּ� ���̸� ���Ѵ�.

	UINT x0 = j * CellsPerPatch;
	UINT x1 = (j + 1) * CellsPerPatch;

	UINT y0 = i * CellsPerPatch;
	UINT y1 = (i + 1) * CellsPerPatch;

	// �� ��ġ(64x64)�� �ش��ϴ� AABB �浹 �ڽ� ����, ����ü �浹�� �����
	float minY = +MathHelper::Infinity;
	float maxY = -MathHelper::Infinity;
	for (UINT y = y0; y <= y1; ++y)
	{
		for (UINT x = x0; x <= x1; ++x)
		{
			UINT k = y * mInfo.HeightmapWidth + x;
			minY = MathHelper::Min(minY, mHeightmap[k]);
			maxY = MathHelper::Max(maxY, mHeightmap[k]);
		}
	}

	UINT patchID = i * (mNumPatchVertCols - 1) + j;
	mPatchBoundsY[patchID] = XMFLOAT2(minY, maxY);
}

void Terrain::BuildQuadPatchVB(ID3D11Device* device)
{
	std::vector<Vertex::Terrain> patchVertices(mNumPatchVertRows*mNumPatchVertCols); // ���̸� ������

	// ���� ���� ���� ���鶧�� �����ϰ� ����
	float halfWidth = 0.5f*GetWidth();
	float halfDepth = 0.5f*GetDepth();

	// ��ĭ ����, ���� ����
	float patchWidth = GetWidth() / (mNumPatchVertCols-1);
	float patchDepth = GetDepth() / (mNumPatchVertRows-1);
	// �ؽ��� ���� ��ǥ ����, ���� ����
	float du = 1.0f / (mNumPatchVertCols-1);
	float dv = 1.0f / (mNumPatchVertRows-1);

	for(UINT i = 0; i < mNumPatchVertRows; ++i)
	{
		float z = halfDepth - i*patchDepth;
		for(UINT j = 0; j < mNumPatchVertCols; ++j)
		{
			float x = -halfWidth + j*patchWidth;

			patchVertices[i*mNumPatchVertCols+j].Pos = XMFLOAT3(x, 0.0f, z);

			// �ؽ��İ� ���� ��ü�� �������� ������ Tex ��ǥ �Ҵ�
			patchVertices[i*mNumPatchVertCols+j].Tex.x = j*du;
			patchVertices[i*mNumPatchVertCols+j].Tex.y = i*dv;
		}
	}

	// �� ���� �������� Y ������ ���� ��� ������ ��ġ�� ������ �д�.
	/*
	1 ------ 2
	|		 |
	|		 |
	|		 |
	|		 |
	3 ------ 4
	���⿡�� ���� 1�� �� �簢���� ������ ��踦 ������ �дٴ� ���̴�. 
	*/
	for (UINT i = 0; i < mNumPatchVertRows - 1; ++i)
	{
		for (UINT j = 0; j < mNumPatchVertCols - 1; ++j)
		{
			UINT patchID = i * (mNumPatchVertCols - 1) + j;
			patchVertices[i * mNumPatchVertCols + j].BoundsY = mPatchBoundsY[patchID];
		}
	}

    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::Terrain) * patchVertices.size();
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &patchVertices[0];
    HR(device->CreateBuffer(&vbd, &vinitData, &mQuadPatchVB));
}

void Terrain::BuildQuadPatchIB(ID3D11Device* device)
{
	// �׼����̼��� �� ���̱� ������ ������ ���� �ε��� ����� �ﰢ������
	// �� k += 6 �̷��� ����� �Ǿ����� �׼����̼� ������ 4�� �簢������ �׸� ���̱⿡
	// �ε��� ����� k += 4 ������� �̷������.
	std::vector<USHORT> indices(mNumPatchQuadFaces * 4); // �簢�� ��ġ�� 4���� �ε���

	// �� �簢�� ���� ���ε��� ����Ѵ�.
	int k = 0;
	for (UINT i = 0; i < mNumPatchVertRows - 1; ++i)
	{
		for (UINT j = 0; j < mNumPatchVertCols - 1; ++j)
		{
			// 2x2 �簢�� ��ġ�� ���� ���� �� ��
			indices[k] = i * mNumPatchVertCols + j;
			indices[k + 1] = i * mNumPatchVertCols + j + 1;

			// 2x2 �簢�� ��ġ�� �Ʒ��� ���� �� ��
			indices[k + 2] = (i + 1) * mNumPatchVertCols + j;
			indices[k + 3] = (i + 1) * mNumPatchVertCols + j + 1;

			k += 4; // ���� �簢������ �̵�
		}
	}

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(USHORT) * indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(device->CreateBuffer(&ibd, &iinitData, &mQuadPatchIB));
}

void Terrain::BuildHeightmapSRV(ID3D11Device* device)
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = mInfo.HeightmapWidth;
	texDesc.Height = mInfo.HeightmapHeight;
    texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format    = DXGI_FORMAT_R16_FLOAT; // 16��Ʈ �ε��Ҽ��� ���
	texDesc.SampleDesc.Count   = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	// HALF�� 16��Ʈ �ε��Ҽ��� ������ ���� �������� xnamath.h�� ���ǵ�.
	std::vector<HALF> hmap(mHeightmap.size());
	std::transform(mHeightmap.begin(), mHeightmap.end(), hmap.begin(), XMConvertFloatToHalf);
	
	// ���̸��� ���� ������ ������ ���� ���ҽ��� �����Ѵ�.
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &hmap[0];
	data.SysMemPitch = mInfo.HeightmapWidth * sizeof(HALF); // ������ ũ��
    data.SysMemSlicePitch = 0; // 3���� �迭���� ���� ���� �ʿ������� �̰� texture2D�̴�.

	ID3D11Texture2D* hmapTex = 0;
	HR(device->CreateTexture2D(&texDesc, &data, &hmapTex));

	// ���������� mHeightMapSRV ���⿡ [0, 255] --> �˸��� ���̷� ��ȯ�̵�
	// ���ҽ��� ������ �ȴ�.
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	HR(device->CreateShaderResourceView(hmapTex, &srvDesc, &mHeightMapSRV));

	// SRV saves reference.
	ReleaseCOM(hmapTex);
}