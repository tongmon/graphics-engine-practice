#include "BaseGame.h"

BaseGame::BaseGame()
{
	srand((unsigned int)time(0));
	mLastMousePos = { -1,-1 };
	mNextSceneOffset = { 0,0 };
	mBackButtonOffset = { 0,0 };
	mCurStageNum = 0;
	mSceneChangeStance = false;
	mScore = 0;
	mPrevGameState = GAMESTATE::INIT;
	mMaxEnemyDropTime[0] = 280;
	mMaxEnemyDropTime[1] = 260;
	mMaxEnemyDropTime[2] = 220;
	mMaxEnemyDropTime[3] = 200;
	mMaxEnemyDropTime[4] = 200;
	mMaxEnemyDropTime[5] = 200;
	mEnemyRange = 0;

	/*
	std::string BGSoundFile[] = { "Sounds\\BGSound\\MainBG.mp3", "Sounds\\BGSound\\StageBG.mp3"  };
	std::string EffectSoundFile[] = { "Sounds\\EffSound\\buttonclick.ogg", "Sounds\\EffSound\\dooropen.ogg", "Sounds\\EffSound\\footstep.ogg",
	"Sounds\\EffSound\\jump.wav", "Sounds\\EffSound\\shoot.mp3", "Sounds\\EffSound\\soybeanhit.ogg" ,
	"Sounds\\EffSound\\bullethit.ogg", "Sounds\\EffSound\\bombhit.wav", "Sounds\\EffSound\\ending.mp3", "Sounds\\EffSound\\gameover.wav",
	"Sounds\\EffSound\\pickup.wav", "Sounds\\EffSound\\hitby.wav",  "Sounds\\EffSound\\Brick1.ogg", "Sounds\\EffSound\\Brick2.ogg",
	"Sounds\\EffSound\\foedie.wav" };
	mGameSound.CreateBGSound(BGSoundType::BGSoundCount, BGSoundFile);
	mGameSound.CreateEffectSound(EffectSoundType::EffSoundCount, EffectSoundFile);
	*/
}

BaseGame::~BaseGame()
{
	for (int i = 0; i < mStages.size(); i++)
	{
		delete mStages.at(i);
	}

	for (auto iter = mItemList.begin(); iter != mItemList.end(); iter++)
	{
		delete *iter;
	}

	for (auto iter = mEnemyList.begin(); iter != mEnemyList.end(); iter++)
	{
		delete* iter;
	}

	for (auto iter = mEffectList.begin(); iter != mEffectList.end(); iter++)
	{
		delete* iter;
	}

	mGameSound.ReleaseSound();
}

