/////////////////////////////////////////////////////////////////////////////////////////////
// sound.cpp
/////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <windowsx.h>

#define CDXINCLUDEALL
#include <CDX.h>

#include "breakout.h"

CDXSound *Sound;
CDXSoundBuffer *Sounds[SOUND_MAX];

BOOL InitSound(HWND hWnd)
{
	Sound = new CDXSound();
	if(FAILED(Sound->Create(hWnd)))
		return FALSE;
	
	Sounds[SOUND_BALLHIT] = new CDXSoundBuffer;
	Sounds[SOUND_BALLHIT]->Load(Sound, "BALLHIT.WAV");

	Sounds[SOUND_BLOCKHIT] = new CDXSoundBuffer;
	Sounds[SOUND_BLOCKHIT]->Load(Sound, "BLOCKHIT.WAV");

	Sounds[SOUND_WALLHIT] = new CDXSoundBuffer;
	Sounds[SOUND_WALLHIT]->Load(Sound, "WALLHIT.WAV");

	Sounds[SOUND_DEAD] = new CDXSoundBuffer;
	Sounds[SOUND_DEAD]->Load(Sound, "DEAD.WAV");

	Sounds[SOUND_WON] = new CDXSoundBuffer;
	Sounds[SOUND_WON]->Load(Sound, "WON.WAV");
	
	return TRUE;
}

void FiniSound(void)
{
	SAFEDELETE(Sounds[SOUND_BALLHIT]);
	SAFEDELETE(Sounds[SOUND_BLOCKHIT]);
	SAFEDELETE(Sounds[SOUND_WALLHIT]);
	SAFEDELETE(Sounds[SOUND_DEAD]);
	SAFEDELETE(Sounds[SOUND_WON]);

	SAFEDELETE(Sound);
}
