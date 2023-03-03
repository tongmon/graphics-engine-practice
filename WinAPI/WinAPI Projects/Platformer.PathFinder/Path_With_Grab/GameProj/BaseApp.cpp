#include "BaseApp.h"

BaseApp::BaseApp()
{
    mhMainWnd = NULL;
	mhAppInst = NULL; // ���� ���α׷� �ν��Ͻ� �ڵ�
	mAppPaused = false; // ���� ���߾����� �Ǵ�
	mMinimized = false; // ���� �ּ�ȭ �������� �Ǵ�
	mMaximized = false; // ���� �ִ�ȭ �������� �Ǵ�
	mResizing = false; // ����ڰ� ũ�� ������ �׵θ��� ����ϴ� �������� �Ǵ�
	mClientWidth = mClientHeight = 0;
    mBaseWidth = 1280;
    mBaseHeight = 720;

	xForm.eM11 = (FLOAT)1; // x ���� �׷���
	xForm.eM12 = (FLOAT)0.0;
	xForm.eM21 = (FLOAT)0.0;
	xForm.eM22 = (FLOAT)-1; // y �ุ ������ �ݴ� ����
	xForm.eDx = (FLOAT)0.0;
	xForm.eDy = (FLOAT)0.0;
}

BaseApp::~BaseApp()
{
	ReleaseDC(mhMainWnd, mHDC);
}

HRESULT BaseApp::AppInit(HINSTANCE hIn, HWND hWnd)
{
	mhAppInst = hIn;
	mhMainWnd = hWnd;
	mClientWidth = mBaseWidth;
	mClientHeight = mBaseHeight;

	// ��ǥ�踦 �����ϴ��� (0, 0)�̰� y�� ���ΰ� ����, x�� ������ ������ �����.
	mHDC = GetDC(mhMainWnd);
	SetGraphicsMode(mHDC, GM_ADVANCED);
	SetWorldTransform(mHDC, &xForm);
	SetViewportOrgEx(mHDC, 0, mBaseHeight, NULL);

	/*
	// MM_LOENGLISH, MM_LOMETRIC ��ǥ�踦 ���� ��ǥ��� ��ȯ
	// ���� ����� �ƴ� ���� ������ε� ������ �ȴ�.
	SetMapMode(g_hDC, MM_LOENGLISH);
	SetViewportOrgEx(g_hDC, 0, mBaseHeight, NULL);
	*/

	Resize();
    OnInit();
    return S_OK;
}

void BaseApp::Render()
{
	HDC bufferDC = CreateCompatibleDC(mHDC); // bufferDC�� ��ȭ�� ����

	HBITMAP hbitmap = CreateCompatibleBitmap(mHDC, mBaseWidth, mBaseHeight);
	HBITMAP oldbitmap = (HBITMAP)SelectObject(bufferDC, hbitmap);
	
	OnRender(bufferDC);
	SetViewportOrgEx(mHDC, 0, mClientHeight, NULL);

    StretchBlt(mHDC, mPrintWnd.left, -mPrintWnd.top, mClientWidth, mClientHeight, bufferDC, 0, 0, mBaseWidth, mBaseHeight, SRCCOPY);

	DeleteObject(SelectObject(bufferDC, oldbitmap)); // ���� ������� �� �� ����
	DeleteDC(bufferDC); // hMemDC ����
}

void BaseApp::Cleanup()
{
    OnRelease();
}

void BaseApp::Update()
{
    OnUpdate();
}

void BaseApp::Resize()
{
	float ratioX = mClientWidth / (float)mBaseWidth;
	float ratioY = mClientHeight / (float)mBaseHeight;

	float ratio = min(ratioX, ratioY);

	mClientWidth = mBaseWidth * ratio;
	mClientHeight = mBaseHeight * ratio;

	mRatio = ratio;

	GetClientRect(mhMainWnd, &mPrintWnd);

	mPrintWnd.left = (mPrintWnd.right - mClientWidth) / 2;
	mPrintWnd.top = (mPrintWnd.bottom - mClientHeight) / 2;

	OnResize();
}

void BaseApp::CalculateFrameStats()
{
	// ��� FPS ���
	// �ϳ��� �������� �������ϴ� �� �ɸ��� ��� �ð��� ���
	// �� ���ġ���� â�� �����ٿ� �߰�

	// ���� ������ �ѹ��� �ʱ�ȭ�ȴ�.
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// 1�� ���� ��� ������ �� ���
	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << L"Tongstar's PathFinder" << L"    " // ���� �����鿡�Դ� �ʴ� �����Ӽ��� �������̰�����
			<< L"FPS: " << fps << L"    " // �����ڿ��Դ� �����Ӵ� �ʰ� ������ �� �ȴ�.
			<< L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(mhMainWnd, outs.str().c_str());

		// ���� �������� ���ϱ� ���� �ٽ� �ʱ�ȭ
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}