#include "GameBot.h"
#include "GameMap.h"

GameBot::GameBot()
{
    mHero = NULL;
}

GameBot::~GameBot()
{

}

// 목표 지점이 공중이면 그 바닥면을 목표 지점으로 채택
void GameBot::TappedOnTile(POINT mapPos)
{
    if (mapPos.x < 1 || mapPos.x >= mMap->mWidth - 1 || mapPos.y < 1 || mapPos.y >= mMap->mHeight - 1)
        return;

    while (!(mMap->IsGround(mapPos.x, mapPos.y)))
        --mapPos.y;

    POINT pos = { mapPos.x, mapPos.y + 1 };
    MoveTo(pos);
}

void GameBot::InitBot(EnemyType EnemType, GameMap *Stage, GameSprite *Sprite, POINTFLOAT Point, Character *Hero)
{
    mSprite = Sprite;
    mHero = Hero;
    mPosition = Point;
    mMap = Stage;
    mLazyLedgeGrabs = false;
    mGrabLedgeFlag = false;
    mHeroFlag = false;
    mCurJumpCnt = 0;
    mMaxJumpCnt = 1;
    mEType = EnemType;

    if (EnemType == EnemyType::Grunt) {
        mAABB.halfSize = { 18.0f, 28.0f };
        mJumpSpeed = 510.0f;
        mWalkSpeed = 180.0f;
        mMaxJumpHeight = 3;
        mCurHealth = 2;
        mDamage = 1;
        mKnockBacked = { 270.f, 200.f }; // 넉백 당하는 수치
        mKnockBacks = { 260.f, 200.f }; // 적 몸통 박치기 할 때 주는 넉백 수치
    }
    else if (EnemType == EnemyType::Heavy) {
        mAABB.halfSize = { 9.0f, 21.0f };
        mJumpSpeed = 510.0f;
        mWalkSpeed = 200.0f;
        mMaxJumpHeight = 3;
        mCurHealth = 3;
        mDamage = 1;
        mKnockBacked = { 340.f, 200.f }; // 넉백 당하는 수치
        mKnockBacks = { 260.f, 200.f }; // 적 몸통 박치기 할 때 주는 넉백 수치
    }
    else if (EnemType == EnemyType::Massive) {
        mAABB.halfSize = { 13.0f, 32.0f };
        mJumpSpeed = 510.0f;
        mWalkSpeed = 190.0f;
        mMaxJumpHeight = 3;
        mCurHealth = 6;
        mDamage = 2;
        mKnockBacked = { 130.f, 140.f }; // 넉백 당하는 수치
        mKnockBacks = { 400.f, 250.f }; // 적 몸통 박치기 할 때 주는 넉백 수치
    }
    else if (EnemType == EnemyType::Ninja) {
        mAABB.halfSize = { 18.0f, 28.0f };
        mJumpSpeed = 600.0f;
        mWalkSpeed = 230.0f;
        mMaxJumpHeight = 4;
        mCurHealth = 4;
        mDamage = 2;
        mKnockBacked = { 270.f, 200.f }; // 넉백 당하는 수치
        mKnockBacks = { 260.f, 200.f }; // 적 몸통 박치기 할 때 주는 넉백 수치
    }

    mWidth = (int)ceil(mAABB.halfSize.x / (mMap->mTileSize * 0.5f));
    mHeight = (int)ceil(mAABB.halfSize.y / (mMap->mTileSize * 0.5f));

    mAABBOffset.y = mAABB.halfSize.y;
    
    mLedgeGrabOffset = 4.0f;

    mHeroFindTime = cHeroFindTime;

    InitPathFinder(mMap);
}

void GameBot::SetJumpHeight(int value)
{
    if (value < 0 || value > 6)
        value = 5;
    mMaxJumpHeight = value;
    mJumpSpeed = cJumpSpeed[mMaxJumpHeight - 1];
}

void GameBot::SetLazyLedgeGrabs(bool value)
{
    mLazyLedgeGrabs = value;
}

