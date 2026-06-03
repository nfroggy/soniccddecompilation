#include <stddef.h>

#include "equ.h"
#include "enemy.h"
#include "action.h"
#include "actset.h"
#include "dircol.h"
#include "dummy.h"
#include "etc.h"
#include "loader2.h"
#include "playsub.h"
#include "suicide.h"

#pragma pack(push, 1)
typedef struct {
    Sint32 x_velocity;
    Sint16 frameout_x;
} ari_work;

typedef struct {
    Sint16 origin_x;
    union {
        Sint32 sine_base_y;
        struct {
            Sint16 sine_low_word;
            Sint16 origin_y;
        };
    };
    Sint32 x_velocity;
    union {
        Sint16 angle;
        struct {
            Uint8 angle_low;
            Uint8 angle_high;
        };
    };
    Sint16 angle_step;
    Sint16 sine_shift;
} chou_work;

typedef struct {
    Sint16 origin_x;
    Sint32 x_velocity;
    Uint8 **change_table;
    Sint16 stop_timer;
} ka_work;

typedef struct {
    Sint16 origin_x;
    Sint32 x_velocity;
    Uint8 **change_table;
    Sint16 wait_timer;
} kamemusi_work;

typedef struct {
    Sint32 x_velocity;
    Sint32 y_velocity;
    Sint32 x_acceleration;
    Sint32 y_acceleration;
    Uint8 unused16[5];
    Uint8 direction;
} tama_work;

typedef struct {
    Sint16 origin_x;
    Sint16 origin_y;
    Uint8 unused4[2];
    Sint32 initial_y_velocity;
    Sint32 current_y_velocity;
    Sint32 gravity;
    Uint8 **change_table;
} tagame_work;
#pragma pack(pop)

_Static_assert(sizeof(Uint8 **) == 4,
               "enemy work animation table pointers are 32-bit");
_Static_assert(offsetof(ari_work, x_velocity) == 0,
               "ari_work.x_velocity offset");
_Static_assert(offsetof(ari_work, frameout_x) == 4,
               "ari_work.frameout_x offset");
_Static_assert(sizeof(ari_work) <= sizeof(((sprite_status *)0)->actfree),
               "ari_work fits in actfree");
_Static_assert(offsetof(chou_work, origin_x) == 0,
               "chou_work.origin_x offset");
_Static_assert(offsetof(chou_work, sine_low_word) == 2,
               "chou_work.sine_low_word offset");
_Static_assert(offsetof(chou_work, sine_base_y) == 2,
               "chou_work.sine_base_y offset");
_Static_assert(offsetof(chou_work, origin_y) == 4,
               "chou_work.origin_y offset");
_Static_assert(offsetof(chou_work, x_velocity) == 6,
               "chou_work.x_velocity offset");
_Static_assert(offsetof(chou_work, angle) == 10,
               "chou_work.angle offset");
_Static_assert(offsetof(chou_work, angle_high) == 11,
               "chou_work.angle_high offset");
_Static_assert(offsetof(chou_work, angle_step) == 12,
               "chou_work.angle_step offset");
_Static_assert(offsetof(chou_work, sine_shift) == 14,
               "chou_work.sine_shift offset");
_Static_assert(sizeof(chou_work) <= sizeof(((sprite_status *)0)->actfree),
               "chou_work fits in actfree");
_Static_assert(offsetof(ka_work, origin_x) == 0,
               "ka_work.origin_x offset");
_Static_assert(offsetof(ka_work, x_velocity) == 2,
               "ka_work.x_velocity offset");
_Static_assert(offsetof(ka_work, change_table) == 6,
               "ka_work.change_table offset");
_Static_assert(offsetof(ka_work, stop_timer) == 10,
               "ka_work.stop_timer offset");
_Static_assert(sizeof(ka_work) <= sizeof(((sprite_status *)0)->actfree),
               "ka_work fits in actfree");
