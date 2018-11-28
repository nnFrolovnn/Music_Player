#pragma once
#define CW_WINDOW_CLASS_NAME "MainWindowClass"
#define CW_WINDOW_NAME "WinApp"

#define CW_WINDOW_WIDTH 500
#define CW_WINDOW_HEIGHT 300

#define CW_TRACKBAR_WIDTH CW_WINDOW_WIDTH
#define CW_TRACKBAR_HEIGHT 6
#define CW_TRACKBAR_X 0
#define CW_TRACKBAR_Y (int)(CW_WINDOW_HEIGHT/2.0)+60
#define CW_TRACKBAR_MIN 0
#define CW_TRACKBAR_MAX 100
#define CW_TRACKBAR_IDENTIFIER 1
#define CW_VOLUMEBAR_IDENTIFIER 2


#define CW_IMAGE_MENU_TOP CW_TRACKBAR_Y+CW_TRACKBAR_HEIGHT
#define CW_IMAGE_PLAY_PATH "resources//play.bmp"
#define CW_IMAGE_FORWARD_PATH "recources//ff.bmp"
#define CW_IMAGE_REWIND_PATH "recources//rewind.bmp"
#define CW_IMAGE_PAUSE_PATH "resources//pause.bmp"
#define CW_IMAGE_STOP_PATH "recources//stop.bmp"
#define CW_IMAGE_PLAYLIST_PATH "recources//playlist2.bmp"

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

/*void TrackBar::GradientRectangle(HDC hdc, LONG x1, LONG y1, LONG x2, LONG y2, COLORREF top, COLORREF bottom)
{
TRIVERTEX vertex[2];
vertex[0].x = x1;
vertex[0].y = y1;
vertex[0].Red = 0x0000;
vertex[0].Green = 0x8000;
vertex[0].Blue = 0x8000;
vertex[0].Alpha = 0x0000;

vertex[1].x = x2;
vertex[1].y = y2;
vertex[1].Red = 0x0000;
vertex[1].Green = 0xd000;
vertex[1].Blue = 0xd000;
vertex[1].Alpha = 0x0000;

// Create a GRADIENT_RECT structure that
// references the TRIVERTEX vertices.
GRADIENT_RECT gRect;
gRect.UpperLeft = 0;
gRect.LowerRight = 1;

// Draw a shaded rectangle.
GradientFill(hdc, vertex, 2, &gRect, 1, GRADIENT_FILL_RECT_V);
}*/
