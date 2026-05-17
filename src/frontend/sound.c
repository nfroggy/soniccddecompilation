#include <SDL3/SDL.h>
#include "sound.h"

void WaveRequest(Sint16 req_no) {
	SDL_Log("WaveRequest(%d)", req_no);
}

void CDPlay(Sint16 track_number) {
	SDL_Log("CDPlay(%d)", track_number);
}

void CDPause(Sint16 mode) {
	SDL_Log("CDPause(%d)", mode);
}

void WaveAllStop(void) {

}