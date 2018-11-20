#pragma once
#define CW_WINDOW_CLASS_NAME "MainWindowClass"
#define CW_WINDOW_NAME "WinApp"

#define CW_WINDOW_WIDTH 500
#define CW_WINDOW_HEIGHT 300

#define CW_TRACKBAR_WIDTH CW_WINDOW_WIDTH-5
#define CW_TRACKBAR_HEIGHT (int)(CW_WINDOW_HEIGHT/10.0)
#define CW_TRACKBAR_X -5
#define CW_TRACKBAR_Y (int)(CW_WINDOW_HEIGHT/2.0)+20

#define CW_IMAGE_MENU_TOP CW_TRACKBAR_Y+CW_TRACKBAR_HEIGHT
#define TRANSPARENT_COLOR 0x00FFFFFF
#define CW_IMAGE_PLAY_PATH "resources//play.bmp"
#define CW_IMAGE_FORWARD_PATH "recources//ff.bmp"
#define CW_IMAGE_REWIND_PATH "recources//rewind.bmp"
#define CW_IMAGE_RESUME_PATH "resources//resume.bmp"
#define CW_IMAGE_STOP_PATH "recources//stop.bmp"
#define CW_IMAGE_PLAYLIST_PATH "recources//playlist.bmp"

#define CW_NUMBER_PLAY_LIST_BUTTON 1
#define CW_NUMBER_REWIND_BUTTON 2
#define CW_NUMBER_PLAY_BUTTON 3
#define CW_NUMBER_FAST_F_BUTTON 4
#define CW_NUMBER_STOP_BUTTON 5
#define CW_NUMBER_RESUME_BUTTON 6

//second way to do
/*HWND button = CreateWindowExA(NULL, "button", "Пуск", WS_VISIBLE | WS_CHILD | BS_BITMAP,
50, 50, 50, 50, hwnd, NULL, NULL, NULL);

HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, CW_IMAGE_PLAY_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

SendMessage(button, BM_SETIMAGE, IMAGE_BITMAP, LPARAM(hBitmap));
SendMessage(button, WM_PAINT, 0, 0);*/