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

// XMVECTOR�� �Ű������� �ѱ� ��� �����ؾ� �� ����
// �Լ��� ó�� �� XMVECTOR �Ű������� �ݵ�� FXMVECTOR�� ����Ѵ�.
// �������� CXMVECTOR�� ����Ѵ�.

ostream& operator<<(ostream& os, FXMVECTOR v) // ���� ��� ������ ������
{
	XMFLOAT3 dest;
	XMStoreFloat3(&dest, v);

	os << "(" << dest.x << ", " << dest.y << ", " << dest.z << ")";
	return os;
}

int main()
{
	cout.setf(ios_base::boolalpha);

	// XNA MATH ȣȯ�� �˻�
	if (!XMVerifyCPUSupport())
	{
		cout << "xna math not supported" << endl;
		return 0;
	}
	cout << "XNA MATH ���� ���" << endl;
	// ���� ����
	XMVECTOR n = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR u = XMVectorSet(1.0f, 2.0f, 3.0f, 0.0f);
	XMVECTOR v = XMVectorSet(-2.0f, 1.0f, -3.0f, 0.0f);
	XMVECTOR w = XMVectorSet(0.707f, 0.707f, 0.0f, 0.0f);

	// ���� ��
	XMVECTOR a = u + v;

	// ���� ��
	XMVECTOR b = u - v;

	// ���Ϳ� ��Į�� ���� ������ ���۷��̴��� ���ǵǾ� �ִ�.
	XMVECTOR c = 10.0f * u;

	// ������ ũ��
	XMVECTOR L = XMVector3Length(u);

	// ���� ���ͷ� ��ȯ, ������ ������ ũ�Ⱑ 1
	XMVECTOR d = XMVector3Normalize(u);

	// ���� ����, �� ���� ���հ��� �˾Ƴ� ��� �����ϴ�.
	XMVECTOR s = XMVector3Dot(u, v);

	// ���� ����, �� ���Ϳ� ������ ���� ���͸� �����ϴµ� ���
	XMVECTOR e = XMVector3Cross(u, v);

	// proj_n(w) = ������ �̿��Ͽ� �������� ������ ���� ����
	// perp_n(w) = ���� ���� ����
	XMVECTOR projW;
	XMVECTOR perpW;
	XMVector3ComponentsFromNormal(&projW, &perpW, w, n); // �� ���� 2���� �����ָ� ���� ����, ���� ���� ����

	// projW + perpW == w �ΰ�? �� �˷��ش�.
	bool equal = XMVector3Equal(projW + perpW, w) != 0;
	bool notEqual = XMVector3NotEqual(projW + perpW, w) != 0;

	// projW �� perpW �� ������ �����ش�. �� ���� ���� 90������ �Ѵ�.
	XMVECTOR angleVec = XMVector3AngleBetweenVectors(projW, perpW);
	float angleRadians = XMVectorGetX(angleVec);
	float angleDegrees = XMConvertToDegrees(angleRadians);

	cout << "u                   = " << u << endl;
	cout << "v                   = " << v << endl;
	cout << "w                   = " << w << endl;
	cout << "n                   = " << n << endl;
	cout << "a = u + v           = " << a << endl;
	cout << "b = u - v           = " << b << endl;
	cout << "c = 10 * u          = " << c << endl;
	cout << "d = u / ||u||       = " << d << endl;
	cout << "e = u x v           = " << e << endl;
	cout << "L  = ||u||          = " << L << endl;
	cout << "s = u.v             = " << s << endl;
	cout << "projW               = " << projW << endl;
	cout << "perpW               = " << perpW << endl;
	cout << "projW + perpW == w  = " << equal << endl;
	cout << "projW + perpW != w  = " << notEqual << endl;
	cout << "angle               = " << angleDegrees << "\n\n\n";
	
	cout << "�ε��Ҽ��� ������ ���� 1�� ũ�Ⱑ ����Ǿ�� �ϴ� ���� ������ ��ġ�鼭 1�� ������ �ʴ´�." << endl;
	
	XMVECTOR U = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f); // ���� ����
	XMVECTOR N = XMVector3Normalize(U); // ���� ���ͷ� ��ȯ

	float LU = XMVectorGetX(XMVector3Length(N)); // ���� ������ ũ�⸦ �޴´�.

	// ���� ������ ũ��� 1�̴�? ������ ���� 1�� ���ñ�?
	cout << "�������� ũ��: " << LU << endl; // ���⼱ 1�� ���� ���̴�.
	if (LU == 1.0f)
		cout << "Length 1" << endl;
	else
		cout << "Length not 1" << endl; // �ܼ�â�� 1�̶�� �������� ����� 1�� �ƴ϶�� ���Ѵ�.

	// 1�� � ���� ������ �ص� 1�� �����ؾ� �� ���̴�. ������ �����?
	float powLU = powf(LU, 1.0e6f); // �� �� �����ϰ� ��������
	cout << "LU^(10^6) = " << powLU << endl; // 1�� �ƴ϶�� ���� ����Ǿ���.

	/*
	// �ذ� ���� �پ��� ���� �Լ����� ���õǾ� ������ �Ը���� ��� ���� �ȴ�.
	XMVECTOR p = XMVectorSet(2.0f, 2.0f, 1.0f, 0.0f);
	XMVECTOR q = XMVectorSet(2.0f, -0.5f, 0.5f, 0.1f);
	XMVECTOR u = XMVectorSet(1.0f, 2.0f, 4.0f, 8.0f);
	XMVECTOR v = XMVectorSet(-2.0f, 1.0f, -3.0f, 2.5f);
	XMVECTOR w = XMVectorSet(0.0f, XM_PIDIV4, XM_PIDIV2, XM_PI);

	cout << "XMVectorAbs(v)                 = " << XMVectorAbs(v) << endl;
	cout << "XMVectorCos(w)                 = " << XMVectorCos(w) << endl;
	cout << "XMVectorLog(u)                 = " << XMVectorLog(u) << endl;
	cout << "XMVectorExp(p)                 = " << XMVectorExp(p) << endl;
	
	cout << "XMVectorPow(u, p)              = " << XMVectorPow(u, p) << endl;
	cout << "XMVectorSqrt(u)                = " << XMVectorSqrt(u) << endl;
	
	cout << "XMVectorSwizzle(u, 2, 2, 1, 3) = " 
		<< XMVectorSwizzle(u, 2, 2, 1, 3) << endl;
	cout << "XMVectorSwizzle(u, 2, 1, 0, 3) = " 
		<< XMVectorSwizzle(u, 2, 1, 0, 3) << endl;

	cout << "XMVectorMultiply(u, v)         = " << XMVectorMultiply(u, v) << endl;
	cout << "XMVectorSaturate(q)            = " << XMVectorSaturate(q) << endl;
	cout << "XMVectorMin(p, v               = " << XMVectorMin(p, v) << endl;
	cout << "XMVectorMax(p, v)              = " << XMVectorMax(p, v) << endl;
	*/

	return 0;
}