#include "Window_Manager.h"


Window_Manager::Window_Manager()
{

}

LRESULT Window_Manager::MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int err = 0;
	char* file;
	switch (uMsg)
	{
	case WM_RBUTTONDOWN:
		file = OnOpenFile(hWnd);
		err = bass_manager.StreamCreate(file);
		bass_manager.StreamPlay();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		draw_manager.Draw(hWnd);
		break;
	default:
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



char* Window_Manager::OnOpenFile(HWND hwnd)
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
