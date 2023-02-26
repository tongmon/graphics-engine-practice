#pragma once
#include "CD3DApp.h"

struct CUSTOMVERTEX
{
	float x, y, z, rhw; // rhw�� 2d ��, ����� ���� ��ǥ�踦 ���ؼ� ������ ���� �� ���� ���δ�.
	DWORD color;
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE) // ���� ��� ��� ����, Format

class CGameEdu01 : public CD3DApp
{
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;

private:
	virtual void OnInit();
	virtual void OnRender();
	virtual void OnUpdate();
	virtual void OnRelease();

public:
	CGameEdu01(void);
	~CGameEdu01(void);
};

