#include "CGameEdu01.h"

string g_strBGSound[] = { "Sound_Dir\\init.ogg", "Sound_Dir\\war.mp3", "Sound_Dir\\gameover.mp3" };
string g_strEffectSound[] = { "Sound_Dir\\story.mp3", "Sound_Dir\\stage.ogg", "Sound_Dir\\play_fire.wav", "Sound_Dir\\enemy_fire.wav", "Sound_Dir\\collision.wav" };

BOOL CheckCubeIntersection(D3DXVECTOR3* vMin1, D3DXVECTOR3* vMax1,
	D3DXVECTOR3* vMin2, D3DXVECTOR3* vMax2) // ť�� �浹 üũ �Լ�, �� ť���� �ִ����� �ּ����� ���Ͽ� ��ġ�� �κ��� ������ �浹�� �Ǵ��Ѵ�.
{
	if (vMin1->x <= vMax2->x && vMax1->x >= vMin2->x &&
		vMin1->y <= vMax2->y && vMax1->y >= vMin2->y &&
		vMin1->z <= vMax2->z && vMax1->z >= vMin2->z)
	{
		return TRUE;
	}
	return FALSE;
}

CGameEdu01::CGameEdu01()
{
	m_nGameState = INIT; // ���� ���´� �׻� ó���� �ʱ�ȭ���� �����̴�.
}

CGameEdu01::~CGameEdu01()
{

}

