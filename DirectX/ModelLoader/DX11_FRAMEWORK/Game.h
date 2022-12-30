//
// Game.h
//

#pragma once

#include "goh.h"
#include "DeviceResources.h"
#include "StepTimer.h"
#include "d3dx11Effect.h"
#include "GeometryGenerator.h"
#include "MathHelper.h"
#include "LightHelper.h"
#include "EssEffects.h"
#include "Vertex.h"
#include "Camera.h"
#include "Sky.h"
#include "RenderStates.h"
#include "ShadowMap.h"
#include "Ssao.h"
#include "TextureMgr.h"
#include "BasicModel.h"
#include "SkinnedModel.h"
#include "ModelLoader.h"
#include "Model.h"

// Effects11.lib, DirectXTK.lib �� ���̺귯���� ������ Windows Kit�� �� �־�� �Ѵ�.
// ������ ���̺귯�� �غ��ϴ� ����� �ؽ��� ���� ���� ����
#if defined( DEBUG ) || defined( _DEBUG )
#pragma comment (lib,"Effects11d.lib")
#pragma comment (lib,"DirectXTKd.lib")
#else
#pragma comment (lib,"Effects11.lib")
#pragma comment (lib,"DirectXTK.lib")
#endif

// ���̺귯�� ����
#pragma comment (lib,"assimp-vc142-mtd.lib")

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
    HRESULT	CompileShaderFromFile(LPCWSTR pFileName, const D3D_SHADER_MACRO* pDefines, LPCSTR pEntryPoint, LPCSTR pShaderModel, ID3DBlob** ppBytecodeBlob);

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

    // ���� ���콺 ��ǥ
    POINT                                   mLastMousePos;

private:
    void DrawSceneToSsaoNormalDepthMap();
    void DrawSceneToShadowMap();
    void DrawScreenQuad(ID3D11ShaderResourceView* srv);
    void BuildShadowTransform();
    void BuildShapeGeometryBuffers();
    void BuildSkullGeometryBuffers();
    void BuildScreenQuadGeometryBuffers();

private:
    TextureMgr mTexMgr;

    Sky* mSky;

    Model* mTestModels[4];
    // �׽�Ʈ ��
    Model* mTest;
    ModelInst mTestInst_1;

    // Ű �Է�
    bool mKeyInput[50];

    ID3D11Buffer* mShapesVB;
    ID3D11Buffer* mShapesIB;

    ID3D11Buffer* mSkullVB;
    ID3D11Buffer* mSkullIB;

    ID3D11Buffer* mSkySphereVB;
    ID3D11Buffer* mSkySphereIB;

    ID3D11Buffer* mScreenQuadVB;
    ID3D11Buffer* mScreenQuadIB;

    ID3D11ShaderResourceView* mStoneTexSRV;
    ID3D11ShaderResourceView* mBrickTexSRV;

    ID3D11ShaderResourceView* mStoneNormalTexSRV;
    ID3D11ShaderResourceView* mBrickNormalTexSRV;

    // ��� ��ü�� ���� ��, ��鿡 ���� ����ü�� ����� �� �̿�
    DirectX::BoundingSphere mSceneBounds;

    // �׸��� �ʿ� ���� ũ��, �� ���� ���� ����
    static const int SMapSize = 2048;
    ShadowMap* mSmap;
    XMFLOAT4X4 mLightView;
    XMFLOAT4X4 mLightProj;
    XMFLOAT4X4 mShadowTransform;

    Ssao* mSsao; // AO�� ���� ������

    float mLightRotationAngle;
    XMFLOAT3 mOriginalLightDir[3];
    DirectionalLight mDirLights[3];
    Material mGridMat;
    Material mBoxMat;
    Material mCylinderMat;
    Material mSphereMat;
    Material mSkullMat;

    // ��ü�� ���� ��ȯ ���
    XMFLOAT4X4 mSphereWorld[10];
    XMFLOAT4X4 mCylWorld[10];
    XMFLOAT4X4 mBoxWorld;
    XMFLOAT4X4 mGridWorld;
    XMFLOAT4X4 mSkullWorld;

    int mBoxVertexOffset;
    int mGridVertexOffset;
    int mSphereVertexOffset;
    int mCylinderVertexOffset;

    UINT mBoxIndexOffset;
    UINT mGridIndexOffset;
    UINT mSphereIndexOffset;
    UINT mCylinderIndexOffset;

    UINT mBoxIndexCount;
    UINT mGridIndexCount;
    UINT mSphereIndexCount;
    UINT mCylinderIndexCount;

    UINT mSkullIndexCount;

    Camera mCam; // ī�޶�
    
    // ī�޶�κ��� �÷��̾���� �Ÿ�
    float mCamDist;
    XMFLOAT3 mCamCenter;
};
