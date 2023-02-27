#include "Bot.h"

#include "Map.h"

Bot::Bot()
{

}

Bot::~Bot()
{

}

// 목표 지점이 공중이면 그 바닥면을 목표 지점으로 채택
void Bot::TappedOnTile(POINT mapPos)
{
    if (mapPos.x < 1 || mapPos.x >= mMap->mWidth - 1 || mapPos.y < 1 || mapPos.y >= mMap->mHeight - 1)
        return;

    while (!(mMap->IsGround(mapPos.x, mapPos.y)))
        --mapPos.y;

    POINT pos = { mapPos.x, mapPos.y + 1 };
    MoveTo(pos);
}

void Bot::InitBot(POINTFLOAT point, POINTFLOAT Scale, Map* stage)
{
    mScale = Scale;

    mPosition = point;

    mMap = stage;

    mAABB.halfSize = { 18, 28 };

    mWidth = (int)ceil(mAABB.halfSize.x / (mMap->mTileSize * 0.5f));
    mHeight = (int)ceil(mAABB.halfSize.y / (mMap->mTileSize * 0.5f));

    mJumpSpeed = 510.f;
    mWalkSpeed = 180.f;

    mGrabLedgeFlag = false;

    mMaxJumpHeight = 3;

    mAABBOffset.y = mAABB.halfSize.y;
    mLedgeGrabOffset = 4.0f;

    mLazyLedgeGrabs = false;

    for (int i = 0; i < KeyInput::Count; i++)
    {
        mInputs.push_back(false);
    }

    mPathFinder.InitPathFinder(mMap);
}


int Bot::GetJumpFrameCount(int deltaY)
{
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
    /*
    // 도착치까지 블록 차이가 만약 3이면 20프레임을 점프 누르고 있겠다는 것
    if (deltaY <= 0)
        return 0;
    else
    {
        switch (deltaY)
        {
        case 1:
            return 1;
        case 2:
            return 4;
        case 3:
            return 8;
        case 4:
            return 11;
        case 5:
            return 15;
        case 6:
            return 21;
        default:
            return 30;
        }
    }
    */
}

void Bot::SetJumpHeight(int value)
{
    if (value < 0 || value > 6)
        value = 5;
    mMaxJumpHeight = value;
    mJumpSpeed = cJumpSpeed[mMaxJumpHeight - 1];
}

void Bot::SetLazyLedgeGrabs(bool value)
{
    mLazyLedgeGrabs = value;
}

void Bot::SetCharacterWidth(int value)
{
    if (value < 0 || value > 9)
        value = 1;
    
    mAABB.halfSize.x = cHalfSizeX[value];

    mWidth = (int)ceil(mAABB.halfSize.x / (mMap->mTileSize * 0.5f));

    mPosition = mTpos;

    mScale.x = sign(mScale.x) * (float)mWidth;
    mLscale = { mScale.x, mScale.y };
}

void Bot::SetCharacterHeight(int value)
{
    if (value < 0 || value > 9)
        value = 1;

    mAABB.halfSize.y = cHalfSizeY[value];

    mHeight = (int)ceil(mAABB.halfSize.y / (mMap->mTileSize * 0.5f));

    mPosition = mTpos;

    mScale.y = (float)mHeight * 0.33333f;
    mLscale = { mScale.x, mScale.y };

    mAABBOffset.y = mAABB.halfSize.y;
}

bool Bot::IsOnGroundAndFitsPos(POINT pos)
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

