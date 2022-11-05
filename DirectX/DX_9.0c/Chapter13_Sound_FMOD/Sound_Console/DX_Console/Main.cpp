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

FMOD_SYSTEM* g_System;  // Note: FMOD system ������ ���� ����, �ٷ�ó�� ��� �����͸� ���ؼ� �Ҹ� ����Ѵ�
FMOD_SOUND* g_Sound;   // Note: FMOD Sound ������ ���� ����, �Ҹ� ���ϰ� 1:1 �����Ѵ�.

void FMOD_Init() // FMOD �ý��� �ʱ�ȭ �Լ�
{
	// FMOD �ʱ�ȭ �� ���� ���� �ε�
	FMOD_System_Create(&g_System);
	FMOD_System_Init(g_System, 32, FMOD_INIT_NORMAL, NULL);
	FMOD_System_CreateSound(g_System, "Sound_Dir\\wave.mp3", FMOD_DEFAULT, 0, &g_Sound); // FMOD_DEFAULT�� �ѹ��� ����ϰ� ������. �ݺ��� �ȵ�
}

void FMOD_Release() // FMOD �޸� ���� �Լ�
{
	// Note: FMOD ����
	FMOD_Sound_Release(g_Sound);
	FMOD_System_Close(g_System);
	FMOD_System_Release(g_System);
}

int main()
{
	int nKey;
	FMOD_CHANNEL* channel = NULL; // ä���� �� �Ҹ��� ������ ��������, ���� �Ҹ� �ߴ��� �� ���� ����
	float volume = 0.3f; // ���� ������ 0.0f ~ 1.0f ����

	FMOD_Init(); // �ʱ�ȭ �� SYSTEM ����Ҵ�

	printf("----------------------\n");
	printf("�� MUSIC PLAYER �� \n");
	printf("p : ���� \n");
	printf("s : ���� \n");
	printf("u : ���� ���� \n");
	printf("d : ���� ���� \n");
	printf("q : ���� \n");
	printf("----------------------\n");

	while (1)
	{
		if (_kbhit())  // Ű �Է� ����   
		{
			nKey = _getch();   // �Էµ� Ű ���� Ű���� ���۷κ��� ������
			if (nKey == 'q')  // q Ű�� ������ ���α׷� ����, Ű �Է��� ���� �ֿ켱 ������ �ȴ�. 
			{
				break;
			}

			switch (nKey)
			{
			case 'p':
				FMOD_System_PlaySound(g_System, g_Sound, 0, 0, &channel);
				FMOD_Channel_SetVolume(channel, volume);
				break;
			case 's':
				FMOD_Channel_Stop(channel);
				break;
			case 'u': // ���� ��
				volume += 0.1f;
				if (volume > 1.0f)
				{
					volume = 1.0f;
				}
				FMOD_Channel_SetVolume(channel, volume);
				break;
			case 'd': // ���� �ٿ�
				volume -= 0.1f;
				if (volume < 0.0f)
				{
					volume = 0.0f;
				}
				FMOD_Channel_SetVolume(channel, volume);
				break;
			}
		}
		FMOD_System_Update(g_System); // FMOD ������Ʈ, ������Ʈ�� ���ִ� ������ ��Ʈ���� ������� �κ� �κ� �߶� �޸𸮿� �÷����� ����.
		// Ʋ����� ���� ��ü�� �ѹ��� �޸𸮿� ž������ �ʴ´�. (��ȿ����)
	}

	FMOD_Release(); // ����
	return 0;
}