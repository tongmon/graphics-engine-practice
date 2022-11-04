#pragma once

#include <Windows.h>
#include "GameSprite.h"
#include "AABB.h"

enum class BulletType
{
	SoyBean,
	Bullet,
	Rock,
	BulletCount,
};

class GameBullet
{
public:
	BulletType mType;
	AABB mAABB;
	POINTFLOAT mSpeed;
	POINTFLOAT mKnockBacks;
	float mDamage;
	int mBulletFrame;
	GameSprite* mSprite;

public:
	GameBullet();
	~GameBullet();
	void SetBullet(BulletType Type, POINTFLOAT Shooter, POINTFLOAT Point);
	void UpdateBullet(float dTime);
};