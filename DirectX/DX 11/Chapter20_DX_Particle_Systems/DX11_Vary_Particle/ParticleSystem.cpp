//***************************************************************************************
// ParticleSystem.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "ParticleSystem.h"
#include "TextureMgr.h"
#include "Vertex.h"
#include "Effects.h"
#include "Camera.h"
 
ParticleSystem::ParticleSystem()
: mInitVB(0), mDrawVB(0), mStreamOutVB(0), mTexArraySRV(0), mRandomTexSRV(0)
{
	mFirstRun = true;
	mGameTime = 0.0f;
	mTimeStep = 0.0f;
	mAge      = 0.0f;

	mEyePosW  = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mEmitPosW = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mEmitDirW = XMFLOAT3(0.0f, 1.0f, 0.0f);
}

ParticleSystem::~ParticleSystem()
{
	ReleaseCOM(mInitVB);
	ReleaseCOM(mDrawVB);
	ReleaseCOM(mStreamOutVB);
}

float ParticleSystem::GetAge()const
{
	return mAge;
}

void ParticleSystem::SetEyePos(const XMFLOAT3& eyePosW)
{
	mEyePosW = eyePosW;
}

void ParticleSystem::SetEmitPos(const XMFLOAT3& emitPosW)
{
	mEmitPosW = emitPosW;
}

void ParticleSystem::SetEmitDir(const XMFLOAT3& emitDirW)
{
	mEmitDirW = emitDirW;
}

void ParticleSystem::Init(ID3D11Device* device, ParticleEffect* fx, ID3D11ShaderResourceView* texArraySRV, 
	                      ID3D11ShaderResourceView* randomTexSRV, UINT maxParticles)
{
	mMaxParticles = maxParticles;

	mFX = fx;

	mTexArraySRV  = texArraySRV;
	mRandomTexSRV = randomTexSRV; 

	BuildVB(device);
}

void ParticleSystem::Reset()
{
	mFirstRun = true;
	mAge      = 0.0f;
}

void ParticleSystem::Update(float dt, float gameTime)
{
	mGameTime = gameTime;
	mTimeStep = dt;

	mAge += dt;
}

