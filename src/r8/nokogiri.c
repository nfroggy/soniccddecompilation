#include "../equ.h"
#include "nokogiri.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../playsub.h"

#if defined(R83)
#define SPRITE_NOKOGIRI_BASE 404
#else
#define SPRITE_NOKOGIRI_BASE 425
#endif

static char p00[4] = {1, 0, 1, -1};
static char p01[4] = {1, 2, 3, -1};
static char p02[4] = {1, 4, 5, -1};
static char p03[4] = {1, 6, 7, -1};
static char p04[4] = {1, 8, 9, -1};
static char p05[4] = {1, 10, 11, -1};
static char p06[4] = {1, 12, 13, -1};
static char p07[4] = {1, 14, 15, -1};
static char p08[4] = {1, 16, 17, -1};
static char *pchg[10] = {p00, p01, p02, p03, p04, p05, p06, p07, p08, p08};
static sprite_pattern pat00 = {1, {{-32, -34, 0, SPRITE_NOKOGIRI_BASE}}};
static sprite_pattern pat01 = {1, {{-32, -34, 0, SPRITE_NOKOGIRI_BASE + 1}}};
static sprite_pattern pat02 = {1, {{-43, -43, 0, SPRITE_NOKOGIRI_BASE + 2}}};
static sprite_pattern pat03 = {1, {{-43, -43, 0, SPRITE_NOKOGIRI_BASE + 3}}};
static sprite_pattern pat04 = {1, {{-52, -54, 0, SPRITE_NOKOGIRI_BASE + 4}}};
static sprite_pattern pat05 = {1, {{-52, -54, 0, SPRITE_NOKOGIRI_BASE + 5}}};
static sprite_pattern pat06 = {1, {{-63, -63, 0, SPRITE_NOKOGIRI_BASE + 6}}};
static sprite_pattern pat07 = {1, {{-63, -63, 0, SPRITE_NOKOGIRI_BASE + 7}}};
static sprite_pattern pat08 = {1, {{-72, -74, 0, SPRITE_NOKOGIRI_BASE + 8}}};
static sprite_pattern pat09 = {1, {{-72, -74, 0, SPRITE_NOKOGIRI_BASE + 9}}};
static sprite_pattern pat10 = {1, {{-83, -83, 0, SPRITE_NOKOGIRI_BASE + 10}}};
static sprite_pattern pat11 = {1, {{-83, -83, 0, SPRITE_NOKOGIRI_BASE + 11}}};
static sprite_pattern pat12 = {1, {{-92, -94, 0, SPRITE_NOKOGIRI_BASE + 12}}};
static sprite_pattern pat13 = {1, {{-92, -94, 0, SPRITE_NOKOGIRI_BASE + 13}}};
static sprite_pattern pat14 = {1, {{-103, -103, 0, SPRITE_NOKOGIRI_BASE + 14}}};
static sprite_pattern pat15 = {1, {{-103, -103, 0, SPRITE_NOKOGIRI_BASE + 15}}};
static sprite_pattern pat16 = {1, {{-112, -114, 0, SPRITE_NOKOGIRI_BASE + 16}}};
static sprite_pattern pat17 = {1, {{-112, -114, 0, SPRITE_NOKOGIRI_BASE + 17}}};
sprite_pattern *pat_nokogiri[20] = {
    &pat00, &pat01, &pat02, &pat03, &pat04, &pat05, &pat06, &pat07, &pat08,
    &pat09, &pat10, &pat11, &pat12, &pat13, &pat14, &pat15, &pat16, &pat17};
static sprite_pattern pat_bar0 = {1,
                                  {{-41, -43, 0, SPRITE_NOKOGIRI_BASE + 18}}};
sprite_pattern *pat_bar[1] = {&pat_bar0};

void nokogiri(sprite_status *actionwk) {
    if (actionwk->userflag.b.l != 0) {
        bar(actionwk);
        return;
    }
    switch (actionwk->r_no0) {
    case 0:
        act_init(actionwk);
    case 2:
        act_open(actionwk);
        break;
    case 4:
        act_opend(actionwk);
        break;
    case 6:
        act_close(actionwk);
        break;
    case 8:
        act_closed(actionwk);
        break;
    }
    patchg(actionwk, (Uint8 **)pchg);
    actionsub(actionwk);
    frameout_s00(actionwk, ((Sint16 *)actionwk)[29]);
}

