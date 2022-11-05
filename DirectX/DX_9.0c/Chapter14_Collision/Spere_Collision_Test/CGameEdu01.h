#pragma once
#include "CD3DApp.h"
#include "CAxis.h"
#include "CCube.h"
#include "CGround.h"

// � �����̳� ������ ����� ������ ���� �ʿ��� ������ ��� ǥ���ؾ� �ڵ尡 ���������.
struct SPHERE_PROPERTY // ����� ���� �Ӽ� ����
{
	D3DXVECTOR3 vTans; // �߽����� ���� ����
	float fScaling; // ������(ũ��)
	float fRadius; // ������
};

class CGameEdu01 : public CD3DApp
{
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj; // ���� ��ȯ�� ���� ���
	D3DXVECTOR3 m_Eye, m_At, m_Up; // �� ��ȯ�� ���� ��ġ (ī�޶� ��ġ, ���� ����)

	// ��Ʈ
	LPD3DXFONT m_pFont;

	SPHERE_PROPERTY m_Sphere[2]; // �浹 üũ�� ���� �� 2��
	BOOL m_bIsCollision; // �浹���� �ƴ��� BOOL��
	LPD3DXMESH m_pMesh; // ���� ��� �Ž�

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

