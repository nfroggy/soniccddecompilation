#include "../equ.h"
#include "tagameb4.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../suicide.h"
#include "playsub4.h"

typedef struct {
    union {
        struct {
            Sint16 timer;
            Sint32 move_speed;
            Sint16 spike_indices[3];
            Sint16 timer_reset;
            Sint16 origin_x;
            Uint8 saved_cdsts;
            Sint16 parent_index;
        } master;
        struct {
            Sint32 x_speed;
            Sint32 y_speed;
            Sint32 x_accel;
            Sint32 y_accel;
            Uint8 saved_cdsts;
            Sint16 parent_index;
        } projectile;
    };
} tagameb4_work;

static tagameb4_work *tagameb4_get_work(sprite_status *pActwk) {
    return (tagameb4_work *)pActwk->actfree;
}

#if defined(R41A)
#define SPRITE_TAGAMEB4_BASE 485
#elif defined(R41B)
#define SPRITE_TAGAMEB4_BASE 446
#elif defined(R41C)
#define SPRITE_TAGAMEB4_BASE 463
#elif defined(R41D)
#define SPRITE_TAGAMEB4_BASE 453
#elif defined(R42A)
#define SPRITE_TAGAMEB4_BASE 491
#elif defined(R42B)
#define SPRITE_TAGAMEB4_BASE 444
#else
#define SPRITE_TAGAMEB4_BASE 448
#endif

static void tagameb0(sprite_status *pActwk);
static void a_init(sprite_status *pActwk);
static void make_toge(sprite_status *pActwk);
static void a_init_sub(Sint16 subact, sprite_status *pActwk);
static void a_move(sprite_status *pActwk);
static Sint32 a_check(sprite_status *pActwk);
static void a_stop(sprite_status *pActwk);
static void a_stop1(sprite_status *pActwk);
static void a_dash(sprite_status *pActwk);
static void tagameb1(sprite_status *pActwk);
static void b_init(sprite_status *pActwk);
static void b_follow(sprite_status *pActwk);
static void b_move(sprite_status *pActwk);
static void set_projectile_motion(sprite_status *pActwk, Sint32 x_speed,
                                  Sint32 y_speed);

static Uint8 pchg0[4] = {30, 0, 1, 255};
static Uint8 *pchg[1] = {pchg0};
static sprite_pattern spr_tagame_b_00 = {1,
                                         {{-20, -12, 0, SPRITE_TAGAMEB4_BASE}}};
static sprite_pattern spr_tagame_b_01 = {
    1, {{-20, -12, 0, SPRITE_TAGAMEB4_BASE + 1}}};
static sprite_pattern spr_tagame_e_00 = {
    1, {{-20, -12, 0, SPRITE_TAGAMEB4_BASE + 2}}};
static sprite_pattern spr_tagame_e_01 = {
    1, {{-20, -12, 0, SPRITE_TAGAMEB4_BASE + 3}}};
static sprite_pattern spr_tagame_99 = {1,
                                       {{0, 0, 0, SPRITE_TAGAMEB4_BASE + 4}}};
sprite_pattern *pat_tagameb_b[2] = {&spr_tagame_b_00, &spr_tagame_b_01};
sprite_pattern *pat_tagameb_e[2] = {&spr_tagame_e_00, &spr_tagame_e_01};
static sprite_pattern *pat[1] = {&spr_tagame_99};

void tagameb(sprite_status *pActwk) {
    if (pActwk->userflag.b.h >= 0)
        tagameb0(pActwk);
    else
        tagameb1(pActwk);
}

static void tagameb0(sprite_status *pActwk) {
    static void (*tbl[5])(sprite_status *) = {&a_init, &a_move, &a_stop,
                                              &a_stop1, &a_dash};

    if (enemy_suicide(pActwk))
        return;
    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s(pActwk);
}

static void a_init(sprite_status *pActwk) {
    tagameb4_work *pWork = tagameb4_get_work(pActwk);

    pWork->master.origin_x = pActwk->xposi.w.h;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->sproffset = 9168;
    pActwk->sprhs = 20;
    pActwk->sprhsize = 20;
    pActwk->sprvsize = 8;
    pActwk->colino = 45;
    pActwk->r_no0 += 2;

    if (!pActwk->userflag.b.h) {
        pActwk->patbase = pat_tagameb_e;
        pWork->master.move_speed = -65536;
        pWork->master.timer_reset = 200;
    } else {
        pActwk->patbase = pat_tagameb_b;
        pWork->master.move_speed = -32768;
        pWork->master.timer_reset = 400;
        return;
    }
    make_toge(pActwk);
}

