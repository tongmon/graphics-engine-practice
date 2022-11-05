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

	m_Eye.x = 0.0f;
	m_Eye.y = 0.1f;
	m_Eye.z = -10.0f;

	m_At.x = 0.0f; m_At.y = 0.0f; m_At.z = 0.0f;

	m_Up.x = 0.0f; m_Up.y = 1.0f; m_Up.z = 0.0f;

	D3DXMatrixLookAtLH(&m_matView, &m_Eye, &m_At, &m_Up); // �� ��ȯ ��� ����
	m_pd3dDevices->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f); // �������� ��ȯ ��� ���� (����)
	m_pd3dDevices->SetTransform(D3DTS_PROJECTION, &m_matProj);
	m_pd3dDevices->SetViewport(&vp); // �� ��Ʈ ����

	// �ﰢ�� ����, ��Ʈ�� ���·� ����ҰŶ� �ߺ��Ǵ� ������ ����.
	CUSTOMVERTEX vertices[] = {
		{ D3DXVECTOR3(-1.0f, 1.0f, 0.0f), 0.0f, 0.0f },
		{ D3DXVECTOR3(1.0f, 1.0f, 0.0f), 1.0f, 0.0f },
		{ D3DXVECTOR3(-1.0f, -1.0f, 0.0f), 0.0f, 1.0f },
		{ D3DXVECTOR3(1.0f, -1.0f, 0.0f), 1.0f, 1.0f },
	};

	// �Ϲ����� ���� ���� ����
	m_pd3dDevices->CreateVertexBuffer(sizeof(vertices), 0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &m_pVB, NULL);

	VOID* pVertices;
	m_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0);
	memcpy(pVertices, vertices, sizeof(vertices));
	m_pVB->Unlock();

	// �ܺηκ��� �̹����� �о� �ؽ��� ��ü�� m_pTexture�� �������ش�.
	D3DXCreateTextureFromFile(m_pd3dDevices, "banana.bmp", &m_pTexture); // �̹����� tu, tv�� �°� �������ִ� ���� �Լ�

	// �����ϸ� ��� ����
	D3DXMatrixScaling(&m_matScale[0], 0.5f, 0.5f, 0.5f);
	D3DXMatrixScaling(&m_matScale[1], 1.5f, 1.5f, 1.5f);

	// �̵� ��� ����
	D3DXMatrixTranslation(&m_matTranslate[0], -2.0f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&m_matTranslate[1], 1.5f, 0.0f, 0.0f);

	m_pd3dDevices->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void CGameEdu01::OnRender()
{
	D3DXMATRIX matWorld;

	m_pd3dDevices->SetTexture(0, m_pTexture); // �ؽ��� ����, ���� ���ڴ� �ؽ����� �ε����� �ִ� 8�������� �ؽ��ĸ� ��� �������� �����ϴ�.
	m_pd3dDevices->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEX));
	m_pd3dDevices->SetFVF(D3DFVF_CUSTOMVERTEX);

	for (int i = 0; i < 2; i++)
	{
		matWorld = m_matScale[i] * m_matTranslate[i];
		m_pd3dDevices->SetTransform(D3DTS_WORLD, &matWorld); // ST ����
		m_pd3dDevices->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2); // �ﰢ�� 2�� ���޾� ��Ʈ������ ���, ��Ʈ���̶� ���� �ߺ��� ����.
	}
}

void CGameEdu01::OnUpdate()
{
	
}

void CGameEdu01::OnRelease()
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
	}
	if (m_pVB != NULL)
	{
		m_pVB->Release();
	}
}