_Static_assert(offsetof(kamemusi_work, origin_x) == 0,
               "kamemusi_work.origin_x offset");
_Static_assert(offsetof(kamemusi_work, x_velocity) == 2,
               "kamemusi_work.x_velocity offset");
_Static_assert(offsetof(kamemusi_work, change_table) == 6,
               "kamemusi_work.change_table offset");
_Static_assert(offsetof(kamemusi_work, wait_timer) == 10,
               "kamemusi_work.wait_timer offset");
_Static_assert(sizeof(kamemusi_work) <= sizeof(((sprite_status *)0)->actfree),
               "kamemusi_work fits in actfree");
_Static_assert(offsetof(tama_work, x_velocity) == 0,
               "tama_work.x_velocity offset");
_Static_assert(offsetof(tama_work, y_velocity) == 4,
               "tama_work.y_velocity offset");
_Static_assert(offsetof(tama_work, x_acceleration) == 8,
               "tama_work.x_acceleration offset");
_Static_assert(offsetof(tama_work, y_acceleration) == 12,
               "tama_work.y_acceleration offset");
_Static_assert(offsetof(tama_work, direction) == 21,
               "tama_work.direction offset");
_Static_assert(sizeof(tama_work) <= sizeof(((sprite_status *)0)->actfree),
               "tama_work fits in actfree");
_Static_assert(offsetof(tagame_work, origin_x) == 0,
               "tagame_work.origin_x offset");
_Static_assert(offsetof(tagame_work, origin_y) == 2,
               "tagame_work.origin_y offset");
_Static_assert(offsetof(tagame_work, initial_y_velocity) == 6,
               "tagame_work.initial_y_velocity offset");
_Static_assert(offsetof(tagame_work, current_y_velocity) == 10,
               "tagame_work.current_y_velocity offset");
_Static_assert(offsetof(tagame_work, gravity) == 14,
               "tagame_work.gravity offset");
_Static_assert(offsetof(tagame_work, change_table) == 18,
               "tagame_work.change_table offset");
_Static_assert(sizeof(tagame_work) <= sizeof(((sprite_status *)0)->actfree),
               "tagame_work fits in actfree");

static ari_work *ari_get_work(sprite_status *pActwk) {
    return (ari_work *)pActwk->actfree;
}

static chou_work *chou_get_work(sprite_status *pActwk) {
    return (chou_work *)pActwk->actfree;
}

static ka_work *ka_get_work(sprite_status *pActwk) {
    return (ka_work *)pActwk->actfree;
}

static kamemusi_work *kamemusi_get_work(sprite_status *pActwk) {
    return (kamemusi_work *)pActwk->actfree;
}

static tama_work *tama_get_work(sprite_status *pActwk) {
    return (tama_work *)pActwk->actfree;
}

static tagame_work *tagame_get_work(sprite_status *pActwk) {
    return (tagame_work *)pActwk->actfree;
}

static Uint8 ari_pchg00[4] = {3, 0, 1, 255};
static Uint8 ari_pchg01[4] = {7, 2, 3, 255};
Uint8 *ari_pchg[2] = {ari_pchg00, ari_pchg01};
static sprite_pattern ari0 = {
    3, {{-12, -19, 0, 370}, {-8, 4, 0, 371}, {14, -4, 0, 372}}};
static sprite_pattern ari1 = {
    3, {{-12, -20, 0, 373}, {-8, 4, 0, 374}, {18, -4, 0, 375}}};
static sprite_pattern ari2 = {
    3, {{-12, -19, 0, 376}, {-8, 4, 0, 377}, {14, -4, 0, 378}}};
static sprite_pattern ari3 = {
    3, {{-12, -20, 0, 379}, {-8, 4, 0, 380}, {18, -4, 0, 381}}};
