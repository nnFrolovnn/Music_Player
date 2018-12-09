#pragma once
#include <Windows.h>
#include "BitMapImage.h"
#include "Bass_Manager.h"
#include "CW_Types.h"

#define CW_PLP_SHOW_IMAGE_PATH "resources//Expand Arrow_left_20px.bmp"
#define WM_PLAYFILE (WM_USER + 100)
class Play_List_Panel
{
public:
	Play_List_Panel(int left, int top, int width, int height, Bass_Manager* bass_man);
	void Draw(HDC hdc);
	BOOL MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void SetShownState();
	void SelectNext();
	void SelectPrevios();

private:
	Bass_Manager* bass_manager;
	int left, top, width, height, wheeling;
	STATE state, bkState;
	Line selectedMusicLine;

	int* topCoordinates;
	musicFile* playList;
	BitMapImage* imageShow;

	int CalculateHeight(HDC hdc, int fromFile = 0);
	BOOL LButtonDown(HWND hwnd, LPARAM lParam);
	BOOL LButtonUp(HWND hwnd, LPARAM lParam);
	void OnMouseMove(HWND hwnd, LPARAM lParam);
};