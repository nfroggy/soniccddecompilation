#include "../equ.h"
#include "iwa5roll.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../ridechk.h"

static void iwa5roll_init(sprite_status *pActwk);
static void iwa5roll_move(sprite_status *pActwk);

typedef struct {
    Sint16 parent_actor;
    int_union origin_x;
    int_union origin_y;
    short_union angle;
    Sint16 angular_speed;
} iwa5roll_work;

static iwa5roll_work *iwa5roll_work_get(sprite_status *pActwk) {
    return (iwa5roll_work *)pActwk->actfree;
}

static sprite_pattern pat_iwa5roll_00 = {1, {{-8, -8, 0, 429}}};
sprite_pattern *pat_iwa5roll[1] = {&pat_iwa5roll_00};

void iwa5roll(sprite_status *pActwk) {
    if (pActwk->r_no0)
        iwa5roll_move(pActwk);
    else
        iwa5roll_init(pActwk);
}

static void iwa5roll_init(sprite_status *pActwk) {
    iwa5roll_work *work = iwa5roll_work_get(pActwk);
    sprite_status *pNewactwk;
    iwa5roll_work *new_work;
    Sint16 wD1, wD3, wD4, wD5, wD6;
    Sint16 wA5;
    Sint16 iwa5roll_tbl[6] = {128, 1, 192, 1, 256, 1};

    wD1 = iwa5roll_tbl[time_flag * 2];
    wA5 = iwa5roll_tbl[time_flag * 2 + 1];
    if (pActwk->userflag.b.h)
        wD1 *= -1;

    work->angular_speed = wD1;

    wD6 = 3;
    wD5 = 0;
    wD4 = 0;
    wD3 = 0;
    pNewactwk = pActwk;
    goto label1;

    do {
        wD5 = 0;
        wD3 = wD4;

        do {
            if (actwkchk(&pNewactwk) != 0)
                goto label2;

        label1:
            new_work = iwa5roll_work_get(pNewactwk);
            new_work->parent_actor = (Sint16)(pActwk - actwk);
            pNewactwk->actflg |= 4;
            pNewactwk->r_no0 = 2;
            pNewactwk->sprpri = 3;
            pNewactwk->sprhs = 8;
            pNewactwk->sprhsize = 8;
            pNewactwk->sprvsize = 8;
            pNewactwk->sproffset = 17152;
            pNewactwk->patbase = pat_iwa5roll;
            pNewactwk->actno = pActwk->actno;
            pNewactwk->xposi.w.h = pActwk->xposi.w.h;
            pNewactwk->yposi.w.h = pActwk->yposi.w.h;
            new_work->origin_x.l = pActwk->xposi.l;
            new_work->origin_y.l = pActwk->yposi.l;
            new_work->angular_speed = work->angular_speed;

            new_work->angle.w = wD3;
            wD3 += 2048;
        } while (++wD5 <= wA5);
        wD4 += 16384;
    } while (--wD6 >= 0);

label2:
    work->parent_actor = 0;
}

static void iwa5roll_move(sprite_status *pActwk) {
    iwa5roll_work *work = iwa5roll_work_get(pActwk);
    Uint8 bD0;
    Sint16 Sp;
    Uint16 Sin, Cos;
    int_union lD0, lD1;
    int_union lD4, lD5;
    Sint32 lD3;

    if (work->parent_actor) {
        if (actwk[work->parent_actor].actno != 39) {
            frameout(pActwk);
            return;
        }
    }

    work->angle.w += work->angular_speed;
    lD1.l = 0;
    lD0.l = 0;
    sinset(work->angle.b.h, (Sint16 *)&Sin, (Sint16 *)&Cos);
    lD1.w.h = Cos;
    lD0.w.h = Sin;
    lD5.l = lD1.l / 4 + lD1.l / 8;
    lD4.l = lD0.l / 4 + lD0.l / 8;
    lD5.l += work->origin_x.l;
    lD4.l += work->origin_y.l;
    lD3 = pActwk->xposi.l;
    pActwk->xposi.l = lD5.l;
    pActwk->yposi.l = lD4.l;
    lD5.l = (lD5.l - lD3) >> 8;
    pActwk->xspeed.w = lD5.w.l;

    Sp = pActwk->xspeed.w;
    pActwk->xspeed.w = 0;
    pActwk->sprvsize = 10;
    if (hitchk(pActwk, &actwk[0]))
        bD0 = 255;
    else
        bD0 = 0;
    pActwk->xspeed.w = Sp;
    pActwk->sprvsize = 8;
    if (bD0)
        hitchk(pActwk, &actwk[0]);

    actionsub(pActwk);

    if (!work->parent_actor) {
        frameout_s00(pActwk, work->origin_x.w.h);
    }
}