void GameBot::SetCharacterWidth(int value)
{
    if (value < 0 || value > 9)
        value = 1;

    mAABB.halfSize.x = cHalfSizeX[value];

    mWidth = (int)ceil(mAABB.halfSize.x / (mMap->mTileSize * 0.5f));
}

void GameBot::SetCharacterHeight(int value)
{
    if (value < 0 || value > 9)
        value = 1;

    mAABB.halfSize.y = cHalfSizeY[value];

    mHeight = (int)ceil(mAABB.halfSize.y / (mMap->mTileSize * 0.5f));

    mAABBOffset.y = mAABB.halfSize.y;
}

// 해당 지역에 봇이 들어갈 수 있고, 지면이 있는지 검사
bool GameBot::IsOnGroundAndFitsPos(POINT pos)
{
    for (int y = pos.y; y < pos.y + mHeight; ++y)
    {
        for (int x = pos.x; x < pos.x + mWidth; ++x)
        {
            if (mMap->IsObstacle(x, y))
                return false;
        }
    }

    for (int x = pos.x; x < pos.x + mWidth; ++x)
    {
        if (mMap->IsGround(x, pos.y - 1))
            return true;
    }

    return false;
}

void GameBot::MoveToHero()
{
    if (mHeroFindTime > 0) {
        mHeroFindTime--;
        return;
    }
    else
        mHeroFindTime = cHeroFindTime;
    if (!mOnGround) // 적이 바닥에 있지 않으면 길찾기 불가
        return;
    int Dir[9][2] = { {0,0}, {0,-1}, {1,0}, {0,1}, {-1,0}, {1,-1}, {1,1}, {-1,1}, {-1,-1} };
    POINT MapPoint = mMap->GetMapTileAtPoint(mHero->mPosition);
    for (int i = 0; i < 9; i++)
    {
        POINT Pos = { MapPoint.x + Dir[i][0], MapPoint.y + Dir[i][1] };
        if (IsOnGroundAndFitsPos(Pos)) {
            MoveTo(Pos);
            break;
        }
    }
}

void GameBot::MoveTo(POINT destination)
{
    mStuckFrames = 0;

    POINT startTile = mMap->GetMapTileAtPoint({ mAABB.center.x - mAABB.halfSize.x + mMap->mTileSize / 2, mAABB.center.y - mAABB.halfSize.y + mMap->mTileSize / 2 });

    if (mOnGround && !IsOnGroundAndFitsPos(startTile))
    {
        if (IsOnGroundAndFitsPos({ startTile.x + 1, startTile.y }))
            startTile.x += 1;
        else
            startTile.x -= 1;
    }

    mPath.clear();

    std::vector<POINT>* path = FindPath(startTile, destination, mWidth, mHeight, (short)mMaxJumpHeight, false);

    std::vector<POINT> path1;

    if (mGrabLedgeFlag)
    {
        if (path != NULL) {
            for (int i = 0; i < path->size(); i++)
                path1.push_back(path->at(i));
        }

        path = FindPath(startTile, destination, mWidth, mHeight, (short)mMaxJumpHeight, true);

        mGrabsLedges = true;

        if (mLazyLedgeGrabs && !path1.empty() && path1.size() <= path->size() + 6)
        {
            path = &path1;
            mGrabsLedges = false;
        }
    }

    if (path != NULL && path->size() > 1)
    {
        for (int i = (int)path->size() - 1; i >= 0; --i)
            mPath.push_back(path->at(i));

        mCurrentNodeId = 1;
        mReachedNodeX = false;
        mReachedNodeY = false;
        mCanGrabLedge = false;

        mCurrentBotState = BotState::MoveAhead;

        if (mGrabLedgeFlag)
            mFramesOfJumping = GetJumpFramesForNode(0, mGrabsLedges);
        else
            mFramesOfJumping = GetJumpFramesForNode(0, false);
    }
    else
    {
        mCurrentNodeId = -1;

        if (mCurrentBotState == BotState::MoveAhead)
            mCurrentBotState = BotState::None;
    }
}

void GameBot::MoveTo(POINTFLOAT destination)
{
    MoveTo(mMap->GetMapTileAtPoint(destination));
}

