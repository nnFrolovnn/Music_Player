#include "Window_Menu.h"



Window_Menu::Window_Menu(int left, int top, int width, int height)
{
	this->left = left;
	this->top = top;
	this->height = height;
	this->width = width;

	buttons = new BitMapImage[CW_WM_IMAGES_COUNT];
	buttons[0] = BitMapImage(CW_WM_CLOSE_BUTTON_ID, width - 50, 0, CW_WM_CLOSE_BUTTON_PATH);
	buttons[1] = BitMapImage(CW_WM_MINIMIZE_BUTTON_ID, width - 50 - buttons[0].GetWidth(), 0, CW_WM_MINIMIZE_BUTTON_PATH);
	minimezedImage = new BitMapImage(CW_WM_MINIMIZED_IMAGE_ID, width / 2 - 10, 0, CW_WM_MINIMIZED_IMAGE_PATH);

	minimezedImage->SetVisible(TRUE);
	buttons[0].SetVisible(FALSE);
	buttons[1].SetVisible(FALSE);

	state = 0;
	drawBk = FALSE;

	menuRect.top = top;
	menuRect.left = left;
	menuRect.bottom = top + height;
	menuRect.right = left + width;
}

void Window_Menu::Draw(HDC hdc)
{
	if (drawBk)
	{
		HBRUSH brush = CreateSolidBrush(0x70FFFFFF);
		RECT rc;
		rc.left = left;
		rc.top = top;
		rc.bottom = top + height;
		rc.right = left + width;
		FillRect(hdc, &rc, brush);
	}

	for (int i = 0; i < CW_WM_IMAGES_COUNT; i++)
	{
		buttons[i].Draw(hdc);
	}

	minimezedImage->Draw(hdc);
}

BOOL Window_Menu::MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL result = FALSE;

	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		result = LButtonDown(hwnd, lParam);
		break;
	case WM_TIMER:

		break;
	case WM_MOUSEMOVE:
		OnMouseMove(hwnd, lParam);
		break;
	}

	return result;
}

RECT Window_Menu::GetMenuRect()
{
	return menuRect;
}

BOOL Window_Menu::LButtonDown(HWND hwnd, LPARAM lParam)
{
	BOOL result = FALSE;
	POINT point;
	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);

	if (state == CW_WM_STATE_MINIMIZED && left <= point.x && left + minimezedImage->GetWidth() >= point.x &&
		top <= point.y && top + minimezedImage->GetHeight() >= point.y)
	{
		state = CW_WM_STATE_MAXIMIZED;		
		for (int i = 0; i < CW_WM_IMAGES_COUNT; i++)
		{
			buttons[i].SetTop(buttons[i].GetTop() - height);
			buttons[i].SetVisible(TRUE);
		}
		minimezedImage->SetVisible(FALSE);
		result = TRUE;
	}
	else if (state == CW_WM_STATE_MAXIMIZED)
	{
		selectedImage = NULL;
		for (int i = 0; i < CW_WM_IMAGES_COUNT; i++)
		{
			if (buttons[i].IsVisible() && buttons[i].ContainsPoint(point))
			{
				selectedImage = &(buttons[i]);
				buttons[i].Move(2, 3);
				break;
			}
		}
		if (selectedImage != NULL)
		{
			top = -height;
			SetTimer(hwnd, CW_WM_TIMER_2, 100, NULL);
			result = TRUE;
		}
	}
	return result;
}

BOOL Window_Menu::LButtonUp(HWND hwnd, LPARAM lParam)
{
	BOOL result = FALSE;

	switch (selectedImage->GetNumber())
	{
	case CW_WM_CLOSE_BUTTON_ID:
		result = TRUE;
		SendMessage(hwnd, WM_DESTROY, 0, 0);
		break;
	case CW_WM_MINIMIZED_IMAGE_ID:
		result = TRUE;
		SendMessage(hwnd, WM_SIZE, SIZE_MINIMIZED, 0);
		break;
	}

	return result;
}

void Window_Menu::OnMouseMove(HWND hwnd, LPARAM lParam)
{
	POINT point;
	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);

	if (state == CW_WM_STATE_MINIMIZED && left <= point.x && left + minimezedImage->GetWidth() >= point.x &&
		top <= point.y && top + minimezedImage->GetHeight() >= point.y)
	{
		if (!drawBk)
		{
			drawBk = TRUE;
			SendMessage(hwnd, WM_PAINT, 0, 0);
		}
	}
	else
	{
		if (drawBk)
		{
			drawBk = FALSE;
			SendMessage(hwnd, WM_PAINT, 0, 0);
		}
	}
}

BOOL Window_Menu::OnTimer(HWND hwnd, WPARAM timerID)
{
	BOOL result = FALSE;

	if (timerID == CW_WM_TIMER_2)
	{
		if (top < 0)
		{
			top++;
			result = TRUE;
			SendMessage(hwnd, WM_PAINT, 0, 0);
		}
		else
		{
			KillTimer(hwnd, timerID);
		}
	}

	return result;
}
