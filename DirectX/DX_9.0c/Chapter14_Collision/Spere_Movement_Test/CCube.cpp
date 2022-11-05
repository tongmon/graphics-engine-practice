#include "CCube.h"
#include <d3dx9math.h>

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

struct CUSTOMVERTEX
{
	D3DXVECTOR3 position; // The transformed position for the vertex.
	DWORD color;        // The vertex color.
};

CCube::CCube(void)
{
	m_pVB = NULL;
	m_pIB = NULL;
}

CCube::~CCube(void)
{
}

void CCube::OnInit(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;

	// ������ü ���� 8�� ����
	CUSTOMVERTEX Vertices[] = {
		{ D3DXVECTOR3(-1.0f, 1.0f,  1.0f) ,  D3DCOLOR_RGBA(255, 0, 255, 255) },
		{ D3DXVECTOR3(1.0f, 1.0f,  1.0f)  , D3DCOLOR_RGBA(0, 255, 0, 255) },
		{ D3DXVECTOR3(1.0f, 1.0f, -1.0f)  , D3DCOLOR_RGBA(0, 0, 255, 255) },
		{ D3DXVECTOR3(-1.0f, 1.0f, -1.0f)  , D3DCOLOR_RGBA(255, 255, 0, 255) },

		{ D3DXVECTOR3(-1.0f, -1.0f,  1.0f) , D3DCOLOR_RGBA(255, 0, 0, 255) },
		{ D3DXVECTOR3(1.0f, -1.0f,  1.0f) , D3DCOLOR_RGBA(0, 255, 10, 255) },
		{ D3DXVECTOR3(1.0f, -1.0f, -1.0f) , D3DCOLOR_RGBA(0, 0, 255, 255) },
		{ D3DXVECTOR3(-1.0f, -1.0f, -1.0f) , D3DCOLOR_RGBA(255, 0, 0, 255) }
	};

	// Note: �ε��� ����, �� Vertices���� ������ �������� �ð�������� �ﰢ�� ������ ���� �ε������� �����Ѵ�.
	// ������ü�� �簢���� 6���̰� �簢�� 1���� �ﰢ�� 2���� �̷�����⿡ �ε����� 12����.
	// �ε����� ���� ������ �������� ����� �ϰ� �� �ÿ� �ﰢ�� �� �� ��¿� ���� 3���� �ʿ��ѵ� �̰�
	// ������ü ��½ÿ� �ߺ��� �Ǹ鼭 36���� Ŀ���� �ȴ�.
	// �̸� �ε��� ���۸� ����ϸ� 12���� ���� ���� �ֱ⿡ �ε��� ���۸� ����Ѵ�.
	WORD Indices[] =
	{
			0, 1, 2,
			0, 2, 3,
			4, 6, 5,
			4, 7, 6,
			0, 3, 7,
			0, 7, 4,
			1, 5, 6,
			1, 6, 2,
			3, 2, 6,
			3, 6, 7,
			0, 4, 5,
			0, 5, 1
	};

	// ���� ���� ���� �� ����
	m_pd3dDevice->CreateVertexBuffer(sizeof(Vertices), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pVB, NULL);

	void* pVertices;
	m_pVB->Lock(0, sizeof(Vertices), &pVertices, 0);
	memcpy(pVertices, Vertices, sizeof(Vertices));
	m_pVB->Unlock();

	// �ε��� ���� ���� �� ����	
	m_pd3dDevice->CreateIndexBuffer(sizeof(Indices), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, NULL);

	void* pIndices;
	m_pIB->Lock(0, sizeof(Indices), &pIndices, 0);
	memcpy(pIndices, Indices, sizeof(Indices));
	m_pIB->Unlock();
}

void CCube::OnRender()
{
	// ���̾� ������ ��� ���, ������ ���븸 ���δ�. 
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	// �ø��� �� �κ��� ���� ȭ�鿡 ��µ��� �ʴµ� �̸� �����ϰ� �� ������ִ� ���
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	// �״�� ����ϱ� ���� ������� ���
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	// �鿡 ���� ���� ���͸� ������ �ʾұ� ������ ����Ʈ�� �� ���� ������ ����Ѵ�.
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���� ��ǥ�� ���� ��ü ����
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	// ���� ���ۿ� �ִ� ������ ��� ����
	m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEX));
	m_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

	// ���� �ε����� ���ۿ� ���������� ��� ������ ���ش�.
	m_pd3dDevice->SetIndices(m_pIB);
	m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 6 * 2); // sizeof( Indices ) / sizeof(WORD) / 3

	// �������� �������
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// �ø� ��� ���� �������
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CCube::OnRelease()
{
	if (m_pVB != NULL)
		m_pVB->Release();

	if (m_pIB != NULL)
		m_pIB->Release();
}