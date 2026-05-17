#include <SDL3/SDL.h>
#include "dll.h"

void (*game_init)(void);
Sint32(*game)(void);
void (*DLL_meminit)(char ***pBufTbl, void **pFuncTbl);
void (*DLL_memfree)(void);
void (*SWdataSet)(ushort_union sw1, ushort_union sw2);
Sint32(*Get_vscroll)(void);
Sint32(*Get_scra_h_posiw)(void);
Sint32(*Get_scrb_h_posiw)(void);
Sint32(*FadeProc)(void);
void (*SetDebugFlag)(Uint32 newVal);
void (*GetRoundStr)(Uint16 StageNo, Uint8 Time_Flag, char *buf);

static SDL_SharedObject *currentDll = NULL;

static inline void *loadFunction(SDL_SharedObject *dll, const char *name) {
    void *func = SDL_LoadFunction(dll, name);
    if (!func) {
        SDL_Log("missing export %s", name);
    }
    return func;
}

int DLL_Load(const char *path) {
    DLL_Unload();

    SDL_SharedObject *dll = SDL_LoadObject(path);
    if (!dll) {
        SDL_Log("Failed to load %s: %s", path, SDL_GetError());
        return 0;
    }

    game_init = loadFunction(dll, "game_init");
    game = loadFunction(dll, "game");
    DLL_meminit = loadFunction(dll, "DLL_meminit");
    DLL_memfree = loadFunction(dll, "DLL_memfree");
    SWdataSet = loadFunction(dll, "SWdataSet");
    Get_vscroll = loadFunction(dll, "Get_vscroll");
    Get_scra_h_posiw = loadFunction(dll, "Get_scra_h_posiw");
    Get_scrb_h_posiw = loadFunction(dll, "Get_scrb_h_posiw");
    FadeProc = loadFunction(dll, "FadeProc");
    SetDebugFlag = loadFunction(dll, "SetDebugFlag");
    GetRoundStr = loadFunction(dll, "GetRoundStr");

    if (!game_init || !game || !DLL_meminit) {
        SDL_Log("Missing essential exports!");
        SDL_UnloadObject(dll);
        return 0;
    }

    currentDll = dll;
    return 1;
}

void DLL_Unload(void) {
    if (currentDll) {
        SDL_UnloadObject(currentDll);
        currentDll = NULL;
    }
}