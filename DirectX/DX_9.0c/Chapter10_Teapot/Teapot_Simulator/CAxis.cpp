#include "CAxis.h"

CAxis::CAxis(void)
{
	m_pd3dDevice = NULL;
	m_pVB = NULL;
}


CAxis::~CAxis(void)
{
}


void CAxis::OnInit(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;

	// �� �׸���, ���� ���ϸ� �ȵǴ� Init���� ó���Ѵ�.
	CUSTOMVERTEX vertices[] =
	{
		{ -3.0f,  0.0f, 0.0f, D3DCOLOR_RGBA(255, 0, 0, 255) },  // x �� (������)
		{ 3.0f,  0.0f, 0.0f, D3DCOLOR_RGBA(255, 0, 0, 255) },

		{ 0.0f,  -3.0f, 0.0f, D3DCOLOR_RGBA(0, 255, 0, 255) }, // y �� (�ʷϻ�)
		{ 0.0f,  3.0f, 0.0f, D3DCOLOR_RGBA(0, 255, 0, 255) },

		{ 0.0f,  0.0f, -3.0f, D3DCOLOR_RGBA(0, 255, 255, 255) }, // z �� (����Ÿ���� ����)
		{ 0.0f,  0.0f, 3.0f, D3DCOLOR_RGBA(0, 255, 255, 255) }		
	};

	// ���� ���ۿ� ������ ���� �ֱ�
	m_pd3dDevice->CreateVertexBuffer(6 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &m_pVB, NULL);

	VOID* pVertices;
	m_pVB->Lock(0, sizeof(vertices), &pVertices, 0);
	memcpy(pVertices, vertices, sizeof(vertices));
	m_pVB->Unlock();
}

void CAxis::OnRender()
{
	D3DXMATRIX matWorld;
	// �������(E), ������ ����(vertices)�鿡 ��� ��ȭ�� ���� �ʰ� �״�� ����ϱ⿡ ������� ���
	D3DXMatrixIdentity(&matWorld);

	// ���� ���� ����� �ȼ����� ���ִ� ����
	// �鿡 ���� ���͸� ������ �ʰ� �ܼ��ϰ� ���� �ߴ� ���̱⿡ �������� ����.
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	// ���� ���� ���
	m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEX));
	m_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	
	// �� ��ǥ�踦 �״�� ���ڴٴ� ����
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	// �� �׸���
	m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, 3);

	// �ٽ� ������ Ų ���� ���·� ����
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CAxis::OnRelease()
{
	if (m_pVB != NULL)
		m_pVB->Release();
}