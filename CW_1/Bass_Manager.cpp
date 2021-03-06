#include "Bass_Manager.h"

Bass_Manager::Bass_Manager()
{
	stream = 0;
	musicFilesCount = 0;
	playList = NULL;
	isPlaying = FALSE;
	isPause = FALSE;
	currentMusicFile = 0;
	BOOL init = BASS_Init(-1, 44100, BASS_DEVICE_STEREO | BASS_DEVICE_3D , 0, 0);
}

Bass_Manager::Bass_Manager(HWND hwnd)
{
	stream = 0;
	musicFilesCount = 0;
	playList = NULL;
	isPlaying = FALSE;
	isPause = FALSE;
	currentMusicFile = 0;
	BOOL init = BASS_Init(-1, 44100, BASS_DEVICE_STEREO | BASS_DEVICE_3D, hwnd, 0);
}

Bass_Manager::~Bass_Manager()
{
	BASS_Free();
}

void Bass_Manager::StreamPlay()
{
	if (musicFilesCount == 0 && MusicIsPlayingOrIsPaused())
	{
		StreamStop();
	}

	StreamPlayFromPosition(0);
}

void Bass_Manager::StreamPlayFromPosition(double percent)
{
	if (playList != NULL)
	{
		if (!isPlaying && !isPause)
		{
			if (stream != 0)
			{
				BASS_ChannelStop(stream);
				BASS_StreamFree(stream);
			}
			if (percent <= 0.01)
			{
				percent = 0;
			}
			stream = LoadMusicForPlaying(&playList[currentMusicFile], percent);
			if (stream > 0) 
			{
				BASS_ChannelPlay(stream, TRUE);
				isPlaying = TRUE;
				isPause = FALSE;
			}
			else
			{
				//TODO: send err
			}
		}
		else if (isPlaying && !isPause)
		{
			StreamPause();
		}
		else if (!isPlaying && isPause && stream != 0)
		{
			BASS_ChannelPlay(stream, FALSE);
			isPause = FALSE;
			isPlaying = TRUE;
		}
	}
}

void Bass_Manager::StreamStop()
{
	if (stream != 0)
	{
		if (isPlaying || isPause)
		{
			BASS_ChannelStop(stream);
			BASS_StreamFree(stream);
			stream = 0;
		}
		isPlaying = FALSE;
		isPause = FALSE;
	}
}

void Bass_Manager::StreamPause()
{
	if (stream != 0 && isPlaying)
	{
		BASS_ChannelPause(stream);
		isPause = TRUE;
		isPlaying = FALSE;
	}
}

void Bass_Manager::StreamPlayNext()
{
	if (musicFilesCount > 0 )
	{
		currentMusicFile = (currentMusicFile + 1) % musicFilesCount;
		StreamStop();
		StreamPlay();
	}
	else if (MusicIsPlayingOrIsPaused())
	{
		StreamStop();
	}
}

void Bass_Manager::StreamPlayPrevios()
{
	if (musicFilesCount > 0)
	{
		currentMusicFile = (currentMusicFile - 1 + musicFilesCount) % musicFilesCount;
		StreamStop();
		StreamPlay();
	}
	else if(MusicIsPlayingOrIsPaused())
	{
		StreamStop();		
	}
}

void Bass_Manager::SetCurrentFile(int fileNumber)
{
	currentMusicFile = (fileNumber > 0 && fileNumber < musicFilesCount) ? fileNumber : currentMusicFile;
}

void Bass_Manager::StreamSetPosition(double percent)
{
	if (stream != 0 && percent >= 0 && percent <= 1)
	{
		StreamStop();
		StreamPlayFromPosition(percent);
	}
}

void Bass_Manager::StreamSetVolume(double volume)
{
	if (MusicIsPlayingOrIsPaused())
	{
		if (volume >= 0 && volume <= 1)
		{
			BASS_SetVolume(volume);
		}
	}
}

