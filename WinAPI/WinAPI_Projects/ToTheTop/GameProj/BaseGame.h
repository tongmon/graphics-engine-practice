#pragma once

#include "BaseApp.h"
#include "GameMap.h"
#include "GameItem.h"
#include "GameEffects.h"
#include "FmodSound.h"

class BaseGame : public BaseApp
{
private:
	std::list<GameBot*> mEnemyList; // 적이 담기는 리스트
	std::list<GameItem*> mItemList; // 아이템이 담기는 리스트
	std::list<GameEffects*> mEffectList; // 이펙트 효과 리스트
	std::vector<GameMap*> mStages; // 맵들이 담기는 저장 벡터
	POINT mLastMousePos; // 예전 마우스 좌표
	Character mPlayer; // 플레이어
	bool mSceneChangeStance; // 화면 전환 상태
	int mCurStageNum; // 현재 스테이지 번호

	// 화면 이동에 필요한 오프셋 위치들
	POINTFLOAT mNextSceneOffset;
	POINTFLOAT mBackButtonOffset;
	POINTFLOAT mMainStringOffset;
	POINTFLOAT mHowToStringOffset;

	GameSprite mSprite; // 각종 이미지들을 그리는 관리 객체

	int mScore; // 게임 점수

	CFmodSound mGameSound; // 게임 사운드 관리 객체

	// 적 드롭하는 시간
	int mCurEnemyDropTime;
	int mMaxEnemyDropTime[100];

	int mEnemyRange; // 적들 범위
private:
	virtual void OnInit();
	virtual void OnRender();
	virtual void OnUpdate();
	virtual void OnResize();
	virtual void OnRelease();

public:
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	void OnMouseWheel(WPARAM btnState, int x, int y);
	void OnCommand(WPARAM wParam, LPARAM lParam);
	void OnSceneUpdate(); // 장면 전환
	void Scene(GAMESTATE State);
	void ShowBrickPos();
	void ShowCharacterInterFace();
	BaseGame(void);
	~BaseGame(void);
};
