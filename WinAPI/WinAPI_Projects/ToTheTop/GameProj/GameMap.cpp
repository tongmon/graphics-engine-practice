#include "GameMap.h"
#include "GameSprite.h"
#include "Constants.h"

GameMap::GameMap()
{

}

GameMap::~GameMap()
{

}

void GameMap::InitMap(std::vector<std::vector<short>> *Grid, GameSprite* Sprite)
{
    mSprite = Sprite;
    
    // 타일 한개 크기 조절
    mTileSize = cTileSize;

    std::vector<TileType> tileLine;
    for (int i = 0; i < Grid->size(); i++)
    {
        mTiles.push_back(tileLine);
        for (int j = 0; j < Grid->at(i).size(); j++)
        {
            if ((TileType)Grid->at(i).at(j) == TileType::Block &&
                (j == 0 || i == 0 || j == Grid->at(i).size() - 1 || i == Grid->size() - 1))
                mTiles[i].push_back(TileType::Border);
            else
                mTiles[i].push_back((TileType)Grid->at(i).at(j));
        }
    }

    // 맵 너비, 높이 획득
    mWidth = (int)mTiles[0].size();
    mHeight = (int)mTiles.size();

    int WidthLen = mWidth * mTileSize;
    int HeightLen = mHeight * mTileSize;

    // 맵 시작 좌표 설정
    mStPositon = { (cBaseWidth - WidthLen) / 2 + mTileSize / 2, (cBaseHeight - HeightLen) / 2 + mTileSize / 2 };

    // 실제 위치 계산에 용이한 형태로 배열을 만듦
    for (int i = 0, j = mHeight - 1; i < mHeight / 2; i++, j--)
        std::swap(mTiles[i], mTiles[j]);

    std::vector<short> gridLine;
    for (int y = 0; y < mHeight; ++y)
    {
        mGrid.push_back(gridLine);
        for (int x = 0; x < mWidth; ++x)
        {
            if (mTiles[y][x] == TileType::Block || mTiles[y][x] == TileType::Door || mTiles[y][x] == TileType::Border)
                mGrid[y].push_back(0);
            else
                mGrid[y].push_back(1);
        }
    }
}

// 해당 칸의 타일 획득
TileType GameMap::GetTile(int x, int y)
{
    if (x < 0 || x >= mWidth
        || y < 0 || y >= mHeight)
        return TileType::Block;

    return mTiles[y][x];
}

// 단일 방향 블록인지 확인
bool GameMap::IsOneWayPlatform(int x, int y)
{
    if (x < 0 || x >= mWidth
        || y < 0 || y >= mHeight)
        return false;

    return (mTiles[y][x] == TileType::OneWay);
}

// 해당 블록이 지면 역할을 하는지 확인
bool GameMap::IsGround(int x, int y)
{
    if (x < 0 || x >= mWidth
        || y < 0 || y >= mHeight)
        return false;

    return (mTiles[y][x] == TileType::OneWay || mTiles[y][x] == TileType::Block || mTiles[y][x] == TileType::Border);
}

bool GameMap::IsGround(POINT pos)
{
    return IsGround(pos.x, pos.y);
}

// 해당 칸이 블록인지 확인
bool GameMap::IsObstacle(int x, int y)
{
    if (x < 0 || x >= mWidth
        || y < 0 || y >= mHeight)
        return true;

    return (mTiles[y][x] == TileType::Block || mTiles[y][x] == TileType::Border);
}

// 해당 칸이 안 비었는지 확인
bool GameMap::IsNotEmpty(int x, int y)
{
    if (x < 0 || x >= mWidth
        || y < 0 || y >= mHeight)
        return true;

    return (mTiles[y][x] != TileType::Empty);
}

// 실제 좌표를 맵에서 해당 좌표로 반환
void GameMap::GetMapTileAtPoint(POINTFLOAT point, int& tileIndexX, int& tileIndexY)
{
    tileIndexY = (int)((point.y - mStPositon.y + mTileSize / 2.0f) / (float)(mTileSize));
    tileIndexX = (int)((point.x - mStPositon.x + mTileSize / 2.0f) / (float)(mTileSize));
}

POINT GameMap::GetMapTileAtPoint(POINTFLOAT point)
{
    return { (int)((point.x - mStPositon.x + mTileSize / 2.0f) / (float)(mTileSize)),
        (int)((point.y - mStPositon.y + mTileSize / 2.0f) / (float)(mTileSize)) };
}

POINTFLOAT GameMap::GetMapTilePosition(int tileIndexX, int tileIndexY)
{
    return { (float)(tileIndexX * mTileSize) + mStPositon.x, (float)(tileIndexY * mTileSize) + mStPositon.y };
}

POINTFLOAT GameMap::GetMapTilePosition(POINT tileCoords)
{
    return { (float)(tileCoords.x * mTileSize) + mStPositon.x, (float)(tileCoords.y * mTileSize) + mStPositon.y };
}

// 맵 블록과 충돌했는지 여부 파악
bool GameMap::CollidesWithMapTile(AABB aabb, int tileIndexX, int tileIndexY)
{
    POINTFLOAT tilePos = GetMapTilePosition(tileIndexX, tileIndexY);

    return aabb.Overlaps(tilePos, { (float)(mTileSize) / 2.0f, (float)(mTileSize) / 2.0f });
}

// 범위 내에 블록 있는지 판단
bool GameMap::AnySolidBlockInRectangle(POINTFLOAT start, POINTFLOAT end)
{
    POINT Spoint = GetMapTileAtPoint(start);
    POINT Epoint = GetMapTileAtPoint(end);
    return AnySolidBlockInRectangle(Spoint, Epoint);
}

