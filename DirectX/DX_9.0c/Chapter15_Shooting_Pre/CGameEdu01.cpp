#include "CGameEdu01.h"

BOOL CheckCubeIntersection(D3DXVECTOR3* vMin1, D3DXVECTOR3* vMax1,
	D3DXVECTOR3* vMin2, D3DXVECTOR3* vMax2) // ť�� �浹 üũ �Լ�, �� ť���� �ִ����� �ּ����� ���Ͽ� ��ġ�� �κ��� ������ �浹�� �Ǵ��Ѵ�.
{
	if (vMin1->x <= vMax2->x && vMax1->x >= vMin2->x &&
		vMin1->y <= vMax2->y && vMax1->y >= vMin2->y &&
		vMin1->z <= vMax2->z && vMax1->z >= vMin2->z)
		return TRUE;
	return FALSE;
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

	// ī�޶� ��ġ
	m_Eye.x = 0.0f;
	m_Eye.y = 10.0f;
	m_Eye.z = -32.0f;

	m_At.x = 0.0f; m_At.y = 0.0f; m_At.z = 0.0f; // �Ĵٺ��� ��ġ

	m_Up.x = 0.0f; m_Up.y = 1.0f; m_Up.z = 0.0f;

	D3DXMatrixLookAtLH(&m_matView, &m_Eye, &m_At, &m_Up); // �� ��ȯ ��� ����
	m_pd3dDevices->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f); // �������� ��ȯ ��� ���� (����)
	m_pd3dDevices->SetTransform(D3DTS_PROJECTION, &m_matProj);
	m_pd3dDevices->SetViewport(&vp); // �� ��Ʈ ����

	// ���ΰ�(������) �޽� ����
	D3DXCreateTeapot(m_pd3dDevices, &m_pTeapotMesh, NULL);
	// �Ѿ� �޽� ����, �޽��� �Ѿ��� ���ٰ� ���� ������ �ʿ䰡 ���� �ϳ��� �����ϰ� �Ѿ� �Ӽ� ��ü(BULLET)�� ���� �����ϸ� �Ǵ� ���̴�.
	D3DXCreateCylinder(m_pd3dDevices, 2.0f, 0.0f, 10.0f, 15, 10, &m_pPlayerBulletMesh, NULL);
	// ���� ����
	m_Ground.Create(m_pd3dDevices, 20, 6, 2.0f);

	// Note: ���ΰ� ����
	m_sPlayer.nLife = 10; // ü�� 10����
	m_sPlayer.fScale = 0.7f; // ũ�� ���� ũ�⺸�� �ణ �۰�
	m_sPlayer.fRotationY = -D3DXToRadian(90); // �ݴ� 90���� ȸ��
	m_sPlayer.vPos = D3DXVECTOR3(0.0f, 0.0f, -9 * 2.0f); // �ʱ� ��ġ ����(ī�޶� ���� z�� �ڷ� ���ش�.)
	m_sPlayer.fVelocity = 0.005f; // �ӵ� ����
	m_sPlayer.dwBulletFireTime = 400; // �Ѿ� ��� ����
	m_sPlayer.dwOldBulletFireTime = GetTickCount64(); // �Ѿ� �� �ð�, �׳� �ʱ�ȭ��� �����ϸ� ��

	D3DXMatrixScaling(&m_sPlayer.matScale, m_sPlayer.fScale, m_sPlayer.fScale, m_sPlayer.fScale); // ĳ������ �����ϸ� ��� ����
	D3DXMatrixTranslation(&m_sPlayer.matTranslation, m_sPlayer.vPos.x, m_sPlayer.vPos.y, m_sPlayer.vPos.z); // ĳ������ �̵� ��� ����
	D3DXMatrixRotationY(&m_sPlayer.matRotationY, m_sPlayer.fRotationY); // ĳ������ ȸ�� ��� ����

	// Note: ���ΰ� �Ѿ� ����
	m_sPlayerBulletProperty.fBulletVelcoty = 0.01f;
	m_sPlayerBulletProperty.fScale = 0.08f;
	D3DXMatrixScaling(&m_sPlayerBulletProperty.matScale, m_sPlayerBulletProperty.fScale, m_sPlayerBulletProperty.fScale, m_sPlayerBulletProperty.fScale); // �Ѿ� �������� ���

	// ��Ʈ ����
	D3DXCreateFont(m_pd3dDevices, 15, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "����ü", &m_pFont);
}

void CGameEdu01::OnRender()
{
	char string[100];
	RECT rt; // ���ڸ� ��ġ��Ű�� ���� �簢��
	D3DXMATRIX matWorld; // ������, ��ġ, ȸ�� ��ȯ�� ��� ĳ���͸� ��ȯ�ϰ� �� ��� (SRT)
	m_Ground.OnRender(); // ���� ���

	// �������� �ϴ� ������ ���븸 ���̰� ����Ѵ�.
	m_pd3dDevices->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevices->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	// ���ΰ� �̻���
	for (int i = 0; i < 10; i++)
	{
		if (m_sPlayerBullet[i].nLife > 0) // �Ѿ� ü���� 1 �̻��̸� �Ӽ������ ������ش�.
		{
			matWorld = m_sPlayerBulletProperty.matScale * m_sPlayerBullet[i].matTranslation; // �Ѿ��� ȸ���� �ʿ���� ST�� ���ش�.
			m_pd3dDevices->SetTransform(D3DTS_WORLD, &matWorld); // �Ѿ��� ���忡 ��ȯ�� ���·� ������ ����
			m_pPlayerBulletMesh->DrawSubset(0);
		}
	}

	// ���ΰ� ���		
	m_pd3dDevices->SetTransform(D3DTS_WORLD, &m_sPlayer.matWorld);
	m_pTeapotMesh->DrawSubset(0);

	// FPS ���� ���
	SetRect(&rt, 10, 10, 0, 0);
	wsprintf(string, "FPS: %d", m_nFPS);
	m_pFont->DrawText(NULL, string, -1, &rt, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)); // ������ RGB �׸��� ������ ��ġ�Ѵ�.
	
	m_nFPSCount++; // 1�ʰ� �Ǳ� ������ FPS�� ��� �������� �ش�.
}

