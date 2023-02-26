//***************************************************************************************
// PickingDemo.cpp by Frank Luna (C) 2011 All Rights Reserved.
//
// Demonstrates picking.
//
// Controls:
//      Click the right mouse button to select.
//
//      Hold '1' for wireframe mode.
//
//***************************************************************************************

// 3D �������� ���콺 Ŭ������ ��ü�� �� ���ư� ��ü�� �� �������� �����ϴ�
// ����� ���� �����̴�.
// �ϴ� �����ϰ� ���� �������� �������� ��� �� �������� ó������ ���� ��ü�� �����Ѵ�.
// �� ��ü�� ��� ������ �߿� ���� ������ ����� �������� ���õȴ�.
// �̸� �����ϱ⿡ �߿��� �κ��� �ϴ� �������� ������ �� �־�� �ϴ� ���̰�
// �������� �浹�̴�. ��Ȯ�ϰ� �������� �浹 �ڽ����� �浹, �ﰢ ������� �������� �浹
// �� �ΰ����� �����ؾ� �Ѵ�.

#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "GeometryGenerator.h"
#include "MathHelper.h"
#include "LightHelper.h"
#include "Effects.h"
#include "Vertex.h"
#include "Camera.h"
#include "RenderStates.h"
#include "xnacollision.h"

#pragma comment (lib,"d3d11.lib")
#pragma comment (lib,"d3dx11d.lib")
#pragma comment (lib,"D3DCompiler.lib")
#pragma comment (lib,"Effects11d.lib")
#pragma comment (lib,"dxgi.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib,"winmm.lib")

class PickingApp : public D3DApp
{
public:
	PickingApp(HINSTANCE hInstance);
	~PickingApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	void BuildMeshGeometryBuffers();
	void Pick(int sx, int sy);

private:

	ID3D11Buffer* mMeshVB;
	ID3D11Buffer* mMeshIB;

	// ���õ� �༮�� �߸��� ���ؼ��� ���纻�� �ʿ�
	std::vector<Vertex::Basic32> mMeshVertices;
	std::vector<UINT> mMeshIndices;

	XNA::AxisAlignedBox mMeshBox;

	DirectionalLight mDirLights[3];
	Material mMeshMat;
	Material mPickedTriangleMat; // ���õ� �༮�� ������ �ٸ���

	// Define transformations from local spaces to world space.
	XMFLOAT4X4 mMeshWorld;

	UINT mMeshIndexCount;

	UINT mPickedTriangle;

	Camera mCam;

	POINT mLastMousePos;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	PickingApp theApp(hInstance);

	if (!theApp.Init())
		return 0;

	return theApp.Run();
}


PickingApp::PickingApp(HINSTANCE hInstance)
	: D3DApp(hInstance), mMeshVB(0), mMeshIB(0), mMeshIndexCount(0), mPickedTriangle(-1)
{
	mMainWndCaption = L"Picking Demo";

	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	mCam.SetPosition(0.0f, 2.0f, -15.0f);

	XMMATRIX MeshScale = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	XMMATRIX MeshOffset = XMMatrixTranslation(0.0f, 1.0f, 0.0f);
	XMStoreFloat4x4(&mMeshWorld, XMMatrixMultiply(MeshScale, MeshOffset));

	mDirLights[0].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[0].Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLights[0].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLights[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	mDirLights[1].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[1].Diffuse = XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
	mDirLights[1].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	mDirLights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

	mDirLights[2].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Direction = XMFLOAT3(0.0f, -0.707f, -0.707f);

	mMeshMat.Ambient = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	mMeshMat.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	mMeshMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);

	mPickedTriangleMat.Ambient = XMFLOAT4(0.0f, 0.8f, 0.4f, 1.0f);
	mPickedTriangleMat.Diffuse = XMFLOAT4(0.0f, 0.8f, 0.4f, 1.0f);
	mPickedTriangleMat.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 16.0f);
}

PickingApp::~PickingApp()
{
	ReleaseCOM(mMeshVB);
	ReleaseCOM(mMeshIB);

	Effects::DestroyAll();
	InputLayouts::DestroyAll();
}

bool PickingApp::Init()
{
	if (!D3DApp::Init())
		return false;

	// Must init Effects first since InputLayouts depend on shader signatures.
	Effects::InitAll(md3dDevice);
	InputLayouts::InitAll(md3dDevice);
	RenderStates::InitAll(md3dDevice);

	BuildMeshGeometryBuffers();

	return true;
}

