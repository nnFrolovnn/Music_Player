#pragma once
#include <Windows.h>
#include <CommCtrl.h>
#include <string>
#include <IImgCtx.h>
#include <shellapi.h>
#include "Bass_Manager.h"
#include "CW_Header.h"
#include "BitMapImage.h"

#pragma comment (lib, "ComCtl32")

#define TIMER_1 1
#define IMAGES_COUNT 6
#define ADD_ONCLICK_X 2
#define ADD_ONCLICK_Y 3

class Window_Manager
{
public:
	Window_Manager();

	LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL SetHinstance(HINSTANCE hinst);

private:
	Bass_Manager bass_manager;
	HINSTANCE hinstance;
	HWND trackBar;
	double trackTime;
	double currentTrackTime;
	BitMapImage* images;
	BitMapImage * selectedImage;

	void OnTimer(HWND hwnd, int timerID);
	char* OpenFile(HWND hwnd);
	void OnMusicLoad(HWND hwnd);
	void OnCreate(HWND hwnd);
	void OnPaint(HWND hwnd);
	void OnLButtonDown(HWND hwnd, LPARAM lParam);
	void OnLButtonUp(HWND hwnd, LPARAM lParam);
	void OnDropFiles(HWND hwnd, WPARAM wParam);
	char* TimeToString(long time);
	HWND WINAPI CreateTrackbar(HWND hwndDlg, UINT iMin, UINT iMax);
};