void CGameEdu01::OnInit()
{
	int i;
	RECT rect;
	D3DVIEWPORT9 vp;

	switch (m_nGameState) // ���� ���� ���¿� ���� ����Ǵ� ������ �޶�����
	{
	case INIT: // ���� ó�� �����ϸ� ����Ǵ� �κ�
		srand((unsigned int)time(NULL)); // ���� ������ ���� �ʱ�ȭ
		GetClientRect(m_hWnd, &rect);

		// ����Ʈ ����
		vp.X = 0;
		vp.Y = 0;
		vp.Width = rect.right - rect.left;  // ��ǥ������ �ƴ϶� ũ�� ����
		vp.Height = rect.bottom - rect.top;
		vp.MinZ = 0.0f;
		vp.MaxZ = 1.0f;

		// ī�޶� ��ġ ����
		m_Eye.x = 0.0f;
		m_Eye.y = 0.0f;
		m_Eye.z = -12.0f; // ���� ���� �� �޴� ȭ���̶� -12�� ����

		// ī�޶� �ٶ󺸴� ��ġ ����
		m_At.x = 0.0f;
		m_At.y = 0.0f;
		m_At.z = 0.0f;

		m_Up.x = 0.0f;
		m_Up.y = 1.0f;
		m_Up.z = 0.0f;

		// ī�޶� ����
		D3DXMatrixLookAtLH(&m_matView, &m_Eye, &m_At, &m_Up);
		m_pd3dDevices->SetTransform(D3DTS_VIEW, &m_matView);

		// 3D ���� ��ȯ ����
		D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
		m_pd3dDevices->SetTransform(D3DTS_PROJECTION, &m_matProj);
		m_pd3dDevices->SetViewport(&vp);

		// Note: �޽� ����
		D3DXCreateTeapot(m_pd3dDevices, &m_pTeapotMesh, NULL);
		D3DXCreateCylinder(m_pd3dDevices, 2.0f, 0.0f, 10.0f, 15, 10, &m_pPlayerBulletMesh, NULL);
		D3DXCreateBox(m_pd3dDevices, 1.0f, 1.0f, 1.0f, &m_pEnemyBoxMesh, NULL);
		D3DXCreateSphere(m_pd3dDevices, 0.1f, 20, 10, &m_pEnemyBulletMesh, NULL);
		m_Ground.Create(m_pd3dDevices, 20, 6, 2.0f);

		// �� ��Ʈ ����
		D3DXCreateFont(m_pd3dDevices, 50, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
			"Arial", &m_pFont);
		D3DXCreateFont(m_pd3dDevices, 30, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
			"Arial", &m_pFont2);
		D3DXCreateFont(m_pd3dDevices, 20, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
			"Arial", &m_pFont3);

		// �Ҹ� ����
		m_FMODSound.CreateBGSound(3, g_strBGSound); // ����� 3��
		m_FMODSound.PlaySoundBG(0); // ���� ����� ù��° ����
		break;

	case LOADING: // �ε� �κ�
		m_FMODSound.StopSoundBG(0); // ù��° ����� ��� ����

		m_FMODSound.CreateEffectSound(5, g_strEffectSound); // ȿ���� 5�� ����
		// ���ΰ��� �ִ� �ּ� ���� �� <= �� ��ǥ
		D3DXVECTOR3* pVertices;
		m_pTeapotMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertices);
		D3DXComputeBoundingBox(pVertices, m_pTeapotMesh->GetNumVertices(), D3DXGetFVFVertexSize(m_pTeapotMesh->GetFVF()), &m_sPlayer.vMin, &m_sPlayer.vMax);
		m_pTeapotMesh->UnlockVertexBuffer();

		// ���ΰ� �̻��� �ִ� �ּ����� ���ϱ�
		m_pPlayerBulletMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertices);
		D3DXComputeBoundingBox(pVertices, m_pPlayerBulletMesh->GetNumVertices(), D3DXGetFVFVertexSize(m_pPlayerBulletMesh->GetFVF()), &m_sPlayerBulletProperty.vMin, &m_sPlayerBulletProperty.vMax);
		m_pPlayerBulletMesh->UnlockVertexBuffer();

		// �� �浹 ��ǥ 	
		m_pEnemyBoxMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertices);
		D3DXComputeBoundingBox(pVertices, m_pEnemyBoxMesh->GetNumVertices(), D3DXGetFVFVertexSize(m_pEnemyBoxMesh->GetFVF()), &m_EnemyProperty.vMin, &m_EnemyProperty.vMax);
		m_pEnemyBoxMesh->UnlockVertexBuffer();

		// �� �Ѿ� 
		m_pEnemyBulletMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertices);
		D3DXComputeBoundingBox(pVertices, m_pEnemyBoxMesh->GetNumVertices(), D3DXGetFVFVertexSize(m_pEnemyBulletMesh->GetFVF()), &m_EnemyBulletProperty.vMin, &m_EnemyBulletProperty.vMax);
		m_pEnemyBulletMesh->UnlockVertexBuffer();
		break;

	case READY: // �غ� �ܰ�
		m_FMODSound.PlaySoundEffect(1); // ȿ���� �ι�° ���

		// ���� ��� ����
		m_Eye.x = 0.0f;
		m_Eye.y = 10.0f;
		m_Eye.z = -32.0f; // ���� ������ �Ǵ� ��� -32�� ����

		m_At.x = 0.0f;
		m_At.y = 0.0f;
		m_At.z = 0.0f;

		m_Up.x = 0.0f;
		m_Up.y = 1.0f;
		m_Up.z = 0.0f;

		// ī�޶� �ٽ� ����
		D3DXMatrixLookAtLH(&m_matView, &m_Eye, &m_At, &m_Up);
		m_pd3dDevices->SetTransform(D3DTS_VIEW, &m_matView);

		// Note: ���ΰ� ����	
		m_sPlayer.nLife = 5; // �÷��̾� ����
		m_sPlayer.fScale = 0.7f;
		m_sPlayer.fRotationY = -D3DXToRadian(90);
		m_sPlayer.vPos = D3DXVECTOR3(0.0f, 0.0f, -9 * 2.0f);
		m_sPlayer.fVelocity = 0.005f;
		m_sPlayer.dwBulletFireTime = 400;
		m_sPlayer.dwOldBulletFireTime = GetTickCount64();
		for (i = 0; i < 10; i++)
		{
			m_sPlayerBullet[i].nLife = 0;
		}

		// ũ��, �̵�, ȸ�� ��ȯ ��� ����
		D3DXMatrixScaling(&m_sPlayer.matScale, m_sPlayer.fScale, m_sPlayer.fScale, m_sPlayer.fScale);
		D3DXMatrixTranslation(&m_sPlayer.matTranslation, m_sPlayer.vPos.x, m_sPlayer.vPos.y, m_sPlayer.vPos.z);
		D3DXMatrixRotationY(&m_sPlayer.matRotationY, m_sPlayer.fRotationY);

		// Note: ���ΰ� �Ѿ� ����
		m_sPlayerBulletProperty.fBulletVelcoty = 0.01f;
		m_sPlayerBulletProperty.fScale = 0.08f;
		D3DXMatrixScaling(&m_sPlayerBulletProperty.matScale, m_sPlayerBulletProperty.fScale, m_sPlayerBulletProperty.fScale, m_sPlayerBulletProperty.fScale);

		// Note: �� ĳ���� ����
		m_EnemyProperty.fScale = 1.0f;
		m_EnemyProperty.fMoveVelcoty = 0.003f;
		m_EnemyProperty.dwFireTime = 1600;
		D3DXMatrixScaling(&m_EnemyProperty.matScale, m_EnemyProperty.fScale, m_EnemyProperty.fScale, m_EnemyProperty.fScale);

		// �� ���� ����
		int nRandomNum;
		for (i = 0; i < EnemyCount; i++)
		{
			m_Enemy[i].nLife = 0;
			m_Enemy[i].dwAppearTime = 2000 + (i * 800);
			m_Enemy[i].dwOldFireTime = GetTickCount64();
			nRandomNum = rand() % 7;
			if (nRandomNum % 2)
				m_Enemy[i].vPos.x = -1.0f * nRandomNum;
			else
				m_Enemy[i].vPos.x = (float)nRandomNum;

			m_Enemy[i].vPos.y = 0.0f;
			m_Enemy[i].vPos.z = 20.0f;

			D3DXMatrixTranslation(&m_Enemy[i].matTranslation, m_Enemy[i].vPos.x, m_Enemy[i].vPos.y, m_Enemy[i].vPos.z);
		}

		// Note: �� �Ѿ� ����
		m_EnemyBulletProperty.fBulletVelcoty = 0.01f;
		m_EnemyBulletProperty.fScale = 0.9f;
		D3DXMatrixScaling(&m_EnemyBulletProperty.matScale, m_EnemyBulletProperty.fScale, m_EnemyBulletProperty.fScale, m_EnemyBulletProperty.fScale);
		for (i = 0; i < 100; i++)
		{
			m_EnemyBullet[i].nLife = 0;
		}

		m_nEnemyIndex = 0; // �� �ε��� �ʱ�ȭ
		m_nGrade = 0; // ���� �ʱ�ȭ
		m_dwGameStartTime = GetTickCount64(); // �ʱ�ȭ�� �������� ���� ���� �ð� ���
		break;
	}
}

