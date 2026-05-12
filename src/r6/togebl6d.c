#include "../equ.h"
#include "togebl6a.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"

#if defined(R63C) || defined(R63D)
#define SPRITE_TOGEBL6D_BASE 460
#else
#define SPRITE_TOGEBL6D_BASE 497
#endif

static void act_init(sprite_status *actionwk);
static void act_move(sprite_status *actionwk);

static sprite_pattern spat_chg0 = {1, {{-8, -8, 0, SPRITE_TOGEBL6D_BASE}}};
static sprite_pattern spat_chg1 = {1, {{-8, -8, 0, SPRITE_TOGEBL6D_BASE + 1}}};
sprite_pattern *pat_chg[1] = {&spat_chg0};
sprite_pattern *pat_togeball[1] = {&spat_chg1};

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

static void act_init(sprite_status *actionwk) {
    sprite_status *a1;

    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->sprpri = 1;
    actionwk->sprhsize = 8;
    actionwk->sprvsize = 8;
    actionwk->patbase = pat_chg;
    actionwk->sproffset = 1016;
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

    if (actwkchk2(actionwk, &a1) != 0) {
        frameout(actionwk);
        return;
    }
    ((Uint16 *)a1)[33] = actionwk - actwk;
    ((Uint16 *)actionwk)[29] = a1 - actwk;
    a1->actno = actionwk->actno;
    a1->userflag.b.h = -1;
    a1->sprhsize = 8;
    a1->sprvsize = 8;

    if (actwkchk2(actionwk, &a1) != 0) {
        frameout(actionwk);
        return;
    }
    ((Uint16 *)a1)[33] = actionwk - actwk;
    ((Uint16 *)actionwk)[30] = a1 - actwk;
    a1->actno = actionwk->actno;
    a1->userflag.b.h = -1;
    a1->sprhsize = 8;
    a1->sprvsize = 8;
}

static void act_move(sprite_status *actionwk) {
    Sint16 d0, ano;
    Uint16 sin, cos;
    Sint32 d0l, d1l, d2l, d3l, d4l, d5l;
    sprite_status *adr1;

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
    ano = actionwk->actfree[4];
    *(Sint32 *)&actwk[ano].actfree[4] = actionwk->yposi.l + d0l;
    *(Sint32 *)&actwk[ano].actfree[0] = actionwk->xposi.l + d1l;

    adr1 = &actwk[((Uint16 *)actionwk)[25]];
    *(Sint32 *)&adr1->actfree[4] = actionwk->yposi.l + d0l;
    *(Sint32 *)&adr1->actfree[0] = actionwk->xposi.l + d1l;
    adr1 = &actwk[((Uint16 *)actionwk)[26]];
    *(Sint32 *)&adr1->actfree[4] = actionwk->yposi.l + d2l;
    *(Sint32 *)&adr1->actfree[0] = actionwk->xposi.l + d3l;
    adr1 = &actwk[((Uint16 *)actionwk)[27]];
    *(Sint32 *)&adr1->actfree[4] = actionwk->yposi.l + d0l + d2l;
    *(Sint32 *)&adr1->actfree[0] = actionwk->xposi.l + d1l + d3l;

    adr1 = &actwk[((Uint16 *)actionwk)[28]];
    *(Sint32 *)&adr1->actfree[4] = actionwk->yposi.l + d4l;
    *(Sint32 *)&adr1->actfree[0] = actionwk->xposi.l + d5l;
    adr1 = &actwk[((Uint16 *)actionwk)[29]];
    *(Sint32 *)&adr1->actfree[4] = actionwk->yposi.l + d0l + d4l;
    *(Sint32 *)&adr1->actfree[0] = actionwk->xposi.l + d1l + d5l;
    adr1 = &actwk[((Uint16 *)actionwk)[30]];
    *(Sint32 *)&adr1->actfree[4] = actionwk->yposi.l + d2l + d4l;
    *(Sint32 *)&adr1->actfree[0] = actionwk->xposi.l + d3l + d5l;
}

void togeball_opt(sprite_status *actionwk) {
    Sint16 ano;

    switch (actionwk->r_no0) {
    case 0:
        act_init2(actionwk);
    case 2:
        act_move2(actionwk);
        break;
    }
    ano = actionwk->actfree[20];
    if (actwk[ano].actno != 45) {
        frameout(actionwk);
        return;
    }
    actionsub(actionwk);
}

void act_init2(sprite_status *actionwk) {
    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->sprpri = 1;
    actionwk->patbase = pat_togeball;
    actionwk->sproffset = 1016;
    actionwk->colino = 185;
}

void act_move2(sprite_status *actionwk) {
    actionwk->xposi.w.h = ((Sint16 *)actionwk)[24];
    actionwk->yposi.w.h = ((Sint16 *)actionwk)[26];
}
