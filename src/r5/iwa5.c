#include <stddef.h>

#include "../equ.h"
#include "../dircol.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../ridechk.h"

typedef struct {
    Sint16 dx;
    Sint16 dy;
    Sint32 sx;
    Sint32 sy;
} init_data;

#pragma pack(push, 1)
typedef struct {
    Sint32 x_velocity;
    Sint32 y_velocity;
} iwa5_work;
#pragma pack(pop)

_Static_assert(offsetof(iwa5_work, x_velocity) == 0,
               "iwa5_work.x_velocity must map to offset 0");
_Static_assert(offsetof(iwa5_work, y_velocity) == 4,
               "iwa5_work.y_velocity must map to offset 4");
_Static_assert(sizeof(iwa5_work) <= sizeof(((sprite_status *)0)->actfree),
               "iwa5_work must fit in sprite_status.actfree");

static iwa5_work *iwa5_work_get(sprite_status *pActwk) {
    return (iwa5_work *)pActwk->actfree;
}

static void m_init(sprite_status *pActwk);
static void m_wait(sprite_status *pActwk);
static void m_fall(sprite_status *pActwk);

static sprite_pattern pat00 = {1, {{-16, -16, 128, 450}}};
static sprite_pattern pat01 = {1, {{-8, -8, 0, 451}}};
static sprite_pattern pat02 = {1, {{-8, -8, 0, 452}}};
static sprite_pattern pat03 = {1, {{-8, -8, 0, 453}}};
static sprite_pattern pat04 = {1, {{-8, -8, 0, 454}}};
sprite_pattern *pat_iwa5[5] = {&pat00, &pat01, &pat02, &pat03, &pat04};

void iwa5(sprite_status *pActwk) {
    iwa5_work *work = iwa5_work_get(pActwk);
    void (*tbl[3])(sprite_status *) = {&m_init, &m_wait, &m_fall};

    if (pActwk->userflag.b.h & 128) {
        work->y_velocity += 16384;
        pActwk->xposi.l += work->x_velocity;
        pActwk->yposi.l += work->y_velocity;
        actionsub(pActwk);

        if (actwk[0].yposi.w.h - pActwk->yposi.w.h <= -224)
            frameout(pActwk);
    } else {
        tbl[pActwk->r_no0 / 2](pActwk);
        actionsub(pActwk);
        frameout_s(pActwk);
    }
}

static void m_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->patno = 0;
    pActwk->sprpri = 3;
    pActwk->sprhs = 16;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 16;
    pActwk->sproffset = 17202;
    pActwk->patbase = pat_iwa5;

    m_wait(pActwk);
}

static void m_wait(sprite_status *pActwk) {
    sprite_status *pPlayerwk;
    Sint16 d0;
    Sint16 carry_flag;

    pPlayerwk = &actwk[0];
    if (pActwk->userflag.b.h) {

        d0 = pPlayerwk->yposi.w.h;
        d0 -= pActwk->yposi.w.h;
        d0 -= 8;
        if ((Uint16)d0 < 88) {
            carry_flag = 1;
            d0 = pPlayerwk->xposi.w.h;
            d0 -= pActwk->xposi.w.h;
            d0 += 8;
            if ((Uint16)d0 < 16) {
                carry_flag = 1;
            } else {
                carry_flag = 0;
            }
        } else {
            carry_flag = 0;
        }
        if (carry_flag) {
            pActwk->r_no0 += 2;
            pActwk->colino = 248;
        }
    }
    hitchk(pActwk, pPlayerwk);
}

static void m_fall(sprite_status *pActwk) {
    iwa5_work *work = iwa5_work_get(pActwk);
    sprite_status *pPlayerwk;
    sprite_status *pNewActwk;
    iwa5_work *new_work;
    Sint16 a6;
    Sint16 d0, d1;
    Uint8 d6;
    init_data tbl0[4] = {{-8, -8, -65536, -262144},
                         {8, -8, 65536, -262144},
                         {-8, 8, -131072, -196608},
                         {8, 8, 131072, -196608}};

    if (pActwk->colicnt == 0) {

        pActwk->yposi.l += work->y_velocity;
        work->y_velocity += 16384;
        if (emycol_d(pActwk) >= 0) {

            pPlayerwk = &actwk[0];
            d0 = pPlayerwk->yposi.w.h;
            d0 -= pActwk->yposi.w.h;
            if (d0 <= -224) {
                frameout(pActwk);
            }
            return;
        }
    }
    a6 = 0;
    d6 = 3;
    do {
        if (actwkchk(&pNewActwk) != 0)
            break;

        pNewActwk->sprpri = 3;
        pNewActwk->userflag.b.h = -1;
        pNewActwk->sprhs = 8;
        pNewActwk->sprhsize = 8;
        pNewActwk->sprvsize = 8;
        pNewActwk->actno = pActwk->actno;
        pNewActwk->xposi.w.h = pActwk->xposi.w.h;
        pNewActwk->yposi.w.h = pActwk->yposi.w.h;
        pNewActwk->actflg = pActwk->actflg;
        pNewActwk->patbase = pActwk->patbase;
        pNewActwk->sproffset = pActwk->sproffset;
        d0 = tbl0[a6].dx;
        d1 = tbl0[a6].dy;
        pNewActwk->xposi.w.h += d0;
        pNewActwk->yposi.w.h += d1;
        new_work = iwa5_work_get(pNewActwk);
        new_work->x_velocity = tbl0[a6].sx;
        new_work->y_velocity = tbl0[a6].sy;
        ++a6;
        pNewActwk->patno = d6 + 1;
    } while (d6--);

    pActwk->actno = 24;
    pActwk->r_no0 = 0;
    pActwk->r_no1 = 1;
    pActwk->colino = 0;
    if (pActwk->actflg & 128) {
        soundset(158);
    }
}
