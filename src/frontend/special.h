#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <SDL3/SDL.h>
#include "frontend_state.h"
#include "../types.h"

#define SPECIAL_STAGE_MENU_BASE 470

int Special_LoadDLL(const char *path);
int Special_LoadStage(Uint32 stageMenuId);
frontend_state Special_Run(SDL_Renderer *renderer);
void Special_Unload(void);

#ifdef __cplusplus
}
#endif
