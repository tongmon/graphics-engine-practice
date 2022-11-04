#include "Map.h"
#include "Constants.h"

Map::Map()
{

}

Map::~Map()
{

}

void Map::InitMap()
{
    // 맵 시작 좌표 설정
    position = { 20, 30 };

    mTileSize = cTileSize;

    /*
    std::vector<std::vector<short>> bufGrid =
    {
        { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
    };
    */

    /*
    std::vector<std::vector<short>> bufGrid =
    {
        { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
    };
    */

    std::vector<std::vector<short>> bufGrid =
    {
        { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
                { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
                { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
                { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
                { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
                { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
                { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
                { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
                { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
                { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
                { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
                { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
                { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
                { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
                { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
                { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
                { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
    };

    std::vector<TileType> tileLine;
    for (int i = 0; i < bufGrid.size(); i++)
    {
        tiles.push_back(tileLine);
        for (int j = 0; j < bufGrid[i].size(); j++)
        {
            tiles[i].push_back((TileType)bufGrid[i][j]);
        }
    }

    mWidth = tiles[0].size();
    mHeight = tiles.size();

    for (size_t i = 0, j = mHeight - 1; i < mHeight / 2; i++, j--)
        std::swap(tiles[i], tiles[j]);

    std::vector<short> gridLine;
    for (int y = 0; y < mHeight; ++y)
    {
        mGrid.push_back(gridLine);
        for (int x = 0; x < mWidth; ++x)
        {
            if (tiles[y][x] == TileType::Block)
                mGrid[y].push_back(0);
            else
                mGrid[y].push_back(1);
        }
    }
}

TileType Map::GetTile(int x, int y)
{
    if (x < 0 || x >= mWidth
        || y < 0 || y >= mHeight)
        return TileType::Block;

    return tiles[y][x];
}

bool Map::IsOneWayPlatform(int x, int y)
{
    if (x < 0 || x >= mWidth
        || y < 0 || y >= mHeight)
        return false;

    return (tiles[y][x] == TileType::OneWay);
}

bool Map::IsGround(int x, int y)
{
    if (x < 0 || x >= mWidth
        || y < 0 || y >= mHeight)
        return false;

    return (tiles[y][x] == TileType::OneWay || tiles[y][x] == TileType::Block);
}

bool Map::IsGround(POINT pos)
{
    return IsGround(pos.x, pos.y);
}

bool Map::IsObstacle(int x, int y)
{
    if (x < 0 || x >= mWidth
        || y < 0 || y >= mHeight)
        return true;

    return (tiles[y][x] == TileType::Block);
}

bool Map::IsNotEmpty(int x, int y)
{
    if (x < 0 || x >= mWidth
        || y < 0 || y >= mHeight)
        return true;

    return (tiles[y][x] != TileType::Empty);
}

void Map::GetMapTileAtPoint(POINTFLOAT point, int &tileIndexX, int &tileIndexY)
{
	tileIndexY = (int)((point.y - position.y + mTileSize / 2.0f) / (float)(mTileSize));
	tileIndexX = (int)((point.x - position.x + mTileSize / 2.0f) / (float)(mTileSize));
}

POINT Map::GetMapTileAtPoint(POINTFLOAT point)
{
	return { (int)((point.x - position.x + mTileSize / 2.0f) / (float)(mTileSize)),
		(int)((point.y - position.y + mTileSize / 2.0f) / (float)(mTileSize)) };
}

POINTFLOAT Map::GetMapTilePosition(int tileIndexX, int tileIndexY)
{
	return { (float)(tileIndexX * mTileSize) + position.x, (float)(tileIndexY * mTileSize) + position.y };
}

POINTFLOAT Map::GetMapTilePosition(POINT tileCoords)
{
	return { (float)(tileCoords.x * mTileSize) + position.x, (float)(tileCoords.y * mTileSize) + position.y };
}

bool Map::CollidesWithMapTile(AABB aabb, int tileIndexX, int tileIndexY)
{
    POINTFLOAT tilePos = GetMapTilePosition(tileIndexX, tileIndexY);

    return aabb.Overlaps(tilePos, { (float)(mTileSize) / 2.0f, (float)(mTileSize) / 2.0f });
}

bool Map::AnySolidBlockInRectangle(POINTFLOAT start, POINTFLOAT end)
{
    POINT Spoint = GetMapTileAtPoint(start);
    POINT Epoint = GetMapTileAtPoint(end);
    return AnySolidBlockInRectangle(Spoint, Epoint);
}

// 줄 하나로 y0 ~ y1 사이에 장애물이 있는지 검사
bool Map::AnySolidBlockInStripe(int x, int y0, int y1)
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
        if (GetTile(x, y) == TileType::Block)
            return true;
    }

    return false;
}

// start ~ end 사이에 장애물이 있는지 검사
bool Map::AnySolidBlockInRectangle(POINT start, POINT end)
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
            if (GetTile(x, y) == TileType::Block)
                return true;
        }
    }

    return false;
}

void Map::SetTile(int x, int y, TileType type)
{
    if (x < 1 || x >= mWidth - 1 || y < 1 || y >= mHeight - 1)
        return;

    tiles[y][x] = type;

    if (type == TileType::Block)
        mGrid[y][x] = 0;
    else
        mGrid[y][x] = 1;
}

void Map::DrawMap()
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

    int BrickW, BrickH;
    for (int i = 0; i < mHeight; i++)
    {
        BrickH = position.y + mTileSize * i - mTileSize / 2;
        for (int j = 0; j < mWidth; j++)
        {
            BrickW = position.x + j * mTileSize - mTileSize / 2;

            OldPen = (HPEN)SelectObject(mHdc, NewPen[0]);
            OldBrush = (HBRUSH)SelectObject(mHdc, NewBrush[0]);
            if (GetTile(j,i) == TileType::Block) {
                OldPen = (HPEN)SelectObject(mHdc, NewPen[1]);
                OldBrush = (HBRUSH)SelectObject(mHdc, NewBrush[1]);
            }
            else if (GetTile(j, i) == TileType::OneWay) {
                OldPen = (HPEN)SelectObject(mHdc, NewPen[2]);
                OldBrush = (HBRUSH)SelectObject(mHdc, NewBrush[2]);
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

void Map::GetHDC(HDC hdc)
{
    mHdc = hdc;
}