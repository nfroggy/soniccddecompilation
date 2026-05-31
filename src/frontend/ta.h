#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <SDL3/SDL.h>
#include "frontend_state.h"

int TimeAttack_Load(Sint32 newSession);
frontend_state TimeAttack_Run(SDL_Renderer *renderer);
void TimeAttack_Unload(void);

#ifdef __cplusplus
}
#endif