void PickingApp::OnResize()
{
	D3DApp::OnResize();

	mCam.SetLens(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
}

void PickingApp::UpdateScene(float dt)
{
	//
	// Control the camera.
	//
	if (GetAsyncKeyState('W') & 0x8000)
		mCam.Walk(10.0f * dt);

	if (GetAsyncKeyState('S') & 0x8000)
		mCam.Walk(-10.0f * dt);

	if (GetAsyncKeyState('A') & 0x8000)
		mCam.Strafe(-10.0f * dt);

	if (GetAsyncKeyState('D') & 0x8000)
		mCam.Strafe(10.0f * dt);
}

void PickingApp::DrawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Silver));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	md3dImmediateContext->IASetInputLayout(InputLayouts::Basic32);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	mCam.UpdateViewMatrix();

	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();
	XMMATRIX viewProj = mCam.ViewProj();




	// Set per frame constants.
	Effects::BasicFX->SetDirLights(mDirLights);
	Effects::BasicFX->SetEyePosW(mCam.GetPosition());

	ID3DX11EffectTechnique* activeMeshTech = Effects::BasicFX->Light3Tech;

	D3DX11_TECHNIQUE_DESC techDesc;
	activeMeshTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		// Draw the Mesh.

		if (GetAsyncKeyState('1') & 0x8000)
			md3dImmediateContext->RSSetState(RenderStates::WireframeRS);

		md3dImmediateContext->IASetVertexBuffers(0, 1, &mMeshVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mMeshIB, DXGI_FORMAT_R32_UINT, 0);

		XMMATRIX world = XMLoadFloat4x4(&mMeshWorld);
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world * view * proj;

		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);
		Effects::BasicFX->SetMaterial(mMeshMat);

		activeMeshTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mMeshIndexCount, 0, 0); // ��ü �ڵ����� �ѹ� �׸���

		// ������ȭ�� �ܰ� ����
		md3dImmediateContext->RSSetState(0);

		//  ���õ� �ﰢ���� �ٽ� �ٸ� ������ �׷��ش�.

		if (mPickedTriangle != -1)
		{
			// ���� �ﰢ���� �ι� �׸��� ���̹Ƿ� �Ϲ����� ����� < �̰ɷ� ���ϸ� ���� �������� ©����.
			// ���� <= �� �ٲپ� �������ְ� ���� ���ٽ� ���¸� �ٲپ� �־�� �Ѵ�.

			md3dImmediateContext->OMSetDepthStencilState(RenderStates::LessEqualDSS, 0);

			Effects::BasicFX->SetMaterial(mPickedTriangleMat); // ���� �ٲٰ�
			activeMeshTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
			md3dImmediateContext->DrawIndexed(3, 3 * mPickedTriangle, 0); // mPickedTriangle ��°�� �ﰢ���� �׸���.

			// ����,���ٽ� ���� ����
			md3dImmediateContext->OMSetDepthStencilState(0, 0);
		}
	}

	HR(mSwapChain->Present(0, 0));
}

void PickingApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		mLastMousePos.x = x;
		mLastMousePos.y = y;

		SetCapture(mhMainWnd);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		Pick(x, y); // ���� Ŭ������ ����
	}
}

void PickingApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void PickingApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePos.y));

		mCam.Pitch(dy);
		mCam.RotateY(dx);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void PickingApp::BuildMeshGeometryBuffers()
{
	std::ifstream fin("Models/car.txt");

	if (!fin)
	{
		MessageBox(0, L"Models/car.txt not found.", 0, 0);
		return;
	}

	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;

	fin >> ignore >> vcount;
	fin >> ignore >> tcount;
	fin >> ignore >> ignore >> ignore >> ignore;

	XMFLOAT3 vMinf3(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	XMFLOAT3 vMaxf3(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

	XMVECTOR vMin = XMLoadFloat3(&vMinf3);
	XMVECTOR vMax = XMLoadFloat3(&vMaxf3);
	mMeshVertices.resize(vcount);
	for (UINT i = 0; i < vcount; ++i)
	{
		fin >> mMeshVertices[i].Pos.x >> mMeshVertices[i].Pos.y >> mMeshVertices[i].Pos.z;
		fin >> mMeshVertices[i].Normal.x >> mMeshVertices[i].Normal.y >> mMeshVertices[i].Normal.z;

		XMVECTOR P = XMLoadFloat3(&mMeshVertices[i].Pos);

		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);
	}

	XMStoreFloat3(&mMeshBox.Center, 0.5f * (vMin + vMax));
	XMStoreFloat3(&mMeshBox.Extents, 0.5f * (vMax - vMin));

	fin >> ignore;
	fin >> ignore;
	fin >> ignore;

	mMeshIndexCount = 3 * tcount;
	mMeshIndices.resize(mMeshIndexCount);
	for (UINT i = 0; i < tcount; ++i)
	{
		fin >> mMeshIndices[i * 3 + 0] >> mMeshIndices[i * 3 + 1] >> mMeshIndices[i * 3 + 2];
	}

	fin.close();

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::Basic32) * vcount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &mMeshVertices[0];
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mMeshVB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * mMeshIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &mMeshIndices[0];
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mMeshIB));
}