sprite_pattern *ari_pat[4] = {&ari0, &ari1, &ari2, &ari3};
static Uint8 chou_pchg00[7] = {7, 0, 0, 1, 2, 1, 255};
static Uint8 chou_pchg01[7] = {3, 0, 0, 1, 2, 1, 255};
Uint8 *chou_pchg[2] = {chou_pchg00, chou_pchg01};
static sprite_pattern chou1e = {1, {{-16, -16, 0, 394}}};
static sprite_pattern chou2e = {1, {{-16, -8, 0, 395}}};
static sprite_pattern chou3e = {1, {{-16, -16, 0, 396}}};
static sprite_pattern chou1b = {1, {{-16, -16, 0, 397}}};
static sprite_pattern chou2b = {1, {{-16, -8, 0, 398}}};
static sprite_pattern chou3b = {1, {{-16, -16, 0, 399}}};
sprite_pattern *e_chou_pat[3] = {&chou1e, &chou2e, &chou3e};
sprite_pattern *b_chou_pat[3] = {&chou1b, &chou2b, &chou3b};
static Uint8 pchg_e_0[4] = {1, 0, 1, 255};
static Uint8 pchg_e_1[5] = {6, 2, 3, 4, 252};
Uint8 *pchg_e[2] = {pchg_e_0, pchg_e_1};
static Uint8 pchg_b_0[4] = {4, 0, 1, 255};
static Uint8 pchg_b_1[5] = {14, 2, 3, 4, 252};
Uint8 *pchg_b[2] = {pchg_b_0, pchg_b_1};
static sprite_pattern ka1e = {1, {{-16, -12, 0, 400}}};
static sprite_pattern ka2e = {1, {{-16, -4, 0, 401}}};
static sprite_pattern ka3e = {1, {{-16, -16, 0, 402}}};
static sprite_pattern ka4e = {1, {{-8, -16, 0, 403}}};
static sprite_pattern ka5e = {1, {{-12, -16, 0, 404}}};
sprite_pattern *e_ka_pat[5] = {&ka1e, &ka2e, &ka3e, &ka4e, &ka5e};
static sprite_pattern ka1b = {1, {{-16, -12, 0, 405}}};
static sprite_pattern ka2b = {1, {{-16, -4, 0, 406}}};
static sprite_pattern ka3b = {1, {{-16, -16, 0, 407}}};
static sprite_pattern ka4b = {1, {{-8, -16, 0, 408}}};
static sprite_pattern ka5b = {1, {{-12, -16, 0, 409}}};
sprite_pattern *b_ka_pat[5] = {&ka1b, &ka2b, &ka3b, &ka4b, &ka5b};
static Uint8 e_kamem_pchg00[4] = {29, 0, 1, 255};
static Uint8 e_kamem_pchg01[3] = {59, 2, 252};
Uint8 *e_kamem_pchg[2] = {e_kamem_pchg00, e_kamem_pchg01};
static Uint8 b_kamem_pchg00[18] = {5, 0, 1, 0, 1, 0, 1, 0, 1,
                                   0, 1, 0, 1, 0, 1, 0, 1, 252};
static Uint8 b_kamem_pchg01[3] = {11, 2, 252};
Uint8 *b_kamem_pchg[2] = {b_kamem_pchg00, b_kamem_pchg01};
static sprite_pattern kamem_pat00e = {1, {{-16, -15, 0, 410}}};
static sprite_pattern kamem_pat00b = {1, {{-16, -16, 0, 411}}};
static sprite_pattern kamem_pat01 = {1, {{-16, -14, 0, 412}}};
static sprite_pattern kamem_pat02 = {1, {{-17, -16, 0, 413}}};
sprite_pattern *e_kamem_pat[3] = {&kamem_pat00e, &kamem_pat01, &kamem_pat02};
sprite_pattern *b_kamem_pat[3] = {&kamem_pat00b, &kamem_pat01, &kamem_pat02};
static Uint8 tama_pchg00[6] = {2, 0, 1, 2, 3, 255};
Uint8 *tama_pchg[1] = {tama_pchg00};
static sprite_pattern tama_pat00 = {1, {{-4, -4, 0, 414}}};
static sprite_pattern tama_pat01 = {1, {{-4, -4, 0, 415}}};
static sprite_pattern tama_pat02 = {1, {{-8, -8, 0, 416}}};
static sprite_pattern tama_pat03 = {1, {{-8, -8, 0, 417}}};
sprite_pattern *tama_pat[4] = {&tama_pat00, &tama_pat01, &tama_pat02,
                               &tama_pat03};
