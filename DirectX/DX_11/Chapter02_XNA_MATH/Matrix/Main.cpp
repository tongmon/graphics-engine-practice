// �� ������ ���Ѿ��Ѵ�... �ȱ׷��� ��10���� �ٷ� �����ÿ� ��������.
#define _XM_NO_INTRINSICS_
#include <DirectXMath.h>
using namespace DirectX;

#include <iostream>
using namespace std;

#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib,"d3dx9.lib")
#pragma comment (lib,"DxErr.lib")
#pragma comment (lib,"d3dxof.lib")
#pragma comment (lib,"winmm.lib")

ostream& operator<<(ostream& os, FXMVECTOR v)
{
	XMFLOAT4 dest;
	XMStoreFloat4(&dest, v);

	os << "(" << dest.x << ", " << dest.y << ", " << dest.z << ", " << dest.w << ")";
	return os;
}

// XMMATRIX�� �Ű������� �����Ϸ� �� ���� CXMMATRIX�� ����� �����ؾ� �Ѵ�.
// Ư���� ��쿡 FXMMATRIX�� ����� �ѱ�⵵ �ϴµ� �ذ� ���� ����̴�.
// 1. �ϴ��� �������� XMMATRIX �̿� ���ƾ� �Ѵ�.
// 2. XMMATRIX ���� �տ� XMVECTOR�� 3�� �̻� �������� ���� ��
// 3. XMMATRIX ���� �ڿ� 2�� �̻��� float�� double, XMVECTOR�� �������� ���� ��
// �������� ��� �׳� CXMMATRIX�� ����Ѵٰ� ���� �ȴ�.

ostream& operator<<(ostream& os, CXMMATRIX m)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			os << m(i, j) << "\t";
		}
		os << endl;
	}
	return os;
}

int main()
{
	// XNA MATH ȣȯ�� �˻�
	if (!XMVerifyCPUSupport())
	{
		cout << "xna math not supported" << endl;
		return 0;
	}
	cout << "XNA MATH ��� �Լ� ���" << endl;
	
	XMMATRIX A(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 4.0f, 0.0f,
		1.0f, 2.0f, 3.0f, 1.0f); // �����ڸ� ���� ������ ��� ����

	XMMATRIX B = XMMatrixIdentity(); // ���� ��� ����

	XMMATRIX C = A * B; // ������ ���ϰ� ���ǵǾ� �ִ�.

	XMMATRIX D = XMMatrixTranspose(A); // ��ġ ��� ����

	// ��Ľ� ����, ���� ����� ���� �� ���� ���ȴ�.
	XMVECTOR det = XMMatrixDeterminant(A); // ��� ���� �ϳ����� ���ͷ� ��ȯ�Ǿ� ��� ���� ��Ľ��� ���� ���Ͱ� ����ȴ�.
	
	XMMATRIX E = XMMatrixInverse(&det, A); // ����� ����, ũ���̸� �������� ���Ѵ�.

	XMMATRIX F = A * E; // ��������� Ƣ��;� ����

	cout << "A = " << endl << A << endl;
	cout << "B = " << endl << B << endl;
	cout << "C = A*B = " << endl << C << endl;
	cout << "D = transpose(A) = " << endl << D << endl;
	cout << "det = determinant(A) = " << det << endl << endl;
	cout << "E = inverse(A) = " << endl << E << endl;
	cout << "F = A*E = " << endl << F << endl;

	return 0;
}