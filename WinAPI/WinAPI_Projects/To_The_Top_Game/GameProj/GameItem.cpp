#include "GameItem.h"
#include "Constants.h"
#include "GameMap.h"

GameItem::GameItem()
{
	mSolidBrickCnt = 0;
	mOneWayBrickCnt = 0;
	mBulletCnt[(int)BulletType::Rock] = 0;
	mBulletCnt[(int)BulletType::Bullet] = 0;
	mHealCnt = 0;
	mItemFrame = 0;
}

GameItem::~GameItem()
{

}

void GameItem::SetItem(int Rate, POINTFLOAT Point, GameMap* Map, GameSprite *Sprite)
{
	mSprite = Sprite;
	mFadeTime = cItemFadeTime;
	mPosition = Point;
	mAABB.halfSize = { 18.f,18.f };
	mMap = Map;

	if (0 <= Rate && Rate < 3) {
		mType = ItemType::BunchOfBricks;
	}
	else if (3 <= Rate && Rate < 6) {
		mType = ItemType::BunchOfOneWayBricks;
	}
	else if (6 <= Rate && Rate < 12) {
		mType = ItemType::SomeBricks;
	}
	else if (12 <= Rate && Rate < 18) {
		mType = ItemType::SomeOneWayBricks;
	}
	else if (18 <= Rate && Rate < 26) {
		mType = ItemType::OneOneWayBrick;
	}
	else if (26 <= Rate && Rate < 34) {
		mType = ItemType::OneBrick;
	}
	else if (34 <= Rate && Rate < 55) {
		mType = ItemType::MedicPack;
	}
	else if (55 <= Rate && Rate < 70) {
		mType = ItemType::RockMagazine;
	}
	else
		mType = ItemType::BulletMagazine;

	if (mType == ItemType::OneBrick)
	{
		mSolidBrickCnt = 1;
		mAABB.halfSize = { 16.f,16.f };
	}
	else if (mType == ItemType::SomeBricks)
	{
		mSolidBrickCnt = rand() % 3 + 2;
		mAABB.halfSize = { 22.f,22.f };
	}
	else if (mType == ItemType::BunchOfBricks)
	{
		mSolidBrickCnt = rand() % 5 + 3;
		mAABB.halfSize = { 32.f,32.f };
	}
	else if (mType == ItemType::OneOneWayBrick)
	{
		mOneWayBrickCnt = 1;
		mAABB.halfSize = { 16.f,16.f };
	}
	else if (mType == ItemType::SomeOneWayBricks)
	{
		mOneWayBrickCnt = rand() % 3 + 2;
		mAABB.halfSize = { 22.f,22.f };
	}
	else if (mType == ItemType::BunchOfOneWayBricks)
	{
		mOneWayBrickCnt = rand() % 5 + 3;
		mAABB.halfSize = { 32.f,32.f };
	}
	else if (mType == ItemType::BulletMagazine)
	{
		mBulletCnt[(int)BulletType::Bullet] = rand() % 15 + 3;
		mAABB.halfSize = { 15.f,15.f };
	}
	else if (mType == ItemType::RockMagazine)
	{
		mBulletCnt[(int)BulletType::Rock] = rand() % 5 + 1;
		mAABB.halfSize = { 15.f,15.f };
	}
	else if (mType == ItemType::MedicPack)
	{
		mHealCnt = 2;
		mAABB.halfSize = { 15.f,15.f };
	}
	float Signed[2] = { 1.f,-1.f };
	mSpeed = { float(rand() % 110 + 10) * Signed[rand() % 2], float(rand() % 200 + 300) };
}

void GameItem::UpdateItem(float dtime)
{
	mFadeTime--;
	mDeltaTime = dtime;
	mSpeed.y += cGravity * mDeltaTime;
	if (mSpeed.x < 0) {
		mSpeed.x = min(0, mSpeed.x + 90.0f * mDeltaTime);
	}
	else {
		mSpeed.x = max(0, mSpeed.x - 90.0f * mDeltaTime);
	}
	UpdatePhysics();
}

void GameItem::DrawItem()
{
	POINTFLOAT Size = { 60.f,60.f };
	if (mType == ItemType::OneBrick || mType == ItemType::SomeBricks || mType == ItemType::BunchOfBricks)
	{
		mSprite->DrawIMG(mHdc, SpriteType::BrickB, false,
			int(mAABB.center.x - mAABB.halfSize.x), int(mAABB.center.y - mAABB.halfSize.y),
			mAABB.halfSize.x * 2, mAABB.halfSize.y * 2);
	}
	else if (mType == ItemType::OneOneWayBrick || mType == ItemType::SomeOneWayBricks || mType == ItemType::BunchOfOneWayBricks)
	{
		mSprite->DrawIMG(mHdc, SpriteType::OneWayB, false,
			int(mAABB.center.x - mAABB.halfSize.x), int(mAABB.center.y - mAABB.halfSize.y),
			mAABB.halfSize.x * 2, mAABB.halfSize.y * 2);
	}
	else if (mType == ItemType::BulletMagazine)
	{
		mSprite->Animation(mHdc, false, SpriteType::BulletMag, mItemFrame,
			int(mAABB.center.x - Size.x / 2.f), int(mAABB.center.y - Size.y / 2.f),
			Size.x, Size.y);
	}
	else if (mType == ItemType::RockMagazine)
	{
		mSprite->Animation(mHdc, false, SpriteType::RockMag, mItemFrame,
			int(mAABB.center.x - Size.x / 2.f), int(mAABB.center.y - Size.y / 2.f),
			Size.x, Size.y);
	}
	else if (mType == ItemType::MedicPack)
	{
		mSprite->Animation(mHdc, false, SpriteType::Medic, mItemFrame,
			int(mAABB.center.x - Size.x / 2.f), int(mAABB.center.y - Size.y / 2.f),
			Size.x, Size.y);
	}
}