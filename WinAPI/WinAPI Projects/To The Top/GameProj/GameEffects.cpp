#include "GameEffects.h"
#include "Constants.h"

GameEffects::GameEffects()
{
	mRemove = false;
	mEffFrame = 0;
}

GameEffects::~GameEffects()
{

}

void GameEffects::InitEffects(POINTFLOAT Center, EffectType Type, GameSprite* Sprite)
{
	mType = Type;
	mSprite = Sprite;
	mAABB.center = Center;
}

void GameEffects::DrawEffects(HDC hdc)
{
	if (mType == EffectType::BombEff)
	{
		mAABB.halfSize = { 60,60 };
		mSprite->Animation(hdc, true, SpriteType::BombEffect, mEffFrame,
			mAABB.center.x - mAABB.halfSize.x, mAABB.center.y - mAABB.halfSize.y,
			mAABB.halfSize.x * 2, mAABB.halfSize.y * 2, mRemove);
	}
	else if (mType == EffectType::BulletEff)
	{
		mAABB.halfSize = { 20,20 };
		mSprite->Animation(hdc, true, SpriteType::BulletEffect, mEffFrame,
			mAABB.center.x - mAABB.halfSize.x, mAABB.center.y - mAABB.halfSize.y,
			mAABB.halfSize.x * 2, mAABB.halfSize.y * 2, mRemove);
	}
	else if (mType == EffectType::EnemyDieEff)
	{
		mAABB.halfSize = { 40,40 };
		mSprite->Animation(hdc, true, SpriteType::BulletEffect, mEffFrame,
			mAABB.center.x - mAABB.halfSize.x, mAABB.center.y - mAABB.halfSize.y,
			mAABB.halfSize.x * 2, mAABB.halfSize.y * 2, mRemove);
	}
	else if (mType == EffectType::CharacterDieEff)
	{
		mAABB.halfSize = { 40,40 };
		mSprite->Animation(hdc, true, SpriteType::BulletEffect, mEffFrame,
			mAABB.center.x - mAABB.halfSize.x, mAABB.center.y - mAABB.halfSize.y,
			mAABB.halfSize.x * 2, mAABB.halfSize.y * 2, mRemove);
	}
	else if (mType == EffectType::CollectEff)
	{
		mAABB.halfSize = { 50,50 };
		mSprite->Animation(hdc, true, SpriteType::Collect, mEffFrame,
			mAABB.center.x - mAABB.halfSize.x, mAABB.center.y - mAABB.halfSize.y,
			mAABB.halfSize.x * 2, mAABB.halfSize.y * 2, mRemove);
	}
}

