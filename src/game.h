#include "types.h"
void DLL_meminit(char ***pBufTbl, void **pFuncTbl);
void DLL_memfree(void);
Sint32 Get_vscroll(void);
Sint32 Get_scra_h_posiw(void);
Sint32 Get_scrb_h_posiw(void);
void SetDebugFlag(Uint32 NewVal);
Sint32 game(void);
void game_init(void);
void play_act_set(void);
void flow_act_set(void);
void scdset(void);
void syspatchg(void);
void sdfdout(void);
void sdfdin(void);
void da_set(void);
