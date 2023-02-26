#pragma once
#include "CD3DApp.h"
#include "CAxis.h"
#include "CCube.h"
#include "CGround.h"
#include "FmodSound.h"

#define EnemyCount 50 // �� �ִ� ����

typedef enum _Game_State // ���� ���� �����Ǿ��ִ� ������ �κе��� �������� ���� ������
{
	INIT, LOADING, READY, RUN, STOP, SUCCESS, FAILED, END
}GAME_STATE;

struct BULLET // �Ѿ� ��ü�� ���� �Ӽ�, �Ѿ� �ѹ߸��� �ٸ� ������ ��ġ�Ѵ�.
{
	int nLife;
	D3DXVECTOR3 vPos;
	D3DXMATRIX matTranslation; // �̵� ���
};

struct BULLET_PROPERTY // �Ѿ� ��¿� ���� �Ӽ�, ��� �Ѿ��� �����ϰ� �Ǵ� �Ӽ� ������ ��ġ�Ѵ�.
{
	float fBulletVelcoty; // �Ѿ� �ӷ�
	float fScale; // �Ѿ� ũ��
	D3DXMATRIX matScale; // ũ�� ���

	// �浹 ��ǥ
	D3DXVECTOR3 vMin, vMax; // �浹 �ּ���, �ִ���, ��¥�� �浹���� SRT ��ȯ�� ���Ͽ� �� ��ü�� �浹������ ��ȯ�� �����ϴ� Ư���� �̷��� �ھ� ���⸸ �ϸ� �ȴ�.
};

struct PLAYER // ���ΰ�
{
	int nLife; // �÷��̾� ü��
	D3DXVECTOR3 vPos; // ��ġ
	float fScale; // ũ��
	float fRotationY; // ȸ���� ���� ��

	float fVelocity; // ������ �̵� �ӵ�

	// �Ѿ˿� ���� �Ӽ�	
	DWORD dwBulletFireTime; // �Ѿ� �߻� ����
	DWORD dwOldBulletFireTime; // �Ѿ��� ���� �ð�

	// ���� �׸���� ���� Player�� ��� ������ ���α׷��� ���⿡ ���� �̷��� �־�ΰ� ����ϴ� ��찡 �ִ�.
	D3DXMATRIX matWorld;
	D3DXMATRIX matScale;
	D3DXMATRIX matRotationY;
	D3DXMATRIX matTranslation;

	// ���ΰ� �浹 �ڽ�
	D3DXVECTOR3 vMin, vMax;
};

struct ENEMY // �� ��ü������ ���� ������ ��ġ�Ѵ�.
{
	int nLife; // ����
	DWORD dwAppearTime; // ����ð�, ������������ �����Ѵ�.
	D3DXVECTOR3 vPos; // ��ġ

	DWORD dwOldFireTime; // �Ѿ� �߻� ���� �ð�
	D3DXMATRIX matTranslation; // �� �̵� ���
};

struct ENEMY_PROPERTY // ������ �����ϰ� �Ǵ� �Ӽ�, �������� ġ�� '����' �̶�� �� �� �ִ�. �� ������ �پ��ϰ� �����ν� �پ��� ������ ������ �����ȴ�.
{
	float fMoveVelcoty; // �̵� �ӵ�
	float fScale; // �� ũ��

	DWORD dwFireTime; // �̻��� �߻� �ð� ����
	D3DXMATRIX matScale; // ũ�� ��ȯ ���

	// �� �浹 �ڽ�
	D3DXVECTOR3 vMin, vMax;
};

class CGameEdu01 : public CD3DApp
{
	D3DXMATRIX m_matView; // ī�޶� ���
	D3DXMATRIX m_matProj; // ���� ��ȯ�� ���� ���
	D3DXVECTOR3 m_Eye, m_At, m_Up; // �� ��ȯ�� ���� ��ġ (ī�޶� ��ġ, ���� ����)

	// ���� ���
	CGround m_Ground;

	// ���ΰ� 
	PLAYER m_sPlayer; // �÷��̾� ����
	BULLET_PROPERTY m_sPlayerBulletProperty; // �Ѿ� Ư��
	BULLET m_sPlayerBullet[10]; // �ѹ��� �������� �� �� �ִ�.

	// ���� �޽� ����
	LPD3DXMESH m_pTeapotMesh; // ���ΰ� (������ �޽�)	
	LPD3DXMESH m_pPlayerBulletMesh; // ���ΰ� �Ѿ� (����� �޽�)
	LPD3DXMESH m_pEnemyBoxMesh; // ���� �Ǵ� �ڽ� �޽�
	LPD3DXMESH m_pEnemyBulletMesh; // ���� �Ѿ� �޽�

	// �� ĳ���� 
	ENEMY_PROPERTY m_EnemyProperty; // ���� ������ ���⼱ �����ϰ� �Ѱ��� ����
	ENEMY m_Enemy[EnemyCount]; //�� ĳ���� EnemyCount ��ŭ
	BULLET_PROPERTY m_EnemyBulletProperty; // �Ѿ� ������ �ϴ� �Ѱ��� ����
	BULLET m_EnemyBullet[100]; // �� �Ѿ� ������ ���� ���緮
	int m_nEnemyIndex; // �� ���� �����ϴ� ������ �ε����̴�. �ᱹ ������ �迭�� �̷�����ֱ⿡ �����ϱ� ���� �ε��� ������ �ʿ��ϴ�.

	// �������� ����
	DWORD m_dwGameStartTime; // �ʱ�ȭ�� ���� ������ ���� ���� �ð�
	DWORD m_dwGameElapsedTime; // ���� �ð� �Ŀ� ������� �귯�� �ð�

	// ���� �ܰ� ���ҿ� ���Ǵ� ������
	unsigned short m_nGameState; // ������ ��� �������� �����ϴ� ���� ex) �ʱ�ȭ, ����, ���ӿ���...
	int m_nStage; // ��� ���������ΰ�
	int m_nLoadingCount; // �ε��� �󸶳� �Ǿ��°�
	int m_nGrade; // ����
	int m_nTotalGrade; // �� ����
	int m_nEnemyCount; 

	// ���� ��� ��Ʈ
	LPD3DXFONT m_pFont, m_pFont2, m_pFont3;

	// ���� 
	CFmodSound m_FMODSound;

	// ���� �ð��� ���� ����
	DWORD m_dwElapsedTime; // ������ �ð�
	unsigned int m_nFPS, m_nFPSCount; // 1�ʰ� �Ǿ��� ��� ��µǴ� FPS, 1�� �Ǳ� ������ ī��Ʈ�Ǵ� FPS ��

private:
	virtual void OnInit();
	virtual void OnRender();
	virtual void OnUpdate();
	virtual void OnRelease();

public:
	CGameEdu01(void);
	~CGameEdu01(void);
};