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


// � �����̳� ������ ����� ������ ���� �ʿ��� ������ ��� ǥ���ؾ� �ڵ尡 ���������.
struct BOX_PROPERTY // ť���� �Ӽ�
{
	D3DXVECTOR3 vTans; // �߽����� ���� ����
	float fScaling; // ������(ũ��)
	D3DXVECTOR3 vMin, vMax; // ť�꿡�� �ּ����� �ִ���, ���� �缱 ������ �� ���̶�� �����ϸ� ���ϴ�.
};

class CGameEdu01 : public CD3DApp
{
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj; // ���� ��ȯ�� ���� ���
	D3DXVECTOR3 m_Eye, m_At, m_Up; // �� ��ȯ�� ���� ��ġ (ī�޶� ��ġ, ���� ����)

	// ���� ��� ��Ʈ
	LPD3DXFONT m_pFont;

	BOX_PROPERTY m_Box[2]; // �浹 üũ�� ���� �ڽ� 2��
	D3DXVECTOR3 m_vMin, m_vMax; // ť�� �浹�� ���� �ִ���, �ּ��� ���÷� ����
	BOOL m_bIsCollision; // �浹 ���� BOOL��
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

