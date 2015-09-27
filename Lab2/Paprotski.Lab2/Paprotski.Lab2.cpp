// Paprotski.Lab2.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Paprotski.Lab2.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна

HWND hWndListL;
HWND hWndListR;
HWND hWndEdit;
HWND hWndButtonAdd;
HWND hWndButtonClear;
HWND hWndButtonToRight;
HWND hWndButtonDelete;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

// CRUD for ListBox
void AddItem(HWND hwnd, PTSTR pstr, HBITMAP hbmp);
void DeleteItem(HWND hwnd, int index);
int SelectedItem(HWND hwnd);
void ToRightElement(HWND firstHwnd, HWND secondHwnd);
void ListsClear(HWND firstHwnd, HWND secondHwnd);

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
	LoadString(hInstance, IDC_PAPROTSKILAB2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAPROTSKILAB2));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAPROTSKILAB2));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PAPROTSKILAB2);
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
      CW_USEDEFAULT, CW_USEDEFAULT, 575, 350, NULL, NULL, hInstance, NULL);

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
		hWndListL = CreateWindowEx(NULL, TEXT("LISTBOX"), NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | LBS_NOTIFY, 
			140, 10, 200, 280, hWnd, (HMENU)IDC_LEFT_LIST, hInst, NULL);

		hWndListR = CreateWindowEx(NULL, TEXT("LISTBOX"), NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | LBS_NOTIFY,
			350, 10, 200, 280, hWnd, (HMENU)IDC_RIGHT_LIST, hInst, NULL);

		hWndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
			10, 10, 120, 30, hWnd, (HMENU)IDC_EDIT, hInst, NULL);

		hWndButtonAdd = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Add"), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			10, 50, 120, 30, hWnd, (HMENU)IDC_BUTTON_ADD, hInst, NULL);

		hWndButtonClear = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Clear"), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			10, 90, 120, 30, hWnd, (HMENU)IDC_BUTTON_CLEAR, hInst, NULL);

		hWndButtonToRight = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("ToRight"), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			10, 130, 120, 30, hWnd, (HMENU)IDC_BUTTON_TO_RIGHT, hInst, NULL);

		hWndButtonDelete = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Delete"), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			10, 170, 120, 30, hWnd, (HMENU)IDC_BUTTON_DELETE, hInst, NULL);
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
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDC_BUTTON_ADD:
			try
			{
				TCHAR buffer[1 << 10];
				GetWindowText(hWndEdit, buffer, sizeof(buffer));
				AddItem(hWndListL, buffer, NULL);
			}
			catch (...)
			{
				MessageBox(hWnd, L"Try again!", L"Oops", MB_OK);
			}
			break;
		case IDC_BUTTON_CLEAR:
			ListsClear(hWndListL, hWndListR);
			break;
		case IDC_BUTTON_DELETE:
			DeleteItem(hWndListL, SelectedItem(hWndListL));
			DeleteItem(hWndListR, SelectedItem(hWndListR));
			break;
		case IDC_BUTTON_TO_RIGHT:
			ToRightElement(hWndListL, hWndListR);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
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


void ToRightElement(HWND firstHwnd, HWND secondHwnd)
{
	int itemIndex = SelectedItem(firstHwnd);

	if (itemIndex == -1) return;

	int textLength = SendMessage(firstHwnd, LB_GETTEXTLEN, itemIndex, 0);

	auto textBuffer = new TCHAR[textLength + 1];
	SendMessage(firstHwnd, LB_GETTEXT, itemIndex, (LPARAM)textBuffer);

	AddItem(secondHwnd, textBuffer, NULL);
	DeleteItem(firstHwnd, itemIndex);

	delete[] textBuffer;
}

void AddItem(HWND hwnd, PTSTR pstr, HBITMAP hbmp)
{
	if (pstr == NULL || pstr[0] == 0) return;
	if (SendMessage(hwnd, LB_FINDSTRINGEXACT, -1, (LPARAM)pstr) != -1) return;

	int lbItem = SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)pstr);
	SendMessage(hwnd, LB_SETITEMDATA, (WPARAM)lbItem, (LPARAM)hbmp);
}

void DeleteItem(HWND hwnd, int index)
{
	if (index == -1) return;

	SendMessage(hwnd, LB_DELETESTRING, index, NULL);
}

int SelectedItem(HWND hwnd)
{
	return SendMessage(hwnd, LB_GETCURSEL, NULL, NULL);
}

void ListsClear(HWND firstHwnd, HWND secondHwnd)
{
	SendMessage(firstHwnd, LB_RESETCONTENT, NULL, NULL);
	SendMessage(secondHwnd, LB_RESETCONTENT, NULL, NULL);
}