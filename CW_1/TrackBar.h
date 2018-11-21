#pragma once

#include <Windows.h>
#include "CW_Header.h"

#define PENS_COUNT 6
#define PEN_WIDTH 1
#define PARAMETER_ERROR 2001
#define CW_TBM_SETPOS (WM_USER + 5)
#define PAINT_MESS_FROM_TRACKBAR 10

#define SLIDER_HEIGHT CW_TRACKBAR_HEIGHT
#define SLIDER_WIDTH SLIDER_HEIGHT

class TrackBar
{
public:
	TrackBar();
	TrackBar(HWND hwnd, int x, int y, int width);
	TrackBar(HWND hwnd, int x, int y, int width, int min, int max);

	BOOL MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void Draw(HDC hdc, LPARAM lParam);
private:
	int width, height;
	POINT leftCenterPoint;
	HPEN *pens;

	BOOL isPressed;

	HWND parentWindow;
	int max, min;
	int currentState;
	RECT slider;
	BOOL isSliderVisible;
	RECT barRect;

	BOOL OnButtonDown(HWND hWnd, LPARAM lParam);
	BOOL OnButtonUp(HWND hWnd, LPARAM lParam);

	void OnMouseMove(HWND hWnd, LPARAM lParam);
	
};
