#pragma once

#include <Windows.h>
#include "BitMapImage.h"

#define CW_WM_STATE_MINIMIZED 1
#define CW_WM_STATE_MAXIMIZED 2

#define CW_WM_TIMER_2 2

#define CW_WM_IMAGES_COUNT 2
#define CW_WM_CLOSE_BUTTON_PATH ""
#define CW_WM_MINIMIZE_BUTTON_PATH ""
#define CW_WM_MINIMIZED_IMAGE_PATH ""

#define CW_WM_CLOSE_BUTTON_ID 1
#define CW_WM_MINIMIZE_BUTTON_ID 2
#define CW_WM_MINIMIZED_IMAGE_ID 3

class Window_Menu
{
public:
	Window_Menu(int left, int top, int width, int height);

	void Draw(HDC hdc);
	BOOL MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	RECT GetMenuRect();

private:
	int left, top;
	int width, height;
	BOOL drawBk;
	RECT menuRect;
	int state;

	BitMapImage* buttons;
	BitMapImage* minimezedImage;
	BitMapImage* selectedImage;


	BOOL LButtonDown(HWND hwnd, LPARAM lParam);
	BOOL LButtonUp(HWND hwnd, LPARAM lParam);
	void OnMouseMove(HWND hwnd, LPARAM lParam);
	BOOL OnTimer(HWND hwnd, WPARAM timerID);
};

