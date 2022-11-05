#include <iostream>
#include <conio.h>
#include <d3d9.h>
#include <d3dx9math.h>
#include <fmod.h>
using namespace std;

#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib,"d3dx9.lib")
#pragma comment (lib,"DxErr.lib")
#pragma comment (lib,"d3dxof.lib")
#pragma comment (lib,"winmm.lib")
#pragma comment (lib,"fmod_vc.lib")
#pragma comment (lib,"fmodL_vc.lib")

/*
�ڵ尡 �ణ �����̶� �򰥸� �� ������ https://m.blog.naver.com/PostView.nhn?blogId=zoomen1004&logNo=110175532047&proxyReferer=https:%2F%2Fwww.google.com%2F
�� Ȩ������ ����
*/

FMOD_SYSTEM* g_System; // FMOD ���� �ý����� �̿��ϱ� ���� ������ ��ü
FMOD_SOUND* g_Sound[2]; // ������ �迭���� ����!
FMOD_CHANNEL* g_Channel; // ����� ���

void FMOD_Init() // FMOD �ý��� �ʱ�ȭ �Լ�
{
	// FMOD �ʱ�ȭ �� ���� ���� �ε�
	FMOD_System_Create(&g_System);
	FMOD_System_Init(g_System, 32, FMOD_INIT_NORMAL, NULL); // FMOD_INIT_NORMAL�� �Ϲ� ȿ������ ���̴� ���
	// ���� �Ҹ������� �� g_Sound�� 1:1 ������Ŵ, �ݺ��� FMOD_LOOP_NORMAL, �⺻ ȿ������ FMOD_DEFAULT �̿� 
	FMOD_System_CreateSound(g_System, "Sound_Dir\\wave.mp3", FMOD_LOOP_NORMAL, 0, &g_Sound[0]);
	FMOD_System_CreateSound(g_System, "Sound_Dir\\safe.wav", FMOD_DEFAULT, 0, &g_Sound[1]);

	// ����� ���
	FMOD_System_PlaySound(g_System, g_Sound[0], 0, 0, &g_Channel);
	FMOD_Channel_SetVolume(g_Channel, 0.3f);
}

void FMOD_Release() // FMOD �޸� ���� �Լ�
{
	// Note: FMOD ����
	int i;

	for (i = 0; i < 2; i++)
		FMOD_Sound_Release(g_Sound[i]); // FMOD sound ��ü ����

	FMOD_System_Close(g_System); // FMOD system ��ü clsoe
	FMOD_System_Release(g_System); // FMOD system ��ü ����
}

int main()
{
	int nKey;
	FMOD_CHANNEL* pChannel;
	float volume = 0.5f;

	FMOD_Init();

	printf("s�� ���� ȿ���� ���\n");

	while (1)
	{
		if (_kbhit())
		{
			nKey = _getch();

			if (nKey == 's') // s ������ ȿ���� ���
			{
				FMOD_System_PlaySound(g_System, g_Sound[1], 0, 0, &pChannel);
				FMOD_Channel_SetVolume(pChannel, volume);
			}
		}

		FMOD_System_Update(g_System);
	}

	FMOD_Release();
	return 0;
}