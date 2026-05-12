#include "types.h"
void flagwkclr(void);
void actsetchk(void);
void actsetinit(void);
void actset(void);
Sint32 tm_setchk(Uint8 cnt, Uint16 *pIndex);
Sint32 actnoset(Uint8 cnt, Uint16 *pIndex);
Sint32 actwkchk(sprite_status **ppActwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint32 frameout_s0(sprite_status *pActwk);
