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

	state = CW_WM_STATE_MINIMIZED;
	drawBk = FALSE;

	menuRect.top = top;
	menuRect.left = left;
	menuRect.bottom = top + height;
	menuRect.right = left + width;
}

void Window_Menu::Draw(HDC hdc)
{
	int old_bkMode = SetBkMode(hdc, TRANSPARENT);

	if (drawBk)
	{
		HBRUSH brush = CreateSolidBrush(0x50000000);
		RECT rc;
		rc.left = left;
		rc.top = top;
		rc.bottom = top + minimezedImage->GetHeight() + 1;
		rc.right = left + width;
		FillRect(hdc, &rc, brush);
		//HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

		//Rectangle(hdc, left, top, left + width, top + minimezedImage->GetHeight() + 1);
		//SelectObject(hdc, oldBrush);
		DeleteObject(brush);
	}

	for (int i = 0; i < CW_WM_IMAGES_COUNT; i++)
	{
		buttons[i].Draw(hdc);
	}

	minimezedImage->Draw(hdc);
	SetBkMode(hdc, old_bkMode);
}

BOOL Window_Menu::MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL result = FALSE;

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

	if (state == CW_WM_STATE_MINIMIZED && minimezedImage->ContainsPoint(point))
	{
		result = TRUE;
		selectedImage = minimezedImage;		
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
		if (selectedImage == NULL && point.y > top + height + 1)
		{
			state = CW_WM_STATE_MINIMIZED;
			top = 0;
			for (int i = 0; i < CW_WM_IMAGES_COUNT; i++)
			{
				buttons[i].SetTop(top);
				buttons[i].SetVisible(FALSE);
			}
			minimezedImage->SetVisible(TRUE);
			InvalidateRect(hwnd, NULL, true);
		}
	}
	return result;
}

BOOL Window_Menu::LButtonUp(HWND hwnd, LPARAM lParam)
{
	BOOL result = FALSE;
	if (selectedImage != NULL)
	{
		switch (selectedImage->GetNumber())
		{
		case CW_WM_CLOSE_BUTTON_ID:
			result = TRUE;
			SendMessage(hwnd, WM_DESTROY, 0, 0);
			break;
		case CW_WM_MINIMIZED_IMAGE_ID:
			result = TRUE;
			drawBk = FALSE;
			state = CW_WM_STATE_MAXIMIZED;

			for (int i = 0; i < CW_WM_IMAGES_COUNT; i++)
			{
				buttons[i].SetVisible(TRUE);
			}
			minimezedImage->SetVisible(FALSE);
			InvalidateRect(hwnd, NULL, true);
			break;
		case CW_WM_MINIMIZE_BUTTON_ID:
			result = TRUE;
			SendMessage(hwnd, WM_SIZE, SIZE_MINIMIZED, 0);
			break;
		}
	}
	return result;
}

void Window_Menu::OnMouseMove(HWND hwnd, LPARAM lParam)
{
	POINT point;
	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);

	if (state == CW_WM_STATE_MINIMIZED && left <= point.x && left + width >= point.x &&
		top <= point.y && top + height >= point.y)
	{
		if (!drawBk)
		{
			drawBk = TRUE;
			InvalidateRect(hwnd, NULL, true);
			//SendMessage(hwnd, WM_PAINT, 0, 0);
		}
	}
	else
	{
		if (drawBk)
		{
			drawBk = FALSE;
			InvalidateRect(hwnd, NULL, true);
			//SendMessage(hwnd, WM_PAINT, 0, 0);
		}
	}
}