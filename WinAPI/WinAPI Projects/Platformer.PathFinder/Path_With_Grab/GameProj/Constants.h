#pragma once

#define sign(x) (((x) >= 0) ? 1 : -1)

extern const float cGravity;
extern const float cMaxFallingSpeed;

extern float cJumpSpeed[6];
extern float cWalkSpeed[6];

extern float cHalfSizeY[9];
extern float cHalfSizeX[9];

extern const int cTileSize;

extern const int cJumpFramesThreshold;

extern const float cBotMaxPositionError;

extern const float cMinJumpSpeed;

extern const float cGrabLedgeStartY;

extern const float cGrabLedgeEndY;