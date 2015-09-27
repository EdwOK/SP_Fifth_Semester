// Paprotski.Lab3.cpp: ���������� ����� ����� ��� ����������.
//

#include "stdafx.h"
#include "Paprotski.Lab3.h"

#define MAX_LOADSTRING 100

// ���������� ����������:
HINSTANCE hInst;								// ������� ���������
TCHAR szTitle[MAX_LOADSTRING];					// ����� ������ ���������
TCHAR szWindowClass[MAX_LOADSTRING];			// ��� ������ �������� ����

HWND hWndFirstButton;
HWND hWndSecondButton;
bool IsRender = false;

// ��������� ���������� �������, ���������� � ���� ������ ����:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void DrawImage(HDC hdc);
void DrawButton(LPDRAWITEMSTRUCT lpInfo);
void DrawBitmap(HDC hDC, INT posX, INT posY, HBITMAP hBitmap);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���������� ��� �����.
	MSG msg;
	HACCEL hAccelTable;

	// ������������� ���������� �����
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PAPROTSKILAB3, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ��������� ������������� ����������:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAPROTSKILAB3));

	// ���� ��������� ���������:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  �������: MyRegisterClass()
//
//  ����������: ������������ ����� ����.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAPROTSKILAB3));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PAPROTSKILAB3);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   �������: InitInstance(HINSTANCE, int)
//
//   ����������: ��������� ��������� ���������� � ������� ������� ����.
//
//   �����������:
//
//        � ������ ������� ���������� ���������� ����������� � ���������� ����������, � �����
//        ��������� � ��������� �� ����� ������� ���� ���������.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��������� ���������� ���������� � ���������� ����������

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �������: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����������:  ������������ ��������� � ������� ����.
//
//  WM_COMMAND	- ��������� ���� ����������
//  WM_PAINT	-��������� ������� ����
//  WM_DESTROY	 - ������ ��������� � ������ � ���������.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		hWndFirstButton = CreateWindowEx(NULL, _T("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
			10, 10, 100, 50, hWnd, (HMENU)IDC_FIRST_BUTTON, hInst, NULL);

		hWndSecondButton = CreateWindowEx(NULL, _T("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
			120, 10, 100, 50, hWnd, (HMENU)IDC_SECOND_BUTTON, hInst, NULL);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// ��������� ����� � ����:
		switch (wmId)
		{
		case IDC_FIRST_BUTTON:
			IsRender = true;
			InvalidateRect(hWnd, NULL, false);
			break;
		case IDC_SECOND_BUTTON:
			IsRender = false;
			InvalidateRect(hWnd, NULL, true);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_DRAWITEM:
		DrawButton((LPDRAWITEMSTRUCT)lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (IsRender) DrawImage(hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���������� ��������� ��� ���� "� ���������".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void DrawButton(LPDRAWITEMSTRUCT lpInfo)
{
	if (lpInfo->CtlType != ODT_BUTTON) return;

	HBITMAP hbitmap;
	int ResourceID;

	switch (lpInfo->CtlID)
	{
		case IDC_FIRST_BUTTON:
		case IDC_SECOND_BUTTON:
		{
			ResourceID = IDB_BITMAP1;
			break;
		}
		default:
			return;
	}

	if (lpInfo->itemState & ODS_SELECTED)
	{
		ResourceID = IDB_BITMAP2;
	}

	hbitmap = LoadBitmap(hInst, MAKEINTRESOURCE(ResourceID));

	if (!hbitmap) return;

	if ((lpInfo->itemAction & ODA_DRAWENTIRE) || (lpInfo->itemAction & ODA_SELECT))
	{
		DrawBitmap(lpInfo->hDC, (lpInfo->rcItem).left, (lpInfo->rcItem).top, hbitmap);
	}

	DeleteObject(hbitmap);
}

void DrawBitmap(HDC hDC, INT posX, INT posY, HBITMAP hBitmap)
{
	HBITMAP hbm;
	HDC hMemDC;
	BITMAP bm;
	POINT ptSize, ptOrg;

	// ������� �������� ������, �����������
	// � ���������� �����������
	hMemDC = CreateCompatibleDC(hDC);

	// �������� ����������� bitmap � �������� ������
	hbm = (HBITMAP)SelectObject(hMemDC, hBitmap);

	// ���� �� ���� ������, ���������� ������
	if (hbm)
	{
		// ��� ��������� ������ ������������� ��� ��
		// ����� �����������, ��� ������������ �
		// ��������� �����������
		SetMapMode(hMemDC, GetMapMode(hDC));

		// ���������� ������� �����������
		GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);

		ptSize.x = bm.bmWidth;  // ������
		ptSize.y = bm.bmHeight; // ������

		// ����������� ���������� ���������� � ����������
		// ��� ���������� ������
		DPtoLP(hDC, &ptSize, 1);

		ptOrg.x = 0;
		ptOrg.y = 0;

		// ����������� ���������� ���������� � ����������
		// ��� ��������� ������
		DPtoLP(hMemDC, &ptOrg, 1);

		// ������ ����������� bitmap
		BitBlt(hDC, posX, posY, ptSize.x, ptSize.y, hMemDC, ptOrg.x, ptOrg.y, SRCCOPY);

		// ��������������� �������� ������
		SelectObject(hMemDC, hbm);
	}

	DeleteDC(hMemDC);
}

void DrawImage(HDC hdc)
{
	HBRUSH hBrush;
	POINT roof[4], pipe[5];

	hBrush = CreateSolidBrush(RGB(124, 255, 47));
	SelectObject(hdc, hBrush);

	Rectangle(hdc, 283, 480, 725, 520);

	hBrush = CreateSolidBrush(RGB(158, 255, 134));
	SelectObject(hdc, hBrush);

	Rectangle(hdc, 320, 288, 690, 481);

	hBrush = CreateSolidBrush(RGB(255, 234, 0));
	SelectObject(hdc, hBrush);

	Rectangle(hdc, 440, 330, 570, 420);

	hBrush = CreateSolidBrush(RGB(47, 84, 38));
	SelectObject(hdc, hBrush);

	roof[0].x = 280;
	roof[0].y = 288;

	roof[1].x = 500;
	roof[1].y = 102;

	roof[2].x = 720;
	roof[2].y = 288;

	roof[3].x = 280;
	roof[3].y = 288;

	Polygon(hdc, roof, 4);

	MoveToEx(hdc, 503, 330, NULL);
	LineTo(hdc, 503, 420);

	MoveToEx(hdc, 440, 373, NULL);
	LineTo(hdc, 569, 373);

	hBrush = CreateSolidBrush(RGB(255, 234, 0));
	SelectObject(hdc, hBrush);

	Ellipse(hdc, 463, 180, 543, 246);

	hBrush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(hdc, hBrush);

	pipe[0].x = 598;
	pipe[0].y = 181;

	pipe[1].x = 598;
	pipe[1].y = 145;

	pipe[2].x = 624;
	pipe[2].y = 145;

	pipe[3].x = 624;
	pipe[3].y = 205;

	pipe[4].x = 598;
	pipe[4].y = 185;

	Polygon(hdc, pipe, 5);

	DeleteObject(hBrush);
	DeleteObject(hdc);
}