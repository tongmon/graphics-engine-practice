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

    // 입력 변수
    std::vector<bool> mInputs;
    std::vector<bool> mPrevInputs;

    // 탄환 관련 정보들
    std::vector<GameBullet> mBullets;

    // 적 리스트 포인터
    std::list<GameBot*> *mEnemyList;

    int mFramesFromJumpStart = 0;

    // 점프 관련 변수
    int mCurJumpCnt; // 현재 점프 가능 개수
    int mMaxJumpCnt; // 최대 점프 가능 개수
    float mJumpSpeed; // 점프 속도

    float mWalkSpeed; // 걷기 속도

    // 벽잡기
    POINT mLedgeTile;
    float mLedgeGrabOffset; // 벽잡기 어디까지 가능한지 위치

    const int cLedgeGrabStopFrames = 5;
    int mLedgeGrabStopFrames = 0;
    // 벽잡기시 한쪽으로 연속적으로 계속가는 것 방지 프레임
    int mCannotGoLeftFrames = 0;
    int mCannotGoRightFrames = 0;

    // 아이템 관련 변수
    // 체력
    int mCurHealth;
    int mMaxHealth;
    // 블록 관련 변수
    float mSetBlockRange; // 블록 놓을 수 있는 범위
    int mSolidBlockCnt; // 단단한 블록 소지개수
    int mOneWayBlockCnt; // 일방 통해 블록 소지개수
    // 블록 소지 제한
    int mMaxSolidBlock;
    int mMaxOneWayBlock;
    // 탄환 제한과 소지 개수
    int mMaxBullets[(int)BulletType::BulletCount];
    int mCurBullets[(int)BulletType::BulletCount];

    // 현 플레이어가 사용할 탄창 속성
    BulletType mCurBulletType;

    int mReloadTime[(int)BulletType::BulletCount]; // 재장전 하면 대기 시간
    int mCurReloadTime[(int)BulletType::BulletCount]; // 현재 재장전 시간

    // 넉백 관련 변수
    // 현 캐릭터의 넉백 속도
    POINTFLOAT mKnockBackSpeed;
    // 해당 캐릭터, 적이 주는 넉백율
    POINTFLOAT mKnockBacks;
    // 받는 넉백율
    POINTFLOAT mKnockBacked;

    // 무적 프레임 시간
    float mInvincTime;
    float mCurInvincTime;

    // 맵에서 칸 얼마나 차지하는지
    int mWidth = 0;
    int mHeight = 0;

    // 해당 캐릭터가 주인공인지 아닌지 구분
    bool mHeroFlag = true;

    // 애니메이션 프레임들
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

