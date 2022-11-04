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

	// AABB �߽� ȹ��
	POINTFLOAT aabbPos = mAABB.center;

	/*
	// �ٴ� üũ ���� ���
	POINTFLOAT bottomLeft = { aabbPos.x - mAABB.halfSize.x + 1, aabbPos.y - mAABB.halfSize.y - 1 };
	POINTFLOAT bottomRight = { bottomLeft.x + mAABB.halfSize.x * 2.0f - 2.0f, bottomLeft.y };

	OldPen = (HPEN)SelectObject(mHdc, NewPen[1]);
	OldBrush = (HBRUSH)SelectObject(mHdc, NewBrush[1]);
	MoveToEx(mHdc, (int)bottomLeft.x, (int)bottomLeft.y, NULL);
	LineTo(mHdc, (int)bottomRight.x, (int)bottomRight.y);

	// õ�� üũ ���� ���
	POINTFLOAT topRight = { aabbPos.x + mAABB.halfSize.x - 1, aabbPos.y + mAABB.halfSize.y + 1 };
	POINTFLOAT topLeft = { topRight.x - mAABB.halfSize.x * 2.0f + 2.0f, topRight.y };

	MoveToEx(mHdc, (int)topLeft.x, (int)topLeft.y, NULL);
	LineTo(mHdc, (int)topRight.x, (int)topRight.y);

	// ���� üũ ���� ���
	bottomLeft = { aabbPos.x - mAABB.halfSize.x - 1, aabbPos.y - mAABB.halfSize.y };
	topLeft = bottomLeft;
	topLeft.y += mAABB.halfSize.y * 2.0f;

	MoveToEx(mHdc, (int)topLeft.x, (int)topLeft.y, NULL);
	LineTo(mHdc, (int)bottomLeft.x, (int)bottomLeft.y);

	// ������ üũ ���� ���
	bottomRight = { aabbPos.x + mAABB.halfSize.x + 1, aabbPos.y - mAABB.halfSize.y };
	topRight = bottomRight;
	topRight.y += mAABB.halfSize.y * 2.0f;

	MoveToEx(mHdc, (int)topRight.x, (int)topRight.y, NULL);
	LineTo(mHdc, (int)bottomRight.x, (int)bottomRight.y);
	*/

	// ��ü �浹 �ڽ� ���
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
	// AABB �߽� ȹ��
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

// õ�� �浹 �Լ�
bool GameObj::HasCeiling(POINTFLOAT position, float& ceilingY)
{
	POINTFLOAT center = { position.x + mAABBOffset.x, position.y + mAABBOffset.y };

	// õ�� y ��ǥ �ʱ�ȭ
	ceilingY = 0.0f;

	// �浹 üũ�� ���� ���� �浹 �κ� ȹ��
	POINTFLOAT topRight = { center.x + mAABB.halfSize.x - 1, center.y + mAABB.halfSize.y + 1 };
	POINTFLOAT topLeft = { topRight.x - mAABB.halfSize.x * 2.0f + 2.0f, topRight.y };

	// get the indices of a tile below us on our left side
	int tileIndexX, tileIndexY;

	// ���� ���� �����ʱ��� �浹�ϴ� ����� �ִ��� �˻�
	for (POINTFLOAT Tile = topLeft; Tile.x < topRight.x + mMap->mTileSize; Tile.x += mMap->mTileSize)
	{
		// topRight ��ġ�� �Ѿ���� Ž���ϴ� �� ����
		Tile.x = min(Tile.x, topRight.x);

		mMap->GetMapTileAtPoint(Tile, tileIndexX, tileIndexY);

		if (tileIndexY < 0 || tileIndexY >= mMap->mHeight) 
			return false;
		if (tileIndexX < 0 || tileIndexX >= mMap->mWidth) 
			return false;

		// Ȯ�� �Ϸ��� Ÿ�� ���� ����� �ƴϸ�
		if (!mMap->IsObstacle(tileIndexX, tileIndexY - 1))
		{
			// �ٷ� ���� �ش� Ÿ���� ������� Ȯ��
			if (mMap->IsObstacle(tileIndexX, tileIndexY))
			{
				// �浹�� õ���� �ٴںκ� �˻�
				ceilingY = (float)tileIndexY * mMap->mTileSize - mMap->mTileSize / 2.0f + mMap->mStPositon.y;
				return true;
			}
		}

		// ��� ��� �˻� �ߴ��� �浹�� ���� ���
		if (Tile.x == topRight.x)
			return false;
	}

	// �浹 �ڽ� ���� ����� ���� ���
	return false;
}

