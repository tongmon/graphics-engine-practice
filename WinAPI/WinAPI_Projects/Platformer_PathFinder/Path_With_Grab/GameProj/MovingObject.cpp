#include "MovingObject.h"
#include "Map.h"

MovingObject::MovingObject()
{
	mColor = RGB(250, 250, 0);
}

MovingObject::~MovingObject()
{

}

void MovingObject::DrawMovingObject()
{
	HBRUSH NewBrush[2], OldBrush;
	NewBrush[0] = (HBRUSH)CreateSolidBrush(mColor); // GetStockObject(NULL_BRUSH);
	NewBrush[1] = (HBRUSH)CreateSolidBrush(RGB(220, 0, 220));

	HPEN NewPen[2], OldPen;
	NewPen[0] = CreatePen(PS_SOLID, 1, mColor);
	NewPen[1] = CreatePen(PS_SOLID, 2, RGB(220, 0, 220));
	
	//calculate the position of the aabb's center
	POINTFLOAT aabbPos = mAABB.center;

	// 바닥 체크 센서 출력
	POINTFLOAT bottomLeft = { aabbPos.x - mAABB.halfSize.x + 1, aabbPos.y - mAABB.halfSize.y - 1 };
	POINTFLOAT bottomRight = { bottomLeft.x + mAABB.halfSize.x * 2.0f - 2.0f, bottomLeft.y };

	OldPen = (HPEN)SelectObject(mHdc, NewPen[1]);
	OldBrush = (HBRUSH)SelectObject(mHdc, NewBrush[1]);
	MoveToEx(mHdc, (int)bottomLeft.x, (int)bottomLeft.y, NULL);
	LineTo(mHdc, (int)bottomRight.x, (int)bottomRight.y);

	// 천장 체크 센서 출력
	POINTFLOAT topRight = { aabbPos.x + mAABB.halfSize.x - 1, aabbPos.y + mAABB.halfSize.y + 1 };
	POINTFLOAT topLeft = { topRight.x - mAABB.halfSize.x * 2.0f + 2.0f, topRight.y };

	MoveToEx(mHdc, (int)topLeft.x, (int)topLeft.y, NULL);
	LineTo(mHdc, (int)topRight.x, (int)topRight.y);

	// 왼쪽 체크 센서 출력
	bottomLeft = { aabbPos.x - mAABB.halfSize.x - 1, aabbPos.y - mAABB.halfSize.y };
	topLeft = bottomLeft;
	topLeft.y += mAABB.halfSize.y * 2.0f;

	MoveToEx(mHdc, (int)topLeft.x, (int)topLeft.y, NULL);
	LineTo(mHdc, (int)bottomLeft.x, (int)bottomLeft.y);

	// 오른쪽 체크 센서 출력
	bottomRight = { aabbPos.x + mAABB.halfSize.x + 1, aabbPos.y - mAABB.halfSize.y };
	topRight = bottomRight;
	topRight.y += mAABB.halfSize.y * 2.0f;

	MoveToEx(mHdc, (int)topRight.x, (int)topRight.y, NULL);
	LineTo(mHdc, (int)bottomRight.x, (int)bottomRight.y);

	// 캐릭터 충돌 박스 출력
	OldPen = (HPEN)SelectObject(mHdc, NewPen[0]);
	OldBrush = (HBRUSH)SelectObject(mHdc, NewBrush[0]);
	Rectangle(mHdc, int(aabbPos.x - mAABB.halfSize.x), int(aabbPos.y + mAABB.halfSize.y), int(aabbPos.x + mAABB.halfSize.x), int(aabbPos.y - mAABB.halfSize.y));

	for (int i = 0; i < 2; i++)
	{
		OldPen = (HPEN)SelectObject(mHdc, NewPen[i]);
		SelectObject(mHdc, OldPen);
		DeleteObject(NewPen[i]);
		OldBrush = (HBRUSH)SelectObject(mHdc, NewBrush[i]);
		SelectObject(mHdc, OldBrush);
		DeleteObject(NewBrush[i]);
	}
}

