#pragma once
#include "../types.h"

extern void (*game_init)(void);
extern Sint32 (*game)(void);
extern void (*DLL_meminit)(char ***pBufTbl, void **pFuncTbl);
extern void (*DLL_memfree)(void);
extern void (*SWdataSet)(ushort_union sw1, ushort_union sw2);
extern Sint32 (*Get_vscroll)(void);
extern Sint32 (*Get_scra_h_posiw)(void);
extern Sint32 (*Get_scrb_h_posiw)(void);
extern Sint32 (*FadeProc)(void);
extern void (*SetDebugFlag)(Uint32 newVal);
extern void (*GetRoundStr)(Uint16 StageNo, Uint8 Time_Flag, char *buf);

int DLL_Load(const char *path);
void DLL_Unload(void);