// 점프 얼마나 높게 뛸지 조절
int GameBot::GetJumpFrameCount(int deltaY)
{
    // 도착치까지 블록 차이가 만약 3이면 20프레임을 점프 누르고 있겠다는 것
    if (deltaY <= 0)
        return 0;
    else
    {
        switch (deltaY)
        {
        case 1:
            return 5;
        case 2:
            return 9;
        case 3:
            return 25;
        case 4:
            return 30;
        case 5:
            return 35;
        case 6:
            return 40;
        default:
            return 45;
        }
    }
}

// 노드 해당 위치 X에 도착했느냐
bool GameBot::ReachedNodeOnXAxis(POINTFLOAT pathPosition, POINTFLOAT prevDest, POINTFLOAT currentDest)
{
    return (prevDest.x <= currentDest.x && pathPosition.x >= currentDest.x)
        || (prevDest.x >= currentDest.x && pathPosition.x <= currentDest.x)
        || abs(pathPosition.x - currentDest.x) <= cBotMaxPositionError;
}

// 노드 해당 위치 Y에 도착했느냐
bool GameBot::ReachedNodeOnYAxis(POINTFLOAT pathPosition, POINTFLOAT prevDest, POINTFLOAT currentDest)
{
    return (prevDest.y <= currentDest.y && pathPosition.y >= currentDest.y)
        || (prevDest.y >= currentDest.y && pathPosition.y <= currentDest.y)
        || (abs(pathPosition.y - currentDest.y) <= cBotMaxPositionError);
}

// 봇의 위치 문맥 파악
void GameBot::GetContext(POINTFLOAT& prevDest, POINTFLOAT& currentDest, POINTFLOAT& nextDest, bool& destOnGround)
{
    prevDest = { float(mPath[mCurrentNodeId - 1].x * mMap->mTileSize + mMap->mStPositon.x),
        float(mPath[mCurrentNodeId - 1].y * mMap->mTileSize + mMap->mStPositon.y) };
    currentDest = { float(mPath[mCurrentNodeId].x * mMap->mTileSize + mMap->mStPositon.x),
        float(mPath[mCurrentNodeId].y * mMap->mTileSize + mMap->mStPositon.y) };
    nextDest = currentDest;

    if (mPath.size() > mCurrentNodeId + 1)
    {
        nextDest = { float(mPath[mCurrentNodeId + 1].x * mMap->mTileSize + mMap->mStPositon.x),
            float(mPath[mCurrentNodeId + 1].y * mMap->mTileSize + mMap->mStPositon.y) };
    }

    destOnGround = false;
    for (int x = mPath[mCurrentNodeId].x; x < mPath[mCurrentNodeId].x + mWidth; ++x)
    {
        if (mMap->IsGround(x, mPath[mCurrentNodeId].y - 1))
        {
            destOnGround = true;
            break;
        }
    }

    POINTFLOAT pathPosition = { mAABB.center.x - mAABB.halfSize.x + mMap->mTileSize * 0.5f, mAABB.center.y - mAABB.halfSize.y + mMap->mTileSize * 0.5f };

    if (!mReachedNodeX)
        mReachedNodeX = ReachedNodeOnXAxis(pathPosition, prevDest, currentDest);

    if (!mReachedNodeY)
        mReachedNodeY = ReachedNodeOnYAxis(pathPosition, prevDest, currentDest);

    // cBotMaxPositionError인 위치보다 더 먼 상태로 도착지에 도달했으면 위치 스냅
    if (mReachedNodeX && abs(pathPosition.x - currentDest.x) > cBotMaxPositionError 
        && abs(pathPosition.x - currentDest.x) < cBotMaxPositionError * 3.0f 
        && !mPrevInputs[(int)KeyInput::GoRight] 
        && !mPrevInputs[(int)KeyInput::GoLeft] 
        && !mCanGrabLedge)
    {
        pathPosition.x = currentDest.x;
        mPosition.x = pathPosition.x - mMap->mTileSize * 0.5f + mAABB.halfSize.x + mAABBOffset.x;
    }

    if ((destOnGround && !mOnGround)
        || ((mMustGrabLeftLedge || mMustGrabRightLedge) && mCurrentState != CharacterState::GrabLedge))
        mReachedNodeY = false;

    mMustGrabLeftLedge = mGrabsLedges && !destOnGround && CanGrabLedgeOnLeft(mCurrentNodeId);
    mMustGrabRightLedge = mGrabsLedges && !destOnGround && CanGrabLedgeOnRight(mCurrentNodeId);
}

