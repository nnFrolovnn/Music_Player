#pragma once
#include <stdio.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>

#include "bass.h"
#include "bass_cmponents\bassenc.h"
#include "bass_cmponents\bassenc_mp3.h"
#include "bass_cmponents\bassmod.h"
#include "bass_cmponents\unmo3.h"

#pragma comment (lib, "bass.lib")
#pragma comment (lib, "unmo3.lib")
#pragma comment (lib, "bassmod.lib")

struct musicFile {
	QWORD fileSize;
	char* filePath;
	char* name;
};

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

	//percent 0..1
	void StreamSetPosition(double percent);
	//volume  0..1
	void StreamSetVolume(double volume);

	int GetLastError();
	char* GetStreamInfo();
	double GetStreamTime();
	DWORD GetFlags();

	BOOL AddFileNameToList(char* filePath);

	BOOL MusicHasPlayed();
	BOOL MusicCanPlay();
	BOOL MusicIsPlayingOrIsPaused();

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
	HMUSIC LoadMusicForPlaying(musicFile* fileToPlay, double percent, BOOL sameFile = FALSE);
		  
	void StreamPlayFromPosition(double percent);
};

