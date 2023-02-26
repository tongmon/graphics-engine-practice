#include "BaseApp.h"

BaseApp::BaseApp()
{
    mhMainWnd = NULL;
	mhAppInst = NULL; // 응용 프로그램 인스턴스 핸들
	mAppPaused = false; // 앱이 멈추었는지 판단
	mMinimized = false; // 앱이 최소화 상태인지 판단
	mMaximized = false; // 앱이 최대화 생태인지 판단
	mResizing = false; // 사용자가 크기 조정용 테두리를 사용하는 상태인지 판단
	mClientWidth = mClientHeight = 0;
    mBaseWidth = cBaseWidth;
    mBaseHeight = cBaseHeight;

	mBitMapSize = { mBaseWidth, mBaseHeight };
	mPaperRect = { 0,0,mBaseWidth,mBaseHeight };
	mGameState = GAMESTATE::INIT;

	xForm.eM11 = (FLOAT)1; // x 축은 그래도
	xForm.eM12 = (FLOAT)0.0;
	xForm.eM21 = (FLOAT)0.0;
	xForm.eM22 = (FLOAT)-1; // y 축만 기존의 반대 방향
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

	// 좌표계를 좌측하단이 (0, 0)이고 y축 위로가 증가, x축 우측을 증가로 만든다.
	mHDC = GetDC(mhMainWnd);
	SetGraphicsMode(mHDC, GM_ADVANCED);
	SetWorldTransform(mHDC, &xForm);
	SetViewportOrgEx(mHDC, 0, mBaseHeight, NULL);

	/*
	// MM_LOENGLISH, MM_LOMETRIC 좌표계를 수학 좌표계로 변환
	// 위의 방식이 아닌 밑의 방식으로도 진행이 된다.
	SetMapMode(g_hDC, MM_LOENGLISH);
	SetViewportOrgEx(g_hDC, 0, mBaseHeight, NULL);
	*/

	Resize();
    OnInit();
    return S_OK;
}

void BaseApp::Render()
{
	mPaperDC = CreateCompatibleDC(mHDC); // mPaperDC가 도화지 역할

	HBITMAP hbitmap = CreateCompatibleBitmap(mHDC, mBitMapSize.x, mBitMapSize.y);
	HBITMAP oldbitmap = (HBITMAP)SelectObject(mPaperDC, hbitmap);
	
	OnRender();
	SetViewportOrgEx(mHDC, 0, mClientHeight, NULL);

	StretchBlt(mHDC, mPrintWnd.left, -mPrintWnd.top, mClientWidth, mClientHeight, mPaperDC, 
		mPaperRect.left, mPaperRect.top, mPaperRect.right, mPaperRect.bottom, SRCCOPY);

	DeleteObject(SelectObject(mPaperDC, oldbitmap)); // 종이 원래대로 한 후 제거
	DeleteDC(mPaperDC); // mPaperDC 제거
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

	mClientWidth = int(mBaseWidth * ratio);
	mClientHeight = int(mBaseHeight * ratio);

	mRatio = ratio;

	GetClientRect(mhMainWnd, &mPrintWnd);

	mPrintWnd.left = (mPrintWnd.right - mClientWidth) / 2;
	mPrintWnd.top = (mPrintWnd.bottom - mClientHeight) / 2;

	OnResize();
}

void BaseApp::CalculateFrameStats()
{
	// 평균 FPS 계산
	// 하나의 프레임을 렌더링하는 데 걸리는 평균 시간도 계산
	// 이 통계치들을 창의 제목줄에 추가

	// 전역 변수라 한번만 초기화된다.
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// 1초 동안 평균 프레임 수 계산
	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << L"9기 프로그래머 지원자: 이경준" << L"    " // 게임 유저들에게는 초당 프레임수가 가시적이겠지만
			<< L"FPS: " << fps << L"    " // 개발자에게는 프레임당 초가 도움이 더 된다.
			<< L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(mhMainWnd, outs.str().c_str());

		// 다음 프레임을 구하기 위해 다시 초기화
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}