#pragma once
#define CW_WINDOW_CLASS_NAME "MainWindowClass"
#define CW_WINDOW_NAME "WinApp"

#define CW_WINDOW_WIDTH 500
#define CW_WINDOW_HEIGHT 300
#define CW_BK_COLOR RGB(30, 30, 30)
#define CW_BK_COLOR_2 RGB(51, 51, 51)
#define CW_PEN_COLOR RGB(43, 145, 175)
#define CW_PEN_COLOR_2 RGB(30, 62, 123)
#define CW_TEXT_COLOR CW_PEN_COLOR

#define CW_TRACKBAR_WIDTH CW_WINDOW_WIDTH
#define CW_TRACKBAR_HEIGHT 6
#define CW_TRACKBAR_X 0
#define CW_TRACKBAR_Y (int)(CW_WINDOW_HEIGHT/2.0)+100
#define CW_TRACKBAR_MIN 0
#define CW_TRACKBAR_MAX 100
#define CW_TRACKBAR_IDENTIFIER 1
#define CW_VOLUMEBAR_IDENTIFIER 2


#define CW_IMAGE_MENU_TOP CW_TRACKBAR_Y+CW_TRACKBAR_HEIGHT
#define CW_IMAGE_PLAY_PATH "resources//play.bmp"
#define CW_IMAGE_FORWARD_PATH "resources//ff.bmp"
#define CW_IMAGE_REWIND_PATH "resources//rewind.bmp"
#define CW_IMAGE_PAUSE_PATH "resources//pause.bmp"
#define CW_IMAGE_STOP_PATH "resources//stop.bmp"
#define CW_IMAGE_PLAYLIST_PATH "resources//playlist2.bmp"

#define CW_NUMBER_PLAY_LIST_BUTTON 1
#define CW_NUMBER_REWIND_BUTTON 2
#define CW_NUMBER_PLAY_BUTTON 3
#define CW_NUMBER_FAST_F_BUTTON 4
#define CW_NUMBER_STOP_BUTTON 5
#define CW_NUMBER_PAUSE_BUTTON 6