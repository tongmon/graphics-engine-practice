//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;

struct Vertex // ���� ����
{
    XMFLOAT3 Pos; // ��ġ
    XMFLOAT4 Color; // ����
};

Game::Game() noexcept(false) : mBoxVB(0), mBoxIB(0), mFX(0), mTech(0), mfxWorldViewProj(0), mInputLayout(0)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);

    mLastMousePos.x = 0;
    mLastMousePos.y = 0;

    mTheta = 1.5f * XM_PI;
    mPhi = 0.25f * XM_PI;
    mRadius = 5.0f;

    // XMStoreFloat4x4 ---> XMMATRIX ���� XMFloat4x4 ���� ����
    XMMATRIX I = XMMatrixIdentity(); // ���� ��ķ� �ʱ�ȭ
    XMStoreFloat4x4(&mWorld, I);
    XMStoreFloat4x4(&mView, I);
    XMStoreFloat4x4(&mProj, I);
}

Game::~Game()
{
    // Com ��ü
    mFX->Release();
    mTech->Release();
    mfxWorldViewProj->Release();
}

// ���۽ÿ� Direct3D �ڿ� �ʱ�ȭ
void Game::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    BuildGeometryBuffers(); // ���� ����
    BuildFX(); // FX ���� �о ���̵�
    BuildVertexLayout(); // �Է� ��ġ ���� �Լ�

    // TODO: Ÿ�̸� ������ ���ϸ� �ٲ� �� �ִ�.
    // ���� ���ô� 60 ������ �������� �����ϴ� ����̴�.
    /*
    m_timer.SetFixedTimeStep(true); // ���� ������ ���
    m_timer.SetTargetElapsedSeconds(1.0 / 60); // 60 ������ ����
    */
}

void Game::BuildGeometryBuffers() // ���� ���� �Լ�
{
    auto device = m_deviceResources->GetD3DDevice();

    XMFLOAT4 Col[8];
    XMStoreFloat4(&Col[0], Colors::White); XMStoreFloat4(&Col[1], Colors::Black);
    XMStoreFloat4(&Col[2], Colors::Red); XMStoreFloat4(&Col[3], Colors::Green);
    XMStoreFloat4(&Col[4], Colors::Blue); XMStoreFloat4(&Col[5], Colors::Yellow);
    XMStoreFloat4(&Col[6], Colors::Cyan); XMStoreFloat4(&Col[7], Colors::Magenta);

    // ������ ����
    Vertex vertices[] =
    {
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), Col[0] },
        { XMFLOAT3(-1.0f, +1.0f, -1.0f), Col[1] },
        { XMFLOAT3(+1.0f, +1.0f, -1.0f), Col[2] },
        { XMFLOAT3(+1.0f, -1.0f, -1.0f), Col[3] },
        { XMFLOAT3(-1.0f, -1.0f, +1.0f), Col[4] },
        { XMFLOAT3(-1.0f, +1.0f, +1.0f), Col[5] },
        { XMFLOAT3(+1.0f, +1.0f, +1.0f), Col[6] },
        { XMFLOAT3(+1.0f, -1.0f, +1.0f), Col[7] }
    };

    D3D11_BUFFER_DESC vbd;
    // ������ D3D11_USAGE_DEFAULT�� �ؼ� GPU�� �ڿ��� �а� ���� ������ 
    // ���⼱ ����ȭ�� ���� ���� ���� �Ŀ� ���� ������ ���� D3D11_USAGE_IMMUTABLE�� ���
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex) * 8; // ������ ���� ������ ũ��
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // ���� ���۴� ������ �̰� ����.
    vbd.CPUAccessFlags = 0; // CPU�� ���� ���� ��꿡 �������� �����Ƿ� 0
    vbd.MiscFlags = 0; // ���� ���ۿ����� �̰� ������ 0
    vbd.StructureByteStride = 0; // ������ ���� ��� ���ϸ� 0
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = vertices;
    HR(device->CreateBuffer(&vbd, &vinitData, mBoxVB.GetAddressOf())); // vbd(���� ���� �Ӽ�), vinitData(���� �ʱ�ȭ), mBoxVB(������ ������ ���)

    // �ε��� ���� ����, �ð������ DX���� �ո�
    UINT indices[] = {
        // front face
        0, 1, 2,
        0, 2, 3,

        // back face
        4, 6, 5,
        4, 7, 6,

        // left face
        4, 5, 1,
        4, 1, 0,

        // right face
        3, 2, 6,
        3, 6, 7,

        // top face
        1, 5, 6,
        1, 6, 2,

        // bottom face
        4, 0, 3,
        4, 3, 7
    };

    // �ε��� ���� Ư�� ����, ����� ���� ���ۿ� ���
    D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT) * 36; // 36���� �ε���
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // �ε��� ���۴� �̷��� ����
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    ibd.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA iinitData; // ���� ���� �ʱ�ȭ�� �ڷḦ ����
    iinitData.pSysMem = indices;
    HR(device->CreateBuffer(&ibd, &iinitData, mBoxIB.GetAddressOf())); // ���� ���� ����, HR�� ����� �뵵
}

