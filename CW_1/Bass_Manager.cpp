#include "Bass_Manager.h"

Bass_Manager::Bass_Manager()
{
	BOOL init = BASS_Init(-1, 44100, BASS_DEVICE_3D, NULL, NULL);
	if (!init)
	{
		throw BM_BASS_INIT_ERROR;
	}
}


Bass_Manager::~Bass_Manager()
{
	BASS_Free();
}
