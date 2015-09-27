// Paprotski.Lab9.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Paprotski.Lab9.h"

#define MAX_LOADSTRING 100
#define MAX_REGEDIT_PARAMS 13
#define MAX_PICTURES 7
#define MAX_VALUE_NAME 16383

// Глобальные переменные:
HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HWND hWndList;
HWND hWndPicturesList;
HWND hWndScanner;

HWND hWndEditKey;
HWND hWndEditButton;

DWORD saveSelectID = -1;

TCHAR regeditSettingsNames[MAX_REGEDIT_PARAMS][MAX_LOADSTRING] = {
	_T("Запретить IE предлагать использование по умолчанию"),
	_T("Показывать скрытые файлы, папки и диски"),
	_T("Показывать пустые диски в папке пк"),
	_T("Отображать Мой Компьютер на рабочем столе"),
	_T("Отключить предупреждение при открытии файлов, загруженных из интернета"),
	_T("Отключить значок Центр поддержки из области уведомлений"),
	_T("Изменить обои Windows"),
	_T("Swap клавиш мыши"),
	_T("Выполнение программ Администратором без запроса"),
	_T("Отключить автоматическое обновление системы"),
	_T("Отключить службу Центр обновления Windows"),
	_T("Отключить службу Брандмауэр Windows"),
	_T("Отключить сдужбу Центр обеспечения безопасности")
};

TCHAR regeditSettingsKeyPaths[MAX_REGEDIT_PARAMS][MAX_LOADSTRING] = {
	_T("Software\\Microsoft\\Internet Explorer\\Main"),
	_T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced"),
	_T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced"),
	_T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\HideDesktopIcons\\NewStartPanel"),
	_T("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Attachments"),
	_T("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer"),
	_T("Control Panel\\Desktop"),
	_T("Control Panel\\Mouse"),
	_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"),
	_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\WindowsUpdate\\Auto Update"),
	_T("SYSTEM\\CurrentControlSet\\Services\\wuauserv"),
	_T("SYSTEM\\CurrentControlSet\\Services\\MpsSvc"),
	_T("SYSTEM\\CurrentControlSet\\Services\\wscsvc")
};

TCHAR regeditSettingsKeyValues[MAX_REGEDIT_PARAMS][MAX_LOADSTRING] = {
	_T("Check_Associations"),
	_T("Hidden"),
	_T("HideDrivesWithNoMedia"),
	_T("{20D04FE0-3AEA-1069-A2D8-08002B30309D}"),
	_T("SaveZoneInformation"),
	_T("HideSCAHealth"),
	_T("Wallpaper"),
	_T("SwapMouseButtons"),
	_T("ConsentPromptBehaviorAdmin"),
	_T("AUOptions"),
	_T("Start"),
	_T("Start"),
	_T("Start")
};

TCHAR directory[MAX_LOADSTRING] = _T("C:\\Users\\Эдуард\\Pictures\\");

TCHAR pathPictures[MAX_PICTURES][MAX_LOADSTRING] = {
	_T("windows 7 aero.jpg"),
	_T("xubuntu-trusty.jpg"),
	_T("linux_simple_background_xfce.jpg"),
	_T("collision-wallpaper.jpg"),
	_T("bridge_stourhead_wiltshire_united_kingdom.jpg"),
	_T("cosmic_peaks-wallpaper.jpg")
};

void InitSettingsList(HWND hWnd);
void InitPathsPicturesInList(HWND hWnd);
void ReadAndPrintRegeditKeyValue(HWND hWnd, HKEY hRoot, LPCTSTR lpValueName, LPCTSTR lpValue);
void ChangeRegeditKeyValue(HWND hwnd, HKEY hRoot, DWORD selectID, LPCTSTR lpValue);

