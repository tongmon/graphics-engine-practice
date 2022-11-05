#include <windows.h>
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9math.h>
#include "CGameEdu01.h"
#include "resource.h"

#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib,"d3dx9.lib")
#pragma comment (lib,"DxErr.lib")
#pragma comment (lib,"d3dxof.lib")
#pragma comment (lib,"winmm.lib")

#define MAX_LOADSTRING 100

CGameEdu01 Mechanism;

// ���� ����:
HINSTANCE hInst; // ���� �ν��Ͻ��Դϴ�.
HWND g_hWnd, g_hMenuWnd; // ���� ������, ���̾�α� ������ �ڵ�
int g_nDlgWidth, g_nDlgHeight; // ���̾�α� ����, ���� ũ��

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam); // ���� ������

INT_PTR CALLBACK MenuDlgProc(HWND hDlg, UINT nMsg, 
	WPARAM wParam, LPARAM lParam); // ��Ʈ�� �г� ���̾�α�

LPCTSTR lpszClass = TEXT("DX Sample"); // Ÿ��Ʋ �̸�

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

	hInst = hInstance;
	RECT rect = { 0, 0, 900, 900 };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, true);

	g_hWnd = hwnd = CreateWindow(lpszClass, //�����찡 �����Ǹ� �ڵ�(hwnd)�� ��ȯ
		lpszClass, //������ Ŭ����, Ÿ��Ʋ �̸�
		WS_OVERLAPPEDWINDOW, //������ ��Ÿ��
		CW_USEDEFAULT, //������ ��ġ, x��ǥ
		CW_USEDEFAULT, //������ ��ġ, y��ǥ
		rect.right - rect.left, //������ ��   
		rect.bottom - rect.top, //������ ����   
		NULL, //�θ� ������ �ڵ� 
		NULL, //�޴� �ڵ�
		hInstance,     //���� ���α׷� ID
		NULL      //������ ������ ����
	);

	ShowWindow(hwnd, nCmdShow); //�������� ȭ�� ���
	UpdateWindow(hwnd); //O/S �� WM_PAINT �޽��� ����

	RECT rtWindow, rtDlg;
	GetWindowRect(g_hWnd, &rtWindow); // ��ũ���󿡼� ������ ��ǥ

	g_hMenuWnd = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, MenuDlgProc);
	GetWindowRect(g_hMenuWnd, &rtDlg); // ���̾�α� ũ��
	g_nDlgWidth = rtDlg.right - rtDlg.left + 1; // ���̾�α� ���� ����
	g_nDlgHeight = rtDlg.bottom - rtDlg.top + 1; // ���̾�α� ���� ����

	// ���̾�α� â�� ������ ���� �����쿡 �� �ٿ��ֱ� ���� ��� 
	MoveWindow(g_hMenuWnd, rtWindow.right, rtWindow.top, g_nDlgWidth, g_nDlgHeight, TRUE);  

	Mechanism.D3DInit(g_hWnd);

	// ��Ʈ�� �� �ʱ�ȭ 
	char string[10];
	snprintf(string, sizeof(string), "%.1f", Mechanism.m_Material.Diffuse.r);
	SetDlgItemText(g_hMenuWnd, IDC_EDIT1, string);
	snprintf(string, sizeof(string), "%.1f", Mechanism.m_Material.Diffuse.g);
	SetDlgItemText(g_hMenuWnd, IDC_EDIT2, string);
	snprintf(string, sizeof(string), "%.1f", Mechanism.m_Material.Diffuse.b);
	SetDlgItemText(g_hMenuWnd, IDC_EDIT3, string);

	snprintf(string, sizeof(string), "%.1f", Mechanism.m_Light.Diffuse.r);
	SetDlgItemText(g_hMenuWnd, IDC_EDIT4, string);
	snprintf(string, sizeof(string), "%.1f", Mechanism.m_Light.Diffuse.g);
	SetDlgItemText(g_hMenuWnd, IDC_EDIT5, string);
	snprintf(string, sizeof(string), "%.1f", Mechanism.m_Light.Diffuse.b);
	SetDlgItemText(g_hMenuWnd, IDC_EDIT6, string);
	snprintf(string, sizeof(string), "%.1f", Mechanism.m_Light.Specular.r);
	SetDlgItemText(g_hMenuWnd, IDC_EDIT7, string);
	snprintf(string, sizeof(string), "%.1f", Mechanism.m_Light.Specular.g);
	SetDlgItemText(g_hMenuWnd, IDC_EDIT8, string);
	snprintf(string, sizeof(string), "%.1f", Mechanism.m_Light.Specular.b);
	SetDlgItemText(g_hMenuWnd, IDC_EDIT9, string);
	snprintf(string, sizeof(string), "%.1f", Mechanism.m_Light.Ambient.r);
	SetDlgItemText(g_hMenuWnd, IDC_EDIT10, string);
	snprintf(string, sizeof(string), "%.1f", Mechanism.m_Light.Ambient.g);
	SetDlgItemText(g_hMenuWnd, IDC_EDIT11, string);
	snprintf(string, sizeof(string), "%.1f", Mechanism.m_Light.Ambient.b);
	SetDlgItemText(g_hMenuWnd, IDC_EDIT12, string);

	snprintf(string, sizeof(string), "%.1f", Mechanism.m_Light.Direction.x);
	SetDlgItemText(g_hMenuWnd, IDC_EDIT13, string);
	snprintf(string, sizeof(string), "%.1f", Mechanism.m_Light.Direction.y);
	SetDlgItemText(g_hMenuWnd, IDC_EDIT14, string);
	snprintf(string, sizeof(string), "%.1f", Mechanism.m_Light.Direction.z);
	SetDlgItemText(g_hMenuWnd, IDC_EDIT15, string);

	ShowWindow(g_hMenuWnd, nCmdShow); //�������� ȭ�� ���

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
	RECT rtWindow;
	// PAINTSTRUCT ps;

	switch (iMsg)
	{
	case WM_CREATE:
		break;

	case WM_MOVE: // ������ �̵��ÿ� ���̾�α� �����쵵 ���� �ٿ��� �������ֱ� ���� �뵵
		GetWindowRect(hwnd, &rtWindow); // ��ũ���󿡼� ������ ��ǥ
		MoveWindow(g_hMenuWnd, rtWindow.right, rtWindow.top, g_nDlgWidth, g_nDlgHeight, TRUE);
		break;

	case WM_DESTROY:
		Mechanism.Cleanup();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam); //CASE���� ���ǵ��� ���� �޽����� Ŀ���� ó���ϵ��� �޽��� ����
}