static Uint8 e_tagame_pchg00[3] = {89, 6, 252};
static Uint8 e_tagame_pchg01[20] = {7, 0, 0, 0, 1, 0, 1, 2, 3, 5,
                                    5, 5, 5, 4, 5, 4, 5, 4, 5, 255};
Uint8 *e_tagame_pchg[2] = {e_tagame_pchg00, e_tagame_pchg01};
static Uint8 b_tagame_pchg00[3] = {89, 6, 252};
static Uint8 b_tagame_pchg01[25] = {4, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 3,  3,
                                    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 255};
Uint8 *b_tagame_pchg[2] = {b_tagame_pchg00, b_tagame_pchg01};
static sprite_pattern tagame_pat00 = {2,
                                      {{-16, -10, 0, 418}, {-16, -22, 0, 419}}};
static sprite_pattern tagame_pat01 = {2,
                                      {{-16, -10, 0, 420}, {-16, -22, 0, 421}}};
static sprite_pattern tagame_pat02 = {1, {{-16, -16, 0, 422}}};
static sprite_pattern tagame_pat03e = {1, {{-16, -16, 0, 423}}};
static sprite_pattern tagame_pat04e = {2,
                                       {{-16, -22, 0, 424}, {-16, -2, 0, 425}}};
static sprite_pattern tagame_pat05e = {2,
                                       {{-16, -22, 0, 426}, {-16, -2, 0, 427}}};
static sprite_pattern tagame_pat03b = {1, {{-16, -16, 0, 428}}};
static sprite_pattern tagame_pat04b = {2,
                                       {{-16, -22, 0, 429}, {-16, -2, 0, 430}}};
static sprite_pattern tagame_pat05b = {2,
                                       {{-16, -22, 0, 431}, {-16, -2, 0, 432}}};
static sprite_pattern tagame_pat06 = {1, {{-16, -16, 0, 433}}};
sprite_pattern *e_tagame_pat[7] = {
    &tagame_pat00,  &tagame_pat01,  &tagame_pat02, &tagame_pat03e,
    &tagame_pat04e, &tagame_pat05e, &tagame_pat06};
sprite_pattern *b_tagame_pat[7] = {
    &tagame_pat00,  &tagame_pat01,  &tagame_pat02, &tagame_pat03b,
    &tagame_pat04b, &tagame_pat05b, &tagame_pat06};

void ene_ari(sprite_status *pActwk) {
    void (*tbl[3])(sprite_status *) = {&ari_init, &ari_fall, &ari_move};

    if (enemy_suicide(pActwk))
        return;
    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s00(pActwk, ari_get_work(pActwk)->frameout_x);
}

void ari_init(sprite_status *pActwk) {
    Sint16 c;

    pActwk->actflg |= 4;
    pActwk->sprpri = 4;
    pActwk->patbase = ari_pat;
    pActwk->sprhs = 24;
    pActwk->sprhsize = 24;
    pActwk->sprvsize = 19;
    pActwk->colino = 41;
    ari_get_work(pActwk)->frameout_x = pActwk->xposi.w.h;

    if (pActwk->userflag.b.h == 0) {
        ari_get_work(pActwk)->x_velocity = -65536;
        pActwk->mstno.b.h = 0;
    } else {
        ari_get_work(pActwk)->x_velocity = -32768;
        pActwk->mstno.b.h = 1;
    }

    pActwk->yposi.l += 65536;
    c = emycol_d(pActwk);
    if (c < 0) {
        pActwk->r_no0 += 2;
    }
}

