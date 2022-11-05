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

	m_Eye.x = 15.0f;
	m_Eye.y = 10.0f;
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
}

void CGameEdu01::OnRender()
{
	D3DXMATRIX matRotate, matTrans, matScaling, matWorld; // ��ġ �����ϱ� ���� ���
	float fScaling[3] = { 0.3f, 0.6f, 1.0f }; // ���� ������, ���� ������, ���� ũ��
	D3DXVECTOR3 vTrans[3] = { D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(2.0f, 0.0f, 0.0f) , D3DXVECTOR3(5.0f, 0.0f, 0.0f) }; // �� ��ġ, x�� 2�̵�, x�� 5�̵�

	m_Axis.OnRender();
	//m_Cube.OnRender();

	m_pd3dDevices->SetRenderState(D3DRS_LIGHTING, FALSE); // ����Ʈ ���ָ� �Ⱥ��̱⿡ �ϴ� ����
	m_pd3dDevices->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); // ���븸 ���̱�

	D3DXMatrixRotationY(&matRotate, GetTickCount64() * 0.004f); // ȸ�� ��� ���

	// ù��° ������ �¾� �� ���
	D3DXMatrixScaling(&matScaling, fScaling[0], fScaling[0], fScaling[0]);
	D3DXMatrixTranslation(&matTrans, vTrans[0].x, vTrans[0].y, vTrans[0].z);
	// SRT ���� �����ϸ� -> ȸ�� -> �̵� ����
	matWorld = matScaling * matRotate * matTrans;
	m_pd3dDevices->SetTransform(D3DTS_WORLD, &matWorld); // ����, �� ��ü ��ǥ�� ȸ���ؼ� ��ġ��Ų��.
	m_pTeapotMesh->DrawSubset(0); // ������ ���

	// �ι�° ������ �¾� �� ���
	D3DXMatrixScaling(&matScaling, fScaling[1], fScaling[1], fScaling[1]);
	D3DXMatrixTranslation(&matTrans, vTrans[1].x, vTrans[1].y, vTrans[1].z);
	// SRT ���� �����ϸ� -> ȸ�� -> �̵� ����
	matWorld = matScaling * matRotate * matTrans;
	m_pd3dDevices->SetTransform(D3DTS_WORLD, &matWorld); // ����, �� ��ü ��ǥ�� ȸ���ؼ� ��ġ��Ų��.
	m_pTeapotMesh->DrawSubset(0); // ������ ���

	// ����° ������ �¾� �� ���
	D3DXMatrixScaling(&matScaling, fScaling[2], fScaling[2], fScaling[2]);
	D3DXMatrixTranslation(&matTrans, vTrans[2].x, vTrans[2].y, vTrans[2].z);
	// STR ���� �����ϸ� -> �̵� ���� -> ȸ�� (SRT�� �������� �̰� �� �Ⱦ���.)
	// �̷��� �� ��� ȸ���� ���� ���� ������ ��� ���� ���� �� �������� ũ�� �� ���� �� ���� ǥ���ȴ�.
	// �̵��� �Ÿ���ŭ�� ȸ������ �������̶�� �����ϸ� ����.
	matWorld = matScaling * matTrans * matRotate;
	m_pd3dDevices->SetTransform(D3DTS_WORLD, &matWorld); // ����, �� ��ü ��ǥ�� ȸ���ؼ� ��ġ��Ų��.
	m_pTeapotMesh->DrawSubset(0); // ������ ���

}

void CGameEdu01::OnUpdate()
{
	
}

void CGameEdu01::OnRelease()
{
	m_Axis.OnRelease();
	//m_Cube.OnRelease();

	m_pTeapotMesh->Release();
}