#include "../equ.h"
#include "hachi6.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../suicide.h"

#if defined(R61B) || defined(R62B)
#define SPRITE_HACHI6_BASE 441
#else
#define SPRITE_HACHI6_BASE 447
#endif

static void act_init(sprite_status *actionwk);
static void act_move(sprite_status *actionwk);
static Sint16 act_check(sprite_status *actionwk, sprite_status *pw);

static sprite_pattern pat00 = {1, {{-24, -19, 0, SPRITE_HACHI6_BASE}}};
static sprite_pattern pat01 = {1, {{-24, -15, 0, SPRITE_HACHI6_BASE + 1}}};
static sprite_pattern pat02 = {1, {{-16, -23, 0, SPRITE_HACHI6_BASE + 2}}};
static sprite_pattern pat03 = {1, {{-16, -19, 0, SPRITE_HACHI6_BASE + 3}}};
static sprite_pattern pat04 = {1, {{-24, -19, 0, SPRITE_HACHI6_BASE + 4}}};
static sprite_pattern pat05 = {1, {{-24, -15, 0, SPRITE_HACHI6_BASE + 5}}};
static sprite_pattern pat06 = {1, {{-8, -8, 0, SPRITE_HACHI6_BASE + 6}}};
static sprite_pattern pat07 = {1, {{-8, -8, 0, SPRITE_HACHI6_BASE + 7}}};
static sprite_pattern pat08 = {1, {{-8, -8, 0, SPRITE_HACHI6_BASE + 8}}};
static sprite_pattern pat09 = {1, {{-8, -8, 0, SPRITE_HACHI6_BASE + 9}}};
sprite_pattern *pat_hachi6_e[4] = {&pat00, &pat01, &pat02, &pat03};
sprite_pattern *pat_hachi6_b[2] = {&pat04, &pat05};
sprite_pattern *pat_tama[4] = {&pat06, &pat07, &pat08, &pat09};
static char pchg00[4] = {1, 0, 1, -1};
static char pchg01[4] = {1, 2, 3, -1};
static char *pchg[2] = {pchg00, pchg01};
char pchg_tama0[4] = {1, 2, 3, -1};
char *pchg_tama[1] = {pchg_tama0};

void hachi6(sprite_status *actionwk) {
    void (*act_tbl[10])(sprite_status *) = {
        act_init, act_move, act_move1, act_rev,   act_rev1,
        act_rev2, act_shot, act_shot1, act_shot2, act_shot3};

    if (actionwk->userflag.b.h < 0) {
        tama(actionwk);
        return;
    }
    if (enemy_suicide(actionwk) == -1)
        return;
    act_tbl[actionwk->r_no0 / 2](actionwk);
    patchg(actionwk, (Uint8 **)pchg);
    actionsub(actionwk);
    frameout_s(actionwk);
}

static void act_init(sprite_status *actionwk) {
    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->sprpri = 1;
    actionwk->sprhs = 24;
    actionwk->sprhsize = 24;
    actionwk->sprvsize = 12;
    actionwk->sproffset = 42071;
    actionwk->colino = 49;
    ((Sint16 *)actionwk)[26] = -8;

    if (actionwk->userflag.b.h == 0) {
        actionwk->patbase = pat_hachi6_e;
        ((Sint32 *)actionwk)[12] = -65536;
    } else {
        actionwk->patbase = pat_hachi6_b;
        ((Sint32 *)actionwk)[12] = -32768;
    }

    act_move(actionwk);
}

static void act_move(sprite_status *actionwk) {
    actionwk->r_no0 += 2;
    if (actionwk->userflag.b.h == 0)
        ((Sint16 *)actionwk)[23] = 512;
    else
        ((Sint16 *)actionwk)[23] = 1024;
}

void act_move1(sprite_status *actionwk) {
    actionwk->xposi.l += ((Sint32 *)actionwk)[12];
    if (actionwk->userflag.b.h == 0) {
        if (((Sint16 *)actionwk)[27] != 0) {
            --((Sint16 *)actionwk)[27];
        } else {

            if (act_check(actionwk, &actwk[0])) {
                actionwk->r_no0 = 12;
                return;
            }
        }
    }

    if (--((Sint16 *)actionwk)[23] == 0)
        actionwk->r_no0 = 6;
}

static Sint16 act_check(sprite_status *actionwk, sprite_status *pw) {
    char d1;
    char d2;
    Sint16 d0w;

    d0w = pw->yposi.w.h;
    d0w -= actionwk->yposi.w.h;
    if (d0w < -96 || d0w > 96)
        return 0;
    if ((Sint16)(pw->xposi.w.h - actionwk->xposi.w.h) >= 0)
        d1 = -1;
    else
        d1 = 0;

    d0w = pw->xposi.w.h;
    d0w -= actionwk->xposi.w.h;
    if (d0w < -120 || d0w > 120)
        return 0;
    if (actionwk->actflg & 1)
        d2 = -1;
    else
        d2 = 0;
    d2 ^= d1;
    if (d2 != 0) {
        ((Sint32 *)actionwk)[12] = -((Sint32 *)actionwk)[12];
        ((Sint16 *)actionwk)[26] = -((Sint16 *)actionwk)[26];
        actionwk->actflg ^= 1;
        actionwk->cddat ^= 1;
    }
    return -1;
}