void ari_fall(sprite_status *pActwk) {
    Sint16 c;

    pActwk->yposi.l += 65536;
    c = emycol_d(pActwk);
    if (c < 0) {
        pActwk->r_no0 += 2;
    }
}

void ari_move(sprite_status *pActwk) {
    Sint16 xpos;
    Sint16 xpos_m;
    Sint16 c;
    Sint32 spd_x;

    do {
        spd_x = ari_get_work(pActwk)->x_velocity;
        pActwk->xposi.l = pActwk->xposi.l + spd_x;
        xpos = pActwk->xposi.w.h;
        xpos_m = ari_get_work(pActwk)->frameout_x;
        xpos = xpos - xpos_m;
        if (xpos < 0)
            xpos = -xpos;
        if (xpos < 128) {
            c = emycol_d(pActwk);
            if (c >= -7) {
                if (c < 8) {
                    pActwk->yposi.w.h = pActwk->yposi.w.h + c;
                    patchg(pActwk, ari_pchg);
                    break;
                }
            }
        }
        spd_x = ari_get_work(pActwk)->x_velocity;
        ari_get_work(pActwk)->x_velocity = -spd_x;
        pActwk->actflg = pActwk->actflg ^ 1;
        pActwk->cddat = pActwk->cddat ^ 1;
    } while (1);
}

void ene_chou(sprite_status *pActwk) {
    if (enemy_suicide(pActwk))
        return;
    if (pActwk->r_no0 == 0)
        chou_init(pActwk);
    else
        chou_move(pActwk);
}

void chou_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->colino = 42;
    pActwk->sprhs = 16;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 16;
    chou_get_work(pActwk)->origin_x = pActwk->xposi.w.h;
    chou_get_work(pActwk)->origin_y = pActwk->yposi.w.h;
    chou_get_work(pActwk)->sine_low_word = -32768;
    if (pActwk->userflag.b.h == 0) {
        chou_get_work(pActwk)->x_velocity = -32768;
        chou_get_work(pActwk)->angle_step = -512;
        chou_get_work(pActwk)->sine_shift = 3;
        pActwk->mstno.b.h = 0;
        pActwk->patbase = e_chou_pat;
    } else {
        chou_get_work(pActwk)->x_velocity = -16384;
        chou_get_work(pActwk)->angle_step = -256;
        chou_get_work(pActwk)->sine_shift = 4;
        pActwk->mstno.b.h = 1;
        pActwk->patbase = b_chou_pat;
    }
}

void chou_move(sprite_status *pActwk) {
    Sint16 xpos;
    Sint16 shift;
    Sint16 sSin, sCos;
    int_union lSin;
    chou_work *pWork = chou_get_work(pActwk);

    pActwk->xposi.l += pWork->x_velocity;
    xpos = pActwk->xposi.w.h;
    xpos -= pWork->origin_x;
    if (xpos < 0)
        xpos *= -1;
    if (xpos >= 128) {
        pWork->x_velocity *= -1;
        pActwk->xposi.l += pWork->x_velocity;
        pActwk->actflg = pActwk->actflg ^ 1;
        pActwk->cddat = pActwk->cddat ^ 1;
        pWork->angle = 0;
    }
    pWork->angle += pWork->angle_step;
    sinset(pWork->angle_high, &sSin, &sCos);
    lSin.w.h = sSin;
    lSin.w.l = 0;
    shift = pWork->sine_shift;
    while (shift--)
        lSin.l /= 2;
    lSin.l += pWork->sine_base_y;
    pActwk->yposi.l = lSin.l;

    patchg(pActwk, chou_pchg);
    actionsub(pActwk);
    frameout_s00(pActwk, pWork->origin_x);
}

