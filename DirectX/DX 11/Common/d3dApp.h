#pragma once

#include "d3dUtil.h"
#include "GameTimer.h"
#include <string>

class D3DApp
{
public:
	D3DApp(HINSTANCE hInstance);
	virtual ~D3DApp();

	HINSTANCE AppInst()const; // ���� ���α׷� �ν��Ͻ� �ڵ��� ���纻�� �����ִ� �Լ�
	HWND      MainWnd()const; // ���� ������ �ڵ��� ���纻�� �����ִ� �Լ�
	float     AspectRatio()const; // ���̿� ���� �ʺ��� ������ �������ִ� �Լ�(��Ⱦ��)

	int Run(); // ������ ����, ���� ���� ���� ������� ���� �ȴ�.

	// ������ ��ũ �޼����� �Ļ� Ŭ���̾�Ʈ Ŭ������ �� �޼������ �Ը���� �������Ͽ� ����ȴ�.

	// �� �״�� �ʱ�ȭ �Լ�, ������ â �ʱ�ȭ, DX �ʱ�ȭ �̿ܿ��� �ڽ� Ŭ�������� �������ؼ� �ʱ�ȭ �������� ���� ������ �ȴ�.
	virtual bool Init();
	// â ũ�� �������ÿ� ���� ����, ����_���Ľ� ������ ũ�� ���� ������ ���־�� �ϴµ� �̸� ���
	virtual void OnResize();
	virtual void UpdateScene(float dt) = 0; // �� �״�� ���� ���� ��ȭ�� �̷������ ������Ʈ �Լ�
	virtual void DrawScene() = 0; // ���� ���̴� ���� �����Ǵ� �׸��� �Լ�
	// Ư�� �޽����� �����쿡�� �ñ��� �ʰ� ���� �������α׷����� ���� ó���ϰ� ���� ��� ȣ�� �ϴ� �Լ�
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// ���콺 �Է� ó���� ���� ���ǿ� ���� �޼����
	virtual void OnMouseDown(WPARAM btnState, int x, int y) { }
	virtual void OnMouseUp(WPARAM btnState, int x, int y) { }
	virtual void OnMouseMove(WPARAM btnState, int x, int y) { }

protected:
	bool InitMainWindow(); // WIN32 ���� ������ �ʱ�ȭ
	bool InitDirect3D(); // Direct3D 11�� �ʱ�ȭ �ϴ� �Լ�

	void CalculateFrameStats(); // �ʴ� ������, ��� �����Ӵ� �и��ʸ� ����ϴ� �Լ�

protected:

	HINSTANCE mhAppInst; // ���� ���α׷� �ν��Ͻ� �ڵ�
	HWND      mhMainWnd; // ���� ������ �ڵ�
	bool      mAppPaused; // ���� ���߾����� �Ǵ�
	bool      mMinimized; // ���� �ּ�ȭ �������� �Ǵ�
	bool      mMaximized; // ���� �ִ�ȭ �������� �Ǵ�
	bool      mResizing; // ����ڰ� ũ�� ������ �׵θ��� ����ϴ� �������� �Ǵ�
	UINT      m4xMsaaQuality; // 4X MSAA ���ϸ���� ǰ�� ����

	GameTimer mTimer; // ���� �ð� ���� Ŭ���� ����

	// D3D11 ��ġ, ���� ��ȯ ��ȯ �罽
	// ����_���ٽ� ���۸� ���� 2���� �ؽ���, ���� ���� 2���� �ؽ���
	// ����_���ٽ� ��, ����Ʈ... � ���� ����
	ID3D11Device* md3dDevice;
	ID3D11DeviceContext* md3dImmediateContext;
	IDXGISwapChain* mSwapChain;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11DepthStencilView* mDepthStencilView;
	D3D11_VIEWPORT mScreenViewport;

	// �������� GPU���� ���� �۵��ϴ� �ɼ��� D3D_DRIVER_TYPE_HARDWARE �� Ÿ���� �����Ѵ�.
	std::wstring mMainWndCaption;
	D3D_DRIVER_TYPE md3dDriverType;
	int mClientWidth;
	int mClientHeight;
	bool mEnable4xMsaa;
};