#pragma once
#include <Windows.h>
//#include "CW_Header.h"

#define TRANSPARENT_COLOR 0x00FFFFFF

class BitMapImage
{
public:
	BitMapImage();
	BitMapImage(const char* path);
	BitMapImage(UINT numb, int nx, int ny, const char* path);

	void Draw(HDC hdc);
	void Move(int addx, int addy);

	void SetVisible(BOOL visible);
	void SetTop(int ntop);
	void SetLeft(int nleft);

	int GetWidth();
	int GetHeight();
	int GetTop();
	int GetLeft();
	BOOL IsVisible();
	UINT GetNumber();

private:
	HBITMAP hbitmap;
	HDC source;
	HDC hdcMem;
	UINT number;         //is used for identifying button
	int width;
	int height;
	int x, y;		     //coordinates of left top point of picture
	BOOL isTransparent;  //indicates has bitmap background or not
	BOOL isVisible;		 //indicates has we draw image or not

	void MakeTransparent(HDC hdc);
	void LoadBitMapImage(const char* path);
};

