#pragma once

#include <Windows.h>

class CScene
{
public:

	CScene( ) = default;
	virtual ~CScene( ) = default;

public:

	virtual					void create( ) = 0;

	virtual					void initialize( ) = 0;

	virtual					void update( float dt ) = 0;
	virtual					void render( HDC hdc, float dt ) = 0;

	virtual					void clear( ) = 0;

	virtual					void destroy( ) = 0;

	// 마우스 컨드롤
	virtual					void OnMouseUp(WPARAM btnState, int x, int y) = 0;
	virtual					void OnMouseDown(WPARAM btnState, int x, int y) = 0;
	// virtual				void OnMouseWheel(WPARAM btnState, int x, int y) = 0;
	// virtual				void OnMouseMove(WPARAM btnState, int x, int y) = 0;
};
