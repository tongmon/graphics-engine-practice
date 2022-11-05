#pragma once
#include "CD3DApp.h"

struct CUSTOMVERTEX
{
	D3DXVECTOR3 vPos;
	float tu, tv; // tu, tv�� �׸��� �����Ǵ� ���� ���̴�. ������ (0,0) ~ (1,1) ���� �����ȴ�.
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)

class CGameEdu01 : public CD3DApp
{
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj; // ���� ��ȯ�� ���� ���
	D3DXVECTOR3 m_Eye, m_At, m_Up; // �� ��ȯ�� ���� ��ġ (ī�޶� ��ġ, ���� ����)

	// ���� ���۵�
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DTEXTURE9 m_pTexture; // �ؽ��Ŀ� �����Ǵ� ���� ����

	D3DXMATRIX m_matScale[2], m_matTranslate[2];

private:
	virtual void OnInit();
	virtual void OnRender();
	virtual void OnUpdate();
	virtual void OnRelease();

public:
	CGameEdu01(void);
	~CGameEdu01(void);
};

