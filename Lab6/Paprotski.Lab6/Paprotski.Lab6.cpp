// Paprotski.Lab6.cpp: ���������� ����� ����� ��� ����������.
//

#include "stdafx.h"
#include "commctrl.h"
#include "Paprotski.Lab6.h"

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

HWND hWndButtonStop;
HWND hWndButtonStart;
HWND hWndProgressBarFirst;
HWND hWndProgressBarSecond;

DWORD WINAPI MyThreadFunction(LPVOID lpParam);
void InitThreads();
void PauseThreads();
void ResumeThreads();

HANDLE hFirstThread, hSecondThread;

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
	LoadString(hInstance, IDC_PAPROTSKILAB6, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ��������� ������������� ����������:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAPROTSKILAB6));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAPROTSKILAB6));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH) (COLOR_WINDOW + 1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PAPROTSKILAB6);
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
      CW_USEDEFAULT, CW_USEDEFAULT, 250, 225, NULL, NULL, hInstance, NULL);

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
		hWndButtonStart = CreateWindowEx(NULL, _T("BUTTON"), _T("Start"), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			10, 10, 120, 30, hWnd, (HMENU)IDC_BUTTON_START, hInst, NULL);

		hWndButtonStop = CreateWindowEx(NULL, _T("BUTTON"), _T("Stop"), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			10, 50, 120, 30, hWnd, (HMENU)IDC_BUTTON_STOP, hInst, NULL);

		hWndProgressBarFirst = CreateWindowEx(NULL, PROGRESS_CLASS, (LPTSTR)NULL, WS_CHILD | WS_VISIBLE,
			10, 90, 200, 30, hWnd, (HMENU)IDC_PROGRESSBAR_FIRST, hInst, NULL);

		hWndProgressBarSecond = CreateWindowEx(NULL, PROGRESS_CLASS, (LPTSTR)NULL, WS_CHILD | WS_VISIBLE,
			10, 130, 200, 30, hWnd, (HMENU)IDC_PROGRESSBAR_SECOND, hInst, NULL);

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
		case IDC_BUTTON_START:
			InitThreads();
			break;
		case IDC_BUTTON_STOP:
			PauseThreads();
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

DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
	while (true)
	{
		HWND progressBar = (HWND)lpParam;

		if (progressBar == hWndProgressBarFirst)
		{
			Sleep(1000);
		}
		else if (progressBar == hWndProgressBarSecond)
		{
			Sleep(800);
		}

		SendMessage(progressBar, PBM_STEPIT, 0, 0);
	}

	return true;
}

void InitThreads()
{
	DWORD firstWait = WaitForSingleObject(hFirstThread, 0);
	DWORD secondWait = WaitForSingleObject(hSecondThread, 0);

	if (firstWait == WAIT_TIMEOUT || secondWait == WAIT_TIMEOUT)
	{
		ResumeThreads();
	}
	else
	{
		hFirstThread = CreateThread(NULL, 0, MyThreadFunction, hWndProgressBarFirst, 0, NULL);
		hSecondThread = CreateThread(NULL, 0, MyThreadFunction, hWndProgressBarSecond, 0, NULL);
	}
}

void PauseThreads()
{
	DWORD firstWait = WaitForSingleObject(hFirstThread, 0);
	DWORD secondWait = WaitForSingleObject(hSecondThread, 0);

	if (firstWait == WAIT_OBJECT_0 || secondWait == WAIT_OBJECT_0) return;

	SuspendThread(hFirstThread);
	SuspendThread(hSecondThread);
}

void ResumeThreads()
{
	DWORD firstWait = WaitForSingleObject(hFirstThread, 0);
	DWORD secondWait = WaitForSingleObject(hSecondThread, 0);

	if (firstWait == WAIT_OBJECT_0 && secondWait == WAIT_OBJECT_0) return;

	ResumeThread(hFirstThread);
	ResumeThread(hSecondThread);
}