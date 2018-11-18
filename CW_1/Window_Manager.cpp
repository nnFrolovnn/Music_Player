#include "Window_Manager.h"



Window_Manager::Window_Manager()
{
}


Window_Manager::~Window_Manager()
{
}

LRESULT Window_Manager::MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

char* Window_Manager::OnOpenFile(HWND hwnd)
{
	OPENFILENAME openFileDialog;
	char szDirect[260];
	char szFileName[260];
	BITMAP bm;

	memset(&openFileDialog, 0, sizeof(OPENFILENAME));
	openFileDialog.hwndOwner = hwnd;
	openFileDialog.Flags = OFN_EXPLORER;
	openFileDialog.hInstance = NULL;
	openFileDialog.lpstrFilter = "*.mp3";
	openFileDialog.lpstrCustomFilter = (LPSTR)"*.mp3";
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
	return openFileDialog.lpstrFile;
}
