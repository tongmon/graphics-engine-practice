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

	POINTFLOAT mOldPosition; // ���� ��ġ
	POINTFLOAT mPosition; // �� ��ġ

	POINTFLOAT mSpeed; // �ӵ�
	POINTFLOAT mOldSpeed; // ���� �ӵ�

	AABB mAABB; // �浹 �ڽ�
	POINTFLOAT mAABBOffset = { 0,0 }; // �浹 �ڽ� ��ġ

	GameMap* mMap; // ��ü�� �����ϴ� ��

	bool mPushesRightWall = false; // �� �����ӿ� ������ ���� ��� �־�����
	bool mPushedRightWall = false;

	bool mPushesLeftWall = false; // �� �����ӿ� ���� ���� ��� �־�����
	bool mPushedLeftWall = false;

	bool mAtCeiling = false; // �� �����ӿ� õ�忡 ��� �־�����
	bool mWasAtCeiling = false; // ���� �����ӿ� õ�忡 ��� �־�����

	bool mOnGround = false; // �� �����ӿ� ���鿡 ��� �־�����
	bool mWasOnGround = false; // ���� �����ӿ� ���鿡 ��� �־�����.

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

