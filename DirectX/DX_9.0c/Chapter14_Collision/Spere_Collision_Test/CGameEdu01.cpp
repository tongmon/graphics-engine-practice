#include "CGameEdu01.h"

BOOL CheckSphereIntersect(D3DXVECTOR3 vCenter1, float fRadius1,
	D3DXVECTOR3  vCenter2, float fRadius2) // �� ��ġ�� ���� �浹üũ �Ǵ� �Լ�, ������ �߽ɰ� �������� �̿��ϸ� ���� �� �ִ�.
{
	float fDistance;
	D3DXVECTOR3 vDiff;

	vDiff = vCenter2 - vCenter1;
	fDistance = D3DXVec3Length(&vDiff);

	if (fDistance <= (fRadius1 + fRadius2))
	{
		return TRUE;  // �浹
	}
	return FALSE; // �� �浹
}

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
	m_Eye.y = 5.0f;
	m_Eye.z = -20.0f;

	m_At.x = 0.0f; m_At.y = 0.0f; m_At.z = 0.0f;

	m_Up.x = 0.0f; m_Up.y = 1.0f; m_Up.z = 0.0f;

	D3DXMatrixLookAtLH(&m_matView, &m_Eye, &m_At, &m_Up); // �� ��ȯ ��� ����
	m_pd3dDevices->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f); // �������� ��ȯ ��� ���� (����)
	m_pd3dDevices->SetTransform(D3DTS_PROJECTION, &m_matProj);
	m_pd3dDevices->SetViewport(&vp); // �� ��Ʈ ����

	D3DXCreateFont(m_pd3dDevices, 15, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		"����ü", &m_pFont); // ��Ʈ ���� 1

	D3DXCreateSphere(m_pd3dDevices, 1.0f, 20, 20, &m_pMesh, NULL); // �� ����

	// ��_1 �Ӽ� �ʱ�ȭ
	m_Sphere[0].fRadius = 1.0f;
	m_Sphere[0].fScaling = 1.0f;
	m_Sphere[0].vTans = D3DXVECTOR3(0, 0, 0);
	// ��_2 �Ӽ� �ʱ�ȭ
	m_Sphere[1].fRadius = 1.0f;
	m_Sphere[1].fScaling = 2.0f;
	m_Sphere[1].vTans = D3DXVECTOR3(5, 0, 0);

	m_bIsCollision = FALSE; // �ʹ� �浹 X

	m_pd3dDevices->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevices->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}

void CGameEdu01::OnRender()
{
	char string[100];
	RECT rt; // ���ڸ� ��ġ��Ű�� ���� �簢��
	D3DXMATRIX matWorld, matScale, matTrans;

	for (int i = 0; i < 2; i++) // �� ������ �Ӽ��� �°� ���
	{
		D3DXMatrixTranslation(&matTrans, m_Sphere[i].vTans.x, m_Sphere[i].vTans.y, m_Sphere[i].vTans.z);
		D3DXMatrixScaling(&matScale, m_Sphere[i].fScaling, m_Sphere[i].fScaling, m_Sphere[i].fScaling);
		matWorld = matScale * matTrans;
		m_pd3dDevices->SetTransform(D3DTS_WORLD, &matWorld);

		m_pMesh->DrawSubset(0);
	}

	SetRect(&rt, 10, 10, 0, 0);
	wsprintf(string, "FPS: %d", m_nFPS);
	m_pFont->DrawText(NULL, string, -1, &rt, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)); // ������ RGB �׸��� ������ ��ġ�Ѵ�.
	
	m_nFPSCount++; // 1�ʰ� �Ǳ� ������ FPS�� ��� �������� �ش�.

	SetRect(&rt, 10, 32, 0, 0);
	if (m_bIsCollision)
	{
		m_pFont->DrawText(NULL, "�浹", -1, &rt, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	}
	else
	{
		m_pFont->DrawText(NULL, "���浹", -1, &rt, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	}
}

void CGameEdu01::OnUpdate()
{
	DWORD dwCurTime = GetTickCount64(); // ���� �ð��� ���´�
	static DWORD dwOldTime = GetTickCount64(); // ���� ������ ���� �ð��� �ѹ��� �ʱ�ȭ�ϰ� �� ���� ȣ��� ���� �ð��� �ȴ�.
	static DWORD dwAccumulateTime = 0; // ���� �ð�
	m_dwElapsedTime = dwCurTime - dwOldTime; // ���� �ð��� ���� �ð��� ���̸� ���� �ð��� �־��ش�. (�� ������)
	dwOldTime = dwCurTime; // ���� �ð��� ���� ȣ��� ���� �ð��� �ȴ�.

	dwAccumulateTime = dwAccumulateTime + m_dwElapsedTime; // ���� �ð��� ���� �ð��� �׿�����.
	if (dwAccumulateTime >= 1000) // 1�ʰ� �Ǿ����� FPS ���
	{
		dwAccumulateTime = 0; // ���� �ð� �ʱ�ȭ
		m_nFPS = m_nFPSCount; // ���� ���� ������Ų FPS ���� ȭ�鿡 ����ϱ� ���� m_nFPS�� �־��ش�.
		m_nFPSCount = 0; // �ٽ� FPS �ʱ�ȭ
	}

	// ��ŸŸ������ ������
	if (GetAsyncKeyState(VK_LEFT))
	{
		m_Sphere[1].vTans.x -= m_dwElapsedTime * 0.003f; // m_dwElapsedTime(�ð�), 0.003f(�ӵ�) -> �ð� x �ӵ� = �Ÿ�
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		m_Sphere[1].vTans.x += m_dwElapsedTime * 0.003f;
	}
	m_bIsCollision = CheckSphereIntersect(m_Sphere[0].vTans, m_Sphere[0].fRadius * m_Sphere[0].fScaling,
		m_Sphere[1].vTans, m_Sphere[1].fRadius * m_Sphere[1].fScaling); // �浹 üũ
}

void CGameEdu01::OnRelease()
{
	m_pFont->Release();
	m_pMesh->Release();
}