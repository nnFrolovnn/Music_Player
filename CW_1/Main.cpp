#include <Windows.h>
#include "CW_Header.h"
#include "Window_Manager.h"

Window_Manager window_manager = Window_Manager();

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return window_manager.MainWindowProc(hWnd, uMsg, wParam, lParam);
}

void RegWNDClassEX(HINSTANCE hInstance)
{
	HBRUSH brush = CreateSolidBrush(CW_BK_COLOR);
	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = MainWindowProc;
	wcex.hInstance = hInstance;
	wcex.hIcon = (HICON)LoadImage(hInstance, "resources//icons8_Music_Folder.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wcex.hCursor = LoadCursor(0, IDC_ARROW); //IDC_HAND
	wcex.hbrBackground = brush;
	wcex.lpszClassName = CW_WINDOW_CLASS_NAME;
	RegisterClassExA(&wcex);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	RegWNDClassEX(hInstance);

	/*WS_OVERLAPPEDWINDOW*/

	HWND hMainWindow = CreateWindowExA(0, CW_WINDOW_CLASS_NAME, CW_WINDOW_NAME, WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZE   , 0, 0,
		CW_WINDOW_WIDTH, CW_WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	ShowWindow(hMainWindow, SW_SHOWNORMAL);
	UpdateWindow(hMainWindow);

	window_manager.SetHinstance(hInstance);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}