// ���̾�α� ��Ʈ�� �г�, ���̾�α� ���ᰡ �ʿ���⿡ EndDialog() �Լ� ����.
INT_PTR CALLBACK MenuDlgProc(HWND hDlg, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	char str[10];
	float fRed, fGreen, fBlue;

	switch (nMsg)
	{
	case WM_INITDIALOG:
		break;
	case WM_COMMAND:
		if (wParam == IDC_BUTTON1)  // ��Ƽ���� �¾�
		{
			GetDlgItemText(hDlg, IDC_EDIT1, str, 9);
			fRed = atof(str);
			GetDlgItemText(hDlg, IDC_EDIT2, str, 9);
			fGreen = atof(str);
			GetDlgItemText(hDlg, IDC_EDIT3, str, 9);
			fBlue = atof(str);
			Mechanism.SetMaterial(fRed, fGreen, fBlue);
		}
		else if (wParam == IDC_BUTTON2) // ����Ʈ �¾�
		{
			GetDlgItemText(hDlg, IDC_EDIT4, str, 9);
			Mechanism.m_Light.Diffuse.r = atof(str);
			GetDlgItemText(hDlg, IDC_EDIT5, str, 9);
			Mechanism.m_Light.Diffuse.g = atof(str);
			GetDlgItemText(hDlg, IDC_EDIT6, str, 9);
			Mechanism.m_Light.Diffuse.b = atof(str);

			GetDlgItemText(hDlg, IDC_EDIT7, str, 9);
			Mechanism.m_Light.Specular.r = atof(str);
			GetDlgItemText(hDlg, IDC_EDIT8, str, 9);
			Mechanism.m_Light.Specular.g = atof(str);
			GetDlgItemText(hDlg, IDC_EDIT9, str, 9);
			Mechanism.m_Light.Specular.b = atof(str);

			GetDlgItemText(hDlg, IDC_EDIT10, str, 9);
			Mechanism.m_Light.Ambient.r = atof(str);
			GetDlgItemText(hDlg, IDC_EDIT11, str, 9);
			Mechanism.m_Light.Ambient.g = atof(str);
			GetDlgItemText(hDlg, IDC_EDIT12, str, 9);
			Mechanism.m_Light.Ambient.b = atof(str);

			GetDlgItemText(hDlg, IDC_EDIT13, str, 9);
			Mechanism.m_Light.Direction.x = atof(str);
			GetDlgItemText(hDlg, IDC_EDIT14, str, 9);
			Mechanism.m_Light.Direction.y = atof(str);
			GetDlgItemText(hDlg, IDC_EDIT15, str, 9);
			Mechanism.m_Light.Direction.z = atof(str);

			Mechanism.SetDirectionalLight();
		}
		break;
	}
	return FALSE;
}