void ene_ka(sprite_status *pActwk) {
    void (*tbl[5])(sprite_status *) = {&ka_init, &ka_move, &ka_turn, &ka_down,
                                       &ka_stop};

    if (enemy_suicide(pActwk))
        return;
    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s00(pActwk, ka_get_work(pActwk)->origin_x);
}

void ka_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->sprhs = 16;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 16;
    pActwk->colino = 43;
    ka_get_work(pActwk)->origin_x = pActwk->xposi.w.h;

    if (pActwk->userflag.b.h == 0) {
        pActwk->patbase = e_ka_pat;
        ka_get_work(pActwk)->change_table = pchg_e;
        ka_get_work(pActwk)->x_velocity = -65536;
    } else {
        pActwk->patbase = b_ka_pat;
        ka_get_work(pActwk)->change_table = pchg_b;
        ka_get_work(pActwk)->x_velocity = -32768;
    }
    ka_move(pActwk);
}

void ka_move(sprite_status *pActwk) {
    sprite_status *pPlayerwk;
    Sint16 d0;

    if (editmode.w == 0) {
        pPlayerwk = &actwk[0];
        d0 = pPlayerwk->yposi.w.h;
        d0 -= pActwk->yposi.w.h;
        d0 += 48;
        if ((Uint16)d0 < 112) {
            d0 = pPlayerwk->xposi.w.h;
            d0 -= pActwk->xposi.w.h;
            d0 += 48;
            if ((Uint16)d0 < 96) {
                pActwk->r_no0 += 2;
                pActwk->mstno.b.h = 1;
                return;
            }
        }
    }
    do {
        pActwk->xposi.l += ka_get_work(pActwk)->x_velocity;
        d0 = pActwk->xposi.w.h;
        d0 -= ka_get_work(pActwk)->origin_x;
        if (d0 < 0)
            d0 *= -1;
        if (d0 < 128)
            break;
        ka_get_work(pActwk)->x_velocity *= -1;
        pActwk->actflg = pActwk->actflg ^ 1;
        pActwk->cddat = pActwk->cddat ^ 1;
    } while (1);
    patchg(pActwk, ka_get_work(pActwk)->change_table);
}

void ka_turn(sprite_status *pActwk) { patchg(pActwk, ka_get_work(pActwk)->change_table); }

void ka_down(sprite_status *pActwk) {
    Sint16 c;

    pActwk->yposi.w.h += 6;
    c = emycol_d(pActwk);
    if (c < -7) {
        c += 8;
        pActwk->yposi.w.h += c;
        pActwk->r_no0 += 2;
        if ((char)pActwk->actflg < 0)
            soundset(167);
    }
}

void ka_stop(sprite_status *pActwk) {
    if ((char)pActwk->actflg < 0)
        return;
    frameout_s0(pActwk);
}

void ene_kamemusi(sprite_status *pActwk) {
    void (*tbl[6])(sprite_status *) = {&kamemusi_init,  &kamemusi_fall,
                                       &kamemusi_move,  &kamemusi_stop,
                                       &kamemusi_stop1, &kamemusi_tama};

    if (pActwk->userflag.b.h == 1) {
        tama(pActwk);
        return;
    }

    if (enemy_suicide(pActwk))
        return;
    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s00(pActwk, kamemusi_get_work(pActwk)->origin_x);
}

void kamemusi_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 4;
    pActwk->colino = 44;
    pActwk->sprhs = 16;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 15;
    kamemusi_get_work(pActwk)->origin_x = pActwk->xposi.w.h;

    if (pActwk->userflag.b.h == 0) {
        pActwk->patbase = e_kamem_pat;
        kamemusi_get_work(pActwk)->change_table = e_kamem_pchg;
        kamemusi_get_work(pActwk)->x_velocity = -40960;
    } else {
        pActwk->patbase = b_kamem_pat;
        kamemusi_get_work(pActwk)->change_table = b_kamem_pchg;
        kamemusi_get_work(pActwk)->x_velocity = -20480;
    }
    kamemusi_fall(pActwk);
}

