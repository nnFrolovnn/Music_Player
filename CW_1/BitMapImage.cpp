#include "BitMapImage.h"
#include <math.h>

BitMapImage::BitMapImage()
{
	isTransparent = FALSE;
	x = 0;
	y = 0;
	hdcMem = 0;
	source = 0;
	hbitmap = 0;
	number = 0;

	isVisible = FALSE;
}

BitMapImage::BitMapImage(const char* path)
{
	width = 50;
	height = 50;
	x = 10;
	y = 10;
	hbitmap = NULL;
	hdcMem = NULL;
	isTransparent = FALSE;
	isVisible = TRUE;
	number = 0;

	LoadBitMapImage(path);
}

BitMapImage::BitMapImage(UINT numb, int nx, int ny, const char* path) :BitMapImage(path)
{
	x = nx;
	y = ny;
	number = numb;
}

void BitMapImage::Draw(HDC hdc)
{
	if (isVisible) 
	{
		if (!isTransparent)
		{
			MakeTransparent(hdc);
			isTransparent = TRUE;
		}

		BitBlt(hdc, x, y, width, height, hdcMem, 0, 0, SRCCOPY);
	}
}

void BitMapImage::Move(int addx, int addy)
{
	x += addx;
	y += addy;
}

void BitMapImage::SetVisible(BOOL visible)
{
	isVisible = visible;
}

void BitMapImage::SetTop(int ntop)
{
	y = ntop;
}

void BitMapImage::SetLeft(int nleft)
{
	x = nleft;
}

int BitMapImage::GetWidth()
{
	return width;
}

int BitMapImage::GetHeight()
{
	return height;
}

int BitMapImage::GetTop()
{
	return y;
}

int BitMapImage::GetLeft()
{
	return x;
}

BOOL BitMapImage::IsVisible()
{
	return isVisible;
}

UINT BitMapImage::GetNumber()
{
	return number;
}

void BitMapImage::MakeTransparent(HDC hdc)
{
	BITMAP     bm;
	COLORREF   cColor;
	HBITMAP    bmAndBack, bmAndObject, bmAndMem, bmSave;
	HDC        hdcBack, hdcObject, hdcTemp;
	POINT      ptSize;

	hdcTemp = source;

	GetObject(hbitmap, sizeof(BITMAP), (LPSTR)&bm);
	ptSize.x = bm.bmWidth;
	ptSize.y = bm.bmHeight;
	DPtoLP(hdcTemp, &ptSize, 1);

	hdcBack = CreateCompatibleDC(hdcTemp);
	hdcObject = CreateCompatibleDC(hdcTemp);
	hdcMem = CreateCompatibleDC(hdcTemp);

	bmAndBack = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	bmAndMem = CreateCompatibleBitmap(hdcTemp, ptSize.x, ptSize.y);
	bmSave = CreateCompatibleBitmap(hdcTemp, ptSize.x, ptSize.y);

	SelectObject(hdcBack, bmAndBack);
	SelectObject(hdcObject, bmAndObject);
	SelectObject(hdcMem, bmAndMem);

	SetMapMode(hdcTemp, GetMapMode(hdc));

	cColor = SetBkColor(hdcTemp, TRANSPARENT_COLOR);

	BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

	SetBkColor(hdcTemp, cColor);

	BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, NOTSRCCOPY);

	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdc, 0, 0, SRCCOPY);

	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);

	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack, 0, 0, SRCAND);

	// XOR
	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCPAINT);

	DeleteDC(source);
	DeleteDC(hdcBack);
	DeleteDC(hdcObject);
	DeleteDC(hdcTemp);
}

void BitMapImage::LoadBitMapImage(const char* path)
{
	BITMAP bm;
	hbitmap = (HBITMAP)LoadImageA(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	DWORD err = GetLastError();

	GetObjectA(hbitmap, sizeof(BITMAP), &bm);

	source = CreateCompatibleDC(NULL);
	SelectObject(source, hbitmap);

	width = bm.bmWidth;
	height = bm.bmHeight;
}
