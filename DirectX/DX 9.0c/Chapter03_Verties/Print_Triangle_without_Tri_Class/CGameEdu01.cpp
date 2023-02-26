#include "CGameEdu01.h"

CGameEdu01::CGameEdu01()
{
	m_pVB = NULL;
}

CGameEdu01::~CGameEdu01()
{

}

void CGameEdu01::OnInit()
{
	CUSTOMVERTEX vertices[] =
	{
		{150.0f, 50.0f, 0.5f, 1.0f, 0xffff0000},
		{250.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00},
		{50.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff}
	};
	// ����� �������� ���� ���ۿ� �ִ´�.
	m_pd3dDevices->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pVB, NULL);

	void* pVertices;
	m_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0);
	// ���� ���ۿ� �ִ� �����͵� ��ŭ �޸� ������ ������ pVertices�� �Ű� ��밡���ϰ� �����.
	memcpy(pVertices, vertices, sizeof(vertices)); 
	m_pVB->Unlock();
}

void CGameEdu01::OnRender()
{
	m_pd3dDevices->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEX)); // ����� ���� ���۸� ����̽��� ������ ��Ʈ���� ����
	m_pd3dDevices->SetFVF(D3DFVF_CUSTOMVERTEX); // ���� ��� ���� ����
	m_pd3dDevices->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1); // �ﰢ�� 1�� �׸���
}

void CGameEdu01::OnUpdate()
{

}

void CGameEdu01::OnRelease()
{
	if (m_pVB != NULL)
	{
		m_pVB->Release();
	}
}