bool MovingObject::HasCeiling(POINTFLOAT position, float &ceilingY)
{
	POINTFLOAT center = { position.x + mAABBOffset.x, position.y + mAABBOffset.y };

	//init the groundY
	ceilingY = 0.0f;

	//set the Vector2is right below us on our left and right sides
	POINTFLOAT topRight = { center.x + mAABB.halfSize.x - 1, center.y + mAABB.halfSize.y + 1 };
	POINTFLOAT topLeft = { topRight.x - mAABB.halfSize.x * 2.0f + 2.0f, topRight.y };

	//get the indices of a tile below us on our left side
	int tileIndexX, tileIndexY;

	//iterate over all the tiles that the object may collide with from the left to the right
	for (POINTFLOAT checkedVector2i = topLeft; checkedVector2i.x < topRight.x + mMap->mTileSize; checkedVector2i.x += mMap->mTileSize)
	{
		//makre sure that we don't check beyound the top right corner
		checkedVector2i.x = min(checkedVector2i.x, topRight.x);

		mMap->GetMapTileAtPoint(checkedVector2i, tileIndexX, tileIndexY);

		if (tileIndexY < 0 || tileIndexY >= mMap->mHeight) return false;
		if (tileIndexX < 0 || tileIndexX >= mMap->mWidth) return false;

		//if below this tile there is another tile, that means we can't possibly
		//hit it without hitting the one below, so we can immidiately skip to the topRight corner check
		if (!mMap->IsObstacle(tileIndexX, tileIndexY - 1))
		{
			//if the tile is not empty, it means we have ceiling right above us
			if (mMap->IsObstacle(tileIndexX, tileIndexY))
			{
				//calculate the y position of the bottom of the ceiling tile
				ceilingY = (float)tileIndexY * mMap->mTileSize - mMap->mTileSize / 2.0f + mMap->position.y;
				return true;
			}
		}

		//if we checked all the possible tiles and there's nothing right above the aabb
		if (checkedVector2i.x == topRight.x)
			return false;
	}

	//there's nothing right above the aabb
	return false;
}

bool MovingObject::HasGround(POINTFLOAT position, float &groundY)
{
	// AABB의 중심 좌표 획득
	POINTFLOAT center = { position.x + mAABBOffset.x, position.y + mAABBOffset.y };

	//init the groundY
	groundY = 0.0f;

	// 바닥 충돌 센서라인은 하단 보다 1픽셀 더 밑
	POINTFLOAT bottomLeft = { center.x - mAABB.halfSize.x + 1, center.y - mAABB.halfSize.y - 1 };
	POINTFLOAT bottomRight = { bottomLeft.x + mAABB.halfSize.x * 2.0f - 2.0f, bottomLeft.y };

	//left side
	//calculate the indices of a tile below us on our left side
	int tileIndexX, tileIndexY;

	// 물체가 지면 타일과 충돌하는지 검사
	// 왼쪽 센서 부터 오른쪽 센서까지 타일 검사
	for (POINTFLOAT checkedVector2i = bottomLeft; checkedVector2i.x < bottomRight.x + mMap->mTileSize; checkedVector2i.x += mMap->mTileSize)
	{
		// make sure that we don't check beyound the bottom right corner
		checkedVector2i.x = min(checkedVector2i.x, bottomRight.x);

		mMap->GetMapTileAtPoint(checkedVector2i, tileIndexX, tileIndexY);

		if (tileIndexY < 0 || tileIndexY >= mMap->mHeight)
			return false;
		if (tileIndexX < 0 || tileIndexX >= mMap->mWidth)
			return false;

		// if above this tile there is another tile, that means we can't possibly
		// hit it without hitting the one above
		if (!mMap->IsObstacle(tileIndexX, tileIndexY + 1))
		{
			float floorTop = (float)tileIndexY * mMap->mTileSize + mMap->mTileSize / 2.0f + mMap->position.y;
			// if the tile is not empty, it means we have a floor right below us
			if (mMap->IsObstacle(tileIndexX, tileIndexY))
			{
				// calculate the y position of the floor tile's top
				groundY = floorTop;
				return true;
			}// if there's a one way platform below us, treat it as a floor only if we're falling or standing
			else if ((mMap->IsOneWayPlatform(tileIndexX, tileIndexY) && !mIgnoresOneWayPlatforms) && mSpeed.y <= 0.0f
				&& abs(checkedVector2i.y - floorTop) <= cOneWayPlatformThreshold + mOldPosition.y - position.y)
			{
				groundY = floorTop;
				mOnOneWayPlatform = true;
			}
		}

		//if we checked all the possible tiles and there's nothing right below the aabb
		if (checkedVector2i.x == bottomRight.x)
		{
			if (mOnOneWayPlatform)
				return true;
			return false;
		}
	}

	//there's nothing right beneath the aabb
	return false;
}

