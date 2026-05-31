#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "../types.h"
#include "../title/common/engine_dll.h"

extern void (*game_init)(void);
extern Sint32(*game)(void);
extern void (*DLL_meminit)(char ***pBufTbl, void **pFuncTbl);
extern void (*DLL_memfree)(void);
extern void (*Title_DLLInit)(engine_dll *lpDllIn, char ***pBufTbl, void **pFuncTbl);
extern Sint32(*Title_DLLMain)(void);
extern void (*Title_DLLEnd)(void);
extern void (*SWdataSet)(ushort_union sw1, ushort_union sw2);
extern Sint32(*Get_vscroll)(void);
extern Sint32(*Get_scra_h_posiw)(void);
extern Sint32(*Get_scrb_h_posiw)(void);
extern Sint32(*FadeProc)(void);
extern void (*SetDebugFlag)(Uint32 newVal);
extern void (*GetRoundStr)(Uint16 StageNo, Uint8 Time_Flag, char *buf);
extern void (*Special_block_chg)(Uint16 *hane1, Uint16 *hane2, Uint16 *dmg1, Uint16 *dmg2);

int DLL_Load(const char *path);
int DLL_LoadTitle(const char *path);
void DLL_Unload(void);

#ifdef __cplusplus
}
#endif
