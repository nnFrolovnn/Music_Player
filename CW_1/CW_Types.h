#pragma once
enum STATE 
{
	Hidden, Shown, Minimized, Maximized, Move
};

struct Line
{
	int x1, x2, y1, y2, number;
	BOOL used;
	void MakeLine(int x1, int x2, int y1, int y2, int number)
	{
		this->x1 = x1;
		this->y1 = y1;
		this->x2 = x2;
		this->y2 = y2;
		this->number = number;
		this->used = TRUE;
	}
	void Draw(HDC hdc)
	{
		MoveToEx(hdc, x1, y1, NULL);
		LineTo(hdc, x2, y2);
	}
};

struct musicFile 
{
	unsigned long long fileSize;
	char* filePath;
	char* name;
};