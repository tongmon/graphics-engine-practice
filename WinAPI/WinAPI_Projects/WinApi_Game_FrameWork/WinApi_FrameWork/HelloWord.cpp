#include "HelloWord.h"
#include "Bitmap.h"

HelloWord::HelloWord()
{
}

HelloWord::~HelloWord()
{
}

void HelloWord::create()
{
}

void HelloWord::initialize()
{
}

void HelloWord::update(float dt)
{
	if (GetAsyncKeyState(VK_LEFT) && 0x8000)
	{
		x -= 160 * dt;
	}
	if (GetAsyncKeyState(VK_RIGHT) && 0x8000)
	{
		x += 160 * dt;
	}
	if (GetAsyncKeyState(VK_UP) && 0x8000)
	{
		y -= 160 * dt;
	}
	if (GetAsyncKeyState(VK_DOWN) && 0x8000)
	{
		y += 160 * dt;
	}
}

void HelloWord::render(HDC hdc, float dt)
{
	CBitmap bitmap("Resources/image3.bmp");
	bitmap.drawBitmap(hdc, x, y, 0, 0);

	if (show_img)
		bitmap.drawBitmap(hdc, imgx, imgy, 50, 50);
}

void HelloWord::OnMouseDown(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0) // 좌측 클릭
	{
		show_img = true;
	}
	else if ((btnState & MK_RBUTTON) != 0) // 우측 클릭
	{

	}
}

void HelloWord::OnMouseUp(WPARAM btnState, int x, int y)
{
	show_img = false;
}

void HelloWord::clear()
{
}

void HelloWord::destroy()
{
}