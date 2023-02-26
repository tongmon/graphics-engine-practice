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

	m_Axis.OnInit(m_pd3dDevices);
	//m_Cube.OnInit(m_pd3dDevices);

	// m_pTeapotMesh�� ������ ������ ���� �� �ߴ� �ε��� ����, ���� ���� � ���� ������ �� ����ִ�.
	// ������ �Լ��� �ּ����� ��� �����ִ�.
	// �����̽��� ����, ����ũ �ڸ����� �ڸ��°�, ������ �ҽ��� �ڸ����� �ڸ��°� ��� ��������
	// �����̽��� ������ ���� ������ ���� �ε巯�� ��� ���� �� ������ ������ ������ ���귮�� �����Ѵ�.
	D3DXCreateTeapot( m_pd3dDevices, &m_pTeapotMesh, NULL ); // ������
	m_fScale = 1.0f;
}

void CGameEdu01::OnRender()
{
	D3DXMATRIX matScale; // ������ �����ϱ� ���� ���

	m_Axis.OnRender();
	//m_Cube.OnRender();

	m_pd3dDevices->SetRenderState(D3DRS_LIGHTING, FALSE); // ����Ʈ ���ָ� �Ⱥ��̱⿡ �ϴ� ����
	m_pd3dDevices->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); // ���븸 ���̱�
	m_pd3dDevices->SetRenderState( D3DRS_CULLMODE,  D3DCULL_NONE); // �ø� ����, �޸� ���� ����

	D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale); // �����ϸ� ���ش�.
	m_pd3dDevices->SetTransform(D3DTS_WORLD, &matScale); // ����, �� ��ü ��ǥ�� �����ϸ��ؼ� ��ġ��Ų��.

	// ������ ���
    m_pTeapotMesh->DrawSubset(0);	
}

void CGameEdu01::OnUpdate()
{
	if (GetAsyncKeyState('A')<0)
	{
		m_fScale = m_fScale + 0.1f;
	}
	if (GetAsyncKeyState('D') < 0)
	{
		m_fScale = m_fScale - 0.1f;
	}
}

void CGameEdu01::OnRelease()
{
	m_Axis.OnRelease();
	//m_Cube.OnRelease();

	m_pTeapotMesh->Release();
}