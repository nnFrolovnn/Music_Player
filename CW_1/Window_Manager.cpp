#include "Window_Manager.h"


Window_Manager::Window_Manager()
{
	trackBar = NULL;
	trackTime = 0;
	currentTrackTime = 0;
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
	case WM_LBUTTONUP:
		bass_manager.StreamPlay();
		break;
	case WM_DESTROY:
		KillTimer(hWnd, TIMER_1);
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		//OnPaint(hWnd);
		break;
	case WM_TIMER:
		OnTimer(hWnd, wParam);
		break;
	case WM_HSCROLL:
		//TODO: trackbar 
		break;
	case WM_LBUTTONDOWN:

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
	char szDirect[260];
	char szFileName[260];

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
	bass_manager.StreamCreate(file);
	trackTime = bass_manager.GetStreamTime();
	currentTrackTime = 0;

	bass_manager.StreamPlay();
	SetTimer(hwnd, TIMER_1, 1000, NULL);
}

void Window_Manager::OnCreate(HWND hwnd)
{
	trackBar = CreateTrackbar(hwnd, 0, 100);
	ShowWindow(trackBar, SW_SHOWNORMAL);
	
	InvalidateRect(hwnd, NULL, true);
}

void Window_Manager::OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	
	EndPaint(hwnd, &ps);
	ValidateRect(hwnd, NULL);
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
