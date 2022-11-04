#include "Character.h"
#include "GameMap.h"
#include "GameBot.h"

Character::Character()
{
    mInputs.resize(KeyInput::Count);
    mPrevInputs.resize(KeyInput::Count);

    for (int i = 0; i < mInputs.size(); i++)
        mInputs[i] = false;
    
    mPosition = { 0,0 };

    mAABB.halfSize = { 0, 0 };
    mAABBOffset.y = mAABB.halfSize.y;

    mWalkSpeed = 160.0f;
    mJumpSpeed = 300.0f;

    mCurJumpCnt = 0;
    mMaxJumpCnt = 1;

    mLedgeGrabOffset = 1.0f;

    for (int i = 0; i < (int)BulletType::BulletCount; i++)
    {
        mCurReloadTime[i] = 0;
    }
    mReloadTime[(int)BulletType::SoyBean] = 30;
    mReloadTime[(int)BulletType::Rock] = 25;
    mReloadTime[(int)BulletType::Bullet] = 10;

    mCurInvincTime = 0; // 초반 무적시간 없음
    mInvincTime = 60; // 무적 프레임 수 초기화

    mKnockBackSpeed = { 0,0 }; // 넉백 속도 초기화

    mKnockBacked = { 0, 0 };

    mHeroFlag = true;

    mSetBlockRange = 125.f;

    mEnemyList = NULL;

    mCurHealth = 6; // 플레이어 초기 체력
    mMaxHealth = 6; // 플레이어 최대 체력
    mMaxBullets[(int)BulletType::SoyBean] = 2100000000;
    mMaxBullets[(int)BulletType::Rock] = 10;
    mMaxBullets[(int)BulletType::Bullet] = 30;
    // 최대 소지 블록 제한
    mMaxOneWayBlock = 10;
    mMaxSolidBlock = 10;

    mCurBulletType = BulletType::SoyBean;
}

Character::~Character()
{

}

// 캐릭터 초기화 함수
void Character::InitCharacter(POINTFLOAT Point, POINTFLOAT hSize, GameMap* Stage, GameSprite* Sprite, float WalkSpeed, float JumpSpeed)
{
    mPosition = Point;
    mSprite = Sprite;
    mAABB.halfSize = { cPlayerWidth,cPlayerHeight };
    mMap = Stage;
    mWalkSpeed = 190.f;
    mJumpSpeed = 430.f;

    mKnockBackSpeed = { 0,0 }; // 넉백 속도 초기화
    mCurInvincTime = 0; // 초반 무적시간 없음

    mSpeed = { 0,0 };
    mBullets.clear();
    mFramesFromJumpStart = 0;

    mCurHealth = 6; // 플레이어 초기 체력
    mSolidBlockCnt = 0;
    mOneWayBlockCnt = 0;
    mCurBullets[(int)BulletType::Bullet] = 11;
    mCurBullets[(int)BulletType::Rock] = 3;
    mCurBullets[(int)BulletType::SoyBean] = 2100000000;
}

void Character::UpdatePrevInputs()
{
    for (int i = 0; i < (int)KeyInput::Count; ++i)
        mPrevInputs[i] = mInputs[i];
}

