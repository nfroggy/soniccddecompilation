#include "../equ.h"
#include "hasira5.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../ridechk.h"

static void m_init(sprite_status *pActwk);
static void m_wait(sprite_status *pActwk);
static void m_down(sprite_status *pActwk);

static sprite_pattern pat00 = {
    3, {{-16, -32, 0, 509}, {-16, 24, 0, 510}, {-16, -48, 0, 511}}};
static sprite_pattern pat01 = {1, {{-16, -12, 0, 512}}};
static sprite_pattern pat02 = {1, {{-8, -8, 0, 513}}};
static sprite_pattern pat03 = {1, {{-8, -8, 0, 514}}};
static sprite_pattern pat04 = {1, {{-8, -8, 0, 515}}};
static sprite_pattern pat05 = {1, {{-8, -8, 0, 516}}};
static sprite_pattern pat06 = {1, {{-16, -12, 0, 517}}};
static sprite_pattern pat07 = {1, {{-8, -8, 0, 518}}};
sprite_pattern *pat_hasira5[8] = {&pat00, &pat01, &pat02, &pat03,
                                  &pat04, &pat05, &pat06, &pat07};
static Uint8 tbl0patno[8] = {2, 5, 3, 5, 4, 5, 7, 5};
static Sint16 tbl0dposi[16] = {-8, -24, 16, 0,  0, 16, 16, 16,
                               0,  32,  16, 32, 0, 48, 16, 48};
static Sint32 tbl0speed[16] = {
    -77101,  -131072, 77101,  -131072, -163840, -98304, 163840, -98304,
    -327680, 32768,   327680, 32768,   -163840, 65536,  163840, 65536};

void hasira5(sprite_status *pActwk) {
    void (*tbl[3])(sprite_status *) = {&m_init, &m_wait, &m_down};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s(pActwk);
}

static void m_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 4;
    pActwk->sprhs = 16;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 48;
    pActwk->colino = 238;
    pActwk->sproffset = 17515;
    pActwk->patbase = pat_hasira5;

    m_wait(pActwk);
}

static void m_wait(sprite_status *pActwk) {
    Sint16 *a6;
    Sint32 *a5;
    Uint8 *a4;
    sprite_status *a1;
    sprite_status *pPlayerwk;
    Uint8 d6;

    pPlayerwk = &actwk[0];
    d6 = pActwk->colicnt;
    if (d6) {
        pActwk->colicnt = 0;
        pPlayerwk->xspeed.w >>= 1;
        pPlayerwk->mspeed.w >>= 1;
        if (hitchk(pActwk, pPlayerwk)) {
            ride_on_clr(pActwk, pPlayerwk);
        }
    } else {
        hitchk(pActwk, pPlayerwk);
        return;
    }

    soundset(176);
    pActwk->r_no0 += 2;
    pActwk->colino = 0;
    a6 = tbl0dposi;
    a5 = tbl0speed;
    a4 = tbl0patno;
    d6 = 7;
    a1 = pActwk;
    do {
        if (d6 != 7) {
            if (actwkchk(&a1) != 0)
                break;
            a1->actno = pActwk->actno;
            a1->r_no0 = pActwk->r_no0;
            a1->xposi.w.h = pActwk->xposi.w.h;
            a1->yposi.w.h = pActwk->yposi.w.h;
            a1->actflg = pActwk->actflg;
            a1->sprpri = pActwk->sprpri;
            a1->patbase = pActwk->patbase;
            a1->sproffset = pActwk->sproffset;
        }
        a1->sprhs = 16;
        a1->sprhsize = 16;
        a1->sprvsize = 12;
        a1->xposi.w.h += *a6++;
        a1->yposi.w.h += *a6++;
        *(Sint32 *)&a1->actfree[0] = *a5++;
        *(Sint32 *)&a1->actfree[4] = *a5++;
        a1->patno = *a4++;
    } while (d6--);

    m_down(pActwk);
}

static void m_down(sprite_status *pActwk) {
    sprite_status *pPlayerwk;
    Sint16 d0;

    *(Sint32 *)&pActwk->actfree[4] += 16384;
    pActwk->xposi.l += *(Sint32 *)&pActwk->actfree[0];
    pActwk->yposi.l += *(Sint32 *)&pActwk->actfree[4];
    pPlayerwk = &actwk[0];
    d0 = pPlayerwk->yposi.w.h;
    d0 -= pActwk->yposi.w.h;
    if (d0 > -224)
        actionsub(pActwk);
    else
        frameout(pActwk);
}
