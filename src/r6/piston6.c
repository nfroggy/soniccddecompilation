#include "../equ.h"
#include "piston6.h"
#include "../action.h"
#include "../actset.h"
#include "../ridechk.h"

#if defined(R61B) || defined(R62B)
#define SPRITE_PISTON6_BASE 464
#elif defined(R63C) || defined(R63D)
#define SPRITE_PISTON6_BASE 447
#else
#define SPRITE_PISTON6_BASE 470
#endif

static sprite_pattern nullpat = {1, {{0, 0, 0, 0}}};
static sprite_pattern piston6pat0 = {1, {{-32, -40, 0, SPRITE_PISTON6_BASE}}};
sprite_pattern *piston6pat[2] = {&piston6pat0, &nullpat};

void piston6(sprite_status *actionwk) {
    if (actionwk->userflag.b.h < 0) {
        piston6_side(actionwk);
        return;
    }
    switch (actionwk->r_no0) {
    case 0:
        piston6_init(actionwk);
    case 2:
        piston6_move(actionwk);
        break;
    }
    actionsub(actionwk);
    frameout_s(actionwk);
}

void piston6_ridechk(sprite_status *actionwk) { ridechk(actionwk, &actwk[0]); }

void piston6_init(sprite_status *actionwk) {
    sprite_status *a1;

    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;

    actionwk->sproffset = 832;
    actionwk->patbase = piston6pat;
    actionwk->sprvsize = 40;
    actionwk->sprhsize = 32;
    ((Sint16 *)actionwk)[29] = actionwk->xposi.w.h;
    ((Sint16 *)actionwk)[27] = actionwk->yposi.w.h;
    if (actwkchk(&a1) == 0)
        pis6_setdata(actionwk, a1, 32);
    if (actwkchk(&a1) == 0)
        pis6_setdata(actionwk, a1, -32);
}

void piston6_move(sprite_status *actionwk) {
    Sint16 d0;

    piston6_sub(actionwk);
    d0 = actionwk->actfree[17];
    d0 = -d0;
    d0 += ((Sint16 *)actionwk)[27];
    actionwk->yposi.w.h = d0;

    if (actionwk->actfree[18] != 0) {
        if (actionwk->actfree[16] == 0)
            goto label1;
    }

    if (actionwk->actfree[17] >= 33) {
        ride_on_clr(actionwk, &actwk[0]);
        return;
    }
label1:
    piston6_ridechk(actionwk);
}

void piston6_sub(sprite_status *actionwk) {
    if (actionwk->actfree[16] != 0) {
        if (--actionwk->actfree[16] != 0)
            return;
    }

    if (actionwk->actfree[18] != 0) {
        --actionwk->actfree[17];
        if ((char)actionwk->actfree[17] > 0)
            return;
        actionwk->actfree[17] = actionwk->actfree[18] = 0;

        actionwk->actfree[16] = 60;
    } else {

        actionwk->actfree[17] += 8;
        if (actionwk->actfree[17] < 80)
            return;
        actionwk->actfree[17] = 80;
        actionwk->actfree[18] = 1;
        actionwk->actfree[16] = 60;
    }
}

void pis6_setdata(sprite_status *actionwk, sprite_status *a1, Sint16 d0) {
    a1->actno = 32;
    ((Uint16 *)a1)[28] = actionwk - actwk;
    a1->userflag.b.h = -1;
    a1->xposi.w.h = actionwk->xposi.w.h + d0;
    a1->yposi.w.h = actionwk->yposi.w.h;
    a1->actflg |= 4;
    a1->patbase = piston6pat;
    a1->sprvsize = 40;

    a1->patno = 1;
}

void piston6_side(sprite_status *actionwk) {
    sprite_status *a1;

    a1 = &actwk[((Uint16 *)actionwk)[28]];
    if (a1->actno != 32) {
        frameout(actionwk);
        return;
    }
    actionwk->yposi.w.h = a1->yposi.w.h;

    hitchk(actionwk, &actwk[0]);
    actionsub(actionwk);
}
