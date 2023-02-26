#pragma once
#include <d3d9.h>

struct CUSTOMVERTEX // ���� ����ü ��� 1
{
	float x, y, z, rhw; // rhw�� 2d ��, ����� ���� ��ǥ�踦 ���ؼ� ������ ���� �� ���� ���δ�.
	DWORD color;
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE) // ���� ��� ��� ����

class CTriangle
{
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DDEVICE9 m_pd3dDevice;

public:
	void OnInit(LPDIRECT3DDEVICE9 pd3dDevice);
	void OnRender();
	void OnUpdate();
	void OnRelease();

public:
	CTriangle();
	~CTriangle();
};
