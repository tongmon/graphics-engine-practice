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

	m_Eye.x = 40.0f;
	m_Eye.y = 30.0f;
	m_Eye.z = -10.0f;

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
}

void CGameEdu01::OnRender()
{
	char string[100];
	RECT rt; // ���ڸ� ��ġ��Ű�� ���� �簢��

	SetRect(&rt, 100, 100, 0, 0);
	m_pFont2->DrawText(NULL, "3D ���� ���α׷���", -1, &rt, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

	SetRect(&rt, 100, 300, 0, 0);
	m_pFont2->DrawText(NULL, "3D Game Programming", -1, &rt, DT_NOCLIP, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

	SetRect(&rt, 300, 400, 0, 0);
	m_pFont->DrawText(NULL, "����Ŀ�´����̼�", -1, &rt, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

	SetRect(&rt, 10, 10, 0, 0);
	wsprintf(string, "FPS: %d", m_nFPS);
	m_pFont->DrawText(NULL, string, -1, &rt, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)); // ������ RGB �׸��� ������ ��ġ�Ѵ�.
	
	m_nFPSCount++; // 1�ʰ� �Ǳ� ������ FPS�� ��� �������� �ش�.
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
}

void CGameEdu01::OnRelease()
{
	m_pFont->Release();
	m_pFont2->Release();
}