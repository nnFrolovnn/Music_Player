#include "Window_Manager.h"

Window_Manager::Window_Manager()
{
	trackTime = 0;
	currentTrackTime = 0;
	images = NULL;
	selectedButton = NULL;
	trackBar = NULL;
	volumeBar = NULL;
	bass_manager = NULL;
	window_menu = NULL;
	brush = CreateSolidBrush(CW_BK_COLOR);
}

LRESULT Window_Manager::MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL handled = FALSE;
	POINT point;

	//is message for trackbar
	if (trackBar != NULL)
	{
		handled = trackBar->MainWindowProc(uMsg, wParam, lParam);
	}
	//is message for volumeBar
	if (!handled && volumeBar != NULL)
	{
		handled = volumeBar->MainWindowProc(uMsg, wParam, lParam);
	}

	//is message for window_menu
	if (!handled && window_menu != NULL)
	{
		handled = window_menu->MainWindowProc(hWnd, uMsg, wParam, lParam);
	}

	if (!handled && playListPanel != NULL)
	{
		handled = playListPanel->MainWindowProc(hWnd, uMsg, wParam, lParam);
	}

	if (!handled)
	{
		switch (uMsg)
		{
		case WM_CREATE:
			//SetWindowLong(hWnd, GWL_EXSTYLE, 0x80000);
			//SetLayeredWindowAttributes(hWnd, 0, (255 * 70) / 100, LWA_ALPHA);
			OnCreate(hWnd);
			break;
		case WM_RBUTTONDOWN:
			OnMusicLoad(hWnd);
			break;
		case WM_LBUTTONDOWN:
			OnLButtonDown(hWnd, lParam);
			break;
		case WM_LBUTTONUP:
			OnLButtonUp(hWnd, lParam);
			break;
		case WM_DESTROY:
			KillTimer(hWnd, TIMER_1);
			PostQuitMessage(0);
			break;
		case WM_PAINT:
			OnPaint(hWnd, lParam);
			break;
		case WM_TIMER:
			OnTimer(hWnd, wParam);
			break;
		case WM_DROPFILES:
			OnDropFiles(hWnd, wParam);
			break;
		case WM_HSCROLL:
			OnScroll(hWnd, wParam, lParam);
			break;
		case WM_PLAYFILE:
			PlayFile(hWnd, wParam);
			break;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);

}

BOOL Window_Manager::SetHinstance(HINSTANCE hinst)
{
	if (hinst != NULL)
	{
		hinstance = hinst;
		return 1;
	}

	return 0;
}

void Window_Manager::OnTimer(HWND hwnd, int timerID)
{
	switch (timerID)
	{
	case TIMER_1:
		if (trackTime > currentTrackTime)
		{
			currentTrackTime++;
			SendMessage(trackBar->GetHWND(), TBM_SETPOS, TRUE, (int)((currentTrackTime / trackTime) * 100));
			InvalidateRect(hwnd, NULL, false);
			SendMessage(hwnd, WM_PAINT, 0, 0);
		}
		else
		{
			currentTrackTime = 0;
			bass_manager->StreamPlayNext();
			trackTime = bass_manager->GetStreamTime();
			SendMessage(trackBar->GetHWND(), TBM_SETPOS, TRUE, 0);
			playListPanel->SelectNext();

			InvalidateRect(hwnd, NULL, false);
			SendMessage(hwnd, WM_PAINT, 0, 0);
		}
		break;
	}
}