void CGameEdu01::OnUpdate()
{
	int i; // �ݺ����� ���� ����
	DWORD dwCurTime = GetTickCount64(); // ���� �ð��� ���´�
	static DWORD dwOldTime = GetTickCount64(); // ���� ������ ���� �ð��� �ѹ��� �ʱ�ȭ�ϰ� �� ���� ȣ��� ���� �ð��� �ȴ�.
	static DWORD dwAccumulateTime = 0; // ���� �ð�
	m_dwElapsedTime = dwCurTime - dwOldTime; // ���� �ð��� ���� �ð��� ���̸� ���� �ð��� �־��ش�. (�� ������)
	dwOldTime = dwCurTime; // ���� �ð��� ���� ȣ��� ���� �ð��� �ȴ�.

	// Ű�Է¿� ���� ��ġ �� ����
	if (GetAsyncKeyState(VK_LEFT) < 0)
	{
		if (m_sPlayer.vPos.x - (m_dwElapsedTime * m_sPlayer.fVelocity) >= -6.0f) // ���� ���� ������ ���ϰ� ����
			m_sPlayer.vPos.x -= m_dwElapsedTime * m_sPlayer.fVelocity;
	}
	if (GetAsyncKeyState(VK_RIGHT) < 0)
	{
		if (m_sPlayer.vPos.x + (m_dwElapsedTime * m_sPlayer.fVelocity) <= 6.0f)
			m_sPlayer.vPos.x += m_dwElapsedTime * m_sPlayer.fVelocity;
	}

	if (GetAsyncKeyState(VK_UP) < 0)
	{
		if (m_sPlayer.vPos.z + (m_dwElapsedTime * m_sPlayer.fVelocity) <= 20.0f)
			m_sPlayer.vPos.z += m_dwElapsedTime * m_sPlayer.fVelocity;
	}
	if (GetAsyncKeyState(VK_DOWN) < 0)
	{
		if (m_sPlayer.vPos.z - (m_dwElapsedTime * m_sPlayer.fVelocity) >= -19.0f)
			m_sPlayer.vPos.z -= m_dwElapsedTime * m_sPlayer.fVelocity;
	}

	if (GetAsyncKeyState('S') < 0)  // ���ΰ� �̻��� �߻�
	{
		if (dwCurTime - m_sPlayer.dwOldBulletFireTime >= m_sPlayer.dwBulletFireTime)
		{
			m_sPlayer.dwOldBulletFireTime = dwCurTime;

			for (i = 0; i < 10; i++)
			{
				if (m_sPlayerBullet[i].nLife <= 0)
				{
					m_sPlayerBullet[i].nLife = 1;
					m_sPlayerBullet[i].vPos = m_sPlayer.vPos;
					m_sPlayerBullet[i].vPos.z += 1.0f;
					break; // �Ѿ��� �ѹ��� �ѹ߾� �߻��Ϸ��� �̷��� break�� �ɾ��־���Ѵ�. �̰� ���ٸ� 10���� ������ �ѹ߸� ������ ������ �Ұ��� �� ���̴�.
				}
			}
		}
	}

	// Note: ���ΰ� �Ѿ� �̵�
	for (i = 0; i < 10; i++)
	{
		if (m_sPlayerBullet[i].nLife > 0)
		{
			m_sPlayerBullet[i].vPos.z += m_dwElapsedTime * m_sPlayerBulletProperty.fBulletVelcoty; // �ð� * �ӷ� = �Ÿ�, ���� ������ ��Ŀ����
			if (m_sPlayerBullet[i].vPos.z >= 20.0f) // ��� ���� �浹
			{
				m_sPlayerBullet[i].nLife = 0; // �浹 �ߴٸ� ü���� 0���� �Ͽ� ��� ��󿡼� ����
			}
			else // �ƴ϶�� �̵��� ��ŭ �̵� ����� ����
			{
				D3DXMatrixTranslation(&m_sPlayerBullet[i].matTranslation, m_sPlayerBullet[i].vPos.x, m_sPlayerBullet[i].vPos.y, m_sPlayerBullet[i].vPos.z);
			}
		}
	}

	D3DXMatrixTranslation(&m_sPlayer.matTranslation, m_sPlayer.vPos.x, m_sPlayer.vPos.y, m_sPlayer.vPos.z); // �̵� ��� ���ϱ�
	m_sPlayer.matWorld = m_sPlayer.matScale * m_sPlayer.matRotationY * m_sPlayer.matTranslation; // SRT, ũ��, ȸ��, �̵� ��� ���� ��

	dwAccumulateTime = dwAccumulateTime + m_dwElapsedTime; // ���� �ð��� ���� �ð��� �׿�����.
	if (dwAccumulateTime >= 1000) // 1�ʰ� �Ǿ����� FPS ���
	{
		dwAccumulateTime = 0; // ���� �ð� �ʱ�ȭ
		m_nFPS = m_nFPSCount; // ���� ���� ������Ų FPS ���� ȭ�鿡 ����ϱ� ���� m_nFPS�� �־��ش�.
		m_nFPSCount = 0; // �ٽ� FPS �ʱ�ȭ
	}
}

void CGameEdu01::OnRelease()
{
	m_pFont->Release();
}