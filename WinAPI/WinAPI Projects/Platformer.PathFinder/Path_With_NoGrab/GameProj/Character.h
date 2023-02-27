#pragma once

#include <Windows.h>
#include <vector>
#include "MovingObject.h"
#include "Constants.h"

enum KeyInput
{
    GoLeft = 0,
    GoRight,
    GoDown,
    Jump,
    Count
};

class Character : public MovingObject
{
public:
    enum class CharacterState
    {
        Stand,
        Run,
        Jump,
        GrabLedge,
    };

    CharacterState mCurrentState = CharacterState::Stand;

    int mFramesFromJumpStart = 0;

    std::vector<bool> mInputs;
    std::vector<bool> mPrevInputs;

    float mJumpSpeed;

    float mWalkSpeed;

    std::vector<POINT> mPath;

    int mWidth = 1;
    int mHeight = 3;

    POINT mLedgeTile;
    float mLedgeGrabOffset;

    int mCannotGoLeftFrames = 0;
    int mCannotGoRightFrames = 0;

    const int cLedgeGrabStopFrames = 5;
    int mLedgeGrabStopFrames = 0;

    Character();
    ~Character();

    void InitCharacter(POINTFLOAT point, POINTFLOAT Scale, Map *stage);
    void CharacterUpdate(std::vector<bool> &Inputs);
    void HandleJumping();
    void UpdatePrevInputs();
};