// ���� �浹 �Լ�
bool GameObj::HasGround(POINTFLOAT position, float& groundY)
{
	// AABB�� �߽� ��ǥ ȹ��
	POINTFLOAT center = { position.x + mAABBOffset.x, position.y + mAABBOffset.y };

	// ���� y ��ǥ �ʱ�ȭ
	groundY = 0.0f;

	// �ٴ� �浹 ���������� �ϴ� ���� 1�ȼ� �� ��
	POINTFLOAT bottomLeft = { center.x - mAABB.halfSize.x + 1, center.y - mAABB.halfSize.y - 1 };
	POINTFLOAT bottomRight = { bottomLeft.x + mAABB.halfSize.x * 2.0f - 2.0f, bottomLeft.y };

	int tileIndexX, tileIndexY;

	// ��ü�� ���� Ÿ�ϰ� �浹�ϴ��� �˻�
	// ���� ���� ���� ������ �������� Ÿ�� �˻�
	for (POINTFLOAT Tile = bottomLeft; Tile.x < bottomRight.x + mMap->mTileSize; Tile.x += mMap->mTileSize)
	{
		// ���� �Ѿ�� Ž���ϴ� �� ����
		Tile.x = min(Tile.x, bottomRight.x);

		mMap->GetMapTileAtPoint(Tile, tileIndexX, tileIndexY);

		if (tileIndexY < 0 || tileIndexY >= mMap->mHeight)
			return false;
		if (tileIndexX < 0 || tileIndexX >= mMap->mWidth)
			return false;

		if (!mMap->IsObstacle(tileIndexX, tileIndexY + 1))
		{
			float floorTop = (float)tileIndexY * mMap->mTileSize + mMap->mTileSize / 2.0f + mMap->mStPositon.y;
			
			// Ÿ���� ������� ������ �ٷ� ���� ����
			if (mMap->IsObstacle(tileIndexX, tileIndexY))
			{
				// ������ ���� y ��ǥ ��ȯ
				groundY = floorTop;
				return true;
			}
			// �� ���� ����̸� ���ִ� �������� �ƴ����� ���� ��� ��ȯ
			else if ((mMap->IsOneWayPlatform(tileIndexX, tileIndexY) && !mIgnoresOneWayPlatforms) && mSpeed.y <= 0.0f
				&& abs(Tile.y - floorTop) <= cOneWayPlatformThreshold + mOldPosition.y - position.y)
			{
				groundY = floorTop;
				mOnOneWayPlatform = true; // ������ �� ���� ����� ����� �˸�
			}
		}

		// ��� ��� �� Ž�� �� ���
		if (Tile.x == bottomRight.x)
		{
			if (mOnOneWayPlatform)
				return true;
			return false;
		}
	}

	// �浹 �ڽ� �ؿ� �ƹ��͵� �������� ���� ���
	return false;
}

