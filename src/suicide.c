#include "equ.h"
#include "suicide.h"
#include "action.h"
#include "actset.h"
#include "loader2.h"

Sint16 enemy_suicide(sprite_status *pActwk) {
    Sint16 xposi, yposi;

    if (generate_flag == 0)
        return 0;
    if (time_flag == 1) {
        if (pActwk->userflag.b.h == 0)
            return 0;
    }
    xposi = pActwk->xposi.w.h;
    yposi = pActwk->yposi.w.h;
    frameout(pActwk);
    pActwk->xposi.w.h = xposi;
    pActwk->yposi.w.h = yposi;
    pActwk->actno = 24;
    if (pActwk->actflg < 0)
        soundset(158);
    return -1;
}

Sint16 friend_suicide(sprite_status *pActwk) {
    if (pActwk->userflag.b.h < 0)
        return 0;
    if (time_flag < 2) {
        if (projector_flag)
            return 0;
        frameout_s(pActwk);
        return -1;
    }
    if (generate_flag)
        return 0;
    frameout(pActwk);
    return -1;
}
