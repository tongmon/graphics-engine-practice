#include "Character.h"

#include "Map.h"

Character::Character()
{
    mWalkSpeed = cWalkSpeed[4];
    mJumpSpeed = cJumpSpeed[4];
    mPosition = { 0,0 };
    mAABB.halfSize = { cHalfSizeX[1],cHalfSizeY[1] };
    mAABBOffset.y = mAABB.halfSize.y;
    mScale = { 1,1 };
    mLedgeGrabOffset = 4.0f;
}

Character::~Character()
{

}

void Character::InitCharacter(POINTFLOAT Point, POINTFLOAT Scale, Map *stage)
{
    mPosition = Point;
    mScale = Scale;
    GetMap(stage);
}

void Character::UpdatePrevInputs()
{
    int count = (int)KeyInput::Count;
    mPrevInputs.resize(count);
    for (int i = 0; i < count; ++i)
        mPrevInputs[i] = mInputs[i];
}

void Character::HandleJumping()
{
    //increase the number of frames that we've been in the jump state
    ++mFramesFromJumpStart;

    //if we hit the ceiling, we don't want to compensate pro jumping, we can prevent by faking a huge mFramesFromJumpStart
    if (mAtCeiling)
        mFramesFromJumpStart = 100;

    mSpeed.y += cGravity * DeltaTime; // ���� �ӵ��� �߷� �߰�

    mSpeed.y = max(mSpeed.y, cMaxFallingSpeed); // �ʹ� ���� �������� ���� ����

    // ���� ���߿� ����Ű ���� �ӵ� 200���� ����
    // �� ����Ű�� ��� ������ ���� ���� ����
    if (!mInputs[(int)KeyInput::Jump] && mSpeed.y > 0.0f)
    {
        mSpeed.y = min(mSpeed.y, cMinJumpSpeed);
        mFramesFromJumpStart = 100;
    }

    // �¿�Ű�� ��� ������ �¿� �ӵ��� 0
    if (mInputs[(int)KeyInput::GoRight] == mInputs[(int)KeyInput::GoLeft])
    {
        mSpeed.x = 0.0f;
    }
    else if (mInputs[(int)KeyInput::GoRight]) // ���� Ű�� �������� �������� �̵�
    {
        mLscale = { -mScale.x, mScale.y };
        mSpeed.x = mWalkSpeed;

        //..W
        //.H.     <- to not get stuck in these kind of situations we beed to advance
        //..W			the hero forward if he doesn't push a wall anymore
        if (mPushedRightWall && !mPushesRightWall)
            mPosition.x += 1.0f;
    }
    else if (mInputs[(int)KeyInput::GoLeft]) // ���� Ű�� �������� �������� �̵�
    {
        mLscale = { mScale.x, mScale.y };
        mSpeed.x = -mWalkSpeed;

        //W..
        //.H.     <- to not get stuck in these kind of situations we need to advance
        //W..			the hero forward if he doesn't push a wall anymore
        if (mPushedLeftWall && !mPushesLeftWall)
            mPosition.x -= 1.0f;
    }

    // �������� ���ε� ���� Ű�� �����ٸ� ������ ��
    if (mInputs[(int)KeyInput::Jump] && (mOnGround || (mSpeed.y < 0.0f && mFramesFromJumpStart < cJumpFramesThreshold)))
        mSpeed.y = mJumpSpeed;
}

