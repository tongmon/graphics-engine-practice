#include "PrepareWnd.h"
#include "GameDirector.h"

CPrepareWnd::CPrepareWnd()
{
	cbSize = sizeof(WNDCLASSEX);
	style = CS_HREDRAW | CS_VREDRAW;
	lpfnWndProc = nullptr;
	cbClsExtra = 0;
	cbWndExtra = 0;
	hIcon = nullptr;
	hCursor = LoadCursor(nullptr, IDC_ARROW);
	hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	lpszClassName = "winclass";
	lpfnWndProc = CGameDirector::WndProc;

	lpWindowName = "test";
	dwStyle = WS_OVERLAPPEDWINDOW;
	X = CW_USEDEFAULT;
	Y = CW_USEDEFAULT;
	nWidth = 800;
	nHeight = 600;
	hWndParent = nullptr;
	hMenu = nullptr;
	lpParam = nullptr;

	nCmdShow = SW_SHOW;
}