// 점프 처리 함수
void Character::HandleJumping()
{
    // 점프 상태면 점프 프레임 증가
    // 점프 횟수를 모두 쓴 마지막 점프에서만 프레임 증가
    if (mCurJumpCnt >= mMaxJumpCnt)
        ++mFramesFromJumpStart;

    // 천장에 닿으면 점프 프레임 최대치로 변경
    if (mAtCeiling && mCurJumpCnt >= mMaxJumpCnt) {
        mFramesFromJumpStart = 100;
    }

    mSpeed.y += cGravity * mDeltaTime; // 점프 속도에 중력 추가

    mSpeed.y = max(mSpeed.y, cMaxFallingSpeed); // 너무 빨리 떨어지는 현상 방지

    // 점프 도중에 점프키 떼면 특정 속도로 고정
    // 즉 점프키를 길게 눌러야 오래 점프 가능
    if (!mInputs[(int)KeyInput::Jump] && mSpeed.y > 0.0f)
    {
        mSpeed.y = min(mSpeed.y, cMinJumpSpeed);
        if (mCurJumpCnt >= mMaxJumpCnt)
            mFramesFromJumpStart = 100;
    }

    // 좌우키가 모두 눌리면 좌우 속도는 0
    if (mInputs[(int)KeyInput::GoRight] == mInputs[(int)KeyInput::GoLeft])
    {
        mSpeed.x = 0.0f + mKnockBackSpeed.x;
    }
    else if (mInputs[(int)KeyInput::GoRight]) // 우측 키가 눌렸으면 우측으로 이동
    {
        // 좌우 방향 바꾸어주는 작업 필요 (이미지 씌울 경우)
        misLeft = false;

        mSpeed.x = mWalkSpeed + mKnockBackSpeed.x;
        //..W
        //.H.     <- to not get stuck in these kind of situations we beed to advance
        //..W			the hero forward if he doesn't push a wall anymore
        if (mPushedRightWall && !mPushesRightWall)
            mPosition.x += 1.0f;
    }
    else if (mInputs[(int)KeyInput::GoLeft]) // 좌측 키가 눌렸으면 좌측으로 이동
    {
        // 좌우 방향 바꾸어주는 작업 필요 (이미지 씌울 경우)
        misLeft = true;

        mSpeed.x = -mWalkSpeed + mKnockBackSpeed.x;
        //W..
        //.H.     <- to not get stuck in these kind of situations we need to advance
        //W..			the hero forward if he doesn't push a wall anymore
        if (mPushedLeftWall && !mPushesLeftWall)
            mPosition.x -= 1.0f;
    }

    // 살짝 떨어지는 중인데 점프 키를 눌렀다면 점프를 함, 속임수
    if (mInputs[(int)KeyInput::Jump]
        && (mOnGround || (mSpeed.y < 0.0f && mFramesFromJumpStart < cJumpFramesThreshold))
        && mCurJumpCnt < mMaxJumpCnt) {
        if (mHeroFlag)
            mGameSound->PlaySoundEffect(EffectSoundType::JumpStep);
        mCurJumpCnt++;
        mSpeed.y = mJumpSpeed;
    }
}

