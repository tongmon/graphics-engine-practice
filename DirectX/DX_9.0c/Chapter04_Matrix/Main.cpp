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
	D3DXMATRIX matIdentity, matMatrix, matResult;
	D3DXMatrixIdentity(&matIdentity); // matIdentity�� ���� ��ķ� �����.
	for (int i = 0;i < 4;i++)
	{
		for (int j = 0; j < 4; j++)
		{
			matMatrix(i, j) = float(i * 4 + j + 1);
		}
	}
	cout << "Mat_1" << endl;
	PrintMat(&matMatrix); cout << "\n\n";

	cout << "Mat_2" << endl;
	PrintMat(&matIdentity); cout << "\n\n";

	cout << "Mat_1 * Mat_2" << endl;
	matResult = matIdentity * matMatrix;
	PrintMat(&matResult); cout << "\n\n";

	cout << "Mat_1 + Mat_2" << endl;
	matResult = matIdentity + matMatrix;
	PrintMat(&matResult); cout << "\n\n";

	cout << "Mat_1�� ��ġ���" << endl;
	D3DXMatrixTranspose(&matResult, &matMatrix);
	PrintMat(&matResult); cout << "\n\n";

	cout << "Mat_1�� ȸ�����" << endl;
	// X���� �������� ȸ���� �� ����� ���ϴ� �Լ�
	D3DXMatrixRotationX(&matMatrix, 0.3f); // ȸ�� ��Ŀ� ���ؼ��� ������ ������� ���� �ϱ⿡ ���
	PrintMat(&matMatrix); cout << "\n\n";

	cout << "�� ȸ������� �����" << endl;
	D3DXMatrixInverse(&matResult, NULL, &matMatrix);
	PrintMat(&matResult); cout << "\n\n";
}