void GameBot::TestJumpValues()
{
    /*
    if (Input.GetKeyDown(KeyCode.Alpha1))
        mFramesOfJumping = GetJumpFrameCount(1);
    else if (Input.GetKeyDown(KeyCode.Alpha2))
        mFramesOfJumping = GetJumpFrameCount(2);
    else if (Input.GetKeyDown(KeyCode.Alpha3))
        mFramesOfJumping = GetJumpFrameCount(3);
    else if (Input.GetKeyDown(KeyCode.Alpha4))
        mFramesOfJumping = GetJumpFrameCount(4);
    else if (Input.GetKeyDown(KeyCode.Alpha5))
        mFramesOfJumping = GetJumpFrameCount(5);
    else if (Input.GetKeyDown(KeyCode.Alpha6))
        mFramesOfJumping = GetJumpFrameCount(6);
    */
}

int GameBot::GetJumpFramesForNode(int prevNodeId, bool grabLedges)
{
    int currentNodeId = prevNodeId + 1;

    if ((mPath[currentNodeId].y - mPath[prevNodeId].y > 0
        || (mPath[currentNodeId].y - mPath[prevNodeId].y == 0 
            && !mMap->IsGround(mPath[currentNodeId].x, mPath[currentNodeId].y - 1) 
            && mPath[currentNodeId + 1].y - mPath[prevNodeId].y > 0))
        && (mOnGround || mCurrentState == CharacterState::GrabLedge))
    {
        int jumpHeight = 1;
        for (int i = currentNodeId; i < mPath.size(); ++i)
        {
            if (mPath[i].y - mPath[prevNodeId].y >= jumpHeight)
                jumpHeight = mPath[i].y - mPath[prevNodeId].y;
            if (mPath[i].y - mPath[prevNodeId].y < jumpHeight || mMap->IsGround(mPath[i].x, mPath[i].y - 1))
                return (GetJumpFrameCount(jumpHeight));
            else if (grabLedges && CanGrabLedge(i))
                return (GetJumpFrameCount(jumpHeight) + 4);
        }
    }

    return mFramesOfJumping;
}

bool GameBot::CanGrabLedge(int nodeId)
{
    return CanGrabLedgeOnLeft(nodeId) || CanGrabLedgeOnRight(nodeId);
}

bool GameBot::CanGrabLedgeOnLeft(int nodeId)
{
    return (mMap->IsObstacle(mPath[nodeId].x - 1, mPath[nodeId].y + mHeight - 1)
        && !mMap->IsObstacle(mPath[nodeId].x - 1, mPath[nodeId].y + mHeight));
}

bool GameBot::CanGrabLedgeOnRight(int nodeId)
{
    return (mMap->IsObstacle(mPath[nodeId].x + mWidth, mPath[nodeId].y + mHeight - 1)
        && !mMap->IsObstacle(mPath[nodeId].x + mWidth, mPath[nodeId].y + mHeight));
}

