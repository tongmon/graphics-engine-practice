#include "GameSprite.h"
#include "GameMap.h"
#include "Constants.h"

GameSprite::GameSprite()
{
	for (int i = 0; i < SpriteType::SpriteCount; i++)
	{
		mSpriteDC[i] = CreateCompatibleDC(NULL);
	}
	HBITMAP oldBitMap;

	// 타일 세팅
	mSpriteBitMap[SpriteType::Tile] = (HBITMAP)LoadImage(NULL, L"Sprites/Tiles/Tile.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::Tile], mSpriteBitMap[SpriteType::Tile]);
	GetObject(mSpriteBitMap[SpriteType::Tile], sizeof(BITMAP), &mBitMap[SpriteType::Tile]);
	StretchBlt(mSpriteDC[SpriteType::Tile], mBitMap[SpriteType::Tile].bmWidth, mBitMap[SpriteType::Tile].bmHeight,
		-mBitMap[SpriteType::Tile].bmWidth, -mBitMap[SpriteType::Tile].bmHeight,
		mSpriteDC[SpriteType::Tile], 0, 0, mBitMap[SpriteType::Tile].bmWidth, mBitMap[SpriteType::Tile].bmHeight, SRCCOPY);

	// 뒷 배경 타일 1
	mSpriteBitMap[SpriteType::BackTile1] = (HBITMAP)LoadImage(NULL, L"Sprites/Tiles/Blue.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::BackTile1], mSpriteBitMap[SpriteType::BackTile1]);
	GetObject(mSpriteBitMap[SpriteType::BackTile1], sizeof(BITMAP), &mBitMap[SpriteType::BackTile1]);

	// 뒷 배경 타일 2
	mSpriteBitMap[SpriteType::BackTile2] = (HBITMAP)LoadImage(NULL, L"Sprites/Tiles/Brown.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::BackTile2], mSpriteBitMap[SpriteType::BackTile2]);
	GetObject(mSpriteBitMap[SpriteType::BackTile2], sizeof(BITMAP), &mBitMap[SpriteType::BackTile2]);

	// 뒷 배경 타일 3
	mSpriteBitMap[SpriteType::BackTile3] = (HBITMAP)LoadImage(NULL, L"Sprites/Tiles/Gray.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::BackTile3], mSpriteBitMap[SpriteType::BackTile3]);
	GetObject(mSpriteBitMap[SpriteType::BackTile3], sizeof(BITMAP), &mBitMap[SpriteType::BackTile3]);

	// 뒷 배경 타일 4
	mSpriteBitMap[SpriteType::BackTile4] = (HBITMAP)LoadImage(NULL, L"Sprites/Tiles/Purple.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::BackTile4], mSpriteBitMap[SpriteType::BackTile4]);
	GetObject(mSpriteBitMap[SpriteType::BackTile4], sizeof(BITMAP), &mBitMap[SpriteType::BackTile4]);

	// 마지막 타일
	mSpriteBitMap[SpriteType::FinalFlag] = (HBITMAP)LoadImage(NULL, L"Sprites/FinishFlag.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::FinalFlag], mSpriteBitMap[SpriteType::FinalFlag]);
	GetObject(mSpriteBitMap[SpriteType::FinalFlag], sizeof(BITMAP), &mBitMap[SpriteType::FinalFlag]);
	Cuts[SpriteType::FinalFlag] = 10;

	// 도착 타일
	mSpriteBitMap[SpriteType::DoorTile] = (HBITMAP)LoadImage(NULL, L"Sprites/Tiles/DoorTile.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::DoorTile], mSpriteBitMap[SpriteType::DoorTile]);
	GetObject(mSpriteBitMap[SpriteType::DoorTile], sizeof(BITMAP), &mBitMap[SpriteType::DoorTile]);

	// 글자 스프라이트
	mSpriteBitMap[SpriteType::String] = (HBITMAP)LoadImage(NULL, L"Sprites/Text_B.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::String], mSpriteBitMap[SpriteType::String]);
	GetObject(mSpriteBitMap[SpriteType::String], sizeof(BITMAP), &mBitMap[SpriteType::String]);
	StretchBlt(mSpriteDC[SpriteType::String], 0, mBitMap[SpriteType::String].bmHeight,
		mBitMap[SpriteType::String].bmWidth, -mBitMap[SpriteType::String].bmHeight,
		mSpriteDC[SpriteType::String], 0, 0, mBitMap[SpriteType::String].bmWidth, mBitMap[SpriteType::String].bmHeight, SRCCOPY);

	// 캐릭터 기본 상태
	mSpriteBitMap[SpriteType::CharacterIdle] = (HBITMAP)LoadImage(NULL, L"Sprites/Hero/Idle.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::CharacterIdle], mSpriteBitMap[SpriteType::CharacterIdle]);
	GetObject(mSpriteBitMap[SpriteType::CharacterIdle], sizeof(BITMAP), &mBitMap[SpriteType::CharacterIdle]);
	Cuts[SpriteType::CharacterIdle] = 11;

	// 캐릭터 걷기 동작
	mSpriteBitMap[SpriteType::CharacterWalking] = (HBITMAP)LoadImage(NULL, L"Sprites/Hero/Run.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::CharacterWalking], mSpriteBitMap[SpriteType::CharacterWalking]);
	GetObject(mSpriteBitMap[SpriteType::CharacterWalking], sizeof(BITMAP), &mBitMap[SpriteType::CharacterWalking]);
	Cuts[SpriteType::CharacterWalking] = 12;

	// 캐릭터 점프 동작
	mSpriteBitMap[SpriteType::CharacterJumping] = (HBITMAP)LoadImage(NULL, L"Sprites/Hero/Jump.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::CharacterJumping], mSpriteBitMap[SpriteType::CharacterJumping]);
	GetObject(mSpriteBitMap[SpriteType::CharacterJumping], sizeof(BITMAP), &mBitMap[SpriteType::CharacterJumping]);

	// 캐릭터 떨어지기 동작
	mSpriteBitMap[SpriteType::CharacterFall] = (HBITMAP)LoadImage(NULL, L"Sprites/Hero/Fall.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::CharacterFall], mSpriteBitMap[SpriteType::CharacterFall]);
	GetObject(mSpriteBitMap[SpriteType::CharacterFall], sizeof(BITMAP), &mBitMap[SpriteType::CharacterFall]);

	// 캐릭터 잡기 동작
	mSpriteBitMap[SpriteType::CharacterGrab] = (HBITMAP)LoadImage(NULL, L"Sprites/Hero/Wall.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::CharacterGrab], mSpriteBitMap[SpriteType::CharacterGrab]);
	GetObject(mSpriteBitMap[SpriteType::CharacterGrab], sizeof(BITMAP), &mBitMap[SpriteType::CharacterGrab]);
	Cuts[SpriteType::CharacterGrab] = 5;

	// Grunt 기본 상태
	mSpriteBitMap[SpriteType::GruntIdle] = (HBITMAP)LoadImage(NULL, L"Sprites/Grunt/Idle.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::GruntIdle], mSpriteBitMap[SpriteType::GruntIdle]);
	GetObject(mSpriteBitMap[SpriteType::GruntIdle], sizeof(BITMAP), &mBitMap[SpriteType::GruntIdle]);
	Cuts[SpriteType::GruntIdle] = 11;

	// Grunt 걷기 동작
	mSpriteBitMap[SpriteType::GruntWalking] = (HBITMAP)LoadImage(NULL, L"Sprites/Grunt/Run.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::GruntWalking], mSpriteBitMap[SpriteType::GruntWalking]);
	GetObject(mSpriteBitMap[SpriteType::GruntWalking], sizeof(BITMAP), &mBitMap[SpriteType::GruntWalking]);
	Cuts[SpriteType::GruntWalking] = 12;

	// Grunt 점프 동작
	mSpriteBitMap[SpriteType::GruntJumping] = (HBITMAP)LoadImage(NULL, L"Sprites/Grunt/Jump.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::GruntJumping], mSpriteBitMap[SpriteType::GruntJumping]);
	GetObject(mSpriteBitMap[SpriteType::GruntJumping], sizeof(BITMAP), &mBitMap[SpriteType::GruntJumping]);

	// Grunt 떨어지기 동작
	mSpriteBitMap[SpriteType::GruntFall] = (HBITMAP)LoadImage(NULL, L"Sprites/Grunt/Fall.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::GruntFall], mSpriteBitMap[SpriteType::GruntFall]);
	GetObject(mSpriteBitMap[SpriteType::GruntFall], sizeof(BITMAP), &mBitMap[SpriteType::GruntFall]);

	// Grunt 잡기 동작
	mSpriteBitMap[SpriteType::GruntGrab] = (HBITMAP)LoadImage(NULL, L"Sprites/Grunt/Wall.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::GruntGrab], mSpriteBitMap[SpriteType::GruntGrab]);
	GetObject(mSpriteBitMap[SpriteType::GruntGrab], sizeof(BITMAP), &mBitMap[SpriteType::GruntGrab]);
	Cuts[SpriteType::GruntGrab] = 5;

	// Heavy 기본 상태
	mSpriteBitMap[SpriteType::HeavyIdle] = (HBITMAP)LoadImage(NULL, L"Sprites/Heavy/Idle.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::HeavyIdle], mSpriteBitMap[SpriteType::HeavyIdle]);
	GetObject(mSpriteBitMap[SpriteType::HeavyIdle], sizeof(BITMAP), &mBitMap[SpriteType::HeavyIdle]);
	Cuts[SpriteType::HeavyIdle] = 11;

	// Heavy 걷기 동작
	mSpriteBitMap[SpriteType::HeavyWalking] = (HBITMAP)LoadImage(NULL, L"Sprites/Heavy/Run.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::HeavyWalking], mSpriteBitMap[SpriteType::HeavyWalking]);
	GetObject(mSpriteBitMap[SpriteType::HeavyWalking], sizeof(BITMAP), &mBitMap[SpriteType::HeavyWalking]);
	Cuts[SpriteType::HeavyWalking] = 12;

	// Heavy 점프 동작
	mSpriteBitMap[SpriteType::HeavyJumping] = (HBITMAP)LoadImage(NULL, L"Sprites/Heavy/Jump.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::HeavyJumping], mSpriteBitMap[SpriteType::HeavyJumping]);
	GetObject(mSpriteBitMap[SpriteType::HeavyJumping], sizeof(BITMAP), &mBitMap[SpriteType::HeavyJumping]);

	// Heavy 떨어지기 동작
	mSpriteBitMap[SpriteType::HeavyFall] = (HBITMAP)LoadImage(NULL, L"Sprites/Heavy/Fall.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::HeavyFall], mSpriteBitMap[SpriteType::HeavyFall]);
	GetObject(mSpriteBitMap[SpriteType::HeavyFall], sizeof(BITMAP), &mBitMap[SpriteType::HeavyFall]);

	// Heavy 잡기 동작
	mSpriteBitMap[SpriteType::HeavyGrab] = (HBITMAP)LoadImage(NULL, L"Sprites/Heavy/Wall.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::HeavyGrab], mSpriteBitMap[SpriteType::HeavyGrab]);
	GetObject(mSpriteBitMap[SpriteType::HeavyGrab], sizeof(BITMAP), &mBitMap[SpriteType::HeavyGrab]);
	Cuts[SpriteType::HeavyGrab] = 5;

	// Ninja 기본 상태
	mSpriteBitMap[SpriteType::NinjaIdle] = (HBITMAP)LoadImage(NULL, L"Sprites/Ninja/Idle.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::NinjaIdle], mSpriteBitMap[SpriteType::NinjaIdle]);
	GetObject(mSpriteBitMap[SpriteType::NinjaIdle], sizeof(BITMAP), &mBitMap[SpriteType::NinjaIdle]);
	Cuts[SpriteType::NinjaIdle] = 11;

	// Ninja 걷기 동작
	mSpriteBitMap[SpriteType::NinjaWalking] = (HBITMAP)LoadImage(NULL, L"Sprites/Ninja/Run.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::NinjaWalking], mSpriteBitMap[SpriteType::NinjaWalking]);
	GetObject(mSpriteBitMap[SpriteType::NinjaWalking], sizeof(BITMAP), &mBitMap[SpriteType::NinjaWalking]);
	Cuts[SpriteType::NinjaWalking] = 12;

	// Ninja 점프 동작
	mSpriteBitMap[SpriteType::NinjaJumping] = (HBITMAP)LoadImage(NULL, L"Sprites/Ninja/Jump.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::NinjaJumping], mSpriteBitMap[SpriteType::NinjaJumping]);
	GetObject(mSpriteBitMap[SpriteType::NinjaJumping], sizeof(BITMAP), &mBitMap[SpriteType::NinjaJumping]);

	// Ninja 떨어지기 동작
	mSpriteBitMap[SpriteType::NinjaFall] = (HBITMAP)LoadImage(NULL, L"Sprites/Ninja/Fall.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::NinjaFall], mSpriteBitMap[SpriteType::NinjaFall]);
	GetObject(mSpriteBitMap[SpriteType::NinjaFall], sizeof(BITMAP), &mBitMap[SpriteType::NinjaFall]);

	// Ninja 잡기 동작
	mSpriteBitMap[SpriteType::NinjaGrab] = (HBITMAP)LoadImage(NULL, L"Sprites/Ninja/Wall.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::NinjaGrab], mSpriteBitMap[SpriteType::NinjaGrab]);
	GetObject(mSpriteBitMap[SpriteType::NinjaGrab], sizeof(BITMAP), &mBitMap[SpriteType::NinjaGrab]);
	Cuts[SpriteType::NinjaGrab] = 5;

	// Massive 기본 상태
	mSpriteBitMap[SpriteType::MassiveIdle] = (HBITMAP)LoadImage(NULL, L"Sprites/Massive/Idle.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::MassiveIdle], mSpriteBitMap[SpriteType::MassiveIdle]);
	GetObject(mSpriteBitMap[SpriteType::MassiveIdle], sizeof(BITMAP), &mBitMap[SpriteType::MassiveIdle]);
	Cuts[SpriteType::MassiveIdle] = 11;

	// Massive 걷기 동작
	mSpriteBitMap[SpriteType::MassiveWalking] = (HBITMAP)LoadImage(NULL, L"Sprites/Massive/Run.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::MassiveWalking], mSpriteBitMap[SpriteType::MassiveWalking]);
	GetObject(mSpriteBitMap[SpriteType::MassiveWalking], sizeof(BITMAP), &mBitMap[SpriteType::MassiveWalking]);
	Cuts[SpriteType::MassiveWalking] = 12;

	// Massive 점프 동작
	mSpriteBitMap[SpriteType::MassiveJumping] = (HBITMAP)LoadImage(NULL, L"Sprites/Massive/Jump.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::MassiveJumping], mSpriteBitMap[SpriteType::MassiveJumping]);
	GetObject(mSpriteBitMap[SpriteType::MassiveJumping], sizeof(BITMAP), &mBitMap[SpriteType::MassiveJumping]);

	// Massive 떨어지기 동작
	mSpriteBitMap[SpriteType::MassiveFall] = (HBITMAP)LoadImage(NULL, L"Sprites/Massive/Fall.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::MassiveFall], mSpriteBitMap[SpriteType::MassiveFall]);
	GetObject(mSpriteBitMap[SpriteType::MassiveFall], sizeof(BITMAP), &mBitMap[SpriteType::MassiveFall]);

	// Massive 잡기 동작
	mSpriteBitMap[SpriteType::MassiveGrab] = (HBITMAP)LoadImage(NULL, L"Sprites/Massive/Wall.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::MassiveGrab], mSpriteBitMap[SpriteType::MassiveGrab]);
	GetObject(mSpriteBitMap[SpriteType::MassiveGrab], sizeof(BITMAP), &mBitMap[SpriteType::MassiveGrab]);
	Cuts[SpriteType::MassiveGrab] = 5;

	// 플레이 버튼
	mSpriteBitMap[SpriteType::PlayButton] = (HBITMAP)LoadImage(NULL, L"Sprites/Buttons/Play.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::PlayButton], mSpriteBitMap[SpriteType::PlayButton]);
	GetObject(mSpriteBitMap[SpriteType::PlayButton], sizeof(BITMAP), &mBitMap[SpriteType::PlayButton]);

	// 재개 버튼
	mSpriteBitMap[SpriteType::ResumeButton] = (HBITMAP)LoadImage(NULL, L"Sprites/Buttons/Next.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::ResumeButton], mSpriteBitMap[SpriteType::ResumeButton]);
	GetObject(mSpriteBitMap[SpriteType::ResumeButton], sizeof(BITMAP), &mBitMap[SpriteType::ResumeButton]);

	// 뒤로가기 버튼
	mSpriteBitMap[SpriteType::BackButton] = (HBITMAP)LoadImage(NULL, L"Sprites/Buttons/Back.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::BackButton], mSpriteBitMap[SpriteType::BackButton]);
	GetObject(mSpriteBitMap[SpriteType::BackButton], sizeof(BITMAP), &mBitMap[SpriteType::BackButton]);

	// 나가기 버튼
	mSpriteBitMap[SpriteType::QuitButton] = (HBITMAP)LoadImage(NULL, L"Sprites/Buttons/Close.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::QuitButton], mSpriteBitMap[SpriteType::QuitButton]);
	GetObject(mSpriteBitMap[SpriteType::QuitButton], sizeof(BITMAP), &mBitMap[SpriteType::QuitButton]);

	// 멈춤 버튼
	mSpriteBitMap[SpriteType::PauseButton] = (HBITMAP)LoadImage(NULL, L"Sprites/Buttons/Levels.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::PauseButton], mSpriteBitMap[SpriteType::PauseButton]);
	GetObject(mSpriteBitMap[SpriteType::PauseButton], sizeof(BITMAP), &mBitMap[SpriteType::PauseButton]);

	// 설명 버튼
	mSpriteBitMap[SpriteType::HowToButton] = (HBITMAP)LoadImage(NULL, L"Sprites/Buttons/Achievements.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::HowToButton], mSpriteBitMap[SpriteType::HowToButton]);
	GetObject(mSpriteBitMap[SpriteType::HowToButton], sizeof(BITMAP), &mBitMap[SpriteType::HowToButton]);

	// 옵션 버튼
	mSpriteBitMap[SpriteType::OptionButton] = (HBITMAP)LoadImage(NULL, L"Sprites/Buttons/Settings.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::OptionButton], mSpriteBitMap[SpriteType::OptionButton]);
	GetObject(mSpriteBitMap[SpriteType::OptionButton], sizeof(BITMAP), &mBitMap[SpriteType::OptionButton]);

	// No 버튼
	mSpriteBitMap[SpriteType::SoyBeanCnt] = (HBITMAP)LoadImage(NULL, L"Sprites/InterFace/SoyBeanCnt.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::SoyBeanCnt], mSpriteBitMap[SpriteType::SoyBeanCnt]);
	GetObject(mSpriteBitMap[SpriteType::SoyBeanCnt], sizeof(BITMAP), &mBitMap[SpriteType::SoyBeanCnt]);

	// 뒷배경
	mSpriteBitMap[SpriteType::BackGround] = (HBITMAP)LoadImage(NULL, L"Sprites/Background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::BackGround], mSpriteBitMap[SpriteType::BackGround]);
	GetObject(mSpriteBitMap[SpriteType::BackGround], sizeof(BITMAP), &mBitMap[SpriteType::BackGround]);

	// Block
	mSpriteBitMap[SpriteType::BrickB] = (HBITMAP)LoadImage(NULL, L"Sprites/Items/Block.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::BrickB], mSpriteBitMap[SpriteType::BrickB]);
	GetObject(mSpriteBitMap[SpriteType::BrickB], sizeof(BITMAP), &mBitMap[SpriteType::BrickB]);

	// OneWay
	mSpriteBitMap[SpriteType::OneWayB] = (HBITMAP)LoadImage(NULL, L"Sprites/Items/OneWay.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::OneWayB], mSpriteBitMap[SpriteType::OneWayB]);
	GetObject(mSpriteBitMap[SpriteType::OneWayB], sizeof(BITMAP), &mBitMap[SpriteType::OneWayB]);

	// MedicPac
	mSpriteBitMap[SpriteType::Medic] = (HBITMAP)LoadImage(NULL, L"Sprites/Items/Apple.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::Medic], mSpriteBitMap[SpriteType::Medic]);
	GetObject(mSpriteBitMap[SpriteType::Medic], sizeof(BITMAP), &mBitMap[SpriteType::Medic]);
	Cuts[SpriteType::Medic] = 17;

	// RockMagazine
	mSpriteBitMap[SpriteType::RockMag] = (HBITMAP)LoadImage(NULL, L"Sprites/Items/Kiwi.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::RockMag], mSpriteBitMap[SpriteType::RockMag]);
	GetObject(mSpriteBitMap[SpriteType::RockMag], sizeof(BITMAP), &mBitMap[SpriteType::RockMag]);
	Cuts[SpriteType::RockMag] = 17;
	
	// BulletMagazine
	mSpriteBitMap[SpriteType::BulletMag] = (HBITMAP)LoadImage(NULL, L"Sprites/Items/Cherries.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::BulletMag], mSpriteBitMap[SpriteType::BulletMag]);
	GetObject(mSpriteBitMap[SpriteType::BulletMag], sizeof(BITMAP), &mBitMap[SpriteType::BulletMag]);
	Cuts[SpriteType::BulletMag] = 17;

	// 아이템 줍기 모션
	mSpriteBitMap[SpriteType::Collect] = (HBITMAP)LoadImage(NULL, L"Sprites/Items/collect.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::Collect], mSpriteBitMap[SpriteType::Collect]);
	GetObject(mSpriteBitMap[SpriteType::Collect], sizeof(BITMAP), &mBitMap[SpriteType::Collect]);
	Cuts[SpriteType::Collect] = 6;

	// SoyBean
	mSpriteBitMap[SpriteType::SoyBeanB] = (HBITMAP)LoadImage(NULL, L"Sprites/Bullets/SoyBean.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::SoyBeanB], mSpriteBitMap[SpriteType::SoyBeanB]);
	GetObject(mSpriteBitMap[SpriteType::SoyBeanB], sizeof(BITMAP), &mBitMap[SpriteType::SoyBeanB]);
	Cuts[SpriteType::SoyBeanB] = 6;

	// Bomb
	mSpriteBitMap[SpriteType::BombB] = (HBITMAP)LoadImage(NULL, L"Sprites/Bullets/Bomb.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::BombB], mSpriteBitMap[SpriteType::BombB]);
	GetObject(mSpriteBitMap[SpriteType::BombB], sizeof(BITMAP), &mBitMap[SpriteType::BombB]);
	Cuts[SpriteType::BombB] = 4;

	// Bullet
	mSpriteBitMap[SpriteType::BulletB] = (HBITMAP)LoadImage(NULL, L"Sprites/Bullets/Bullet.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::BulletB], mSpriteBitMap[SpriteType::BulletB]);
	GetObject(mSpriteBitMap[SpriteType::BulletB], sizeof(BITMAP), &mBitMap[SpriteType::BulletB]);

	// 폭탄 이펙트
	mSpriteBitMap[SpriteType::BombEffect] = (HBITMAP)LoadImage(NULL, L"Sprites/Effects/Boooooom.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::BombEffect], mSpriteBitMap[SpriteType::BombEffect]);
	GetObject(mSpriteBitMap[SpriteType::BombEffect], sizeof(BITMAP), &mBitMap[SpriteType::BombEffect]);
	Cuts[SpriteType::BombEffect] = 6;

	// 총 맞는 이펙트
	mSpriteBitMap[SpriteType::BulletEffect] = (HBITMAP)LoadImage(NULL, L"Sprites/Effects/Hited.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::BulletEffect], mSpriteBitMap[SpriteType::BulletEffect]);
	GetObject(mSpriteBitMap[SpriteType::BulletEffect], sizeof(BITMAP), &mBitMap[SpriteType::BulletEffect]);
	Cuts[SpriteType::BulletEffect] = 4;

	// 인터페이스용
	// 체력
	mSpriteBitMap[SpriteType::Hearts] = (HBITMAP)LoadImage(NULL, L"Sprites/InterFace/Heart.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::Hearts], mSpriteBitMap[SpriteType::Hearts]);
	GetObject(mSpriteBitMap[SpriteType::Hearts], sizeof(BITMAP), &mBitMap[SpriteType::Hearts]);
	Cuts[SpriteType::Hearts] = 8;

	// 체력 뒷 칸
	mSpriteBitMap[SpriteType::HeartsBack] = (HBITMAP)LoadImage(NULL, L"Sprites/InterFace/Selection.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::HeartsBack], mSpriteBitMap[SpriteType::HeartsBack]);
	GetObject(mSpriteBitMap[SpriteType::HeartsBack], sizeof(BITMAP), &mBitMap[SpriteType::HeartsBack]);

	// 콩알탄 소지
	mSpriteBitMap[SpriteType::SoyBeanCnt] = (HBITMAP)LoadImage(NULL, L"Sprites/InterFace/SoyBeanCnt.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::SoyBeanCnt], mSpriteBitMap[SpriteType::SoyBeanCnt]);
	GetObject(mSpriteBitMap[SpriteType::SoyBeanCnt], sizeof(BITMAP), &mBitMap[SpriteType::SoyBeanCnt]);

	// 폭탄 소지
	mSpriteBitMap[SpriteType::BombCnt] = (HBITMAP)LoadImage(NULL, L"Sprites/InterFace/BombCnt.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::BombCnt], mSpriteBitMap[SpriteType::BombCnt]);
	GetObject(mSpriteBitMap[SpriteType::BombCnt], sizeof(BITMAP), &mBitMap[SpriteType::BombCnt]);

	// 총알 소지
	mSpriteBitMap[SpriteType::BulletCnt] = (HBITMAP)LoadImage(NULL, L"Sprites/InterFace/BulletCnt.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::BulletCnt], mSpriteBitMap[SpriteType::BulletCnt]);
	GetObject(mSpriteBitMap[SpriteType::BulletCnt], sizeof(BITMAP), &mBitMap[SpriteType::BulletCnt]);

	// 표적
	mSpriteBitMap[SpriteType::CrossHair] = (HBITMAP)LoadImage(NULL, L"Sprites/crosshair.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	oldBitMap = (HBITMAP)SelectObject(mSpriteDC[SpriteType::CrossHair], mSpriteBitMap[SpriteType::CrossHair]);
	GetObject(mSpriteBitMap[SpriteType::CrossHair], sizeof(BITMAP), &mBitMap[SpriteType::CrossHair]);
}

GameSprite::~GameSprite()
{
	for (int i = 0; i < SpriteType::SpriteCount; i++)
	{
		if (mSpriteBitMap[i] != NULL)
			DeleteObject(mSpriteBitMap[i]);
		if (mSpriteDC[i] != NULL)
			DeleteDC(mSpriteDC[i]);
	}
}

void GameSprite::DrawIMG(HDC hdc, std::wstring Strs, bool isLeft, float x, float y, float Width, float Height)
{
	HDC MemDC = CreateCompatibleDC(hdc);
	BITMAP BitMap;
	HBITMAP MyBitMap = (HBITMAP)LoadImage(NULL, Strs.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HBITMAP oldBitMap = (HBITMAP)SelectObject(MemDC, MyBitMap);

	GetObject(MyBitMap, sizeof(BITMAP), &BitMap);

	if (isLeft)
		StretchBlt(MemDC, 0, BitMap.bmHeight, BitMap.bmWidth, -BitMap.bmHeight, MemDC, 0, 0, BitMap.bmWidth, BitMap.bmHeight, SRCCOPY);
	else
		StretchBlt(MemDC, BitMap.bmWidth, BitMap.bmHeight, -BitMap.bmWidth, -BitMap.bmHeight, MemDC, 0, 0, BitMap.bmWidth, BitMap.bmHeight, SRCCOPY);

	TransparentBlt(hdc, x, y, Width, Height, MemDC, 0, 0, BitMap.bmWidth, BitMap.bmHeight, RGB(255, 255, 255));
	SelectObject(MemDC, oldBitMap);
	DeleteObject(MyBitMap);
	DeleteDC(MemDC);
}

void GameSprite::DrawIMG(HDC hdc, SpriteType Type, bool isLeft, float x, float y, float Width, float Height)
{
	if (isLeft)
		StretchBlt(mSpriteDC[Type], 0, mBitMap[Type].bmHeight, mBitMap[Type].bmWidth, -mBitMap[Type].bmHeight, mSpriteDC[Type], 
			0, 0, mBitMap[Type].bmWidth, mBitMap[Type].bmHeight, SRCCOPY);
	else
		StretchBlt(mSpriteDC[Type], mBitMap[Type].bmWidth, mBitMap[Type].bmHeight, -mBitMap[Type].bmWidth, -mBitMap[Type].bmHeight, mSpriteDC[Type], 
			0, 0, mBitMap[Type].bmWidth, mBitMap[Type].bmHeight, SRCCOPY);

	TransparentBlt(hdc, x, y, Width, Height, mSpriteDC[Type], 0, 0, mBitMap[Type].bmWidth, mBitMap[Type].bmHeight, RGB(255, 255, 255));

	if (isLeft)
		StretchBlt(mSpriteDC[Type], 0, mBitMap[Type].bmHeight, mBitMap[Type].bmWidth, -mBitMap[Type].bmHeight, mSpriteDC[Type],
			0, 0, mBitMap[Type].bmWidth, mBitMap[Type].bmHeight, SRCCOPY);
	else
		StretchBlt(mSpriteDC[Type], mBitMap[Type].bmWidth, mBitMap[Type].bmHeight, -mBitMap[Type].bmWidth, -mBitMap[Type].bmHeight, mSpriteDC[Type],
			0, 0, mBitMap[Type].bmWidth, mBitMap[Type].bmHeight, SRCCOPY);
}

void GameSprite::Text(HDC hdc, char Letter, float x, float y, float Width, float Height)
{
	int GridX = mBitMap[SpriteType::String].bmWidth / 10, GridY = mBitMap[SpriteType::String].bmHeight / 5, X = 0, Y = 0;

	if ('a' <= Letter && Letter <= 'z')
	{
		int Pos = Letter - 'a';
		int Px = Pos % 10, Py = Pos / 10;
		X = GridX * Px; Y = GridY * (4 - Py);
	}
	else if ('0' <= Letter && Letter <= '9')
	{
		int Pos = Letter - '0';
		int Px = Pos % 10;
		X = GridX * Px; Y = GridY;
	}
	else {
		char Ary[9] = { '.',',',':','?','!','(',')','+','-' };
		int i = 0;
		for (; i < 9; i++)
		{
			if (Ary[i] == Letter) {
				X = GridX * i;
				break;
			}
		}
		if (i == 9) {
			X = GridX * 8; Y = GridY * 2;
		}
	}
	TransparentBlt(hdc, x, y, Width, Height, mSpriteDC[SpriteType::String], X, Y, GridX, GridY, RGB(255, 255, 255));
}

void GameSprite::SpriteString(HDC hdc, std::wstring Strs, float x, float y, float hSize)
{
	int X = x;
	for (int i = 0; i < Strs.size(); i++)
	{
		Text(hdc, Strs[i], X + hSize * i, y, hSize, hSize);
	}
}

void GameSprite::ScenterString(HDC hdc, std::wstring Strs, float x, float y, float hSize)
{
	int X = x - Strs.size() * hSize / 2;
	for (int i = 0; i < Strs.size(); i++)
	{
		Text(hdc, Strs[i], X + hSize * i, y, hSize, hSize);
	}
}

void GameSprite::Animation(HDC hdc, bool isLeft, SpriteType Type, int& AnimFrame, float x, float y, float Width, float Height)
{
	AnimFrame++;
	int CutWidth = mBitMap[Type].bmWidth / Cuts[Type];
	int CutNum = (AnimFrame / cAnimeFrame) % Cuts[Type];

	if (isLeft)
		StretchBlt(mSpriteDC[Type], 0, mBitMap[Type].bmHeight, mBitMap[Type].bmWidth, -mBitMap[Type].bmHeight, 
			mSpriteDC[Type], 0, 0, mBitMap[Type].bmWidth, mBitMap[Type].bmHeight, SRCCOPY);
	else
		StretchBlt(mSpriteDC[Type], mBitMap[Type].bmWidth, mBitMap[Type].bmHeight, -mBitMap[Type].bmWidth, -mBitMap[Type].bmHeight, 
			mSpriteDC[Type], 0, 0, mBitMap[Type].bmWidth, mBitMap[Type].bmHeight, SRCCOPY);

	TransparentBlt(hdc, x, y, Width, Height, mSpriteDC[Type], CutNum * CutWidth, 0, CutWidth, mBitMap[Type].bmHeight, RGB(255, 255, 255));

	if (isLeft)
		StretchBlt(mSpriteDC[Type], 0, mBitMap[Type].bmHeight, mBitMap[Type].bmWidth, -mBitMap[Type].bmHeight, 
			mSpriteDC[Type], 0, 0, mBitMap[Type].bmWidth, mBitMap[Type].bmHeight, SRCCOPY);
	else
		StretchBlt(mSpriteDC[Type], mBitMap[Type].bmWidth, mBitMap[Type].bmHeight, -mBitMap[Type].bmWidth, -mBitMap[Type].bmHeight, 
			mSpriteDC[Type], 0, 0, mBitMap[Type].bmWidth, mBitMap[Type].bmHeight, SRCCOPY);
}

void GameSprite::Animation(HDC hdc, bool isLeft, SpriteType Type, int& AnimFrame, float x, float y, float Width, float Height, bool &remove)
{
	AnimFrame++;
	int CutWidth = mBitMap[Type].bmWidth / Cuts[Type];
	int CutNum = (AnimFrame / cAnimeFrame) % Cuts[Type];

	if (CutNum == Cuts[Type] - 1)
		remove = true;

	if (isLeft)
		StretchBlt(mSpriteDC[Type], 0, mBitMap[Type].bmHeight, mBitMap[Type].bmWidth, -mBitMap[Type].bmHeight, mSpriteDC[Type], 0, 0, mBitMap[Type].bmWidth, mBitMap[Type].bmHeight, SRCCOPY);
	else
		StretchBlt(mSpriteDC[Type], mBitMap[Type].bmWidth, mBitMap[Type].bmHeight, -mBitMap[Type].bmWidth, -mBitMap[Type].bmHeight, mSpriteDC[Type], 0, 0, mBitMap[Type].bmWidth, mBitMap[Type].bmHeight, SRCCOPY);

	TransparentBlt(hdc, x, y, Width, Height, mSpriteDC[Type], CutNum * CutWidth, 0, CutWidth, mBitMap[Type].bmHeight, RGB(255, 255, 255));

	if (isLeft)
		StretchBlt(mSpriteDC[Type], 0, mBitMap[Type].bmHeight, mBitMap[Type].bmWidth, -mBitMap[Type].bmHeight, mSpriteDC[Type], 0, 0, mBitMap[Type].bmWidth, mBitMap[Type].bmHeight, SRCCOPY);
	else
		StretchBlt(mSpriteDC[Type], mBitMap[Type].bmWidth, mBitMap[Type].bmHeight, -mBitMap[Type].bmWidth, -mBitMap[Type].bmHeight, mSpriteDC[Type], 0, 0, mBitMap[Type].bmWidth, mBitMap[Type].bmHeight, SRCCOPY);
}

void GameSprite::DrawTile(HDC hdc, TileType Type, float x, float y, float tSize)
{
	if (Type == TileType::Border) {
		TransparentBlt(hdc, x, y, tSize, tSize, mSpriteDC[SpriteType::Tile], 113, 129, 32, 32, RGB(255, 255, 255));
	}
	else if (Type == TileType::Block) {
		TransparentBlt(hdc, x, y, tSize, tSize, mSpriteDC[SpriteType::Tile], 209, 1, 48, 48, RGB(255, 255, 255));
	}
	else if (Type == TileType::OneWay) {
		TransparentBlt(hdc, x, y, tSize, tSize, mSpriteDC[SpriteType::Tile], 209, 65, 48, 48, RGB(255, 255, 255));
	}
	else if (Type == TileType::Door) {
		// TransparentBlt(hdc, x, y, tSize, tSize, mSpriteDC[SpriteType::Tile], 113, 1, 32, 32, RGB(255, 255, 255));
		TransparentBlt(hdc, x, y, tSize, tSize, mSpriteDC[SpriteType::DoorTile], 27, 58, 8, 6, RGB(255, 255, 255));
	}
	else if (Type == TileType::Empty) {
		//TransparentBlt(hdc, x, y, tSize, tSize, mSpriteDC[SpriteType::Tile], 35, 68, 25, 25, RGB(255, 255, 255));
		TransparentBlt(hdc, x, y, tSize, tSize, mSpriteDC[SpriteType::BackTile2], 0, 0, 64, 64, RGB(255, 255, 255));
	}
}