char* Window_Manager::GetFilePath(HWND hwnd)
{
	OPENFILENAME openFileDialog;
	char szDirect[MAX_PATH];
	char szFileName[MAX_PATH];

	memset(&openFileDialog, 0, sizeof(OPENFILENAME));
	openFileDialog.hwndOwner = hwnd;
	openFileDialog.Flags = OFN_EXPLORER;
	openFileDialog.hInstance = NULL;
	openFileDialog.lpstrFilter = NULL;
	openFileDialog.lpstrCustomFilter = NULL;
	openFileDialog.nFilterIndex = 1;
	openFileDialog.lpstrFile = szDirect;
	*(openFileDialog.lpstrFile) = 0;
	openFileDialog.lpstrInitialDir = NULL;
	openFileDialog.lStructSize = sizeof(openFileDialog);
	openFileDialog.nMaxFile = sizeof(szDirect);
	openFileDialog.lpstrFileTitle = szFileName;
	*(openFileDialog.lpstrFileTitle) = 0;
	openFileDialog.nMaxFileTitle = sizeof(szFileName);
	openFileDialog.Flags = OFN_EXPLORER;

	int load = GetOpenFileNameA(&openFileDialog);
	if (load == 0)
	{
		return NULL;
	}

	int l = strlen(openFileDialog.lpstrFile);
	char* n = new char[l];
	strcpy_s(n, sizeof(char)*l + 2, openFileDialog.lpstrFile);
	n[l + 1] = '\0';
	return n;
}

void Window_Manager::OnMusicLoad(HWND hwnd)
{
	char* file = GetFilePath(hwnd);
	bass_manager->AddFileNameToList(file);
}

void Window_Manager::OnCreate(HWND hwnd)
{
	DragAcceptFiles(hwnd, TRUE);

	RECT rect;
	rect.left = rect.right = rect.top = rect.bottom = 0;
	GetClientRect(hwnd, &rect);
	OffsetRect(&rect, -rect.left, -rect.top);

	bass_manager = new Bass_Manager(hwnd);
	trackBar = new TrackBar(hwnd, CW_TRACKBAR_X, CW_TRACKBAR_Y, rect.right, CW_TRACKBAR_MIN, CW_TRACKBAR_MAX, CW_TRACKBAR_IDENTIFIER);
	volumeBar = new TrackBar(hwnd, rect.right - 80, CW_IMAGE_MENU_TOP + 25, 50, CW_TRACKBAR_MIN, CW_TRACKBAR_MAX, CW_VOLUMEBAR_IDENTIFIER);

	window_menu = new Window_Menu(0, 0, rect.right, 40);

	playListPanel = new Play_List_Panel(0, 40, 170, CW_TRACKBAR_Y - 57, bass_manager);

	images = new BitMapImage[IMAGES_COUNT];
	images[0] = BitMapImage(CW_NUMBER_PLAY_LIST_BUTTON, 5, CW_IMAGE_MENU_TOP, CW_IMAGE_PLAYLIST_PATH);
	images[1] = BitMapImage(CW_NUMBER_REWIND_BUTTON, 170, CW_IMAGE_MENU_TOP, CW_IMAGE_REWIND_PATH);
	images[2] = BitMapImage(CW_NUMBER_PLAY_BUTTON, 225, CW_IMAGE_MENU_TOP, CW_IMAGE_PLAY_PATH);
	images[3] = BitMapImage(CW_NUMBER_FAST_F_BUTTON, 280, CW_IMAGE_MENU_TOP, CW_IMAGE_FORWARD_PATH);
	images[4] = BitMapImage(CW_NUMBER_STOP_BUTTON, 335, CW_IMAGE_MENU_TOP, CW_IMAGE_STOP_PATH);
	images[5] = BitMapImage(CW_NUMBER_PAUSE_BUTTON, 225, CW_IMAGE_MENU_TOP, CW_IMAGE_PAUSE_PATH);

	images[5].SetVisible(FALSE);
}

