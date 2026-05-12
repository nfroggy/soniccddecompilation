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
    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->sprpri = 1;
    actionwk->sprvsize = 16;

    actionwk->sprhs = 19;
    actionwk->sprhsize = 19;
    actionwk->sproffset = 42152;
    actionwk->colino = 54;

    actionwk->actfree[8] = actionwk->userflag.b.l;
    if (actionwk->userflag.b.h != 0)
        actionwk->patbase = pat_semi_b;
    else
        actionwk->patbase = pat_semi_e;
}

void act_wait(sprite_status *actionwk) {
    if (--((Sint16 *)actionwk)[27] > 0)
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
    ((Sint16 *)actionwk)[28] = pw->xposi.w.h - actionwk->xposi.w.h;

    d0w = pw->xposi.w.h;
    d0w -= actionwk->xposi.w.h;
    if (d0w >= -120 && d0w <= 120)
        return 1;
    return 0;
}

void act_movea(sprite_status *actionwk) {
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

    if (((Sint16 *)actionwk)[28] >= 0) {
        d0 = -d0;
    }
    *(Sint32 *)&actionwk->actfree[0] = d0;
    *(Sint32 *)&actionwk->actfree[4] = d1;
    ((Sint16 *)actionwk)[27] = d2;
}

void act_movea1(sprite_status *actionwk) {
    if (--((Sint16 *)actionwk)[27] <= 0)
        actionwk->r_no0 += 2;

    actionwk->xposi.l += *(Sint32 *)&actionwk->actfree[0];
    actionwk->yposi.l += *(Sint32 *)&actionwk->actfree[4];
    patchg(actionwk, (Uint8 **)pchg);
}

void act_moveb(sprite_status *actionwk) {
    Sint32 d0;

    actionwk->r_no0 += 2;
    ((Sint16 *)actionwk)[27] = 0;
    d0 = 65536;
    if (actionwk->userflag.b.h != 0)
        d0 = 49152;

    if (((Sint16 *)actionwk)[28] >= 0) {
        d0 = -d0;
    }
    *(Sint32 *)&actionwk->actfree[0] = d0;
}

void act_moveb1(sprite_status *actionwk) {
    sprite_status *a1;

    if (actionwk->userflag.b.h == 0) {
        ((Sint16 *)actionwk)[27] &= 63;
        if (((Sint16 *)actionwk)[27] == 0) {
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
        ++((Sint16 *)actionwk)[27];
    }

    actionwk->xposi.l += *(Sint32 *)&actionwk->actfree[0];
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
    actionwk->r_no0 += 2;
    actionwk->colino = 183;

    actionwk->sprvsize = 6;
    actionwk->sprhs = 6;
    actionwk->sprhsize = 6;
    actionwk->sproffset = 33992;
    actionwk->patbase = pat_bomb;
    *(Sint32 *)&actionwk->actfree[4] = 32768;
}

void bomb_fall(sprite_status *actionwk) {
    Sint16 d1;

    actionwk->yposi.l += *(Sint32 *)&actionwk->actfree[4];
    *(Sint32 *)&actionwk->actfree[4] += 16384;
    d1 = emycol_d(actionwk);
    if (d1 < 0) {
        actionwk->r_no0 += 2;
        actionwk->yposi.w.h += d1;
        ((Sint16 *)actionwk)[27] = 120;
    }
}

void bomb_wait(sprite_status *actionwk) {
    Sint16 t;

    --((Sint16 *)actionwk)[27];
    t = ((Sint16 *)actionwk)[27];
    if (t < 0) {
        actionwk->r_no0 += 2;
        ((Sint16 *)actionwk)[27] = 120;
    }
}

void bomb_blink(sprite_status *actionwk) {
    Sint16 t;

    --((Sint16 *)actionwk)[27];
    t = ((Sint16 *)actionwk)[27];
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
