#include <windows.h>
#include <d3d9.h>
#include "CGameEdu01.h"

#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib,"d3dx9.lib")
#pragma comment (lib,"DxErr.lib")
#pragma comment (lib,"d3dxof.lib")
#pragma comment (lib,"winmm.lib")

// �ﰢ���� �׸� ��� ���� �ﰢ�� Ŭ������ ����� CGameEdu01 Ŭ���� ���� ��������� Ȱ���ϴ� ���
// ���� �� ����� ���� Ŭ���� ������ �ʰ� �����ڴ� �ͺ��� �������� ���� ���̰� ȿ�����̴�.
// �� �ڵ带 ���鼭 ���� �����ӿ�ũ�� ��� ȿ�������� �����Ǵ��� �˾Ƴ� �� �ִ�.
// �� �����ڴ� �ﰢ���� �׸� �� �� �׸��� �Ϳ��� �����ϸ� �ȴ�. 
// ����̽��� ��� �Ҵ�, ���� ������ ���� ���� ������ ��ũ�� ���ش�.
CGameEdu01 Mechanism;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam);

LPCTSTR lpszClass = TEXT("Api Sample"); // Ÿ��Ʋ �̸�
HWND g_hWnd;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{ //szCmdLine : Ŀ��Ʈ���� �󿡼� ���α׷� ���� �� ���޵� ���ڿ�
	HWND hwnd; //iCmdShow : �����찡 ȭ�鿡 ��µ� ����
	MSG msg;
	WNDCLASS WndClass; //WndClass ��� ����ü ���� 
	WndClass.style = CS_HREDRAW | CS_VREDRAW; //��½�Ÿ�� : ����/������ ��ȭ�� �ٽ� �׸�
	WndClass.lpfnWndProc = WndProc; //���ν��� �Լ���
	WndClass.cbClsExtra = 0; //O/S ��� ���� �޸� (Class)
	WndClass.cbWndExtra = 0; //O/s ��� ���� �޸� (Window)
	WndClass.hInstance = hInstance; //���� ���α׷� ID
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //������ ����
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //Ŀ�� ����
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//����   
	WndClass.lpszMenuName = NULL; //�޴� �̸�
	WndClass.lpszClassName = lpszClass; //Ŭ���� �̸�
	RegisterClass(&WndClass); //�ռ� ������ ������ Ŭ������ �ּ�

	g_hWnd = hwnd = CreateWindow(lpszClass, //�����찡 �����Ǹ� �ڵ�(hwnd)�� ��ȯ
		lpszClass, //������ Ŭ����, Ÿ��Ʋ �̸�
		WS_OVERLAPPEDWINDOW, //������ ��Ÿ��
		CW_USEDEFAULT, //������ ��ġ, x��ǥ
		CW_USEDEFAULT, //������ ��ġ, y��ǥ
		CW_USEDEFAULT, //������ ��   
		CW_USEDEFAULT, //������ ����   
		NULL, //�θ� ������ �ڵ� 
		NULL, //�޴� �ڵ�
		hInstance,     //���� ���α׷� ID
		NULL      //������ ������ ����
	);
	ShowWindow(hwnd, nCmdShow); //�������� ȭ�� ���
	UpdateWindow(hwnd); //O/S �� WM_PAINT �޽��� ����

	Mechanism.D3DInit(g_hWnd);

	// ���� �޽��� �����Դϴ�:
	while (true)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Update and Render
			Mechanism.Update();
			Mechanism.Render();
		}
	}
	return (int)msg.wParam;
}

HDC hdc;


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

	//PAINTSTRUCT ps;

	switch (iMsg)
	{
	case WM_CREATE:
		break;

	case WM_PAINT:
		Mechanism.Render();
		break;

	case WM_DESTROY:
		Mechanism.Cleanup();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam); //CASE���� ���ǵ��� ���� �޽����� Ŀ���� ó���ϵ��� �޽��� ����
}