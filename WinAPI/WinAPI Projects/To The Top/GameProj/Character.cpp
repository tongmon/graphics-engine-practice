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

    mCurInvincTime = 0; // �ʹ� �����ð� ����
    mInvincTime = 60; // ���� ������ �� �ʱ�ȭ

    mKnockBackSpeed = { 0,0 }; // �˹� �ӵ� �ʱ�ȭ

    mKnockBacked = { 0, 0 };

    mHeroFlag = true;

    mSetBlockRange = 125.f;

    mEnemyList = NULL;

    mCurHealth = 6; // �÷��̾� �ʱ� ü��
    mMaxHealth = 6; // �÷��̾� �ִ� ü��
    mMaxBullets[(int)BulletType::SoyBean] = 2100000000;
    mMaxBullets[(int)BulletType::Rock] = 10;
    mMaxBullets[(int)BulletType::Bullet] = 30;
    // �ִ� ���� ��� ����
    mMaxOneWayBlock = 10;
    mMaxSolidBlock = 10;

    mCurBulletType = BulletType::SoyBean;
}

Character::~Character()
{

}

// ĳ���� �ʱ�ȭ �Լ�
void Character::InitCharacter(POINTFLOAT Point, POINTFLOAT hSize, GameMap* Stage, GameSprite* Sprite, float WalkSpeed, float JumpSpeed)
{
    mPosition = Point;
    mSprite = Sprite;
    mAABB.halfSize = { cPlayerWidth,cPlayerHeight };
    mMap = Stage;
    mWalkSpeed = 190.f;
    mJumpSpeed = 430.f;

    mKnockBackSpeed = { 0,0 }; // �˹� �ӵ� �ʱ�ȭ
    mCurInvincTime = 0; // �ʹ� �����ð� ����

    mSpeed = { 0,0 };
    mBullets.clear();
    mFramesFromJumpStart = 0;

    mCurHealth = 6; // �÷��̾� �ʱ� ü��
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

// ���� ó�� �Լ�
void Character::HandleJumping()
{
    // ���� ���¸� ���� ������ ����
    // ���� Ƚ���� ��� �� ������ ���������� ������ ����
    if (mCurJumpCnt >= mMaxJumpCnt)
        ++mFramesFromJumpStart;

    // õ�忡 ������ ���� ������ �ִ�ġ�� ����
    if (mAtCeiling && mCurJumpCnt >= mMaxJumpCnt) {
        mFramesFromJumpStart = 100;
    }

    mSpeed.y += cGravity * mDeltaTime; // ���� �ӵ��� �߷� �߰�

    mSpeed.y = max(mSpeed.y, cMaxFallingSpeed); // �ʹ� ���� �������� ���� ����

    // ���� ���߿� ����Ű ���� Ư�� �ӵ��� ����
    // �� ����Ű�� ��� ������ ���� ���� ����
    if (!mInputs[(int)KeyInput::Jump] && mSpeed.y > 0.0f)
    {
        mSpeed.y = min(mSpeed.y, cMinJumpSpeed);
        if (mCurJumpCnt >= mMaxJumpCnt)
            mFramesFromJumpStart = 100;
    }

    // �¿�Ű�� ��� ������ �¿� �ӵ��� 0
    if (mInputs[(int)KeyInput::GoRight] == mInputs[(int)KeyInput::GoLeft])
    {
        mSpeed.x = 0.0f + mKnockBackSpeed.x;
    }
    else if (mInputs[(int)KeyInput::GoRight]) // ���� Ű�� �������� �������� �̵�
    {
        // �¿� ���� �ٲپ��ִ� �۾� �ʿ� (�̹��� ���� ���)
        misLeft = false;

        mSpeed.x = mWalkSpeed + mKnockBackSpeed.x;
        //..W
        //.H.     <- to not get stuck in these kind of situations we beed to advance
        //..W			the hero forward if he doesn't push a wall anymore
        if (mPushedRightWall && !mPushesRightWall)
            mPosition.x += 1.0f;
    }
    else if (mInputs[(int)KeyInput::GoLeft]) // ���� Ű�� �������� �������� �̵�
    {
        // �¿� ���� �ٲپ��ִ� �۾� �ʿ� (�̹��� ���� ���)
        misLeft = true;

        mSpeed.x = -mWalkSpeed + mKnockBackSpeed.x;
        //W..
        //.H.     <- to not get stuck in these kind of situations we need to advance
        //W..			the hero forward if he doesn't push a wall anymore
        if (mPushedLeftWall && !mPushesLeftWall)
            mPosition.x -= 1.0f;
    }

    // ��¦ �������� ���ε� ���� Ű�� �����ٸ� ������ ��, ���Ӽ�
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
    // AABB �߽� ȹ��
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
    mCurInvincTime = max(0, mCurInvincTime - 1); // �����ð� ����
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

        mSpeed = mKnockBackSpeed; // ó���� �ӵ� 0���� ����

        if (!mOnGround) // ĳ���Ͱ� ���鿡 �־�� ������ �� �ִ�.
        {
            mCurrentState = CharacterState::Jump; // ������ �� ������ ���� ���·� ����.
            break;
        }

        // GoRight, GoLeft �� �߿� �� Ű�� ������ ��� �ȱ� ���°� �ȴ�.
        if (mInputs[(int)KeyInput::GoRight] != mInputs[(int)KeyInput::GoLeft])
        {
            mCurrentState = CharacterState::Run;
        }
        else if (mInputs[(int)KeyInput::Jump] && mCurJumpCnt < mMaxJumpCnt) // ���� Ű�� �����ٸ� ���� ���°� �ȴ�.
        {
            if (mHeroFlag)
                mGameSound->PlaySoundEffect(EffectSoundType::JumpStep);
            mSpeed.y = mJumpSpeed; // ���� �ӵ� ����
            mCurJumpCnt++;
            mCurrentState = CharacterState::Jump;
        }

        if (mInputs[(int)KeyInput::GoDown] && mOnOneWayPlatform)
            mPosition.y -= cOneWayPlatformThreshold;

        break;
    case CharacterState::Run:

        // GoRight, GoLeft Ű�� �Ѵ� ���� ���� ���� ���°� �ȴ�.
        if (mInputs[(int)KeyInput::GoRight] == mInputs[(int)KeyInput::GoLeft])
        {
            mCurrentState = CharacterState::Stand;
            mSpeed = mKnockBackSpeed;
        }
        else if (mInputs[(int)KeyInput::GoRight]) // ���� �̵�
        {
            // �¿� ���� �ٲپ��ִ� �۾� �ʿ� (�̹��� ���� ���)
            misLeft = false;

            mSpeed.x = mWalkSpeed + mKnockBackSpeed.x; // �¿� �������� �ȱ� �ӵ��� ����
        }
        else if (mInputs[(int)KeyInput::GoLeft])
        {
            // �¿� ���� �ٲپ��ִ� �۾� �ʿ� (�̹��� ���� ���)
            misLeft = true;

            mSpeed.x = -mWalkSpeed + mKnockBackSpeed.x; // ���� �̵��� ���� �ӵ�
        }

        if (mInputs[(int)KeyInput::Jump] && mCurJumpCnt < mMaxJumpCnt) // ���� Ű�� ����
        {
            if (mHeroFlag)
                mGameSound->PlaySoundEffect(EffectSoundType::JumpStep);
            mSpeed.y = mJumpSpeed; // �ӵ��� ���� �ӵ�
            mCurJumpCnt++;
            mCurrentState = CharacterState::Jump;
        }
        else if (!mOnGround) // ���� Ű�� �ȴ����� �׳� ���� ����
        {
            mCurrentState = CharacterState::Jump;
            break;
        }

        // ���� ���� ��� �������̸� �ӵ��� 0����
        if (mPushesLeftWall)
            mSpeed.x = max(mSpeed.x, 0.0f);
        // ���� ���� ���������� �ӵ��� 0����
        else if (mPushesRightWall)
            mSpeed.x = min(mSpeed.x, 0.0f);

        if (mInputs[(int)KeyInput::GoDown] && mOnOneWayPlatform) // ������ �������� ���� �ٴ� ���̰� �Ʒ� Ű�� ������ ������ �̵�
            mPosition.y -= cOneWayPlatformThreshold;

        break;
    case CharacterState::Jump:

        HandleJumping();

        // �������� ���� ���ϴ� �������� ������ ���� ������ ����
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

        // ����� �Ǵ�
        // ĳ���Ͱ� �������� �ְ�, õ�忡 ���� ������, ���� �浹�ߴµ� �� ������ �̵��Ϸ���
        if (mSpeed.y <= 0.0f && (mFramesFromJumpStart > 5 || mCurJumpCnt < mMaxJumpCnt) && !mAtCeiling
            && ((mPushesRightWall && mInputs[(int)KeyInput::GoRight]) || (mPushesLeftWall && mInputs[(int)KeyInput::GoLeft])))
        {
            POINTFLOAT halfSize;

            // �¿����� ���� ���� �ٸ���
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

            // �̹� �и� ���̶�� ���� ��ǥ�� ��� Ÿ�� ��ġ, �ƴ϶�� ���� ��� Ÿ�� ��ġ
            int startTile = (mPushedLeftWall && mPushesLeftWall) || (mPushedRightWall && mPushesRightWall) ? oldTileY : tileIndexY;

            // ������ �Ʒ��� ��� Ÿ�� Ž��
            for (int y = startTile; y >= tileIndexY; --y)
            {
                // ����� �Ŀ� �浹 ����
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

                // ���� �� �ִ� Ÿ������ �˻�
                // ��� ���� ���� Ÿ���� ��� �־�� �ϰ�
                // �� �Ʒ� Ÿ���� ���� �� �ִ� ����̿��� �Ѵ�.
                if (!collidesAfterSnapping
                    && !mMap->IsObstacle(tileIndexX, y)
                    && mMap->IsObstacle(tileIndexX, y - 1)
                    && !mMap->IsObstacle(tileIndexX - (int)sign(halfSize.x), y))
                {
                    // Ÿ�� �𼭸� ��ġ ���
                    POINTFLOAT tileCorner = mMap->GetMapTilePosition(tileIndexX, y - 1);
                    tileCorner.x -= sign(halfSize.x) * mMap->mTileSize * 0.5f;
                    tileCorner.y += mMap->mTileSize * 0.5f;

                    if (y != tileIndexY ||
                        (tileCorner.y - (mAABB.center.y + halfSize.y) >= -cGrabLedgeEndY
                            && tileCorner.y - (mAABB.center.y + halfSize.y) <= -cGrabLedgeStartY))
                    {
                        // ��� �ִ� Ÿ���� ����
                        mLedgeTile = { tileIndexX, y - 1 };

                        // �浹 �ڽ��� Ÿ�� �𼭸��� �پ��ֵ��� ��ġ ���
                        mPosition.y = tileCorner.y - halfSize.y - mAABBOffset.y;
                        mSpeed = mKnockBackSpeed;

                        // �𼭸� ���� ���·� ����
                        mCurrentState = CharacterState::GrabLedge;
                        mLedgeGrabStopFrames = cLedgeGrabStopFrames;
                        break;
                    }
                }
            }
        }

        // ���鿡 �浹�ϸ�
        if (mOnGround)
        {
            mCurJumpCnt = 0; // ���� Ƚ�� ����
            
            // �¿� Ű�� ���ÿ� ������ ������ ����
            if (mInputs[(int)KeyInput::GoRight] == mInputs[(int)KeyInput::GoLeft])
            {
                mCurrentState = CharacterState::Stand;
                mSpeed = { mKnockBackSpeed.x,0 };
            }
            else // �� �� �ϳ��� ���ȴٸ� �ȱ� ���·� ��ȯ
            {
                mCurrentState = CharacterState::Run;
                mSpeed.y = 0.0f;
            }
        }
        break;

    case CharacterState::GrabLedge:

        bool ledgeOnLeft = mMap->mStPositon.x + mLedgeTile.x * mMap->mTileSize < mPosition.x;
        bool ledgeOnRight = !ledgeOnLeft;

        mCurJumpCnt = mMaxJumpCnt - 1; // ����� �Ŀ��� ���� �� �ѹ��� ����

        // �� ��ư�� ������ ������ ��������.
        if (mInputs[(int)KeyInput::GoDown]
            || (mInputs[(int)KeyInput::GoLeft] && ledgeOnRight)
            || (mInputs[(int)KeyInput::GoRight] && ledgeOnLeft))
        {
            if (ledgeOnLeft)
                mCannotGoLeftFrames = 3;
            else
                mCannotGoRightFrames = 3;

            mCurrentState = CharacterState::Jump;
            // ���� ���� ����
        }
        else if (mInputs[(int)KeyInput::Jump] && mCurJumpCnt < mMaxJumpCnt)
        {
            if (mHeroFlag)
                mGameSound->PlaySoundEffect(EffectSoundType::JumpStep);
            // ���� �ٸ� ���� ���·� ��ȯ
            mSpeed.y = mJumpSpeed;
            mCurJumpCnt++;
            mCurrentState = CharacterState::Jump;
        }
        else
        {
            mSpeed = mKnockBackSpeed;
            mCurrentState = CharacterState::Jump;
        }

        // ���� Ÿ���� �������� ��������.
        if (!mMap->IsObstacle(mLedgeTile.x, mLedgeTile.y))
            mCurrentState = CharacterState::Jump;

        break;
    }

    UpdatePhysics();

    // ������ ���鿡 �־����� ���� ���鿡 ������ ������ ������ ���̴� ������ �缳��
    if (mWasOnGround && !mOnGround)
        mFramesFromJumpStart = 0;

    // ���̶� �΋H������ Ȯ��
    if (mHeroFlag && mEnemyList != NULL) {
        for (auto& Bots : *mEnemyList) {
            if (Bots->mAABB.Overlaps(mAABB)) {
                Hitted(*Bots, true);
                break;
            }
        }
    }

    // �Ѿ� ����
    for (int i = 0; i < mBullets.size(); i++) {
        POINTFLOAT LeftTop = { mBullets[i].mAABB.center.x - mBullets[i].mAABB.halfSize.x, mBullets[i].mAABB.center.y + mBullets[i].mAABB.halfSize.y };
        POINTFLOAT RightBottom = { mBullets[i].mAABB.center.x + mBullets[i].mAABB.halfSize.x, mBullets[i].mAABB.center.y - mBullets[i].mAABB.halfSize.y };
        POINT LTBrick = mMap->GetMapTileAtPoint(LeftTop);
        POINT RBBrick = mMap->GetMapTileAtPoint(RightBottom);

        // ���� �Ѿ� �¾Ҵ��� Ȯ��
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

        // �Ѿ��� ���� �¾Ҵ��� Ȯ��
        if (HitbyBullet || mMap->AnySolidBlockInRectangle(LTBrick, RBBrick)) {
            // �Ѿ� ����Ʈ
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
                mGameSound->PlaySoundEffect(EffectSoundType::BombHit); // �Ҹ�

                // ��ź�� ���� ���� ó��
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
    
    mCurReloadTime[(int)mCurBulletType]++; // ������ �ð�

    UpdatePrevInputs();
}

// �Ѿ� �߻�
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
        mGameSound->PlaySoundEffect(EffectSoundType::PlayerHitBy); // �Ҹ�

    mKnockBackSpeed.x += sign(mAABB.center.x - Hitter.mAABB.center.x) * (mKnockBacked.x + Hitter.mKnockBacks.x);
    mKnockBackSpeed.y += mKnockBacked.y + Hitter.mKnockBacks.y;
    mSpeed.y = mKnockBackSpeed.y;
    mCurHealth -= Hitter.mDamage;
    if (InvincFlag)
        mCurInvincTime = mInvincTime;
}