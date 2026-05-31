#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <SDL3/SDL.h>
#include "frontend_state.h"

int Warp_LoadDLL(const char *path);
int Warp_Load(void);
frontend_state Warp_Run(SDL_Renderer *renderer);
void Warp_Unload(void);

#ifdef __cplusplus
}
#endif
