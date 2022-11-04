#pragma once

#define sign(x) (((x) >= 0) ? 1 : -1)
#define PI 3.14159265f
#define getRad(x) ((x) * (PI / 180))

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

extern float cPlayerWidth;
extern float cPlayerHeight;
extern float cPlayerWalkSpeed;
extern float cPlayerJumpSpeed;

extern float cKnockBackSpeedY;
extern float cKnockBackSpeedX;

extern int cItemFadeTime;

extern int cHeroFindTime; // 적이 플레이어 다시 찾기까지 대기 시간

extern int cBaseWidth;
extern int cBaseHeight;

extern int cAnimeFrame;