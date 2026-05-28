#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <stdio.h>
#include <string.h>
#include "sound.h"

static MIX_Mixer *mixer;
static MIX_Track *musicTrack;
static MIX_Audio *musicAudio;
static Sint16 currTrack = -1;

typedef struct {
	char *filename;
	bool loop;
} MusicInfo;

static MusicInfo musicTbl[] = {
	{ NULL, false },
	{ "R1A.ogg", true},
	{ "R1C.ogg", true },
	{ "R1D.ogg", true },
	{ "R3A.ogg", true },
	{ "R3C.ogg", true },
	{ "R3D.ogg", true },
	{ "R4A.ogg", true },
	{ "R4C.ogg", true },
	{ "R4D.ogg", true },
	{ "R5A.ogg", true },
	{ "R5C.ogg", true },
	{ "R5D.ogg", true },
	{ "R6A.ogg", true },
	{ "R6C.ogg", true },
	{ "R6D.ogg", true },
	{ "R7A.ogg", true },
	{ "R7C.ogg", true },
	{ "R7D.ogg", true },
	{ "R8A.ogg", true },
	{ "R8C.ogg", true },
	{ "R8D.ogg", true },
	{ "Boss.ogg", true },
	{ "FinalBoss.ogg", true },
	{ "TitleScreen.ogg", false },
	{ "TimeAttack.ogg", true },
	{ "ZoneComplete.ogg", false },
	{ "SpeedShoes.ogg", false },
	{ "Invincibility.ogg", false },
	{ "GameOver.ogg", false },
	{ "SpecialStage.ogg", true },
	{ "DAGarden.ogg", true },
	{ NULL, false },
	{ NULL, false },
	{ "R1B.ogg", true },
	{ "R3B.ogg", true },
	{ "R4B.ogg", true },
	{ "R5B.ogg", true },
	{ "R6B.ogg", true },
	{ "R7B.ogg", true },
	{ "R8B.ogg", true },
};

#define NUM_SOUND_EFFECTS 86
static MIX_Audio *soundEffects[NUM_SOUND_EFFECTS];

void WaveRequest(Sint16 req_no) {
	SDL_Log("WaveRequest(%d)", req_no);
	if (req_no < 0 || req_no >= NUM_SOUND_EFFECTS || !soundEffects[req_no]) {
		return;
	}
	MIX_PlayAudio(mixer, soundEffects[req_no]);
}

void CDPlay(Sint16 track_number) {
	SDL_Log("CDPlay(%d)", track_number);
	if (track_number < 2 || (track_number - 2 >= SDL_arraysize(musicTbl)) || (currTrack == track_number)) {
		return;
	}
	MusicInfo *mi = &musicTbl[track_number - 2];
	MIX_StopTrack(musicTrack, 0);
	if (mi->filename) {
		char filenameBuff[80];
		snprintf(filenameBuff, sizeof(filenameBuff), "assets/Music/%s", mi->filename);
		if (musicAudio) {
			MIX_DestroyAudio(musicAudio);
		}
		musicAudio = MIX_LoadAudio(mixer, filenameBuff, false);
		if (!musicAudio) {
			SDL_Log("Couldn't read audio: %s", SDL_GetError());
			return;
		}
		MIX_SetTrackAudio(musicTrack, musicAudio);
		SDL_PropertiesID options = SDL_CreateProperties();
		SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, mi->loop ? -1 : 0);
		MIX_PlayTrack(musicTrack, options);
		SDL_DestroyProperties(options);
		currTrack = track_number;
	}

}

void CDPause(Sint16 mode) {
	SDL_Log("CDPause(%d)", mode);
	if (mode) {
		MIX_ResumeTrack(musicTrack);
	}
	else {
		MIX_PauseTrack(musicTrack);
	}
}

void WaveAllStop(void) {
	SDL_Log("WaveAllStop()");
}

int Sound_Init(void) {
	if (!MIX_Init()) {
		SDL_Log("Couldn't init SDL_Mixer: %s", SDL_GetError());
		return 0;
	}

	mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
	if (!mixer) {
		SDL_Log("Couldn't create mixer on default device: %s", SDL_GetError());
		return 0;
	}

	musicTrack = MIX_CreateTrack(mixer);
	if (!musicTrack) {
		SDL_Log("Couldn't create music track: %s", SDL_GetError());
		return 0;
	}

	char filenameBuff[80];
	for (int i = 0; i < NUM_SOUND_EFFECTS; i++) {
		snprintf(filenameBuff, sizeof(filenameBuff), "assets/SFX/%02d.wav", i);
		soundEffects[i] = MIX_LoadAudio(mixer, filenameBuff, true);
		if (!soundEffects[i]) {
			SDL_Log("couldn't init sound %d", i);
		}
	}

	return 1;
}
