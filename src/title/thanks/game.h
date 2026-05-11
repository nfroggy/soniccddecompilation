#include "../../types.h"
void DLL_meminit(char*** pBufTbl, void** pFuncTbl);
void DLL_memfree(void);
void SWdataSet(ushort_union sw1, ushort_union sw2);
void game_init(void);
Sint32 game(void);
void cgmwrt_a(void);
void cgmwrt_b(void);
void v_int(void);
void PutAscii(Uint16 c, Uint16 XPos, Uint16 YPos);
void soundset(Sint16 ReqNo);
void EndingMesColor(void);
Sint32 EndingMes(void);
