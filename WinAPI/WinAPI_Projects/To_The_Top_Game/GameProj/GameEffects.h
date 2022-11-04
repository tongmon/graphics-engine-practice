#pragma once

#include <Windows.h>
#include "GameSprite.h"
#include "AABB.h"

enum EffectType
{
	CharacterDieEff,
	BombEff,
	BulletEff,
	EnemyDieEff,
	SummonEff,
	CollectEff,
	EffCount,
};

class GameEffects
{
public:
	HDC mHdc;
	AABB mAABB;
	EffectType mType;
	GameSprite* mSprite;
	int mEffFrame;
	bool mRemove;
	GameEffects();
	~GameEffects();
	void InitEffects(POINTFLOAT Center, EffectType Type, GameSprite *Sprite);
	void DrawEffects(HDC hdc);
};

