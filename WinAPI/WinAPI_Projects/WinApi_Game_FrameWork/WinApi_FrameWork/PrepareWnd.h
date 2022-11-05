#pragma once

#include <windows.h>

class CPrepareWnd
{
public:

	CPrepareWnd( );
	~CPrepareWnd( ) = default;

public: // WNDCLASSEX

	UINT					cbSize;
	UINT					style;
	WNDPROC					lpfnWndProc;
	int						cbClsExtra;
	int						cbWndExtra;
	HINSTANCE				hInstance;
	HICON					hIcon;
	HCURSOR					hCursor;
	HBRUSH					hbrBackground;
	LPCSTR					lpszMenuName;
	LPCSTR					lpszClassName;
	
public: // FOR CREATEWINDOW

	LPCSTR					lpWindowName;
	DWORD					dwStyle;
	int						X;
	int						Y;
	int						nWidth;
	int						nHeight;
	HWND					hWndParent;
	HMENU					hMenu;
	LPVOID					lpParam;

public: // FOR SHOWWINDOW

	int						nCmdShow;

};
