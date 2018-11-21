#include "Bass_Manager.h"

Bass_Manager::Bass_Manager()
{
	stream = 0;
	musicFilesCount = 0;
	playList = NULL;
	isPlaying = FALSE;
	isPause = FALSE;

	BOOL init = BASS_Init(-1, 44100, BASS_DEVICE_STEREO | BASS_DEVICE_3D, NULL, NULL);
}

Bass_Manager::~Bass_Manager()
{
	BASS_Free();
}

void Bass_Manager::StreamPlay()
{
	if (playList != NULL) 
	{
		if (!isPlaying && !isPause)
		{
			stream = BASS_StreamCreateFile(FALSE, playList[0].filePath, 0, 0, 0);

			BASS_ChannelPlay(stream, TRUE);
			isPlaying = TRUE;
			isPause = FALSE;
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

BOOL Bass_Manager::AddFileNameToList(char * filePath)
{
	if (filePath != NULL)
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

		const int len = strlen(filePath);

		playList[musicFilesCount - 1].filePath = new char[len + 1];
		strcpy_s(playList[musicFilesCount - 1].filePath, len + 1, filePath);
		playList[musicFilesCount - 1].name = playList[musicFilesCount - 1].filePath;

		return TRUE;
	}
	return FALSE;
}

BOOL Bass_Manager::MusicHasPlayed()
{
	return (stream != 0 && (isPlaying || isPause))? TRUE: FALSE;
}

BOOL Bass_Manager::MusicCanPlay()
{
	return (playList != NULL)? TRUE: FALSE;
}

