#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <SDL3/SDL.h>
#include "frontend_state.h"
#include "../types.h"

#define ROUND_STAGE_R11A 400
#define ROUND_STAGE_R81A 460
#define ROUND_STAGE_R83D 469

int Round_LoadDLL(const char *path);
int Round_LoadStageByMenu(Uint32 stageMenuId);
int Round_LoadStageByName(const char *stageId);
frontend_state Round_Run(SDL_Renderer *renderer);
void Round_Unload(void);

#ifdef __cplusplus
}
#endif