void Window_Manager::OnPaint(HWND hwnd, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	HDC tempDC = CreateCompatibleDC(hdc);
	// get window size
	RECT rect;
	rect.left = rect.right = rect.top = rect.bottom = 0;
	GetClientRect(hwnd, &rect);
	OffsetRect(&rect, -rect.left, -rect.top);

	// set region to draw
	HBITMAP tempBitMap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
	SelectObject(tempDC, tempBitMap);
	FillRect(tempDC, &rect, brush);

	int oldMode = SetBkMode(tempDC, TRANSPARENT);

	// draw buttons
	for (int i = IMAGES_COUNT - 1; i >= 0; i--)
	{
		images[i].Draw(tempDC);
	}

	// draw bars
	trackBar->Draw(tempDC);
	volumeBar->Draw(tempDC);

	// draw playlist
	playListPanel->Draw(tempDC);

	// draw menu
	window_menu->Draw(tempDC);

	// draw track time
	char * trackTimeString = TimeToString(trackTime);
	int lenTT = strlen(trackTimeString);
	RECT rcText;
	rcText.left = rcText.right = rcText.top = rcText.bottom = 0;
	DrawText(tempDC, trackTimeString, lenTT, &rcText, DT_CALCRECT);
	TextOut(tempDC, rect.right - rcText.right - 1, CW_TRACKBAR_Y - rcText.bottom - 3, trackTimeString, lenTT);

	char * currentTimeString = TimeToString(currentTrackTime);
	lenTT = strlen(currentTimeString);
	rcText.left = rcText.right = rcText.top = rcText.bottom = 0;
	DrawText(tempDC, trackTimeString, lenTT, &rcText, DT_CALCRECT);
	TextOut(tempDC, 1, CW_TRACKBAR_Y - rcText.bottom - 3, currentTimeString, strlen(currentTimeString));


	SetBkMode(tempDC, oldMode);

	// load drawn region
	BitBlt(hdc, 0, 0, rect.right, rect.bottom, tempDC, 0, 0, SRCCOPY);

	ValidateRect(hwnd, NULL);
	EndPaint(hwnd, &ps);

	// free resourses
	DeleteObject(tempBitMap);
	DeleteDC(tempDC);
	DeleteDC(hdc);
}

void Window_Manager::OnLButtonDown(HWND hwnd, LPARAM lParam)
{
	selectedButton = NULL;

	// coordinates
	POINT point;
	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);

	// check is button selected
	for (int i = 0; i < IMAGES_COUNT; i++)
	{
		int left = images[i].GetLeft();

		if (left + 5 <= point.x && left + images[i].GetWidth() - 5 >= point.x && images[i].IsVisible())
		{
			int top = images[i].GetTop();
			if (top + 5 <= point.y && top + images[i].GetHeight() - 5 >= point.y)
			{
				selectedButton = &(images[i]);
				break;
			}
		}
	}

	// if button selected
	if (selectedButton != NULL)
	{
		selectedButton->Move(ADD_ONCLICK_X, ADD_ONCLICK_Y);
		InvalidateRect(hwnd, NULL, false);
		SendMessage(hwnd, WM_PAINT, 0, 0);
	}
}

void Window_Manager::OnLButtonUp(HWND hwnd, LPARAM lParam)
{
	BOOL hasPlayed = TRUE;

	// if button was clicked
	if (selectedButton != NULL)
	{
		// return default state to button
		selectedButton->Move(-ADD_ONCLICK_X, -ADD_ONCLICK_Y);

		// define which button was selected
		switch (selectedButton->GetNumber())
		{
		case CW_NUMBER_PLAY_LIST_BUTTON:
			playListPanel->SetShownState();
			SendMessage(hwnd, WM_PAINT, 0, 0);
			break;
		case CW_NUMBER_REWIND_BUTTON:
			if (bass_manager->MusicIsPlayingOrIsPaused())
			{
				bass_manager->StreamPlayPrevios();
				playListPanel->SelectPrevios();
				currentTrackTime = 0;
				trackTime = bass_manager->GetStreamTime();
				SendMessage(trackBar->GetHWND(), TBM_SETPOS, TRUE, (int)((currentTrackTime / trackTime) * 100));
				SetTimer(hwnd, TIMER_1, 1000, NULL);
			}
			break;
		case CW_NUMBER_PLAY_BUTTON:
			selectedButton->SetVisible(FALSE);
			images[5].SetVisible(TRUE);

			if (bass_manager->MusicCanPlay())
			{
				hasPlayed = bass_manager->MusicHasPlayed();
				bass_manager->StreamPlay();

				if (!hasPlayed)
				{
					currentTrackTime = 0;
					trackTime = bass_manager->GetStreamTime();
				}

				SetTimer(hwnd, TIMER_1, 1000, NULL);
			}

			break;
		case CW_NUMBER_FAST_F_BUTTON:
			if (bass_manager->MusicIsPlayingOrIsPaused())
			{
				bass_manager->StreamPlayNext();
				playListPanel->SelectNext();
				currentTrackTime = 0;
				trackTime = bass_manager->GetStreamTime();
				SendMessage(trackBar->GetHWND(), TBM_SETPOS, TRUE, (int)((currentTrackTime / trackTime) * 100));
				SetTimer(hwnd, TIMER_1, 1000, NULL);
			}
			break;
		case CW_NUMBER_STOP_BUTTON:
			if (bass_manager->MusicHasPlayed())
			{
				KillTimer(hwnd, TIMER_1);
				SendMessage(trackBar->GetHWND(), TBM_SETPOS, TRUE, 0);
				currentTrackTime = 0;
				trackTime = 0;
			}
			bass_manager->StreamStop();
			images[2].SetVisible(TRUE);
			images[5].SetVisible(FALSE);
			break;
		case CW_NUMBER_PAUSE_BUTTON:
			if (bass_manager->MusicHasPlayed())
			{
				KillTimer(hwnd, TIMER_1);
				bass_manager->StreamPlay();
			}
			selectedButton->SetVisible(FALSE);
			images[2].SetVisible(TRUE);
			break;
		}

		selectedButton = NULL;
		InvalidateRect(hwnd, NULL, false);
		SendMessage(hwnd, WM_PAINT, 0, 0);
	}
}

