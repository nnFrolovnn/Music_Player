#pragma once
#include <stdio.h>
#include "bass.h"
#pragma comment (lib, "bass.lib")

#define BM_BASS_INIT_ERROR 1001

struct musicFile {
	QWORD fileSize;
	char* filePath;
	char* name;
};

class Bass_Manager
{
public:
	Bass_Manager();
	~Bass_Manager();

	void StreamPlay();
	void StreamStop();
	void StreamPause();
	void StreamSetPosition(double percent);

	int GetLastError();
	char* GetStreamInfo();
	double GetStreamTime();


	BOOL AddFileNameToList(char* filePath);

	BOOL MusicHasPlayed();
	BOOL MusicCanPlay();
	BOOL MusicPlayingOrPaused();

private:
	HSTREAM stream;
	musicFile* playList;
	int musicFilesCount;
	BOOL isPlaying;
	BOOL isPause;
};

