#include "../types.h"
void clchgctr(void);
void colchg6a(void);
void clchg_sub0(Uint8 *pChgTime, Uint8 *pChgCnt, Uint8 *pCntTbl,
                PALETTEENTRY *pColTbl);
void fadein0(void);
Sint32 fadein0_new(void);
void fadeout(void);
void flashin(void);
void flashout(void);
void colorset(Sint32 ColorNo);
void colorset2(Sint32 ColorNo);
void colorset3(Sint32 ColorNo);
Sint32 FadeProc(void);
void fin_boss6(Uint8 *pChgTime, Uint8 *pChgCnt);
void fout_boss6(Uint8 *pChgTime, Uint8 *pChgCnt);
void fset_boss6(char d0, Uint8 *pChgCnt);
void flashin_boss6(void);
