#pragma once
#include <Windows.h>
#include <CommCtrl.h>
#include "Bass_Manager.h"
#include "CW_Header.h"
#include <string>
#include <IImgCtx.h>

#define TIMER_1 1

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

	
	void OnTimer(HWND hwnd, int timerID);
	char* OpenFile(HWND hwnd);
	void OnMusicLoad(HWND hwnd);
	void OnCreate(HWND hwnd);
	void OnPaint(HWND hwnd);
	char* TimeToString(long time);
	HWND WINAPI CreateTrackbar(HWND hwndDlg, UINT iMin, UINT iMax);
};

