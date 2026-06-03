#include <stddef.h>

#include "../equ.h"
#include "semi.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../suicide.h"

#if defined(R61B) || defined(R62B)
#define SPRITE_SEMI_BASE 465
#else
#define SPRITE_SEMI_BASE 471
#endif

static void act_init(sprite_status *actionwk);
static Sint16 act_check(sprite_status *actionwk, sprite_status *pw);
static void bomb(sprite_status *actionwk);

#pragma pack(push, 1)
typedef struct {
    Sint32 x_speed;
    Sint32 y_speed;
    union {
        Sint16 timer;
        struct {
            Uint8 subtype;
            Uint8 timer_hi;
        };
    };
    Sint16 player_delta_x;
} semi_work;
#pragma pack(pop)

_Static_assert(offsetof(semi_work, x_speed) == 0,
               "semi_work.x_speed offset");
_Static_assert(offsetof(semi_work, y_speed) == 4,
               "semi_work.y_speed offset");
_Static_assert(offsetof(semi_work, timer) == 8,
               "semi_work.timer offset");
_Static_assert(offsetof(semi_work, subtype) == 8,
               "semi_work.subtype offset");
_Static_assert(offsetof(semi_work, player_delta_x) == 10,
               "semi_work.player_delta_x offset");
_Static_assert(sizeof(semi_work) <= sizeof(((sprite_status *)0)->actfree),
               "semi_work fits in actfree");

static semi_work *semi_get_work(sprite_status *actionwk) {
    return (semi_work *)actionwk->actfree;
}

static char pchg0[6] = {3, 1, 2, 3, 2, -1};
char pchg_bomb0[4] = {1, 0, 1, -1};
static char *pchg[1] = {pchg0};
char *pchg_bomb[1] = {pchg_bomb0};
static sprite_pattern pat00 = {1, {{-16, -16, 0, SPRITE_SEMI_BASE}}};
static sprite_pattern pat01 = {1, {{-16, -16, 0, SPRITE_SEMI_BASE + 1}}};
static sprite_pattern pat02 = {1, {{-20, -16, 0, SPRITE_SEMI_BASE + 2}}};
static sprite_pattern pat03 = {1, {{-16, -16, 0, SPRITE_SEMI_BASE + 3}}};
static sprite_pattern pat04 = {1, {{-16, -16, 0, SPRITE_SEMI_BASE + 4}}};
static sprite_pattern pat05 = {1, {{-16, -16, 0, SPRITE_SEMI_BASE + 5}}};
static sprite_pattern pat06 = {1, {{-20, -16, 0, SPRITE_SEMI_BASE + 6}}};
static sprite_pattern pat07 = {1, {{-16, -16, 0, SPRITE_SEMI_BASE + 7}}};
static sprite_pattern pat08 = {1, {{-8, -8, 0, SPRITE_SEMI_BASE + 8}}};
static sprite_pattern pat09 = {1, {{-8, -8, 0, SPRITE_SEMI_BASE + 9}}};
sprite_pattern *pat_semi_e[4] = {&pat00, &pat01, &pat02, &pat03};
sprite_pattern *pat_semi_b[4] = {&pat04, &pat05, &pat06, &pat07};
static sprite_pattern *pat_bomb[2] = {&pat08, &pat09};

void semi(sprite_status *actionwk) {
    if (actionwk->userflag.b.l < 0) {
        bomb(actionwk);
        return;
    }
    if (enemy_suicide(actionwk) == -1)
        return;
    switch (actionwk->r_no0) {

    case 0:
        act_init(actionwk);
    case 2:
        act_wait(actionwk);
        break;
    case 4:
        act_wait1(actionwk);
        break;
    case 6:
        act_movea(actionwk);
    case 8:
        act_movea1(actionwk);
        break;
    case 10:
        act_moveb(actionwk);
    case 12:
        act_moveb1(actionwk);
        break;
    }
    actionsub(actionwk);
    frameout_s(actionwk);
}

static void act_init(sprite_status *actionwk) {
    semi_work *work = semi_get_work(actionwk);

    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->sprpri = 1;
    actionwk->sprvsize = 16;

    actionwk->sprhs = 19;
    actionwk->sprhsize = 19;
    actionwk->sproffset = 42152;
    actionwk->colino = 54;

    work->subtype = actionwk->userflag.b.l;
    if (actionwk->userflag.b.h != 0)
        actionwk->patbase = pat_semi_b;
    else
        actionwk->patbase = pat_semi_e;
}

void act_wait(sprite_status *actionwk) {
    semi_work *work = semi_get_work(actionwk);

    if (--work->timer > 0)
        return;
    actionwk->r_no0 += 2;
}

void act_wait1(sprite_status *actionwk) {
    if (!act_check(actionwk, &actwk[0]))
        return;
    actionwk->r_no0 += 2;
}

