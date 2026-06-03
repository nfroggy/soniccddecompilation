#include "../equ.h"
#include "tonbo.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../suicide.h"
#include "playsub4.h"

#if defined(R41A)
#define SPRITE_TONBO_BASE 473
#elif defined(R41B)
#define SPRITE_TONBO_BASE 434
#elif defined(R41C)
#define SPRITE_TONBO_BASE 451
#elif defined(R42A)
#define SPRITE_TONBO_BASE 479
#elif defined(R42B)
#define SPRITE_TONBO_BASE 432
#elif defined(R43C) || defined(R43D)
#define SPRITE_TONBO_BASE 429
#else
#define SPRITE_TONBO_BASE 436
#endif

static void act_init(sprite_status *pActwk);
static void act_lr(sprite_status *pActwk);

static Uint8 pchg0[4] = {2, 0, 1, 255};
static Uint8 pchg1[4] = {4, 0, 2, 255};
static Uint8 *pchg[2] = {pchg0, pchg1};
static sprite_pattern spr_tonbo_e_00 = {1,
                                        {{-29, -17, 0, SPRITE_TONBO_BASE + 3}}};
static sprite_pattern spr_tonbo_e_01 = {1,
                                        {{-29, -13, 0, SPRITE_TONBO_BASE + 4}}};
static sprite_pattern spr_tonbo_e_02 = {1,
                                        {{-29, -17, 0, SPRITE_TONBO_BASE + 5}}};
static sprite_pattern spr_tonbo_b_00 = {1, {{-29, -17, 0, SPRITE_TONBO_BASE}}};
static sprite_pattern spr_tonbo_b_01 = {1,
                                        {{-29, -13, 0, SPRITE_TONBO_BASE + 1}}};
static sprite_pattern spr_tonbo_b_02 = {1,
                                        {{-29, -17, 0, SPRITE_TONBO_BASE + 2}}};
sprite_pattern *pat_tonbo_e[3] = {&spr_tonbo_e_00, &spr_tonbo_e_01,
                                  &spr_tonbo_e_02};
sprite_pattern *pat_tonbo_b[3] = {&spr_tonbo_b_00, &spr_tonbo_b_01,
                                  &spr_tonbo_b_02};

#pragma pack(push, 1)
typedef struct {
    Sint32 base_y;
    short_union angle;
    Sint32 x_velocity;
    Sint16 turn_step;
    Sint16 turn_timer;
    Sint16 turn_period;
    Uint8 unused[4];
    Sint16 origin_x;
} tonbo_work;
#pragma pack(pop)

static tonbo_work *get_work(sprite_status *pActwk) {
    return (tonbo_work *)pActwk->actfree;
}

void tonbo(sprite_status *pActwk) {
    Sint16 temp;
    static void (*act_tbl[2])(sprite_status *) = {&act_init, &act_lr};

    if (enemy_suicide(pActwk))
        return;
    act_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    temp = get_work(pActwk)->origin_x;
    frameout_s00(pActwk, temp);
}

static void act_init(sprite_status *pActwk) {
    tonbo_work *work = get_work(pActwk);

    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->colino = 44;
    pActwk->sprpri = 1;
    pActwk->sprhs = 28;
    pActwk->sprhsize = 28;
    pActwk->sprvsize = 16;
    pActwk->sproffset = 9238;
    work->origin_x = pActwk->xposi.w.h;
    work->base_y = pActwk->yposi.l;

    if (!pActwk->userflag.b.h) {
        pActwk->patbase = pat_tonbo_e;
        work->x_velocity = -65536;
        work->turn_step = 4;
        work->turn_period = 256;
    } else {
        pActwk->patbase = pat_tonbo_b;
        work->x_velocity = -32768;
        work->turn_step = 1;
        work->turn_period = 512;
    }
    work->turn_timer = work->turn_period;
    work->turn_timer >>= 1;
}

static void act_lr(sprite_status *pActwk) {
    tonbo_work *work = get_work(pActwk);
    Sint16 sin;
    Sint16 cos;
    int_union sinl;

    pActwk->xposi.l += work->x_velocity;
    sinset((Uint8)work->angle.b.l, &sin, &cos);
    sinl.l = 0;
    sinl.w.h = sin;

    sinl.l >>= 4;
    pActwk->yposi.l = work->base_y + sinl.l;

    work->angle.w += work->turn_step;

    --work->turn_timer;
    if (!work->turn_timer) {
        work->turn_timer = work->turn_period;
        work->x_velocity *= -1;
        pActwk->actflg ^= 1;
        pActwk->cddat ^= 1;
    }

    patchg(pActwk, pchg);
}
