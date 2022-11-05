#include "GameDirector.h"
#include "SceneManager.h"

CGameDirector::CGameDirector( ) : m_FixedFPS( 60 )
{
	m_SceneManager = new CSceneManager;
	m_AppPaused = false;
}

CGameDirector::~CGameDirector( )
{
	delete m_SceneManager;
}

LRESULT CALLBACK CGameDirector::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		getGameDirector()->OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		getGameDirector()->OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			getGameDirector()->setPausedState(true);
			getGameDirector()->Stop();
		}
		else
		{
			getGameDirector()->setPausedState(false);
			getGameDirector()->Start();
		}
		return 0;
	case WM_DESTROY:
			PostQuitMessage( 0 );
			return 0;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}

CGameDirector* CGameDirector::getGameDirector( )
{
	static CGameDirector dir;

	return &dir;
}

void CGameDirector::update( float dt )
{
	m_SceneManager->update( dt );
}

void CGameDirector::render( HDC hdc, float dt )
{
	HDC hMemDC;
	RECT windowRect;
	HBITMAP bitmap;

	GetClientRect( this->getWndHandle( ), &windowRect );

	hMemDC = CreateCompatibleDC( hdc );
	bitmap = CreateCompatibleBitmap( hdc, windowRect.right, windowRect.bottom );

	SelectObject( hMemDC, bitmap );
	FillRect( hMemDC, &windowRect, ( HBRUSH )COLOR_BACKGROUND );

	/*
	if(mGameState == MAIN)
	{
		m_SceneManager->reserveChangeScene( "메인 화면" )
	}
	else if(mGameState == LOADING)
	{
		m_SceneManager->reserveChangeScene( "로딩 화면" )
	}
	...
	*/

	m_SceneManager->render( hMemDC, dt );

	BitBlt( hdc, 0, 0, windowRect.right, windowRect.bottom, hMemDC, 0, 0, SRCCOPY );

	DeleteObject( bitmap );
	DeleteDC( hMemDC );
}

void CGameDirector::ProcessingLoop( )
{
	/*
	static DWORD prevFrameTime = 0;
	if( GetTickCount64( ) - prevFrameTime > m_FrameInterval )
	{			
		update( ( ( float )GetTickCount64( ) - ( float )prevFrameTime ) / 1000.f );
			
		HDC hdc = GetDC( m_hWnd );

		render( hdc, ( ( float )GetTickCount64( ) - ( float )prevFrameTime ) / 1000.f );

		ReleaseDC( m_hWnd, hdc );

		prevFrameTime = GetTickCount64( );
	}
	*/

	/*
	static int SkipTicks = 1000 / m_FixedFPS;
	static int SkipRate = 10;
	static DWORD NextTicks = GetTickCount64();
	static float dTime = 0;

	std::ostringstream buf;
	buf.precision(7);
	buf << " GetTickCount64: " << GetTickCount64() << "\n";
	OutputDebugString(buf.str().c_str());

	std::ostringstream buf1;
	buf1.precision(7);
	buf1 << " TotalTime: " << TotalTime() << "\n";
	OutputDebugString(buf1.str().c_str());

	if (!m_AppPaused) 
	{
		int Loops = 0;
		dTime += DeltaTime();
		while (GetTickCount64() >= NextTicks && Loops < SkipRate)
		{
			CalculateFrameStats();

			update(dTime);

			NextTicks += SkipTicks;
			Loops++;
			dTime = 0;
		}

		HDC hdc = GetDC(m_hWnd);

		render(hdc, DeltaTime());

		ReleaseDC(m_hWnd, hdc);
	}
	*/
	
	static float AccumlationTime = 0;

	if (m_AppPaused)
		return;

	AccumlationTime += DeltaTime();

	if (AccumlationTime > 1 / (float)m_FixedFPS)
	{
		CalculateFrameStats();
		
		update(AccumlationTime);

		HDC hdc = GetDC(m_hWnd);

		render(hdc, AccumlationTime);

		ReleaseDC(m_hWnd, hdc);

		AccumlationTime = 0.f;
	}
}

CSceneManager* CGameDirector::getSceneManager( )
{
	return m_SceneManager;
}

void CGameDirector::setPausedState(bool state)
{
	m_AppPaused = state;
}

void CGameDirector::OnMouseUp(WPARAM btnState, int x, int y)
{
	m_SceneManager->OnMouseUp(btnState, x, y);
	ReleaseCapture();
}

void CGameDirector::OnMouseDown(WPARAM btnState, int x, int y)
{
	m_SceneManager->OnMouseDown(btnState, x, y);
}

/*
void CGameDirector::setFrameInterval( DWORD interval )
{
	m_FrameInterval = interval;
}

DWORD CGameDirector::getFrameInterval( )
{
	return m_FrameInterval;
}
*/