void Character::CharacterUpdate(std::vector<bool> &Inputs)
{
    mInputs = Inputs; // �Է��� �޴´�.
    
    switch (mCurrentState)
    {
    case CharacterState::Stand:

        mSpeed = { 0,0 }; // ó���� �ӵ� 0���� ����

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
        else if (mInputs[(int)KeyInput::Jump]) // ���� Ű�� �����ٸ� ���� ���°� �ȴ�.
        {
            mSpeed.y = mJumpSpeed; // ���� �ӵ� ����
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
            mSpeed = { 0,0 };
        }
        else if (mInputs[(int)KeyInput::GoRight]) // ���� �̵�
        {
            mSpeed.x = mWalkSpeed; // �¿� �������� �ȱ� �ӵ��� ����
            mLscale = { -mScale.x, mScale.y };
        }
        else if (mInputs[(int)KeyInput::GoLeft])
        {
            mSpeed.x = -mWalkSpeed; // ���� �̵��� ���� �ӵ�
            mLscale = { mScale.x, mScale.y };
        }

        // ���� �� �ִ� Ÿ���� ������ ���� ���·� ����
        if (mInputs[(int)KeyInput::Jump]) // ���� Ű�� ����
        {
            mSpeed.y = mJumpSpeed; // �ӵ��� ���� �ӵ�
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

        if (mSpeed.y <= 0.0f && mFramesFromJumpStart > 5 && !mAtCeiling
            && ((mPushesRightWall && mInputs[(int)KeyInput::GoRight]) || (mPushesLeftWall && mInputs[(int)KeyInput::GoLeft])))
        {
            //we'll translate the original aabb's halfSize so we get a vector Vector2iing
            //the top right corner of the aabb when we want to grab the right edge
            //and top left corner of the aabb when we want to grab the left edge
            POINTFLOAT halfSize;

            if (mPushesRightWall && mInputs[(int)KeyInput::GoRight])
                halfSize = mAABB.halfSize;
            else
                halfSize = { -mAABB.halfSize.x - 1.0f, mAABB.halfSize.y };

            halfSize.y += mLedgeGrabOffset;

            int tileIndexX, tileIndexY;
            mMap->GetMapTileAtPoint({ mAABB.center.x + halfSize.x, mAABB.center.y + halfSize.y }, tileIndexX, tileIndexY);

            int oldTileX, oldTileY;
            mMap->GetMapTileAtPoint({ mOldPosition.x + mAABBOffset.x + halfSize.x, mOldPosition.y + mAABBOffset.y + halfSize.y }, oldTileX, oldTileY);

            int startTile = (mPushedLeftWall && mPushesLeftWall) || (mPushedRightWall && mPushesRightWall) ? oldTileY : tileIndexY;

            for (int y = startTile; y >= tileIndexY; --y)
            {
                //check if by snapping into the grabbing position we won't go into a solid block
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

                //check whether the tile on our right corner is empty, if it is then check
                //whether the tile below it (the one we want to grab onto) is not empty
                //and finally if there's a block above us, if there is there's no point to grabbing a ledge cause we can't jump of it
                if (!collidesAfterSnapping
                    && !mMap->IsObstacle(tileIndexX, y)
                    && mMap->IsObstacle(tileIndexX, y - 1)
                    && !mMap->IsObstacle(tileIndexX - (int)sign(halfSize.x), y))
                {
                    //calculate the appropriate corner
                    POINTFLOAT tileCorner = mMap->GetMapTilePosition(tileIndexX, y - 1);
                    tileCorner.x -= sign(halfSize.x) * mMap->mTileSize * 0.5f;
                    tileCorner.y += mMap->mTileSize * 0.5f;

                    //check whether the tile's corner is between our grabbing Vector2is
                    if (y != tileIndexY ||
                        (tileCorner.y - (mAABB.center.y + halfSize.y) >= -cGrabLedgeEndY
                            && tileCorner.y - (mAABB.center.y + halfSize.y) <= -cGrabLedgeStartY))
                    {
                        //save the tile we are holding so we can check later on if we can still hold onto it
                        mLedgeTile = { tileIndexX, y - 1 };

                        //calculate our position so the corner of our AABB and the tile's are next to each other
                        mPosition.y = tileCorner.y - halfSize.y - mAABBOffset.y;
                        mSpeed = { 0,0 };

                        //finally grab the edge
                        mCurrentState = CharacterState::GrabLedge;
                        mLedgeGrabStopFrames = cLedgeGrabStopFrames;
                        break;
                    }
                }
            }
        }

        //if we hit the ground
        if (mOnGround)
        {
            //if there's no movement change state to standing
            if (mInputs[(int)KeyInput::GoRight] == mInputs[(int)KeyInput::GoLeft])
            {
                mCurrentState = CharacterState::Stand;
                mSpeed = { 0,0 };
            }
            else	//either go right or go left are pressed so we change the state to walk
            {
                mCurrentState = CharacterState::Run;
                mSpeed.y = 0.0f;
            }
        }
        break;

    case CharacterState::GrabLedge:

        bool ledgeOnLeft = mMap->position.x + mLedgeTile.x * mMap->mTileSize < mPosition.x;
        bool ledgeOnRight = !ledgeOnLeft;

        //if down button is held then drop down
        if (mInputs[(int)KeyInput::GoDown]
            || (mInputs[(int)KeyInput::GoLeft] && ledgeOnRight)
            || (mInputs[(int)KeyInput::GoRight] && ledgeOnLeft))
        {
            if (ledgeOnLeft)
                mCannotGoLeftFrames = 3;
            else
                mCannotGoRightFrames = 3;

            mCurrentState = CharacterState::Jump;
            //mGame.PlayOneShot(SoundType.Character_LedgeRelease, mPosition, Game.sSfxVolume);
        }
        else if (mInputs[(int)KeyInput::Jump])
        {
            //the speed is positive so we don't have to worry about hero grabbing an edge
            //right after he jumps because he doesn't grab if speed.y > 0
            mSpeed.y = mJumpSpeed;
            mCurrentState = CharacterState::Jump;
        }

        //when the tile we grab onto gets destroyed
        if (!mMap->IsObstacle(mLedgeTile.x, mLedgeTile.y))
            mCurrentState = CharacterState::Jump;

        break;
    }

    UpdatePhysics();

    if (mWasOnGround && !mOnGround)
        mFramesFromJumpStart = 0;

    UpdatePrevInputs();
}