void Bot::MoveTo(POINT destination)
{
    /*
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

    std::vector<POINT> *path = mPathFinder.FindPath(startTile, destination, mWidth, mHeight, (short)mMaxJumpHeight, false);
    
    std::vector<POINT> *path1 = path;

    std::vector<POINT>* path2 = mPathFinder.FindPath(startTile, destination, mWidth, mHeight, (short)mMaxJumpHeight, true);

    path = path2;
    mGrabsLedges = true;

    if (mLazyLedgeGrabs && path1 != NULL && path1->size() <= path2->size() + 6)
    {
        path = path1;
        mGrabsLedges = false;
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

        mFramesOfJumping = GetJumpFramesForNode(0, mGrabsLedges);
    }
    else
    {
        mCurrentNodeId = -1;

        if (mCurrentBotState == BotState::MoveAhead)
            mCurrentBotState = BotState::None;
    }

    // DrawPathLines();
    */

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

    std::vector<POINT>* path = mPathFinder.FindPath(startTile, destination, mWidth, mHeight, (short)mMaxJumpHeight, false);

    std::vector<POINT> path1;

    if (mGrabLedgeFlag)
    {
        if (path != NULL) {
            for (int i = 0; i < path->size(); i++)
                path1.push_back(path->at(i));
        }

        path = mPathFinder.FindPath(startTile, destination, mWidth, mHeight, (short)mMaxJumpHeight, true);

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

void Bot::MoveTo(POINTFLOAT destination)
{
    MoveTo(mMap->GetMapTileAtPoint(destination));
}

bool Bot::ReachedNodeOnXAxis(POINTFLOAT pathPosition, POINTFLOAT prevDest, POINTFLOAT currentDest)
{
    return (prevDest.x <= currentDest.x && pathPosition.x >= currentDest.x)
        || (prevDest.x >= currentDest.x && pathPosition.x <= currentDest.x)
        || abs(pathPosition.x - currentDest.x) <= cBotMaxPositionError;
}

bool Bot::ReachedNodeOnYAxis(POINTFLOAT pathPosition, POINTFLOAT prevDest, POINTFLOAT currentDest)
{
    return (prevDest.y <= currentDest.y && pathPosition.y >= currentDest.y)
        || (prevDest.y >= currentDest.y && pathPosition.y <= currentDest.y)
        || (abs(pathPosition.y - currentDest.y) <= cBotMaxPositionError);
}

void Bot::GetContext(POINTFLOAT& prevDest, POINTFLOAT& currentDest, POINTFLOAT& nextDest, bool& destOnGround)
{
    prevDest = { float(mPath[mCurrentNodeId - 1].x * mMap->mTileSize + mMap->position.x),
        float(mPath[mCurrentNodeId - 1].y * mMap->mTileSize + mMap->position.y) };
    currentDest = { float(mPath[mCurrentNodeId].x * mMap->mTileSize + mMap->position.x),
        float(mPath[mCurrentNodeId].y * mMap->mTileSize + mMap->position.y) };
    nextDest = currentDest;

    if (mPath.size() > mCurrentNodeId + 1)
    {
        nextDest = { float(mPath[mCurrentNodeId + 1].x * mMap->mTileSize + mMap->position.x),
            float(mPath[mCurrentNodeId + 1].y * mMap->mTileSize + mMap->position.y) };
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

    //snap the character if it reached the goal but overshot it by more than cBotMaxPositionError
    if (mReachedNodeX && abs(pathPosition.x - currentDest.x) > cBotMaxPositionError && abs(pathPosition.x - currentDest.x) < cBotMaxPositionError * 3.0f && !mPrevInputs[(int)KeyInput::GoRight] && !mPrevInputs[(int)KeyInput::GoLeft] && !mCanGrabLedge)
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

void Bot::TestJumpValues()
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

int Bot::GetJumpFramesForNode(int prevNodeId, bool grabLedges)
{
    int currentNodeId = prevNodeId + 1;

    if ((mPath[currentNodeId].y - mPath[prevNodeId].y > 0
        || (mPath[currentNodeId].y - mPath[prevNodeId].y == 0 && !mMap->IsGround(mPath[currentNodeId].x, mPath[currentNodeId].y - 1) && mPath[currentNodeId + 1].y - mPath[prevNodeId].y > 0))
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
                return (GetJumpFrameCount(jumpHeight + 1)); // + 4
        }
    }

    return mFramesOfJumping;
}

bool Bot::CanGrabLedge(int nodeId)
{
    return CanGrabLedgeOnLeft(nodeId) || CanGrabLedgeOnRight(nodeId);
}

bool Bot::CanGrabLedgeOnLeft(int nodeId)
{
    return (mMap->IsObstacle(mPath[nodeId].x - 1, mPath[nodeId].y + mHeight - 1)
        && !mMap->IsObstacle(mPath[nodeId].x - 1, mPath[nodeId].y + mHeight));
}

bool Bot::CanGrabLedgeOnRight(int nodeId)
{
    return (mMap->IsObstacle(mPath[nodeId].x + mWidth, mPath[nodeId].y + mHeight - 1)
        && !mMap->IsObstacle(mPath[nodeId].x + mWidth, mPath[nodeId].y + mHeight));
}

void Bot::MovementUpdate()
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
            ((pathPosition.y < currentDest.y && /*currentDest.y - pathPosition.y < Map.mTileSize - 4 &&*/ (currentDest.y + mMap->mTileSize * mHeight) < pathPosition.y + mAABB.halfSize.y * 2) //approach from bottom
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
        else if (!mReachedNodeY && mPath.size() > mCurrentNodeId + 1 && !destOnGround && !(mMustGrabLeftLedge || mMustGrabRightLedge))
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

            if (checkedX != 0 && (!mMap->AnySolidBlockInStripe(checkedX, tileY - 1, mPath[mCurrentNodeId + 1].y) || (abs(pathPosition.x - currentDest.x) > cBotMaxPositionError)))
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
            (mCurrentState == CharacterState::GrabLedge || !mOnGround || (mReachedNodeX && !destOnGround) || (mOnGround && destOnGround)))
        {
            mInputs[(int)KeyInput::Jump] = true;
            if (!mOnGround)
                --mFramesOfJumping;
        }

        if (mCurrentState == CharacterState::GrabLedge && mFramesOfJumping <= 0)
        {
            mInputs[(int)KeyInput::GoDown] = true;
        }

        if (mPosition.x == mOldPosition.x && mPosition.y == mOldPosition.y)
        {
            ++mStuckFrames;
            if (mStuckFrames > cMaxStuckFrames) {
                MoveTo(mPath[(int)mPath.size() - 1]);
            }
        }
        else
            mStuckFrames = 0;

        break;
    }

    CharacterUpdate(mInputs);
}

void Bot::BotUpdate() // 매번 업데이트 해주어야 한다 고정으로
{   
    if (mLedgeGrabStopFrames > 0)
    {
        --mLedgeGrabStopFrames;
        return;
    }

    MovementUpdate();
}