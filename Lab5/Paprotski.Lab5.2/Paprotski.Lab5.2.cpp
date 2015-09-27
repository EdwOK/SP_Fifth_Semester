// Paprotski.Lab5.2.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Paprotski.Lab5.2.h"

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

typedef struct tagMYREC
{
	INT colorNumber; 
	INT figureNumber;
	BOOL IsDraw;
} MYREC;

PCOPYDATASTRUCT pcds;
MYREC MyRec;
POINT mouseCursor;

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
	LoadString(hInstance, IDC_PAPROTSKILAB52, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAPROTSKILAB52));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAPROTSKILAB52));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PAPROTSKILAB52);
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
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_LBUTTONDOWN:
		GetCursorPos(&mouseCursor);
		if (MyRec.IsDraw) InvalidateRect(hWnd, NULL, true);
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		HBRUSH hBrush = CreateSolidBrush(
			RGB(MyRec.colorNumber == IDM_RBUTTON_RED ? 255 : 0,
				MyRec.colorNumber == IDM_RBUTTON_GREEN ? 255 : 0,
				MyRec.colorNumber == IDM_RBUTTON_BLUE ? 255 : 0)
		);
		SelectObject(hdc, hBrush);

		switch (MyRec.figureNumber)
		{
			case IDM_RBUTTON_CIRCLE:
				Ellipse(hdc, mouseCursor.x - 200, mouseCursor.y - 190, mouseCursor.x, mouseCursor.y);
				break;
			case IDM_RBUTTON_SQUARE:
				Rectangle(hdc, mouseCursor.x - 200, mouseCursor.y - 180, mouseCursor.x, mouseCursor.y);
				break;
			case IDM_RBUTTON_DIAMOND:
			{
				POINT diamond[5];

				diamond[0].x = mouseCursor.x;
				diamond[0].y = mouseCursor.y + 110;

				diamond[1].x = mouseCursor.x + 80;
				diamond[1].y = mouseCursor.y;

				diamond[2].x = mouseCursor.x;
				diamond[2].y = mouseCursor.y - 110;

				diamond[3].x = mouseCursor.x - 80;
				diamond[3].y = mouseCursor.y;

				diamond[4].x = mouseCursor.x;
				diamond[4].y = mouseCursor.y + 110;

				Polygon(hdc, diamond, 5);
				break;
			}
			case IDM_RBUTTON_STAR:
			{
				POINT star[11];

				star[0].x = mouseCursor.x;
				star[0].y = mouseCursor.y + 50;

				star[1].x = mouseCursor.x + 65;
				star[1].y = mouseCursor.y + 90;

				star[2].x = mouseCursor.x + 40;
				star[2].y = mouseCursor.y + 15;

				star[3].x = mouseCursor.x + 100;
				star[3].y = mouseCursor.y - 30;

				star[4].x = mouseCursor.x + 30;
				star[4].y = mouseCursor.y - 30;

				star[5].x = mouseCursor.x;
				star[5].y = mouseCursor.y - 100;

				star[6].x = mouseCursor.x - 30;
				star[6].y = mouseCursor.y - 30;

				star[7].x = mouseCursor.x - 100;
				star[7].y = mouseCursor.y - 30;

				star[8].x = mouseCursor.x - 40;
				star[8].y = mouseCursor.y + 15;

				star[9].x = mouseCursor.x - 65;
				star[9].y = mouseCursor.y + 90;

				star[10].x = mouseCursor.x;
				star[10].y = mouseCursor.y + 50;

				Polygon(hdc, star, 11);
				break;
			}
		}

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_COPYDATA:
		pcds = (PCOPYDATASTRUCT)lParam;
		MyRec.IsDraw = ((MYREC*)(pcds->lpData))->IsDraw;
		MyRec.colorNumber = ((MYREC*)(pcds->lpData))->colorNumber;
		MyRec.figureNumber = ((MYREC*)(pcds->lpData))->figureNumber;
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
