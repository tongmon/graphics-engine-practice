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

	POINTFLOAT mOldPosition; // 이전 위치
	POINTFLOAT mPosition; // 현 위치
	
	POINTFLOAT mScale; // 크기

	POINTFLOAT mSpeed; // 속도
	POINTFLOAT mOldSpeed; // 예전 속도

	AABB mAABB; // 충돌 박스
	POINTFLOAT mAABBOffset = { 0,0 }; // 충돌 박스 위치

	Map *mMap; // 물체가 존재하는 맵

	bool mPushesRightWall = false; // 막 프레임에 오른쪽 벽에 닿아 있었는지
	bool mPushedRightWall = false;

	bool mPushesLeftWall = false; // 막 프레임에 왼쪽 벽에 닿아 있었는지
	bool mPushedLeftWall = false;

	bool mAtCeiling = false; // 막 프레임에 천장에 닿아 있었는지
	bool mWasAtCeiling = false; // 예전 프레임에 천장에 닿아 있었는지
	
	bool mOnGround = false; // 막 프레임에 지면에 닿아 있었는지
	bool mWasOnGround = false; // 예전 프레임에 지면에 닿아 있었는지.

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