// ���� ���� ȭ�鿡 �����ֱ⸸ �ϴ� �κ�, ��¸� ����ϰ� ���� ���� ���� ��Ʈ���� Update���� ó���ϰ� �ȴ�.
// ���� ���� �ܰ�� �ѱ�� ���� ���� �κп��� ������ �ʴ´�.
void CGameEdu01::OnRender() 
{
	int i;
	D3DXMATRIX matWorld, matScale, matTrans, matRotation;
	RECT rt;
	char string[100];

	switch (m_nGameState) // �������� �Ȱ��� ���¿� ���� ������ ó���ϰ� �ȴ�.
	{
	case INIT: // ���� ���� �κ�, �޴��� �ش��Ѵ�.
		// ������ Y�� ȸ���ϸ鼭 ���
		D3DXMatrixScaling(&matScale, 2.0f, 2.0f, 2.0f);
		D3DXMatrixTranslation(&matTrans, 0.0f, -1.0f, 0.0f);
		D3DXMatrixRotationY(&matRotation, GetTickCount64() * 0.001f); // �ð��� ���� �޽� ȸ�� ��Ŵ
		m_pd3dDevices->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pd3dDevices->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		matWorld = matScale * matRotation * matTrans; // SRT ��ȯ
		m_pd3dDevices->SetTransform(D3DTS_WORLD, &matWorld);
		m_pTeapotMesh->DrawSubset(0);

		// �Ѿ� X�� ȸ���ϸ鼭 ���
		D3DXMatrixScaling(&matScale, 0.1f, 0.1f, 0.1f);
		D3DXMatrixTranslation(&matTrans, 0.0f, 1.5f, 0.0f);
		D3DXMatrixRotationX(&matRotation, GetTickCount64() * 0.002f);
		matWorld = matScale * matRotation * matTrans;
		m_pd3dDevices->SetTransform(D3DTS_WORLD, &matWorld);
		m_pPlayerBulletMesh->DrawSubset(0);

		// �ؽ�Ʈ ���� ���� ���
		SetRect(&rt, 360, 130, 0, 0);
		m_pFont->DrawText(NULL, "3D ������ ����", -1, &rt, DT_NOCLIP,
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		SetRect(&rt, 360, 800, 0, 0);
		m_pFont2->DrawText(NULL, "Space Ű�� ���� ���� ����!", -1, &rt, DT_NOCLIP,
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		break;

	case LOADING: // �ε� �κ�
		SetRect(&rt, 150, 430, 0, 0);
		m_pFont->DrawText(NULL, "Loading ...", -1, &rt, DT_NOCLIP,
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		// ������ ä���
		for (i = 0; i < m_nLoadingCount; i++)
		{
			SetRect(&rt, 200 + i * 10, 500, 0, 0);
			m_pFont2->DrawText(NULL, "��", -1, &rt, DT_NOCLIP,
				D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		}
		break;

	case READY: // �غ� �ܰ�
		SetRect(&rt, 400, 250, 0, 0);
		wsprintf(string, "Stage %d", m_nStage);
		m_pFont->DrawText(NULL, string, -1, &rt, DT_NOCLIP,
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		m_nEnemyCount = EnemyCount;
		break;

	case RUN: // ���� ���� ����
		m_Ground.OnRender(); // ���� ���

		m_pd3dDevices->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pd3dDevices->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		// ���ΰ� �Ѿ�
		for (i = 0; i < 10; i++)
		{
			if (m_sPlayerBullet[i].nLife > 0)
			{
				matWorld = m_sPlayerBulletProperty.matScale * m_sPlayerBullet[i].matTranslation;
				m_pd3dDevices->SetTransform(D3DTS_WORLD, &matWorld);
				m_pPlayerBulletMesh->DrawSubset(0);
			}
		}

		// �� �Ѿ� 
		for (i = 0; i < 100; i++)
		{
			if (m_EnemyBullet[i].nLife > 0)
			{
				matWorld = m_EnemyBulletProperty.matScale * m_EnemyBullet[i].matTranslation;
				m_pd3dDevices->SetTransform(D3DTS_WORLD, &matWorld);
				m_pEnemyBulletMesh->DrawSubset(0);
			}
		}

		// �� ĳ����		
		for (i = 0; i < m_nEnemyIndex; i++)
		{
			if (m_Enemy[i].nLife)
			{
				matWorld = m_EnemyProperty.matScale * m_Enemy[i].matTranslation;
				m_pd3dDevices->SetTransform(D3DTS_WORLD, &matWorld);
				m_pEnemyBoxMesh->DrawSubset(0);
			}
		}

		// ���ΰ� 	
		m_pd3dDevices->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); // ���̾� ���������� ���
		m_pd3dDevices->SetTransform(D3DTS_WORLD, &m_sPlayer.matWorld);
		m_pTeapotMesh->DrawSubset(0);
		m_pd3dDevices->SetRenderState(D3DRS_LIGHTING, TRUE);

		SetRect(&rt, 10, 20, 0, 0);
		wsprintf(string, "Total Score : %d   Score :  %d", m_nTotalGrade, m_nGrade);
		m_pFont3->DrawText(NULL, string, -1, &rt, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

		SetRect(&rt, 680, 20, 0, 0);
		wsprintf(string, "%s", "��"); // �÷��̾� ü�� ���
		for (i = 0; i < m_sPlayer.nLife; i++)
		{
			rt.left = 680 + i * 15; // �ѱ��� ũ�Ⱑ 15���� 15�� �и鼭 ���
			m_pFont2->DrawText(NULL, string, -1, &rt, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		}
		break;

	case SUCCESS: // ����
		SetRect(&rt, 400, 130, 0, 0);
		m_pFont->DrawText(NULL, "SUCCESS", -1, &rt, DT_NOCLIP,
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		break;

	case FAILED: // ����
		SetRect(&rt, 360, 230, 0, 0);
		m_pFont2->DrawText(NULL, "�ٽ� �Ͻðڽ��ϱ�? (Yes/No)", -1, &rt, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		break;

	case END: // ���� ����
		SetRect(&rt, 380, 280, 0, 0);
		wsprintf(string, "Total Score : %d", m_nTotalGrade);
		m_pFont2->DrawText(NULL, string, -1, &rt, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		SetRect(&rt, 380, 320, 0, 0);
		wsprintf(string, "Q�� ���� ���� ����");
		m_pFont2->DrawText(NULL, string, -1, &rt, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		break;
	}
}

void CGameEdu01::OnUpdate()
{
	int i, j;
	static DWORD OldTime;
	DWORD dwCurTime = GetTickCount64();
	m_dwElapsedTime = dwCurTime - OldTime; // ��� �ð�
	OldTime = dwCurTime;
	D3DXVECTOR3 vMin1, vMax1, vMin2, vMax2; // �浹�� ���� ����
	D3DXMATRIX matWorld;
	float fTemp;
	static DWORD dwReadyTime; // �غ� ��� �ð��� ������Ű�� ���ؼ�

	// ���� ��� �ð� üũ �κ�
	m_dwGameElapsedTime = dwCurTime - m_dwGameStartTime;

	switch (m_nGameState)
	{
	case INIT: // SPACE Ű�� �Է� �޾� INIT ���¸� LOADING ���� �ٲپ� �ش�.
		if (GetAsyncKeyState(VK_SPACE) < 0)
		{
			m_nGameState = LOADING;
			OnInit(); // �ε� ������ �ʱ�ȭ ����
		}
		break;

	case LOADING: 
		Sleep(100);
		m_nLoadingCount++; // �ε� ī��Ʈ ����
		if (m_nLoadingCount == 50) // �ε� ī���Ͱ� 50�� �Ǹ� READY ���·� �ٲ۴�.
		{
			m_nGameState = READY;
			m_nStage++;
			dwReadyTime = 0;
			OnInit(); // READY ������ �ʱ�ȭ ����
		}
		break;

	case READY:
		dwReadyTime += m_dwElapsedTime; // ��� �ð��� ��� ����, 2�ʵ��� READY ������ �����ֱ� ����
		if (dwReadyTime >= 2000) //3�ʸ� �������� ����
		{
			m_nGameState = RUN; // ���� ���¸� RUN���� ����
			m_FMODSound.PlaySoundBG(1); // �� ���� �����
			dwReadyTime = 0;
		}
		break;

	case RUN: // ���� ���� �κ�, ���� ������ ���⼭ ����ȴ�.
		if (GetAsyncKeyState(VK_LEFT) < 0)
		{
			if (m_sPlayer.vPos.x - (m_dwElapsedTime * m_sPlayer.fVelocity) >= -6.0f)
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

						m_FMODSound.PlaySoundEffect(2); // �߻� ȿ���� ���
						break;
					}
				}
			}
		}

		// �� ĳ���� ����
		for (i = m_nEnemyIndex; i < EnemyCount; i++)
		{
			if (m_Enemy[i].dwAppearTime <= m_dwGameElapsedTime)
			{
				m_Enemy[i].nLife = 1;
				m_nEnemyIndex++;
			}
			else 
			{
				break;
			}
		}

		// �� ĳ���� �̵��� �� ĳ���� �Ѿ� �߻�
		for (i = 0; i < m_nEnemyIndex; i++)
		{
			if (m_Enemy[i].nLife)
			{
				m_Enemy[i].vPos.z -= m_dwElapsedTime * m_EnemyProperty.fMoveVelcoty;
				if (m_Enemy[i].vPos.z <= -20.0f) // ��� ����
				{
					m_Enemy[i].nLife = 0;
					m_nEnemyCount--;    // �� ĳ������ ���� 0 �̵Ǹ� ���� 
					if (m_nEnemyCount == 0)
					{
						m_nGameState = STOP;
					}
					continue;
				}

				D3DXMatrixTranslation(&m_Enemy[i].matTranslation, m_Enemy[i].vPos.x, m_Enemy[i].vPos.y, m_Enemy[i].vPos.z);

				// Note: ��� �ִ� �� ĳ���͸��� �Ѿ��� �߻��� �� �����Ƿ�
				if (dwCurTime - m_Enemy[i].dwOldFireTime >= m_EnemyProperty.dwFireTime)
				{
					m_Enemy[i].dwOldFireTime = dwCurTime;
					for (j = 0; j < 100; j++)
					{
						if (!m_EnemyBullet[j].nLife)
						{
							m_EnemyBullet[j].nLife = 1;
							m_EnemyBullet[j].vPos = m_Enemy[i].vPos;
							m_EnemyBullet[j].vPos.z -= 0.5f; // �ణ �տ��� �Ѿ� �߻簡 �ǵ��� �ϱ� ����									
							break; // <-- �̰� �߿��ϴ�. �Ѿ� �ϳ��� �츮�� �κ��� �� 
						}
					}
				}
			}
		}

		// �� ĳ���� �Ѿ� �̵�
		for (i = 0; i < 100; i++)
		{
			if (m_EnemyBullet[i].nLife > 0)
			{
				m_EnemyBullet[i].vPos.z -= m_dwElapsedTime * m_EnemyBulletProperty.fBulletVelcoty;
				if (m_EnemyBullet[i].vPos.z <= -20.0f) // ��� ���� �浹
					m_EnemyBullet[i].nLife = 0;
				else
					D3DXMatrixTranslation(&m_EnemyBullet[i].matTranslation, m_EnemyBullet[i].vPos.x, m_EnemyBullet[i].vPos.y, m_EnemyBullet[i].vPos.z);
			}
		}

		// Note: ���ΰ� �Ѿ� �̵�
		for (i = 0; i < 10; i++)
		{
			if (m_sPlayerBullet[i].nLife > 0)
			{
				m_sPlayerBullet[i].vPos.z += m_dwElapsedTime * m_sPlayerBulletProperty.fBulletVelcoty;
				if (m_sPlayerBullet[i].vPos.z >= 20.0f) // ��� ���� �浹
					m_sPlayerBullet[i].nLife = 0;
				else
					D3DXMatrixTranslation(&m_sPlayerBullet[i].matTranslation, m_sPlayerBullet[i].vPos.x, m_sPlayerBullet[i].vPos.y, m_sPlayerBullet[i].vPos.z);
			}
		}

		// Note: ���ΰ��� �� ĳ���� �浹
		D3DXMatrixTranslation(&m_sPlayer.matTranslation, m_sPlayer.vPos.x, m_sPlayer.vPos.y, m_sPlayer.vPos.z);

		// ���ΰ��� �浹 �ڽ�
		matWorld = m_sPlayer.matScale * m_sPlayer.matRotationY * m_sPlayer.matTranslation; // SRT ��ȯ
		D3DXVec3TransformCoord(&vMin1, &m_sPlayer.vMin, &matWorld);
		D3DXVec3TransformCoord(&vMax1, &m_sPlayer.vMax, &matWorld);

		if (vMin1.x > vMax1.x)  // ȸ���� �����Ƿ� �ּ��� ���� �����Ѵ�.
		{
			fTemp = vMin1.x;
			vMin1.x = vMax1.x;
			vMax1.x = fTemp;
		}
		if (vMin1.y > vMax1.y)
		{
			fTemp = vMin1.y;
			vMin1.y = vMax1.y;
			vMax1.y = fTemp;
		}
		if (vMin1.z > vMax1.z)
		{
			fTemp = vMin1.z;
			vMin1.z = vMax1.z;
			vMax1.z = fTemp;
		}

		// ���ΰ� ����� �� ���� �浹
		for (i = 0; i < m_nEnemyIndex; i++)
		{
			if (m_Enemy[i].nLife > 0)
			{
				D3DXMatrixTranslation(&m_Enemy[i].matTranslation, m_Enemy[i].vPos.x, m_Enemy[i].vPos.y, m_Enemy[i].vPos.z);
				matWorld = m_EnemyProperty.matScale * m_Enemy[i].matTranslation;

				D3DXVec3TransformCoord(&vMin2, &m_EnemyProperty.vMin, &matWorld);
				D3DXVec3TransformCoord(&vMax2, &m_EnemyProperty.vMax, &matWorld);

				if (CheckCubeIntersection(&vMin1, &vMax1, &vMin2, &vMax2))
				{
					m_Enemy[i].nLife = 0;
					m_sPlayer.nLife--;
					m_nEnemyCount--;    // �� ĳ������ ���� 0 �̵Ǹ� ���� 
					m_nGrade += 10; // ���ΰ��� �浹�ϸ鼭 ���� ���̹Ƿ� ���� ����
					m_FMODSound.PlaySoundEffect(4);

					if (m_sPlayer.nLife == 0)
					{
						m_nGameState = STOP;
					}

					if (m_nEnemyCount == 0)
					{
						m_nGameState = STOP;
					}
				}
			}
		}

		// Note: ���ΰ��� �� �Ѿ� �浹
		for (i = 0; i < 100; i++)
		{
			if (m_EnemyBullet[i].nLife > 0)
			{
				D3DXMatrixTranslation(&m_EnemyBullet[i].matTranslation, m_EnemyBullet[i].vPos.x, m_EnemyBullet[i].vPos.y, m_EnemyBullet[i].vPos.z);
				matWorld = m_EnemyBullet[i].matTranslation;
				D3DXVec3TransformCoord(&vMin2, &m_EnemyBulletProperty.vMin, &matWorld);
				D3DXVec3TransformCoord(&vMax2, &m_EnemyBulletProperty.vMax, &matWorld);

				if (CheckCubeIntersection(&vMin1, &vMax1, &vMin2, &vMax2))
				{
					m_EnemyBullet[i].nLife = 0;
					m_sPlayer.nLife--;
					m_FMODSound.PlaySoundEffect(4);
					if (m_sPlayer.nLife == 0)
					{
						m_nGameState = STOP;
					}
				}
			}
		}

		// Note: ���ΰ� �Ѿ˰� �� ĳ���� �浹
		for (i = 0; i < 10; i++)
		{
			if (m_sPlayerBullet[i].nLife > 0)
			{
				D3DXMatrixTranslation(&m_sPlayerBullet[i].matTranslation, m_sPlayerBullet[i].vPos.x, m_sPlayerBullet[i].vPos.y, m_sPlayerBullet[i].vPos.z);
				matWorld = m_sPlayerBulletProperty.matScale * m_sPlayerBullet[i].matTranslation;
				D3DXVec3TransformCoord(&vMin1, &m_sPlayerBulletProperty.vMin, &matWorld);
				D3DXVec3TransformCoord(&vMax1, &m_sPlayerBulletProperty.vMax, &matWorld);

				// Note: �� ĳ���Ϳ� �浹 üũ
				for (j = 0; j < m_nEnemyIndex; j++)
				{
					if (m_Enemy[j].nLife > 0)
					{
						D3DXMatrixTranslation(&m_Enemy[j].matTranslation, m_Enemy[j].vPos.x, m_Enemy[j].vPos.y, m_Enemy[j].vPos.z);
						matWorld = m_EnemyProperty.matScale * m_Enemy[j].matTranslation;

						D3DXVec3TransformCoord(&vMin2, &m_EnemyProperty.vMin, &matWorld);
						D3DXVec3TransformCoord(&vMax2, &m_EnemyProperty.vMax, &matWorld);

						if (CheckCubeIntersection(&vMin1, &vMax1, &vMin2, &vMax2))
						{
							m_Enemy[j].nLife = 0;
							m_sPlayerBullet[i].nLife = 0;
							m_nGrade += 10; // ���� ����

							m_nEnemyCount--;    // �� ĳ������ ���� 0 �̵Ǹ� ���� 
							m_FMODSound.PlaySoundEffect(4);
							if (m_nEnemyCount == 0)
							{
								m_nGameState = STOP;
							}
							break;
						}
					}
				}

				// Note: ���ΰ� �Ѿ˰� �� �Ѿ� �浹 üũ
				if (m_sPlayerBullet[i].nLife > 0)
				{
					for (j = 0; j < 100; j++)
					{
						if (m_EnemyBullet[j].nLife > 0)
						{
							D3DXMatrixTranslation(&m_EnemyBullet[j].matTranslation, m_EnemyBullet[j].vPos.x, m_EnemyBullet[j].vPos.y, m_EnemyBullet[j].vPos.z);
							matWorld = m_EnemyBullet[j].matTranslation;
							D3DXVec3TransformCoord(&vMin2, &m_EnemyBulletProperty.vMin, &matWorld);
							D3DXVec3TransformCoord(&vMax2, &m_EnemyBulletProperty.vMax, &matWorld);

							if (CheckCubeIntersection(&vMin1, &vMax1, &vMin2, &vMax2))
							{
								m_EnemyBullet[j].nLife = 0;
								m_sPlayerBullet[i].nLife = 0;
								m_FMODSound.PlaySoundEffect(4);
								break;
							}
						}
					}
				}
			}
		}

		D3DXMatrixTranslation(&m_sPlayer.matTranslation, m_sPlayer.vPos.x, m_sPlayer.vPos.y, m_sPlayer.vPos.z);
		m_sPlayer.matWorld = m_sPlayer.matScale * m_sPlayer.matRotationY * m_sPlayer.matTranslation;
		break;

	case STOP:
		m_FMODSound.StopSoundBG(1); // ����� ��� ����
		if (m_sPlayer.nLife <= 0 || m_nEnemyCount > 0) // �÷��̾� ü���� 0 �����ε� �� ���� ���������� ����
		{
			m_nGameState = FAILED;
			m_FMODSound.PlaySoundBG(2); // ���� ȿ����
		}
		else 
		{
			m_nGameState = SUCCESS;
		}

		dwReadyTime = 0; // � �ܰ��� 2�� ���� �ӹ����� ���ִ� ���� �ٽ� �ʱ�ȭ
		break;

	case SUCCESS:
		dwReadyTime += m_dwElapsedTime;
		if (dwReadyTime >= 3000) //3�ʸ� �������� ����
		{
			m_nGameState = READY; // �ٽ� �غ� ���·� �ǵ���
			m_nStage++;
			m_nTotalGrade += m_nGrade;
			dwReadyTime = 0;
			OnInit();
		}
		break;

	case FAILED:
		if (GetAsyncKeyState('Y') < 0)
		{
			m_nGameState = READY;
			m_nGrade = 0;
			m_FMODSound.StopSoundBG(2);
			dwReadyTime = 0;
			OnInit();
			break;
		}
		if (GetAsyncKeyState('N') < 0)
		{
			m_nGameState = END; // ���� ����
			dwReadyTime = 0;
			m_nTotalGrade += m_nGrade;
			m_FMODSound.StopSoundBG(2);
			break;
		}
		break;

	case END:
		if (GetAsyncKeyState('Q') < 0)
		{
			Cleanup();
			PostQuitMessage(0);
		}
		break;
	}

	m_FMODSound.Update();
}

void CGameEdu01::OnRelease()
{
	m_FMODSound.ReleaseSound();
	m_pFont->Release();
	m_pFont2->Release();
	m_pFont3->Release();
	m_pEnemyBoxMesh->Release();
	m_pEnemyBulletMesh->Release();
	m_pTeapotMesh->Release();
	m_pPlayerBulletMesh->Release();
	m_Ground.OnRelease();
}