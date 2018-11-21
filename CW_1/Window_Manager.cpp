#include "Window_Manager.h"

Window_Manager::Window_Manager()
{
	trackBarHwnd = NULL;
	trackTime = 0;
	trackBar = NULL;
	currentTrackTime = 0;
	images = NULL;
	selectedImage = NULL;
}

LRESULT Window_Manager::MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL handled = FALSE;
	//is message for trackbar
	if (trackBar != NULL)
	{
		handled = trackBar->MainWindowProc(hWnd, uMsg, wParam, lParam);
	}

	if (!handled)
	{
		switch (uMsg)
		{
		case WM_CREATE:
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
			SendMessage(trackBarHwnd, TBM_SETPOS, TRUE, (int)((currentTrackTime / trackTime) * 100));
			InvalidateRect(hwnd, NULL, true);
		}
		else
		{
			KillTimer(hwnd, TIMER_1);
		}
		break;
	}
}

char* Window_Manager::OpenFile(HWND hwnd)
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
	char* file = OpenFile(hwnd);
	bass_manager.AddFileNameToList(file);
}

void Window_Manager::OnCreate(HWND hwnd)
{
	DragAcceptFiles(hwnd, TRUE);

	trackBar = new TrackBar(hwnd, CW_TRACKBAR_X, CW_TRACKBAR_Y, CW_TRACKBAR_WIDTH, CW_TRACKBAR_MIN, CW_TRACKBAR_MAX);
	trackBarHwnd = hwnd;

	ShowWindow(trackBarHwnd, SW_SHOWNORMAL);

	images = new BitMapImage[IMAGES_COUNT];

	images[0] = BitMapImage(CW_NUMBER_PLAY_LIST_BUTTON, 5, CW_IMAGE_MENU_TOP, CW_IMAGE_PLAYLIST_PATH);
	images[1] = BitMapImage(CW_NUMBER_REWIND_BUTTON, 170, CW_IMAGE_MENU_TOP, CW_IMAGE_REWIND_PATH);
	images[2] = BitMapImage(CW_NUMBER_PLAY_BUTTON, 225, CW_IMAGE_MENU_TOP, CW_IMAGE_PLAY_PATH);
	images[3] = BitMapImage(CW_NUMBER_FAST_F_BUTTON, 280, CW_IMAGE_MENU_TOP, CW_IMAGE_FORWARD_PATH);
	images[4] = BitMapImage(CW_NUMBER_STOP_BUTTON, 335, CW_IMAGE_MENU_TOP, CW_IMAGE_STOP_PATH);
	images[5] = BitMapImage(CW_NUMBER_RESUME_BUTTON, 225, CW_IMAGE_MENU_TOP, CW_IMAGE_RESUME_PATH);

	images[5].SetVisible(FALSE);
}

void Window_Manager::OnPaint(HWND hwnd, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);

	if (lParam != PAINT_MESS_FROM_TRACKBAR)
	{
		for (int i = 0; i < IMAGES_COUNT; i++)
		{
			images[i].Draw(hdc);
		}
	}
	trackBar->Draw(hdc, lParam);

	EndPaint(hwnd, &ps);
	ValidateRect(hwnd, NULL);

}

void Window_Manager::OnLButtonDown(HWND hwnd, LPARAM lParam)
{
	selectedImage = NULL;
	POINT point;
	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);

	for (int i = 0; i < IMAGES_COUNT; i++)
	{
		int left = images[i].GetLeft();

		if (left + 5 <= point.x && left + images[i].GetWidth() - 5 >= point.x && images[i].IsVisible())
		{
			int top = images[i].GetTop();
			if (top + 5 <= point.y && top + images[i].GetHeight() - 5 >= point.y)
			{
				selectedImage = &(images[i]);
				break;
			}
		}
	}

	if (selectedImage != NULL)
	{
		selectedImage->Move(ADD_ONCLICK_X, ADD_ONCLICK_Y);
		InvalidateRect(hwnd, NULL, true);
	}
}

