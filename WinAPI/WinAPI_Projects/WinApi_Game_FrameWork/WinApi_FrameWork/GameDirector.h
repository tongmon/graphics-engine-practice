#pragma once

#include "WrappedWnd.h"

class CSceneManager;

class CGameDirector : public CWrappedWnd
{
private:

	CGameDirector();
	~CGameDirector();

public:

	static CGameDirector* getGameDirector();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:

	virtual void ProcessingLoop() override;

	void update(float dt);
	void render(HDC hdc, float dt);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseWheel(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:

	CSceneManager* m_SceneManager;

public:

	CSceneManager* getSceneManager();

public:

	// void					setFrameInterval( DWORD interval );
	// DWORD				getFrameInterval( );
	void setPausedState(bool state);

private:

	// DWORD				m_FrameInterval; // 프레임 사이 간격
	bool m_AppPaused;
	int m_FixedFPS; // 고정 프레임
};
