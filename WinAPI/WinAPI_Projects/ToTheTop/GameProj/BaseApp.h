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

typedef enum _Game_State // ���� ���� �����Ǿ��ִ� ������ �κе��� �������� ���� ������
{
	INIT, MAINMENU, HOWTO, OPTION, RUN, STOP, SUCCESS, FAILED, END, GAMEOVER,
}GAMESTATE;

class BaseApp : public GameSprite
{
public:
	GameTimer mTimer;
	HINSTANCE mhAppInst; // ���� ���α׷� �ν��Ͻ� �ڵ�
	HWND      mhMainWnd; // ���� ������ �ڵ�
	XFORM	  xForm; // ȭ�� ��ǥ ���� ����ü
	HDC		  mHDC; // ȭ�� ��� ���� DC
	HDC		  mPaperDC; // ȭ�鿡 �׸� ��ȭ�� DC
	POINT	  mBitMapSize; // ȭ�鿡 ǥ���� ��Ʈ�� ũ��
	RECT	  mPaperRect; // ȭ�鿡 ǥ���� ��Ʈ�� ����
	GAMESTATE mGameState; // ���� ����
	GAMESTATE mPrevGameState; // ���� ���� ����
	bool      mAppPaused; // ���� ���߾����� �Ǵ�
	bool      mMinimized; // ���� �ּ�ȭ �������� �Ǵ�
	bool      mMaximized; // ���� �ִ�ȭ �������� �Ǵ�
	bool      mResizing; // ����ڰ� ũ�� ������ �׵θ��� ����ϴ� �������� �Ǵ�

	// ���� ��ư
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

	float mRatio; // ȭ�� ���� ����

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
	void CalculateFrameStats(); // �ʴ� ������, ��� �����Ӵ� �и��ʸ� ����ϴ� �Լ�
};