bool MovingObject::CollidesWithRightWall(POINTFLOAT position, float& wallX)
{
	POINTFLOAT center = { position.x + mAABBOffset.x, position.y + mAABBOffset.y };

	//init the wallX
	wallX = 0.0f;

	//calculate the bottom left and top left vertices of our aabb
	POINTFLOAT bottomRight = { center.x + mAABB.halfSize.x + 1, center.y - mAABB.halfSize.y };
	POINTFLOAT topRight = { bottomRight.x, bottomRight.y + mAABB.halfSize.y * 2.0f };

	//get the bottom right vertex's tile indices
	int tileIndexX, tileIndexY;

	//iterate over all the tiles that the object may collide with from the top to the bottom
	for (POINTFLOAT checkedVector2i = bottomRight; checkedVector2i.y < topRight.y + mMap->mTileSize; checkedVector2i.y += mMap->mTileSize)
	{
		//make sure that we don't check beyound the top right corner
		checkedVector2i.y = min(checkedVector2i.y, topRight.y);

		mMap->GetMapTileAtPoint(checkedVector2i, tileIndexX, tileIndexY);

		if (tileIndexY < 0 || tileIndexY >= mMap->mHeight) return false;
		if (tileIndexX < 0 || tileIndexX >= mMap->mWidth) return false;

		//if the tile has another tile on the left, we can't touch the tile's left side because it's blocked
		if (!mMap->IsObstacle(tileIndexX - 1, tileIndexY))
		{
			//if the tile is not empty, then we hit the wall
			if (mMap->IsObstacle(tileIndexX, tileIndexY))
			{
				//calculate the x position of the left side of the wall
				wallX = (float)tileIndexX * mMap->mTileSize - mMap->mTileSize / 2.0f + mMap->position.x;
				return true;
			}
		}

		//if we checked all the possible tiles and there's nothing right next to the aabb
		if (checkedVector2i.y == topRight.y)
			return false;
	}

	return false;
}

bool MovingObject::CollidesWithLeftWall(POINTFLOAT position, float& wallX)
{
	POINTFLOAT center = { position.x + mAABBOffset.x, position.y + mAABBOffset.y };

	//init the wallX
	wallX = 0.0f;

	//calculate the bottom left and top left vertices of our mAABB.
	POINTFLOAT bottomLeft = { center.x - mAABB.halfSize.x - 1, center.y - mAABB.halfSize.y };
	POINTFLOAT topLeft = { bottomLeft.x, bottomLeft.y + mAABB.halfSize.y * 2.0f };

	//get the bottom left vertex's tile indices
	int tileIndexX, tileIndexY;

	//iterate over all the tiles that the object may collide with from the top to the bottom
	for (POINTFLOAT checkedVector2i = bottomLeft; checkedVector2i.y < topLeft.y + mMap->mTileSize; checkedVector2i.y += mMap->mTileSize)
	{
		//make sure that we don't check beyound the top right corner
		checkedVector2i.y = min(checkedVector2i.y, topLeft.y);

		mMap->GetMapTileAtPoint(checkedVector2i, tileIndexX, tileIndexY);

		if (tileIndexY < 0 || tileIndexY >= mMap->mHeight) return false;
		if (tileIndexX < 0 || tileIndexX >= mMap->mWidth) return false;

		//if the tile has another tile on the right, we can't touch the tile's right side because it's blocked
		if (!mMap->IsObstacle(tileIndexX + 1, tileIndexY))
		{
			//if the tile is not empty, then we hit the wall
			if (mMap->IsObstacle(tileIndexX, tileIndexY))
			{
				//calculate the x position of the right side of the wall
				wallX = (float)tileIndexX * mMap->mTileSize + mMap->mTileSize / 2.0f + mMap->position.x;
				return true;
			}
		}

		//if we checked all the possible tiles and there's nothing right next to the aabb
		if (checkedVector2i.y == topLeft.y)
			return false;
	}

	return false;
}