static Sint16 act_check(sprite_status *actionwk, sprite_status *pw) {
    Sint16 d0w;

    d0w = pw->yposi.w.h;
    d0w -= actionwk->yposi.w.h;
    if (d0w < -96 || d0w > 96)
        return 0;
    semi_get_work(actionwk)->player_delta_x =
        pw->xposi.w.h - actionwk->xposi.w.h;

    d0w = pw->xposi.w.h;
    d0w -= actionwk->xposi.w.h;
    if (d0w >= -120 && d0w <= 120)
        return 1;
    return 0;
}

void act_movea(sprite_status *actionwk) {
    semi_work *work = semi_get_work(actionwk);
    Sint32 d0, d1;
    Sint16 d2;

    actionwk->r_no0 += 2;
    d0 = 65536;
    d1 = -32768;
    d2 = 96;
    if (actionwk->userflag.b.h != 0) {
        d0 = 49152;
        d1 = 24576;
        d2 = 42;
    }

    if (work->player_delta_x >= 0) {
        d0 = -d0;
    }
    work->x_speed = d0;
    work->y_speed = d1;
    work->timer = d2;
}

void act_movea1(sprite_status *actionwk) {
    semi_work *work = semi_get_work(actionwk);

    if (--work->timer <= 0)
        actionwk->r_no0 += 2;

    actionwk->xposi.l += work->x_speed;
    actionwk->yposi.l += work->y_speed;
    patchg(actionwk, (Uint8 **)pchg);
}

void act_moveb(sprite_status *actionwk) {
    semi_work *work = semi_get_work(actionwk);
    Sint32 d0;

    actionwk->r_no0 += 2;
    work->timer = 0;
    d0 = 65536;
    if (actionwk->userflag.b.h != 0)
        d0 = 49152;

    if (work->player_delta_x >= 0) {
        d0 = -d0;
    }
    work->x_speed = d0;
}

void act_moveb1(sprite_status *actionwk) {
    semi_work *work = semi_get_work(actionwk);
    sprite_status *a1;

    if (actionwk->userflag.b.h == 0) {
        work->timer &= 63;
        if (work->timer == 0) {
            if (act_check(actionwk, &actwk[0]) != 0) {
                if (actwkchk(&a1) == 0) {
                    a1->actno = actionwk->actno;
                    a1->xposi.l = actionwk->xposi.l;
                    a1->yposi.l = actionwk->yposi.l;
                    a1->yposi.w.h += 10;
                    a1->userflag.b.l = -1;
                    a1->actflg = actionwk->actflg;
                    a1->sprpri = actionwk->sprpri;
                    ++a1->sprpri;
                }
            }
        }
        ++work->timer;
    }

    actionwk->xposi.l += work->x_speed;
    patchg(actionwk, (Uint8 **)pchg);
}

static void bomb(sprite_status *actionwk) {
    switch (actionwk->r_no0) {
    case 0:
        bomb_init(actionwk);
    case 2:
        bomb_fall(actionwk);
        break;
    case 4:
        bomb_wait(actionwk);
        break;
    case 6:
        bomb_blink(actionwk);
        break;
    case 8:
        bomb_die(actionwk);
        break;
    }
    actionsub(actionwk);
    frameout_s(actionwk);
}

void bomb_init(sprite_status *actionwk) {
    semi_work *work = semi_get_work(actionwk);

    actionwk->r_no0 += 2;
    actionwk->colino = 183;

    actionwk->sprvsize = 6;
    actionwk->sprhs = 6;
    actionwk->sprhsize = 6;
    actionwk->sproffset = 33992;
    actionwk->patbase = pat_bomb;
    work->y_speed = 32768;
}

void bomb_fall(sprite_status *actionwk) {
    semi_work *work = semi_get_work(actionwk);
    Sint16 d1;

    actionwk->yposi.l += work->y_speed;
    work->y_speed += 16384;
    d1 = emycol_d(actionwk);
    if (d1 < 0) {
        actionwk->r_no0 += 2;
        actionwk->yposi.w.h += d1;
        work->timer = 120;
    }
}

void bomb_wait(sprite_status *actionwk) {
    semi_work *work = semi_get_work(actionwk);
    Sint16 t;

    --work->timer;
    t = work->timer;
    if (t < 0) {
        actionwk->r_no0 += 2;
        work->timer = 120;
    }
}

void bomb_blink(sprite_status *actionwk) {
    semi_work *work = semi_get_work(actionwk);
    Sint16 t;

    --work->timer;
    t = work->timer;
    if (t < 0) {
        actionwk->r_no0 += 2;
    }

    patchg(actionwk, (Uint8 **)pchg_bomb);
}

void bomb_die(sprite_status *actionwk) {
    actionwk->actno = 24;
    actionwk->r_no0 = 0;
    actionwk->r_no1 = 1;
    if ((char)actionwk->actflg < 0)
        soundset(158);
}
