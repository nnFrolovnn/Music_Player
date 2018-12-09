#include "Play_List_Panel.h"

Play_List_Panel::Play_List_Panel(int left, int top, int width, int height, Bass_Manager * bass_man)
{
	this->left = left;
	this->top = top;
	this->width = width;
	this->height = height;
	bass_manager = bass_man;
	state = Hidden;
	bkState = Hidden;
	wheeling = 0;
	selectedMusicLine.MakeLine(0, 0, 0, 0, 0);
	imageShow = new BitMapImage(1, left + 1, top + height / 2, CW_PLP_SHOW_IMAGE_PATH);
}

void Play_List_Panel::Draw(HDC hdc)
{
	if (bass_manager != NULL && state == Shown)
	{
		const int length = bass_manager->GetPlayListSize();
		playList = bass_manager->GetPlayList();
		if (length > 0 && playList != NULL)
		{
			int currentTop = top;
			BOOL wheelingDisabled = (height >= CalculateHeight(hdc)) ? TRUE : FALSE;
			for (int i = 0; i < length; i++)
			{
				if (wheeling <= i + 1 || wheelingDisabled)
				{
					RECT rect;
					rect.left = 0;
					rect.top = 0;
					DrawTextA(hdc, playList[i].name, strlen(playList[i].name), &rect, DT_CALCRECT);
					if (rect.bottom - rect.top + currentTop <= top + height)
					{
						rect.right = left + width;
						rect.left = left;
						rect.top = currentTop;
						rect.bottom += currentTop;
						DrawTextA(hdc, playList[i].name, strlen(playList[i].name), &rect, DT_LEFT);
						currentTop += rect.bottom - rect.top;
						if (i == selectedMusicLine.number && selectedMusicLine.used)
						{
							selectedMusicLine.MakeLine(left, left + (rect.right > width) ? width : rect.right,
								rect.bottom, rect.bottom, i);
							selectedMusicLine.Draw(hdc);
						}
					}
				}
			}
		}
	}
	else if (state == Hidden)
	{
		imageShow->Draw(hdc);
		if (bkState == Shown)
		{
			MoveToEx(hdc, left + imageShow->GetWidth(), top, NULL);
			LineTo(hdc, left + imageShow->GetWidth(), top + height);
		}
	}
}

BOOL Play_List_Panel::MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL result = FALSE;
	int wheelDelta;

	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		result = LButtonDown(hwnd, lParam);
		break;
	case WM_LBUTTONUP:
		result = LButtonUp(hwnd, lParam);
		break;
	case WM_MOUSEMOVE:
		OnMouseMove(hwnd, lParam);
		break;
	case WM_MOUSEWHEEL:
		wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (wheelDelta < 0)
			wheeling = (height < CalculateHeight(GetDC(hwnd), wheeling)) ? wheeling + 1 : wheeling;
		if (wheelDelta > 0)
			wheeling = (wheeling >= 1)? wheeling - 1: 0;
		InvalidateRect(hwnd, NULL, false);
		break;
	}

	return result;
}

void Play_List_Panel::SetShownState()
{
	state = Shown;
}

void Play_List_Panel::SelectNext()
{
	selectedMusicLine.number = ((selectedMusicLine.number + 1) % bass_manager->GetPlayListSize());
}

void Play_List_Panel::SelectPrevios()
{
	selectedMusicLine.number = ((selectedMusicLine.number - 1) % bass_manager->GetPlayListSize());
}

int Play_List_Panel::CalculateHeight(HDC hdc, int fromFile)
{
	const int length = bass_manager->GetPlayListSize();
	playList = bass_manager->GetPlayList();
	int currHeight = 0;
	if (length > 0 && playList != NULL)
	{		
		for (int i = 0; i < length; i++)
		{
			if (fromFile <= i + 1)
			{
				RECT r;
				r.left = r.top = 0;
				DrawTextA(hdc, playList[i].name, strlen(playList[i].name), &r, DT_CALCRECT);
				currHeight += r.bottom;
			}
		}
	}
	return currHeight;
}

BOOL Play_List_Panel::LButtonDown(HWND hwnd, LPARAM lParam)
{
	BOOL result = FALSE;
	POINT point;
	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);

	if (state == Shown && (!(left + width >= point.x && point.x > left) && (top < point.y && point.y < top + height)))
	{
		state = Hidden;
		InvalidateRect(hwnd, NULL, false);
		result = TRUE;
		SendMessage(hwnd, WM_PAINT, 0, 0);
	}
	else if (state == Shown && (left + width >= point.x && point.x > left) && (top < point.y && point.y < top + height))
	{
		const int length = bass_manager->GetPlayListSize();
		musicFile* list = bass_manager->GetPlayList();
		HDC hdc = GetDC(hwnd);
		int currTop = top;
		int pressedFile = -1;
		for (int i = 0; i < length; i++)
		{
			if (wheeling <= i + 1)
			{
				RECT r;
				r.left = r.top = 0;
				DrawTextA(hdc, playList[i].name, strlen(playList[i].name), &r, DT_CALCRECT);
				if (currTop <= point.y && point.y <= currTop + r.bottom)
				{
					pressedFile = i;
					if (bass_manager->MusicIsPlayingOrIsPaused())
					{
						selectedMusicLine.number = i;
					}
					break;
				}
				currTop += r.bottom;
			}
		}
		if (pressedFile > -1)
		{
			result = TRUE;
			InvalidateRect(hwnd, NULL, false);
			SendMessage(hwnd, WM_PAINT, 0, 0);
			SendMessage(hwnd, WM_PLAYFILE, pressedFile, lParam);
		}
	}

	return result;
}

BOOL Play_List_Panel::LButtonUp(HWND hwnd, LPARAM lParam)
{
	return 0;
}

void Play_List_Panel::OnMouseMove(HWND hwnd, LPARAM lParam)
{
}