void kamemusi_fall(sprite_status *pActwk) {
    Sint16 d1;

    pActwk->yposi.l += 65536;
    d1 = emycol_d(pActwk);
    if (d1 < 0)
        pActwk->r_no0 += 2;
}

void kamemusi_move(sprite_status *pActwk) {
    Sint16 d0, d1;

    do {
        if (editmode.w == 0) {
            if (pActwk->userflag.b.h == 0) {
                if (kamemusi_get_work(pActwk)->wait_timer) {
                    --kamemusi_get_work(pActwk)->wait_timer;
                } else {
                    if (area(pActwk)) {
                        pActwk->r_no0 += 2;
                        break;
                    }
                }
            }
        }
        pActwk->xposi.l += kamemusi_get_work(pActwk)->x_velocity;
        d0 = pActwk->xposi.w.h;
        d0 -= kamemusi_get_work(pActwk)->origin_x;
        if (d0 < 0)
            d0 *= -1;
        if (d0 < 128) {
            d1 = emycol_d(pActwk);
            if (d1 >= -7 && d1 < 8) {
                pActwk->yposi.w.h += d1;
                patchg(pActwk, kamemusi_get_work(pActwk)->change_table);
                break;
            }
        }
        kamemusi_get_work(pActwk)->x_velocity *= -1;
        pActwk->actflg ^= 1;
        pActwk->cddat ^= 1;
    } while (1);
}

Sint16 area(sprite_status *pActwk) {
    sprite_status *pPlayerwk;
    Sint16 d0, d1;
    Sint16 carry_flag;

    pPlayerwk = &actwk[0];
    d0 = pPlayerwk->yposi.w.h;
    d0 -= pActwk->yposi.w.h;
    d0 += 80;
    if ((Uint16)d0 < 160) {
        carry_flag = 1;
        d0 = pPlayerwk->xposi.w.h;
        d0 -= pActwk->xposi.w.h;
        d1 = d0;
        d1 += 80;
        if ((Uint16)d1 < 160)
            carry_flag = 1;
        else
            carry_flag = 0;
    } else {
        carry_flag = 0;
    }
    return carry_flag;
}

void kamemusi_stop(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->mstno.b.h = 1;

    kamemusi_stop1(pActwk);
}

void kamemusi_stop1(sprite_status *pActwk) {
    patchg(pActwk, kamemusi_get_work(pActwk)->change_table);
}

void kamemusi_tama(sprite_status *pActwk) {
    sprite_status *pTama1wk;
    sprite_status *pTama2wk;

    pActwk->r_no0 = 4;
    pActwk->mstno.b.h = 0;
    kamemusi_get_work(pActwk)->wait_timer = 120;

    if (pActwk->userflag.b.h == 0) {
        if (actwkchk(&pTama1wk) == 0) {
            if (pActwk->actflg & 128) {
                soundset(160);
            }
            kamemusi_tama_init(pActwk, pTama1wk);
            tama_get_work(pTama1wk)->direction = 0;
            if (actwkchk(&pTama2wk) == 0) {
                kamemusi_tama_init(pActwk, pTama2wk);
                tama_get_work(pTama2wk)->direction = 255;
            }
        }
    }
}

void kamemusi_tama_init(sprite_status *pActwk, sprite_status *pTamawk) {
    pTamawk->actno = pActwk->actno;
    pTamawk->userflag.b.h = 1;
    pTamawk->sproffset = pActwk->sproffset;
    pTamawk->sprpri = pActwk->sprpri + 1;
    pTamawk->xposi.w.h = pActwk->xposi.w.h;
    pTamawk->yposi.w.h = pActwk->yposi.w.h - 10;

    pTamawk->actflg |= 128;
}

void tama(sprite_status *pActwk) {
    void (*tbl[2])(sprite_status *) = {&tama_init, &tama_move};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
}

