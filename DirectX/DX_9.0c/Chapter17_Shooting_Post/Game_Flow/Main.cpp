#include <windows.h>
#include <iostream>
#include <conio.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <d3d9.h>
#include <d3dx9math.h>
#include "Screen.h" // �ܼ� ȭ�� ����� ���� �Լ����� ��ġ
using namespace std;

#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib,"d3dx9.lib")
#pragma comment (lib,"DxErr.lib")
#pragma comment (lib,"d3dxof.lib")
#pragma comment (lib,"winmm.lib")

typedef enum _GAME_STATE { INIT, READY, RUNNING, STOP, SUCCESS, FAILED, RESULT } GAME_STATE;

unsigned short g_GameState = INIT; // ���� �帧�� ��� �������� �˱� ���� ô�� ����, ó���� ������ �ʱ�ȭ�� INIT �κ��̴�.
int g_nGoal, g_nStage, g_GameStartTime;

char g_strMessage[100]; // ���� ���¸� �����ϴ� ���ڿ�
clock_t g_OldTime; // ���� ���� ���̸� ���� ���� �ð� ����

void Init()
{
	g_OldTime = clock();
}

void Update()
{
	clock_t CurTime = clock();

	switch (g_GameState)
	{
	case INIT: // �ʱ�ȭ �κ�
		if (g_nStage == 0)
		{
			wsprintf(g_strMessage, "%s", "���� �� ���� �ʱ�ȭ");
			if (CurTime - g_OldTime > 3000) // 3�� ���Ŀ� �ʱ�ȭ�� ��� �����ٰ� ģ��.
			{
				g_OldTime = CurTime;
				g_nStage = 1;
			}
		}
		else 
		{
			wsprintf(g_strMessage, "[INIT] ���� %d �������� �ʱ�ȭ", g_nStage);
			if (CurTime - g_OldTime > 3000)
			{
				g_OldTime = CurTime; // ���� �ܰ�� �Ѿ�� OldTime ����, �� �ܰ躰 3�ʸ� �����־�� �ϱ� ����
				g_GameState = READY; // �ʱ�ȭ�� �������� �غ� �ܰ�� �Ѿ��.
			}
		}
		break;
	case READY: // ���� �غ� �κ�
		wsprintf(g_strMessage, "[READY] %d ��������", g_nStage);
		if (CurTime - g_OldTime > 3000) // �� �ܰ躰 3�ʰ� ���� �Ŀ� ���� �ܰ�� ����
		{
			g_OldTime = CurTime;
			g_GameState = RUNNING; // �غ� �������� RUNNING���� �Ѿ��.
			g_GameStartTime = CurTime; // ���� ������ ������ ���� �ð� ���
		}
		break;
	case RUNNING: // ���� ���� �� ����
		if (CurTime - g_GameStartTime > 10000) // Note: ���� �ð�
		{
			g_GameState = STOP; // 10�ʰ� ������ STOP �ܰ�� �Ѿ��.
		}
		else 
		{
			wsprintf(g_strMessage, "[RUNNING] ���� �ð� : 10��  ����ð� : %d", (CurTime - g_GameStartTime) / 1000);
		}
		break;
	case STOP: // ���� ���� ����
		if (g_nGoal == 1) // ���� ���� ���·� STOP ���¿� �Դٸ� ������ ���¸� SUCCESS�� �ѱ��.
		{
			g_GameState = SUCCESS;
		}
		else // ���� ���־��ٸ� FAILED ���·� �ѱ��.
		{
			g_GameState = FAILED;
		}
		break;
	case SUCCESS: // ���� ����
		wsprintf(g_strMessage, "%s", "�̼� ����");
		if (CurTime - g_OldTime > 3000)
		{
			g_OldTime = CurTime;
			g_GameState = INIT;
			++g_nStage;
		}
		break;
	case FAILED: // ���� ����
		wsprintf(g_strMessage, "%s", "�̼� ����! ��� �Ͻðڽ��ϱ�? <y/n> ");
		break;

	case RESULT: // ��� ����
		wsprintf(g_strMessage, "%s", "���� ��� ȭ��");
		if (CurTime - g_OldTime > 3000)
		{
			g_OldTime = CurTime;
		}
		break;
	}
}

void Render() // ���� �κ�, �����δ� ������ �� �ܰ躰�� ���������� ������ ���⼱ ���� �ϳ��� ����ϱ⿡ ������ �ʾҴ�.
{
	clock_t CurTime = clock();

	ScreenClear();

	// Note: ������ ����	
	ScreenPrint(20, 10, g_strMessage);

	// Note: ������ �� 
	ScreenFlipping();
}

void Release()
{
}

int main(void)
{
	int nKey;

	ScreenInit();
	Init(); // �ʱ�ȭ

	while (true)
	{
		if (g_GameState == RESULT) // ��� ȭ���̸� ��
		{
			return 1;
		}

		if (_kbhit())
		{
			nKey = _getch();

			if (nKey == 'q') // q�� ������
				break;

			switch (nKey)
			{
			case 's':
				g_nGoal = 1;
				break;
			case 'f':
				g_nGoal = 0;
				break;

			// y, Y �̸� ���� 
			case 'y':
			case 'Y':
				if (g_GameState == FAILED) // �����ϸ�
				{
					g_GameState = INIT; // �����Ͽ� �ٽ� ����
					g_OldTime = clock();
				}
				break;

			// n, N �̸� ���� 
			case 'n':
			case 'N':
				if (g_GameState == FAILED) // ���� ���¿��� �ź��ϸ� ��� ���·� �̵�
				{
					g_GameState = RESULT;
					g_OldTime = clock();
				}
				break;
			}
		}

		Update();    // ������ ����
		Render();    // ȭ�� ���		
	}

	Release();   // ����
	ScreenRelease();
	return 0;
}