void Character::DrawCharacter()
{
    // AABB 중심 획득
    POINTFLOAT aabbPos = mAABB.center;
    POINTFLOAT Size = { 64 , 64 };

    static bool FadeInOut = false;
    if (mCurInvincTime)
        FadeInOut = FadeInOut == true ? false : true;
    else
        FadeInOut = false;

    if (FadeInOut == true)
        return;

    if (mSpeed.x == 0 && mSpeed.y == 0 && mCurrentState == CharacterState::Stand) {
        mSprite->Animation(mHdc, misLeft ^ 1, SpriteType::CharacterIdle, mIdleFrame,
            int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
    }
    else if (!mOnGround && mPushesRightWall && mSpeed.y <= 0 && !mAtCeiling) {
        mSprite->Animation(mHdc, misLeft ^ 1, SpriteType::CharacterGrab, mWallFrame,
            int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
    }
    else if (!mOnGround && mPushesLeftWall && mSpeed.y <= 0 && !mAtCeiling) {
        mSprite->Animation(mHdc, misLeft ^ 1, SpriteType::CharacterGrab, mWallFrame,
            int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
    }
    else if (mSpeed.y > 0 && mCurrentState == CharacterState::Jump) {
        mSprite->DrawIMG(mHdc, SpriteType::CharacterJumping, misLeft ^ 1,
            int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
    }
    else if (mSpeed.y <= 0 && mCurrentState == CharacterState::Jump) {
        mSprite->DrawIMG(mHdc, SpriteType::CharacterFall, misLeft ^ 1,
            int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
    }
    else if (mCurrentState == CharacterState::Run) {
        mSprite->Animation(mHdc, misLeft ^ 1, SpriteType::CharacterWalking, mWalkFrame,
            int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
    }
}

void Character::ReSetInputs()
{
    for (int i = 0; i < KeyInput::Count; i++)
        mInputs[i] = false;
}

void Character::SetInputs(KeyInput CurKey)
{
    mInputs[CurKey] = true;
}

void Character::CharacterUpdate()
{    
    mCurInvincTime = max(0, mCurInvincTime - 1); // 무적시간 감소
    mKnockBackSpeed.y = max(0, mKnockBackSpeed.y + cGravity * mDeltaTime);

    if (mKnockBackSpeed.x < 0) {
        mKnockBackSpeed.x = min(0, mKnockBackSpeed.x + 500.0f * mDeltaTime);
    }
    else {
        mKnockBackSpeed.x = max(0, mKnockBackSpeed.x - 500.0f * mDeltaTime);
    }

    /*
    if (mHeroFlag)
    {
        mWalkSoundFrame++;
        if (mCurrentState != CharacterState::Run)
            mWalkSoundFrame = 1;

        if (mCurrentState == CharacterState::Run && mOnGround && mWalkSoundFrame % 20 == 0)
            mGameSound->PlaySoundEffect(EffectSoundType::FootStep);
    }
    */

    switch (mCurrentState)
    {
    case CharacterState::Stand:

        mSpeed = mKnockBackSpeed; // 처음에 속도 0으로 설정

        if (!mOnGround) // 캐릭터가 지면에 있어야 서있을 수 있다.
        {
            mCurrentState = CharacterState::Jump; // 서있을 수 없으니 점프 상태로 간다.
            break;
        }

        // GoRight, GoLeft 둘 중에 한 키만 눌렀을 경우 걷기 상태가 된다.
        if (mInputs[(int)KeyInput::GoRight] != mInputs[(int)KeyInput::GoLeft])
        {
            mCurrentState = CharacterState::Run;
        }
        else if (mInputs[(int)KeyInput::Jump] && mCurJumpCnt < mMaxJumpCnt) // 점프 키를 눌렀다면 점프 상태가 된다.
        {
            if (mHeroFlag)
                mGameSound->PlaySoundEffect(EffectSoundType::JumpStep);
            mSpeed.y = mJumpSpeed; // 점프 속도 설정
            mCurJumpCnt++;
            mCurrentState = CharacterState::Jump;
        }

        if (mInputs[(int)KeyInput::GoDown] && mOnOneWayPlatform)
            mPosition.y -= cOneWayPlatformThreshold;

        break;
    case CharacterState::Run:

        // GoRight, GoLeft 키가 둘다 눌린 경우는 정지 상태가 된다.
        if (mInputs[(int)KeyInput::GoRight] == mInputs[(int)KeyInput::GoLeft])
        {
            mCurrentState = CharacterState::Stand;
            mSpeed = mKnockBackSpeed;
        }
        else if (mInputs[(int)KeyInput::GoRight]) // 우측 이동
        {
            // 좌우 방향 바꾸어주는 작업 필요 (이미지 씌울 경우)
            misLeft = false;

            mSpeed.x = mWalkSpeed + mKnockBackSpeed.x; // 좌우 움직임은 걷기 속도로 설정
        }
        else if (mInputs[(int)KeyInput::GoLeft])
        {
            // 좌우 방향 바꾸어주는 작업 필요 (이미지 씌울 경우)
            misLeft = true;

            mSpeed.x = -mWalkSpeed + mKnockBackSpeed.x; // 좌측 이동은 음의 속도
        }

        if (mInputs[(int)KeyInput::Jump] && mCurJumpCnt < mMaxJumpCnt) // 점프 키가 눌림
        {
            if (mHeroFlag)
                mGameSound->PlaySoundEffect(EffectSoundType::JumpStep);
            mSpeed.y = mJumpSpeed; // 속도는 점프 속도
            mCurJumpCnt++;
            mCurrentState = CharacterState::Jump;
        }
        else if (!mOnGround) // 점프 키가 안눌리면 그냥 점프 상태
        {
            mCurrentState = CharacterState::Jump;
            break;
        }

        // 좌측 벽에 닿아 못움직이면 속도를 0으로
        if (mPushesLeftWall)
            mSpeed.x = max(mSpeed.x, 0.0f);
        // 우측 벽도 마찬가지로 속도를 0으로
        else if (mPushesRightWall)
            mSpeed.x = min(mSpeed.x, 0.0f);

        if (mInputs[(int)KeyInput::GoDown] && mOnOneWayPlatform) // 밑으로 내려가기 가능 바닥 위이고 아래 키가 눌리면 밑으로 이동
            mPosition.y -= cOneWayPlatformThreshold;

        break;
    case CharacterState::Jump:

        HandleJumping();

        // 수평으로 가지 못하는 프레임이 있으면 수평 움직임 봉인
        if (mCannotGoLeftFrames > 0)
        {
            --mCannotGoLeftFrames;
            mInputs[(int)KeyInput::GoLeft] = false;
        }
        if (mCannotGoRightFrames > 0)
        {
            --mCannotGoRightFrames;
            mInputs[(int)KeyInput::GoRight] = false;
        }

        // 벽잡기 판단
        // 캐릭터가 떨어지고 있고, 천장에 있지 않으며, 벽과 충돌했는데 그 쪽으로 이동하려함
        if (mSpeed.y <= 0.0f && (mFramesFromJumpStart > 5 || mCurJumpCnt < mMaxJumpCnt) && !mAtCeiling
            && ((mPushesRightWall && mInputs[(int)KeyInput::GoRight]) || (mPushesLeftWall && mInputs[(int)KeyInput::GoLeft])))
        {
            POINTFLOAT halfSize;

            // 좌우측에 따라 연산 다르게
            if (mPushesRightWall && mInputs[(int)KeyInput::GoRight]) {
                halfSize = mAABB.halfSize;
            }
            else {
                halfSize = { -mAABB.halfSize.x - 1.0f, mAABB.halfSize.y };
            }

            halfSize.y += mLedgeGrabOffset;

            int tileIndexX, tileIndexY;
            mMap->GetMapTileAtPoint({ mAABB.center.x + halfSize.x, mAABB.center.y + halfSize.y }, tileIndexX, tileIndexY);

            int oldTileX, oldTileY;
            mMap->GetMapTileAtPoint({ mOldPosition.x + mAABBOffset.x + halfSize.x, mOldPosition.y + mAABBOffset.y + halfSize.y }, oldTileX, oldTileY);

            // 이미 밀린 벽이라면 예전 좌표의 잡기 타일 위치, 아니라면 현재 잡기 타일 위치
            int startTile = (mPushedLeftWall && mPushesLeftWall) || (mPushedRightWall && mPushesRightWall) ? oldTileY : tileIndexY;

            // 위에서 아래로 잡기 타일 탐색
            for (int y = startTile; y >= tileIndexY; --y)
            {
                // 벽잡기 후에 충돌 여부
                bool collidesAfterSnapping = false;
                int widthInTiles = mWidth;
                for (int x = tileIndexX - (int)sign(halfSize.x); abs(tileIndexX - x) <= widthInTiles; x -= (int)sign(halfSize.x))
                {
                    if (mMap->IsObstacle(x, y - 1 - (int)(halfSize.y * 2.0f) / mMap->mTileSize))
                    {
                        collidesAfterSnapping = true;
                        break;
                    }
                }

                // 잡을 수 있는 타일인지 검사
                // 잡기 센서 위의 타일은 비어 있어야 하고
                // 그 아래 타일은 잡을 수 있는 블록이여야 한다.
                if (!collidesAfterSnapping
                    && !mMap->IsObstacle(tileIndexX, y)
                    && mMap->IsObstacle(tileIndexX, y - 1)
                    && !mMap->IsObstacle(tileIndexX - (int)sign(halfSize.x), y))
                {
                    // 타일 모서리 위치 계산
                    POINTFLOAT tileCorner = mMap->GetMapTilePosition(tileIndexX, y - 1);
                    tileCorner.x -= sign(halfSize.x) * mMap->mTileSize * 0.5f;
                    tileCorner.y += mMap->mTileSize * 0.5f;

                    if (y != tileIndexY ||
                        (tileCorner.y - (mAABB.center.y + halfSize.y) >= -cGrabLedgeEndY
                            && tileCorner.y - (mAABB.center.y + halfSize.y) <= -cGrabLedgeStartY))
                    {
                        // 잡고 있는 타일을 저장
                        mLedgeTile = { tileIndexX, y - 1 };

                        // 충돌 박스와 타일 모서리가 붙어있도록 위치 계산
                        mPosition.y = tileCorner.y - halfSize.y - mAABBOffset.y;
                        mSpeed = mKnockBackSpeed;

                        // 모서리 잡은 상태로 변경
                        mCurrentState = CharacterState::GrabLedge;
                        mLedgeGrabStopFrames = cLedgeGrabStopFrames;
                        break;
                    }
                }
            }
        }

        // 지면에 충돌하면
        if (mOnGround)
        {
            mCurJumpCnt = 0; // 점프 횟수 복구
            
            // 좌우 키가 동시에 눌리면 움직임 없음
            if (mInputs[(int)KeyInput::GoRight] == mInputs[(int)KeyInput::GoLeft])
            {
                mCurrentState = CharacterState::Stand;
                mSpeed = { mKnockBackSpeed.x,0 };
            }
            else // 둘 중 하나만 눌렸다면 걷기 상태로 변환
            {
                mCurrentState = CharacterState::Run;
                mSpeed.y = 0.0f;
            }
        }
        break;

    case CharacterState::GrabLedge:

        bool ledgeOnLeft = mMap->mStPositon.x + mLedgeTile.x * mMap->mTileSize < mPosition.x;
        bool ledgeOnRight = !ledgeOnLeft;

        mCurJumpCnt = mMaxJumpCnt - 1; // 벽잡기 후에는 점프 단 한번만 가능

        // 밑 버튼이 눌리면 밑으로 떨어진다.
        if (mInputs[(int)KeyInput::GoDown]
            || (mInputs[(int)KeyInput::GoLeft] && ledgeOnRight)
            || (mInputs[(int)KeyInput::GoRight] && ledgeOnLeft))
        {
            if (ledgeOnLeft)
                mCannotGoLeftFrames = 3;
            else
                mCannotGoRightFrames = 3;

            mCurrentState = CharacterState::Jump;
            // 게임 사운드 삽입
        }
        else if (mInputs[(int)KeyInput::Jump] && mCurJumpCnt < mMaxJumpCnt)
        {
            if (mHeroFlag)
                mGameSound->PlaySoundEffect(EffectSoundType::JumpStep);
            // 점프 뛰면 점프 상태로 변환
            mSpeed.y = mJumpSpeed;
            mCurJumpCnt++;
            mCurrentState = CharacterState::Jump;
        }
        else
        {
            mSpeed = mKnockBackSpeed;
            mCurrentState = CharacterState::Jump;
        }

        // 잡은 타일이 없어지면 떨어진다.
        if (!mMap->IsObstacle(mLedgeTile.x, mLedgeTile.y))
            mCurrentState = CharacterState::Jump;

        break;
    }

    UpdatePhysics();

    // 예전에 지면에 있었는지 현재 지면에 없으면 점프를 시작한 것이니 프레임 재설정
    if (mWasOnGround && !mOnGround)
        mFramesFromJumpStart = 0;

    // 적이랑 부딫혔는지 확인
    if (mHeroFlag && mEnemyList != NULL) {
        for (auto& Bots : *mEnemyList) {
            if (Bots->mAABB.Overlaps(mAABB)) {
                Hitted(*Bots, true);
                break;
            }
        }
    }

    // 총알 갱신
    for (int i = 0; i < mBullets.size(); i++) {
        POINTFLOAT LeftTop = { mBullets[i].mAABB.center.x - mBullets[i].mAABB.halfSize.x, mBullets[i].mAABB.center.y + mBullets[i].mAABB.halfSize.y };
        POINTFLOAT RightBottom = { mBullets[i].mAABB.center.x + mBullets[i].mAABB.halfSize.x, mBullets[i].mAABB.center.y - mBullets[i].mAABB.halfSize.y };
        POINT LTBrick = mMap->GetMapTileAtPoint(LeftTop);
        POINT RBBrick = mMap->GetMapTileAtPoint(RightBottom);

        // 적이 총알 맞았는지 확인
        bool HitbyBullet = false;
        if (mEnemyList != NULL) {
            for (auto iter = mEnemyList->begin(); iter != mEnemyList->end(); iter++)
            {
                if (mBullets[i].mAABB.Overlaps((*iter)->mAABB))
                {
                    if (mBullets[i].mType != BulletType::Rock)
                        (*iter)->Hitted(mBullets[i], false);
                    HitbyBullet = true;
                    break;
                }
            }
        }

        // 총알이 벽에 맞았는지 확인
        if (HitbyBullet || mMap->AnySolidBlockInRectangle(LTBrick, RBBrick)) {
            // 총알 이펙트
            GameEffects* NewEff = new GameEffects;
            if (mBullets[i].mType == BulletType::SoyBean)
            {
                NewEff->InitEffects(mBullets[i].mAABB.center, EffectType::BulletEff, mSprite);
                mGameSound->PlaySoundEffect(EffectSoundType::SoyBeanHit);
            }
            else if (mBullets[i].mType == BulletType::Bullet)
            {
                NewEff->InitEffects(mBullets[i].mAABB.center, EffectType::BulletEff, mSprite);
                mGameSound->PlaySoundEffect(EffectSoundType::BulletHit);
            }
            else if (mBullets[i].mType == BulletType::Rock)
            {
                NewEff->InitEffects(mBullets[i].mAABB.center, EffectType::BombEff, mSprite);
                mGameSound->PlaySoundEffect(EffectSoundType::BombHit); // 소리

                // 폭탄은 범위 공격 처리
                AABB bufAABB = mBullets[i].mAABB;
                bufAABB.halfSize = { 60,60 };
                if (mEnemyList != NULL) {
                    for (auto iter = mEnemyList->begin(); iter != mEnemyList->end(); iter++)
                    {
                        if (bufAABB.Overlaps((*iter)->mAABB))
                            (*iter)->Hitted(mBullets[i], false);
                    }
                }
            }
            mEffList->push_back(NewEff);

            mBullets.erase(mBullets.begin() + i);
            i--;
        }
        else
            mBullets[i].UpdateBullet(mDeltaTime);
    }
    
    mCurReloadTime[(int)mCurBulletType]++; // 재장전 시간

    UpdatePrevInputs();
}

// 총알 발사
void Character::ReloadBullets(POINTFLOAT Point)
{
    if (mCurReloadTime[(int)mCurBulletType] >= mReloadTime[(int)mCurBulletType]) {
        mGameSound->PlaySoundEffect(EffectSoundType::Shoot);
        mCurReloadTime[(int)mCurBulletType] = 0;
        mCurBullets[(int)mCurBulletType]--;
        GameBullet Bulletbuffer;
        Bulletbuffer.SetBullet(mCurBulletType, mAABB.center, Point);
        mBullets.push_back(Bulletbuffer);
    }
}

void Character::DrawBullets()
{
    for (int i = 0; i < mBullets.size(); i++)
    {
        AABB BulletAABB = mBullets[i].mAABB;
        if (mBullets[i].mType == BulletType::SoyBean) {
            mSprite->Animation(mHdc, false, SpriteType::SoyBeanB, mBullets[i].mBulletFrame,
                BulletAABB.center.x - BulletAABB.halfSize.x, BulletAABB.center.y - BulletAABB.halfSize.y,
                BulletAABB.halfSize.x * 2, BulletAABB.halfSize.y * 2);
        }
        else if (mBullets[i].mType == BulletType::Rock) {
            mSprite->Animation(mHdc, false, SpriteType::BombB, mBullets[i].mBulletFrame,
                BulletAABB.center.x - 60, BulletAABB.center.y - 60,
                120, 120);
        }
        else if(mBullets[i].mType == BulletType::Bullet) {
            mSprite->DrawIMG(mHdc, SpriteType::BulletB, true,
                BulletAABB.center.x - BulletAABB.halfSize.x, BulletAABB.center.y - BulletAABB.halfSize.y,
                BulletAABB.halfSize.x * 2, BulletAABB.halfSize.y * 2);
        }
    }
}

void Character::Hitted(GameBullet &Hitter, bool InvincFlag)
{
    if (mCurInvincTime)
        return;
    mKnockBackSpeed.x += sign(mAABB.center.x - Hitter.mAABB.center.x) * (mKnockBacked.x + Hitter.mKnockBacks.x);
    mKnockBackSpeed.y += mKnockBacked.y + Hitter.mKnockBacks.y;
    mSpeed.y = mKnockBackSpeed.y;
    mCurHealth -= (int)Hitter.mDamage;
    if (InvincFlag)
        mCurInvincTime = mInvincTime;
}

void Character::Hitted(GameBot& Hitter, bool InvincFlag)
{
    if (mCurInvincTime)
        return;
    if (mHeroFlag)
        mGameSound->PlaySoundEffect(EffectSoundType::PlayerHitBy); // 소리

    mKnockBackSpeed.x += sign(mAABB.center.x - Hitter.mAABB.center.x) * (mKnockBacked.x + Hitter.mKnockBacks.x);
    mKnockBackSpeed.y += mKnockBacked.y + Hitter.mKnockBacks.y;
    mSpeed.y = mKnockBackSpeed.y;
    mCurHealth -= Hitter.mDamage;
    if (InvincFlag)
        mCurInvincTime = mInvincTime;
}