void Bass_Manager::RemoveFile(musicFile filetoRemove)
{
	if (playList != NULL)
	{
		musicFile* newPlayList = new musicFile[musicFilesCount - 1];
		int j = 0;
		BOOL removed = FALSE;
		for (int i = 0; i < musicFilesCount; i++)
		{
			if (strcmp(playList[i].name, filetoRemove.name) &&
				strcmp(playList[i].filePath, filetoRemove.filePath) &&
				playList[i].fileSize != filetoRemove.fileSize && !removed)
			{
				newPlayList[j] = playList[i];
				j++;
			}
			else
			{
				removed = TRUE;
			}
		}

		playList = newPlayList;
		musicFilesCount--;
		if (musicFilesCount == 0)
		{
			playList = NULL;
		}
	}
}

void Bass_Manager::RemoveFile(int number)
{
	if (playList != NULL)
	{
		musicFile* newPlayList = new musicFile[musicFilesCount - 1];
		int j = 0;
		for (int i = 0; i < musicFilesCount; i++)
		{
			if (i != number)
			{
				newPlayList[j] = playList[i];
				j++;
			}
		}
		
		playList = newPlayList;
		musicFilesCount--;

		if (musicFilesCount == 0)
		{
			playList = NULL;
			currentMusicFile = 0;
		}
		else
		{
			if (currentMusicFile > number)
			{
				currentMusicFile = (currentMusicFile - 1) % musicFilesCount;
			}
		}
	}
}

int Bass_Manager::GetLastError()
{
	return BASS_ErrorGetCode();
}

char * Bass_Manager::GetStreamInfo()
{


	return nullptr;
}

double Bass_Manager::GetStreamTime()
{
	if (stream != 0)
	{
		QWORD len = BASS_ChannelGetLength(stream, BASS_POS_BYTE);
		return BASS_ChannelBytes2Seconds(stream, len);
	}

	return 0;
}

double Bass_Manager::GetVolume()
{
	return BASS_GetVolume();
}

DWORD Bass_Manager::GetFlags()
{
	return 0;
}

int Bass_Manager::GetCurrentPlayingFile()
{
	return currentMusicFile;
}

musicFile * Bass_Manager::GetPlayList()
{
	return playList;
}

int Bass_Manager::GetPlayListSize()
{
	return musicFilesCount;
}

BOOL Bass_Manager::AddFileNameToList(char * filePath)
{
	
	if (filePath != NULL)
	{
		const int len = strlen(filePath);
		const char* extensions = ".mod.mp3.wav.aif.mpc.it";

		int slashPos = 0;
		int dotPos = len - 1;
		for (int i = len - 1; i > 0; i--)
		{
			if (filePath[i] == '.' && dotPos == len - 1)
			{
				dotPos = i;
			}
			if (filePath[i] == '\\' || filePath[i] == '//')
			{
				slashPos = i + 1;
				break;
			}
		}

		//check extension
		char * fileExtension = new char[len - dotPos + 1];
		for (int i = 0; i < len - dotPos; i++)
		{
			fileExtension[i] = filePath[i + dotPos];
		}
		fileExtension[len - dotPos] = '\0';

		if (strstr(extensions, fileExtension))
		{
			if (playList != NULL)
			{
				musicFile* temp = new musicFile[musicFilesCount + 1];
				for (int i = 0; i < musicFilesCount; i++)
				{
					temp[i] = playList[i];
				}
				playList = temp;
				musicFilesCount++;
				temp = NULL;
			}
			else
			{
				musicFilesCount = 1;
				playList = new musicFile[1];
			}

			//get name of file
			char * tempName = new char[dotPos - slashPos + 2];
			for (int i = slashPos; i < dotPos; i++)
			{
				tempName[i - slashPos] = filePath[i];
			}
			tempName[dotPos - slashPos] = '\0';
			playList[musicFilesCount - 1].name = tempName;

			//save path
			playList[musicFilesCount - 1].filePath = new char[len + 1];
			strcpy_s(playList[musicFilesCount - 1].filePath, len + 1, filePath);

			//get file size

			FILE* file;
			fopen_s(&file, playList[musicFilesCount - 1].filePath, "r");
			long int size_of_file = 0;
			fseek(file, 0L, SEEK_END);
			size_of_file = ftell(file);
			fclose(file);

			playList[musicFilesCount - 1].fileSize = size_of_file;

			return TRUE;
		}
	}
	return FALSE;
}

