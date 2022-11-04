#pragma once

#include "BaseApp.h"
#include "GameMap.h"
#include "GameItem.h"
#include "GameEffects.h"
#include "FmodSound.h"

class BaseGame : public BaseApp
{
private:
	std::list<GameBot*> mEnemyList; // ���� ���� ����Ʈ
	std::list<GameItem*> mItemList; // �������� ���� ����Ʈ
	std::list<GameEffects*> mEffectList; // ����Ʈ ȿ�� ����Ʈ
	std::vector<GameMap*> mStages; // �ʵ��� ���� ���� ����
	POINT mLastMousePos; // ���� ���콺 ��ǥ
	Character mPlayer; // �÷��̾�
	bool mSceneChangeStance; // ȭ�� ��ȯ ����
	int mCurStageNum; // ���� �������� ��ȣ

	// ȭ�� �̵��� �ʿ��� ������ ��ġ��
	POINTFLOAT mNextSceneOffset;
	POINTFLOAT mBackButtonOffset;
	POINTFLOAT mMainStringOffset;
	POINTFLOAT mHowToStringOffset;

	GameSprite mSprite; // ���� �̹������� �׸��� ���� ��ü

	int mScore; // ���� ����

	CFmodSound mGameSound; // ���� ���� ���� ��ü

	// �� ����ϴ� �ð�
	int mCurEnemyDropTime;
	int mMaxEnemyDropTime[100];

	int mEnemyRange; // ���� ����
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
	void OnSceneUpdate(); // ��� ��ȯ
	void Scene(GAMESTATE State);
	void ShowBrickPos();
	void ShowCharacterInterFace();
	BaseGame(void);
	~BaseGame(void);
};
