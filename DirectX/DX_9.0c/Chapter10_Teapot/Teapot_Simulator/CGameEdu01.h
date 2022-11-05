#pragma once
#include "CD3DApp.h"
#include "CAxis.h"
#include "CCube.h"

class CGameEdu01 : public CD3DApp
{
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj; // ���� ��ȯ�� ���� ���
	D3DXVECTOR3 m_Eye, m_At, m_Up; // �� ��ȯ�� ���� ��ġ (ī�޶� ��ġ, ���� ����)
	CAxis m_Axis; // ȭ�鿡 ����� ���� ����
	CCube m_Cube; // ȭ�鿡 ����� ť���� ����

	// ���� ���� ��°� �ٸ��� �ε��� ���۴� ���� ���۴� �ʿ���� 
	// �� ���� �Լ��� ����� ������ ���� Ŭ���� ���� ��� ������ ����
	LPD3DXMESH m_pTeapotMesh; // ������ �޽� ����
	float m_fScale;

private:
	virtual void OnInit();
	virtual void OnRender();
	virtual void OnUpdate();
	virtual void OnRelease();

public:
	CGameEdu01(void);
	~CGameEdu01(void);
};

