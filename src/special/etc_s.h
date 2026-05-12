#include "../types.h"
void ufo_dec(void);
void ring_add(Uint16 iD0);
void time_dec(void);
void dec(void);
void time_time_attack(void);
void angleget(Sint16 iXtget, Sint16 iYtget, Sint16 iXbase, Sint16 iYbase,
              Uint8 *iAngle, Uint8 *iDirflg);
void speedget(Uint8 bAngle, Uint8 bDirflg, Sint16 iBaseSpd, Sint32 *lXspeed,
              Sint32 *lYspeed);
Sint32 dstnsget(Uint8 bAngle, Uint8 bDirflg, Sint16 iXposi, Sint16 iYposi,
                Sint16 iXbase, Sint16 iYbase);
Sint32 random(void);
