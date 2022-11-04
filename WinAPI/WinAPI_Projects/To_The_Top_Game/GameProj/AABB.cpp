#include "AABB.h"

AABB::AABB()
{
	center = { 0,0 };
	halfSize = { 0,0 };
}

AABB::AABB(POINTFLOAT Center, POINTFLOAT HalfSize)
{
	center = Center;
	halfSize = HalfSize;
}

AABB::~AABB()
{

}

bool AABB::Overlaps(AABB other) // 충돌 검사
{
	// 두 사각형 중심 간의 거리보다 두 사각형의 반지름을 더한 것이 더 크면
	// 충돌을 하지 않는 것이고, 그 반대면 충돌이 된 것입니다.
	if (abs(center.x - other.center.x) > halfSize.x + other.halfSize.x) 
		return false;
	if (abs(center.y - other.center.y) > halfSize.y + other.halfSize.y) 
		return false;
	return true;
}

bool AABB::Overlaps(POINTFLOAT otherCenter, POINTFLOAT otherHalfSize)
{
	if (abs(center.x - otherCenter.x) > halfSize.x + otherHalfSize.x) 
		return false;
	if (abs(center.y - otherCenter.y) > halfSize.y + otherHalfSize.y) 
		return false;
	return true;
}

bool AABB::Overlaps(AABB other, float &overlapWidth, float &overlapHeight)
{
	overlapWidth = overlapHeight = 0;
	if (abs(center.x - other.center.x) > halfSize.x + other.halfSize.x) 
		return false;
	if (abs(center.y - other.center.y) > halfSize.y + other.halfSize.y) 
		return false;
	overlapWidth = (other.halfSize.x + halfSize.x) - abs(center.x - other.center.x);
	overlapHeight = (other.halfSize.y + halfSize.y) - abs(center.y - other.center.y);
	return true;
}

bool AABB::Overlaps(AABB a, AABB b)
{
	if (abs(a.center.x - b.center.x) > a.halfSize.x + b.halfSize.x)
		return false;
	if (abs(a.center.y - b.center.y) > a.halfSize.y + b.halfSize.y) 
		return false;
	return true;
}

bool AABB::Overlaps(AABB a, POINTFLOAT otherCenter, POINTFLOAT otherHalfSize)
{
	if (abs(a.center.x - otherCenter.x) > a.halfSize.x + otherHalfSize.x) 
		return false;
	if (abs(a.center.y - otherCenter.y) > a.halfSize.y + otherHalfSize.y) 
		return false;
	return true;
}

bool AABB::Overlaps(AABB a, AABB b, float &overlapWidth, float &overlapHeight)
{
	overlapWidth = overlapHeight = 0;
	if (abs(a.center.x - b.center.x) > a.halfSize.x + b.halfSize.x) 
		return false;
	if (abs(a.center.y - b.center.y) > a.halfSize.y + b.halfSize.y) 
		return false;
	overlapWidth = (b.halfSize.x + a.halfSize.x) - abs(a.center.x - b.center.x);
	overlapHeight = (b.halfSize.y + a.halfSize.y) - abs(a.center.y - b.center.y);
	return true;
}

bool AABB::PointInside(AABB a, POINTFLOAT p)
{
	if (abs(a.center.x - p.x) > a.halfSize.x) 
		return false;
	if (abs(a.center.y - p.y) > a.halfSize.y) 
		return false;
	return true;
}