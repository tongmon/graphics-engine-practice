#pragma once

#include <Windows.h>
#include <algorithm>
#include "AABB.h"

class Map;

class MovingObject
{
public:
	HDC mHdc;

	POINTFLOAT mTpos;
	POINTFLOAT mLscale;

	POINTFLOAT mOldPosition; // ���� ��ġ
	POINTFLOAT mPosition; // �� ��ġ
	
	POINTFLOAT mScale; // ũ��

	POINTFLOAT mSpeed; // �ӵ�
	POINTFLOAT mOldSpeed; // ���� �ӵ�

	AABB mAABB; // �浹 �ڽ�
	POINTFLOAT mAABBOffset = { 0,0 }; // �浹 �ڽ� ��ġ

	Map *mMap; // ��ü�� �����ϴ� ��

	bool mPushesRightWall = false; // �� �����ӿ� ������ ���� ��� �־�����
	bool mPushedRightWall = false;

	bool mPushesLeftWall = false; // �� �����ӿ� ���� ���� ��� �־�����
	bool mPushedLeftWall = false;

	bool mAtCeiling = false; // �� �����ӿ� õ�忡 ��� �־�����
	bool mWasAtCeiling = false; // ���� �����ӿ� õ�忡 ��� �־�����
	
	bool mOnGround = false; // �� �����ӿ� ���鿡 ��� �־�����
	bool mWasOnGround = false; // ���� �����ӿ� ���鿡 ��� �־�����.

	bool mOnOneWayPlatform = false;
	
	float mSpriteDepth = -1.0f;

	float cOneWayPlatformThreshold = 2.0f;

	bool mIgnoresOneWayPlatforms = false;

	float DeltaTime;

	COLORREF mColor;

	MovingObject();
	~MovingObject();
	void DrawMovingObject();
	bool HasCeiling(POINTFLOAT position, float& ceilingY);
	bool HasGround(POINTFLOAT position, float& groundY);
	bool CollidesWithRightWall(POINTFLOAT position, float& wallX);
	bool CollidesWithLeftWall(POINTFLOAT position, float& wallX);
	void GetDeltaTime(float dTime);
	void GetHDC(HDC hdc);
	void GetMap(Map* stage);
	void UpdatePhysics();
};

