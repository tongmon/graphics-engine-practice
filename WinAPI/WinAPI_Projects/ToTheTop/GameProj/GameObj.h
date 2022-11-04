#pragma once

#include <Windows.h>
#include <algorithm>
#include <string>
#include "AABB.h"
#include "GameSprite.h"
#include "GameEffects.h"
#include "FmodSound.h"

class GameMap;

class GameObj
{
public:
	HDC mHdc;

	POINTFLOAT mOldPosition; // 이전 위치
	POINTFLOAT mPosition; // 현 위치

	POINTFLOAT mSpeed; // 속도
	POINTFLOAT mOldSpeed; // 예전 속도

	AABB mAABB; // 충돌 박스
	POINTFLOAT mAABBOffset = { 0,0 }; // 충돌 박스 위치

	GameMap* mMap; // 물체가 존재하는 맵

	bool mPushesRightWall = false; // 막 프레임에 오른쪽 벽에 닿아 있었는지
	bool mPushedRightWall = false;

	bool mPushesLeftWall = false; // 막 프레임에 왼쪽 벽에 닿아 있었는지
	bool mPushedLeftWall = false;

	bool mAtCeiling = false; // 막 프레임에 천장에 닿아 있었는지
	bool mWasAtCeiling = false; // 예전 프레임에 천장에 닿아 있었는지

	bool mOnGround = false; // 막 프레임에 지면에 닿아 있었는지
	bool mWasOnGround = false; // 예전 프레임에 지면에 닿아 있었는지.

	bool mOnOneWayPlatform = false;

	float cOneWayPlatformThreshold = 2.0f;

	bool mIgnoresOneWayPlatforms = false;

	float mDeltaTime;

	COLORREF mColor;

	GameSprite* mSprite;

	CFmodSound* mGameSound;

	std::list<GameEffects*> *mEffList;

	bool misLeft = true;

public:
	GameObj();
	~GameObj();
	void InitObj(POINTFLOAT Point, POINTFLOAT hSize, GameSprite *Sprite);
	void DrawObject();
	void DrawObject(SpriteType Type, bool isAnime, int& Frame);
	bool HasCeiling(POINTFLOAT position, float& ceilingY);
	bool HasGround(POINTFLOAT position, float& groundY);
	bool CollidesWithRightWall(POINTFLOAT position, float& wallX);
	bool CollidesWithLeftWall(POINTFLOAT position, float& wallX);
	void GetDeltaTime(float dTime);
	void SetHDC(HDC hdc);
	void SetMap(GameMap* stage);
	void SetPos(POINTFLOAT Point);
	void UpdatePhysics();
};

