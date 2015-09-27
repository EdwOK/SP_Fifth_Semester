// Paprotski.Lab7.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Paprotski.Lab7.h"

#define MAX_LOADSTRING 100
#define THREAD_COUNT 6
#define MAX_SEM_COUNT 2

// Глобальные переменные:
HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HWND hWndButtonStop;
HWND hWndButtonStart;
HWND hWindow;

DWORD WINAPI ThreadProc(LPVOID lpParam);

HANDLE hThreadsArray[THREAD_COUNT];
HANDLE hSemaphore;

void InitThreads();
void PauseThreads();
void ResumeThreads();
void CloseThreads();

void DrawImage(HDC hdc);

int posX = 10, posY = 130;
bool pauseThreads = false;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: разместите код здесь.
	MSG msg;
	HACCEL hAccelTable;

	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PAPROTSKILAB7, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAPROTSKILAB7));

	// Цикл основного сообщения:
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
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAPROTSKILAB7));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PAPROTSKILAB7);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   hWindow = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
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
		break;
		case WM_COMMAND:
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			// Разобрать выбор в меню:
			switch (wmId)
			{
				case IDM_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDC_BUTTON_START:
					if (pauseThreads)
					{
						ResumeThreads();
					}
					else
					{
						InitThreads();
					}
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
			DrawImage(hdc);
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

// Обработчик сообщений для окна "О программе".
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

// 10 130
void DrawImage(HDC hdc)
{
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(hdc, hBrush);

	RECT rect;
	GetClientRect(hWindow, &rect);
	Rectangle(hdc, posX, posY, posX + 160, posY + 100);

	hBrush = CreateSolidBrush(RGB(0, 255, 0));
	SelectObject(hdc, hBrush);
	Rectangle(hdc, posX + 159, posY + 35, posX + 159 + 70, posY + 35 + 65);

	hBrush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, hBrush);

	Rectangle(hdc, posX + 159 + 5, posY + 40, posX + 159 + 65, posY + 35 + 55);

	hBrush = CreateSolidBrush(RGB(0, 0, 255));
	SelectObject(hdc, hBrush);
	Rectangle(hdc, posX, posY + 99, posX + 270, posY + 99 + 40);

	hBrush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(hdc, hBrush);

	Ellipse(hdc, posX + 10, posY + 120, posX + 50, posY + 160);
	Ellipse(hdc, posX + 210, posY + 120, posX + 250, posY + 160);
}

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	DWORD dwWaitResult;

	while (true)
	{
		dwWaitResult = WaitForSingleObject(hSemaphore, INFINITE);  

		if (dwWaitResult == WAIT_OBJECT_0)
		{
			RECT rect;
			GetClientRect(hWindow, &rect);
			InvalidateRect(hWindow, &rect, true);
			
			if (posX >= 500)
			{
				posX = 10;
				posY = 130;
			}

			posX += 5;
			posY += (INT)lpParam & 1 ? 5 : -5;

			Sleep(100);
			ReleaseSemaphore(hSemaphore, 1, NULL);
		}
	}

	return true;
}

void InitThreads()
{
	hSemaphore = CreateSemaphore(NULL, 1, MAX_SEM_COUNT, NULL);
	if (hSemaphore == NULL) return;

	for (int i = 0; i < THREAD_COUNT; i++)
	{
		hThreadsArray[i] = CreateThread(NULL, 0, ThreadProc, (LPVOID)(i + 1), 0, NULL);
	}
}

void CloseThreads()
{
	WaitForMultipleObjects(THREAD_COUNT, hThreadsArray, TRUE, INFINITE);

	for (int i = 0; i < THREAD_COUNT; i++)
	{
		CloseHandle(hThreadsArray[i]);
	}

	CloseHandle(hSemaphore);
}

void ResumeThreads()
{
	if (!pauseThreads) return;

	for (int i = 0; i < THREAD_COUNT; i++)
	{
		DWORD toWait = WaitForSingleObject(hThreadsArray[i], 0);
		if (toWait == WAIT_OBJECT_0) continue;

		ResumeThread(hThreadsArray[i]);
	}

	pauseThreads = false;
}

void PauseThreads()
{
	if (pauseThreads) return;

	for (int i = 0; i < THREAD_COUNT; i++)
	{
		DWORD toWait = WaitForSingleObject(hThreadsArray[i], 0);
		if (toWait == WAIT_OBJECT_0) continue;

		SuspendThread(hThreadsArray[i]);
	}

	pauseThreads = true;
}