void Window_Manager::OnLButtonUp(HWND hwnd, LPARAM lParam)
{
	BOOL hasPlayed = TRUE;

	if (selectedImage != NULL)
	{
		selectedImage->Move(-ADD_ONCLICK_X, -ADD_ONCLICK_Y);

		switch (selectedImage->GetNumber())
		{
		case CW_NUMBER_PLAY_LIST_BUTTON:

			break;
		case CW_NUMBER_REWIND_BUTTON:

			break;
		case CW_NUMBER_PLAY_BUTTON:		
			selectedImage->SetVisible(FALSE);
			images[5].SetVisible(TRUE);
		
			
			if (bass_manager.MusicCanPlay())
			{
				SetTimer(hwnd, TIMER_1, 1000, NULL);
				hasPlayed = bass_manager.MusicHasPlayed();
			}
			bass_manager.StreamPlay();

			if (!hasPlayed)
			{
				currentTrackTime = 0;
				trackTime = bass_manager.GetStreamTime();
			}
			break;
		case CW_NUMBER_FAST_F_BUTTON:

			break;
		case CW_NUMBER_STOP_BUTTON:
			if (bass_manager.MusicHasPlayed())
			{
				KillTimer(hwnd, TIMER_1);
				SendMessage(trackBarHwnd, TBM_SETPOS, TRUE, 0);
				currentTrackTime = 0;
				trackTime = 0;
			}
			bass_manager.StreamStop();
			break;
		case CW_NUMBER_RESUME_BUTTON:			
			if (bass_manager.MusicHasPlayed())
			{		
				KillTimer(hwnd, TIMER_1);
				bass_manager.StreamPlay();
			}
			selectedImage->SetVisible(FALSE);
			images[2].SetVisible(TRUE);
			break;
		}

		selectedImage = NULL;
		InvalidateRect(hwnd, NULL, true);
	}
}

void Window_Manager::OnDropFiles(HWND hwnd, WPARAM wParam)
{
	HDROP hDrop = (HDROP)wParam;

	TCHAR szFileName[MAX_PATH];
	DWORD dwCount = DragQueryFile(hDrop, 0xFFFFFFFF, szFileName, MAX_PATH);
	for (int i = 0; i < dwCount; i++)
	{
		DragQueryFileA(hDrop, i, szFileName, MAX_PATH);
		bass_manager.AddFileNameToList(szFileName);
	}

	DragFinish(hDrop);
}

void Window_Manager::OnScroll(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	//message from trackBar
	if (wParam == lParam)
	{
		double percent = ((double)lParam - CW_TRACKBAR_MIN) / ((double)CW_TRACKBAR_MAX - CW_TRACKBAR_MIN);
		currentTrackTime = percent*trackTime;

		if (bass_manager.MusicPlayingOrPaused())
		{
			bass_manager.StreamSetPosition(percent);
		}
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

	strTime[i + 1] = ':';
	strTime[i + 2] = '0' + seconds % 10;
	strTime[i + 3] = '0' + seconds / 10;
	strTime[i + 4] = '\0';

	return strTime;
}

/*HWND WINAPI Window_Manager::CreateTrackbar(HWND hwndDlg, UINT iMin, UINT iMax)
{
	HWND hwndTrack = CreateWindowEx(
		0, TRACKBAR_CLASS, "Trackbar Control",
		WS_CHILD, CW_TRACKBAR_X, CW_TRACKBAR_Y,
		CW_TRACKBAR_WIDTH, CW_TRACKBAR_HEIGHT, hwndDlg, (HMENU)1010, hinstance, NULL);

	SendMessage(hwndTrack, TBM_SETRANGE,
		(WPARAM)TRUE,
		(LPARAM)MAKELONG(iMin, iMax));

	SendMessage(hwndTrack, TBM_SETPAGESIZE,
		0, (LPARAM)1);

	return hwndTrack;
}*/
