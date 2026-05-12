#include "../equ.h"
#include "tekkyu.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"

#if defined(R41A) || defined(R42A)
#define SPRITE_TEKKYU_BASE 467
#elif defined(R42C) || defined(R42D)
#define SPRITE_TEKKYU_BASE 472
#else
#define SPRITE_TEKKYU_BASE 493
#endif

static void tekkyu_main(sprite_status *pActwk);
static void act_init(sprite_status *pActwk);
static void act_move(sprite_status *pActwk);
static void tekkyu_opt(sprite_status *pActwk);
static void opt_act_init(sprite_status *pActwk);
static void opt_act_move(sprite_status *pActwk);

static sprite_pattern tekkyu_pat00 = {1, {{-8, -8, 0, SPRITE_TEKKYU_BASE}}};
sprite_pattern *pat_tekkyu[1] = {&tekkyu_pat00};
sprite_pattern **pat_tyuusin = {pat_tekkyu};
static void (*act_tbl[2])(sprite_status *) = {&act_init, &act_move};
static sprite_pattern kusari_pat00 = {1, {{-8, -8, 0, SPRITE_TEKKYU_BASE + 1}}};
sprite_pattern *pat_kusari[1] = {&kusari_pat00};
static sprite_pattern tekkyu1_pat00 = {1,
                                       {{-16, -16, 0, SPRITE_TEKKYU_BASE + 2}}};
sprite_pattern *pat_tekkyu1[1] = {&tekkyu1_pat00};
static void (*opt_act_tbl[2])(sprite_status *) = {&opt_act_init, &opt_act_move};

void tekkyu(sprite_status *pActwk) {
    if (pActwk->userflag.b.h & 128) {
        tekkyu_opt(pActwk);
    } else {
        tekkyu_main(pActwk);
    }
}

static void tekkyu_main(sprite_status *pActwk) {
    act_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s(pActwk);
}

static void act_init(sprite_status *pActwk) {
    sprite_status *pNewact;
    Sint16 *pActidx;
    Sint32 i;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->sprhsize = 8;
    pActwk->sprvsize = 8;
    pActwk->patbase = pat_tyuusin;
    pActwk->sproffset = 872;

    if (pActwk->userflag.b.h == 0) {
        ((Sint16 *)pActwk)[24] = 256;
    } else {
        ((Sint16 *)pActwk)[24] = -256;
    }

    pActidx = &((Sint16 *)pActwk)[25];
    for (i = 0; i < 6; ++i) {
        if (actwkchk(&pNewact) != 0) {
            frameout(pActwk);
            return;
        }

        *pActidx++ = pNewact - actwk;
        pNewact->actno = pActwk->actno;
        pNewact->userflag.b.h = -1;
        pNewact->sprhsize = 8;
        pNewact->sprvsize = 8;
        ((Sint16 *)pNewact)[33] = pActwk - actwk;
    }

    pNewact->userflag.b.h = -2;
    act_move(pActwk);
}

static void act_move(sprite_status *pActwk) {
    int_union ldSin, ldCos;
    Sint32 sinsv1, sinsv2, cossv1, cossv2;
    sprite_status *pSubact;

    ((Sint16 *)pActwk)[23] += ((Sint16 *)pActwk)[24];

    sinset(pActwk->actfree[1], &ldSin.w.h, &ldCos.w.h);
    ldSin.w.l = ldCos.w.l = 0;

    ldSin.l >>= 2;
    ldCos.l >>= 2;
    sinsv1 = ldSin.l;
    cossv1 = ldCos.l;

    ldSin.l >>= 1;
    ldCos.l >>= 1;
    sinsv2 = ldSin.l;
    cossv2 = ldCos.l;

    ldSin.l >>= 1;
    ldCos.l >>= 1;

    pSubact = &actwk[((Sint16 *)pActwk)[25]];
    *(Sint32 *)&pSubact->actfree[4] = pActwk->yposi.l + ldSin.l;
    *(Sint32 *)&pSubact->actfree[0] = pActwk->xposi.l + ldCos.l;

    pSubact = &actwk[((Sint16 *)pActwk)[26]];
    *(Sint32 *)&pSubact->actfree[4] = pActwk->yposi.l + sinsv2;
    *(Sint32 *)&pSubact->actfree[0] = pActwk->xposi.l + cossv2;

    pSubact = &actwk[((Sint16 *)pActwk)[27]];
    *(Sint32 *)&pSubact->actfree[4] = pActwk->yposi.l + ldSin.l + sinsv2;
    *(Sint32 *)&pSubact->actfree[0] = pActwk->xposi.l + ldCos.l + cossv2;

    pSubact = &actwk[((Sint16 *)pActwk)[28]];
    *(Sint32 *)&pSubact->actfree[4] = pActwk->yposi.l + sinsv1;
    *(Sint32 *)&pSubact->actfree[0] = pActwk->xposi.l + cossv1;

    pSubact = &actwk[((Sint16 *)pActwk)[29]];
    *(Sint32 *)&pSubact->actfree[4] = pActwk->yposi.l + ldSin.l + sinsv1;
    *(Sint32 *)&pSubact->actfree[0] = pActwk->xposi.l + ldCos.l + cossv1;

    pSubact = &actwk[((Sint16 *)pActwk)[30]];
    *(Sint32 *)&pSubact->actfree[4] = pActwk->yposi.l + sinsv2 + sinsv1;
    *(Sint32 *)&pSubact->actfree[0] = pActwk->xposi.l + cossv2 + cossv1;
}

static void tekkyu_opt(sprite_status *pActwk) {
    opt_act_tbl[pActwk->r_no0 / 2](pActwk);
    if (actwk[((Sint16 *)pActwk)[33]].actno != 54) {

        frameout(pActwk);
    } else {
        actionsub(pActwk);
    }
}

static void opt_act_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;

    if (pActwk->userflag.b.h == -1) {
        pActwk->sprpri = 3;
        pActwk->patbase = pat_kusari;
        pActwk->sproffset = 872;
    } else {
        pActwk->sprpri = 2;
        pActwk->patbase = pat_tekkyu1;
        pActwk->sproffset = 880;
        pActwk->colino = 169;
    }

    opt_act_move(pActwk);
}

static void opt_act_move(sprite_status *pActwk) {
    pActwk->xposi.w.h = ((Sint16 *)pActwk)[24];
    pActwk->yposi.w.h = ((Sint16 *)pActwk)[26];
}
