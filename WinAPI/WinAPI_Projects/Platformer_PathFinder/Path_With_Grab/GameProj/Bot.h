#pragma once

#include "Character.h"
#include "PathFinderFast.h"

class Bot : public Character
{
	enum BotState
	{
		None = 0,
		MoveAhead,
	};

	BotState mCurrentBotState = BotState::None;

	POINTFLOAT mDestination;

	PathFinderFast mPathFinder;

	int mCurrentNodeId = -1;

	int mFramesOfJumping = 0;
	int mStuckFrames = 0;

	int mMaxJumpHeight = 5; // 블록 단위, 수직으로 점프했을때 5칸은 넘는다는 의미

	const int cMaxStuckFrames = 20;

	bool mReachedNodeX;
	bool mReachedNodeY;

	bool mGrabsLedges = false;
	bool mMustGrabLeftLedge;
	bool mMustGrabRightLedge;
	bool mCanGrabLedge = false;

	bool mLazyLedgeGrabs = false;

public:
	Bot();
	~Bot();
	void TappedOnTile(POINT mapPos);
	void InitBot(POINTFLOAT point, POINTFLOAT Scale, Map* stage);
	void SetJumpHeight(int value);
	void SetLazyLedgeGrabs(bool value);
	void SetCharacterWidth(int value);
	void SetCharacterHeight(int value);
	bool IsOnGroundAndFitsPos(POINT pos);
	void MoveTo(POINT destination);
	void MoveTo(POINTFLOAT destination);
	int GetJumpFrameCount(int deltaY);
	bool ReachedNodeOnXAxis(POINTFLOAT pathPosition, POINTFLOAT prevDest, POINTFLOAT currentDest);
	bool ReachedNodeOnYAxis(POINTFLOAT pathPosition, POINTFLOAT prevDest, POINTFLOAT currentDest);
	void GetContext(POINTFLOAT& prevDest, POINTFLOAT& currentDest, POINTFLOAT& nextDest, bool& destOnGround);
	void TestJumpValues();
	int GetJumpFramesForNode(int prevNodeId, bool grabLedges);
	bool CanGrabLedge(int nodeId);
	bool CanGrabLedgeOnLeft(int nodeId);
	bool CanGrabLedgeOnRight(int nodeId);
	void MovementUpdate();
	void BotUpdate();
};