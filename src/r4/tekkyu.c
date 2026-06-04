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

typedef struct {
    union {
        struct {
            union {
                Sint16 angle;
                struct {
                    Uint8 angle_low;
                    Uint8 angle_high;
                };
            };
            Sint16 angular_speed;
            Sint16 child_index[6];
        };
        struct {
            Sint32 target_x;
            Sint32 target_y;
        };
    };
    Sint16 parent_index;
} tekkyu_work;

static tekkyu_work *tekkyu_get_work(sprite_status *pActwk) {
    return (tekkyu_work *)pActwk->actfree;
}

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
    tekkyu_work *work = tekkyu_get_work(pActwk);
    sprite_status *pNewact;
    Sint32 i;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->sprhsize = 8;
    pActwk->sprvsize = 8;
    pActwk->patbase = pat_tyuusin;
    pActwk->sproffset = 872;

    if (pActwk->userflag.b.h == 0) {
        work->angular_speed = 256;
    } else {
        work->angular_speed = -256;
    }

    for (i = 0; i < 6; ++i) {
        if (actwkchk(&pNewact) != 0) {
            frameout(pActwk);
            return;
        }

        work->child_index[i] = (Sint16)(pNewact - actwk);
        pNewact->actno = pActwk->actno;
        pNewact->userflag.b.h = -1;
        pNewact->sprhsize = 8;
        pNewact->sprvsize = 8;
        tekkyu_get_work(pNewact)->parent_index = (Sint16)(pActwk - actwk);
    }

    pNewact->userflag.b.h = -2;
    act_move(pActwk);
}

static void act_move(sprite_status *pActwk) {
    tekkyu_work *work = tekkyu_get_work(pActwk);
    int_union ldSin, ldCos;
    Sint32 sinsv1, sinsv2, cossv1, cossv2;
    sprite_status *pSubact;

    work->angle += work->angular_speed;

    sinset(work->angle_high, &ldSin.w.h, &ldCos.w.h);
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

    pSubact = &actwk[work->child_index[0]];
    tekkyu_get_work(pSubact)->target_y = pActwk->yposi.l + ldSin.l;
    tekkyu_get_work(pSubact)->target_x = pActwk->xposi.l + ldCos.l;

    pSubact = &actwk[work->child_index[1]];
    tekkyu_get_work(pSubact)->target_y = pActwk->yposi.l + sinsv2;
    tekkyu_get_work(pSubact)->target_x = pActwk->xposi.l + cossv2;

    pSubact = &actwk[work->child_index[2]];
    tekkyu_get_work(pSubact)->target_y = pActwk->yposi.l + ldSin.l + sinsv2;
    tekkyu_get_work(pSubact)->target_x = pActwk->xposi.l + ldCos.l + cossv2;

    pSubact = &actwk[work->child_index[3]];
    tekkyu_get_work(pSubact)->target_y = pActwk->yposi.l + sinsv1;
    tekkyu_get_work(pSubact)->target_x = pActwk->xposi.l + cossv1;

    pSubact = &actwk[work->child_index[4]];
    tekkyu_get_work(pSubact)->target_y = pActwk->yposi.l + ldSin.l + sinsv1;
    tekkyu_get_work(pSubact)->target_x = pActwk->xposi.l + ldCos.l + cossv1;

    pSubact = &actwk[work->child_index[5]];
    tekkyu_get_work(pSubact)->target_y = pActwk->yposi.l + sinsv2 + sinsv1;
    tekkyu_get_work(pSubact)->target_x = pActwk->xposi.l + cossv2 + cossv1;
}

static void tekkyu_opt(sprite_status *pActwk) {
    opt_act_tbl[pActwk->r_no0 / 2](pActwk);
    if (actwk[tekkyu_get_work(pActwk)->parent_index].actno != 54) {

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
    tekkyu_work *work = tekkyu_get_work(pActwk);

    pActwk->xposi.w.h = (Sint16)(work->target_x >> 16);
    pActwk->yposi.w.h = (Sint16)(work->target_y >> 16);
}
