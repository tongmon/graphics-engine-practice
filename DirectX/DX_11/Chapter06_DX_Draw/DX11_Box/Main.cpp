//***************************************************************************************
// BoxDemo.cpp by Frank Luna (C) 2011 All Rights Reserved.
//
// Demonstrates rendering a colored box.
//
// Controls:
//		Hold the left mouse button down and move the mouse to rotate.
//      Hold the right mouse button down to zoom in and out.
//
//***************************************************************************************

#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "MathHelper.h"

#pragma comment (lib,"d3d11.lib")
#pragma comment (lib,"d3dx11d.lib")
#pragma comment (lib,"D3DCompiler.lib")
#pragma comment (lib,"Effects11d.lib")
#pragma comment (lib,"dxgi.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib,"winmm.lib")

struct Vertex // ���� ����
{
	XMFLOAT3 Pos; // ��ġ
	XMFLOAT4 Color; // ����
};

class BoxApp : public D3DApp
{
public:
	BoxApp(HINSTANCE hInstance);
	~BoxApp();

	bool Init(); // ���� ������ �ʱ�ȭ
	void OnResize(); // â ����
	void UpdateScene(float dt); // ȭ�� ����
	void DrawScene(); // �׸���

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	void BuildGeometryBuffers(); // ���� ���� �Լ�
	void BuildFX();
	void BuildVertexLayout();

private:
	ID3D11Buffer* mBoxVB; // ���� ����
	ID3D11Buffer* mBoxIB; // �ε��� ����

	ID3DX11Effect* mFX;
	ID3DX11EffectTechnique* mTech;
	ID3DX11EffectMatrixVariable* mfxWorldViewProj;

	ID3D11InputLayout* mInputLayout;

	XMFLOAT4X4 mWorld; // ���� ���, SRT
	XMFLOAT4X4 mView; // ī�޶� ��ġ
	XMFLOAT4X4 mProj; // ���� ���

	float mTheta;
	float mPhi;
	float mRadius;

	POINT mLastMousePos;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// ����� ���� ��忡�� ������� �޸� ���� ��� Ȱ��ȭ
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	BoxApp theApp(hInstance);

	if (!theApp.Init()) // �ʱ�ȭ
		return 0;

	return theApp.Run(); // �̺�Ʈ ���� �۵�
}

BoxApp::BoxApp(HINSTANCE hInstance)
	: D3DApp(hInstance), mBoxVB(0), mBoxIB(0), mFX(0), mTech(0),
	mfxWorldViewProj(0), mInputLayout(0),
	mTheta(1.5f * MathHelper::Pi), mPhi(0.25f * MathHelper::Pi), mRadius(5.0f)
{
	mMainWndCaption = L"Box Demo";

	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	// XMStoreFloat4x4 ---> XMMATRIX ���� XMFloat4x4 ���� ����
	XMMATRIX I = XMMatrixIdentity(); // ���� ��ķ� �ʱ�ȭ
	XMStoreFloat4x4(&mWorld, I);
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);
}

BoxApp::~BoxApp()
{
	ReleaseCOM(mBoxVB);
	ReleaseCOM(mBoxIB);
	ReleaseCOM(mFX);
	ReleaseCOM(mInputLayout);
}

bool BoxApp::Init() // �ʱ�ȭ
{
	if (!D3DApp::Init()) // D3DApp���� �ʱ�ȭ�� ���� �Լ��� �ִ�.
		return false;

	BuildGeometryBuffers(); // ���� ����
	BuildFX(); // FX ���� �о ���̵�
	BuildVertexLayout(); // �Է� ��ġ ���� �Լ�

	return true;
}

void BoxApp::OnResize()
{
	D3DApp::OnResize();

	// â ũ�� ���濡 ���� ��Ⱦ�� ����, �ٽ� ��� --> AspectRatio()
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f); // ������� ����
	XMStoreFloat4x4(&mProj, P); // ���� ��� ����
}

