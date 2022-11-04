#include "GameBullet.h"
#include "Constants.h"

GameBullet::GameBullet()
{
	mBulletFrame = 0;
}

GameBullet::~GameBullet()
{

}

void GameBullet::SetBullet(BulletType Type, POINTFLOAT Shooter, POINTFLOAT Point)
{
	mAABB.center = Shooter;
	mType = Type;
	float Distance = sqrtf(powf(Shooter.x - Point.x, 2) + powf(Shooter.y - Point.y, 2));

	if (mType == BulletType::SoyBean) {
		//mSpeed.x = (Point.x - Shooter.x);
		//mSpeed.y = (Point.y - Shooter.y) * 2.2f;
		mSpeed.x = (Point.x - Shooter.x) / Distance * 600.0f;
		mSpeed.y = (Point.y - Shooter.y) / Distance * 1000.0f;
		mAABB.halfSize = { 9, 9 };
		mDamage = 1;
		mKnockBacks.y = mKnockBacks.x = 10.0f;
	}
	else if (mType == BulletType::Rock) {
		mSpeed.x = (Point.x - Shooter.x) / Distance * 600.0f;
		mSpeed.y = (Point.y - Shooter.y) / Distance * 1000.0f;
		mAABB.halfSize = { 17, 17 };
		mDamage = 3;
		mKnockBacks.y = mKnockBacks.x = 160.0f;
	}
	else if (mType == BulletType::Bullet) {
		mSpeed.x = (Point.x - Shooter.x) / Distance * 1000.0f;
		mSpeed.y = (Point.y - Shooter.y) / Distance * 1000.0f;
		mAABB.halfSize = { 16, 16 };
		mDamage = 1;
		mKnockBacks.y = mKnockBacks.x = 10.0f;
	}
}

void GameBullet::UpdateBullet(float dTime)
{
	if (mType == BulletType::SoyBean) {
		mSpeed.y += dTime * cGravity;
		mAABB.center.x += dTime * mSpeed.x;
		mAABB.center.y += dTime * mSpeed.y;
	}
	else if (mType == BulletType::Rock) {
		mSpeed.y += dTime * cGravity;
		mAABB.center.x += dTime * mSpeed.x;
		mAABB.center.y += dTime * mSpeed.y;
	}
	else if (mType == BulletType::Bullet) {
		mAABB.center.x += dTime * mSpeed.x;
		mAABB.center.y += dTime * mSpeed.y;
	}
}