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
	m_Eye.y = 3.0f;
	m_Eye.z = -10.0f;

	m_At.x = 0.0f; m_At.y = 0.0f; m_At.z = 0.0f;

	m_Up.x = 0.0f; m_Up.y = 1.0f; m_Up.z = 0.0f;

	D3DXMatrixLookAtLH(&m_matView, &m_Eye, &m_At, &m_Up); // �� ��ȯ ��� ����
	m_pd3dDevices->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f); // �������� ��ȯ ��� ���� (����)
	m_pd3dDevices->SetTransform(D3DTS_PROJECTION, &m_matProj);
	m_pd3dDevices->SetViewport(&vp); // �� ��Ʈ ����

	// DX������ ���� ���� ���� ������ �迭�� ���� ��� LPD3DXBUFFER �̷��� ���� ������ ���¸� ����Ѵ�.
	LPD3DXBUFFER pD3DXMtrlBuffer; // �̷��� ���� ���¿� ���� �̸� �˸°� ����Ѵ�. 

	D3DXLoadMeshFromX("Tiger.x", D3DXMESH_SYSTEMMEM, m_pd3dDevices, NULL,
		&pD3DXMtrlBuffer, NULL, &m_dwNumMaterials, &m_pMesh); // .x ���Ϸκ��� �ʿ��� ����(�ؽ���[tu, tv], ���� �޽�, ���͸���)�� ��ƿ��� �Ľ� �Լ�

	// GetBufferPointer()�Լ��� ������ ���۵Ǵ� �ּҸ� ���̵� ������ ������ ��ȯ�Ѵ�.
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer(); // ���̵� ������ ���̱⿡ �˸°� �� ��ȯ�� ���ش�.
	m_pMeshMaterials = new D3DMATERIAL9[m_dwNumMaterials]; // �Ľ̿��� �̾ƿ� ������ŭ ���͸��� �Ҵ�
	m_pMeshTextures = new LPDIRECT3DTEXTURE9[m_dwNumMaterials]; // ���͸��� ����ŭ �ؽ��� �Ҵ�

	// ���͸��� �� ��ŭ �ؽ��� �ε�
	for (DWORD i = 0; i < m_dwNumMaterials; i++)
	{
		m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

		// Set the ambient color for the material (D3DX does not do this) Ambient�� ������� �ʾƼ� ���� ���� ���� ����
		m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;

		m_pMeshTextures[i] = NULL; // �ؽ��� �ʱ�ȭ
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			strlen(d3dxMaterials[i].pTextureFilename) > 0) // �ؽ��� ������ �����ϸ�
		{
			D3DXCreateTextureFromFile(m_pd3dDevices, d3dxMaterials[i].pTextureFilename,
				&m_pMeshTextures[i]); // �ؽ��� ���� ����, bmp���ϰ� x������ �Բ� �־�� �Ѵ�.
		}
	}

	pD3DXMtrlBuffer->Release();  // ��Ƽ���� ���� ����

	m_pd3dDevices->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pd3dDevices->SetRenderState(D3DRS_AMBIENT, 0xffffffff); // ��� �ֺ���
	m_pd3dDevices->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void CGameEdu01::OnRender()
{
	// ī�޶� ��� �缳��
	D3DXMatrixLookAtLH(&m_matView, &m_Eye, &m_At, &m_Up);
	m_pd3dDevices->SetTransform(D3DTS_VIEW, &m_matView);
	
	for (DWORD i = 0; i < m_dwNumMaterials; i++) // ���͸���� �׿� �ش��ϴ� �ؽ��ĵ� ��� ����
	{
		m_pd3dDevices->SetMaterial(&m_pMeshMaterials[i]);
		m_pd3dDevices->SetTexture(0, m_pMeshTextures[i]);

		m_pMesh->DrawSubset(i); // �ش� �Ǵ� �ε����� �޽��� �׸���.
	}
}

void CGameEdu01::OnUpdate()
{
	// ī�޶� ����
	if (GetAsyncKeyState('A') < 0)
	{
		m_Eye.x = m_Eye.x - 0.3;
	}
	if (GetAsyncKeyState('D') < 0)
	{
		m_Eye.x = m_Eye.x + 0.3;
	}
	if (GetAsyncKeyState('W') < 0)
	{
		m_Eye.y = m_Eye.y + 0.3;
	}
	if (GetAsyncKeyState('S') < 0)
	{
		m_Eye.y = m_Eye.y - 0.3;
	}
	if (GetAsyncKeyState('Q') < 0)
	{
		m_Eye.z = m_Eye.z + 0.3;
	}
	if (GetAsyncKeyState('E') < 0)
	{
		m_Eye.z = m_Eye.z - 0.3;
	}
}

void CGameEdu01::OnRelease()
{
	if (m_pMeshMaterials != NULL)
	{
		delete[] m_pMeshMaterials;
	}

	if (m_pMeshTextures) // �ؽ��Ĵ� �� �������� ���� ���� ���־�� �Ѵ�.
	{
		for (DWORD i = 0; i < m_dwNumMaterials; i++)
		{
			if (m_pMeshTextures[i])
			{
				m_pMeshTextures[i]->Release();
			}
		}
		delete[] m_pMeshTextures; // ���� �Ҵ� �Ǿ��� �迭 ����
	}

	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
	}
}