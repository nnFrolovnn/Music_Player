#pragma once
#include <stdio.h>
#include "bass.h"
#pragma comment (lib, "bass.lib")

#define BM_BASS_INIT_ERROR 1001

class Bass_Manager
{
public:
	Bass_Manager();
	~Bass_Manager();

	int StreamCreate(char* filePath);
	void StreamPlay();
	void StreamStop();
	void StreamPause();
	int GetLastError();
	char* GetStreamInfo();
	double GetStreamTime();

private:
	HSTREAM stream;
};

