#pragma once
#include <windows.h>
#include <string>
#include <sstream>
#include "GameTimer.h"
#include "GameBot.h"
#include "GameSprite.h"
#include "Constants.h"

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

typedef enum _Game_State // 따로 따로 구현되어있는 게임의 부분들을 연결해줄 상태 변수들
{
	INIT, MAINMENU, HOWTO, OPTION, RUN, STOP, SUCCESS, FAILED, END, GAMEOVER,
}GAMESTATE;

class BaseApp : public GameSprite
{
public:
	GameTimer mTimer;
	HINSTANCE mhAppInst; // 응용 프로그램 인스턴스 핸들
	HWND      mhMainWnd; // 메인 윈도우 핸들
	XFORM	  xForm; // 화면 좌표 조정 구조체
	HDC		  mHDC; // 화면 출력 메인 DC
	HDC		  mPaperDC; // 화면에 그릴 도화지 DC
	POINT	  mBitMapSize; // 화면에 표시할 비트맵 크기
	RECT	  mPaperRect; // 화면에 표시할 비트맵 영역
	GAMESTATE mGameState; // 게임 상태
	GAMESTATE mPrevGameState; // 예전 게임 상태
	bool      mAppPaused; // 앱이 멈추었는지 판단
	bool      mMinimized; // 앱이 최소화 상태인지 판단
	bool      mMaximized; // 앱이 최대화 생태인지 판단
	bool      mResizing; // 사용자가 크기 조정용 테두리를 사용하는 상태인지 판단

	// 각종 버튼
	GameObj mPlayButton;
	GameObj mHowToPlayButton;
	GameObj mOptionButton;
	GameObj mQuitButton;
	GameObj mPauseButton;
	GameObj mBackButton;
	GameObj mResumeButton;
	GameObj mHomeButton;

	int mClientWidth;
	int mClientHeight;
	int mBaseWidth;
	int mBaseHeight;
	RECT mPrintWnd;

	float mRatio; // 화면 조정 비율

protected:
	virtual void OnInit() = 0;
	virtual void OnRender() = 0;
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
};