//***************************************************************************************
// Init Direct3D.cpp by Frank Luna (C) 2011 All Rights Reserved.
//
// Demonstrates the sample framework by initializing Direct3D, clearing 
// the screen, and displaying frame stats.
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

class InitDirect3DApp : public D3DApp
{
public:
	InitDirect3DApp(HINSTANCE hInstance);
	~InitDirect3DApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// ����� ������ ��� ������� �޸� ���� ����� �Ҵ�.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	InitDirect3DApp theApp(hInstance);

	if (!theApp.Init())
		return 0;

	return theApp.Run();
}

InitDirect3DApp::InitDirect3DApp(HINSTANCE hInstance)
	: D3DApp(hInstance)
{
}

InitDirect3DApp::~InitDirect3DApp()
{
}

bool InitDirect3DApp::Init()
{
	if (!D3DApp::Init())
		return false;

	return true;
}

void InitDirect3DApp::OnResize()
{
	D3DApp::OnResize();
}

void InitDirect3DApp::UpdateScene(float dt)
{

}

void InitDirect3DApp::DrawScene()
{
	assert(md3dImmediateContext);
	assert(mSwapChain);

	// �ĸ� ���۸� �Ķ������� �����. Colors::Blue�� d3dUtil.h�� ���ǵǾ� �ִ�.
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Blue));
	// ���� ���۸� 1.0f��, ���ٽ� ���۸� 0���� �����.
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// �ĸ� ���۸� ȭ�鿡 �����Ѵ�.
	HR(mSwapChain->Present(0, 0));
}