void GameBot::MovementUpdate()
{
    switch (mCurrentBotState)
    {
    case BotState::None:

        break;

    case BotState::MoveAhead:

    back:

        POINTFLOAT prevDest, currentDest, nextDest;
        bool destOnGround;
        GetContext(prevDest, currentDest, nextDest, destOnGround);

        POINTFLOAT pathPosition = { mAABB.center.x - mAABB.halfSize.x + mMap->mTileSize / 2, mAABB.center.y - mAABB.halfSize.y + mMap->mTileSize / 2 };

        mInputs[(int)KeyInput::GoRight] = false;
        mInputs[(int)KeyInput::GoLeft] = false;
        mInputs[(int)KeyInput::Jump] = false;
        mInputs[(int)KeyInput::GoDown] = false;

        if (pathPosition.y - currentDest.y > cBotMaxPositionError && mOnOneWayPlatform)
            mInputs[(int)KeyInput::GoDown] = true;

        if (mCanGrabLedge && mCurrentState != CharacterState::GrabLedge)
        {
            if (mMustGrabLeftLedge)
                mInputs[(int)KeyInput::GoLeft] = true;
            else if (mMustGrabRightLedge)
                mInputs[(int)KeyInput::GoRight] = true;
        }
        else if (!mCanGrabLedge && mReachedNodeX && (mMustGrabLeftLedge || mMustGrabRightLedge) &&
            ((pathPosition.y < currentDest.y && (currentDest.y + mMap->mTileSize * mHeight) < pathPosition.y + mAABB.halfSize.y * 2) //approach from bottom
                || (pathPosition.y > currentDest.y && pathPosition.y - currentDest.y < mHeight * mMap->mTileSize))) // approach from top
        {
            mCanGrabLedge = true;

            if (mMustGrabLeftLedge)
                mInputs[(int)KeyInput::GoLeft] = true;
            else if (mMustGrabRightLedge)
                mInputs[(int)KeyInput::GoRight] = true;
        }
        else if ((mReachedNodeX && mReachedNodeY) || (mCanGrabLedge && mCurrentState == CharacterState::GrabLedge))
        {
            int prevNodeId = mCurrentNodeId;
            mCurrentNodeId++;
            mReachedNodeX = false;
            mReachedNodeY = false;
            mCanGrabLedge = false;

            if (mCurrentNodeId >= mPath.size())
            {
                mCurrentNodeId = -1;
                mCurrentBotState = BotState::None;
                break;
            }

            mFramesOfJumping = GetJumpFramesForNode(prevNodeId, mGrabsLedges);

            goto back;
        }
        else if (!mReachedNodeX)
        {
            if (currentDest.x - pathPosition.x > cBotMaxPositionError)
                mInputs[(int)KeyInput::GoRight] = true;
            else if (pathPosition.x - currentDest.x > cBotMaxPositionError)
                mInputs[(int)KeyInput::GoLeft] = true;
        }
        else if (!mReachedNodeY && mPath.size() > mCurrentNodeId + 1 
            && !destOnGround && !(mMustGrabLeftLedge || mMustGrabRightLedge))
        {
            int checkedX = 0;

            int tileX, tileY;
            mMap->GetMapTileAtPoint(pathPosition, tileX, tileY);

            if (mPath[mCurrentNodeId + 1].x != mPath[mCurrentNodeId].x)
            {
                if (mPath[mCurrentNodeId + 1].x > mPath[mCurrentNodeId].x)
                    checkedX = tileX + mWidth;
                else
                    checkedX = tileX - 1;
            }

            if (checkedX != 0 && (!mMap->AnySolidBlockInStripe(checkedX, tileY - 1, mPath[mCurrentNodeId + 1].y) 
                || (abs(pathPosition.x - currentDest.x) > cBotMaxPositionError)))
            {
                //Snap character to the current position if overshot it by more than error margin
                if (mOldPosition.x < currentDest.x && pathPosition.x > currentDest.x)
                {
                    mPosition.x = currentDest.x;
                    pathPosition.x = currentDest.x;
                }

                if (nextDest.x - pathPosition.x > cBotMaxPositionError)
                    mInputs[(int)KeyInput::GoRight] = true;
                else if (pathPosition.x - nextDest.x > cBotMaxPositionError)
                    mInputs[(int)KeyInput::GoLeft] = true;

                if (ReachedNodeOnXAxis(pathPosition, currentDest, nextDest) && ReachedNodeOnYAxis(pathPosition, currentDest, nextDest))
                {
                    mCurrentNodeId += 1;
                    goto back;
                }
            }
        }

        if (mFramesOfJumping > 0 &&
            (mCurrentState == CharacterState::GrabLedge || !mOnGround 
                || (mReachedNodeX && !destOnGround) || (mOnGround && destOnGround)))
        {
            mInputs[(int)KeyInput::Jump] = true;
            if (!mOnGround)
                --mFramesOfJumping;
        }

        if (mCurrentState == CharacterState::GrabLedge && mFramesOfJumping <= 0)
        {
            mInputs[(int)KeyInput::GoDown] = true;
        }


        if (abs(mPosition.x - mOldPosition.x) < 0.1 && abs(mPosition.y - mOldPosition.y) < 0.1)
        {
            ++mStuckFrames;
            if (mStuckFrames > cMaxStuckFrames) {
                MoveTo(mPath[(int)mPath.size() - 1]);
            }
        }
        /*
        if (mPosition.x == mOldPosition.x && mPosition.y == mOldPosition.y)
        {
            ++mStuckFrames;
            if (mStuckFrames > cMaxStuckFrames) {
                MoveTo(mPath[(int)mPath.size() - 1]);
            }
        }
        */
        else
            mStuckFrames = 0;

        break;
    }

    CharacterUpdate();
}

