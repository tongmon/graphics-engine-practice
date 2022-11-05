#include <windows.h>
#include <cstdio>
#include <vector>
#include "GameDirector.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevIns, LPSTR lpCmdLine, int nCmdShow )
{
	return CGameDirector::getGameDirector( )->Loop( hInstance, lpCmdLine, nCmdShow );
}
