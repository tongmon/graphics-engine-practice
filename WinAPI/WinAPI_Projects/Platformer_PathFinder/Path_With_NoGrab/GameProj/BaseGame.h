#pragma once
#include "BaseApp.h"
#include "Bot.h"
#include "Map.h"

class Character;

class BaseGame : public BaseApp
{
private:
	POINT mLastMousePos;
	Character mPlayer;
	Bot mNPC;
	Map mGround;
private:
	virtual void OnInit();
	virtual void OnRender(HDC Paper);
	virtual void OnUpdate();
	virtual void OnResize();
	virtual void OnRelease();

public:
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	BaseGame(void);
	~BaseGame(void);
};
