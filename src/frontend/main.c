#include <SDL3/SDL.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dll.h"
#include "file.h"
#include "graphics.h"
#include "score.h"
#include "sound.h"
#include "szdd.h"
#include "utilities.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 224
#define TILE_SIZE 8
#define GRID_W 128
#define GRID_H 64
#define STAGE_GRID_W 64
#define STAGE_GRID_H 32
#define MAX_SPRITES 256
#define FUNC_TABLE_SIZE 64

SDL_Window *window;
SDL_Renderer *renderer;
game_info gameInfo;
score_data scoreData;
ushort_union joy1;
ushort_union joy2;

Uint16 *pMapwk = mapwk;
PALETTEENTRY *pColorwk = colorwk;
PALETTEENTRY *pColorwk2 = colorwk2;
PALETTEENTRY *pColorwk3 = colorwk3;
PALETTEENTRY *pColorwk4 = colorwk4;
int_union *pHscrollbuff = hscrollbuff;

char **memoryTbl[11] = {
    (char **)&pMapwk,
    (char **)&pColorwk,
    (char **)&pColorwk2,
    (char **)&pColorwk3,
    (char **)&pColorwk4,
    (char **)&pHscrollbuff,
    (char **)&fade_flag,
    (char **)&gameInfo,
    (char **)&renderer,
    (char **)&window,
    (char **)&scoreData,
};

void *functionTbl[64] = {
    (void *)SetGrid,
    (void *)EAsprset,
    (void *)ClrSpriteDebug,
    (void *)WaveRequest,
    (void *)CDPlay,
    (void *)CDPause,
    (void *)ChangeTileBmp,
    (void *)ReadScoreIndx,
    (void *)WriteScoreData,
    (void *)SetScoreDate,
    (void *)WaveAllStop,
    (void *)sMemAlloc,
    (void *)sMemFree,
    (void *)sMemSet,
    (void *)sMemCpy,
    (void *)sMemCmp,
    (void *)sRandom,
    (void *)sStrcpy,
    (void *)sStrncmp,
    NULL,
    (void *)sPrintf,
    (void *)sOutputDebugString,
    (void *)sOpenFile,
    (void *)sReadFile,
    (void *)sCloseFile,
    (void *)sGetFileSize,
};

int main(int argc, char **argv) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Couldn't init SDL: %s\n", SDL_GetError());
        return -1;
    }

    if (!SDL_CreateWindowAndRenderer("Sonic CD", SCREEN_WIDTH * 3, SCREEN_HEIGHT * 3, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return -1;
    }
    SDL_SetRenderLogicalPresentation(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderVSync(renderer, 1);

    if (!DLL_Load("build/bin/Debug/r11a.dll")) {
        SDL_Log("Couldn't load DLL");
        return -1;
    }
    gameInfo.time_flag = 1;

    DLL_meminit(memoryTbl, functionTbl);
    SetDebugFlag(0);
    game_init();

    Graphics_LoadSprites("R1/11A/SCMP11A.CM_");

    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = 0;
            }
        }

        Uint8 pressed = 0;
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_W]) { pressed |= 0x01; }
        if (keys[SDL_SCANCODE_S]) { pressed |= 0x02; }
        if (keys[SDL_SCANCODE_A]) { pressed |= 0x04; }
        if (keys[SDL_SCANCODE_D]) { pressed |= 0x08; }
        if (keys[SDL_SCANCODE_J]) { pressed |= 0x10; }
        if (keys[SDL_SCANCODE_K]) { pressed |= 0x20; }
        if (keys[SDL_SCANCODE_L]) { pressed |= 0x40; }
        if (keys[SDL_SCANCODE_RETURN]) { pressed |= 0x80; }
        Uint8 lastPressed = joy1.b.h;
        joy1.b.h = pressed;
        joy1.b.l = (~lastPressed) & pressed;
        SWdataSet(joy1, joy2);

        if (fade_flag && FadeProc) {
            if (FadeProc()) {
                fade_flag = 0;
            }
        }
        else if (game && game()) {
            SDL_Log("game requested exit");
            running = 0;
        }

        Graphics_Draw(renderer);
    }

    DLL_memfree();
    DLL_Unload();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
	return 0;
}
