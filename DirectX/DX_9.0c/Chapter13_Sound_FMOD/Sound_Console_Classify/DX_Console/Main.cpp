#include <iostream>
#include <conio.h>
#include <d3d9.h>
#include <d3dx9math.h>
#include <fmod.h>
#include "FmodSound.h"
using namespace std;

#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib,"d3dx9.lib")
#pragma comment (lib,"DxErr.lib")
#pragma comment (lib,"d3dxof.lib")
#pragma comment (lib,"winmm.lib")
#pragma comment (lib,"fmod_vc.lib")
#pragma comment (lib,"fmodL_vc.lib")

int main()
{
	string strBGFileName[] = { "Sound_Dir\\story.mp3", "Sound_Dir\\wave.mp3" };
	string strEffectFileName[] = { "Sound_Dir\\play_fire.wav", "Sound_Dir\\safe.wav" };
	int nKey;

	CFmodSound FmodSound; // �Ҹ� ��ü ����
	FmodSound.CreateBGSound(2, strBGFileName); // ����� �Ҹ��� �־��ش�.
	FmodSound.CreateEffectSound(2, strEffectFileName); // ȿ���� �Ҹ��� �־��ش�.

	printf("���� �ý��� �غ� �Ϸ�\n");
	printf("Ű 1: ���1 ���	Ű 2: ȿ���� ���	Ű 3: ����� ����	Ű q: ����\n");

	while (true)
	{
		if (_kbhit())
		{
			nKey = _getch();
			if (nKey == '1')
			{
				FmodSound.PlaySoundBG(0);
			}
			if (nKey == '2')
			{
				FmodSound.PlaySoundEffect(1);
			}
			if (nKey == '3')
			{
				FmodSound.StopSoundBG(0);
			}
			if (nKey == 'q')
			{
				break;
			}
		}
		FmodSound.Update();
	}
	FmodSound.ReleaseSound();
	return 0;
}