// 줄 하나로 y0 ~ y1 사이에 장애물이 있는지 검사
bool GameMap::AnySolidBlockInStripe(int x, int y0, int y1)
{
    int startY, endY;

    if (y0 <= y1)
    {
        startY = y0;
        endY = y1;
    }
    else
    {
        startY = y1;
        endY = y0;
    }

    for (int y = startY; y <= endY; ++y)
    {
        if (GetTile(x, y) == TileType::Block || GetTile(x, y) == TileType::Border)
            return true;
    }

    return false;
}

// start ~ end 사이에 장애물이 있는지 검사
bool GameMap::AnySolidBlockInRectangle(POINT start, POINT end)
{
    int startX, startY, endX, endY;

    if (start.x <= end.x)
    {
        startX = start.x;
        endX = end.x;
    }
    else
    {
        startX = end.x;
        endX = start.x;
    }

    if (start.y <= end.y)
    {
        startY = start.y;
        endY = end.y;
    }
    else
    {
        startY = end.y;
        endY = start.y;
    }

    for (int y = startY; y <= endY; ++y)
    {
        for (int x = startX; x <= endX; ++x)
        {
            if (GetTile(x, y) == TileType::Block || GetTile(x, y) == TileType::Border)
                return true;
        }
    }

    return false;
}

void GameMap::SetTile(int x, int y, TileType type)
{
    if (x < 1 || x >= mWidth - 1 || y < 1 || y >= mHeight - 1)
        return;

    mTiles[y][x] = type;

    if (type == TileType::Block || type == TileType::Border) // 블록이면 0
        mGrid[y][x] = 0;
    else
        mGrid[y][x] = 1; // 아니면 1
}

/*
void GameMap::DrawMap()
{
    HBRUSH NewBrush[5], OldBrush;
    NewBrush[0] = (HBRUSH)GetStockObject(NULL_BRUSH);
    NewBrush[1] = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));
    NewBrush[2] = (HBRUSH)CreateSolidBrush(RGB(0, 255, 0));
    NewBrush[3] = (HBRUSH)CreateSolidBrush(RGB(255, 255, 0));
    NewBrush[4] = (HBRUSH)CreateSolidBrush(RGB(255, 0, 255));

    HPEN NewPen[5], OldPen;
    NewPen[0] = CreatePen(PS_SOLID, 1, NULL_PEN);
    NewPen[1] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    NewPen[2] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    NewPen[3] = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
    NewPen[4] = CreatePen(PS_SOLID, 1, RGB(255, 0, 255));

    OldPen = (HPEN)SelectObject(mHdc, NewPen[0]);
    OldBrush = (HBRUSH)SelectObject(mHdc, NewBrush[0]);

    // 충돌 판정이 블록 중심 기준이여서 반블록 크기 만큼 위, 아래로 감소
    int BrickW, BrickH;
    for (int i = 0; i < mHeight; i++)
    {
        BrickH = mStPositon.y + mTileSize * i - mTileSize / 2;
        for (int j = 0; j < mWidth; j++)
        {
            BrickW = mStPositon.x + j * mTileSize - mTileSize / 2;

            OldPen = (HPEN)SelectObject(mHdc, NewPen[0]);
            OldBrush = (HBRUSH)SelectObject(mHdc, NewBrush[0]);
            if (GetTile(j, i) == TileType::Block) {
                OldPen = (HPEN)SelectObject(mHdc, NewPen[1]);
                OldBrush = (HBRUSH)SelectObject(mHdc, NewBrush[1]);
            }
            else if (GetTile(j, i) == TileType::OneWay) {
                OldPen = (HPEN)SelectObject(mHdc, NewPen[2]);
                OldBrush = (HBRUSH)SelectObject(mHdc, NewBrush[2]);
            }
            else if (GetTile(j, i) == TileType::Door) {
                OldPen = (HPEN)SelectObject(mHdc, NewPen[3]);
                OldBrush = (HBRUSH)SelectObject(mHdc, NewBrush[3]);
            }

            Rectangle(mHdc, BrickW, BrickH + mTileSize, BrickW + mTileSize, BrickH);
        }
    }

    for (int i = 0; i < 5; i++)
    {
        OldPen = (HPEN)SelectObject(mHdc, NewPen[i]);
        SelectObject(mHdc, OldPen);
        DeleteObject(NewPen[i]);
        OldBrush = (HBRUSH)SelectObject(mHdc, NewBrush[i]);
        SelectObject(mHdc, OldBrush);
        DeleteObject(NewBrush[i]);
    }
}
*/

void GameMap::DrawMap()
{
    // 충돌 판정이 블록 중심 기준이여서 반블록 크기 만큼 위, 아래로 감소
    int BrickW, BrickH, FlagX = 0, FlagY = 0;
    for (int i = 0; i < mHeight; i++)
    {
        BrickH = mStPositon.y + mTileSize * i - mTileSize / 2;
        for (int j = 0; j < mWidth; j++)
        {
            BrickW = mStPositon.x + j * mTileSize - mTileSize / 2;
            if (mEndStage == true && GetTile(j, i) == TileType::Door) {
                static int mFlagFrame = 0;
                FlagX = BrickW, FlagY = BrickH;
                mSprite->DrawTile(mHdc, TileType::Empty, BrickW, BrickH, mTileSize);
            }
            else
                mSprite->DrawTile(mHdc, GetTile(j, i), BrickW, BrickH, mTileSize);
        }
    }

    if (mEndStage == true)
    {
        static int mFlagFrame = 0;
        mSprite->Animation(mHdc, true, SpriteType::FinalFlag, mFlagFrame, FlagX - 16, FlagY - 4, 100, 100);
    }
}

void GameMap::SetHDC(HDC hdc)
{
    mHdc = hdc;
}