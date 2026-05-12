#include "../types.h"
void clchgctr(void);
void colchg8c(void);
void colchg_boss8(Uint8 *a3, Uint8 *a4);
void clchg_sub0(Uint8 *pChgTime, Uint8 *pChgCnt, Uint8 *pCntTbl,
                PALETTEENTRY *pColTbl);
void fadein0(void);
Sint32 fadein0_new(void);
void fadein_boss8(void);
void fadein1_boss8(PALETTEENTRY *lpPeDest, PALETTEENTRY *lpPeSrc);
void fadeout(void);
void flashin(void);
void flashout(void);
void colorset(Sint32 ColorNo);
void colorset2(Sint32 ColorNo);
void colorset3(Sint32 ColorNo);
Sint32 FadeProc(void);
