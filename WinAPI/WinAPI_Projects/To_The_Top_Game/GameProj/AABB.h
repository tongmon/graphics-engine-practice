#pragma once

#include <Windows.h>
#include <list>
#include <algorithm>

class AABB
{
public:
	POINTFLOAT center; // �浹 �ڽ� �߽�
	POINTFLOAT halfSize; // ��, ���� ���� ũ��

	AABB();
	AABB(POINTFLOAT Center, POINTFLOAT HalfSize);
	~AABB();
	bool Overlaps(AABB other);
	bool Overlaps(POINTFLOAT otherCenter, POINTFLOAT otherHalfSize);
	bool Overlaps(AABB other, float& overlapWidth, float& overlapHeight);
	bool Overlaps(AABB a, AABB b);
	bool Overlaps(AABB a, POINTFLOAT otherCenter, POINTFLOAT otherHalfSize);
	bool Overlaps(AABB a, AABB b, float& overlapWidth, float& overlapHeight);
	bool PointInside(AABB a, POINTFLOAT p);
};

