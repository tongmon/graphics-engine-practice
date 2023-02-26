#pragma once
#include "CD3DApp.h"
#include "CAxis.h"
#include "CCube.h"
#include "CGround.h"

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
};

class CGameEdu01 : public CD3DApp
{
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj; // ���� ��ȯ�� ���� ���
	D3DXVECTOR3 m_Eye, m_At, m_Up; // �� ��ȯ�� ���� ��ġ (ī�޶� ��ġ, ���� ����)

	// ���� ���
	CGround m_Ground;

	// ���ΰ� 
	PLAYER m_sPlayer; // �÷��̾� ����
	BULLET_PROPERTY m_sPlayerBulletProperty;
	BULLET m_sPlayerBullet[10]; // �ѹ��� �������� �� �� �ִ�.

	LPD3DXMESH m_pTeapotMesh; // ���ΰ� (������ �޽�)	
	LPD3DXMESH m_pPlayerBulletMesh; // ���ΰ� �Ѿ� (����� �޽�)

	// ���� ��� ��Ʈ
	LPD3DXFONT m_pFont;

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

