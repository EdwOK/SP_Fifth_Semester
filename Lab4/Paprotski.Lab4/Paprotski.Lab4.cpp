// Paprotski.Lab4.cpp: ���������� ����� ����� ��� ����������.
//

#include "stdafx.h"
#include "Paprotski.Lab4.h"

#define MAX_LOADSTRING 100
#define ID_TIMER 1 

// ���������� ����������:
HINSTANCE hInst;								// ������� ���������
TCHAR szTitle[MAX_LOADSTRING];					// ����� ������ ���������
TCHAR szWindowClass[MAX_LOADSTRING];			// ��� ������ �������� ����

HBITMAP bitmap = NULL;
POINT cursor, image = { 0, 0 };
bool IsMove = false;
int frameNumber, nTimerID; 

// ��������� ���������� �������, ���������� � ���� ������ ����:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void DrawImage(HDC hdc);
void UpdateImage();

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
	LoadString(hInstance, IDC_PAPROTSKILAB4, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ��������� ������������� ����������:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAPROTSKILAB4));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAPROTSKILAB4));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PAPROTSKILAB4);
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
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

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
		bitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// ��������� ����� � ����:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_EXECUTE:
			if (nTimerID != NULL) break;

			nTimerID = SetTimer(hWnd, ID_TIMER, 70, NULL);
			break;
		case IDM_STOP:
			if (nTimerID == NULL) break;

			KillTimer(hWnd, ID_TIMER);
			nTimerID = NULL;
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_TIMER:
		if (IsMove)
		{
			UpdateImage();
			InvalidateRect(hWnd, NULL, true);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		DrawImage(hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		IsMove = true;
		GetCursorPos(&cursor);
		break;
	case WM_DESTROY:
		DeleteObject(bitmap);
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

void UpdateImage()
{
	switch (frameNumber)
	{
		case 1:
			bitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
			break;
		case 2:
			bitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
			break;
		case 3:
			bitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
			break;
		case 4:
			bitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));
			break;
		default:
			bitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
			frameNumber = NULL;
	}

	frameNumber++;
}

void DrawImage(HDC hdc)
{
	BITMAP bm;
	auto hdcMem = CreateCompatibleDC(hdc);
	auto hbmOld = (HBITMAP) SelectObject(hdcMem, bitmap);

	GetObject(bitmap, sizeof(bm), &bm);

	if (IsMove)
	{
		auto offset = 5;

		image.x += (cursor.x - bm.bmWidth - 120 > image.x) ? offset : -offset;
		image.y += (cursor.y - bm.bmHeight - 130 > image.y) ? offset : -offset;

		if (abs(cursor.x - bm.bmWidth - image.x - 120) <= offset && abs(cursor.y - bm.bmHeight - image.y - 130) <= offset)
		{
			IsMove = false;
		}
	}

	BitBlt(hdc, image.x, image.y, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);
}