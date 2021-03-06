#pragma once
#include <Windows.h>
#include <CommCtrl.h>
#include <string>
#include <IImgCtx.h>
#include <shellapi.h>

#include "Window_Menu.h"
#include "Bass_Manager.h"
#include "CW_Header.h"
#include "BitMapImage.h"
#include "TrackBar.h"
#include "Play_List_Panel.h"

#pragma comment (lib, "ComCtl32")

#define TIMER_1 1
#define TIMER_2 2
#define TIMER_1_TIME 1000
#define TIMER_2_TIME 20

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
	Bass_Manager* bass_manager;
	BitMapImage* images;	
	TrackBar* trackBar;
	TrackBar* volumeBar;
	Window_Menu* window_menu;
	Play_List_Panel* playListPanel;
	
	HBRUSH brush, secondBrush;
	HPEN pen;
	HINSTANCE hinstance;
	BitMapImage * selectedButton;
	double trackTime, currentTrackTime;
	float* fft;
	
	void OnTimer(HWND hwnd, int timerID);
	char* GetFilePath(HWND hwnd);
	void OnMusicLoad(HWND hwnd);
	void OnCreate(HWND hwnd);
	void OnPaint(HWND hwnd, LPARAM lParam);
	void OnLButtonDown(HWND hwnd, LPARAM lParam);
	void OnLButtonUp(HWND hwnd, LPARAM lParam);
	void OnDropFiles(HWND hwnd, WPARAM wParam);
	void OnScroll(HWND hwnd, WPARAM wParam, LPARAM lParam);
	char* TimeToString(long time);	
	void PlayFile(HWND hwnd, int number);
};

