#pragma once
#include <d3d9.h>
#include <d3dx9math.h>

struct CUSTOMVERTEX
{
	FLOAT x, y, z; // The transformed position for the vertex.
	DWORD color;        // The vertex color.
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE) // x,y,z ��ǥ�� ������ ����

class CAxis
{
private:
	LPDIRECT3DDEVICE9   m_pd3dDevice; // ����̽� �����ؾ� �ϴϱ� ����̽� ������
	LPDIRECT3DVERTEXBUFFER9 m_pVB; // ������ ��ƾ� �ϴϱ� ���� ���� ������

public:
	void OnInit(LPDIRECT3DDEVICE9 pd3dDevice);
	void OnRender();
	void OnRelease();

public:
	CAxis(void);
	~CAxis(void);
};