void AddItem(HWND hWnd, PTSTR pstr, DWORD id);
DWORD GetSelectID(HWND hWnd);

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
	LoadString(hInstance, IDC_PAPROTSKILAB9, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAPROTSKILAB9));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAPROTSKILAB9));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PAPROTSKILAB9);
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
      CW_USEDEFAULT, CW_USEDEFAULT, 850, 395, NULL, NULL, hInstance, NULL);

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
		hWndList = CreateWindowEx(NULL, TEXT("LISTBOX"), NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | LBS_NOTIFY | LBS_STANDARD,
			10, 10, 400, 280, hWnd, (HMENU)IDC_LIST, hInst, NULL);

		hWndPicturesList = CreateWindowEx(NULL, TEXT("LISTBOX"), NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | LBS_NOTIFY | LBS_STANDARD,
			420, 90, 200, 200, hWnd, (HMENU)IDC_LIST_PICTURES, hInst, NULL);

		hWndScanner = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Scanner"), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			10, 295, 120, 30, hWnd, (HMENU)IDC_BUTTON_SCANNER, hInst, NULL);

		hWndEditKey = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
			420, 10, 200, 30, hWnd, (HMENU)IDC_EDIT_TEXT, hInst, NULL);

		hWndEditButton = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Edit"), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			420, 50, 120, 30, hWnd, (HMENU)IDC_BUTTON_EDIT, hInst, NULL);
		break;
	case WM_INITDIALOG:
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDC_BUTTON_SCANNER:
		{
			SendMessage(hWndList, LB_RESETCONTENT, NULL, NULL);
			InitSettingsList(hWndList);
			break;
		}
		case IDC_BUTTON_EDIT:
		{
			if (saveSelectID == -1) break;

			TCHAR buffer[1 << 8];
			GetWindowText(hWndEditKey, buffer, sizeof(buffer));

			auto root = (saveSelectID >= 8) ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;
			ChangeRegeditKeyValue(hWndEditKey, root, saveSelectID, buffer);
			break;
		}
		case IDC_LIST:
		{
			if (wmEvent == LBN_DBLCLK)
			{
				SendMessage(hWndPicturesList, LB_RESETCONTENT, NULL, NULL);

				auto selectID = GetSelectID(hWndList);
				auto root = (selectID >= 8) ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;

				if (selectID == 6)
				{
					InitPathsPicturesInList(hWndPicturesList);
				}

				ReadAndPrintRegeditKeyValue(hWndEditKey, root, regeditSettingsKeyPaths[selectID], regeditSettingsKeyValues[selectID]);
				saveSelectID = selectID;
			}
			break;
		}
		case IDC_LIST_PICTURES:
		{
			if (wmEvent == LBN_DBLCLK && saveSelectID == 6)
			{
				auto selectID = GetSelectID(hWndPicturesList);

				TCHAR buffer[1 << 8] = _T("");
				_tcscat_s(buffer, 50, directory);
				_tcscat_s(buffer, 100, pathPictures[selectID]);

				SendMessage(hWndEditKey, WM_SETTEXT, NULL, (LPARAM)buffer);
				ChangeRegeditKeyValue(hWndEditKey, HKEY_CURRENT_USER, saveSelectID, buffer);
				SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, SETWALLPAPER_DEFAULT, 0);
			}
			break;
		}
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

void InitSettingsList(HWND hWnd)
{
	for (int i = MAX_REGEDIT_PARAMS - 1, j = 0; i >= 0; --i, j++)
	{
		AddItem(hWnd, regeditSettingsNames[i], j);
	}
}

void InitPathsPicturesInList(HWND hWnd)
{
	for (int i = MAX_PICTURES - 1, j = 0; i >= 0; --i, j++)
	{
		AddItem(hWnd, pathPictures[i], i);
	}
}

void ReadAndPrintRegeditKeyValue(HWND hWnd, HKEY hRoot, LPCTSTR lpValueName, LPCTSTR lpValue)
{
	HKEY hKey;
	LONG lResult;

	lResult = RegOpenKeyEx(hRoot, lpValueName, NULL, KEY_READ, &hKey);
	if (lResult != ERROR_SUCCESS) return;

	DWORD type;
	DWORD cbData;

	lResult = RegQueryValueEx(hKey, lpValue, NULL, &type, NULL, &cbData);
	if (lResult != ERROR_SUCCESS) return;

	if (type == REG_SZ)
	{
		TCHAR keyValue[255];
		lResult = RegQueryValueEx(hKey, lpValue, NULL, NULL, (LPBYTE)&keyValue, &cbData);
		if (lResult != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			return;
		}

		SendMessage(hWnd, WM_SETTEXT, NULL, (LPARAM)keyValue);
	}
	else 
	{
		DWORD keyValue;
		lResult = RegQueryValueEx(hKey, lpValue, NULL, NULL, (LPBYTE)&keyValue, &cbData);
		if (lResult != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			return;
		}

		TCHAR buffer[10];
		_itot_s(keyValue, buffer, 10, 10);

		SendMessage(hWnd, WM_SETTEXT, NULL, (LPARAM)buffer);
	}

	RegCloseKey(hKey);
}

void ChangeRegeditKeyValue(HWND hwnd, HKEY hRoot, DWORD selectID, LPCTSTR lpValue)
{
	HKEY hKey;
	LONG lResult;

	lResult = RegOpenKeyEx(hRoot, regeditSettingsKeyPaths[selectID], NULL, KEY_ALL_ACCESS, &hKey);
	if (lResult != ERROR_SUCCESS) return;

	DWORD type;
	DWORD cbData;

	lResult = RegQueryValueEx(hKey, regeditSettingsKeyValues[selectID], NULL, &type, NULL, &cbData);
	if (lResult != ERROR_SUCCESS) return;

	if (type == REG_SZ)
	{
		lResult = RegSetValueEx(hKey, regeditSettingsKeyValues[selectID], NULL, type, (const BYTE*)lpValue, (_tcslen(lpValue) + 1) * sizeof(LPCTSTR));
	}
	else 
	{
		DWORD dwValue = wcstol(lpValue, NULL, 10);
		lResult = RegSetValueEx(hKey, regeditSettingsKeyValues[selectID], NULL, type, (const BYTE*)&dwValue, sizeof(dwValue));
	}

	RegCloseKey(hKey);
}

void AddItem(HWND hWnd, PTSTR pstr, DWORD id)
{
	if (pstr == NULL || pstr[0] == 0) return;

	int lbItem = SendMessage(hWnd, LB_INSERTSTRING, NULL, (LPARAM)pstr);
	SendMessage(hWnd, LB_SETITEMDATA, (WPARAM)lbItem, (LPARAM)id);
}

DWORD GetSelectID(HWND hWnd)
{
	return SendMessage(hWnd, LB_GETCURSEL, NULL, NULL);
}