void MovingObject::GetHDC(HDC hdc)
{
	mHdc = hdc;
}

void MovingObject::GetDeltaTime(float dTime)
{
	DeltaTime = dTime;
}

void MovingObject::GetMap(Map* stage)
{
	mMap = stage;
}

void MovingObject::UpdatePhysics()
{
	mWasOnGround = mOnGround;
	mPushedRightWall = mPushesRightWall;
	mPushedLeftWall = mPushesLeftWall;
	mWasAtCeiling = mAtCeiling;

	mOnOneWayPlatform = false;

	//save the speed to oldSpeed vector
	mOldSpeed = mSpeed;

	//save the position to the oldPosition vector
	mOldPosition = mPosition;

	// 속도를 이용하여 객체 업데이트
	mPosition.x += mSpeed.x * DeltaTime;
	mPosition.y += mSpeed.y * DeltaTime;

	bool checkAgainLeft = false;

	float groundY, ceilingY;
	float rightWallX = 0.0f, leftWallX = 0.0f;

	// if we overlap a tile on the left then align the hero
	if (mSpeed.x <= 0.0f && CollidesWithLeftWall(mPosition, leftWallX))
	{
		if (mOldPosition.x - mAABB.halfSize.x + mAABBOffset.x >= leftWallX)
		{
			mPosition.x = leftWallX + mAABB.halfSize.x - mAABBOffset.x;
			mSpeed.x = max(mSpeed.x, 0.0f);

			mPushesLeftWall = true;
		}
		else
			checkAgainLeft = true;
	}
	else
		mPushesLeftWall = false;

	bool checkAgainRight = false;

	//if we overlap a tile on the right then align the hero
	if (mSpeed.x >= 0.0f && CollidesWithRightWall(mPosition, rightWallX))
	{
		if (mOldPosition.x + mAABB.halfSize.x + mAABBOffset.x <= rightWallX)
		{
			mPosition.x = rightWallX - mAABB.halfSize.x - mAABBOffset.x;
			mSpeed.x = min(mSpeed.x, 0.0f);

			mPushesRightWall = true;
		}
		else
			checkAgainRight = true;
	}
	else
		mPushesRightWall = false;

	// 물체가 땅에 닿을 경우 검사
	// y 축 속도가 양수인 경우엔 물체는 땅에 닿을 수가 없다
	if (HasGround(mPosition, groundY) && mSpeed.y <= 0.0f
		&& mOldPosition.y - mAABB.halfSize.y + mAABBOffset.y >= groundY - 0.5f)
	{
		// 지면 윗 부분의 y 좌표 계산
		mPosition.y = groundY + mAABB.halfSize.y - mAABBOffset.y;

		// 바닥에 닿으니 y축 속도 0 설정
		mSpeed.y = 0.0f;

		// 땅 위에 있다고 표시
		mOnGround = true;
	}
	else
		mOnGround = false;

	//check if the hero hit the ceiling
	if (HasCeiling(mPosition, ceilingY) && mSpeed.y >= 0.0f
		&& mOldPosition.y + mAABB.halfSize.y + mAABBOffset.y + 1.0f <= ceilingY)
	{
		mPosition.y = ceilingY - mAABB.halfSize.y - mAABBOffset.y - 1.0f;

		//stop going up
		mSpeed.y = 0.0f;

		mAtCeiling = true;
	}
	else
		mAtCeiling = false;

	//if we are colliding with the block but we don't know from which side we had hit him, just prioritize the horizontal alignment
	if (checkAgainLeft && !mOnGround && !mAtCeiling)
	{
		mPosition.x = leftWallX + mAABB.halfSize.x;
		mSpeed.x = max(mSpeed.x, 0.0f);

		mPushesLeftWall = true;
	}
	else if (checkAgainRight && !mOnGround && !mAtCeiling)
	{
		mPosition.x = rightWallX - mAABB.halfSize.x;
		mSpeed.x = min(mSpeed.x, 0.0f);

		mPushesRightWall = true;
	}

	// 최종 AABB 위치 업데이트
	mAABB.center.x = mPosition.x + mAABBOffset.x;
	mAABB.center.y = mPosition.y + mAABBOffset.y;

	//apply the changes to the transform
	mTpos = { (float)round(mPosition.x), (float)round(mPosition.y) };
}