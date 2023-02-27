#pragma once

#include <Windows.h>
#include <ctime>
#include <cstdlib>
#include "GameObj.h"
#include "GameBullet.h"
#include "GameSprite.h"

enum ItemType
{
	OneBrick,
	SomeBricks,
	BunchOfBricks,
	OneOneWayBrick,
	SomeOneWayBricks,
	BunchOfOneWayBricks,
	MedicPack,
	RockMagazine,
	BulletMagazine,
	ItemCount,
};

class GameItem : public GameObj
{
public:
	ItemType mType;
	int mSolidBrickCnt;
	int mOneWayBrickCnt;
	int mBulletCnt[(int)BulletType::BulletCount];
	int mHealCnt;
	int mFadeTime;
	int mItemFrame;

public:
	GameItem();
	~GameItem();
	void SetItem(int Rate, POINTFLOAT Point, GameMap* Map, GameSprite *Sprite);
	void UpdateItem(float dTime);
	void DrawItem();
};