void act_init(sprite_status *actionwk) {
    sprite_status *a1;
    Sint16 d0, d1;

    ((Sint16 *)actionwk)[29] = actionwk->xposi.w.h;
    actionwk->actflg |= 4;
    actionwk->sprpri = 3;
    actionwk->sproffset = 17174;
    actionwk->patbase = pat_nokogiri;
    actionwk->colino = 169;
    actionwk->sprhsize = 80;
    actionwk->sprvsize = 80;

    ((Sint32 *)actionwk)[12] = 65536;
    ((Sint32 *)actionwk)[13] = 65536;

    if (actionwk->userflag.b.h & 1) {
        ((Sint32 *)actionwk)[12] = -((Sint32 *)actionwk)[12];

        actionwk->actflg |= 1;
        actionwk->cddat |= 1;
    }

    if (actionwk->userflag.b.h & 2) {
        ((Sint32 *)actionwk)[13] = -((Sint32 *)actionwk)[13];

        actionwk->actflg |= 2;
        actionwk->cddat |= 2;
    }

    ((Sint16 *)actionwk)[23] = 60;
    actionwk->r_no0 = 8;
    if (actionwk->userflag.b.h < 0) {
        actionwk->mstno.b.h = 9;
        actionwk->r_no0 = 4;
    }

    if (actwkchk2(actionwk, &a1) != 0) {
        frameout(actionwk);
        return;
    }
    ((Uint16 *)actionwk)[31] = a1 - actwk;
    ((Uint16 *)a1)[23] = actionwk - actwk;
    a1->actno = actionwk->actno;
    a1->actflg = actionwk->actflg;
    a1->sproffset = actionwk->sproffset;
    a1->userflag.b.h = actionwk->userflag.b.h;
    a1->userflag.b.l = -1;
    a1->sprpri = actionwk->sprpri - 1;
    a1->xposi.w.h = actionwk->xposi.w.h;
    a1->yposi.w.h = actionwk->yposi.w.h + 1;
    if (actionwk->userflag.b.h & 128) {
        d0 = 80;
        d1 = 79;
        if (actionwk->userflag.b.h & 1)
            d0 = -(d0 - 1);
        if (actionwk->userflag.b.h & 2)
            d1 = -(d1 - 1);
        a1->xposi.w.h -= d0;
        a1->yposi.w.h -= d1;
    } else {
        if (actionwk->userflag.b.h & 1) {
            --a1->yposi.w.h;
            ++a1->xposi.w.h;
        }
        if (actionwk->userflag.b.h & 2) {
            --a1->yposi.w.h;
            --a1->xposi.w.h;
        }
    }
}

void act_open(sprite_status *actionwk) {
    if (--((Sint16 *)actionwk)[23] == 0) {
        ++actionwk->mstno.b.h;
        ((Sint16 *)actionwk)[23] = 9;
        if (--((Sint16 *)actionwk)[28] == 0) {
            actionwk->r_no0 += 2;
            ((Sint16 *)actionwk)[23] = 60;
        }
    }

    actionwk->xposi.l += ((Sint32 *)actionwk)[12];
    actionwk->yposi.l += ((Sint32 *)actionwk)[13];
    _soundset(actionwk);
}

void act_opend(sprite_status *actionwk) {
    if (--((Sint16 *)actionwk)[23] == 0) {
        actionwk->r_no0 += 2;
        ((Sint16 *)actionwk)[28] = 9;
        ((Sint16 *)actionwk)[23] = 1;
    }
    _soundset(actionwk);
}

void act_close(sprite_status *actionwk) {
    if (--((Sint16 *)actionwk)[23] == 0) {
        --actionwk->mstno.b.h;
        ((Sint16 *)actionwk)[23] = 9;
        if (--((Sint16 *)actionwk)[28] == 0) {
            actionwk->r_no0 += 2;
            ((Sint16 *)actionwk)[23] = 60;
        }
    }

    actionwk->xposi.l -= ((Sint32 *)actionwk)[12];
    actionwk->yposi.l -= ((Sint32 *)actionwk)[13];
    _soundset(actionwk);
}

void act_closed(sprite_status *actionwk) {
    if (--((Sint16 *)actionwk)[23] == 0) {
        actionwk->r_no0 -= 6;
        ((Sint16 *)actionwk)[28] = 9;
        ((Sint16 *)actionwk)[23] = 1;
    }
    _soundset(actionwk);
}

void _soundset(sprite_status *actionwk) {
    Sint16 d0;

    if ((char)actionwk->actflg >= 0)
        return;
    ++((Sint16 *)actionwk)[30];
    d0 = ((Sint16 *)actionwk)[30];
    d0 &= 31;
    if (d0 != 0)
        return;
    soundset(177);
}

void bar(sprite_status *actionwk) {
    Sint16 ano;

    ano = ((Sint16 *)actionwk)[23];
    if (actwk[ano].actno != 39) {
        frameout(actionwk);
        return;
    }
    if (actionwk->r_no0) {
        actionsub(actionwk);
        return;
    }

    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->patbase = pat_bar;
    actionwk->sprhsize = 32;
    actionwk->sprvsize = 32;
}