// ���� �ý����� ��Ʈ�� ������� ���� ���̴����� ������ ������ GPU�� ��� (������ X)
// �� ���� ������ ���� ���۷� �Ͽ� ������ �������Ͽ� �׸��� 2���� ó�� ������ �ʿ��ϴ�.
void ParticleSystem::Draw(ID3D11DeviceContext* dc, const Camera& cam)
{
	XMMATRIX VP = cam.ViewProj();

	//
	// ������� ����
	//
	mFX->SetViewProj(VP);
	mFX->SetGameTime(mGameTime);
	mFX->SetTimeStep(mTimeStep);
	mFX->SetEyePosW(mEyePosW);
	mFX->SetEmitPosW(mEmitPosW);
	mFX->SetEmitDirW(mEmitDirW);
	mFX->SetTexArray(mTexArraySRV);
	mFX->SetRandomTex(mRandomTexSRV);

	//
	// �Է� ������ �ܰ踦 ����
	//
	dc->IASetInputLayout(InputLayouts::Particle);
    dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	UINT stride = sizeof(Vertex::Particle);
    UINT offset = 0;

	// ���� �����̸� �ʱ�ȭ�� ���� ���۸� ����ϰ�, �׷��� ������
	// ������ ���� ����� ���� ���� ���۸� ����Ѵ�.
	if( mFirstRun )
		dc->IASetVertexBuffers(0, 1, &mInitVB, &stride, &offset);
	else
		dc->IASetVertexBuffers(0, 1, &mDrawVB, &stride, &offset);

	//
	// ���� ���� ����� ��Ʈ�� ��� ���� ������� �׷��� ���ڵ��� �����Ѵ�.
	// ���ŵ� ���ڵ��� ��Ʈ�� ����� ���ؼ� ��� ���� ���ۿ� ��ϵȴ�.
	//
	dc->SOSetTargets(1, &mStreamOutVB, &offset); // ��Ʈ�� ��¿� ���� ����

    D3DX11_TECHNIQUE_DESC techDesc;
	mFX->StreamOutTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        mFX->StreamOutTech->GetPassByIndex( p )->Apply(0, dc);
        
		if( mFirstRun )
		{
			dc->Draw(1, 0);
			mFirstRun = false;
		}
		else
		{
			// ���� ȭ�鿡 �׷������� �ʰ� ���� ���̴����� ��Ʈ�� ��¿� ����
			// ���ǵǴ� ������ GPU�� ��´�.
			// �� ���� ������ ���� �׸��� ó������ ���� ������ �� ���̴�.
			dc->DrawAuto();
		}
    }

	// ��Ʈ�� ����� �������� ��Ʈ�� ���� ���۸� �����.
	ID3D11Buffer* bufferArray[1] = {0};
	dc->SOSetTargets(1, bufferArray, &offset);

	// ���� ���۵��� �¹ٲ۴�. (����)
	std::swap(mDrawVB, mStreamOutVB);

	//
	// ��� ��Ʈ�� ��µ�, ���ŵ� ���� �ý����� ȭ�鿡 �׸���.
	//
	dc->IASetVertexBuffers(0, 1, &mDrawVB, &stride, &offset);

	mFX->DrawTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        mFX->DrawTech->GetPassByIndex( p )->Apply(0, dc);
        
		dc->DrawAuto();
    }

	// ���������� �� ������ �����غ��� SO �ܰ迡 ���� mStreamOutVB ���⿡ ���ϼ��̴����� �����Ͽ� ������ ��������
	// ���� �� ��� �༮�� swap�ϱ� ���ؼ� SO�ܰ迡�� ����.
	// mStreamOutVB <--> mDrawVB ��׸� �ٲ�ġ�� �ϴµ� ������ DrawVB�� ����ִ� ������ mStreamOutVB ����� �̵��ϰ�
	// mDrawVB�� mStreamOutVB�� ��� �༮���� ���� mDrawVB�� �������ϸ� ȿ���� ȭ�鿡 ���δ�.
	// �׷��� ���� ���� �������� �ٽ� ���ŵǸ� IA �ܰ迡 mDrawVB�� ������ �ǰ� �� �������� hlsl ���Ͽ� �־�����
	// �̸� ����mStreamOutVB���� �ٽ� ���ŵ� ȿ�� �������� ������� ���̰� �̰� �ݺ��Ǹ鼭 � ȿ���� ���ŵǴ� ���̴�.
}

void ParticleSystem::BuildVB(ID3D11Device* device)
{
	//
	// ���ڵ��� ������ ���� ���� ����
	// ����� �����ϴ� ��ȭ�� ���� ������ �Ѵٰ� �� �� �ִ�.
	//

    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(Vertex::Particle) * 1;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// The initial particle emitter has type 0 and age 0.  The rest
	// of the particle attributes do not apply to an emitter.
	Vertex::Particle p;
	ZeroMemory(&p, sizeof(Vertex::Particle));
	p.Age  = 0.0f;
	p.Type = 0; 
 
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &p;

	HR(device->CreateBuffer(&vbd, &vinitData, &mInitVB)); // ���� ���� ���� ����
	
	//
	// ������ ������ ���� ���� 2�� ����
	// �����̶� �ϸ� swap �ϸ鼭 ������ ��ġ�� ��� �ڹٲٴ� ����� ���Ѵ�.
	// �� ���� ���⼭ �� �ϳĸ� �ϳ��� ���� ���۸�
	// IA�ܰ�� SO�ܰ迡 ���ÿ� ����� ���� ��� �ϳ��� SO�ܰ迡 �����ٸ�
	// ���� �༮�� swap���� ���ͼ� �ٽ� IA�ܰ迡 ���� ���ؼ��̴�.
	//
	vbd.ByteWidth = sizeof(Vertex::Particle) * mMaxParticles;
	// ��Ʈ�� ����� �ϴ� �뵵�� �����̸� D3D11_BIND_STREAM_OUTPUT ���
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;

	// ���� ���ҽ��� �ʿ���� ���� ���� ���̴����� ������ ����
	// �� ������ �������� ä�� ���̱⿡ 0���� ���´�.
    HR(device->CreateBuffer(&vbd, 0, &mDrawVB));
	HR(device->CreateBuffer(&vbd, 0, &mStreamOutVB));
}