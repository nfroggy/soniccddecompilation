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
#include "utilities.h"

#define MAX_SPRITES 256
#define FUNC_TABLE_SIZE 64

SDL_Window *window;
SDL_Renderer *renderer;
game_info gameInfo;
score_data scoreData;
ushort_union joy1;
ushort_union joy2;

typedef struct {
    Uint32 On;
    Uint32 Press;
    Uint32 Release;
    Sint16 X1;
    Sint16 Y1;
    Sint16 X2;
    Sint16 Y2;
} frontend_pad_status;

static frontend_pad_status padStatus;

static frontend_pad_status *FrontendPadGet(Uint32 padNo) {
    (void)padNo;
    return &padStatus;
}

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

    if (!Graphics_Init(&window, &renderer)) {
        fprintf(stderr, "Graphics_Init failed\n");
        return -1;
    }

    if (!Sound_Init()) {
        fprintf(stderr, "Sound_Init failed\n");
        return -1;
    }

    if (!DLL_Load("build/bin/Debug/planet.dll")) {
        fprintf(stderr, "DLL_Load planet failed\n");
        SDL_Log("Couldn't load DLL");
        return -1;
    }
    gameInfo.pl_suu = 3;
    functionTbl[54] = (void *)FrontendPadGet;

    DLL_meminit(memoryTbl, functionTbl);
    if (!Graphics_LoadPlanet(gameInfo.sm_adr0, (bmp_info *)gameInfo.pSprBmp, 700)) {
        fprintf(stderr, "Graphics_LoadPlanet failed\n");
        SDL_Log("Couldn't load planet graphics");
        return -1;
    }
    if (SetDebugFlag) {
        SetDebugFlag(0);
    }
    game_init();

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
        Uint32 lastPadOn = padStatus.On;
        joy1.b.h = pressed;
        joy1.b.l = (~lastPressed) & pressed;
        padStatus.On = 0;
        if (keys[SDL_SCANCODE_J]) { padStatus.On |= 0x02; }
        if (keys[SDL_SCANCODE_K]) { padStatus.On |= 0x04; }
        padStatus.Press = (~lastPadOn) & padStatus.On;
        padStatus.Release = lastPadOn & ~padStatus.On;
        SWdataSet(joy1, joy2);

        if (fade_flag && FadeProc) {
            if (FadeProc()) {
                fade_flag = 0;
            }
        }
        else {
            if (game) {
                Sint32 gameResult = game();
                if (gameResult) {
                    SDL_Log("planet mode result %d", gameResult);
                }
            }
        }

        Graphics_DrawPlanet(renderer, &gameInfo);
    }

    DLL_memfree();
    DLL_Unload();
    Graphics_Shutdown();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
	return 0;
}
