#include <iostream>
#include <d3d9.h>
#include <d3dx9math.h>
using namespace std;

#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib,"d3dx9.lib")
#pragma comment (lib,"DxErr.lib")
#pragma comment (lib,"d3dxof.lib")
#pragma comment (lib,"winmm.lib")

struct CUSTOMVERTEX // ���� ����ü ��� 1
{
	float x, y, z, rhw; // rhw�� 2d ��, ����� ���� ��ǥ�踦 ���ؼ� ������ ���� �� ���� ���δ�.
	DWORD color;
};
#define D3DFVX_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE) // ���� ��� ��� ����

struct CUSTOMVERTEX_2 // ���� ����ü ��� 2
{
	D3DXVECTOR3 positon;
	D3DXVECTOR3 normal;
	DWORD diffuse; // ����
	float tu, tv; // ������ �ؽ��ĸ� ���� ��� ���
};
#define D3DFVX_CUSTOMVERTEX_2 (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1) // ���� ��� ��� ����

int main()
{
	D3DXVECTOR3 v1(3.0f, 0.0f, 0.0f);
	D3DXVECTOR3 v2(0.0f, 3.0f, 0.0f);
	D3DXVECTOR3 v3;
	D3DXVECTOR3 v4(3.0f, 3.0f, 1.0f);

	v3 = v1 + v2;
	cout << "������ ����\n" << v3.x << " " << v3.y << " " << v3.z << endl;

	D3DXVec3Add(&v3, &v1, &v2);
	cout << v3.x << " " << v3.y << " " << v3.z << "\n\n";

	v3 = v1 - v2;
	cout << "������ ����\n" << v3.x << " " << v3.y << " " << v3.z << endl;

	D3DXVec3Subtract(&v3, &v1, &v2);
	cout << v3.x << " " << v3.y << " " << v3.z << "\n\n";

	float Length = D3DXVec3Length(&v4);
	cout << "����(v4)�� ũ��: " << Length << "\n\n";

	D3DXVECTOR3 v5(1.0f, 2.0f, 0.0f); 
	cout << "���Ϳ� ��Į���� ��\n" << v5.x << " " << v5.y << " " << v5.z << endl;
	float Scale = 2.0f;
	D3DXVec3Scale(&v5, &v5, Scale);
	cout << v5.x << " " << v5.y << " " << v5.z << "\n\n";

	D3DXVECTOR3 v6(3.0f, 1.0f, 4.0f);
	D3DXVECTOR3 vResult;
	D3DXVec3Normalize(&vResult, &v6);
	float NormLength = D3DXVec3Length(&vResult);
	cout << "v6�� ���� ���Ϳ� �� ũ��\n" << vResult.x << " " << vResult.y << " " << vResult.z << endl;
	cout << "v6 ���� ������ ũ��: " << NormLength << "\n\n";

	// ������ �� ���� ������ ���հ� ��, �ڻ��� ���� �˾Ƴ� �� ���δ�.
	D3DXVECTOR3 v7(3.0f, 1.0f, 4.0f);
	D3DXVECTOR3 v8(2.0f, 7.0f, 1.0f);
	float Dot = D3DXVec3Dot(&v7, &v8); // ����
	float v7_len = D3DXVec3Length(&v7);
	float v8_len = D3DXVec3Length(&v8);
	cout << "�� ���� v7�� v8�� ����: " << Dot << endl;
	cout << "�� ���� v7�� v8 ������ ���հ�(Cosin) ����: " << Dot / (v7_len * v8_len) << "\n\n";

	// ������ �� ���Ͱ� �̷�� ����� ���� ��鿡 ������ �̷�� ���� ���͸� ������ �� ���δ�.
	D3DXVec3Cross(&vResult, &v1, &v2); // v1�� v2 ������ �����϶�. ������ �ٲ�ٸ� �ݴ� ���� ���� ���Ͱ� ����ȴ�.
	D3DXVec3Normalize(&vResult, &vResult);
	cout << "v1�� v2�� ���� ����\n" << vResult.x << " " << vResult.y << " " << vResult.z << "\n";

	// ���� ���Խÿ� �ظ��ϸ� �ð���� ������ ������ ��!
	// ������ ���� �Լ��� ���Խÿ� ������ ���� ����Ǵ� ������ ������ �ٲ� ���� �ִ�.
	// �ð�(CW), �ݽð�(CCW) ������ �� ��������!
	CUSTOMVERTEX vertices[] =
	{
		{150.0f, 50.0f, 0.5f, 1.0f, 0xffff0000},
		{250.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00},
		{50.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff}
	};
}