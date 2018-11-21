#include "TrackBar.h"

TrackBar::TrackBar()
{
	pens = NULL;
	width = 0;
	height = CW_TRACKBAR_HEIGHT;
	isPressed = FALSE;
	isSliderVisible = FALSE;
	barRect.left = 0;
	barRect.right = 0;
	barRect.top = 0;
	barRect.bottom = 0;
}

TrackBar::TrackBar(HWND hwnd, int x, int y, int width)
{
	if (hwnd != NULL && x >= 0 && y >= 0 && width > 0)
	{
		this->width = width;
		leftCenterPoint.x = x;
		leftCenterPoint.y = y;

		height = CW_TRACKBAR_HEIGHT;
		currentState = 0;
		min = 0;
		max = 100;

		slider.left = x;
		slider.right = slider.left + SLIDER_WIDTH;
		slider.top = y - SLIDER_HEIGHT / 2;
		slider.bottom = slider.top + SLIDER_WIDTH;
		isSliderVisible = FALSE;
		isPressed = FALSE;

		parentWindow = hwnd;

		pens = new HPEN[PENS_COUNT];
		pens[0] = CreatePen(PS_SOLID, PEN_WIDTH, 0x00423E3F);
		pens[1] = CreatePen(PS_SOLID, PEN_WIDTH, 0x00191818);
		pens[2] = CreatePen(PS_SOLID, PEN_WIDTH, 0x00FAC97F);
		pens[3] = CreatePen(PS_SOLID, PEN_WIDTH, 0x00B64C01);
		pens[4] = CreatePen(PS_SOLID, PEN_WIDTH, 0x00F29F61);
		pens[5] = CreatePen(PS_SOLID, PEN_WIDTH, 0x004D4B4C);

		barRect.left = x;
		barRect.right = x + width;
		barRect.top = y - height / 2;
		barRect.bottom = barRect.top + height;
	}
	else
	{
		throw PARAMETER_ERROR;
	}
}

TrackBar::TrackBar(HWND hwnd, int x, int y, int width, int min, int max) :TrackBar(hwnd, x, y, width)
{
	if (min < max && min >= 0 && max >= 0)
	{
		this->max = max;
		this->min = min;
	}
	else
	{
		throw PARAMETER_ERROR;
	}
}

BOOL TrackBar::MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL result = FALSE;

	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		result = OnButtonDown(hWnd, lParam);
		break;
	case WM_LBUTTONUP:
		result = OnButtonUp(hWnd, lParam);
		break;
	case CW_TBM_SETPOS:
		if (wParam)
		{
			SendMessage(hWnd, WM_PAINT, 0, PAINT_MESS_FROM_TRACKBAR);
		}

		currentState = (min <= lParam && lParam <= max) ? lParam : currentState;
		slider.left = leftCenterPoint.x + ((float)(currentState- min))/((float)max - min)*width - SLIDER_WIDTH / 2;
		if (slider.left < leftCenterPoint.x)
		{
			slider.left = leftCenterPoint.x;
		}
		slider.right = slider.left + SLIDER_WIDTH;
		if (slider.right > leftCenterPoint.x + width)
		{
			slider.right = width;
			slider.left = slider.right - SLIDER_WIDTH;
		}
		result = TRUE;
		break;
	case WM_MOUSEMOVE:
		OnMouseMove(hWnd, lParam);
		break;
	}

	return result;
}

void TrackBar::Draw(HDC hdc, LPARAM lParam)
{
	if (pens != NULL && width > 0)
	{
		int oldMode = SetBkMode(hdc, TRANSPARENT);

		for (int i = 0; i < PENS_COUNT; i++)
		{
			int x = leftCenterPoint.x + width;
			int y = leftCenterPoint.y - PENS_COUNT / 2 + i;

			SelectObject(hdc, pens[i]);
			MoveToEx(hdc, leftCenterPoint.x, y, NULL);

			if (i != 0 && i != PENS_COUNT - 1)
			{
				x = leftCenterPoint.x + ((float)(currentState - min)) / ((float)max - min) * (float)(width);
			}

			LineTo(hdc, x, y);
		}

		if (isSliderVisible)
		{
			Rectangle(hdc, slider.left, slider.top, slider.right, slider.bottom);
		}
		SetBkMode(hdc, oldMode);
	}
}

BOOL TrackBar::OnButtonDown(HWND hWnd, LPARAM lParam)
{
	POINT point;
	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);

	//somewhere on trackbar 
	if (leftCenterPoint.y - height / 2 <= point.y && point.y <= leftCenterPoint.y + height / 2)
	{
		if (leftCenterPoint.x <= point.x && point.x <= leftCenterPoint.x + width)
		{
			int temp = min + ((float)(point.x - leftCenterPoint.x) / (float)width)*(max - min);
			if (temp < min)
			{
				temp = min;
			}
			if (temp > max)
			{
				temp = max;
			}
			currentState = temp;

			isSliderVisible = TRUE;
			isPressed = TRUE;
			slider.left = point.x - SLIDER_WIDTH / 2;
			slider.right = slider.left + SLIDER_WIDTH;

			InvalidateRect(parentWindow, &barRect, true);
			SendMessage(hWnd, WM_PAINT, 0, PAINT_MESS_FROM_TRACKBAR);
			SendMessage(hWnd, WM_HSCROLL, 0, currentState);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL TrackBar::OnButtonUp(HWND hWnd, LPARAM lParam)
{
	POINT point;
	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);

	if (isPressed)
	{
		OnMouseMove(hWnd, lParam);
		isPressed = FALSE;
		return TRUE;
	}

	return FALSE;
}

void TrackBar::OnMouseMove(HWND hWnd, LPARAM lParam)
{
	POINT point;
	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);

	if (isPressed)
	{
		int temp = min + ((float)(point.x - leftCenterPoint.x) / (float)width)*(max - min);
		if (temp < min)
		{
			temp = min;
		}
		if (temp > max)
		{
			temp = max;
		}
		currentState = temp;
		slider.left = point.x - SLIDER_WIDTH / 2;
		slider.right = slider.left + SLIDER_WIDTH;

		SendMessage(hWnd, WM_HSCROLL, currentState, currentState);
		InvalidateRect(parentWindow, &barRect, true);
		SendMessage(hWnd, WM_PAINT, 0, PAINT_MESS_FROM_TRACKBAR);
	}

	if (!isPressed && leftCenterPoint.y - height / 2 <= point.y && point.y <= leftCenterPoint.y + height / 2
		&& leftCenterPoint.x <= point.x && point.x <= leftCenterPoint.x + width)
	{
		if (!isSliderVisible)
		{
			isSliderVisible = TRUE;
			InvalidateRect(parentWindow, &barRect, true);
			SendMessage(hWnd, WM_PAINT, 0, PAINT_MESS_FROM_TRACKBAR);
		}
		isSliderVisible = TRUE;
	}
	else
	{

		if (isSliderVisible)
		{
			isSliderVisible = FALSE;
			InvalidateRect(parentWindow, &barRect, true);
			SendMessage(hWnd, WM_PAINT, 0, PAINT_MESS_FROM_TRACKBAR);
		}
		isSliderVisible = FALSE;
	}
}
