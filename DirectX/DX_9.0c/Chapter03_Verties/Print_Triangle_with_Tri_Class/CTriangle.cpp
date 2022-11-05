#include "CTriangle.h"

CTriangle::CTriangle()
{
	m_pd3dDevice = NULL;
	m_pVB = NULL;
}

CTriangle::~CTriangle()
{

}

void CTriangle::OnInit(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
	CUSTOMVERTEX vertices[] =
	{
		{150.0f, 50.0f, 0.5f, 1.0f, 0xffff0000},
		{250.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00},
		{50.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff}
	};
	m_pd3dDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pVB, NULL);

	void* pVertices;
	m_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0);
	// ���� ���ۿ� �ִ� �����͵� ��ŭ �޸� ������ ������ pVertices�� �Ű� ��밡���ϰ� �����.
	memcpy(pVertices, vertices, sizeof(vertices));
	m_pVB->Unlock();
}

void CTriangle::OnRender()
{
	m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEX)); // ����� ���� ���۸� ����̽��� ������ ��Ʈ���� ����
	m_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX); // ���� ��� ���� ����
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1); // �ﰢ�� 1�� �׸���
}

void CTriangle::OnUpdate()
{

}

void CTriangle::OnRelease()
{
	if (m_pVB != NULL)
	{
		m_pVB->Release();
	}
}