static void make_toge(sprite_status *pActwk) {
    sprite_status *subActwk;

    if (actwkchk(&subActwk) == 0) {
        subActwk->actno = pActwk->actno;
        subActwk->userflag.b.h = -1;
        tagameb4_get_work(subActwk)->projectile.saved_cdsts = pActwk->cdsts;
        tagameb4_get_work(subActwk)->projectile.parent_index =
            (Sint16)(pActwk - actwk);
        tagameb4_get_work(pActwk)->master.spike_indices[0] =
            (Sint16)(subActwk - actwk);
    } else {
        frameout(pActwk);
        return;
    }
    if (actwkchk(&subActwk) == 0) {
        subActwk->actno = pActwk->actno;
        subActwk->userflag.b.h = -1;
        tagameb4_get_work(subActwk)->projectile.saved_cdsts = pActwk->cdsts;
        tagameb4_get_work(subActwk)->projectile.parent_index =
            (Sint16)(pActwk - actwk);
        tagameb4_get_work(pActwk)->master.spike_indices[1] =
            (Sint16)(subActwk - actwk);
    } else {
        frameout(pActwk);
        return;
    }
    if (actwkchk(&subActwk) == 0) {
        subActwk->actno = pActwk->actno;
        subActwk->userflag.b.h = -1;
        tagameb4_get_work(subActwk)->projectile.saved_cdsts = pActwk->cdsts;
        tagameb4_get_work(subActwk)->projectile.parent_index =
            (Sint16)(pActwk - actwk);
        tagameb4_get_work(pActwk)->master.spike_indices[2] =
            (Sint16)(subActwk - actwk);
    } else {
        frameout(pActwk);
        return;
    }

    a_init_sub(tagameb4_get_work(pActwk)->master.spike_indices[0], pActwk);
    actwk[tagameb4_get_work(pActwk)->master.spike_indices[0]].xposi.w.h += 2;
    actwk[tagameb4_get_work(pActwk)->master.spike_indices[0]].yposi.w.h -= 10;
    a_init_sub(tagameb4_get_work(pActwk)->master.spike_indices[1], pActwk);
    actwk[tagameb4_get_work(pActwk)->master.spike_indices[1]].xposi.w.h -= 3;
    actwk[tagameb4_get_work(pActwk)->master.spike_indices[1]].yposi.w.h -= 10;
    a_init_sub(tagameb4_get_work(pActwk)->master.spike_indices[2], pActwk);
    actwk[tagameb4_get_work(pActwk)->master.spike_indices[2]].xposi.w.h -= 7;
    actwk[tagameb4_get_work(pActwk)->master.spike_indices[2]].yposi.w.h -= 9;
}

static void a_init_sub(Sint16 subact, sprite_status *pActwk) {
    actwk[subact].xposi.w.h = pActwk->xposi.w.h;
    actwk[subact].yposi.w.h = pActwk->yposi.w.h;
    actwk[subact].actflg |= 4;
    actwk[subact].sproffset = 9168;
    actwk[subact].sprpri = 3;
    actwk[subact].sprhs = 2;
    actwk[subact].sprhsize = 2;
    actwk[subact].sprvsize = 2;
}

static void a_move(sprite_status *pActwk) {
    tagameb4_work *pWork = tagameb4_get_work(pActwk);

    if (!pActwk->userflag.b.h) {
        if (a_check(pActwk) != 0) {

            pActwk->r_no0 += 2;
            pWork->master.timer = 60;
            return;
        }
    }

    pActwk->xposi.l += pWork->master.move_speed;

    if (!pActwk->userflag.b.h) {
        actwk[pWork->master.spike_indices[0]].xposi.l +=
            pWork->master.move_speed;
        actwk[pWork->master.spike_indices[1]].xposi.l +=
            pWork->master.move_speed;
        actwk[pWork->master.spike_indices[2]].xposi.l +=
            pWork->master.move_speed;
    }

    --pWork->master.timer;
    if (pWork->master.timer < 0) {
        pWork->master.timer = pWork->master.timer_reset;
        pWork->master.move_speed *= -1;
        pActwk->actflg ^= 1;
        pActwk->cddat ^= 1;

        if (!pActwk->userflag.b.h) {
            actwk[pWork->master.spike_indices[0]].xposi.w.h =
                pActwk->xposi.w.h;
            actwk[pWork->master.spike_indices[1]].xposi.w.h =
                pActwk->xposi.w.h;
            actwk[pWork->master.spike_indices[2]].xposi.w.h =
                pActwk->xposi.w.h;
            if (!(pActwk->actflg & 1)) {
                actwk[pWork->master.spike_indices[0]].xposi.w.h -= 2;
                actwk[pWork->master.spike_indices[1]].xposi.w.h += 3;
                actwk[pWork->master.spike_indices[2]].xposi.w.h += 7;
            } else {
                actwk[pWork->master.spike_indices[0]].xposi.w.h -= 1;
                actwk[pWork->master.spike_indices[1]].xposi.w.h -= 6;
                actwk[pWork->master.spike_indices[2]].xposi.w.h -= 10;
            }
        }
    }

    patchg(pActwk, pchg);
}

