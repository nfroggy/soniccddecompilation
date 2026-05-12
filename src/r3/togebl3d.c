#include "../equ.h"
#include "togebl3a.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"

#if defined(R33C) || defined(R33D)
#define SPRITE_TOGEBL3D_BASE 554
#else
#define SPRITE_TOGEBL3D_BASE 503
#endif

static sprite_pattern spat_chg0 = {1, {{-8, -8, 0, SPRITE_TOGEBL3D_BASE}}};
static sprite_pattern spat_chg1 = {1, {{-8, -8, 0, SPRITE_TOGEBL3D_BASE + 1}}};
sprite_pattern *pat_chg[1] = {&spat_chg0};
sprite_pattern *togeball_pat[1] = {&spat_chg1};

void togeball(sprite_status *actionwk) {
    if (actionwk->userflag.b.h >= 0)
        togeball_main(actionwk);
    else
        togeball_opt(actionwk);
}

void togeball_main(sprite_status *actionwk) {
    switch (actionwk->r_no0) {

    case 0:
        act_init(actionwk);
        break;
    case 2:
        act_move(actionwk);
        break;
    }
    actionsub(actionwk);
    frameout_s(actionwk);
}

void act_init(sprite_status *actionwk) {
    sprite_status *a1;

    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->sprpri = 1;
    actionwk->sprhsize = 8;
    actionwk->sprvsize = 8;
    actionwk->patbase = pat_chg;
    actionwk->sproffset = 957;
    ((Sint16 *)actionwk)[24] = 256;
    if (actionwk->userflag.b.h != 0)
        ((Sint16 *)actionwk)[24] = -((Sint16 *)actionwk)[24];

    if (actwkchk2(actionwk, &a1) != 0) {

        frameout(actionwk);
        return;
    }
    ((Uint16 *)a1)[33] = actionwk - actwk;
    ((Uint16 *)actionwk)[25] = a1 - actwk;
    a1->actno = actionwk->actno;
    a1->userflag.b.h = -1;
    a1->sprhsize = 8;
    a1->sprvsize = 8;

    if (actwkchk2(actionwk, &a1) != 0) {

        frameout(actionwk);
        return;
    }
    ((Uint16 *)a1)[33] = actionwk - actwk;
    ((Uint16 *)actionwk)[26] = a1 - actwk;
    a1->actno = actionwk->actno;
    a1->userflag.b.h = -1;
    a1->sprhsize = 8;
    a1->sprvsize = 8;

    if (actwkchk2(actionwk, &a1) != 0) {

        frameout(actionwk);
        return;
    }
    ((Uint16 *)a1)[33] = actionwk - actwk;
    ((Uint16 *)actionwk)[27] = a1 - actwk;
    a1->actno = actionwk->actno;
    a1->userflag.b.h = -1;
    a1->sprhsize = 8;
    a1->sprvsize = 8;

    if (actwkchk2(actionwk, &a1) != 0) {

        frameout(actionwk);
        return;
    }
    ((Uint16 *)a1)[33] = actionwk - actwk;
    ((Uint16 *)actionwk)[28] = a1 - actwk;
    a1->actno = actionwk->actno;
    a1->userflag.b.h = -1;
    a1->sprhsize = 8;
    a1->sprvsize = 8;
}

void act_move(sprite_status *actionwk) {
    Sint16 d0;
    Uint16 sin, cos;
    Sint32 d0l, d1l, d2l, d3l, d4l, d5l;
    sprite_status *a1;

    d0 = ((Sint16 *)actionwk)[24];
    ((Sint16 *)actionwk)[23] += d0;
    d0 = ((Sint16 *)actionwk)[23];
    d0 >>= 8;

    sinset(d0, (Sint16 *)&sin, (Sint16 *)&cos);
    d0l = d1l = 0;
    d0l = sin << 16;
    d1l = cos << 16;
    d0l >>= 2;
    d1l >>= 2;
    d4l = d0l;
    d5l = d1l;
    d0l >>= 1;
    d1l >>= 1;
    d2l = d0l;
    d3l = d1l;
    d0l >>= 1;
    d1l >>= 1;
    a1 = &actwk[((Uint16 *)actionwk)[25]];
    *(Sint32 *)&a1->actfree[4] = actionwk->yposi.l + d0l;
    *(Sint32 *)&a1->actfree[0] = actionwk->xposi.l + d1l;
    a1 = &actwk[((Uint16 *)actionwk)[26]];
    *(Sint32 *)&a1->actfree[4] = actionwk->yposi.l + d2l;
    *(Sint32 *)&a1->actfree[0] = actionwk->xposi.l + d3l;
    a1 = &actwk[((Uint16 *)actionwk)[27]];
    *(Sint32 *)&a1->actfree[4] = actionwk->yposi.l + d0l + d2l;
    *(Sint32 *)&a1->actfree[0] = actionwk->xposi.l + d1l + d3l;
    a1 = &actwk[((Uint16 *)actionwk)[28]];
    *(Sint32 *)&a1->actfree[4] = actionwk->yposi.l + d4l;
    *(Sint32 *)&a1->actfree[0] = actionwk->xposi.l + d5l;
}

void togeball_opt(sprite_status *actionwk) {
    sprite_status *a1;

    switch (actionwk->r_no0) {

    case 0:
        act_init2(actionwk);
    case 2:
        act_move2(actionwk);
        break;
    }
    a1 = &actwk[((Uint16 *)actionwk)[33]];
    if (a1->actno != 44) {

        frameout(actionwk);
        return;
    }
    actionsub(actionwk);
}

void act_init2(sprite_status *actionwk) {
    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->sprpri = 1;
    actionwk->patbase = togeball_pat;
    actionwk->sproffset = 957;
    actionwk->colino = 139;
}

void act_move2(sprite_status *actionwk) {
    actionwk->xposi.w.h = ((Sint16 *)actionwk)[24];
    actionwk->yposi.w.h = ((Sint16 *)actionwk)[26];
}
