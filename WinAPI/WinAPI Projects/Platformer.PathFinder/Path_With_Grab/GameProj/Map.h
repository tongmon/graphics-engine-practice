#pragma once

#include <Windows.h>
#include <vector>
#include <algorithm>
#include "Bot.h"

enum TileType
{
    Empty,
    Block,
    OneWay
};

class Map
{
public:
	HDC mHdc;

	POINT position; // ȭ�鿡�� ���� ���� ��ġ

	std::vector<std::vector<short>> mGrid;

	std::vector<std::vector<TileType>> tiles;

	int mTileSize = 0; // �ȼ����� Ÿ�� ũ��, 16

	// �ʺ�, ����
	int mWidth = 0;
	int mHeight = 0;

	int lastMouseTileX = -1;
	int lastMouseTileY = -1;

	Map();
	~Map();
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
	void InitMap();
	void DrawMap();
	void GetHDC(HDC hdc);
};