static Sint32 a_check(sprite_status *pActwk) {
    Sint16 wD0;
    Sint32 ret;

    ret = 0;

    wD0 = actwk[0].yposi.w.h - pActwk->yposi.w.h - -56 - 56;
    if (wD0 < 0) {
        wD0 = actwk[0].xposi.w.h - pActwk->xposi.w.h;
        if (!(pActwk->actflg & 1))
            wD0 *= -1;
        wD0 -= 0x28 - 0x78;
        if (wD0 >= 0)
            ret = -1;
    }
    return ret;
}

static void a_stop(sprite_status *pActwk) {
    Sint16 subact;
    tagameb4_work *pWork = tagameb4_get_work(pActwk);

    --pWork->master.timer;
    if (pWork->master.timer < 0) {
        if (pActwk->actflg < 0)
            soundset(179);

        pWork->master.timer = 60;
        pActwk->r_no0 += 2;

        if (!pActwk->userflag.b.h) {
            subact = pWork->master.spike_indices[0];
            set_projectile_motion(&actwk[subact], -0x20000, -196608);
            actwk[subact].r_no0 += 2;
            subact = pWork->master.spike_indices[1];
            set_projectile_motion(&actwk[subact], 65536, -196608);
            actwk[subact].r_no0 += 2;
            subact = pWork->master.spike_indices[2];
            set_projectile_motion(&actwk[subact], 0x20000, -196608);
            actwk[subact].r_no0 += 2;
        }
    }
}

static void a_stop1(sprite_status *pActwk) {
    tagameb4_work *pWork = tagameb4_get_work(pActwk);

    --pWork->master.timer;
    if (pWork->master.timer < 0) {
        pActwk->r_no0 += 2;

        if (!pActwk->userflag.b.h)
            pWork->master.move_speed = 0x40000;
        else
            pWork->master.move_speed = 98304;

        if (!(pActwk->actflg & 1))
            pWork->master.move_speed *= -1;
    }
}

static void a_dash(sprite_status *pActwk) {
    pActwk->xposi.l += tagameb4_get_work(pActwk)->master.move_speed;
}

static void tagameb1(sprite_status *pActwk) {
    Sint16 subact;
    static void (*tbl[3])(sprite_status *) = {&b_init, &b_follow, &b_move};
    tagameb4_work *pWork = tagameb4_get_work(pActwk);

    subact = pWork->projectile.parent_index;
    if (actwk[subact].actno != 45) {
        frameout(pActwk);
        return;
    }
    if (pWork->projectile.saved_cdsts != actwk[subact].cdsts) {
        frameout(pActwk);
        return;
    }
    tbl[pActwk->r_no0 / 2](pActwk);
}

static void b_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->patbase = pat;
    b_follow(pActwk);
}

static void b_follow(sprite_status *pActwk) { actionsub(pActwk); }

static void set_projectile_motion(sprite_status *pActwk, Sint32 x_speed,
                                  Sint32 y_speed) {
    tagameb4_get_work(pActwk)->projectile.x_speed = x_speed;
    tagameb4_get_work(pActwk)->projectile.y_speed = y_speed;
    tagameb4_get_work(pActwk)->projectile.x_accel = 0;
    tagameb4_get_work(pActwk)->projectile.y_accel = 8192;
}

static void b_move(sprite_status *pActwk) {
    tagameb4_work *pWork = tagameb4_get_work(pActwk);

    pActwk->xposi.l += pWork->projectile.x_speed;
    pActwk->yposi.l += pWork->projectile.y_speed;
    pWork->projectile.x_speed += pWork->projectile.x_accel;
    pWork->projectile.y_speed += pWork->projectile.y_accel;

    if (pActwk->yposi.w.h - actwk[0].yposi.w.h > 224) {
        frameout(pActwk);
        return;
    }
    actionsub(pActwk);
}
