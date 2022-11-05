// Directx11 ���ø��̴�.
// VS2019 x86, x64���� ��� �������� �۵��Ѵ�.
// �����ڵ忡�� �۵��ϸ� ������ �غ��ؾ� �� ���� Effects11d.lib, Effects11.lib
// �� �ΰ��� ���̺귯�� ���� ���̴�.
// ������ ��ɵ��� �� ���뿡 �ٿ������� �Ǹ�, DXTK�� ������ ��ɵ��� ���������
// �н��Ͽ� ����ϸ� �ȴ�. (GLUT�� ���� ����� �����ϴ� DXUT�� �� �۵� ���ؼ� ��)

//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "d3dx11effect.h"
#include "d3dxGlobal.h"
#include "dxerr.h"

// ���� ������� ��� pch�� �־���.
// pch�� �̸� �����ϵ� ����� �����ϸ� c1010 ������ �߻��ϱ⿡
// �̸� �����ϵ� ��� ����� �����Ҵ�.
//#include <d3dcompiler.h>
//#include <windowsx.h>
//#include <sstream>

// Effects11d.lib, Effects11.lib �� ���̺귯���� ������ Windows Kit�� �� �־�� �Ѵ�.
// ������ ���̺귯�� �غ��ϴ� ����� �ؽ��� ���� ���� ����
#if defined( DEBUG ) || defined( _DEBUG )
#pragma comment (lib,"Effects11d.lib")
#pragma comment (lib,"DirectXTKd.lib")
#else
#pragma comment (lib,"Effects11.lib")
#pragma comment (lib,"DirectXTK.lib")
#endif

// D3D11 device ���� �� ���� ���� ����
// final -> ���� �Լ� ����� ���⼭ �����ڴ�. �� �̻��� ���� �Լ��� ���ٴ� ���� ���
class Game final : public DX::IDeviceNotify
{
public:

    Game() noexcept(false); // noexcept -> ���� ������ �ʴ´ٴ� ������ �����ν� �� �� ���� �ӵ� ȹ��
    ~Game();

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // �⺻���� Game ����
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // WinApi �޽��� ���
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnWindowSizeChanged(int width, int height);
    void OnMouseDown(WPARAM btnState, int x, int y);
    void OnMouseUp(WPARAM btnState, int x, int y);
    void OnMouseMove(WPARAM btnState, int x, int y);

    // Properties
    void GetDefaultSize(int& width, int& height) const noexcept;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // ��ġ �ڿ��� ����
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;

    // Ÿ�̸�
    DX::StepTimer                           m_timer;

public:

    void BuildGeometryBuffers();
    void BuildFX();
    void BuildVertexLayout();

private:

    Microsoft::WRL::ComPtr<ID3D11Buffer> mBoxVB; // ���� ����
    Microsoft::WRL::ComPtr<ID3D11Buffer> mBoxIB; // �ε��� ����

    // ��׵鵵 ComPtr ����ϸ� �������� �ϴ� �����ϰ� ��ô�.
    // Microsoft::WRL::ComPtr ����Ʈ �����͸� ������� �ʴ� Com ��ü�̱⿡ �Ҹ��� ���� ���缭
    // Release ���־�� �޸� ������ ����.
    ID3DX11Effect* mFX;
    ID3DX11EffectTechnique* mTech;
    ID3DX11EffectMatrixVariable* mfxWorldViewProj;

    Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;

    // XM�� ���� DirectXmath ���̺귯���� ����ϴ� ���¸� ����Ϸ���
    // ��� ���Ͽ����� using namespace DirectX �� ���� �ʴ� �̻�
    // DirectX:: �� ����ؼ� ��ø� ���־�� �Ѵ�.
    // �ȱ׷��� Windows kit�� �ִ� �ٸ� ����� �浹�Ͽ� ��������.
    DirectX::XMFLOAT4X4 mWorld; // ���� ���, SRT
    DirectX::XMFLOAT4X4 mView; // ī�޶� ��ġ
    DirectX::XMFLOAT4X4 mProj; // ���� ���

    float mTheta;
    float mPhi;
    float mRadius;

    POINT mLastMousePos;
};
