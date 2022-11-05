#include "CGameEdu01.h"

CGameEdu01::CGameEdu01()
{
	
}

CGameEdu01::~CGameEdu01()
{

}

void CGameEdu01::OnInit()
{
	RECT rect;
	D3DVIEWPORT9 vp;
	vp.X = 0;
	vp.Y = 0;

	// �� ũ�� ���� (â ũ��� �ϴ� ���� �س��� â ũ�� �ٲ𶧸��� �ٽ� ����)
	GetClientRect(m_hWnd, &rect);
	vp.Width = rect.right - rect.left; // ��ǥ ������ �ƴ϶� ũ�� ����
	vp.Height = rect.bottom - rect.top;

	// ���� ������ ������ 0 ~ 1 ����
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;

	m_Eye.x = 5.0f;
	m_Eye.y = 8.0f;
	m_Eye.z = -10.0f;

	m_At.x = 0.0f; m_At.y = 0.0f; m_At.z = 0.0f;

	m_Up.x = 0.0f; m_Up.y = 1.0f; m_Up.z = 0.0f;

	D3DXMatrixLookAtLH(&m_matView, &m_Eye, &m_At, &m_Up); // �� ��ȯ ��� ����
	m_pd3dDevices->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f); // �������� ��ȯ ��� ���� (����)
	m_pd3dDevices->SetTransform(D3DTS_PROJECTION, &m_matProj);
	m_pd3dDevices->SetViewport(&vp); // �� ��Ʈ ����

	// �񽺵��ϰ� �ﰢ�� ����
	CUSTOMVERTEX vertices[3] = {
		{ D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DCOLOR_RGBA(100, 100, 100, 255) },
		{ D3DXVECTOR3(1.0f, -1.0f, 1.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DCOLOR_RGBA(100, 100, 100, 255) },
		{ D3DXVECTOR3(1.0f, -1.0f, -1.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DCOLOR_RGBA(100, 100, 100, 255) }
	};

	m_pd3dDevices->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &m_pVB, NULL);

	// ���� ���� ���
	D3DXVECTOR3 vS0, vS1;
	vS0 = vertices[2].vPos - vertices[0].vPos; // ���� V2 - V0 �� �Ͽ� ���� ->V0 �� ��´�. ������ ->, �׳� V0�� �ƴ϶�!!!
	vS1 = vertices[1].vPos - vertices[0].vPos;
	D3DXVec3Cross(&vertices[0].vNormal, &vS1, &vS0); // ���� ���� ���ϱ�
	D3DXVec3Normalize(&vertices[0].vNormal, &vertices[0].vNormal); // ũ�� 1�� �����

	// �ϳ��� �ﰢ���� �����ϴ� �������ʹ� ��� ����. 
	vertices[2].vNormal = vertices[1].vNormal = vertices[0].vNormal; // vNormal�� ���� ����

	VOID* pVertices;
	m_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0);
	memcpy(pVertices, vertices, sizeof(vertices));
	m_pVB->Unlock();

	// ���� ���� ����� ���� ���� == 7���� �� Ŭ������ �����ϸ� ����.
	CROSSVERTEX vCrossVertices[6];
	vCrossVertices[0].vPos = vertices[0].vPos; // ��� �׳� �ﰢ�� ������
	// �ﰢ���� �� ������ + ���� ���� <-- �̰��� �ϴ� ������ ������ �׸� �� �ʿ��� ���� ��� ���ؼ���.
	vCrossVertices[1].vPos = vertices[0].vPos + vertices[0].vNormal;

	vCrossVertices[2].vPos = vertices[1].vPos;
	vCrossVertices[3].vPos = vertices[1].vPos + vertices[1].vNormal;

	vCrossVertices[4].vPos = vertices[2].vPos;
	vCrossVertices[5].vPos = vertices[2].vPos + vertices[2].vNormal;
	for (int i = 0; i < 6; i++)
	{
		vCrossVertices[i].dwDiffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	m_pd3dDevices->CreateVertexBuffer(sizeof(vCrossVertices), 0, D3DFVF_CROSSVERTEX,
		D3DPOOL_DEFAULT, &m_pCrossVB, NULL);

	m_pCrossVB->Lock(0, sizeof(vertices), &pVertices, 0);
	memcpy(pVertices, vCrossVertices, sizeof(vCrossVertices));
	m_pCrossVB->Unlock();

	m_pd3dDevices->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void CGameEdu01::OnRender()
{	
	m_pd3dDevices->SetFVF(D3DFVF_CUSTOMVERTEX);
	m_pd3dDevices->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEX));
	m_pd3dDevices->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1); // �ﰢ�� �׸���

	m_pd3dDevices->SetFVF(D3DFVF_CROSSVERTEX);
	m_pd3dDevices->SetStreamSource(0, m_pCrossVB, 0, sizeof(CROSSVERTEX));
	m_pd3dDevices->DrawPrimitive(D3DPT_LINELIST, 0, 3); // ���� 3�� �׸���
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
	if (m_pCrossVB != NULL)
	{
		m_pCrossVB->Release();
	}
}