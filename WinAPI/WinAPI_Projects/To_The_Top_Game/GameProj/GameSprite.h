#pragma once
#include <Windows.h>
#include <string>

enum SpriteType {
	//캐릭터
	CharacterWalking,
	CharacterJumping,
	CharacterIdle,
	CharacterFall,
	CharacterDie,
	CharacterSummoned,
	CharacterGrab,
	// 적
	GruntWalking,
	GruntJumping,
	GruntIdle,
	GruntFall,
	GruntDie,
	GruntSummoned,
	GruntGrab,
	HeavyWalking,
	HeavyJumping,
	HeavyIdle,
	HeavyFall,
	HeavyDie,
	HeavySummoned,
	HeavyGrab,
	MassiveWalking,
	MassiveJumping,
	MassiveIdle,
	MassiveFall,
	MassiveDie,
	MassiveSummoned,
	MassiveGrab,
	NinjaWalking,
	NinjaJumping,
	NinjaIdle,
	NinjaFall,
	NinjaDie,
	NinjaSummoned,
	NinjaGrab,
	EnemyDie,
	// 타일
	Tile,
	BackTile1,
	BackTile2,
	BackTile3,
	BackTile4,
	DoorTile,
	FinalFlag,
	// 배경
	BackGround,
	// 글자
	String,
	// 버튼
	PlayButton,
	BackButton,
	ResumeButton,
	QuitButton,
	HowToButton,
	OptionButton,
	PauseButton,
	NoButton,
	// 효과
	BombEffect,
	BulletEffect,
	SoyBeanEffect,
	// 아이템 스프라이트
	BrickB,
	OneWayB,
	Medic,
	RockMag,
	BulletMag,
	Collect,
	// 총탄
	SoyBeanB,
	BombB,
	BulletB,
	// 인터페이스
	Hearts,
	HeartsBack,
	BombCnt,
	BulletCnt,
	SoyBeanCnt,
	CrossHair,
	SpriteCount,
};

enum TileType;

class GameSprite
{
public:
	HDC mSpriteDC[SpriteType::SpriteCount];
	HBITMAP mSpriteBitMap[SpriteType::SpriteCount];
	BITMAP mBitMap[SpriteType::SpriteCount];
	int Cuts[SpriteType::SpriteCount];

	GameSprite();
	~GameSprite();
	void DrawIMG(HDC hdc, std::wstring Strs, bool isLeft, float x, float y, float Width, float Height);
	void DrawIMG(HDC hdc, SpriteType Type, bool isLeft, float x, float y, float Width, float Height);
	void Text(HDC hdc, char Letter, float x, float y, float Width, float Height);
	void SpriteString(HDC hdc, std::wstring Strs, float x, float y, float hSize);
	void ScenterString(HDC hdc, std::wstring Strs, float x, float y, float hSize);
	void Animation(HDC hdc, bool isLeft, SpriteType Type, int& AnimFrame, float x, float y, float Width, float Height);
	void Animation(HDC hdc, bool isLeft, SpriteType Type, int& AnimFrame, float x, float y, float Width, float Height, bool& remove);
	void DrawTile(HDC hdc, TileType Type, float x, float y, float tSize);
};