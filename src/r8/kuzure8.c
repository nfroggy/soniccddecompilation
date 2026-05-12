#include "../equ.h"
#include "kuzure8.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../ridechk.h"

#if defined(R82)
#define SPRITE_KUZURE8_BASE 422
#elif defined(R83)
#define SPRITE_KUZURE8_BASE 423
#else
#define SPRITE_KUZURE8_BASE 444
#endif

static sprite_pattern pat00 = {1, {{-8, -16, 0, SPRITE_KUZURE8_BASE}}};
static sprite_pattern pat01 = {1, {{-16, -16, 0, SPRITE_KUZURE8_BASE + 1}}};
static sprite_pattern pat02 = {1, {{-24, -16, 0, SPRITE_KUZURE8_BASE + 2}}};
static sprite_pattern pat03 = {1, {{-32, -16, 0, SPRITE_KUZURE8_BASE + 3}}};
static sprite_pattern pat04 = {1, {{-40, -16, 0, SPRITE_KUZURE8_BASE + 4}}};
static sprite_pattern pat05 = {1, {{-48, -16, 0, SPRITE_KUZURE8_BASE + 5}}};
static sprite_pattern pat06 = {1, {{-56, -16, 0, SPRITE_KUZURE8_BASE + 6}}};
static sprite_pattern pat07 = {1, {{-64, -16, 0, SPRITE_KUZURE8_BASE + 7}}};
static sprite_pattern pat08 = {1, {{-8, -8, 0, SPRITE_KUZURE8_BASE + 8}}};
static sprite_pattern pat09 = {1, {{-8, -8, 0, SPRITE_KUZURE8_BASE + 9}}};
sprite_pattern *pat_kuzure_a[10] = {&pat00, &pat01, &pat02, &pat03, &pat04,
                                    &pat05, &pat06, &pat07, &pat08, &pat09};

void kuzure8(sprite_status *actionwk) {
    if (actionwk->userflag.b.l != 0) {
        parts(actionwk);
        return;
    }
    switch (actionwk->r_no0) {
    case 0:
        main_ini(actionwk);
        break;
    case 2:
        main_check(actionwk);
        break;
    case 4:
        main_wait(actionwk);
        break;
    case 6:
        main_break(actionwk);
        break;
    case 8:
        main_hitchk(actionwk);
        break;
    }
}

void main_ini(sprite_status *actionwk) {
    char d0, d1;

    actionwk->actflg |= 4;
    actionwk->patbase = pat_kuzure_a;
    actionwk->sprvsize = 17;
    if (stageno.b.l != 2)
        actionwk->sproffset = 17624;
    else
        actionwk->sproffset = 17274;
    d0 = actionwk->userflag.b.h;
    if (d0 < 0)
        d0 = -d0;

    d1 = d0;
    --d0;
    actionwk->patno = d0;
    d1 <<= 3;
    actionwk->sprhs = d1;
    actionwk->sprhsize = d1;
    d0 = 3;
    d1 = 2;
    if (actionwk->userflag.b.h < 0) {
        d0 = 1;
        d1 = 8;
    }
    actionwk->sprpri = d0;
    actionwk->r_no0 = d1;
}

void main_check(sprite_status *actionwk) {
    if (ridechk(actionwk, &actwk[0]) != 0) {
        actionwk->r_no0 += 2;
        actionwk->cdsts = 0;
        ((Sint16 *)actionwk)[23] = 8;
        if (actwk[0].xspeed.w < 0)
            ((Sint16 *)actionwk)[24] = -8;
        else
            ((Sint16 *)actionwk)[24] = 8;
        soundset(163);
    }
    actionsub(actionwk);
    frameout_s(actionwk);
}

void main_wait(sprite_status *actionwk) {
    Sint16 t;

    ridechk(actionwk, &actwk[0]);
    --((Sint16 *)actionwk)[23];
    t = ((Sint16 *)actionwk)[23];
    if (t < 0)
        actionwk->r_no0 += 2;
    actionsub(actionwk);
}

void main_break(sprite_status *actionwk) {
    Sint16 d1, d2, d3;
    char ride;
    sprite_status *a1;

    ride = 0;
    d1 = -1;
    d2 = 8;
    d3 = (actionwk->userflag.b.h - 1) << 3;
    if (((Sint16 *)actionwk)[24] >= 0)
        d3 = -d3;
    if (actwkchk(&a1) == 0) {
        a1->actno = actionwk->actno;
        a1->actflg = actionwk->actflg;
        a1->sproffset = actionwk->sproffset;
        a1->patbase = actionwk->patbase;
        a1->xposi.w.h = actionwk->xposi.w.h;
        a1->yposi.w.h = actionwk->yposi.w.h;

        a1->userflag.b.l = d1;
        a1->yposi.w.h -= d2;
        a1->xposi.w.h += d3;
        ((Sint16 *)a1)[23] = 4;
        a1->patno = 8;
        if (actwkchk(&a1) == 0) {
            a1->actno = actionwk->actno;
            a1->actflg = actionwk->actflg;
            a1->sproffset = actionwk->sproffset;
            a1->patbase = actionwk->patbase;
            a1->xposi.w.h = actionwk->xposi.w.h;
            a1->yposi.w.h = actionwk->yposi.w.h;

            a1->userflag.b.l = d1;
            a1->yposi.w.h += d2;
            a1->xposi.w.h += d3;
            ((Sint16 *)a1)[23] = 0;
            a1->patno = 9;
        }
    }

    if (ridechk(actionwk, &actwk[0]))
        ride_on_clr(actionwk, &actwk[0]);

    --actionwk->userflag.b.h;
    ride = actionwk->userflag.b.h;
    if (ride <= 0) {
        frameout(actionwk);
        return;
    }
    actionwk->r_no0 -= 2;
    --actionwk->patno;
    actionwk->sprhs -= 8;
    actionwk->sprhsize -= 8;
    ((Sint16 *)actionwk)[23] = 7;
    actionwk->xposi.w.h += ((Sint16 *)actionwk)[24];

    ridechk(actionwk, &actwk[0]);
    actionsub(actionwk);
}

void main_hitchk(sprite_status *actionwk) {
    hitchk(actionwk, &actwk[0]);
    actionsub(actionwk);
    frameout_s(actionwk);
}

void parts(sprite_status *actionwk) {
    switch (actionwk->r_no0) {
    case 0:
        parts_ini(actionwk);
        break;
    case 2:
        parts_wait(actionwk);
        break;
    case 4:
        parts_fall(actionwk);
        break;
    }
}

void parts_ini(sprite_status *actionwk) {
    actionwk->r_no0 += 2;
    actionwk->sprpri = 3;
    actionwk->sprhs = 8;
    actionwk->sprhsize = 8;
    actionwk->sprvsize = 8;
    actionsub(actionwk);
}

void parts_wait(sprite_status *actionwk) {
    Sint16 t;

    t = ((Sint16 *)actionwk)[23]--;
    if (t <= 0)
        actionwk->r_no0 += 2;
    actionsub(actionwk);
}

void parts_fall(sprite_status *actionwk) {
    Sint32 d0;

    if ((char)actionwk->actflg >= 0) {
        frameout(actionwk);
        return;
    }
    d0 = ((Sint32 *)actionwk)[12] + 16384;
    if (d0 > 1441792)
        d0 = 1441792;

    ((Sint32 *)actionwk)[12] = d0;
    actionwk->yposi.l += d0;
    actionsub(actionwk);
}
