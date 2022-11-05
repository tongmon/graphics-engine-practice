#include "CD3DApp.h"

CD3DApp::CD3DApp()
{
    m_pD3D = NULL;
    m_pd3dDevices = NULL;
    m_hWnd = NULL;
}

CD3DApp::~CD3DApp()
{

}

HRESULT CD3DApp::D3DInit(HWND hWnd)
{
    m_hWnd = hWnd;
    m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (m_pD3D == NULL)
    {
        return E_FAIL;
    }
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp, &m_pd3dDevices)))
    {
        return E_FAIL;
    }
    OnInit();
    return S_OK;
}

void CD3DApp::Render()
{
    if (m_pd3dDevices == NULL)
    {
        return;
    }
    m_pd3dDevices->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
    if (SUCCEEDED(m_pd3dDevices->BeginScene()))
    {
        // � ������ �������ϴ� ��Ұ� �̰��� ��ġ�� �� �ִ�.
        OnRender();

        m_pd3dDevices->EndScene();
    }
    m_pd3dDevices->Present(NULL, NULL, NULL, NULL);
}

void CD3DApp::Cleanup()
{
    OnRelease();
    if (m_pd3dDevices != NULL)
    {
        m_pd3dDevices->Release();
    }
    if (m_pD3D != NULL)
    {
        m_pD3D->Release();
    }
}

void CD3DApp::Update()
{
    OnUpdate();
}