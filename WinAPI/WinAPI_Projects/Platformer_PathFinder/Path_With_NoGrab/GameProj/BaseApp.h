#pragma once
#include <windows.h>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "GameTimer.h"

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

class BaseApp
{
public:
	GameTimer mTimer;
	HINSTANCE mhAppInst; // 응용 프로그램 인스턴스 핸들
	HWND      mhMainWnd; // 메인 윈도우 핸들
	XFORM	  xForm; // 화면 좌표 조정 구조체
	HDC		  mHDC; // 화면 출력 DC
	bool      mAppPaused; // 앱이 멈추었는지 판단
	bool      mMinimized; // 앱이 최소화 상태인지 판단
	bool      mMaximized; // 앱이 최대화 생태인지 판단
	bool      mResizing; // 사용자가 크기 조정용 테두리를 사용하는 상태인지 판단

	float mRatio; // 화면 조정 비율

protected:
	virtual void OnInit() = 0;
	virtual void OnRender(HDC Paper) = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRelease() = 0;
	virtual void OnResize() = 0;

public:
	BaseApp(void);
	~BaseApp(void);
	HRESULT AppInit(HINSTANCE hIn, HWND hWnd);
	void Render();
	void Update();
	void Resize();
	void Cleanup();
	void CalculateFrameStats(); // 초당 프레임, 평균 프레임당 밀리초를 계산하는 함수
	int mClientWidth;
	int mClientHeight;
	int mBaseWidth;
	int mBaseHeight;
	RECT mPrintWnd;
};