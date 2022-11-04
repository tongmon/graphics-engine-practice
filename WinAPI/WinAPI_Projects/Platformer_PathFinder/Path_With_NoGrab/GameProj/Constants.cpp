#include "Constants.h"

const float cGravity = -1030.0f;
const float cMaxFallingSpeed = -1800.0f; // -900.0f

// Ÿ�� ���ڿ� ������� ��������Ѵ�.
// �ε����� ��� ������ cHalfSizes[2]�� 40 / 16 = 3(�ݿø�) ����
// �浹 �˻�ÿ� ��� 3ĭ�� �˻��ϰ� �ȴ�.
// ���� �̴� Ÿ�� �����ϳ��� 16 ũ�� �� ��츦 ���ϴ� ���̰�
// Ÿ�� ���ڰ� �޶����� �̵� �޶����⿡ �����ϰ� ����� �Ѵ�.
float cJumpSpeed[] = { 210.0f, 280.0f, 350.0f, 385.0f, 410.0f, 480.0f };
float cWalkSpeed[] = { 80.0f, 100.0f, 140.0f, 150.0f, 160.0f, 210.0f };
float cHalfSizeX[] = { 6.0f, 9.0f, 13.0f, 25.0f, 30.0f, 35.0f, 40.0f, 45.0f, 6.0f };
float cHalfSizeY[] = { 14.0f, 21.0f, 32.0f, 50.0f, 60.0f, 70.0f, 80.0f, 90.0f, 8.0f };

const int cTileSize = 40;

const int cJumpFramesThreshold = 4;

const float cBotMaxPositionError = 1.0f;

const float cMinJumpSpeed = 200.0f;

const float cGrabLedgeStartY = 0.0f;

const float cGrabLedgeEndY = 6.0f;