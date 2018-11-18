#pragma once
#include <Windows.h>
#include "Draw_Manager.h"
#include "Bass_Manager.h"

class Window_Manager
{
public:
	Window_Manager();

	LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	Draw_Manager draw_manager;
	Bass_Manager bass_manager;

	char* OnOpenFile(HWND hwnd);
};

