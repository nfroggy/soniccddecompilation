#include "types.h"
void action(void);
void speedset(sprite_status *pActwk);
void speedset2(sprite_status *pActwk);
void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
void patset(void);
void spatset(Sint16 xposi, Sint16 yposi, sprite_status *pActwk,
             sprite_data *sprdat, Sint16 cnt);
Sint32 scronchk(sprite_status *pActwk);
Sint32 scronchk2(sprite_status *pActwk);
