#pragma once

#include <Windows.h>
#include <vector>
#include <algorithm>
#include "AABB.h"
#include "GameSprite.h"

enum TileType
{
    Empty,
    Block,
    OneWay,
	Door,
	Border,
};

class GameMap
{
protected:
	HDC mHdc; // 맵 출력 DC

	// 맵 출력 격자 값, 표시, 충돌 판정에 사용
	std::vector<std::vector<TileType>> mTiles;

public:
	// 맵 격자, 경로 계산에 사용
	std::vector<std::vector<short>> mGrid;

	POINT mStPositon; // 화면에서 맵의 실제 위치

	// 너비, 높이
	int mWidth = 0;
	int mHeight = 0;

	int mTileSize = 0; // 픽셀에서 타일 크기

	bool mEndStage = false;

	GameMap();
	~GameMap();
	GameSprite* mSprite;
	TileType GetTile(int x, int y);
	bool IsOneWayPlatform(int x, int y);
	bool IsGround(int x, int y);
	bool IsGround(POINT pos);
	bool IsObstacle(int x, int y);
	bool IsNotEmpty(int x, int y);
	POINT GetMapTileAtPoint(POINTFLOAT point);
	void GetMapTileAtPoint(POINTFLOAT point, int& tileIndexX, int& tileIndexY);
	POINTFLOAT GetMapTilePosition(int tileIndexX, int tileIndexY);
	POINTFLOAT GetMapTilePosition(POINT tileCoords);
	bool CollidesWithMapTile(AABB aabb, int tileIndexX, int tileIndexY);
	bool AnySolidBlockInRectangle(POINTFLOAT start, POINTFLOAT end);
	bool AnySolidBlockInStripe(int x, int y0, int y1);
	bool AnySolidBlockInRectangle(POINT start, POINT end);
	void SetTile(int x, int y, TileType type);
	void InitMap(std::vector<std::vector<short>>* Grid, GameSprite *Sprite);
	void DrawMap();
	void SetHDC(HDC hdc);
};

