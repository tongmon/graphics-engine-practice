#include "Vertex.h"
#include "Effects.h"

#pragma region InputLayoutDesc

// �⺻ ���� ���� ����
const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::Basic32[3] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

// ���� ���� ����
const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::TreePointSprite[2] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

#pragma endregion

#pragma region InputLayouts

ID3D11InputLayout* InputLayouts::Basic32 = 0;
ID3D11InputLayout* InputLayouts::TreePointSprite = 0;

void InputLayouts::InitAll(ID3D11Device* device)
{
	D3DX11_PASS_DESC passDesc;

	//
	// Basic32
	//

	Effects::BasicFX->Light1Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(device->CreateInputLayout(InputLayoutDesc::Basic32, 3, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &Basic32));

	//
	// TreePointSprite
	// ȿ�� ��ü ���� ����ü�� ȹ���ϰ� �� ����ü�κ��� ���� ���̴��� �Է� ������ ���� 
	// TreeSpriteFX->Light3Tech->GetPassByIndex(0)->GetDesc(&passDesc)�� �ǹ̴�
	// ȿ�� ��ü ���� --> ȿ�� ��ũ�п��� -> �ε����� �ش��ϴ� �н� ȹ�� -> ȹ���� �н��� �н� �����Ϳ� ����
	// ���� ���� ����� �н� �����͸� �̿��ؼ� fx�� cpp�� ������ �Է� ���̾ƿ���
	// CreateInputLayout�� ����Ͽ� ����
	//

	Effects::TreeSpriteFX->Light3Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(device->CreateInputLayout(InputLayoutDesc::TreePointSprite, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &TreePointSprite));
}

void InputLayouts::DestroyAll()
{
	ReleaseCOM(Basic32);
	ReleaseCOM(TreePointSprite);
}

#pragma endregion