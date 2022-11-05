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
	// ���� �Ǵ� �ڽ� �޽� ����
	D3DXCreateBox(m_pd3dDevices, 1.0f, 1.0f, 1.0f, &m_pEnemyBoxMesh, NULL);
	// ���� �Ѿ��� �� ��(����) �޽� ����
	D3DXCreateSphere(m_pd3dDevices, 0.1f, 20, 10, &m_pEnemyBulletMesh, NULL);
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
	// ��� �� �Ѿ˵��� ��� ü���� ����.
	for (int i = 0; i < 10; i++)
	{
		m_sPlayerBullet[i].nLife = 0;
	}

	D3DXMatrixScaling(&m_sPlayer.matScale, m_sPlayer.fScale, m_sPlayer.fScale, m_sPlayer.fScale); // ĳ������ �����ϸ� ��� ����
	D3DXMatrixTranslation(&m_sPlayer.matTranslation, m_sPlayer.vPos.x, m_sPlayer.vPos.y, m_sPlayer.vPos.z); // ĳ������ �̵� ��� ����
	D3DXMatrixRotationY(&m_sPlayer.matRotationY, m_sPlayer.fRotationY); // ĳ������ ȸ�� ��� ����

	// Note: ���ΰ� �Ѿ� ����
	m_sPlayerBulletProperty.fBulletVelcoty = 0.01f;
	m_sPlayerBulletProperty.fScale = 0.08f;
	D3DXMatrixScaling(&m_sPlayerBulletProperty.matScale, m_sPlayerBulletProperty.fScale, m_sPlayerBulletProperty.fScale, m_sPlayerBulletProperty.fScale); // �Ѿ� �������� ���

	// Note: �� ĳ���� Ư�� ����
	m_EnemyProperty.fScale = 1.0f;
	m_EnemyProperty.fMoveVelcoty = 0.003f;
	m_EnemyProperty.dwFireTime = 1200; // �� 1.2�ʸ��� ������ �Ѿ��� �߻��Ѵ�.
	D3DXMatrixScaling(&m_EnemyProperty.matScale, m_EnemyProperty.fScale, m_EnemyProperty.fScale, m_EnemyProperty.fScale);

	int nRandomNum; // ������
	for (int i = 0; i < 100; i++)
	{
		m_Enemy[i].nLife = 0; // �ʱ� ������ �������� �ʾƾ� �ϱ⿡ ü���� 0�̴�.
		m_Enemy[i].dwAppearTime = 2000 + (i * 800); // 2�� ���� �� ���� ������������ ������ ������ �����ϰ� �ȴ�.
		m_Enemy[i].dwOldFireTime = GetTickCount64();
		nRandomNum = rand() % 7; // x�� �¿�� 7ĭ
		if (nRandomNum % 2)
		{
			m_Enemy[i].vPos.x = -1.0f * nRandomNum; // z������ ���� ���� �κ�
		}
		else
		{
			m_Enemy[i].vPos.x = (float)nRandomNum; // z������ ���� ���� �κ�
		}

		m_Enemy[i].vPos.y = 0.0f;
		m_Enemy[i].vPos.z = 20.0f; // �÷��̾�� �ݴ� �κп� ����

		D3DXMatrixTranslation(&m_Enemy[i].matTranslation, m_Enemy[i].vPos.x, m_Enemy[i].vPos.y, m_Enemy[i].vPos.z); // ���� ���� ������ ��ǥ�� �̵� ��� ����
	}

	// Note: �� �Ѿ� ����
	m_EnemyBulletProperty.fBulletVelcoty = 0.01f;
	m_EnemyBulletProperty.fScale = 0.9f;
	D3DXMatrixScaling(&m_EnemyBulletProperty.matScale, m_EnemyBulletProperty.fScale, m_EnemyBulletProperty.fScale, m_EnemyBulletProperty.fScale); // �Ѿ� ũ�� ��� ����
	// �������� ���� �� �Ѿ��� ��� ü���� 0�̴�.
	// �޸� Ǯ ������� �̸� �޸𸮸� ���� �س��� ����ϴ� �͸� ��� ����ϰ� �ȴ�. ���� �Ѿ��� �ſ� �ſ� �������Ƿ� �Ҵ�, ������ ������带 �ٿ��� �Ѵ�. 
	for (int i = 0; i < 100; i++)
	{
		m_EnemyBullet[i].nLife = 0;
	}

	m_sPlayer.nLife = 3; // �÷��̾� ü���� 3���� �����Ѵ�.

	srand((unsigned int)time(NULL)); // ���� ������ ���� �ʱ�ȭ

	// ��Ʈ ����
	D3DXCreateFont(m_pd3dDevices, 15, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "����ü", &m_pFont);

	m_dwGameStartTime = GetTickCount64(); // ���� �ʱ� ���� �κ��� ��� ���� �Ŀ� ���� ���� �ð��� ����.
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

	// �� �Ѿ� 
	for (int i = 0; i < 100; i++)
	{
		if (m_EnemyBullet[i].nLife > 0)
		{
			matWorld = m_EnemyBulletProperty.matScale * m_EnemyBullet[i].matTranslation; // �Ѿ��� ȸ���� �ʿ���� ST�� ���ش�.
			m_pd3dDevices->SetTransform(D3DTS_WORLD, &matWorld);
			m_pEnemyBulletMesh->DrawSubset(0);
		}
	}

	// �� ĳ����		
	for (int i = 0; i < m_nEnemyIndex; i++)
	{
		if (m_Enemy[i].nLife)
		{
			matWorld = m_EnemyProperty.matScale * m_Enemy[i].matTranslation; // ���� ȸ���� ������ ST ��ȯ�� ����
			m_pd3dDevices->SetTransform(D3DTS_WORLD, &matWorld);
			m_pEnemyBoxMesh->DrawSubset(0);
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
	int i, j; // �ݺ����� ���� ����
	DWORD dwCurTime = GetTickCount64(); // ���� �ð��� ���´�
	static DWORD dwOldTime = GetTickCount64(); // ���� ������ ���� �ð��� �ѹ��� �ʱ�ȭ�ϰ� �� ���� ȣ��� ���� �ð��� �ȴ�.
	static DWORD dwAccumulateTime = 0; // ���� �ð�
	m_dwElapsedTime = dwCurTime - dwOldTime; // ���� �ð��� ���� �ð��� ���̸� ���� �ð��� �־��ش�. (�� ������)
	dwOldTime = dwCurTime; // ���� �ð��� ���� ȣ��� ���� �ð��� �ȴ�.
	D3DXMATRIX matWorld;

	// ���� ��� �ð� üũ �κ�
	// ���� �ð����� ������ ���۵Ǵ� �ð��� ���ָ� ������ �󸶳� �귶���� �Ǵ��� �����ϴ�.
	m_dwGameElapsedTime = dwCurTime - m_dwGameStartTime; // �� �ð��� �������� ���� ���� �������� �����ǰ� �ȴ�. �� ������ ����ð��� ū ������ �������� ���Ѵ�.

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

	if (GetAsyncKeyState('S') < 0)  // ���ΰ� �Ѿ� �߻�
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

	// �� ĳ���� ����
	for (i = m_nEnemyIndex; i < 100; i++)
	{
		if (m_Enemy[i].dwAppearTime <= m_dwGameElapsedTime) // ������ ���� �귯�� �ð����� ����ð��� ������ �� ���� �����ؾ� �Ѵ�.
		{
			// ���� ��� ������ ���۵��� 15�ʰ� �Ǿ��µ� ���� �ð��� 2��, 3��... ������ ������ �ִ� ������ ������ �ؾߵ� ���̴�.
			m_Enemy[i].nLife = 1;
			m_nEnemyIndex++; // ���� �̹� ���������Ƿ� ���� Ž������ �� Ž���� �ʿ䰡 ���⿡ �ε����� �÷��ش�.
		}
		else // ������ �귯�� �ð����� ����ð��� �� ū ������ ������ �ʿ䰡 ����. ���������̱⿡ break�� ���������ش�.
		{
			break;
		}
	}

	// �� ĳ���� �̵��� �� ĳ���� �Ѿ� �߻�, ������ �ִ� �� ĳ���Ϳ� ���ؼ� �˻�
	for (i = 0; i < m_nEnemyIndex; i++)
	{
		if (m_Enemy[i].nLife) // ���� ����ִ°�?
		{
			m_Enemy[i].vPos.z -= m_dwElapsedTime * m_EnemyProperty.fMoveVelcoty; // ���� ������ �´�. �� �����θ� ���� ���� �����̶�� �� �� �ִ�.
			if (m_Enemy[i].vPos.z <= -20.0f) // ��� ����
			{
				m_Enemy[i].nLife = 0;
				continue; // ��� �������� �װԵ� ���� ���� ó������ ������ �ʿ䰡 �����Ƿ� continue�� ���� for���� i++�� �ٷ� �̵�
			}

			D3DXMatrixTranslation(&m_Enemy[i].matTranslation, m_Enemy[i].vPos.x, m_Enemy[i].vPos.y, m_Enemy[i].vPos.z); // �̵� ��� ����

			// Note: ��� �ִ� �� ĳ���͸��� �Ѿ��� �߻��� �� �ִ�. ĳ������ �Ѿ� �߻�� ��Ŀ������ ����.
			if (dwCurTime - m_Enemy[i].dwOldFireTime >= m_EnemyProperty.dwFireTime)
			{
				m_Enemy[i].dwOldFireTime = dwCurTime;
				for (j = 0; j < 100; j++)
				{
					if (!m_EnemyBullet[j].nLife) // �Ѿ��� ���� ���¸�
					{
						m_EnemyBullet[j].nLife = 1; // �츰��.
						m_EnemyBullet[j].vPos = m_Enemy[i].vPos; // �����κ��� �Ѿ��� ������ �ϴ� �� ��ġ�� ���´�.
						m_EnemyBullet[j].vPos.z -= 0.5f; // �ణ �տ��� �̻��� �߻簡 �ǵ��� �ϱ� ����									
						break; // <-- �̰� �߿��ϴ�. �Ѿ� �ϳ��� �츮�� �κ��� �� 
					}
				}
			}
		}
	}

	// �� ĳ���� �Ѿ� �̵�
	for (i = 0; i < 100; i++)
	{
		if (m_EnemyBullet[i].nLife > 0) // �Ѿ��� ����ִٸ� �����δ�.
		{
			m_EnemyBullet[i].vPos.z -= m_dwElapsedTime * m_EnemyBulletProperty.fBulletVelcoty;
			if (m_EnemyBullet[i].vPos.z <= -20.0f) // ��� ���� �浹
			{
				m_EnemyBullet[i].nLife = 0;
			}
			else
			{
				D3DXMatrixTranslation(&m_EnemyBullet[i].matTranslation, m_EnemyBullet[i].vPos.x, m_EnemyBullet[i].vPos.y, m_EnemyBullet[i].vPos.z);
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