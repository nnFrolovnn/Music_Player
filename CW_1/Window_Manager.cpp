#include "Window_Manager.h"

Window_Manager::Window_Manager()
{
	trackBar = NULL;
	trackTime = 0;
	currentTrackTime = 0;
	images = NULL;
	selectedImage = NULL;
}

LRESULT Window_Manager::MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		OnPaint(hWnd);
		break;
	case WM_TIMER:
		OnTimer(hWnd, wParam);
		break;
	case WM_DROPFILES:
		OnDropFiles(hWnd, wParam);
		bass_manager.StreamPlay();
		break;
	case WM_HSCROLL:
		//TODO: trackbar 
		break;
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
			SendMessage(trackBar, TBM_SETPOS, TRUE, (int)((currentTrackTime / trackTime)*100));
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
	trackTime = bass_manager.GetStreamTime();
	currentTrackTime = 0;

	bass_manager.StreamPlay();
	SetTimer(hwnd, TIMER_1, 1000, NULL);
}

void Window_Manager::OnCreate(HWND hwnd)
{
	DragAcceptFiles(hwnd, TRUE);

	trackBar = CreateTrackbar(hwnd, 0, 100);
	ShowWindow(trackBar, SW_SHOWNORMAL);
	
	images = new BitMapImage[IMAGES_COUNT];

	images[0] =  BitMapImage(5, CW_IMAGE_MENU_TOP, CW_IMAGE_PLAYLIST_PATH);
	images[1] =  BitMapImage(170, CW_IMAGE_MENU_TOP, CW_IMAGE_REWIND_PATH);
	images[2] =  BitMapImage(225, CW_IMAGE_MENU_TOP, CW_IMAGE_PLAY_PATH);
	images[3] =  BitMapImage(280, CW_IMAGE_MENU_TOP, CW_IMAGE_FORWARD_PATH);
	images[4] =  BitMapImage(335, CW_IMAGE_MENU_TOP, CW_IMAGE_STOP_PATH);
	images[5] =  BitMapImage(225, CW_IMAGE_MENU_TOP, CW_IMAGE_RESUME_PATH);

	
	images[5].SetVisible(FALSE);
}

void Window_Manager::OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);

	SendMessage(trackBar, WM_PAINT, 0, 0);
	for (int i = 0; i < IMAGES_COUNT; i++)
	{
		images[i].Draw(hdc);
	}


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
		
		if (left + 5 <= point.x && left + images[i].GetWidth() - 5 >= point.x)
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
	if (selectedImage != NULL)
	{
		selectedImage->Move(-ADD_ONCLICK_X, -ADD_ONCLICK_Y);
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

HWND WINAPI Window_Manager::CreateTrackbar(HWND hwndDlg, UINT iMin, UINT iMax)
{
	HWND hwndTrack = CreateWindowEx(
		0, TRACKBAR_CLASS, "Trackbar Control",
		WS_CHILD, CW_TRACKBAR_X, CW_TRACKBAR_Y,
		CW_TRACKBAR_WIDTH, CW_TRACKBAR_HEIGHT, hwndDlg, 0, hinstance, NULL);

	SendMessage(hwndTrack, TBM_SETRANGE,
		(WPARAM)TRUE,
		(LPARAM)MAKELONG(iMin, iMax));

	SendMessage(hwndTrack, TBM_SETPAGESIZE,
		0, (LPARAM)1);

	return hwndTrack;
}
