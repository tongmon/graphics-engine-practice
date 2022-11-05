#pragma once

#include "GameTimer.h"
#include <Windows.h>
#include <sstream>

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

class CPrepareWnd;

class CWrappedWnd : public GameTimer
{
protected:

	CWrappedWnd( );
	~CWrappedWnd( ) = default;

public:

	int						Loop( HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow );

private:

	void					registerWndClass( const CPrepareWnd& value );
	void					createWindow( const CPrepareWnd& value );
	void					showWindow( const CPrepareWnd& value );
	int						MessageLoop( );

protected:
	void					CalculateFrameStats(); // 프레임 시간 계산

public:

	virtual void			ProcessingLoop( ) = 0;

protected:

	HWND					m_hWnd;
	HINSTANCE				m_hInstance;
	std::string				m_Title;

public:

	void					setWndHandle( HWND hWnd );
	HWND					getWndHandle( );

	void					setInstanceHandle( HINSTANCE hInstance );
	HINSTANCE				getInstanceHandle( );

};
