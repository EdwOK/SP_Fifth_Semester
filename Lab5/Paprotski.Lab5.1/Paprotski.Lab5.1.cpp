// Paprotski.Lab5.1.cpp: ���������� ����� ����� ��� ����������.
//

#include "stdafx.h"
#include "Paprotski.Lab5.1.h"

#define MAX_LOADSTRING 100

// ���������� ����������:
HINSTANCE hInst;								// ������� ���������
TCHAR szTitle[MAX_LOADSTRING];					// ����� ������ ���������
TCHAR szWindowClass[MAX_LOADSTRING];			// ��� ������ �������� ����

// ��������� ���������� �������, ���������� � ���� ������ ����:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HWND hWndRButtonRed;
HWND hWndRButtonGreen;
HWND hWndRButtonBlue;

HWND hWndRButtonCircle;
HWND hWndRButtonSquare;
HWND hWndRButtonDiamond;
HWND hWndRButtonStar;

HWND hWndCheckBox;
HWND hwDispatch;

typedef struct tagMYREC
{
	INT colorNumber;
	INT figureNumber;
	BOOL IsDraw;
} MYREC;

COPYDATASTRUCT cds;
MYREC MyRec = { 0, 0, false };

void SendDrawMessage(HWND hWnd, int wmId);

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
	LoadString(hInstance, IDC_PAPROTSKILAB51, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ��������� ������������� ����������:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAPROTSKILAB51));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAPROTSKILAB51));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PAPROTSKILAB51);
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
      CW_USEDEFAULT, CW_USEDEFAULT, 210, 200, NULL, NULL, hInstance, NULL);

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
		hWndRButtonRed = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Red"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
			10, 10, 80, 30, hWnd, (HMENU)IDM_RBUTTON_RED, hInst, NULL);

		hWndRButtonGreen = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Green"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			10, 40, 80, 30, hWnd, (HMENU)IDM_RBUTTON_GREEN, hInst, NULL);

		hWndRButtonBlue = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Blue"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			10, 70, 80, 30, hWnd, (HMENU)IDM_RBUTTON_BLUE, hInst, NULL);

		hWndRButtonCircle = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Circle"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
			100, 10, 80, 30, hWnd, (HMENU)IDM_RBUTTON_CIRCLE, hInst, NULL);

		hWndRButtonSquare = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Square"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			100, 40, 80, 30, hWnd, (HMENU)IDM_RBUTTON_SQUARE, hInst, NULL);

		hWndRButtonDiamond = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Diamond"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			100, 70, 80, 30, hWnd, (HMENU)IDM_RBUTTON_DIAMOND, hInst, NULL);

		hWndRButtonStar = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Star"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			100, 100, 80, 30, hWnd, (HMENU)IDM_RBUTTON_STAR, hInst, NULL);

		hWndCheckBox = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Draw"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
			10, 100, 80, 30, hWnd, (HMENU)IDM_CHECKBOX, hInst, NULL);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// ��������� ����� � ����:
		switch (wmId)
		{
		case IDM_RBUTTON_RED:
		case IDM_RBUTTON_GREEN:
		case IDM_RBUTTON_BLUE:
			MyRec.colorNumber = wmId;
			SendDrawMessage(hWnd, wmId);
			break;
		case IDM_RBUTTON_CIRCLE:
		case IDM_RBUTTON_DIAMOND:
		case IDM_RBUTTON_SQUARE:
		case IDM_RBUTTON_STAR:
			MyRec.figureNumber = wmId;
			SendDrawMessage(hWnd, wmId);
			break;
		case IDM_CHECKBOX:
			MyRec.IsDraw = !MyRec.IsDraw;
			SendDrawMessage(hWnd, wmId);
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
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �������� ����� ��� ���������...
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

void SendDrawMessage(HWND hWnd, int wmId)
{
	if (MyRec.colorNumber == 0 && MyRec.figureNumber == 0) return;

	hwDispatch = FindWindow(NULL, _T("Paprotski.Lab5.2"));
	if (hwDispatch == NULL) return;

	cds.dwData = wmId;
	cds.cbData = sizeof(MyRec);
	cds.lpData = &MyRec;


	SendMessage(hwDispatch, WM_COPYDATA, (WPARAM)(HWND)hWnd, (LPARAM)(LPVOID)&cds);
}