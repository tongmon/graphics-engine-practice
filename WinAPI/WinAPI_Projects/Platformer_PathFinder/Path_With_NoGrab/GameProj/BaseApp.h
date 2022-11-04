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
	HINSTANCE mhAppInst; // ���� ���α׷� �ν��Ͻ� �ڵ�
	HWND      mhMainWnd; // ���� ������ �ڵ�
	XFORM	  xForm; // ȭ�� ��ǥ ���� ����ü
	HDC		  mHDC; // ȭ�� ��� DC
	bool      mAppPaused; // ���� ���߾����� �Ǵ�
	bool      mMinimized; // ���� �ּ�ȭ �������� �Ǵ�
	bool      mMaximized; // ���� �ִ�ȭ �������� �Ǵ�
	bool      mResizing; // ����ڰ� ũ�� ������ �׵θ��� ����ϴ� �������� �Ǵ�

	float mRatio; // ȭ�� ���� ����

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
	void CalculateFrameStats(); // �ʴ� ������, ��� �����Ӵ� �и��ʸ� ����ϴ� �Լ�
	int mClientWidth;
	int mClientHeight;
	int mBaseWidth;
	int mBaseHeight;
	RECT mPrintWnd;
};