void BaseGame::OnInit()
{
	if (mGameState == GAMESTATE::INIT) {

		mGameSound.ReleaseSound();

		std::string BGSoundFile[] = { "Sounds\\BGSound\\MainBG.mp3", "Sounds\\BGSound\\StageBG.mp3" };
		std::string EffectSoundFile[] = { "Sounds\\EffSound\\buttonclick.ogg", "Sounds\\EffSound\\dooropen.ogg", "Sounds\\EffSound\\footstep.ogg",
		"Sounds\\EffSound\\jump.wav", "Sounds\\EffSound\\shoot.mp3", "Sounds\\EffSound\\soybeanhit.ogg" ,
		"Sounds\\EffSound\\bullethit.ogg", "Sounds\\EffSound\\bombhit.wav", "Sounds\\EffSound\\ending.mp3", "Sounds\\EffSound\\gameover.wav",
		"Sounds\\EffSound\\pickup.wav", "Sounds\\EffSound\\hitby.wav",  "Sounds\\EffSound\\Brick1.ogg", "Sounds\\EffSound\\Brick2.ogg",
		"Sounds\\EffSound\\foedie.wav" };
		mGameSound.CreateBGSound(BGSoundType::BGSoundCount, BGSoundFile);
		mGameSound.CreateEffectSound(EffectSoundType::EffSoundCount, EffectSoundFile);

		mNextSceneOffset = { 0,0 };
		mBackButtonOffset = { 0,0 };
		mCurStageNum = 0;
		mSceneChangeStance = false;
		mScore = 0;
		mCurEnemyDropTime = 0;
		mEnemyRange = 0;
		mPrevGameState = GAMESTATE::INIT;

		for (int i = 0; i < mStages.size(); i++)
			delete mStages.at(i);

		for (auto iter = mItemList.begin(); iter != mItemList.end(); iter++)
			delete* iter;

		for (auto iter = mEnemyList.begin(); iter != mEnemyList.end(); iter++)
			delete* iter;

		for (auto iter = mEffectList.begin(); iter != mEffectList.end(); iter++)
			delete* iter;

		mStages.clear(); mItemList.clear(); mEnemyList.clear();

		std::vector<std::vector<std::vector<short>>> Grid =
		{
			{
				{ 1,1,1,1,1,1,1,1,1,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
			},

			{
				{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,1,1,1,1,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
			},

			{
				{ 1,1,1,1,1,1,1,1,3,3,1,1,1,1,1,1,1,1,1,1,1,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
			},

			{
				{ 1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
				{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
			},
		};

		std::vector<std::vector<short>> EndStage =
		{
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
			{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
			{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
			{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
			{ 1,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,1 },
			{ 1,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1 },
			{ 1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1 },
			{ 1,0,0,0,2,0,0,0,0,0,0,0,0,0,2,0,0,0,1 },
			{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
			{ 1,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,1 },
			{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
			{ 1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1 },
			{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
			{ 1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1 },
			{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
			{ 1,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
		};

		Grid.push_back(EndStage);
		
		// 스테이지들 초기화
		for (int i = 0; i < Grid.size(); i++)
		{
			GameMap* Stage = new GameMap;
			Stage->InitMap(&Grid.at(i), &mSprite);
			mStages.push_back(Stage);
		}
		mStages.back()->mEndStage = true;

		// 플레이어 초기화
		mPlayer.InitCharacter({ 0, 0 }, { cPlayerWidth, cPlayerHeight }, mStages.at(mCurStageNum), &mSprite, cPlayerWalkSpeed, cPlayerJumpSpeed);
		mPlayer.mEnemyList = &mEnemyList;
		mPlayer.mEffList = &mEffectList;
		mPlayer.mGameSound = &mGameSound;

		//mGameSound.StopSoundBG(BGSoundType::Stage);
		mGameSound.PlaySoundBG(BGSoundType::Main);
	}
	else if (mGameState == GAMESTATE::HOWTO) {
		// 뒤로가기 버튼 생성
		mHowToStringOffset = { mBaseWidth / 2.f + mNextSceneOffset.x, mBaseHeight / 2.f + 250 + mNextSceneOffset.y };
		mBackButton.InitObj({ mBaseWidth / 2.f + mNextSceneOffset.x, mBaseHeight / 2.f + mNextSceneOffset.y - 200 }, { 50, 50 }, & mSprite);
	}
	/*
	else if (mGameState == GAMESTATE::OPTION) {
		// 뒤로가기 버튼 생성
		mBackButton.InitObj({ mBaseWidth / 2.f + mNextSceneOffset.x, mBaseHeight / 2.f + mNextSceneOffset.y }, { 50, 50 }, & mSprite);
	}
	*/
	else if (mGameState == GAMESTATE::MAINMENU) {
		// 메뉴화면 버튼 생성
		mMainStringOffset = { mBaseWidth / 2.f + mNextSceneOffset.x - 370, mBaseHeight / 2.f + 250 + mNextSceneOffset.y };
		mPlayButton.InitObj({ mBaseWidth / 2.f + mNextSceneOffset.x,mBaseHeight / 2.f + 120 + mNextSceneOffset.y }, { 80, 80 }, & mSprite);
		mHowToPlayButton.InitObj({ mBaseWidth / 2.f + mNextSceneOffset.x,mBaseHeight / 2.f + mNextSceneOffset.y - 60 }, { 80, 80 }, & mSprite);
		// mOptionButton.InitObj({ mBaseWidth / 2.f + mNextSceneOffset.x, mBaseHeight / 2.f - 150 + mNextSceneOffset.y }, { 50, 50 }, & mSprite);
		mQuitButton.InitObj({ mBaseWidth / 2.f + mNextSceneOffset.x,mBaseHeight / 2.f - 250 + mNextSceneOffset.y }, { 80, 80 }, & mSprite);
	}
	else if (mGameState == GAMESTATE::RUN) {
		if (mPrevGameState != GAMESTATE::RUN)
		{
			mPrevGameState = GAMESTATE::RUN;
			mGameSound.StopSoundBG(BGSoundType::Main);
			mGameSound.PlaySoundBG(BGSoundType::Stage);
		}

		// 멈춤 버튼 초기화
		mPauseButton.InitObj({ float(mBaseWidth - 100), float(mBaseHeight - 80) }, { 30,30 }, & mSprite);

		// 새로운 맵에 대한 플레이어 설정, 플레이어 스텟 초기화
		mPlayer.SetMap(mStages.at(mCurStageNum));
		POINT Pos = { mStages[mCurStageNum]->mWidth / 2, 1 };
		POINTFLOAT TilePos = mStages[mCurStageNum]->GetMapTilePosition(Pos);
		TilePos.y += mPlayer.mAABB.halfSize.y;
		mPlayer.SetPos(TilePos); // 플레이어 위치 조정
		mPlayer.mBullets.clear();

		// 아이템, 적 초기화
		for (auto iter = mItemList.begin(); iter != mItemList.end(); iter++)
			delete* iter;
		mItemList.clear();
		for (auto iter = mEnemyList.begin(); iter != mEnemyList.end(); iter++)
			delete* iter;
		mEnemyList.clear();

		// 새로운 맵에 도달할 때 마다 난이도 조절관련 변수 설정
		mCurEnemyDropTime = mMaxEnemyDropTime[mCurStageNum] / 2;
		mEnemyRange = min(mEnemyRange + 1, (int)EnemyType::EnemCount);
	}
	else if (mGameState == GAMESTATE::STOP) {
		mResumeButton.InitObj({ mBaseWidth / 2.f,mBaseHeight / 2.f + 50}, { 50, 50 }, & mSprite);
		mHomeButton.InitObj({ mBaseWidth / 2.f,mBaseHeight / 2.f - 100}, { 50, 50 }, & mSprite);
	}
	else if (mGameState == GAMESTATE::GAMEOVER) {
		mBackButtonOffset.y = 720;
		// 뒤로가기 버튼 생성
		mBackButton.InitObj({ mBaseWidth / 2.f + mBackButtonOffset.x, mBackButtonOffset.y }, { 50, 50 }, & mSprite);
	}
	else if (mGameState == GAMESTATE::END) {
		mBackButtonOffset.y = 720;
		// 뒤로가기 버튼 생성
		mBackButton.InitObj({ mBaseWidth / 2.f + mBackButtonOffset.x, mBackButtonOffset.y }, { 50, 50 }, & mSprite);
	}
}

void BaseGame::OnRender()
{
	RECT Wnd = { 0,mBitMapSize.y,mBitMapSize.x,0 };

	mSprite.DrawIMG(mPaperDC, SpriteType::BackGround, true, 0, 0, mBaseWidth, mBaseHeight);
	mSprite.DrawIMG(mPaperDC, SpriteType::BackGround, true, 0, mBaseHeight, mBaseWidth, mBaseHeight);

	if (mGameState == GAMESTATE::INIT) {

	}
	else if (mGameState == GAMESTATE::HOWTO) {
		if (mSceneChangeStance) {
			Scene(GAMESTATE::MAINMENU);

			Scene(GAMESTATE::HOWTO);
		}
		else {
			Scene(GAMESTATE::HOWTO);
		}
	}
	/*
	else if (mGameState == GAMESTATE::OPTION) {
		if (mSceneChangeStance) {
			Scene(GAMESTATE::MAINMENU);

			Scene(GAMESTATE::OPTION);
		}
		else {
			Scene(GAMESTATE::OPTION);
		}
	}
	*/
	else if (mGameState == GAMESTATE::MAINMENU) {
		if (mSceneChangeStance) {
			if (mPrevGameState == GAMESTATE::HOWTO)
				Scene(GAMESTATE::HOWTO);
			/*
			else if (mPrevGameState == GAMESTATE::OPTION)
				Scene(GAMESTATE::OPTION);
				*/
			else if (mPrevGameState == GAMESTATE::END)
				Scene(GAMESTATE::END);
			else if (mPrevGameState == GAMESTATE::GAMEOVER)
				Scene(GAMESTATE::GAMEOVER);
			else if (mPrevGameState == GAMESTATE::STOP)
				Scene(GAMESTATE::STOP);

			Scene(GAMESTATE::MAINMENU);
		}
		else {
			// 메뉴 화면에 대한 바탕, 버튼 출력
			Scene(GAMESTATE::MAINMENU);
		}
	}
	else if (mGameState == GAMESTATE::RUN) {
		if (mSceneChangeStance) {
			if (mPrevGameState == GAMESTATE::RUN) {
				mStages[mCurStageNum]->SetHDC(mPaperDC);
				mStages[mCurStageNum]->DrawMap();

				mStages[mCurStageNum + 1]->SetHDC(mPaperDC);
				mStages[mCurStageNum + 1]->DrawMap();
			}
			else if(mPrevGameState == GAMESTATE::MAINMENU) {
				Scene(GAMESTATE::MAINMENU);

				mStages[mCurStageNum]->SetHDC(mPaperDC);
				mStages[mCurStageNum]->DrawMap();
			}
		}
		else {
			int buf;
			ShowCharacterInterFace();
			mPauseButton.SetHDC(mPaperDC);
			mPauseButton.DrawObject(SpriteType::OptionButton, false, buf);

			// 맵 출력
			mStages[mCurStageNum]->SetHDC(mPaperDC);
			mStages[mCurStageNum]->DrawMap();

			// 플레이어 출력
			mPlayer.SetHDC(mPaperDC);
			//mPlayer.DrawObject();
			mPlayer.DrawCharacter();
			mPlayer.DrawBullets();

			// 블럭 놓기 가능한 위치 출력
			ShowBrickPos();

			// 아이템 출력
			for (auto& Item : mItemList) {
				Item->SetHDC(mPaperDC);
				Item->DrawItem();
			}

			// 적 출력
			for (auto& Bots : mEnemyList) {
				Bots->SetHDC(mPaperDC);
				Bots->DrawEnemy();
			}
		}
	}
	else if (mGameState == GAMESTATE::STOP) {
		Scene(GAMESTATE::STOP);
	}
	else if (mGameState == GAMESTATE::GAMEOVER) {
		Scene(GAMESTATE::GAMEOVER);
	}
	else if (mGameState == GAMESTATE::END) {
		Scene(GAMESTATE::END);
	}

	// 이펙트 출력
	for (auto& Eff : mEffectList) {
		Eff->DrawEffects(mPaperDC);
	}
}

void BaseGame::OnUpdate()
{
	float DeltaT = mTimer.DeltaTime(); // 델타 타임

	if (mSceneChangeStance) // 장면 전환
	{
		OnSceneUpdate();
		return;
	}

	auto iterEff = mEffectList.begin();
	while (iterEff != mEffectList.end())
	{
		if ((*iterEff)->mRemove == true) {
			delete* iterEff;
			iterEff = mEffectList.erase(iterEff);
			if (iterEff != mEffectList.begin())
				iterEff--;
		}
		else
			iterEff++;
	}

	if (mGameState == GAMESTATE::INIT) {
		mGameState = GAMESTATE::MAINMENU;
		OnInit();
	}
	else if (mGameState == GAMESTATE::HOWTO) {

	}
	else if (mGameState == GAMESTATE::OPTION) {

	}
	else if (mGameState == GAMESTATE::MAINMENU) {

	}
	else if (mGameState == GAMESTATE::RUN) {

		mPlayer.GetDeltaTime(DeltaT);
		mPlayer.ReSetInputs();

		// 최대 적 개수 이하이고 마지막 스테이지가 아니며 소환 시간이 되었을 경우 적 소환
		mCurEnemyDropTime++;
		if (mEnemyList.size() < 20 && mCurStageNum + 1 != mStages.size() && mCurEnemyDropTime > mMaxEnemyDropTime[mCurStageNum])
		{
			mCurEnemyDropTime = 0;
			GameBot *EnemyBuffer = new GameBot;
			POINTFLOAT dropPos = mStages[mCurStageNum]->GetMapTilePosition(rand() % (mStages[mCurStageNum]->mWidth - 2) + 1, mStages[mCurStageNum]->mHeight - 1);
			EnemyBuffer->InitBot(EnemyType(rand() % mEnemyRange), mStages[mCurStageNum], &mSprite, { dropPos.x,dropPos.y }, &mPlayer);
			mEnemyList.push_back(EnemyBuffer);
		}

		if (GetAsyncKeyState('A') < 0)
		{
			mPlayer.SetInputs(KeyInput::GoLeft);
		}
		if (GetAsyncKeyState('D') < 0)
		{
			mPlayer.SetInputs(KeyInput::GoRight);
		}
		if (GetAsyncKeyState('W') < 0)
		{

		}
		if (GetAsyncKeyState('S') < 0)
		{
			mPlayer.SetInputs(KeyInput::GoDown);
		}
		if (GetAsyncKeyState(VK_SPACE) & 0x8001)
		{
			mPlayer.SetInputs(KeyInput::Jump);
		}

		if (GetAsyncKeyState('S') < 0)
		{

		}

		// 캐릭터 업데이트
		mPlayer.CharacterUpdate();
		if (mPlayer.mCurHealth <= 0) // 캐릭터 체력 모두 소진
		{
			// 소리
			mGameSound.StopSoundBG(BGSoundType::Stage);
			mGameSound.PlaySoundEffect(EffectSoundType::GameOverSound);
			
			// 캐릭터 사망시 이펙트
			GameEffects* NewEff = new GameEffects;
			NewEff->InitEffects(mPlayer.mPosition, EffectType::CharacterDieEff, &mSprite);
			mEffectList.push_back(NewEff);

			mGameState = GAMESTATE::GAMEOVER;
			OnInit();
			return;
		}
		POINT Pos = mStages[mCurStageNum]->GetMapTileAtPoint(mPlayer.mPosition);
		if (mStages[mCurStageNum]->GetTile(Pos.x, Pos.y) == TileType::Door) // 문에 도달 시 다음 스테이지로 이동
		{
			if (mCurStageNum + 1 == mStages.size()) // 끝 스테이지까지 도달했다면 게임 끝
			{
				// 소리
				mGameSound.StopSoundBG(BGSoundType::Stage);
				mGameSound.PlaySoundEffect(EffectSoundType::ClearSound);
				
				// 캐릭터 클리어 이펙트
				GameEffects* NewEff = new GameEffects;
				NewEff->InitEffects(mPlayer.mPosition, EffectType::CharacterDieEff, &mSprite);
				mEffectList.push_back(NewEff);

				mScore += 500;
				
				mGameState = GAMESTATE::END;
				OnInit();
			}
			else {
				// 소리
				mGameSound.PlaySoundEffect(EffectSoundType::DoorOpen);

				mScore += 300;
				mSceneChangeStance = true;
				mStages[mCurStageNum + 1]->mStPositon.y += mBaseHeight;
				mBitMapSize.y += mBaseHeight;
			}
			return;
		}

		// 아이템 업데이트
		for (auto& Item : mItemList) {
			Item->UpdateItem(DeltaT);
		}

		// 플레이어가 주은 아이템, 사라질 시간이 된 아이템 삭제
		auto itemIter = mItemList.begin();
		while (itemIter != mItemList.end())
		{
			if ((*itemIter)->mFadeTime <= 0) // 사라질 시간이 된 아이템 삭제
			{
				// 이펙트
				GameEffects* NewEff = new GameEffects;
				NewEff->InitEffects((*itemIter)->mPosition, EffectType::CollectEff, &mSprite);
				mEffectList.push_back(NewEff);
				
				delete* itemIter;
				itemIter = mItemList.erase(itemIter);
				if (itemIter != mItemList.begin())
					itemIter--;
			}
			else if ((*itemIter)->mAABB.Overlaps(mPlayer.mAABB)) // 플레이어가 아이템 획득
			{
				// 이펙트
				GameEffects* NewEff = new GameEffects;
				NewEff->InitEffects((*itemIter)->mPosition, EffectType::CollectEff, &mSprite);
				mEffectList.push_back(NewEff);
				
				// 소리
				mGameSound.PlaySoundEffect(EffectSoundType::ItemGet);
				
				// 해당되는 아이템에 대한 변수 수정
				mPlayer.mSolidBlockCnt = min(mPlayer.mMaxSolidBlock, mPlayer.mSolidBlockCnt + (*itemIter)->mSolidBrickCnt);
				mPlayer.mOneWayBlockCnt = min(mPlayer.mMaxOneWayBlock, mPlayer.mOneWayBlockCnt + (*itemIter)->mOneWayBrickCnt);
				for (int i = 1; i < (int)BulletType::BulletCount; i++)
				{
					mPlayer.mCurBullets[i] = min(mPlayer.mMaxBullets[i], mPlayer.mCurBullets[i] + (*itemIter)->mBulletCnt[i]);
				}
				mPlayer.mCurHealth = min(mPlayer.mMaxHealth, mPlayer.mCurHealth + (*itemIter)->mHealCnt);
				
				delete* itemIter;
				itemIter = mItemList.erase(itemIter);
				if (itemIter != mItemList.begin())
					itemIter--;
			}
			else
				itemIter++;
		}

		// 적이 플레이어 따라오게 만듦
		for (auto& Bot : mEnemyList) {
			Bot->MoveToHero();
		}

		// 적 업데이트
		for (auto& Bots : mEnemyList) {
			Bots->GetDeltaTime(DeltaT);
			Bots->BotUpdate();
		}

		// 체력 0 이하인 적들 삭제
		auto iter = mEnemyList.begin();
		while (iter != mEnemyList.end())
		{
			if ((*iter)->mCurHealth <= 0) {
				// 적 사망시 이펙트
				GameEffects* NewEff = new GameEffects;
				NewEff->InitEffects((*iter)->mPosition, EffectType::EnemyDieEff, &mSprite);
				mEffectList.push_back(NewEff);

				// 적 사망시 아이템 드랍
				GameItem* Newitem = new GameItem;
				Newitem->SetItem(rand() % 100, (*iter)->mPosition, (*iter)->mMap, &mSprite); // 죽을때 아이템 드랍
				mItemList.push_back(Newitem);

				// 죽는 소리
				mGameSound.PlaySoundEffect(EffectSoundType::EnemyDieS);

				// 점수 설정
				if ((*iter)->mEType == EnemyType::Grunt)
					mScore += 20;
				else if((*iter)->mEType == EnemyType::Heavy)
					mScore += 40;
				else if ((*iter)->mEType == EnemyType::Massive)
					mScore += 150;
				else if ((*iter)->mEType == EnemyType::Ninja)
					mScore += 100;

				delete* iter;
				iter = mEnemyList.erase(iter);
				if (iter != mEnemyList.begin())
					iter--;
			}
			else
				iter++;
		}
	}
	else if (mGameState == GAMESTATE::STOP) {

	}
	else if (mGameState == GAMESTATE::GAMEOVER) {
		float Rad = getRad(90 * (mBaseHeight - mBackButtonOffset.y) / (mBaseHeight / 2.f));
		float Speed = max(cosf(Rad) * 1000, 120);
		mBackButtonOffset.y = max(mBackButtonOffset.y - Speed * DeltaT, mBaseHeight / 2.f);

		// 적 업데이트
		for (auto& Bots : mEnemyList) {
			Bots->GetDeltaTime(DeltaT);
			Bots->BotUpdate();
		}

		// 아이템 업데이트
		for (auto& Item : mItemList) {
			Item->UpdateItem(DeltaT);
		}
	}
	else if (mGameState == GAMESTATE::END) {
		float Rad = getRad(90 * (mBaseHeight - mBackButtonOffset.y) / (mBaseHeight / 2.f));
		float Speed = max(cosf(Rad) * 1000, 120);
		mBackButtonOffset.y = max(mBackButtonOffset.y - Speed * DeltaT, mBaseHeight / 2.f);
	}

	mGameSound.Update();
}

void BaseGame::OnRelease()
{
	
}

void BaseGame::OnResize()
{

}

void BaseGame::Scene(GAMESTATE State)
{
	int buf;
	if (State == GAMESTATE::MAINMENU) {
		SpriteString(mPaperDC, L"to the top!", mMainStringOffset.x, mMainStringOffset.y, 70);
		mPlayButton.SetHDC(mPaperDC);
		mPlayButton.DrawObject(SpriteType::PlayButton, false, buf);
		mHowToPlayButton.SetHDC(mPaperDC);
		mHowToPlayButton.DrawObject(SpriteType::HowToButton, false, buf);
		//mOptionButton.SetHDC(mPaperDC);
		//mOptionButton.DrawObject(SpriteType::OptionButton, false, buf);
		mQuitButton.SetHDC(mPaperDC);
		mQuitButton.DrawObject(SpriteType::QuitButton, false, buf);
	}
	else if (State == GAMESTATE::HOWTO) {
		ScenterString(mPaperDC, L"how to play!", mHowToStringOffset.x, mHowToStringOffset.y, 50);
		ScenterString(mPaperDC, L"w,a,d for basic moving!", mHowToStringOffset.x, mHowToStringOffset.y - 100, 30);
		ScenterString(mPaperDC, L"right, middle click to dispose brick!", mHowToStringOffset.x, mHowToStringOffset.y - 170, 30);
		ScenterString(mPaperDC, L"left click for shot to kill.", mHowToStringOffset.x, mHowToStringOffset.y - 240, 30);
		ScenterString(mPaperDC, L"and finally try hard to go up!", mHowToStringOffset.x, mHowToStringOffset.y - 310, 30);
		mBackButton.SetHDC(mPaperDC);
		mBackButton.DrawObject(SpriteType::BackButton, false, buf);
	}
	/*
	else if (State == GAMESTATE::OPTION) {
		mBackButton.SetHDC(mPaperDC);
		mBackButton.DrawObject(SpriteType::BackButton, false, buf);
	}
	*/
	else if (State == GAMESTATE::END || State == GAMESTATE::GAMEOVER) {
		mStages[mCurStageNum]->SetHDC(mPaperDC);
		mStages[mCurStageNum]->DrawMap();

		// 아이템 출력
		for (auto& Item : mItemList) {
			Item->SetHDC(mPaperDC);
			Item->DrawItem();
		}

		// 적 출력
		for (auto& Bots : mEnemyList) {
			Bots->SetHDC(mPaperDC);
			Bots->DrawEnemy();
		}

		// 점수판 살짝 내려오면서 출력
		std::wstring Str = L"score: ";
		Str += std::to_wstring(mScore);
		ScenterString(mPaperDC, Str, mBaseWidth / 2.f + mBackButtonOffset.x, 100 + mBackButtonOffset.y, 50);
		// 메인 메뉴 가기 버튼 살짝 내려오면서 출력
		mBackButton.InitObj({ mBaseWidth / 2.f + mBackButtonOffset.x, mBackButtonOffset.y }, { 50, 50 }, &mSprite);
		mBackButton.SetHDC(mPaperDC);
		mBackButton.DrawObject(SpriteType::BackButton, false, buf);
	}
	else if (State == GAMESTATE::STOP)
	{
		ShowCharacterInterFace();
		mPauseButton.SetHDC(mPaperDC);
		mPauseButton.DrawObject(SpriteType::OptionButton, false, buf);

		mStages[mCurStageNum]->SetHDC(mPaperDC);
		mStages[mCurStageNum]->DrawMap();

		mPlayer.SetHDC(mPaperDC);
		mPlayer.DrawCharacter();
		mPlayer.DrawBullets();

		// 아이템 출력
		for (auto& Item : mItemList) {
			Item->SetHDC(mPaperDC);
			Item->DrawItem();
		}

		// 적 출력
		for (auto& Bots : mEnemyList) {
			Bots->SetHDC(mPaperDC);
			Bots->DrawEnemy();
		}
		
		mSprite.ScenterString(mPaperDC, L"paused", mBaseWidth / 2.f, mBaseHeight / 2.f + 140, 50);
		// 재개 버튼
		mResumeButton.SetHDC(mPaperDC);
		mResumeButton.DrawObject(SpriteType::ResumeButton, false, buf);
		// 메뉴 돌아가기 버튼
		mHomeButton.SetHDC(mPaperDC);
		mHomeButton.DrawObject(SpriteType::BackButton, false, buf);
	}
}

void BaseGame::OnSceneUpdate()
{
	float Rad = getRad(90 * mPaperRect.top / mBaseHeight);
	float Speed = max(cosf(Rad) * 2000, 120);
	if (mSceneChangeStance) {
		if (mGameState == GAMESTATE::INIT) {

		}
		else if (mGameState == GAMESTATE::HOWTO) // 메인화면에서 HowTo 메뉴로 이동시 전환
		{
			if (mPaperRect.top == mBaseHeight) // 스테이지 이동 애니메이션 종료
			{
				mNextSceneOffset.y -= mBaseHeight;
				mBitMapSize.y -= mBaseHeight;
				mPaperRect.top = 0;
				mSceneChangeStance = false;
				OnInit();
			}
			else {
				mPaperRect.top = (LONG)min(mBaseHeight, mPaperRect.top + mTimer.DeltaTime() * Speed);
			}
		}
		/*
		else if (mGameState == GAMESTATE::OPTION) // 메인화면에서 옵션 메뉴로 이동시 전환
		{
			if (mPaperRect.top == mBaseHeight) // 스테이지 이동 애니메이션 종료
			{
				mNextSceneOffset.y -= mBaseHeight;
				mBitMapSize.y -= mBaseHeight;
				mPaperRect.top = 0;
				mSceneChangeStance = false;
				OnInit();
			}
			else {
				mPaperRect.top = (LONG)min(mBaseHeight, mPaperRect.top + mTimer.DeltaTime() * Speed);
			}
		}
		*/
		else if (mGameState == GAMESTATE::MAINMENU) {
			if (mPaperRect.top == mBaseHeight) // 스테이지 이동 애니메이션 종료
			{
				mNextSceneOffset.y -= mBaseHeight;
				mBitMapSize.y -= mBaseHeight;
				mPaperRect.top = 0;
				mSceneChangeStance = false;
				if (mPrevGameState == GAMESTATE::END || mPrevGameState == GAMESTATE::GAMEOVER || mPrevGameState == GAMESTATE::STOP)
					mGameState = GAMESTATE::INIT;
				OnInit();
			}
			else {
				mPaperRect.top = (LONG)min(mBaseHeight, mPaperRect.top + mTimer.DeltaTime() * Speed);
			}
		}
		else if (mGameState == GAMESTATE::RUN) {
			if (mPaperRect.top == mBaseHeight) // 스테이지 이동 애니메이션 종료
			{
				if (mPrevGameState != GAMESTATE::MAINMENU)
					mCurStageNum++;
				mStages[mCurStageNum]->mStPositon.y -= mBaseHeight;
				mBitMapSize.y -= mBaseHeight;
				mPaperRect.top = 0;
				mSceneChangeStance = false;
				// mPrevGameState = GAMESTATE::RUN;
				OnInit();
			}
			else {
				mPaperRect.top = (LONG)min(mBaseHeight, mPaperRect.top + mTimer.DeltaTime() * Speed);
			}
		}
		else if (mGameState == GAMESTATE::STOP) {

		}
		else if (mGameState == GAMESTATE::SUCCESS) {

		}
		else if (mGameState == GAMESTATE::END) {

		}
	}
}

void BaseGame::OnMouseDown(WPARAM btnState, int x, int y) // 마우스 버튼 누를때
{
	float mY = (mClientHeight - y + mPrintWnd.top) / mRatio;
	float mX = (x - mPrintWnd.left) / mRatio;
	POINT MapCord = mStages[mCurStageNum]->GetMapTileAtPoint({ mX,mY });

	if ((btnState & MK_LBUTTON) != 0 && !mSceneChangeStance) // 좌측 클릭
	{
		if (mGameState == GAMESTATE::RUN) {
			if (mPauseButton.mAABB.PointInside(mPauseButton.mAABB, { mX,mY })) {
				mGameState = GAMESTATE::STOP;
				OnInit();
			}
			else {
				if (mPlayer.mCurBullets[(int)mPlayer.mCurBulletType] > 0) {
					mPlayer.ReloadBullets({ mX, mY });
				}
			}
		}
		else if (mGameState == GAMESTATE::MAINMENU) {
			if (mPlayButton.mAABB.PointInside(mPlayButton.mAABB, { mX,mY })) {
				mGameSound.PlaySoundEffect(EffectSoundType::ButtonClick);
				mPrevGameState = GAMESTATE::MAINMENU;
				mGameState = GAMESTATE::RUN;
				mSceneChangeStance = true;
				mStages[mCurStageNum]->mStPositon.y += mBaseHeight;
				mBitMapSize.y += mBaseHeight;
				// OnInit();
			}
			else if (mHowToPlayButton.mAABB.PointInside(mHowToPlayButton.mAABB, { mX,mY })) {
				mGameSound.PlaySoundEffect(EffectSoundType::ButtonClick);
				mGameState = GAMESTATE::HOWTO;
				mNextSceneOffset.y += mBaseHeight;
				mBitMapSize.y += mBaseHeight;
				mSceneChangeStance = true;
				OnInit();
			}
			/*
			else if (mOptionButton.mAABB.PointInside(mOptionButton.mAABB, { mX,mY })) {
				mGameSound.PlaySoundEffect(EffectSoundType::ButtonClick);
				mGameState = GAMESTATE::OPTION;
				mNextSceneOffset.y += mBaseHeight;
				mBitMapSize.y += mBaseHeight;
				mSceneChangeStance = true;
				OnInit();
			}
			*/
			else if (mQuitButton.mAABB.PointInside(mQuitButton.mAABB, { mX,mY })) {
				mGameSound.PlaySoundEffect(EffectSoundType::ButtonClick);
				PostQuitMessage(0);
			}
		}
		else if (mGameState == GAMESTATE::HOWTO) {
			if (mBackButton.mAABB.PointInside(mBackButton.mAABB, { mX,mY })) {
				mGameSound.PlaySoundEffect(EffectSoundType::ButtonClick);
				mPrevGameState = GAMESTATE::HOWTO;
				mGameState = GAMESTATE::MAINMENU;
				mNextSceneOffset.y += mBaseHeight;
				mBitMapSize.y += mBaseHeight;
				mSceneChangeStance = true;
				OnInit();
			}
		}
		/*
		else if (mGameState == GAMESTATE::OPTION) {
			if (mBackButton.mAABB.PointInside(mBackButton.mAABB, { mX,mY })) {
				mGameSound.PlaySoundEffect(EffectSoundType::ButtonClick);
				mPrevGameState = GAMESTATE::OPTION;
				mGameState = GAMESTATE::MAINMENU;
				mNextSceneOffset.y += mBaseHeight;
				mBitMapSize.y += mBaseHeight;
				mSceneChangeStance = true;
				OnInit();
			}
		}
		*/
		else if (mGameState == GAMESTATE::END) {
			if (mBackButton.mAABB.PointInside(mBackButton.mAABB, { mX,mY })) {
				mGameSound.PlaySoundEffect(EffectSoundType::ButtonClick);
				mPrevGameState = GAMESTATE::END;
				mGameState = GAMESTATE::MAINMENU;
				mNextSceneOffset.y += mBaseHeight;
				mBitMapSize.y += mBaseHeight;
				mSceneChangeStance = true;
				OnInit();
			}
		}
		else if (mGameState == GAMESTATE::GAMEOVER) {
			if (mBackButton.mAABB.PointInside(mBackButton.mAABB, { mX,mY })) {
				mGameSound.PlaySoundEffect(EffectSoundType::ButtonClick);
				mPrevGameState = GAMESTATE::GAMEOVER;
				mGameState = GAMESTATE::MAINMENU;
				mNextSceneOffset.y += mBaseHeight;
				mBitMapSize.y += mBaseHeight;
				mSceneChangeStance = true;
				OnInit();
			}
		}
		else if (mGameState == GAMESTATE::STOP) {
			if (mResumeButton.mAABB.PointInside(mResumeButton.mAABB, { mX,mY })) {
				mGameSound.PlaySoundEffect(EffectSoundType::ButtonClick);
				mGameState = GAMESTATE::RUN;
			}
			else if (mHomeButton.mAABB.PointInside(mHomeButton.mAABB, { mX,mY })) {
				mGameSound.StopSoundBG(BGSoundType::Stage);
				mGameSound.PlaySoundEffect(EffectSoundType::ButtonClick);
				mPrevGameState = GAMESTATE::STOP;
				mGameState = GAMESTATE::MAINMENU;
				mNextSceneOffset.y += mBaseHeight;
				mBitMapSize.y += mBaseHeight;
				mSceneChangeStance = true;
				OnInit();
			}
		}
	}
	else if ((btnState & MK_RBUTTON) != 0) // 우측 클릭
	{
		if (mGameState == GAMESTATE::RUN && !mSceneChangeStance) {
			// 마우스 커서가 최대 범위를 넘으면 최대 멀리 놓을 수 있는 곳에 블록 배치
			float Dist = sqrtf(powf(mX - mPlayer.mPosition.x, 2) + powf(mY - mPlayer.mPosition.y, 2));
			if (Dist > mPlayer.mSetBlockRange) {
				float Cos, Sin, NewX, NewY, Quad[4][2] = { {1,1},{-1,1},{-1,-1},{1,-1} };
				int Index;
				Cos = abs(mX - mPlayer.mPosition.x) / Dist;
				Sin = abs(mY - mPlayer.mPosition.y) / Dist;
				if (mPlayer.mPosition.x < mX && mPlayer.mPosition.y < mY) // 0 ~ 90
					Index = 0;
				else if (mPlayer.mPosition.x >= mX && mPlayer.mPosition.y < mY) // 90 ~ 180
					Index = 1;
				else if (mPlayer.mPosition.x >= mX && mPlayer.mPosition.y >= mY) // 180 ~ 270
					Index = 2;
				else // 270 ~ 360
					Index = 3;
				NewX = mPlayer.mPosition.x + Cos * mPlayer.mSetBlockRange * Quad[Index][0];
				NewY = mPlayer.mPosition.y + Sin * mPlayer.mSetBlockRange * Quad[Index][1];
				MapCord = mStages[mCurStageNum]->GetMapTileAtPoint({ NewX,NewY });
			}
			if (1 <= MapCord.x && MapCord.x < mStages[mCurStageNum]->mWidth - 1
				&& 1 <= MapCord.y && MapCord.y < mStages[mCurStageNum]->mHeight - 1
				&& mPlayer.mSolidBlockCnt > 0 
				&& mStages[mCurStageNum]->GetTile(MapCord.x, MapCord.y) != TileType::Block) {
				mGameSound.PlaySoundEffect(EffectSoundType::SolidS);
				mPlayer.mSolidBlockCnt--;
				mStages[mCurStageNum]->SetTile(MapCord.x, MapCord.y, TileType::Block);
			}
		}
	}
	else if ((btnState & MK_MBUTTON) != 0)
	{
		if (mGameState == GAMESTATE::RUN && !mSceneChangeStance) {
			// 마우스 커서가 최대 범위를 넘으면 최대 멀리 놓을 수 있는 곳에 블록 배치
			float Dist = sqrtf(powf(mX - mPlayer.mPosition.x, 2) + powf(mY - mPlayer.mPosition.y, 2));
			if (Dist > mPlayer.mSetBlockRange) {
				float Cos, Sin, NewX, NewY, Quad[4][2] = { {1,1},{-1,1},{-1,-1},{1,-1} };
				int Index;
				Cos = abs(mX - mPlayer.mPosition.x) / Dist;
				Sin = abs(mY - mPlayer.mPosition.y) / Dist;
				if (mPlayer.mPosition.x < mX && mPlayer.mPosition.y < mY) // 0 ~ 90
					Index = 0;
				else if (mPlayer.mPosition.x >= mX && mPlayer.mPosition.y < mY) // 90 ~ 180
					Index = 1;
				else if (mPlayer.mPosition.x >= mX && mPlayer.mPosition.y >= mY) // 180 ~ 270
					Index = 2;
				else // 270 ~ 360
					Index = 3;
				NewX = mPlayer.mPosition.x + Cos * mPlayer.mSetBlockRange * Quad[Index][0];
				NewY = mPlayer.mPosition.y + Sin * mPlayer.mSetBlockRange * Quad[Index][1];
				MapCord = mStages[mCurStageNum]->GetMapTileAtPoint({ NewX,NewY });
			}
			if (1 <= MapCord.x && MapCord.x < mStages[mCurStageNum]->mWidth - 1
				&& 1 <= MapCord.y && MapCord.y < mStages[mCurStageNum]->mHeight - 1
				&& mPlayer.mOneWayBlockCnt > 0
				&& mStages[mCurStageNum]->GetTile(MapCord.x, MapCord.y) != TileType::OneWay) {
				mGameSound.PlaySoundEffect(EffectSoundType::OneWayS);
				mPlayer.mOneWayBlockCnt--;
				mStages[mCurStageNum]->SetTile(MapCord.x, MapCord.y, TileType::OneWay);
			}
		}
	}
	
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void BaseGame::OnMouseUp(WPARAM btnState, int x, int y) // 마우스 버튼 땔 때
{
	ReleaseCapture();
}

void BaseGame::OnMouseMove(WPARAM btnState, int x, int y)
{
	float mY = (mClientHeight - y + mPrintWnd.top) / mRatio;
	float mX = (x - mPrintWnd.left) / mRatio;
	
	if ((btnState & MK_LBUTTON) != 0) // 좌측 클릭
	{

	}
	else if ((btnState & MK_RBUTTON) != 0) // 우측 클릭
	{

	}
	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void BaseGame::OnMouseWheel(WPARAM btnState, int x, int y)
{
	if ((short)HIWORD(btnState) > 0) // 마우스 휠 위로
	{
		if ((int)mPlayer.mCurBulletType == 0) {
			mPlayer.mCurBulletType = BulletType((int)BulletType::BulletCount - 1);
		}
		else
			mPlayer.mCurBulletType = BulletType((int)mPlayer.mCurBulletType - 1);
	}
	else // 아래로
	{
		mPlayer.mCurBulletType = BulletType(((int)mPlayer.mCurBulletType + 1) % ((int)BulletType::BulletCount));
	}
}

void BaseGame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UINT Notice = HIWORD(wParam);
	UINT ControlID = LOWORD(wParam);
	if (ControlID == 1) {
		
	}
	else if (ControlID == 2) {

	}
	else if (ControlID == 3) {
		
	}
	else if (ControlID == 4) {

	}
}

// 블럭 놓을 위치 미리 보여주는 함수
void BaseGame::ShowBrickPos()
{
	float mY = (mClientHeight - mLastMousePos.y + mPrintWnd.top) / mRatio;
	float mX = (mLastMousePos.x - mPrintWnd.left) / mRatio;
	POINT MapCord = mStages[mCurStageNum]->GetMapTileAtPoint({ mX,mY }), PrintBrick;
	POINTFLOAT PrintPos;
	float Dist = sqrtf(powf(mX - mPlayer.mPosition.x, 2) + powf(mY - mPlayer.mPosition.y, 2));
	if (Dist > mPlayer.mSetBlockRange) {
		float Cos, Sin, NewX, NewY, Quad[4][2] = { {1,1},{-1,1},{-1,-1},{1,-1} };
		int Index;
		Cos = abs(mX - mPlayer.mPosition.x) / Dist;
		Sin = abs(mY - mPlayer.mPosition.y) / Dist;
		if (mPlayer.mPosition.x < mX && mPlayer.mPosition.y < mY) // 0 ~ 90
			Index = 0;
		else if (mPlayer.mPosition.x >= mX && mPlayer.mPosition.y < mY) // 90 ~ 180
			Index = 1;
		else if (mPlayer.mPosition.x >= mX && mPlayer.mPosition.y >= mY) // 180 ~ 270
			Index = 2;
		else // 270 ~ 360
			Index = 3;
		NewX = mPlayer.mPosition.x + Cos * mPlayer.mSetBlockRange * Quad[Index][0];
		NewY = mPlayer.mPosition.y + Sin * mPlayer.mSetBlockRange * Quad[Index][1];
		PrintBrick = mStages[mCurStageNum]->GetMapTileAtPoint({ NewX,NewY });
		if (PrintBrick.x < 1 || PrintBrick.x >= mStages[mCurStageNum]->mWidth - 1 || PrintBrick.y < 1 || PrintBrick.y >= mStages[mCurStageNum]->mHeight - 1)
			return;
		PrintPos = mStages[mCurStageNum]->GetMapTilePosition(PrintBrick);
	}
	else {
		if (MapCord.x < 1 || MapCord.x >= mStages[mCurStageNum]->mWidth - 1 || MapCord.y < 1 || MapCord.y >= mStages[mCurStageNum]->mHeight - 1)
			return;
		PrintPos = mStages[mCurStageNum]->GetMapTilePosition(MapCord);
	}
	mSprite.DrawIMG(mPaperDC, SpriteType::CrossHair, true,
		PrintPos.x - mStages[mCurStageNum]->mTileSize / 2.f, PrintPos.y - mStages[mCurStageNum]->mTileSize / 2.f,
		mStages[mCurStageNum]->mTileSize, mStages[mCurStageNum]->mTileSize);
}


void BaseGame::ShowCharacterInterFace()
{
	mSprite.DrawIMG(mPaperDC, SpriteType::HeartsBack, true, 30, mBaseHeight - 150, 120, 140);
	mSprite.DrawIMG(mPaperDC, SpriteType::HeartsBack, true, 30, mBaseHeight - 210, 120, 140);

	int Line = 3; // 한 줄에 몇 개의 엔티티를 그릴 것인지 척도
	int MaxHealth = mPlayer.mMaxHealth;
	int CurHealth = mPlayer.mCurHealth;
	int Row = ceil(MaxHealth / (float)Line);
	static int HeartFrame = 0;
	float X = 24, Y = mBaseHeight - 95;
	for (int i = 0; i < Row; i++)
	{
		float pY = Y - i * 58;
		for (int j = 0; j < Line && 0 < CurHealth; j++)
		{
			float pX = X + j * 40;
			mSprite.Animation(mPaperDC, true, SpriteType::Hearts, HeartFrame, pX, pY, 40, 43);
			HeartFrame--;
			CurHealth--;
		}
	}
	HeartFrame++;

	// 딱딱한 블록
	Line = 5;
	int BrickCnt = mPlayer.mSolidBlockCnt;
	int MaxBrick = mPlayer.mMaxSolidBlock;
	Row = ceil(MaxBrick / (float)Line);
	mSprite.DrawIMG(mPaperDC, SpriteType::HeartsBack, true, 5, 300, 200, 280);
	mSprite.SpriteString(mPaperDC, L"solid brick", 5, 480, 15);
	X = 25, Y = 450;
	for (int i = 0; i < Row; i++)
	{
		float pY = Y - i * 25;
		for (int j = 0; j < Line && 0 < BrickCnt; j++)
		{
			float pX = X + j * 25;
			mSprite.DrawIMG(mPaperDC, SpriteType::BrickB, true, pX, pY, 25, 25);
			BrickCnt--;
		}
	}

	// 내려가기 가능 블록
	BrickCnt = mPlayer.mOneWayBlockCnt;
	MaxBrick = mPlayer.mMaxOneWayBlock;
	Row = ceil(MaxBrick / (float)Line);
	mSprite.DrawIMG(mPaperDC, SpriteType::HeartsBack, true, 5, 140, 200, 280);
	mSprite.SpriteString(mPaperDC, L"pass brick", 15, 320, 15);
	X = 28, Y = 290;
	for (int i = 0; i < Row; i++)
	{
		float pY = Y - i * 25;
		for (int j = 0; j < Line && 0 < BrickCnt; j++)
		{
			float pX = X + j * 25;
			mSprite.DrawIMG(mPaperDC, SpriteType::OneWayB, true, pX, pY, 25, 25);
			BrickCnt--;
		}
	}

	// 콩알탄 출력
	X = mBaseWidth - 200, Y = 380;
	if (mPlayer.mCurBulletType == BulletType::SoyBean)
		mSprite.DrawIMG(mPaperDC, SpriteType::HeartsBack, true, X, Y, 200, 280);
	mSprite.SpriteString(mPaperDC, L"coinshot", X + 35, Y + 180, 18);
	mSprite.DrawIMG(mPaperDC, SpriteType::SoyBeanCnt, true, X + 75, Y + 120, 70, 60);

	// 총알 출력
	X = mBaseWidth - 200, Y = 226;
	if (mPlayer.mCurBulletType == BulletType::Bullet)
		mSprite.DrawIMG(mPaperDC, SpriteType::HeartsBack, true, X, Y, 200, 250);
	mSprite.SpriteString(mPaperDC, L"bullets", X + 45, Y + 154, 18);
	Line = 10;
	int CurBullet = mPlayer.mCurBullets[(int)BulletType::Bullet];
	int MaxBullet = mPlayer.mMaxBullets[(int)BulletType::Bullet];
	Row = ceil(MaxBullet / (float)Line);
	X = X + 32; Y = Y + 134;
	for (int i = 0; i < Row; i++)
	{
		float pY = Y - i * 15;
		for (int j = 0; j < Line && 0 < CurBullet; j++)
		{
			float pX = X + j * 15;
			mSprite.DrawIMG(mPaperDC, SpriteType::BulletCnt, true, pX, pY, 20, 20);
			CurBullet--;
		}
	}

	// 폭탄 출력
	X = mBaseWidth - 200, Y = 0;
	if (mPlayer.mCurBulletType == BulletType::Rock)
		mSprite.DrawIMG(mPaperDC, SpriteType::HeartsBack, true, X, Y, 200, 280);
	mSprite.SpriteString(mPaperDC, L"bombs", mBaseWidth - 135, 180, 18);
	Line = 5;
	CurBullet = mPlayer.mCurBullets[(int)BulletType::Rock];
	MaxBullet = mPlayer.mMaxBullets[(int)BulletType::Rock];
	Row = ceil(MaxBullet / (float)Line);
	X = mBaseWidth - 200, Y = 115;
	for (int i = 0; i < Row; i++)
	{
		float pY = Y - i * 30;
		for (int j = 0; j < Line && 0 < CurBullet; j++)
		{
			float pX = X + j * 30;
			mSprite.DrawIMG(mPaperDC, SpriteType::BombCnt, true, pX, pY, 100, 100);
			CurBullet--;
		}
	}

	// 스코어보드 그리기
}