void BoxApp::UpdateScene(float dt)
{
	// ���� ��ǥ��� (r,��,��)�� ǥ��
	// r = ������
	// �� = y���� ���� �������κ��� ������ ������ ���밡 �̷�� ���������� ��
	// �� = x���� ���� �������κ��� ������ ������ ���밡 �̷�� ������ xz��鿡 ������Ų ���������� ��
	// mRadius = r, mPhi = ��, mTheta = ��
	// �� ��ǥ�谡 �ձ׷� ���� ���� �� �ȿ��� �������� ������ ��ǥ�� ǥ���ϰ� �ȴ�.
	// �̸� ���� ��ǥ��� ��ȯ�ϱ� ���� �ذ� ���� ������ ����Ѵ�.
	float x = mRadius * sinf(mPhi) * cosf(mTheta);
	float z = mRadius * sinf(mPhi) * sinf(mTheta);
	float y = mRadius * cosf(mPhi);

	// �þ� ��� ����
	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f); // ī�޶� ��ġ, ��ǥ�ϱ� 1.0f
	XMVECTOR target = XMVectorZero(); // ������ �ٶ�
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // ���� ī�޶� ���� ����(���� ����)

	XMMATRIX V = XMMatrixLookAtLH(pos, target, up); // �þ���� ����
	XMStoreFloat4x4(&mView, V); // mView = �þ� ���
}

void BoxApp::DrawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::LightSteelBlue)); // ���� ���� ����
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0); // ���� ����

	md3dImmediateContext->IASetInputLayout(mInputLayout); // �Է� ��ġ�� ����ϰ��� �ϴ� ��ġ�� ����
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �ﰢ ����Ʈ ������ �������

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mBoxVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mBoxIB, DXGI_FORMAT_R32_UINT, 0); // �ε��� ���۵��� �Է� ������ �ܰ迡 ����

	// ��ȯ ����� ����
	XMMATRIX world = XMLoadFloat4x4(&mWorld);
	XMMATRIX view = XMLoadFloat4x4(&mView);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);
	XMMATRIX worldViewProj = world * view * proj;

	mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		mTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);

		// 36���� �ڽ� �ε��� ���۵�
		md3dImmediateContext->DrawIndexed(36, 0, 0); // ���� �ε��� ����, ù ���� ��ġ, ������ ������ ���� ������ ������ �׸���
	}

	HR(mSwapChain->Present(0, 0)); // ���� ���۸��� ���� ü��
}

void BoxApp::OnMouseDown(WPARAM btnState, int x, int y) // ���콺 ��ư ������
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void BoxApp::OnMouseUp(WPARAM btnState, int x, int y) // ���콺 ��ư �� ��
{
	ReleaseCapture();
}

void BoxApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0) // ���� Ŭ������ ī�޶� ��ġ �̵�
	{
		// 1�ȼ��� 4���� 1�� (Degree ����) �� �ǰ� �Ѵ�.
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePos.y));

		// ���콺 �Է¿� ������ ������ ���� �ֺ��� �˵� ī�޶� ����
		mTheta += dx;
		mPhi += dy;

		// mPhi(��� �������δ� ���� �� �����̴� ��)�� ���� ������ 0 ~ 180���� �����ϴ� ��.
		mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi - 0.1f);
	}
	else if ((btnState & MK_RBUTTON) != 0) // ���� Ŭ������ ī�޶� Ȯ�� ���
	{
		// 1�ȼ��� ����� 0.005 ������ �ǰ� �Ѵ�.
		float dx = 0.005f * static_cast<float>(x - mLastMousePos.x);
		float dy = 0.005f * static_cast<float>(y - mLastMousePos.y);

		// ���콺 �Է¿� �����Ͽ� �˵� ī�޶��� �������� ����
		mRadius += dx - dy;

		// �������� ������ 3 ~ 15�� ������ �д�.
		mRadius = MathHelper::Clamp(mRadius, 3.0f, 15.0f);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void BoxApp::BuildGeometryBuffers() // ���� ���� �Լ�
{
	// ������ ����, ������ d3dUtil.h�� ���ǵ�
	Vertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), (const float*)&Colors::White   },
		{ XMFLOAT3(-1.0f, +1.0f, -1.0f), (const float*)&Colors::Black   },
		{ XMFLOAT3(+1.0f, +1.0f, -1.0f), (const float*)&Colors::Red     },
		{ XMFLOAT3(+1.0f, -1.0f, -1.0f), (const float*)&Colors::Green   },
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f), (const float*)&Colors::Blue    },
		{ XMFLOAT3(-1.0f, +1.0f, +1.0f), (const float*)&Colors::Yellow  },
		{ XMFLOAT3(+1.0f, +1.0f, +1.0f), (const float*)&Colors::Cyan    },
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f), (const float*)&Colors::Magenta }
	};

	D3D11_BUFFER_DESC vbd;
	// ������ D3D11_USAGE_DEFAULT�� �ؼ� GPU�� �ڿ��� �а� ���� ������ 
	// ���⼱ ����ȭ�� ���� ���� ���� �Ŀ� ���� ������ ���� D3D11_USAGE_IMMUTABLE�� ���
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * 8; // ������ ���� ������ ũ��
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // ���� ���۴� ������ �̰� ����.
	vbd.CPUAccessFlags = 0; // CPU�� ���� ���� ��꿡 �������� �����Ƿ� 0
	vbd.MiscFlags = 0; // ���� ���ۿ����� �̰� ������ 0
	vbd.StructureByteStride = 0; // ������ ���� ��� ���ϸ� 0
	D3D11_SUBRESOURCE_DATA vinitData; 
	vinitData.pSysMem = vertices;
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mBoxVB)); // vbd(���� ���� �Ӽ�), vinitData(���� �ʱ�ȭ), mBoxVB(������ ������ ���)

	// �ε��� ���� ����, �ð������ DX���� �ո�
	UINT indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	// �ε��� ���� Ư�� ����, ����� ���� ���ۿ� ���
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 36; // 36���� �ε���
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // �ε��� ���۴� �̷��� ����
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData; // ���� ���� �ʱ�ȭ�� �ڷḦ ����
	iinitData.pSysMem = indices;
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mBoxIB)); // ���� ���� ����, HR�� ����� �뵵
}

