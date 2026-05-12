#include "../types.h"
Uint16 cntplus(Uint8 *work, Uint16 plus_data, Uint16 under_limit,
               Uint16 upper_limit);
void clchgctr(void);
void colchg3a(void);
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