// ���� �Լ�
// �������� �����ϰ� �浹�� �˻��Ѵ�.
// ȭ�鿡 ���������� ���� ���� ���� ���� ȹ���ؾ��Ѵ�.
// ������ �̿� ����.
// ����Ʈ ����� NDC��ǥ �� ����ȭ�� ��ǥ�迡 ��ġ�� ��ǥ���� �����ָ�
// ���� ����Ϳ� ������ �� ��ǥ�� ���´�.
// �̸� �ݴ�� ����ϸ� ���� ����Ϳ� ������ �� ��ǥ�� NDC ��ǥ�� �� �� �ִ�.
// �� NDC ��ǥ�� ���� ��ǥ��� ����� ���� �ʺ��� �������� �ִ� ��Ⱦ�� �ٽ� ���Ѵ�.
// �׷��� -1 <= x <= 1 --> -r <= x <= r �̷��� �ٽ� ���ϸ� ���� ������ Ŭ���� ��ǥ�� ������µ�
// ���⼭�� ��� �������� �� ������ �������� å������ �� ���ư� z�� 1�� ���߾� ���.
// ����� ���϶�� �׷������� �𸣰ڴµ� ��ư z�� 1�� ���߾� �ַ��� z�� ���� ����
// x,y���ٰ� �������ָ� �Ǵµ� �̸� ǥ���Ͽ� ����� ��������
// �ؿ� ���� (+2.0f * sx / mClientWidth - 1.0f) / P(0, 0), -2.0f * sy / mClientHeight + 1.0f) / P(1, 1)
// �� �� ���̴�.
void PickingApp::Pick(int sx, int sy)
{
	XMMATRIX P = mCam.Proj();

	// ī�޶� ��ǥ�� ������ ������ ���
	// ��� P�� ���Ҹ� ����ؼ� ������ ���� 
	float vx = (+2.0f * sx / mClientWidth - 1.0f) / P(0, 0);
	float vy = (-2.0f * sy / mClientHeight + 1.0f) / P(1, 1);

	// �þ� ������ ������ ����
	XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); // ����, �������� ������ ��� ī�޶� ����
	XMVECTOR rayDir = XMVectorSet(vx, vy, 1.0f, 0.0f); // z == 1 �Ÿ��� ��� �������� ���� ����

	// ���� �������� ��ü�� ���� �������� ���߾� �־�� �Ѵ�.
	// ����ü �Ǻ������� ���� ������ ��ǥ�� ������ ���߾� �־�� �ùٸ� �浹 ������ �����ϴ�.
	XMMATRIX V = mCam.View();
	XMVECTOR Detv = XMMatrixDeterminant(V);
	XMMATRIX invView = XMMatrixInverse(&Detv, V);

	XMMATRIX W = XMLoadFloat4x4(&mMeshWorld);
	XMVECTOR Detw = XMMatrixDeterminant(W);
	XMMATRIX invWorld = XMMatrixInverse(&Detw, W);

	XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld); // ���� * ���� �� ����� �����

	// ���� XMVector3TransformCoord �� �Լ��� ���
	// ���ʹ� XMVector3TransformNormal �� �Լ��� ���
	// �� �Լ��� ���̴� w ���� 1�̳� 0�̳� ���� ���̴�.
	rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
	rayDir = XMVector3TransformNormal(rayDir, toLocal);

	// ������ ����ȭ
	rayDir = XMVector3Normalize(rayDir);

	// �������� �޽��� �����ڿ� �����ϴ� ��쿡�� �޽��� �ﰢ���鿡 ����
	// ������ �� �ﰢ�� ���� ������ �����Ѵ�.
	//
	// �����ڿ� �������� �ʴ´ٸ� �������� �޽ÿ� ������ ���ɼ��� �����Ƿ�,
	// ������ �� �ﰢ�� ���� �������� �ð��� ������ ������ ����.

	// ���� �����ϴ� �ﰢ���� ã�� �������Ƿ� -1�� �ʱ�ȭ
	mPickedTriangle = -1;
	float tmin = 0.0f;
	if (XNA::IntersectRayAxisAlignedBox(rayOrigin, rayDir, &mMeshBox, &tmin)) // �������� AABB�� �浹�ߴٸ�
	{
		// �������� �����ϴ� ���� ����� �ﰢ���� ã�´�.
		tmin = MathHelper::Infinity;
		for (UINT i = 0; i < mMeshIndices.size() / 3; ++i)
		{
			// �ﰢ���� ����
			UINT i0 = mMeshIndices[i * 3 + 0];
			UINT i1 = mMeshIndices[i * 3 + 1];
			UINT i2 = mMeshIndices[i * 3 + 2];

			// ������ ���� ���� ȹ��
			XMVECTOR v0 = XMLoadFloat3(&mMeshVertices[i0].Pos);
			XMVECTOR v1 = XMLoadFloat3(&mMeshVertices[i1].Pos);
			XMVECTOR v2 = XMLoadFloat3(&mMeshVertices[i2].Pos);

			// �����ϴ� ���� ����� �ﰢ���� ã�����ؼ� �޽��� ��� �ﰢ�� ����
			float t = 0.0f;
			if (XNA::IntersectRayTriangle(rayOrigin, rayDir, v0, v1, v2, &t))
			{
				if (t < tmin) // ���� ���� �Ÿ��� ã��
				{
					// �� �ﰢ���� ������� ���� ����� �ﰢ���̴�.
					tmin = t;
					mPickedTriangle = i;
				}
			}
		}
	}
}