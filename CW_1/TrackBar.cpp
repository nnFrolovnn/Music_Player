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
	sliderImage = NULL;
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


		isSliderVisible = FALSE;
		isPressed = FALSE;

		parentHwnd = hwnd;

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

		sliderImage = new BitMapImage(0, x - 1, y - height / 2, TRACK_IMAGE_PATH);
	}
	else
	{
		throw PARAMETER_ERROR;
	}
}

TrackBar::TrackBar(HWND hwnd, int x, int y, int width, int min, int max, int identifier) :TrackBar(hwnd, x, y, width)
{
	if (min < max && min >= 0 && max >= 0)
	{
		this->max = max;
		this->min = min;
		this->identifier = identifier;
	}
	else
	{
		throw PARAMETER_ERROR;
	}
}

BOOL TrackBar::MainWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL result = FALSE;
	int left = leftCenterPoint.x;

	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		result = OnButtonDown(lParam);
		break;
	case WM_LBUTTONUP:
		result = OnButtonUp(lParam);
		break;
	case CW_TBM_SETPOS:
		if (wParam)
		{
			SendMessage(parentHwnd, WM_PAINT, 0, PAINT_MESS_FROM_TRACKBAR);
		}

		currentState = (min <= lParam && lParam <= max) ? lParam : currentState;
		left = leftCenterPoint.x + ((float)(currentState- min))/((float)max - min)*width - SLIDER_WIDTH / 2;
		if (left < leftCenterPoint.x)
		{
			left = leftCenterPoint.x;
		}
		if (left + sliderImage->GetWidth() > leftCenterPoint.x + width)
		{
			left = leftCenterPoint.x + width - sliderImage->GetWidth();
		}
		sliderImage->SetLeft(left - 1);

		result = TRUE;
		break;
	case WM_MOUSEMOVE:
		OnMouseMove(lParam);
		break;
	}

	return result;
}

void TrackBar::Draw(HDC hdc)
{
	if (pens != NULL && width > 0)
	{
		//int oldMode = SetBkMode(hdc, TRANSPARENT);

		SelectObject(hdc, pens[0]);
		MoveToEx(hdc, leftCenterPoint.x, leftCenterPoint.y - height / 2, NULL);
		LineTo(hdc, leftCenterPoint.x, leftCenterPoint.y + height / 2);
		MoveToEx(hdc, leftCenterPoint.x + width, leftCenterPoint.y - height / 2, NULL);
		LineTo(hdc, leftCenterPoint.x + width, leftCenterPoint.y + height / 2);

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
			sliderImage->Draw(hdc);
		}

		SelectObject(hdc, CreatePen(PS_SOLID, 1, CW_PEN_COLOR));
		//SetBkMode(hdc, oldMode);
	}
}

RECT TrackBar::GetTrackBarRect()
{
	return barRect;
}

HWND TrackBar::GetHWND()
{
	return parentHwnd;
}

int TrackBar::GetIdentifier()
{
	return identifier;
}

BOOL TrackBar::OnButtonDown(LPARAM lParam)
{
	POINT point;
	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);

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

			sliderImage->SetLeft(point.x - height / 2 - 1);

			InvalidateRect(parentHwnd, &barRect, false);
			SendMessage(parentHwnd, WM_PAINT, 0, PAINT_MESS_FROM_TRACKBAR);
			SendMessage(parentHwnd, WM_HSCROLL, identifier, currentState);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL TrackBar::OnButtonUp(LPARAM lParam)
{
	POINT point;
	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);

	if (isPressed)
	{
		OnMouseMove(lParam);
		isPressed = FALSE;
		return TRUE;
	}

	return FALSE;
}

void TrackBar::OnMouseMove(LPARAM lParam)
{
	POINT point;
	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);

	if (isPressed)
	{
		// set current state
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

		// set slider position
		temp = point.x - sliderImage->GetWidth() / 2 - 1;
		if (temp < leftCenterPoint.x - 1)
		{
			temp = leftCenterPoint.x - 1;
		}
		if (temp > leftCenterPoint.x + width - 1)
		{
			temp = leftCenterPoint.x + width - sliderImage->GetWidth() / 2 - 1;
		}
		sliderImage->SetLeft(temp);

		//notify parent window
		SendMessage(parentHwnd, WM_HSCROLL, identifier, currentState);
		InvalidateRect(parentHwnd, NULL, false);
		SendMessage(parentHwnd, WM_PAINT, identifier, PAINT_MESS_FROM_TRACKBAR);
	}
	else if (!isPressed && leftCenterPoint.y - height / 2 <= point.y && point.y <= leftCenterPoint.y + height / 2
			 && leftCenterPoint.x <= point.x && point.x <= leftCenterPoint.x + width)
	{
		//in trackBar
		if (!isSliderVisible)
		{
			isSliderVisible = TRUE;
			InvalidateRect(parentHwnd, &barRect, true);
			SendMessage(parentHwnd, WM_PAINT, identifier, PAINT_MESS_FROM_TRACKBAR);
		}
		isSliderVisible = TRUE;
	}
	else
	{
		//out of trackBar
		if (isSliderVisible)
		{
			isSliderVisible = FALSE;
			InvalidateRect(parentHwnd, &barRect, true);
			SendMessage(parentHwnd, WM_PAINT, identifier, PAINT_MESS_FROM_TRACKBAR);
		}
		isSliderVisible = FALSE;
	}
}