// ���� ��ֹ� �浹 �Լ�
bool GameObj::CollidesWithRightWall(POINTFLOAT position, float& wallX)
{
	POINTFLOAT center = { position.x + mAABBOffset.x, position.y + mAABBOffset.y };

	// �� ��ǥ �ʱ�ȭ
	wallX = 0.0f;

	// ������ �Ʒ����� ������ ���� ��ġ ����
	POINTFLOAT bottomRight = { center.x + mAABB.halfSize.x + 1, center.y - mAABB.halfSize.y };
	POINTFLOAT topRight = { bottomRight.x, bottomRight.y + mAABB.halfSize.y * 2.0f };

	int tileIndexX, tileIndexY;

	// �Ʒ����� ���� ���� �浹 ����
	for (POINTFLOAT Tile = bottomRight; Tile.y < topRight.y + mMap->mTileSize; Tile.y += mMap->mTileSize)
	{
		// ���� ����� �����Ǵ� �� ����
		Tile.y = min(Tile.y, topRight.y);

		mMap->GetMapTileAtPoint(Tile, tileIndexX, tileIndexY);

		if (tileIndexY < 0 || tileIndexY >= mMap->mHeight) 
			return false;
		if (tileIndexX < 0 || tileIndexX >= mMap->mWidth) 
			return false;

		// ���� ���� �շ��־�� ���� Ÿ�Ͽ� �΋H�� ���� �ִ�.
		if (!mMap->IsObstacle(tileIndexX - 1, tileIndexY))
		{
			// Ÿ���� ��ֹ��̸� ���� �΋H�� ��
			if (mMap->IsObstacle(tileIndexX, tileIndexY))
			{
				// ���� ���� �κ� üũ
				wallX = (float)tileIndexX * mMap->mTileSize - mMap->mTileSize / 2.0f + mMap->mStPositon.x;
				return true;
			}
		}

		// ��� Ÿ�� �����ϰ� �浹�� ���� ���
		if (Tile.y == topRight.y)
			return false;
	}

	return false;
}

// ���� �� �浹 �˻�
bool GameObj::CollidesWithLeftWall(POINTFLOAT position, float& wallX)
{
	POINTFLOAT center = { position.x + mAABBOffset.x, position.y + mAABBOffset.y };

	// �� ��ǥ �ʱ�ȭ
	wallX = 0.0f;

	// ���� �� ���� ��ġ ����
	POINTFLOAT bottomLeft = { center.x - mAABB.halfSize.x - 1, center.y - mAABB.halfSize.y };
	POINTFLOAT topLeft = { bottomLeft.x, bottomLeft.y + mAABB.halfSize.y * 2.0f };

	int tileIndexX, tileIndexY;

	// �Ʒ����� ���� ���� ���� ����
	for (POINTFLOAT Tile = bottomLeft; Tile.y < topLeft.y + mMap->mTileSize; Tile.y += mMap->mTileSize)
	{
		// ���� �� �Ѿ �����ϴ� �� ����
		Tile.y = min(Tile.y, topLeft.y);

		mMap->GetMapTileAtPoint(Tile, tileIndexX, tileIndexY);

		if (tileIndexY < 0 || tileIndexY >= mMap->mHeight)
			return false;
		if (tileIndexX < 0 || tileIndexX >= mMap->mWidth)
			return false;

		// ���� �� ���� Ÿ���� ����ִ� ��츸 üũ
		if (!mMap->IsObstacle(tileIndexX + 1, tileIndexY))
		{
			// Ÿ���� ������� ������
			if (mMap->IsObstacle(tileIndexX, tileIndexY))
			{
				// �ش� �� ���� �˻�
				wallX = (float)tileIndexX * mMap->mTileSize + mMap->mTileSize / 2.0f + mMap->mStPositon.x;
				return true;
			}
		}

		// ��� Ÿ�� ���� �Ŀ� �浹 ���ߴٸ� �浹 �ȳ��ٰ� ��ȯ
		if (Tile.y == topLeft.y)
			return false;
	}

	return false;
}

// ��ü ��� HDC �ʱ�ȭ
void GameObj::SetHDC(HDC hdc)
{
	mHdc = hdc;
}

// ��Ÿ Ÿ�� ȹ��
void GameObj::GetDeltaTime(float dTime)
{
	mDeltaTime = dTime;
}

// ��ü�� ���� �� �ʱ�ȭ
void GameObj::SetMap(GameMap* stage)
{
	mMap = stage;
}

