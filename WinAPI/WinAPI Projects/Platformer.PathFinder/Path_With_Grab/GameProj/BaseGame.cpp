#include "BaseGame.h"

BaseGame::BaseGame()
{
	mLastMousePos = { -1,-1 };
}

BaseGame::~BaseGame()
{

}

void BaseGame::OnInit()
{
	mGround.InitMap();
	mPlayer.InitCharacter({ 400,100 }, { 1,1 }, &mGround);
	mNPC.InitBot({ 120, 120 }, { 1,1 }, &mGround);
	OnResize();
}

void BaseGame::OnRender(HDC Paper)
{
	RECT Wnd = { 0,mBaseHeight,mBaseWidth,0 };
	
	HBRUSH NewB = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255)); // 배경색 브러쉬
	HBRUSH OldB = (HBRUSH)SelectObject(Paper, NewB);
	FillRect(Paper, &Wnd, NewB); // 배경색 채우기

	mGround.GetHDC(Paper);
	mGround.DrawMap();

	mPlayer.GetHDC(Paper);
	mPlayer.DrawMovingObject();

	mNPC.GetHDC(Paper);
	mNPC.DrawMovingObject();

	SelectObject(Paper, OldB);
	DeleteObject(NewB);
}

void BaseGame::OnUpdate()
{
	float DeltaT = mTimer.DeltaTime(); // 델타 타임

	mPlayer.GetDeltaTime(DeltaT);
	mNPC.GetDeltaTime(DeltaT);

	std::vector<bool> Input(KeyInput::Count, false);

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		Input[(int)KeyInput::GoLeft] = true;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		Input[(int)KeyInput::GoRight] = true;
	}
	if (GetAsyncKeyState(VK_UP) < 0)
	{

	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		Input[(int)KeyInput::GoDown] = true;
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8001)
	{
		Input[(int)KeyInput::Jump] = true;
	}

	if (GetAsyncKeyState('S') < 0)
	{
		
	}

	mPlayer.CharacterUpdate(Input);
	mNPC.BotUpdate();
}

void BaseGame::OnRelease()
{
	
}

void BaseGame::OnResize()
{

}

void BaseGame::OnMouseDown(WPARAM btnState, int x, int y) // 마우스 버튼 누를때
{
	float mY = (mClientHeight - y + mPrintWnd.top) / mRatio;
	float mX = (x - mPrintWnd.left) / mRatio;
	POINT MapCord = mGround.GetMapTileAtPoint({ mX,mY });

	if ((btnState & MK_LBUTTON) != 0) // 좌측 클릭
	{
		mNPC.TappedOnTile(MapCord);
	}
	else if ((btnState & MK_RBUTTON) != 0) // 우측 클릭, 블록 배치
	{
		if (mGround.IsGround(MapCord.x, MapCord.y))
			mGround.SetTile(MapCord.x, MapCord.y, TileType::Empty);
		else
			mGround.SetTile(MapCord.x, MapCord.y, TileType::Block);
	}
	else if ((btnState & MK_MBUTTON) != 0)
	{
		if (mGround.IsGround(MapCord.x, MapCord.y))
			mGround.SetTile(MapCord.x, MapCord.y, TileType::Empty);
		else
			mGround.SetTile(MapCord.x, MapCord.y, TileType::OneWay);
	}
	
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void BaseGame::OnMouseUp(WPARAM btnState, int x, int y) // 마우스 버튼 땔 때
{
	ReleaseCapture();
}

void BaseGame::OnMouseMove(WPARAM btnState, int x, int y)
{
	float mY = (mClientHeight - y + mPrintWnd.top) / mRatio;
	float mX = (x - mPrintWnd.left) / mRatio;
	POINT MapCord = mGround.GetMapTileAtPoint({ mX,mY });

	float lastY = (mClientHeight - mLastMousePos.y + mPrintWnd.top) / mRatio;
	float lastX = (mLastMousePos.x - mPrintWnd.left) / mRatio;
	POINT LastCord = mGround.GetMapTileAtPoint({ lastX,lastY });
	
	if (LastCord.x == MapCord.x && LastCord.y == MapCord.y)
	{
		mLastMousePos.x = x;
		mLastMousePos.y = y;
		return;
	}

	if ((btnState & MK_LBUTTON) != 0) // 좌측 클릭
	{

	}
	else if ((btnState & MK_RBUTTON) != 0) // 우측 클릭, 블록 배치
	{
		if (mGround.IsGround(MapCord.x, MapCord.y))
			mGround.SetTile(MapCord.x, MapCord.y, TileType::Empty);
		else
			mGround.SetTile(MapCord.x, MapCord.y, TileType::Block);
	}
	else if ((btnState & MK_MBUTTON) != 0)
	{
		if (mGround.IsGround(MapCord.x, MapCord.y))
			mGround.SetTile(MapCord.x, MapCord.y, TileType::Empty);
		else
			mGround.SetTile(MapCord.x, MapCord.y, TileType::OneWay);
	}
	mLastMousePos.x = x;
	mLastMousePos.y = y;
}