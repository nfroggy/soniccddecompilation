#include "../equ.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../ridechk.h"

static void a_init(sprite_status *pActwk);
static void a_move(sprite_status *pActwk);
static void com(sprite_status *pActwk);

static sprite_pattern pat00 = {1, {{-32, -16, 0, 455}}};
sprite_pattern *pat_iwa5wave[1] = {&pat00};

#pragma pack(push, 1)
typedef struct {
    Sint16 parent_actor;
    Sint16 origin_x;
    Sint32 left_bound;
    Sint32 right_bound;
    Sint16 origin_y;
    Sint16 angle;
    Sint16 saved_xspeed;
} iwa5wave_work;
#pragma pack(pop)

static iwa5wave_work *get_work(sprite_status *pActwk) {
    return (iwa5wave_work *)pActwk->actfree;
}

void iwa5wave(sprite_status *pActwk) {
    if (pActwk->r_no0)
        a_move(pActwk);
    else
        a_init(pActwk);
}

static void a_init(sprite_status *pActwk) {
    sprite_status *pNewActwk;
    iwa5wave_work *work = get_work(pActwk);
    int_union ld0;
    Sint16 d3, d4, d5, d6;

    work->origin_x = pActwk->xposi.w.h;
    ld0.w.h = pActwk->xposi.w.h;
    ld0.w.l = 0;
    ld0.l -= 0x800000;
    work->left_bound = ld0.l;
    ld0.l += 0x1000000;
    work->right_bound = ld0.l;

    d6 = 1;
    d5 = pActwk->xposi.w.h;
    d5 += 64;
    d4 = 128;
    d3 = -32768;
    if (pActwk->userflag.b.h) {
        d3 = 0;
    }
    pNewActwk = pActwk;

    pNewActwk->actflg |= 4;
    pNewActwk->r_no0 = 2;
    pNewActwk->sprpri = 3;
    pNewActwk->sprhs = 32;
    pNewActwk->sprhsize = 32;
    pNewActwk->sprvsize = 16;
    pNewActwk->sproffset = 17543;
    pNewActwk->patbase = pat_iwa5wave;
    get_work(pNewActwk)->origin_y = pActwk->yposi.w.h;

    do {
        if (d6 != 1) {
            iwa5wave_work *new_work;

            if (actwkchk(&pNewActwk) != 0)
                return;

            new_work = get_work(pNewActwk);
            new_work->parent_actor = (Uint16)(pActwk - actwk);
            pNewActwk->actno = pActwk->actno;
            new_work->left_bound = work->left_bound;
            new_work->right_bound = work->right_bound;
            pNewActwk->yposi.w.h = pActwk->yposi.w.h;

            pNewActwk->actflg |= 4;
            pNewActwk->r_no0 = 2;
            pNewActwk->sprpri = 3;
            pNewActwk->sprhs = 32;
            pNewActwk->sprhsize = 32;
            pNewActwk->sprvsize = 16;
            pNewActwk->sproffset = 17543;
            pNewActwk->patbase = pat_iwa5wave;
            new_work->origin_y = pActwk->yposi.w.h;
        }
        pNewActwk->xposi.w.h = d5;
        d5 -= d4;
        get_work(pNewActwk)->angle = d3;
    } while (d6--);
}

static void a_move(sprite_status *pActwk) {
    sprite_status *pMainwk;
    iwa5wave_work *work = get_work(pActwk);
    int_union ld0, ld1;
    Sint32 d0, d1;
    Sint16 sd0, sd1;
    Uint8 kakudo;

    d0 = pActwk->xposi.l;
    d1 = d0;
    d1 -= 32768;
    if (d1 <= work->left_bound) {
        d0 = work->right_bound;
        d1 = d0;
    }
    pActwk->xposi.l = d1;
    d1 -= d0;
    d1 >>= 8;
    pActwk->xspeed.w = (Uint16)d1;

    work->angle += 256;
    sd0 = work->angle;
    sd0 >>= 8;
    kakudo = (Uint16)(sd0 & 255);
    sinset(kakudo, &sd0, &sd1);
    ld1.w.l = 0;
    ld1.w.h = sd1;
    ld1.l >>= 2;
    ld0.l = ld1.l;
    ld0.l >>= 1;
    ld0.l += ld1.l;
    ld0.l += 32767 + 1;
    sd0 = ld0.w.h;
    ld0.w.h = ld0.w.l;
    ld0.w.l = sd0;
    ld0.w.l += work->origin_y;
    pActwk->yposi.w.h = ld0.w.l;
    if (work->parent_actor) {
        pMainwk = &actwk[work->parent_actor];
        if (pMainwk->actno == 41) {
            com(pActwk);
            return;
        }

        frameout(pActwk);
        return;
    }

    com(pActwk);
    frameout_s00(pActwk, work->origin_x);
}

static void com(sprite_status *pActwk) {
    iwa5wave_work *work = get_work(pActwk);
    Uint8 d0;

    work->saved_xspeed = pActwk->xspeed.w;
    pActwk->xspeed.w = 0;
    pActwk->sprvsize = 18;
    if (ridechk(pActwk, &actwk[0]))
        d0 = 255;
    else
        d0 = 0;
    pActwk->xspeed.w = work->saved_xspeed;
    pActwk->sprvsize = 16;
    if (d0) {
        ridechk(pActwk, &actwk[0]);
    }

    actionsub(pActwk);
}
