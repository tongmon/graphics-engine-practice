#pragma once
#include <string>
#include <fmod.h>
using namespace std;

class CFmodSound
{
private:
	FMOD_SYSTEM* m_pSystem; // FMOD �ý���
	FMOD_SOUND** m_ppBGSound; // ���������ͷ� �� ������ �Ҹ��� ������ �� �� ������ ������ �迭�� ����� �����Ϸ��� ����ߴ�.
	FMOD_SOUND** m_ppEFFSound; // ��׶��� ����� ���� ������ ȿ���� ���嵵 ���������� ����
	FMOD_CHANNEL** m_ppBGChannel; // ä�ε� ���� �������� ���� ������
	int m_nEFSoundCount;
	int m_nBGSoundCount;
public:
	// string* �� ������ �迭�̶�� �����ϸ� ����. �� ���ϸ� ������ ��� ���ϰ� �����ϴ� �뵵
	void CreateEffectSound(int nCount, string* SoundFileName);
	void CreateBGSound(int nCount, string* SoundFileName);
	// �ε����� ������ �迭���� �Ҹ� ������ �۵�
	void PlaySoundEffect(int nIndex);
	void PlaySoundBG(int nIndex);
	void StopSoundBG(int nIndex); // ������� ����� �� ���� �ֱ⿡ ����
	void ReleaseSound();
	void Update();

public:
	CFmodSound(void);
	~CFmodSound(void);
};
