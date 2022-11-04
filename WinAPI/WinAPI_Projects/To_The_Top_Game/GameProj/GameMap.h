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
	HDC mHdc; // �� ��� DC

	// �� ��� ���� ��, ǥ��, �浹 ������ ���
	std::vector<std::vector<TileType>> mTiles;

public:
	// �� ����, ��� ��꿡 ���
	std::vector<std::vector<short>> mGrid;

	POINT mStPositon; // ȭ�鿡�� ���� ���� ��ġ

	// �ʺ�, ����
	int mWidth = 0;
	int mHeight = 0;

	int mTileSize = 0; // �ȼ����� Ÿ�� ũ��

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

