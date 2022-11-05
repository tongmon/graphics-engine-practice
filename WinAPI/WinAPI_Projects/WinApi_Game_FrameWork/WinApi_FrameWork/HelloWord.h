#pragma once
#include "Scene.h"

class HelloWord : public CScene
{
public:

	HelloWord( );
	~HelloWord( );

	void					create( );

	void					initialize( );

	void					update( float dt );
	void					render( HDC hdc, float dt );

	void					clear( );

	void					destroy( );

	void					OnMouseDown(WPARAM btnState, int x, int y);
	void					OnMouseUp(WPARAM btnState, int x, int y);

public:

	float x = 0, y = 0;
	bool show_img = false;
	float imgx = 200, imgy = 300;
};