// ���� ȿ�� ������Ʈ
void GameObj::UpdatePhysics()
{
	// ���� ���� ������Ʈ
	mWasOnGround = mOnGround;
	mPushedRightWall = mPushesRightWall;
	mPushedLeftWall = mPushesLeftWall;
	mWasAtCeiling = mAtCeiling;

	// �� ���� ��� ������Ʈ
	mOnOneWayPlatform = false;

	// ���� ���ǵ� ����
	mOldSpeed = mSpeed;

	// ���� ��ǥ ����
	mOldPosition = mPosition;

	// �ӵ��� �̿��Ͽ� ��ü ������Ʈ
	mPosition.x += mSpeed.x * mDeltaTime;
	mPosition.y += mSpeed.y * mDeltaTime;

	bool checkAgainLeft = false;

	float groundY, ceilingY;
	float rightWallX = 0.0f, leftWallX = 0.0f;

	// ���� Ÿ�ϰ� �浹�ϸ� ���� Ÿ�Ͽ� ��ü ����
	if (mSpeed.x <= 0.0f && CollidesWithLeftWall(mPosition, leftWallX))
	{
		if (leftWallX <= mOldPosition.x - mAABB.halfSize.x + mAABBOffset.x)
		{
			// ��ü�� ���� ���� �ٰ� ����ϴ�.
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

	// ���� Ÿ�ϰ� �浹�ϸ� ���� Ÿ�Ͽ� ��ü ����
	if (mSpeed.x >= 0.0f && CollidesWithRightWall(mPosition, rightWallX))
	{
		if (mOldPosition.x + mAABB.halfSize.x + mAABBOffset.x <= rightWallX)
		{
			// ��ü�� ���� ���� �ٰ� ����ϴ�.
			mPosition.x = rightWallX - mAABB.halfSize.x - mAABBOffset.x;
			mSpeed.x = min(mSpeed.x, 0.0f);

			mPushesRightWall = true;
		}
		else
			checkAgainRight = true;
	}
	else
		mPushesRightWall = false;

	// ��ü�� ���� ���� ��� �˻�
	// y �� �ӵ��� ����� ��쿣 ��ü�� ���� ���� ���� ����
	if (HasGround(mPosition, groundY) && mSpeed.y <= 0.0f
		&& mOldPosition.y - mAABB.halfSize.y + mAABBOffset.y >= groundY - 0.5f)
	{
		// ���� �� �κ��� y ��ǥ ���
		mPosition.y = groundY + mAABB.halfSize.y - mAABBOffset.y;

		// �ٴڿ� ������ y�� �ӵ� 0 ����
		mSpeed.y = 0.0f;

		// �� ���� �ִٰ� ǥ��
		mOnGround = true;
	}
	else
		mOnGround = false;

	// õ�� �浹 Ȯ��
	if (HasCeiling(mPosition, ceilingY) && mSpeed.y >= 0.0f
		&& mOldPosition.y + mAABB.halfSize.y + mAABBOffset.y + 1.0f <= ceilingY)
	{
		mPosition.y = ceilingY - mAABB.halfSize.y - mAABBOffset.y - 1.0f;

		// ���� ���� �� ����
		mSpeed.y = 0.0f;

		mAtCeiling = true;
	}
	else
		mAtCeiling = false;

	// �浹 �ߴµ� ����ʿ��� �浹�ߴ��� �𸣸� ������ ����
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

	// ���� AABB ��ġ ������Ʈ
	mAABB.center.x = mPosition.x + mAABBOffset.x;
	mAABB.center.y = mPosition.y + mAABBOffset.y;

	// apply the changes to the transform
	// mTpos = { (float)round(mPosition.x), (float)round(mPosition.y) };
}

// ��ġ ����
void GameObj::SetPos(POINTFLOAT Point)
{
	mPosition = Point;
	mAABB.center.x = mPosition.x + mAABBOffset.x;
	mAABB.center.y = mPosition.y + mAABBOffset.y;
}