void tama_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->actflg |= 128;
    pActwk->colino = 173;
    pActwk->sprhs = 8;
    pActwk->sprhsize = 8;
    pActwk->sprvsize = 8;
    pActwk->patbase = tama_pat;
    tama_get_work(pActwk)->x_acceleration = 0;
    tama_get_work(pActwk)->y_acceleration = 8192;

    if (tama_get_work(pActwk)->direction == 0) {
        tama_get_work(pActwk)->x_velocity = 0x20000;
        tama_get_work(pActwk)->y_velocity = -0x40000;
    } else {
        tama_get_work(pActwk)->x_velocity = -0x20000;
        tama_get_work(pActwk)->y_velocity = -0x40000;
    }
}

void tama_move(sprite_status *pActwk) {
    Sint16 d1;

    if (!(pActwk->actflg & 128)) {
        frameout(pActwk);
        return;
    }
    d1 = emycol_d(pActwk);
    if (d1 < 0) {
        frameout(pActwk);
        return;
    }

    pActwk->xposi.l += tama_get_work(pActwk)->x_velocity;
    pActwk->yposi.l += tama_get_work(pActwk)->y_velocity;
    tama_get_work(pActwk)->x_velocity += tama_get_work(pActwk)->x_acceleration;
    tama_get_work(pActwk)->y_velocity += tama_get_work(pActwk)->y_acceleration;

    patchg(pActwk, tama_pchg);
}

void ene_tagame_a(sprite_status *pActwk) {
    void (*tbl[5])(sprite_status *) = {
        &tagame_init, &tagame_wait, &tagame_wait1, &tagame_jump, &tagame_jump1};

    if (enemy_suicide(pActwk))
        return;
    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s00(pActwk, tagame_get_work(pActwk)->origin_x);
}

void tagame_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->colino = 173;
    pActwk->sprhs = 16;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 22;
    tagame_get_work(pActwk)->origin_x = pActwk->xposi.w.h;
    tagame_get_work(pActwk)->origin_y = pActwk->yposi.w.h;

    if (pActwk->userflag.b.h == 0) {
        pActwk->patbase = e_tagame_pat;
        tagame_get_work(pActwk)->change_table = e_tagame_pchg;
        tagame_get_work(pActwk)->initial_y_velocity = -245760;
        tagame_get_work(pActwk)->gravity = 4096;
    } else {
        pActwk->patbase = b_tagame_pat;
        tagame_get_work(pActwk)->change_table = b_tagame_pchg;
        tagame_get_work(pActwk)->initial_y_velocity = -196608;
        tagame_get_work(pActwk)->gravity = 4096;
    }
    tagame_wait(pActwk);
}

void tagame_wait(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->mstno.w = 255;
    pActwk->colino = 0;
    pActwk->yposi.w.h = tagame_get_work(pActwk)->origin_y;
}

void tagame_wait1(sprite_status *pActwk) {
    patchg(pActwk, tagame_get_work(pActwk)->change_table);
}

void tagame_jump(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->mstno.w = 511;
    pActwk->colino = 46;
    pActwk->yposi.w.h = tagame_get_work(pActwk)->origin_y;
    tagame_get_work(pActwk)->current_y_velocity =
        tagame_get_work(pActwk)->initial_y_velocity;
    if ((char)pActwk->actflg < 0) {
        soundset(162);
    }
    tagame_jump1(pActwk);
}

void tagame_jump1(sprite_status *pActwk) {
    pActwk->yposi.l += tagame_get_work(pActwk)->current_y_velocity;
    tagame_get_work(pActwk)->current_y_velocity +=
        tagame_get_work(pActwk)->gravity;

    if (pActwk->yposi.w.h > tagame_get_work(pActwk)->origin_y) {
        pActwk->r_no0 = 2;
        if ((char)pActwk->actflg < 0) {
            soundset(162);
        }
    }
    patchg(pActwk, tagame_get_work(pActwk)->change_table);
}

void ene_tama(sprite_status *pActwk) {}
