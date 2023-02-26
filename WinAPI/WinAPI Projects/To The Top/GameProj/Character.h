#pragma once

#include <Windows.h>
#include <vector>
#include "GameObj.h"
#include "GameBullet.h"
#include "Constants.h"

enum KeyInput
{
    GoLeft = 0,
    GoRight,
    GoDown,
    Jump,
    Count
};

class GameBot;

class Character : public GameObj
{
public:
    enum class CharacterState
    {
        Stand,
        Run,
        Jump,
        GrabLedge,
    };

    CharacterState mCurrentState = CharacterState::Stand;

    // �Է� ����
    std::vector<bool> mInputs;
    std::vector<bool> mPrevInputs;

    // źȯ ���� ������
    std::vector<GameBullet> mBullets;

    // �� ����Ʈ ������
    std::list<GameBot*> *mEnemyList;

    int mFramesFromJumpStart = 0;

    // ���� ���� ����
    int mCurJumpCnt; // ���� ���� ���� ����
    int mMaxJumpCnt; // �ִ� ���� ���� ����
    float mJumpSpeed; // ���� �ӵ�

    float mWalkSpeed; // �ȱ� �ӵ�

    // �����
    POINT mLedgeTile;
    float mLedgeGrabOffset; // ����� ������ �������� ��ġ

    const int cLedgeGrabStopFrames = 5;
    int mLedgeGrabStopFrames = 0;
    // ������ �������� ���������� ��Ӱ��� �� ���� ������
    int mCannotGoLeftFrames = 0;
    int mCannotGoRightFrames = 0;

    // ������ ���� ����
    // ü��
    int mCurHealth;
    int mMaxHealth;
    // ��� ���� ����
    float mSetBlockRange; // ��� ���� �� �ִ� ����
    int mSolidBlockCnt; // �ܴ��� ��� ��������
    int mOneWayBlockCnt; // �Ϲ� ���� ��� ��������
    // ��� ���� ����
    int mMaxSolidBlock;
    int mMaxOneWayBlock;
    // źȯ ���Ѱ� ���� ����
    int mMaxBullets[(int)BulletType::BulletCount];
    int mCurBullets[(int)BulletType::BulletCount];

    // �� �÷��̾ ����� źâ �Ӽ�
    BulletType mCurBulletType;

    int mReloadTime[(int)BulletType::BulletCount]; // ������ �ϸ� ��� �ð�
    int mCurReloadTime[(int)BulletType::BulletCount]; // ���� ������ �ð�

    // �˹� ���� ����
    // �� ĳ������ �˹� �ӵ�
    POINTFLOAT mKnockBackSpeed;
    // �ش� ĳ����, ���� �ִ� �˹���
    POINTFLOAT mKnockBacks;
    // �޴� �˹���
    POINTFLOAT mKnockBacked;

    // ���� ������ �ð�
    float mInvincTime;
    float mCurInvincTime;

    // �ʿ��� ĭ �󸶳� �����ϴ���
    int mWidth = 0;
    int mHeight = 0;

    // �ش� ĳ���Ͱ� ���ΰ����� �ƴ��� ����
    bool mHeroFlag = true;

    // �ִϸ��̼� �����ӵ�
    int mIdleFrame;
    int mWalkFrame;
    int mHitFrame;
    int mWallFrame;
    int mBulletFrame;
    int mWalkSoundFrame;

public:
    Character();
    ~Character();

    void InitCharacter(POINTFLOAT Point, POINTFLOAT hSize, GameMap* Stage, GameSprite* Sprite, float WalkSpeed, float JumpSpeed);
    void CharacterUpdate();
    void ReSetInputs();
    void SetInputs(KeyInput CurKey);
    void HandleJumping();
    void UpdatePrevInputs();
    void DrawCharacter();
    void DrawBullets();
    void ReloadBullets(POINTFLOAT Point);
    void Hitted(GameBullet& Hitter, bool InvincFlag);
    void Hitted(GameBot& Hitter, bool InvincFlag);
};

