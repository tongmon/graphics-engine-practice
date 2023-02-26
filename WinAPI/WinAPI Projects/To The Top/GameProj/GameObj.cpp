#include "GameObj.h"
#include "GameMap.h"

GameObj::GameObj()
{
	mColor = RGB(0, 222, 0);
}

GameObj::~GameObj()
{

}

void GameObj::InitObj(POINTFLOAT Point, POINTFLOAT hSize, GameSprite* Sprite)
{
	mSprite = Sprite;
	mPosition = Point;
	mAABB.center = mPosition;
	mAABB.halfSize = hSize;
}

void GameObj::DrawObject()
{
	HBRUSH NewBrush[2], OldBrush;
	NewBrush[0] = (HBRUSH)CreateSolidBrush(mColor); // GetStockObject(NULL_BRUSH);
	NewBrush[1] = (HBRUSH)CreateSolidBrush(RGB(220, 0, 220));

	HPEN NewPen[2], OldPen;
	NewPen[0] = CreatePen(PS_SOLID, 1, mColor);
	NewPen[1] = CreatePen(PS_SOLID, 2, RGB(220, 0, 220));

	// AABB 중심 획득
	POINTFLOAT aabbPos = mAABB.center;

	/*
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
	*/

	// 물체 충돌 박스 출력
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

void GameObj::DrawObject(SpriteType Type, bool isAnime, int &Frame)
{
	// AABB 중심 획득
	POINTFLOAT aabbPos = mAABB.center;

	if (isAnime == true) {
		mSprite->Animation(mHdc, true, Type, Frame, int(aabbPos.x - mAABB.halfSize.x), int(aabbPos.y - mAABB.halfSize.y)
			, mAABB.halfSize.x * 2, mAABB.halfSize.y * 2);
	}
	else {
		mSprite->DrawIMG(mHdc, Type, true, int(aabbPos.x - mAABB.halfSize.x), int(aabbPos.y - mAABB.halfSize.y)
			, mAABB.halfSize.x * 2, mAABB.halfSize.y * 2);
	}
}

// 천장 충돌 함수
bool GameObj::HasCeiling(POINTFLOAT position, float& ceilingY)
{
	POINTFLOAT center = { position.x + mAABBOffset.x, position.y + mAABBOffset.y };

	// 천장 y 좌표 초기화
	ceilingY = 0.0f;

	// 충돌 체크를 위해 윗면 충돌 부분 획득
	POINTFLOAT topRight = { center.x + mAABB.halfSize.x - 1, center.y + mAABB.halfSize.y + 1 };
	POINTFLOAT topLeft = { topRight.x - mAABB.halfSize.x * 2.0f + 2.0f, topRight.y };

	// get the indices of a tile below us on our left side
	int tileIndexX, tileIndexY;

	// 왼쪽 부터 오른쪽까지 충돌하는 블록이 있는지 검사
	for (POINTFLOAT Tile = topLeft; Tile.x < topRight.x + mMap->mTileSize; Tile.x += mMap->mTileSize)
	{
		// topRight 위치를 넘어서까지 탐색하는 것 방지
		Tile.x = min(Tile.x, topRight.x);

		mMap->GetMapTileAtPoint(Tile, tileIndexX, tileIndexY);

		if (tileIndexY < 0 || tileIndexY >= mMap->mHeight) 
			return false;
		if (tileIndexX < 0 || tileIndexX >= mMap->mWidth) 
			return false;

		// 확인 하려는 타일 밑이 블록이 아니면
		if (!mMap->IsObstacle(tileIndexX, tileIndexY - 1))
		{
			// 바로 위의 해당 타일이 블록인지 확인
			if (mMap->IsObstacle(tileIndexX, tileIndexY))
			{
				// 충돌한 천장의 바닥부분 검사
				ceilingY = (float)tileIndexY * mMap->mTileSize - mMap->mTileSize / 2.0f + mMap->mStPositon.y;
				return true;
			}
		}

		// 모든 블록 검사 했는지 충돌이 없는 경우
		if (Tile.x == topRight.x)
			return false;
	}

	// 충돌 박스 위에 블록이 없는 경우
	return false;
}

// 지면 충돌 함수
bool GameObj::HasGround(POINTFLOAT position, float& groundY)
{
	// AABB의 중심 좌표 획득
	POINTFLOAT center = { position.x + mAABBOffset.x, position.y + mAABBOffset.y };

	// 지면 y 좌표 초기화
	groundY = 0.0f;

	// 바닥 충돌 센서라인은 하단 보다 1픽셀 더 밑
	POINTFLOAT bottomLeft = { center.x - mAABB.halfSize.x + 1, center.y - mAABB.halfSize.y - 1 };
	POINTFLOAT bottomRight = { bottomLeft.x + mAABB.halfSize.x * 2.0f - 2.0f, bottomLeft.y };

	int tileIndexX, tileIndexY;

	// 물체가 지면 타일과 충돌하는지 검사
	// 왼쪽 센서 부터 오른쪽 센서까지 타일 검사
	for (POINTFLOAT Tile = bottomLeft; Tile.x < bottomRight.x + mMap->mTileSize; Tile.x += mMap->mTileSize)
	{
		// 범위 넘어가서 탐색하는 것 방지
		Tile.x = min(Tile.x, bottomRight.x);

		mMap->GetMapTileAtPoint(Tile, tileIndexX, tileIndexY);

		if (tileIndexY < 0 || tileIndexY >= mMap->mHeight)
			return false;
		if (tileIndexX < 0 || tileIndexX >= mMap->mWidth)
			return false;

		if (!mMap->IsObstacle(tileIndexX, tileIndexY + 1))
		{
			float floorTop = (float)tileIndexY * mMap->mTileSize + mMap->mTileSize / 2.0f + mMap->mStPositon.y;
			
			// 타일이 비어있지 않으면 바로 밑이 지면
			if (mMap->IsObstacle(tileIndexX, tileIndexY))
			{
				// 지면의 위쪽 y 좌표 반환
				groundY = floorTop;
				return true;
			}
			// 단 방향 블록이면 서있는 상태인지 아닌지에 따라 결과 반환
			else if ((mMap->IsOneWayPlatform(tileIndexX, tileIndexY) && !mIgnoresOneWayPlatforms) && mSpeed.y <= 0.0f
				&& abs(Tile.y - floorTop) <= cOneWayPlatformThreshold + mOldPosition.y - position.y)
			{
				groundY = floorTop;
				mOnOneWayPlatform = true; // 지면이 단 방향 블록인 사실을 알림
			}
		}

		// 모든 경우 다 탐색 한 경우
		if (Tile.x == bottomRight.x)
		{
			if (mOnOneWayPlatform)
				return true;
			return false;
		}
	}

	// 충돌 박스 밑에 아무것도 존재하지 않은 경우
	return false;
}

// 우측 장애물 충돌 함수
bool GameObj::CollidesWithRightWall(POINTFLOAT position, float& wallX)
{
	POINTFLOAT center = { position.x + mAABBOffset.x, position.y + mAABBOffset.y };

	// 벽 좌표 초기화
	wallX = 0.0f;

	// 위에서 아래까지 오른쪽 센서 위치 설정
	POINTFLOAT bottomRight = { center.x + mAABB.halfSize.x + 1, center.y - mAABB.halfSize.y };
	POINTFLOAT topRight = { bottomRight.x, bottomRight.y + mAABB.halfSize.y * 2.0f };

	int tileIndexX, tileIndexY;

	// 아래에서 위로 센서 충돌 감지
	for (POINTFLOAT Tile = bottomRight; Tile.y < topRight.y + mMap->mTileSize; Tile.y += mMap->mTileSize)
	{
		// 위쪽 벗어나서 감지되는 것 방지
		Tile.y = min(Tile.y, topRight.y);

		mMap->GetMapTileAtPoint(Tile, tileIndexX, tileIndexY);

		if (tileIndexY < 0 || tileIndexY >= mMap->mHeight) 
			return false;
		if (tileIndexX < 0 || tileIndexX >= mMap->mWidth) 
			return false;

		// 전의 벽이 뚫려있어야 현재 타일에 부딫힐 수가 있다.
		if (!mMap->IsObstacle(tileIndexX - 1, tileIndexY))
		{
			// 타일이 장애물이면 벽에 부딫힌 것
			if (mMap->IsObstacle(tileIndexX, tileIndexY))
			{
				// 벽의 우측 부분 체크
				wallX = (float)tileIndexX * mMap->mTileSize - mMap->mTileSize / 2.0f + mMap->mStPositon.x;
				return true;
			}
		}

		// 모든 타일 검출하고도 충돌이 없는 경우
		if (Tile.y == topRight.y)
			return false;
	}

	return false;
}

// 좌측 벽 충돌 검사
bool GameObj::CollidesWithLeftWall(POINTFLOAT position, float& wallX)
{
	POINTFLOAT center = { position.x + mAABBOffset.x, position.y + mAABBOffset.y };

	// 벽 좌표 초기화
	wallX = 0.0f;

	// 좌측 벽 센서 위치 설정
	POINTFLOAT bottomLeft = { center.x - mAABB.halfSize.x - 1, center.y - mAABB.halfSize.y };
	POINTFLOAT topLeft = { bottomLeft.x, bottomLeft.y + mAABB.halfSize.y * 2.0f };

	int tileIndexX, tileIndexY;

	// 아래에서 위로 좌측 센서 감지
	for (POINTFLOAT Tile = bottomLeft; Tile.y < topLeft.y + mMap->mTileSize; Tile.y += mMap->mTileSize)
	{
		// 좌측 위 넘어서 감지하는 것 방지
		Tile.y = min(Tile.y, topLeft.y);

		mMap->GetMapTileAtPoint(Tile, tileIndexX, tileIndexY);

		if (tileIndexY < 0 || tileIndexY >= mMap->mHeight)
			return false;
		if (tileIndexX < 0 || tileIndexX >= mMap->mWidth)
			return false;

		// 좌측 벽 이전 타일이 비어있는 경우만 체크
		if (!mMap->IsObstacle(tileIndexX + 1, tileIndexY))
		{
			// 타일이 비어있지 않으면
			if (mMap->IsObstacle(tileIndexX, tileIndexY))
			{
				// 해당 벽 좌측 검사
				wallX = (float)tileIndexX * mMap->mTileSize + mMap->mTileSize / 2.0f + mMap->mStPositon.x;
				return true;
			}
		}

		// 모든 타일 검출 후에 충돌 안했다면 충돌 안났다고 반환
		if (Tile.y == topLeft.y)
			return false;
	}

	return false;
}

// 물체 출력 HDC 초기화
void GameObj::SetHDC(HDC hdc)
{
	mHdc = hdc;
}

// 델타 타임 획득
void GameObj::GetDeltaTime(float dTime)
{
	mDeltaTime = dTime;
}

// 물체가 보일 맵 초기화
void GameObj::SetMap(GameMap* stage)
{
	mMap = stage;
}

// 물리 효과 업데이트
void GameObj::UpdatePhysics()
{
	// 예전 상태 업데이트
	mWasOnGround = mOnGround;
	mPushedRightWall = mPushesRightWall;
	mPushedLeftWall = mPushesLeftWall;
	mWasAtCeiling = mAtCeiling;

	// 단 방향 블록 업데이트
	mOnOneWayPlatform = false;

	// 예전 스피드 저장
	mOldSpeed = mSpeed;

	// 예전 좌표 저장
	mOldPosition = mPosition;

	// 속도를 이용하여 객체 업데이트
	mPosition.x += mSpeed.x * mDeltaTime;
	mPosition.y += mSpeed.y * mDeltaTime;

	bool checkAgainLeft = false;

	float groundY, ceilingY;
	float rightWallX = 0.0f, leftWallX = 0.0f;

	// 좌측 타일과 충돌하면 좌측 타일에 물체 정렬
	if (mSpeed.x <= 0.0f && CollidesWithLeftWall(mPosition, leftWallX))
	{
		if (leftWallX <= mOldPosition.x - mAABB.halfSize.x + mAABBOffset.x)
		{
			// 물체를 좌측 벽에 붙게 만듭니다.
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

	// 우측 타일과 충돌하면 우측 타일에 물체 정렬
	if (mSpeed.x >= 0.0f && CollidesWithRightWall(mPosition, rightWallX))
	{
		if (mOldPosition.x + mAABB.halfSize.x + mAABBOffset.x <= rightWallX)
		{
			// 물체를 우측 벽에 붙게 만듭니다.
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

	// 천장 충돌 확인
	if (HasCeiling(mPosition, ceilingY) && mSpeed.y >= 0.0f
		&& mOldPosition.y + mAABB.halfSize.y + mAABBOffset.y + 1.0f <= ceilingY)
	{
		mPosition.y = ceilingY - mAABB.halfSize.y - mAABBOffset.y - 1.0f;

		// 위로 가는 것 방지
		mSpeed.y = 0.0f;

		mAtCeiling = true;
	}
	else
		mAtCeiling = false;

	// 충돌 했는데 어느쪽에서 충돌했는지 모르면 수평쪽 정렬
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

	// apply the changes to the transform
	// mTpos = { (float)round(mPosition.x), (float)round(mPosition.y) };
}

// 위치 지정
void GameObj::SetPos(POINTFLOAT Point)
{
	mPosition = Point;
	mAABB.center.x = mPosition.x + mAABBOffset.x;
	mAABB.center.y = mPosition.y + mAABBOffset.y;
}