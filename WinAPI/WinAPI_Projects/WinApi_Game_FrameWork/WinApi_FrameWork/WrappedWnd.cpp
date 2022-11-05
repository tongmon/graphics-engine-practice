#include "WrappedWnd.h"
#include "PrepareWnd.h"

void settingWnd( CPrepareWnd& value );
void settingGame( );

CWrappedWnd::CWrappedWnd( ) : m_hWnd( nullptr ), m_hInstance( nullptr )
{
}

int CWrappedWnd::Loop( HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow )
{
	m_hInstance = hInstance;

	CPrepareWnd value;

	value.hInstance = m_hInstance;
	value.nCmdShow = nCmdShow;

	settingWnd( value );
	m_Title = value.lpWindowName;

	this->registerWndClass( value );
	this->createWindow( value );
	this->showWindow( value );

	return this->MessageLoop( );
}

void CWrappedWnd::registerWndClass( const CPrepareWnd& value )
{
	WNDCLASSEX wcex = { 0, };

	wcex.cbSize = sizeof ( wcex );
	wcex.style = value.style;
	wcex.hInstance = value.hInstance;
	wcex.hCursor = value.hCursor;
	wcex.hbrBackground = value.hbrBackground;
	wcex.lpszClassName = value.lpszClassName;
	wcex.lpfnWndProc = value.lpfnWndProc;
	RegisterClassEx( &wcex );
}

void CWrappedWnd::createWindow( const CPrepareWnd& value )
{
	m_hWnd = CreateWindow( 
		value.lpszClassName, value.lpWindowName, value.dwStyle, 
		value.X, value.Y, value.nWidth, value.nHeight,
		value.hWndParent, value.hMenu, value.hInstance, value.lpParam );
}

void CWrappedWnd::showWindow( const CPrepareWnd& value )
{
	ShowWindow( m_hWnd, value.nCmdShow );
	UpdateWindow( m_hWnd );
}

int CWrappedWnd::MessageLoop( )
{
	MSG msg;
	memset( &msg, 0, sizeof( msg ) );

	settingGame( );
	Reset(); // 타이머 초기화

	while( msg.message != WM_QUIT )
	{
		if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			Tick();
			this->ProcessingLoop( );
		}
	}

	return ( int )msg.wParam;
}

void CWrappedWnd::CalculateFrameStats()
{
	// 평균 FPS 계산
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// 1초 동안 평균 프레임 수 계산
	if ((TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::ostringstream Title;
		Title.precision(6);
		Title << m_Title << "    " << "FPS: " << fps << "   "
			<< "Frame Time: " << mspf << "(ms)";

		SetWindowText(m_hWnd, Title.str().c_str());

		// 다음 프레임을 구하기 위해 다시 초기화
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

HWND CWrappedWnd::getWndHandle( )
{
	return m_hWnd;
}

HINSTANCE CWrappedWnd::getInstanceHandle( )
{
	return m_hInstance;
}
