#pragma once
#include <Windows.h>
#include <string>

enum SpriteType {
	//ĳ����
	CharacterWalking,
	CharacterJumping,
	CharacterIdle,
	CharacterFall,
	CharacterDie,
	CharacterSummoned,
	CharacterGrab,
	// ��
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
	// Ÿ��
	Tile,
	BackTile1,
	BackTile2,
	BackTile3,
	BackTile4,
	DoorTile,
	FinalFlag,
	// ���
	BackGround,
	// ����
	String,
	// ��ư
	PlayButton,
	BackButton,
	ResumeButton,
	QuitButton,
	HowToButton,
	OptionButton,
	PauseButton,
	NoButton,
	// ȿ��
	BombEffect,
	BulletEffect,
	SoyBeanEffect,
	// ������ ��������Ʈ
	BrickB,
	OneWayB,
	Medic,
	RockMag,
	BulletMag,
	Collect,
	// ��ź
	SoyBeanB,
	BombB,
	BulletB,
	// �������̽�
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