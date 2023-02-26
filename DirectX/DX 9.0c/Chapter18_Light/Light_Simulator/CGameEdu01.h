#pragma once
#include "CD3DApp.h"

struct CUSTOMVERTEX // ���� ����, ���� ������ �� �ݻ翡 ������ �ֱ⿡ �����ȴ�.
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNormal; // ���� ����
};

struct CROSSVERTEX // ���� ���Ϳ� ���� ����
{
	D3DXVECTOR3 vPos;
	DWORD dwDiffuse;
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL)
#define D3DFVF_CROSSVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

class CGameEdu01 : public CD3DApp
{
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj; // ���� ��ȯ�� ���� ���
	D3DXVECTOR3 m_Eye, m_At, m_Up; // �� ��ȯ�� ���� ��ġ (ī�޶� ��ġ, ���� ����)

	// ���� ������ �ΰ��� ���� ���۵� �� ����.
	LPDIRECT3DVERTEXBUFFER9 m_pVB; // �ﰢ��(����) �׸��� ���� ����
	LPDIRECT3DVERTEXBUFFER9 m_pCrossVB; // ���� ���� ����

private:
	virtual void OnInit();
	virtual void OnRender();
	virtual void OnUpdate();
	virtual void OnRelease();

public:
	// ���� ������ ���� API ����, �Լ���
	D3DMATERIAL9 m_Material; // ���� ó�� ����ü
	D3DLIGHT9 m_Light; // ���� ó�� ����ü
	void SetMaterial(float r, float g, float b);
	void SetDirectionalLight();

public:
	CGameEdu01(void);
	~CGameEdu01(void);
};