void Game::BuildFX()
{
    auto device = m_deviceResources->GetD3DDevice();
    
    DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
    shaderFlags |= D3D10_SHADER_DEBUG;
    shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

    ID3D10Blob* compiledShader = 0;
    ID3D10Blob* compilationMsgs = 0;
    HRESULT hr = D3DCompileFromFile(L"FX/color.fx", 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "none", "fx_5_0", shaderFlags,
        0, &compiledShader, &compilationMsgs);

    if (FAILED(hr))
    {
        if (compilationMsgs)
        {
            OutputDebugStringA((char*)compilationMsgs->GetBufferPointer());
            compilationMsgs->Release();
        }

        if (compiledShader)
            compiledShader->Release();

        return;
    }

    HR(D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(),
        0, device, &mFX)); // ����Ʈ�� mFX�� ����

    // �����ϵ� ���̴��� �� ����ؼ� ������
    compiledShader->Release();

    mTech = mFX->GetTechniqueByName("ColorTech"); // fx ������ ���� ColorTech��� �̸��� ��ũ�� ����ü�� �ִ�. �̸� �������� ��
    mfxWorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix(); // fx ���Ͽ��� gWorldViewProj ������ ��� ���·� ȹ����
}

void Game::BuildVertexLayout()
{
    auto device = m_deviceResources->GetD3DDevice();

    // ���� �Է� ��ġ �����
    // �Է� �������� ������ �˾ƾ� �Ѵ�.
    // ���п� �ο��� �̸�, ����(������ 0������ �ϳ��� ������ ������ �ؽ��İ� ��������� ������ ��ȿ������.)
    // , ������ �ڷ� ����(���� DXGI_FORMAT_R32G32B32_FLOAT�� 3���� 32��Ʈ �ε��Ҽ����� �ǹ�)
    // , �Է� ���� ����(��ġ�� ������ ���� �ɰ��� ������ �Ѵٸ� ���)
    // , ����Ʈ ������ ������(���� Ư���� ��ġ�� 3���� float���̶� 12���� �����Ͽ� 12�� ����)
    // , ������ 2���� �Ӽ��� �ϴ� ����
    // �ڼ��� ������ å 212p ����
    D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    // �Է� ��ġ ��ü ����
    D3DX11_PASS_DESC passDesc; // �н��� �����ϴ� ����ü, �Է� ����
    // Effect ��ü���� �н��� �ְ� �ش� �н����� ������ ���� ���̴��� ����
    // ȿ�� ��ü�κ��� �н��� �����ϴ� ����ü ȹ�� ����
    mTech->GetPassByIndex(0)->GetDesc(&passDesc);
    HR(device->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature,
        passDesc.IAInputSignatureSize, &mInputLayout)); // ������ �Է� ��ġ�� mInputLayout ���⿡ ���� �ȴ�.
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
        {
            Update(m_timer);
        });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.

    // ������ â ������ ǥ��
    std::wostringstream outs;
    outs.precision(6);
    outs << L"DX_Sample" << L"    " // ���� �����鿡�Դ� �ʴ� �����Ӽ��� �������̰�����
        << L"FPS: " << timer.GetFramesPerSecond() << L"    " // �����ڿ��Դ� �����Ӵ� �ʰ� ������ �� �ȴ�.
        << L"Frame Time: " << 1000.f / timer.GetFramesPerSecond() << L" (ms)";
    SetWindowText(m_deviceResources->GetWindow(), outs.str().c_str());
    // ������ â ������ ǥ��

    float x = mRadius * sinf(mPhi) * cosf(mTheta);
    float z = mRadius * sinf(mPhi) * sinf(mTheta);
    float y = mRadius * cosf(mPhi);

    // �þ� ��� ����
    XMVECTOR pos = XMVectorSet(x, y, z, 1.0f); // ī�޶� ��ġ, ��ǥ�ϱ� 1.0f
    XMVECTOR target = XMVectorZero(); // ������ �ٶ�
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // ���� ī�޶� ���� ����(���� ����)

    XMMATRIX V = XMMatrixLookAtLH(pos, target, up); // �þ���� ����
    XMStoreFloat4x4(&mView, V); // mView = �þ� ���

    elapsedTime;
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();

    // TODO: Add your rendering code here.

    context->IASetInputLayout(mInputLayout.Get()); // �Է� ��ġ�� ����ϰ��� �ϴ� ��ġ�� ����
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �ﰢ ����Ʈ ������ �������

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, mBoxVB.GetAddressOf(), &stride, &offset);
    context->IASetIndexBuffer(mBoxIB.Get(), DXGI_FORMAT_R32_UINT, 0); // �ε��� ���۵��� �Է� ������ �ܰ迡 ����

    // ��ȯ ����� ����
    XMMATRIX world = XMLoadFloat4x4(&mWorld);
    XMMATRIX view = XMLoadFloat4x4(&mView);
    XMMATRIX proj = XMLoadFloat4x4(&mProj);
    XMMATRIX worldViewProj = world * view * proj;

    mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

    D3DX11_TECHNIQUE_DESC techDesc;
    mTech->GetDesc(&techDesc);
    for (UINT p = 0; p < techDesc.Passes; ++p)
    {
        mTech->GetPassByIndex(p)->Apply(0, context);

        // 36���� �ڽ� �ε��� ���۵�
        context->DrawIndexed(36, 0, 0); // ���� �ε��� ����, ù ���� ��ġ, ������ ������ ���� ������ ������ �׸���
    }

    context;

    m_deviceResources->PIXEndEvent();

    // Show the new frame.
    m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CadetBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowMoved()
{
    auto r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

void Game::OnMouseDown(WPARAM btnState, int x, int y)
{
    mLastMousePos.x = x;
    mLastMousePos.y = y;

    SetCapture(m_deviceResources->GetWindow());
}

void Game::OnMouseUp(WPARAM btnState, int x, int y)
{
    ReleaseCapture();
}

void Game::OnMouseMove(WPARAM btnState, int x, int y)
{
    if ((btnState & MK_LBUTTON) != 0) // ���� Ŭ������ ī�޶� ��ġ �̵�
    {
        // 1�ȼ��� 4���� 1�� (Degree ����) �� �ǰ� �Ѵ�.
        float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePos.x));
        float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePos.y));

        // ���콺 �Է¿� ������ ������ ���� �ֺ��� �˵� ī�޶� ����
        mTheta += dx;
        mPhi += dy;

        // mPhi(��� �������δ� ���� �� �����̴� ��)�� ���� ������ 0 ~ 180���� �����ϴ� ��.
        mPhi = mPhi < 0.1f ? 0.1f : (mPhi > XM_PI - 0.1f ? XM_PI - 0.1f : mPhi);
    }
    else if ((btnState & MK_RBUTTON) != 0) // ���� Ŭ������ ī�޶� Ȯ�� ���
    {
        // 1�ȼ��� ����� 0.005 ������ �ǰ� �Ѵ�.
        float dx = 0.005f * static_cast<float>(x - mLastMousePos.x);
        float dy = 0.005f * static_cast<float>(y - mLastMousePos.y);

        // ���콺 �Է¿� �����Ͽ� �˵� ī�޶��� �������� ����
        mRadius += dx - dy;

        // �������� ������ 3 ~ 15�� ������ �д�.
        mRadius = mRadius < 3.0f ? 3.0f : (mRadius > 15.0f ? 15.0f : mRadius);
    }

    mLastMousePos.x = x;
    mLastMousePos.y = y;
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();

    // TODO: Initialize device dependent objects here (independent of window size).
    device;
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
    XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * XM_PI, m_deviceResources->GetAspectRatio(), 1.0f, 1000.0f); // ������� ����
    XMStoreFloat4x4(&mProj, P); // ���� ��� ����
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