BOOL Bass_Manager::MusicHasPlayed()
{
	return (stream != 0 && (isPlaying || isPause)) ? TRUE : FALSE;
}

BOOL Bass_Manager::MusicCanPlay()
{
	return (playList != NULL) ? TRUE : FALSE;
}

BOOL Bass_Manager::MusicIsPlaying()
{
	return isPlaying;
}

BOOL Bass_Manager::MusicIsPlayingOrIsPaused()
{
	return (isPause || isPlaying);
}

float* Bass_Manager::GetFFT()
{
	if (stream != NULL)
	{
		float fft[512];
		int count = BASS_ChannelGetData(stream, fft, BASS_DATA_FFT1024);
		return fft;
	}

	return NULL;
}

void * Bass_Manager::LoadMusicFileToMemory(musicFile* mFile)
{
	if (mFile != NULL)
	{
		const QWORD len = mFile->fileSize;
		char * memoryPos = new char[len + 1];

		FILE * file;
		if (fopen_s(&file, mFile->filePath, "rb"))
		{
			free(memoryPos);
			return nullptr;
		}

		const QWORD readedBytes = fread_s(memoryPos, len * sizeof(char), sizeof(char), len, file);

		fclose(file);
		if (readedBytes == len)
		{
			return memoryPos;
		}	
	}
	return nullptr;
}

HSTREAM Bass_Manager::LoadMusicForPlaying(musicFile* fileToPlay, double percent, BOOL sameFile)
{
	HSTREAM music = 0;

	if (fileToPlay != NULL)
	{
		try
		{
			music = BASS_MusicLoad(FALSE, fileToPlay->filePath, (QWORD)(percent*fileToPlay->fileSize), 0,
				BASS_MUSIC_LOOP | BASS_MUSIC_RAMPS | BASS_MUSIC_SURROUND | BASS_UNICODE, 0);
			if (music == 0)
			{
				void* fileInMemory = (void *)LoadMusicFileToMemory(fileToPlay);

				unsigned* filesize = (unsigned*)malloc(sizeof(unsigned));
				*filesize = fileToPlay->fileSize;

				if (fileInMemory != nullptr && filesize != NULL)
				{
					music = BASS_MusicLoad(TRUE, fileInMemory, (QWORD)(percent*fileToPlay->fileSize), *filesize,
						BASS_MUSIC_LOOP | BASS_MUSIC_RAMPS | BASS_MUSIC_SURROUND | BASS_UNICODE, 0);

					if (music == 0)
					{
						int decode = UNMO3_Decode(&fileInMemory, filesize, 0);
						if (decode != 3)
						{
							music = BASS_MusicLoad(TRUE, fileInMemory, (QWORD)(percent*fileToPlay->fileSize), *filesize,
								BASS_MUSIC_LOOP | BASS_MUSIC_RAMPS | BASS_MUSIC_SURROUND | BASS_UNICODE, 0);
						}
						free(fileInMemory);
					}
				}
				free(filesize);
			}

			if (music == 0)
			{
				music = BASS_StreamCreateFile(FALSE, fileToPlay->filePath, (QWORD)(percent*fileToPlay->fileSize), 
					BASS_MUSIC_LOOP | BASS_MUSIC_RAMPS | BASS_MUSIC_SURROUND | BASS_UNICODE, 0);
			}
		}
		catch(...)
		{
			return 0;
		}
	}
	return music;
}