void Window_Manager::OnDropFiles(HWND hwnd, WPARAM wParam)
{
	HDROP hDrop = (HDROP)wParam;

	TCHAR szFileName[MAX_PATH];
	DWORD dwCount = DragQueryFileA(hDrop, 0xFFFFFFFF, szFileName, MAX_PATH);
	for (int i = 0; i < dwCount; i++)
	{
		DragQueryFileA(hDrop, i, szFileName, MAX_PATH);
		bass_manager->AddFileNameToList(szFileName);
	}

	DragFinish(hDrop);
}

void Window_Manager::OnScroll(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	if (wParam == trackBar->GetIdentifier())
	{
		// find position in file to play
		double percent = ((double)lParam - CW_TRACKBAR_MIN) / ((double)CW_TRACKBAR_MAX - CW_TRACKBAR_MIN);
		currentTrackTime = percent * trackTime;

		if (bass_manager->MusicIsPlayingOrIsPaused())
		{
			bass_manager->StreamSetPosition(percent);
		}
	}
	else if (wParam == volumeBar->GetIdentifier())
	{
		double percent = ((double)lParam - CW_TRACKBAR_MIN) / ((double)CW_TRACKBAR_MAX - CW_TRACKBAR_MIN);

		bass_manager->StreamSetVolume(percent);
	}

}

char * Window_Manager::TimeToString(long time)
{
	long minutes = time / 60;
	long seconds = time % 60;

	std::string min = std::to_string(minutes);
	if (min.length() == 0)
	{
		min = std::string("0");
	}
	char* strTime = new char[min.length() + 4];
	int i;
	for (i = 0; i < min.length(); i++)
	{
		strTime[i] = min[i];
	}

	strTime[i] = ':';
	strTime[i + 1] = '0' + seconds / 10;
	strTime[i + 2] = '0' + seconds % 10;
	strTime[i + 3] = '\0';

	return strTime;
}

void Window_Manager::PlayFile(HWND hwnd, int number)
{
	if (bass_manager->MusicHasPlayed())
	{
		images[2].SetVisible(FALSE);
		images[5].SetVisible(TRUE);

		KillTimer(hwnd, TIMER_1);
		SendMessage(trackBar->GetHWND(), TBM_SETPOS, TRUE, 0);
		currentTrackTime = 0;
		trackTime = 0;

		bass_manager->StreamStop();
		bass_manager->SetCurrentFile(number);

		bass_manager->StreamPlay();
		currentTrackTime = 0;
		trackTime = bass_manager->GetStreamTime();
		SetTimer(hwnd, TIMER_1, 1000, NULL);
	}


}