#include "CGameEdu01.h"

CGameEdu01::CGameEdu01()
{
	m_cordX = m_cordY = m_cordZ = 0.0f;
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
	m_Eye.y = 80.0f;
	m_Eye.z = -0.00001f;

	m_At.x = 0.0f; m_At.y = 0.0f; m_At.z = 0.0f;

	m_Up.x = 0.0f; m_Up.y = 1.0f; m_Up.z = 0.0f;

	D3DXMatrixLookAtLH(&m_matView, &m_Eye, &m_At, &m_Up); // �� ��ȯ ��� ����
	m_pd3dDevices->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f); // �������� ��ȯ ��� ���� (����)
	m_pd3dDevices->SetTransform(D3DTS_PROJECTION, &m_matProj);
	m_pd3dDevices->SetViewport(&vp); // �� ��Ʈ ����

	D3DXCreateFont(m_pd3dDevices, 20, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		"System", &m_pFont); // ��Ʈ ���� 1

	D3DXCreateFont(m_pd3dDevices, 30, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		"����ü", &m_pFont2); // ��Ʈ ���� 2

	D3DXCreateSphere(m_pd3dDevices, 3.0f, 30, 10, &m_Circle_Coltest, NULL); // ��

	m_Axis.OnInit(m_pd3dDevices);
}

void CGameEdu01::OnRender()
{
	m_pd3dDevices->SetRenderState(D3DRS_LIGHTING, FALSE); // ����Ʈ ���ָ� �Ⱥ��̱⿡ �ϴ� ����
	m_pd3dDevices->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); // ���븸 ���̱�
	m_pd3dDevices->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); // �ø� ����, �޸� ���� ����

	m_Axis.OnRender();
	
	char string[100];
	RECT rt; // ���ڸ� ��ġ��Ű�� ���� �簢��

	SetRect(&rt, 10, 10, 0, 0);
	wsprintf(string, "FPS: %d", m_nFPS);
	m_pFont->DrawText(NULL, string, -1, &rt, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)); // ������ RGB �׸��� ������ ��ġ�Ѵ�.
	
	m_nFPSCount++; // 1�ʰ� �Ǳ� ������ FPS�� ��� �������� �ش�.


	D3DXMatrixTranslation(&m_Movement, m_cordX, m_cordY, m_cordZ);
	m_pd3dDevices->SetTransform(D3DTS_WORLD, &m_Movement); // ����, �� ��ü ��ǥ�� �����ϸ��ؼ� ��ġ��Ų��.
	m_Circle_Coltest->DrawSubset(0);
	m_pd3dDevices->SetRenderState(D3DRS_LIGHTING, TRUE); // ����Ʈ ���ָ� �Ⱥ��̱⿡ �ϴ� ����
}

void CGameEdu01::OnUpdate()
{
	DWORD dwCurTime = GetTickCount64(); // ���� �ð��� ���´�
	static DWORD dwOldTime = GetTickCount64(); // ���� ������ ���� �ð��� �ѹ��� �ʱ�ȭ�ϰ� �� ���� ȣ��� ���� �ð��� �ȴ�.
	static DWORD dwAccumulateTime = 0; // ���� �ð�
	m_dwElapsedTime = dwCurTime - dwOldTime; // ���� �ð��� ���� �ð��� ���̸� ���� �ð��� �־��ش�.
	dwOldTime = dwCurTime; // ���� �ð��� ���� ȣ��� ���� �ð��� �ȴ�.

	dwAccumulateTime = dwAccumulateTime + m_dwElapsedTime; // ���� �ð��� ���� �ð��� �׿�����.
	if (dwAccumulateTime >= 1000) // 1�ʰ� �Ǿ����� FPS ���
	{
		dwAccumulateTime = 0; // ���� �ð� �ʱ�ȭ
		m_nFPS = m_nFPSCount; // ���� ���� ������Ų FPS ���� ȭ�鿡 ����ϱ� ���� m_nFPS�� �־��ش�.
		m_nFPSCount = 0; // �ٽ� FPS �ʱ�ȭ
	}

	if (GetAsyncKeyState('A') < 0)
	{
		m_cordX = m_cordX - 1.0f;
	}
	if (GetAsyncKeyState('D') < 0)
	{
		m_cordX = m_cordX + 1.0f;
	}
	if (GetAsyncKeyState('W') < 0)
	{
		m_cordZ = m_cordZ + 1.0f;
	}
	if (GetAsyncKeyState('S') < 0)
	{
		m_cordZ = m_cordZ - 1.0f;
	}
}

void CGameEdu01::OnRelease()
{
	m_pFont->Release();
	m_pFont2->Release();

	m_Axis.OnRelease();
	m_Circle_Coltest->Release();
}