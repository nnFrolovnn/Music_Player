#pragma once
#include <Windows.h>

class Window_Manager
{
public:
	Window_Manager();
	~Window_Manager();

	LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	char* OnOpenFile(HWND hwnd);
};

