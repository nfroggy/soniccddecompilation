#pragma once
#include <SDL3/SDL.h>
#include "../types.h"

extern Uint16 mapwk[32768];
extern PALETTEENTRY colorwk[64];
extern PALETTEENTRY colorwk2[64];
extern PALETTEENTRY colorwk3[64];
extern PALETTEENTRY colorwk4[64];
extern int_union hscrollbuff[256];
extern Sint32 fade_flag;

Sint32 SetGrid(Sint32 base, Sint32 x, Sint32 y, Sint32 block, Sint32 frip);
void EAsprset(Sint16 x, Sint16 y, Uint16 index, Uint16 linkdata, Uint16 reverse);
void ClrSpriteDebug(void);
void ChangeTileBmp(Sint32 tile_start, Sint32 bmp_no);

int Graphics_LoadSprites(const char *path);
void Graphics_Draw(SDL_Renderer *renderer);
