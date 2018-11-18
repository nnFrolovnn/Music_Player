#include "Bass_Manager.h"

Bass_Manager::Bass_Manager()
{
	stream = 0;

	BOOL init = BASS_Init(-1, 44100, BASS_DEVICE_STEREO | BASS_DEVICE_3D, NULL, NULL);
}


Bass_Manager::~Bass_Manager()
{
	BASS_Free();
}

int Bass_Manager::StreamCreate(char filePath[])
{
	stream = BASS_StreamCreateFile(FALSE, filePath, 0, 0, 0);
	return 0;
}

void Bass_Manager::StreamPlay()
{
	BASS_ChannelPlay(stream, FALSE);
}

void Bass_Manager::StreamStop()
{
	BASS_ChannelStop(stream);
}

void Bass_Manager::StreamPause()
{
	BASS_ChannelPause(stream);
}

int Bass_Manager::GetLastError()
{
	return BASS_ErrorGetCode();
}