void GameBot::BotUpdate() // 매번 업데이트 해주어야 한다 고정으로
{
    if (mLedgeGrabStopFrames > 0)
    {
        --mLedgeGrabStopFrames;
        return;
    }

    MovementUpdate();
}

void GameBot::DrawEnemy()
{
    POINTFLOAT aabbPos = mAABB.center;
    POINTFLOAT Size;

    if (mEType == EnemyType::Grunt) {
        Size = { 64 , 64 };
        if (mSpeed.x == 0 && mSpeed.y == 0 && mCurrentState == CharacterState::Stand) {
            mSprite->Animation(mHdc, misLeft ^ 1, SpriteType::GruntIdle, mIdleFrame,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
        else if (!mOnGround && mPushesRightWall && mSpeed.y <= 0 && !mAtCeiling) {
            /*
            mSprite->Animation(mHdc, misLeft, SpriteType::GruntGrab, mWallFrame,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
                */
            mSprite->DrawIMG(mHdc, SpriteType::GruntFall, misLeft ^ 1,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
        else if (!mOnGround && mPushesLeftWall && mSpeed.y <= 0 && !mAtCeiling) {
            /*
            mSprite->Animation(mHdc, misLeft, SpriteType::GruntGrab, mWallFrame,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
                */
            mSprite->DrawIMG(mHdc, SpriteType::GruntFall, misLeft ^ 1,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
        else if (mSpeed.y > 0 && mCurrentState == CharacterState::Jump) {
            mSprite->DrawIMG(mHdc, SpriteType::GruntJumping, misLeft ^ 1,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
        else if (mSpeed.y <= 0 && mCurrentState == CharacterState::Jump) {
            mSprite->DrawIMG(mHdc, SpriteType::GruntFall, misLeft ^ 1,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
        else if (mCurrentState == CharacterState::Run) {
            mSprite->Animation(mHdc, misLeft ^ 1, SpriteType::GruntWalking, mWalkFrame,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
        else {
            mSprite->Animation(mHdc, misLeft ^ 1, SpriteType::GruntIdle, mIdleFrame,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
    }
    else if (mEType == EnemyType::Heavy)
    {
        Size = { 42 , 42 };
        if (mSpeed.x == 0 && mSpeed.y == 0 && mCurrentState == CharacterState::Stand) {
            mSprite->Animation(mHdc, misLeft ^ 1, SpriteType::HeavyIdle, mIdleFrame,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
        else if (!mOnGround && mPushesRightWall && mSpeed.y <= 0 && !mAtCeiling) {
            /*
            mSprite->Animation(mHdc, misLeft, SpriteType::HeavyGrab, mWallFrame,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
                */
            mSprite->DrawIMG(mHdc, SpriteType::HeavyFall, misLeft ^ 1,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
        else if (!mOnGround && mPushesLeftWall && mSpeed.y <= 0 && !mAtCeiling) {
            /*
            mSprite->Animation(mHdc, misLeft, SpriteType::HeavyGrab, mWallFrame,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
                */
            mSprite->DrawIMG(mHdc, SpriteType::HeavyFall, misLeft ^ 1,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
        else if (mSpeed.y > 0 && mCurrentState == CharacterState::Jump) {
            mSprite->DrawIMG(mHdc, SpriteType::HeavyJumping, misLeft ^ 1,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
        else if (mSpeed.y <= 0 && mCurrentState == CharacterState::Jump) {
            mSprite->DrawIMG(mHdc, SpriteType::HeavyFall, misLeft ^ 1,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
        else if (mCurrentState == CharacterState::Run) {
            mSprite->Animation(mHdc, misLeft ^ 1, SpriteType::HeavyWalking, mWalkFrame,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
        else {
            mSprite->Animation(mHdc, misLeft ^ 1, SpriteType::HeavyIdle, mIdleFrame,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
    }
    else if (mEType == EnemyType::Massive)
    {
        Size = { 96 , 96 };
        if (mSpeed.x == 0 && mSpeed.y == 0 && mCurrentState == CharacterState::Stand) {
            mSprite->Animation(mHdc, misLeft ^ 1, SpriteType::MassiveIdle, mIdleFrame,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
        else if (!mOnGround && mPushesRightWall && mSpeed.y <= 0 && !mAtCeiling) {
            /*
            mSprite->Animation(mHdc, misLeft, SpriteType::MassiveGrab, mWallFrame,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
                */
            mSprite->DrawIMG(mHdc, SpriteType::MassiveFall, misLeft ^ 1,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
        else if (!mOnGround && mPushesLeftWall && mSpeed.y <= 0 && !mAtCeiling) {
            /*
            mSprite->Animation(mHdc, misLeft, SpriteType::MassiveGrab, mWallFrame,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
                */
            mSprite->DrawIMG(mHdc, SpriteType::MassiveFall, misLeft ^ 1,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
        else if (mSpeed.y > 0 && mCurrentState == CharacterState::Jump) {
            mSprite->DrawIMG(mHdc, SpriteType::MassiveJumping, misLeft ^ 1,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
        else if (mSpeed.y <= 0 && mCurrentState == CharacterState::Jump) {
            mSprite->DrawIMG(mHdc, SpriteType::MassiveFall, misLeft ^ 1,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
        else if (mCurrentState == CharacterState::Run) {
            mSprite->Animation(mHdc, misLeft ^ 1, SpriteType::MassiveWalking, mWalkFrame,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
        else {
            mSprite->Animation(mHdc, misLeft ^ 1, SpriteType::MassiveIdle, mIdleFrame,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
    }
    else if (mEType == EnemyType::Ninja)
    {
        Size = { 64 , 64 };
        if (mSpeed.x == 0 && mSpeed.y == 0 && mCurrentState == CharacterState::Stand) {
            mSprite->Animation(mHdc, misLeft ^ 1, SpriteType::NinjaIdle, mIdleFrame,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
        else if (!mOnGround && mPushesRightWall && mSpeed.y <= 0 && !mAtCeiling) {
            /*
            mSprite->Animation(mHdc, misLeft, SpriteType::NinjaGrab, mWallFrame,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
                */
            mSprite->DrawIMG(mHdc, SpriteType::NinjaFall, misLeft ^ 1,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
        else if (!mOnGround && mPushesLeftWall && mSpeed.y <= 0 && !mAtCeiling) {
            /*
            mSprite->Animation(mHdc, misLeft, SpriteType::NinjaGrab, mWallFrame,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
                */
            mSprite->DrawIMG(mHdc, SpriteType::NinjaFall, misLeft ^ 1,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
        else if (mSpeed.y > 0 && mCurrentState == CharacterState::Jump) {
            mSprite->DrawIMG(mHdc, SpriteType::NinjaJumping, misLeft ^ 1,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
        else if (mSpeed.y <= 0 && mCurrentState == CharacterState::Jump) {
            mSprite->DrawIMG(mHdc, SpriteType::NinjaFall, misLeft ^ 1,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
        else if (mCurrentState == CharacterState::Run) {
            mSprite->Animation(mHdc, misLeft ^ 1, SpriteType::NinjaWalking, mWalkFrame,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
        else {
            mSprite->Animation(mHdc, misLeft ^ 1, SpriteType::NinjaIdle, mIdleFrame,
                int(aabbPos.x - Size.x / 2.f), int(aabbPos.y - mAABB.halfSize.y), Size.x, Size.y);
        }
    }
}