#pragma once
#include "CD3DApp.h"
#include "CAxis.h"
#include "CCube.h"
#include "CGround.h"

class CGameEdu01 : public CD3DApp
{
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj; // ���� ��ȯ�� ���� ���
	D3DXVECTOR3 m_Eye, m_At, m_Up; // �� ��ȯ�� ���� ��ġ (ī�޶� ��ġ, ���� ����)
	CAxis m_Axis; // ȭ�鿡 ����� ���� ����
	CCube m_Cube; // ȭ�鿡 ����� ť���� ����

	// ��Ʈ
	LPD3DXFONT m_pFont;
	LPD3DXFONT m_pFont2;

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

