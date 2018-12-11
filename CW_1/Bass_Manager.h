#pragma once
#include <stdio.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>

#include "bass.h"
#include "bass_cmponents\bassmod.h"
#include "bass_cmponents\unmo3.h"
#include "CW_Types.h"

#pragma comment (lib, "bass.lib")
#pragma comment (lib, "BASSMOD.lib")
#pragma comment (lib, "unmo3.lib")

class Bass_Manager
{
public:
	Bass_Manager();
	Bass_Manager(HWND hwnd);
	~Bass_Manager();

	void StreamPlay();
	void StreamStop();
	void StreamPause();
	void StreamPlayNext();
	void StreamPlayPrevios();
	void SetCurrentFile(int fileNumber);

	//percent 0..1
	void StreamSetPosition(double percent);
	//volume  0..1
	void StreamSetVolume(double volume);

	void RemoveFile(musicFile filetoRemove);
	void RemoveFile(int number);

	int GetLastError();
	char* GetStreamInfo();
	double GetStreamTime();
	double GetVolume();
	DWORD GetFlags();
	musicFile* GetPlayList();
	int GetPlayListSize();

	BOOL AddFileNameToList(char* filePath);

	BOOL MusicHasPlayed();
	BOOL MusicCanPlay();
	BOOL MusicIsPlayingOrIsPaused();

	// return 512 elements
	float* GetFFT();

private:
	HSTREAM stream;
	musicFile* playList;
	int musicFilesCount;
	int currentMusicFile;
	BOOL isPlaying;
	BOOL isPause;

	//
	//loads data from file to memory
	//returns pointer to the first byte
	//else returns nullptr
	void * LoadMusicFileToMemory(musicFile* mFile);

	//
	//returns handle for manipulating stream when successed
	//else returns 0
	HSTREAM LoadMusicForPlaying(musicFile* fileToPlay, double percent, BOOL sameFile = FALSE);
		  
	void StreamPlayFromPosition(double percent);
};

