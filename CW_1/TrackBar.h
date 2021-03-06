#pragma once

#include <Windows.h>
#include "CW_Header.h"
#include "BitMapImage.h"

#define PENS_COUNT 6
#define PEN_WIDTH 1
#define PARAMETER_ERROR 2001
#define CW_TBM_SETPOS (WM_USER + 5)
#define PAINT_MESS_FROM_TRACKBAR 10

#define SLIDER_HEIGHT CW_TRACKBAR_HEIGHT
#define SLIDER_WIDTH SLIDER_HEIGHT

#define TRACK_IMAGE_PATH "resources//rect6.bmp"
class TrackBar
{
public:
	TrackBar();
	TrackBar(HWND hwnd, int x, int y, int width);
	TrackBar(HWND hwnd, int x, int y, int width, int min, int max, int identifier);

	BOOL MainWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void Draw(HDC hdc);

	RECT GetTrackBarRect();
	HWND GetHWND();
	int  GetIdentifier();
private:
	int width, height;
	POINT leftCenterPoint;
	HPEN *pens;

	BOOL isPressed;

	HWND parentHwnd;
	int max, min;
	int identifier;
	int currentState;
	BOOL isSliderVisible;
	RECT barRect;
	BitMapImage * sliderImage;

	BOOL OnButtonDown(LPARAM lParam);
	BOOL OnButtonUp(LPARAM lParam);

	void OnMouseMove(LPARAM lParam);
	
};

