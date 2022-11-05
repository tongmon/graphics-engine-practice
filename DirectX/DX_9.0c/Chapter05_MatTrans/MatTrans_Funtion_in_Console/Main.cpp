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

void PrintMat(D3DXMATRIX *buffer)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("%.2f	", buffer->m[i][j]);
		}
		cout << endl;
	}
}

int main()
{
	D3DXMATRIX matMatrix;

	cout << "�̵� ���\n";
	// ��� ������ ���� 0.5�� �̵��Ǵ� ��� ����
	D3DXMatrixTranslation(&matMatrix, 0.5f, 0.5f, 0.5f); // ���� �� ����� ��ǥ�� �����ָ� ��ǥ�� +0.5 �̵��ϰ� �Ǵ� ���̴�.
	PrintMat(&matMatrix); cout << "\n\n";

	cout << "�����ϸ� ���\n";
	// ���� ��ǥ�� ũ�Ⱑ ���������� ��ȭ�Ǵ� ��� ����
	D3DXMatrixScaling(&matMatrix, 0.5f, 0.5f, 0.5f); // ���� �� ����� ��ǥ�� �����ָ� ��ǥ ���� �������� �پ���.
	PrintMat(&matMatrix); cout << "\n\n";

	// ���� �� �Լ��� �ſ� ���� ���� �Լ��̴�.
	float Radian = 0.785398, Degree = 45;
	cout << "0.785398������ ȣ�������� ��ȯ: " << D3DXToDegree(Radian) << endl;
	cout << "45���� �������� ��ȯ: " << D3DXToRadian(Degree) << "\n\n";

	cout << "X�� ȸ�� ���\n";
	// �޼� ��ǥ���� X���� �������� ȸ�� �����ִ� ��� ����, �޼� ��ǥ���̱⿡ �ð����(CW)���� ȸ�� �Ѵ�.
	// ���� �������� �޴µ� ���� ȣ����->���� ��ȯ �Լ��� ����ؼ� �������־���.
	D3DXMatrixRotationX(&matMatrix, D3DXToRadian(45)); // ���� �� ����� ��ǥ�� �����ָ� ��ǥ�� ��ġ�� X�� ���� 45�� ȸ���ȴ�.
	PrintMat(&matMatrix); cout << "\n\n";

	cout << "Y�� ȸ�� ���\n";
	D3DXMatrixRotationY(&matMatrix, D3DXToRadian(45)); // ���� �� ����� ��ǥ�� �����ָ� ��ǥ�� ��ġ�� Y�� ���� 45�� ȸ���ȴ�.
	PrintMat(&matMatrix); cout << "\n\n";

	cout << "Z�� ȸ�� ���\n";
	D3DXMatrixRotationY(&matMatrix, D3DXToRadian(45)); // ���� �� ����� ��ǥ�� �����ָ� ��ǥ�� ��ġ�� Z�� ���� 45�� ȸ���ȴ�.
	PrintMat(&matMatrix); cout << "\n\n";

	cout << "ZXY�� ȸ�� ���\n";
	// �� �ึ�� Ư�� ������ ȸ�� �����־�� �� ��� ���� �Լ��� ����Ѵ�.
	// Yaw�� Y��, Pitch�� X��, Roll�� Z���� �ǹ��ϸ� �Լ� ���� ������ �̿� ����.
	// ��ȯ ���� Z�� ȸ����� * X�� ȸ����� * Y�� ȸ������� �ȴ�.
	// ���� �� ����� ��ǥ�� �����ָ� ��ǥ�� ��ġ�� z������ 45�� ȸ�� ��, x������ 30�� ȸ�� ��, y������ 20�� ȸ���ϰ� �ȴ�.
	// �� z -> x -> y ȸ�� ���̶�� ���� �������!
	D3DXMatrixRotationYawPitchRoll(&matMatrix, D3DXToRadian(20), D3DXToRadian(30), D3DXToRadian(45));
	PrintMat(&matMatrix); cout << "\n\n";

	cout << "(2, 2, 2)�� ������ ȸ�� ���\n";
	D3DXVECTOR3 Axis(2, 2, 2);
	// ���� �Լ��� ������ �ִ� X,Y,Z ���� �ƴ� ������ �����κ��� ȸ�� ����� �������ش�.
	D3DXMatrixRotationAxis(&matMatrix, &Axis, D3DXToRadian(45)); // ���� �� ����� ��ǥ�� �����ָ� ��ǥ�� ��ġ�� (2, 2, 2)�� ���� 45�� ȸ���ȴ�.
	PrintMat(&matMatrix); cout << "\n\n";

	// �̿� ���� �̵�, ȸ��, �����ϸ� ��ĵ��� ���� ���� �ִµ� �̸� ������ �����ų �� ���ϰ� �Ǵ� ������ �ſ� �߿��ϴ�.
	// S(scale) * R(rotate) * T(Translate) �� ������ �� ��Ű�� ������ ������ ����� ����ȴ�.
	// �� 3�� �� Rotate �ȿ����� z -> x -> y ���� ȸ�� ������ �ִ�. �� 2���� ���̸� ������ ��� ����. x->y / y->x ��� ����
}