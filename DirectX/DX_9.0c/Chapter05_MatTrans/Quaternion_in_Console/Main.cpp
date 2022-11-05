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
	D3DXQUATERNION Quaternion; // ���ʹϿ��� ȸ����(x,y,z)�� ȸ����(Radian)���� �����ȴ�. ȸ�� ������ �ݽð�(CCW)�̴�.
	D3DXMATRIX matMatrix;

	// ������ ������ �����ϱ� ���� ���ʹϿ��� ���ȴ�. ���ʹϿ��� ���� ȸ�� �̵� �ÿ��� ���ȴ�.
	// �� ���ʹϿ��� ���� ������ ��Ʈ������ �ٸ��� ���� ��� R_Mat = R_Mat_1 * R_Mat_2 ��� R_Mat_1�� ȸ���� ���� ���������
	// ���ʹϿ¿����� Qua = Qua_1 * Qua_2 ��� Qua_2 �� ���� ����ȴ�.
	// �ǻ��ÿ��� ���ʹϿ� ��ü�� ����ϴ� ���� �ƴ϶� ���ʹϿ��� �̿��� ȸ�� ����� ����� ����Ѵ�.
	// ���ʹϿ¿� �ִ� wȸ�� ���� �ݽð� ȸ������ ��Ÿ������ ���ʹϿ��� ���� ����� ���� ������ �� ��Ŀ� �ð���� ȸ������ ����.

	cout << "X�� ȸ�� ���\n";
	D3DXMatrixRotationX(&matMatrix, D3DXToRadian(45)); // X�� ���� 45�� �ð���� ȸ��
	PrintMat(&matMatrix); cout << "\n\n";

	cout << "X������ 45�� ȸ���ϴ� ���� ����� ���ʹϿ�\n";
	D3DXQuaternionRotationMatrix(&Quaternion, &matMatrix); // ȸ�� ����� ���ʹϿ����� ��ȯ
	printf("ȸ���� ( %.3f, %.3f, %.3f ) ȸ����(ȣ����): %.3f\n", Quaternion.x, Quaternion.y, Quaternion.z, D3DXToDegree((double)Quaternion.w));
	
	cout << "���ʹϿ����� ����� ȸ�� ���\n";
	D3DXMatrixRotationQuaternion(&matMatrix, &Quaternion); // ���� ���ʹϿ��� ���� ȸ�� ��ķ� ��ȯ�Ͽ� ��� �Լ�, �ǻ��� �� ����� ��ǥ�� ������ ���̴�.
	PrintMat(&matMatrix); cout << "\n\n";

	D3DXQuaternionRotationYawPitchRoll(&Quaternion, D3DXToRadian(0), D3DXToRadian(45), D3DXToRadian(0)); // �ƴϸ� ���� YawPitchRoll�� ����
	cout << "X������ 45�� ȸ���ϴ� ���� ����� ���ʹϿ�\n";
	printf("ȸ���� ( %.3f, %.3f, %.3f ) ȸ����(ȣ����): %.3f\n", Quaternion.x, Quaternion.y, Quaternion.z, D3DXToDegree((double)Quaternion.w));
	
	cout << "���ʹϿ����� ����� ȸ�� ���\n";
	D3DXMatrixRotationQuaternion(&matMatrix, &Quaternion);
	PrintMat(&matMatrix); cout << "\n\n";

	D3DXVECTOR3 Axis(1, 0, 0); // X��
	D3DXQuaternionRotationAxis(&Quaternion, &Axis, D3DXToRadian(45));
	cout << "X������ 45�� ȸ���ϴ� ���� ����� ���ʹϿ�\n";
	printf("ȸ���� ( %.3f, %.3f, %.3f ) ȸ����(ȣ����): %.3f\n", Quaternion.x, Quaternion.y, Quaternion.z, D3DXToDegree((double)Quaternion.w));

	cout << "���ʹϿ����� ����� ȸ�� ���\n";
	D3DXMatrixRotationQuaternion(&matMatrix, &Quaternion);
	PrintMat(&matMatrix); cout << "\n\n";

	cout << "���̰� 1�� ���ʹϿ�\n";
	D3DXQuaternionNormalize(&Quaternion, &Quaternion); // ���̰� 1�� ���ʹϿ� ���
	printf("ȸ���� ( %.3f, %.3f, %.3f ) ȸ����(ȣ����): %.3f\n", Quaternion.x, Quaternion.y, Quaternion.z, D3DXToDegree((double)Quaternion.w));
}