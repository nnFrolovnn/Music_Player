#pragma once
#include <Windows.h>

#include "Bass_Manager.h"


#define TIMER_2 2

class Play_List_Panel
{
public:
	Play_List_Panel(int left, int top, int width, int height, Bass_Manager* bass_man);

	void Draw(HDC hdc);
	BOOL MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	BOOL AddMusicFile(char* file);
	BOOL RemoveMusicFile(int number);

private:
	Bass_Manager* bass_manager;
	int left, top, width, height;
};