void act_rev(sprite_status *actionwk) {
    actionwk->r_no0 += 2;
    ((Sint16 *)actionwk)[23] = 30;
}

void act_rev1(sprite_status *actionwk) {
    if (--((Sint16 *)actionwk)[23] >= 0)
        return;
    actionwk->r_no0 += 2;
    ((Sint16 *)actionwk)[23] = 30;
    ((Sint32 *)actionwk)[12] = -((Sint32 *)actionwk)[12];
    ((Sint16 *)actionwk)[26] = -((Sint16 *)actionwk)[26];
    actionwk->actflg ^= 1;
    actionwk->cddat ^= 1;
}

void act_rev2(sprite_status *actionwk) {
    if (--((Sint16 *)actionwk)[23] >= 0)
        return;
    actionwk->r_no0 = 2;
}

void act_shot(sprite_status *actionwk) {
    actionwk->r_no0 += 2;
    ((Sint16 *)actionwk)[23] = 30;
    act_shot1(actionwk);
}

void act_shot1(sprite_status *actionwk) {
    if (--((Sint16 *)actionwk)[23] >= 0)
        return;
    actionwk->r_no0 += 2;
    ((Sint16 *)actionwk)[23] = 30;
    actionwk->mstno.b.h = 1;
    actionwk->colino = 50;

    actionwk->sprvsize = 16;
    actionwk->sprhs = 16;
    actionwk->sprhsize = 16;
    actionwk->xposi.w.h += ((Sint16 *)actionwk)[26];
    actionwk->yposi.w.h += 4;
}

void act_shot2(sprite_status *actionwk) {
    sprite_status *a1;

    if (--((Sint16 *)actionwk)[23] >= 0)
        return;
    actionwk->r_no0 += 2;
    ((Sint16 *)actionwk)[23] = 30;
    if (actwkchk(&a1) != 0)
        return;
    a1->actno = actionwk->actno;
    a1->userflag.b.h = -1;
    a1->actflg = actionwk->actflg;
    a1->sproffset = actionwk->sproffset;
    a1->patbase = pat_tama;
    a1->sprpri = 1;

    a1->sprvsize = 16;
    a1->sprhs = 16;
    a1->sprhsize = 16;
    a1->colino = 179;
    a1->yposi.w.h = actionwk->yposi.w.h + 23;
    ((Sint32 *)a1)[13] = 0x20000;

    a1->xposi.w.h = actionwk->xposi.w.h;
    if (actionwk->actflg & 1) {
        a1->xposi.w.h += 7;
        ((Sint32 *)a1)[12] = 0x20000;
    } else {
        a1->xposi.w.h -= 7;
        ((Sint32 *)a1)[12] = -0x20000;
    }
    if ((char)actionwk->actflg < 0)
        soundset(160);
}

void act_shot3(sprite_status *actionwk) {
    if (--((Sint16 *)actionwk)[23] >= 0)
        return;
    actionwk->r_no0 = 2;
    ((Sint16 *)actionwk)[27] = 60;
    actionwk->mstno.b.h = 0;
    actionwk->colino = 49;

    actionwk->sprvsize = 12;
    actionwk->sprhs = 24;
    actionwk->sprhsize = 24;
    actionwk->yposi.w.h -= 4;
}

void tama(sprite_status *actionwk) {
    switch (actionwk->r_no0) {

    case 0:
        tam_move(actionwk);
    case 2:
        tam_move1(actionwk);
        break;
    case 4:
        tam_move2(actionwk);
        break;
    case 6:
        tam_move3(actionwk);
        break;
    }
    actionsub(actionwk);
    frameout_s(actionwk);
}

void tam_move(sprite_status *actionwk) {
    actionwk->r_no0 += 2;
    ((Sint16 *)actionwk)[23] = 3;
}

void tam_move1(sprite_status *actionwk) {
    if (--((Sint16 *)actionwk)[23] > 0)
        return;
    actionwk->r_no0 += 2;
    actionwk->patno = 1;
    ((Sint16 *)actionwk)[23] = 10;
}

void tam_move2(sprite_status *actionwk) {
    actionwk->xposi.l += ((Sint32 *)actionwk)[12];
    actionwk->yposi.l += ((Sint32 *)actionwk)[13];
    if (--((Sint16 *)actionwk)[23] > 0)
        return;
    actionwk->r_no0 += 2;
}

void tam_move3(sprite_status *actionwk) {
    if ((char)actionwk->actflg >= 0) {
        frameout(actionwk);
        return;
    }
    actionwk->xposi.l += ((Sint32 *)actionwk)[12];
    actionwk->yposi.l += ((Sint32 *)actionwk)[13];
    patchg(actionwk, (Uint8 **)pchg_tama);
}