void BoxApp::BuildFX()
{
	DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	HRESULT hr = D3DX11CompileFromFile(L"FX/color.fx", 0, 0, 0, "fx_5_0", shaderFlags,
		0, 0, &compiledShader, &compilationMsgs, 0);

	// compilationMsgs�� ���� �޽����� ��� �޽����� ����Ǿ� ���� �� �ִ�.
	if (compilationMsgs != 0)
	{
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, 0);
		ReleaseCOM(compilationMsgs);
	}

	// compilationMsgs�� ���� �޽����� ���ٰ� �ص� ��������� ������ ���� ���θ� �˻�
	if (FAILED(hr))
	{
		DXTrace((WCHAR*)__FILEW__, (DWORD)__LINE__, hr, L"D3DX11CompileFromFile", true);
	}

	HR(D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(),
		0, md3dDevice, &mFX)); // ����Ʈ�� mFX�� ����

	// �����ϵ� ���̴��� �� ����ؼ� ������
	ReleaseCOM(compiledShader);

	mTech = mFX->GetTechniqueByName("ColorTech"); // fx ������ ���� ColorTech��� �̸��� ��ũ�� ����ü�� �ִ�. �̸� �������� ��
	mfxWorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix(); // fx ���Ͽ��� gWorldViewProj ������ ��� ���·� ȹ����
}

void BoxApp::BuildVertexLayout()
{
	// ���� �Է� ��ġ �����
	// �Է� �������� ������ �˾ƾ� �Ѵ�.
	// ���п� �ο��� �̸�, ����(������ 0������ �ϳ��� ������ ������ �ؽ��İ� ��������� ������ ��ȿ������.)
	// , ������ �ڷ� ����(���� DXGI_FORMAT_R32G32B32_FLOAT�� 3���� 32��Ʈ �ε��Ҽ����� �ǹ�)
	// , �Է� ���� ����(��ġ�� ������ ���� �ɰ��� ������ �Ѵٸ� ���)
	// , ����Ʈ ������ ������(���� Ư���� ��ġ�� 3���� float���̶� 12���� �����Ͽ� 12�� ����)
	// , ������ 2���� �Ӽ��� �ϴ� ����
	// �ڼ��� ������ å 212p ����
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// �Է� ��ġ ��ü ����
	D3DX11_PASS_DESC passDesc; // �н��� �����ϴ� ����ü, �Է� ����
	// Effect ��ü���� �н��� �ְ� �ش� �н����� ������ ���� ���̴��� ����
	// ȿ�� ��ü�κ��� �н��� �����ϴ� ����ü ȹ�� ����
	mTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(md3dDevice->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &mInputLayout)); // ������ �Է� ��ġ�� mInputLayout ���⿡ ���� �ȴ�.
}