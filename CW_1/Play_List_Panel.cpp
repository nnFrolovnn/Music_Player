#include "Play_List_Panel.h"

Play_List_Panel::Play_List_Panel(int left, int top, int width, int height, Bass_Manager * bass_man)
{
	this->left = left;
	this->top = top;
	this->width = width;
	this->height = height;
	bass_manager = bass_man;
}

void Play_List_Panel::Draw(HDC hdc)
{
}

BOOL Play_List_Panel::MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

BOOL Play_List_Panel::AddMusicFile(char * file)
{
	return 0;
}

BOOL Play_List_Panel::RemoveMusicFile(int number)
{
	return 0;
}
