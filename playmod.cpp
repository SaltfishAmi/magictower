/*
	Modified from "BASS simple console player" distributed in BASSMOD library.
	Copyright (c) 1999-2012 Un4seen Developments Ltd.
*/

#include "Main.h"
#include "bass.h"
#pragma comment (lib, "bass.lib")

// display error messages
void Error(const char *text) 
{
	printf("Error(%d): %s\n",BASS_ErrorGetCode(),text);
	BASS_Free();
	exit(0);
}

DWORD WINAPI PlayMod(void* argv)
{
	DWORD chan,act,time,level;
	BOOL ismod;
	QWORD pos;
	int a;

	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
		printf("An incorrect version of BASS was loaded");
		return 0;
	}

	// setup output - default device
	if (!BASS_Init(-1,44100,0,0,NULL))return 0;
		//Error("Can't initialize device");

	// try streaming the file
	if ((chan=BASS_StreamCreateFile(FALSE,argv,0,0,BASS_SAMPLE_LOOP))) {
		pos=BASS_ChannelGetLength(chan,BASS_POS_BYTE);
		ismod=FALSE;
	} else {
		// try loading the MOD (with looping, sensitive ramping, and calculate the duration)
		if (!(chan=BASS_MusicLoad(FALSE,argv,0,0,BASS_SAMPLE_LOOP|BASS_MUSIC_RAMPS|BASS_MUSIC_PRESCAN,1)))
			// not a MOD either
			//Error("Can't play the file");
            return 0;
		else { // count channels
			float dummy;
			for (a=0;BASS_ChannelGetAttribute(chan,BASS_ATTRIB_MUSIC_VOL_CHAN+a,&dummy);a++);
		}
		pos=BASS_ChannelGetLength(chan,BASS_POS_BYTE);
		ismod=TRUE;
	}
	BASS_ChannelPlay(chan,FALSE);

	while (act=BASS_ChannelIsActive(chan)){
        WaitForSingleObject(hMusicMutex, INFINITE);
        if(noSound){
            ReleaseMutex(hMusicMutex);
            break;
        }
        ReleaseMutex(hMusicMutex);
        Sleep(200);
    }

	// wind the frequency down...
	//BASS_ChannelSlideAttribute(chan,BASS_ATTRIB_FREQ,1000,500);
	//Sleep(300);
	// ...and fade-out to avoid a "click"
	BASS_ChannelSlideAttribute(chan,BASS_ATTRIB_VOL,-1,200);
	// wait for slide to finish
	while (BASS_ChannelIsSliding(chan,0)) Sleep(1);
    BASS_Free();
    while(1)
    {
        WaitForSingleObject(hMusicMutex, INFINITE);
        if(!noSound){
            ReleaseMutex(hMusicMutex);
            PlayMod(argv);
            return 1;
        }
        ReleaseMutex(hMusicMutex);
    }
    
    return 1;
}
