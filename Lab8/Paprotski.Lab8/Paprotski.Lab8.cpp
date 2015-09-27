// Paprotski.Lab8.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Paprotski.Lab8.h"
#include "psapi.h"
#include "tlhelp32.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HWND hWndListL;
HWND hWndListR;
HWND hWndButtonScanner;

DWORD saveProcessID = NULL;

void PrintModulesName(HWND hWnd, DWORD processID);
void ShowMenuProcessPriority(HWND hWnd, DWORD processID);
void ProcessScannerAndPrintHisName(HWND hWnd);

void AddItem(HWND hWnd, PTSTR pstr, DWORD processID);
DWORD GetSelectProccesID(HWND hWnd);
DWORD GetSelectID(HWND hWnd);

void SetProcessPriority(DWORD processID, DWORD priorityClass);
bool IsValidPriorityID(DWORD id);

TCHAR szPriorityName[6][15] = {
	_T("Below Normal"),
	_T("Normal"),
	_T("Above Normal"),
	_T("Idle"),
	_T("High"),
	_T("Real Time")
};

DWORD priorityArray[6] = {
	BELOW_NORMAL_PRIORITY_CLASS,
	NORMAL_PRIORITY_CLASS,
	ABOVE_NORMAL_PRIORITY_CLASS,
	IDLE_PRIORITY_CLASS,
	HIGH_PRIORITY_CLASS,
	REALTIME_PRIORITY_CLASS
};

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
	LoadString(hInstance, IDC_PAPROTSKILAB8, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAPROTSKILAB8));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAPROTSKILAB8));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PAPROTSKILAB8);
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
      CW_USEDEFAULT, CW_USEDEFAULT, 745, 385, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

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
		hWndListL = CreateWindowEx(NULL, TEXT("LISTBOX"), NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | LBS_NOTIFY | LBS_STANDARD,
			10, 10, 200, 280, hWnd, (HMENU)IDC_LEFT_LIST, hInst, NULL);

		hWndListR = CreateWindowEx(NULL, TEXT("LISTBOX"), NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | LBS_NOTIFY | LBS_STANDARD,
			220, 10, 500, 280, hWnd, (HMENU)IDC_RIGHT_LIST, hInst, NULL);

		hWndButtonScanner = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Scanner"), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			10, 290, 120, 30, hWnd, (HMENU)IDC_BUTTON_SCANNER, hInst, NULL);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDC_BUTTON_SCANNER:
		{
			SendMessage(hWndListL, LB_RESETCONTENT, NULL, NULL);
			ProcessScannerAndPrintHisName(hWndListL);
			break;
		}
		case IDC_LEFT_LIST:
		{
			if (wmEvent == LBN_DBLCLK)
			{
				SendMessage(hWndListR, LB_RESETCONTENT, NULL, NULL);

				auto processID = GetSelectProccesID(hWndListL);
				PrintModulesName(hWndListR, processID);
			}
			break;
		}
		case WM_RBUTTONUP:
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			if (IsValidPriorityID(wmId)) SetProcessPriority(saveProcessID, wmId);
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CONTEXTMENU:
		if ((HWND)wParam == hWndListL)
		{
			auto processID = GetSelectProccesID(hWndListL);
			ShowMenuProcessPriority(hWnd, processID);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: добавьте любой код отрисовки...
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

void PrintModulesName(HWND hWnd, DWORD processID)
{
	MODULEENTRY32 meModuleEntry;

	HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processID);
	if (hSnapshot == INVALID_HANDLE_VALUE) return;

	meModuleEntry.dwSize = sizeof(MODULEENTRY32);
	Module32First(hSnapshot, &meModuleEntry);

	do {
		AddItem(hWnd, meModuleEntry.szExePath, processID);
	} while (Module32Next(hSnapshot, &meModuleEntry));

	CloseHandle(hSnapshot);
}

void ProcessScannerAndPrintHisName(HWND hWnd)
{
	PROCESSENTRY32 peProcessEntry;

	HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) return;

	peProcessEntry.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnapshot, &peProcessEntry);

	do {
		AddItem(hWnd, peProcessEntry.szExeFile, peProcessEntry.th32ProcessID);
	} while (Process32Next(hSnapshot, &peProcessEntry));

	CloseHandle(hSnapshot);
}


void AddItem(HWND hWnd, PTSTR pstr, DWORD processID)
{
	if (pstr == NULL || pstr[0] == 0) return;

	int lbItem = SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)pstr);
	SendMessage(hWnd, LB_SETITEMDATA, (WPARAM)lbItem, (LPARAM)processID);
}

DWORD GetSelectProccesID(HWND hWnd)
{
	return SendMessage(hWnd, LB_GETITEMDATA, GetSelectID(hWnd), NULL);
}

DWORD GetSelectID(HWND hWnd)
{
	return SendMessage(hWnd, LB_GETCURSEL, NULL, NULL);
}

void ShowMenuProcessPriority(HWND hWnd, DWORD processID)
{
	HANDLE hProcess;
	HMENU hPopupMenu;

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, processID);
	if (hProcess == NULL || hWnd == NULL) return;

	saveProcessID = processID;

	DWORD priority = GetPriorityClass(hProcess);
	hPopupMenu = CreatePopupMenu();

	UINT flags;
	for (int i = 0; i < 6; i++)
	{
		flags = MF_BYPOSITION | MF_STRING | (priority == priorityArray[i] ? MF_CHECKED : MF_ENABLED);
		AppendMenu(hPopupMenu, flags, priorityArray[i], szPriorityName[i]);
	}
	SetForegroundWindow(hWnd);

	POINT point;
	GetCursorPos(&point);

	TrackPopupMenu(hPopupMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, 0, hWnd, NULL);
	DestroyMenu(hPopupMenu);
	CloseHandle(hProcess);
}

void SetProcessPriority(DWORD processID, DWORD priorityClass)
{
	if (processID == NULL || !IsValidPriorityID(priorityClass)) return;

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_SET_INFORMATION | PROCESS_VM_READ, false, processID);
	if (hProcess == NULL) return;

	SetPriorityClass(hProcess, priorityClass);
	saveProcessID = NULL;

	CloseHandle(hProcess);
}

bool IsValidPriorityID(DWORD id)
{
	for (int i = 0; i < 6; i++)
	{
		if (id == priorityArray[i])
		{
			return true;
		}
	}
	return false;
}