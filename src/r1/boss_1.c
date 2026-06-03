#include "../equ.h"
#include "boss_1.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../etc.h"
#include "../goal.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../score.h"
#include "col1c.h"

Uint8 egg1_pchg0[3] = {59, 0, 255};
Uint8 egg1_pchg1[4] = {7, 2, 3, 255};
Uint8 egg1_pchg2[6] = {3, 1, 5, 4, 6, 255};
Uint8 egg1_pchg3[4] = {3, 7, 8, 255};
Uint8 egg1_pchg4[4] = {3, 9, 10, 255};
Uint8 *egg1_pchg[5] = {egg1_pchg0, egg1_pchg1, egg1_pchg2, egg1_pchg3,
                       egg1_pchg4};
sprite_pattern spr_egg1_pat0 = {1, {{-28, -24, 0, 512}}};
sprite_pattern spr_egg1_pat1 = {1, {{-28, -40, 0, 513}}};
sprite_pattern spr_egg1_pat2 = {1, {{-28, -24, 0, 514}}};
sprite_pattern spr_egg1_pat3 = {1, {{-28, -24, 0, 515}}};
sprite_pattern spr_egg1_pat4 = {1, {{-28, -40, 0, 516}}};
sprite_pattern spr_egg1_pat5 = {1, {{-28, -24, 0, 517}}};
sprite_pattern spr_egg1_pat6 = {1, {{-28, -24, 0, 518}}};
sprite_pattern spr_egg1_pat7 = {1, {{-28, -24, 0, 519}}};
sprite_pattern spr_egg1_pat8 = {1, {{-28, -24, 0, 520}}};
sprite_pattern spr_egg1_pat9 = {1, {{-28, -24, 0, 521}}};
sprite_pattern spr_egg1_pat10 = {1, {{-28, -24, 0, 522}}};
sprite_pattern *egg1_pat[11] = {&spr_egg1_pat0, &spr_egg1_pat1, &spr_egg1_pat2,
                                &spr_egg1_pat3, &spr_egg1_pat4, &spr_egg1_pat5,
                                &spr_egg1_pat6, &spr_egg1_pat7, &spr_egg1_pat8,
                                &spr_egg1_pat9, &spr_egg1_pat10};
sprite_pattern spr_egg1body_pat0 = {2, {{0, 8, 0, 511}, {-36, -31, 0, 498}}};
sprite_pattern *egg1body_pat[1] = {&spr_egg1body_pat0};
sprite_pattern spr_egg1leg1_pat0 = {1, {{-8, -8, 0, 499}}};
sprite_pattern *egg1leg1_pat[1] = {&spr_egg1leg1_pat0};
sprite_pattern spr_egg1leg2_pat0 = {1, {{-12, -20, 0, 500}}};
sprite_pattern *egg1leg2_pat[1] = {&spr_egg1leg2_pat0};
sprite_pattern spr_egg1leg3_pat0 = {1, {{-32, -20, 0, 501}}};
sprite_pattern *egg1leg3_pat[1] = {&spr_egg1leg3_pat0};
sprite_pattern spr_egg1arm1_pat0 = {1, {{-24, -12, 0, 502}}};
sprite_pattern *egg1arm1_pat[1] = {&spr_egg1arm1_pat0};
sprite_pattern spr_egg1arm2_pat0 = {1, {{-8, -8, 0, 503}}};
sprite_pattern *egg1arm2_pat[1] = {&spr_egg1arm2_pat0};
sprite_pattern spr_egg1arm3_pat10 = {2, {{-14, -8, 0, 504}, {10, -8, 0, 523}}};
sprite_pattern spr_egg1arm3_pat11 = {2, {{-6, -8, 0, 505}, {10, -8, 0, 523}}};
sprite_pattern spr_egg1arm3_pat12 = {2, {{2, -8, 0, 506}, {10, -8, 0, 523}}};
sprite_pattern *egg1arm3_pat1[3] = {&spr_egg1arm3_pat10, &spr_egg1arm3_pat11,
                                    &spr_egg1arm3_pat12};
sprite_pattern spr_egg1arm3_pat20 = {1, {{-14, -8, 0, 507}}};
sprite_pattern spr_egg1arm3_pat21 = {1, {{-6, -8, 0, 508}}};
sprite_pattern spr_egg1arm3_pat22 = {1, {{2, -8, 0, 509}}};
sprite_pattern *egg1arm3_pat2[3] = {&spr_egg1arm3_pat20, &spr_egg1arm3_pat21,
                                    &spr_egg1arm3_pat22};
sprite_pattern spr_egg1arm4_pat0 = {1, {{-20, -16, 0, 510}}};
sprite_pattern *egg1arm4_pat[1] = {&spr_egg1arm4_pat0};
extern Sint16 scr_dir_tbl[6];

void egg1(sprite_status *pActwk) {
    Sint32 (*tbl[8])(sprite_status *) = {&egg1_ini, &egg1_01,  &egg1_02,
                                         &egg1_03,  &egg1_04,  &egg1_05,
                                         &egg1_06,  &egg1_wait};

    egg1_anime(pActwk);
    egg1_jisin(pActwk);
    egg1_warai_chk(pActwk);
    egg1_hit_chk(pActwk);
    if (tbl[pActwk->r_no0 / 2](pActwk) != 0) {
        patchg(pActwk, egg1_pchg);
        actionsub(pActwk);
    }
}

void egg1_anime(sprite_status *pActwk) {
    if (!pActwk->actfree[0])
        return;
    --pActwk->actfree[0];
    if (pActwk->actfree[0])
        return;
    pActwk->patno = 0;
    pActwk->patcnt = 0;
    pActwk->pattim = 0;
    pActwk->pattimm = 0;
    pActwk->mstno.b.h = 0;
}

void egg1_jisin(sprite_status *pActwk) {
    Sint16 temp0 = 784;
    Sint16 temp1 = 1;

    if (bossflag) {
        temp0 = 256;
        if (pActwk->actfree[11]) {
            --pActwk->actfree[11];
            if (pActwk->actfree[11] & 1)
                temp1 *= -1;

            temp0 += temp1;
        }
    }
    scralim_down = temp0;
    scralim_n_down = temp0;
}

void sonic_hajiku(Sint16 subact) {
    Sint16 temp1 = 1024, temp2 = -1024;

    actwk[0].mspeed.w = 1024;
    if (!(actwk[0].cddat & 2)) {
        actwk[0].direc.b.h ^= -128;
        temp2 = 0;
    }
    if (actwk[0].xposi.w.h < actwk[subact].xposi.w.h)
        temp1 *= -1;
    actwk[0].xspeed.w = temp1;
    actwk[0].yspeed.w = temp2;
}

void egg1_coliude(Sint16 subact, sprite_status *pActwk) {

    actwk[subact].actfree[2] |= 16;
    sonic_hajiku(subact);
    egg1coli_set(pActwk);
    soundset(152);
    egg1_warai_chk(pActwk);
}

void egg1_hit_chk(sprite_status *pActwk) {
    Sint16 subact;

    if (!pActwk->actfree[10]) {
        if (!(pActwk->actfree[2] & 8))
            return;

        if (!pActwk->colino) {
            egg1_coli((Uint16)(Uint8)(pActwk - actwk), pActwk);
            return;
        }

        subact = ((Sint16 *)pActwk)[26];
        subact = ((Sint16 *)&actwk[subact])[26];
        subact = ((Sint16 *)&actwk[subact])[26];
        if (!actwk[subact].colino) {
            egg1_coli(subact, pActwk);
            return;
        }
        subact = ((Sint16 *)&actwk[subact])[26];
        if (!actwk[subact].colino) {
            egg1_coli(subact, pActwk);
            return;
        }
        subact = ((Sint16 *)pActwk)[26];
        subact = ((Sint16 *)&actwk[subact])[27];
        subact = ((Sint16 *)&actwk[subact])[26];
        if (!actwk[subact].colino) {
            egg1_coli(subact, pActwk);
            return;
        }
        subact = ((Sint16 *)&actwk[subact])[26];
        if (!actwk[subact].colino) {
            egg1_coli(subact, pActwk);
            return;
        }
        if (pActwk->actfree[1] == 3) {

            subact = ((Sint16 *)pActwk)[27];
            subact = ((Sint16 *)&actwk[subact])[27];
            subact = ((Sint16 *)&actwk[subact])[26];
            subact = ((Sint16 *)&actwk[subact])[26];
            if (!actwk[subact].colino) {
                egg1_coliude(subact, pActwk);
                return;
            }

            subact = ((Sint16 *)pActwk)[27];
            subact = ((Sint16 *)&actwk[subact])[26];
            subact = ((Sint16 *)&actwk[subact])[26];
            subact = ((Sint16 *)&actwk[subact])[26];
            if (!actwk[subact].colino) {
                egg1_coliude(subact, pActwk);
                return;
            }
        }
        if (pActwk->actfree[1] != 2)
            return;

        subact = ((Sint16 *)pActwk)[27];
        subact = ((Sint16 *)&actwk[subact])[26];
        subact = ((Sint16 *)&actwk[subact])[26];
        subact = ((Sint16 *)&actwk[subact])[26];
        if (actwk[subact].colino)
            return;

        egg1_coliude(subact, pActwk);
        return;
    }

    --pActwk->actfree[10];
    if (!pActwk->actfree[10])
        egg1coli_set(pActwk);
}

void egg1_warai_chk(sprite_status *pActwk) {

    if (!pActwk->mstno.b.h) {
        if (*(Sint16 *)&actwk[1].patno != 0 || actwk[0].r_no0 == 6) {

            pActwk->patno = 0;
            pActwk->patcnt = 0;
            pActwk->pattim = 0;
            pActwk->pattimm = 0;
            pActwk->mstno.b.h = 1;
            pActwk->actfree[0] = 60;
        }
    }
}

void egg1_coli(Sint16 subact, sprite_status *pActwk) {

    pActwk->actfree[10] = 20;

    sonic_hajiku(subact);

    pActwk->patno = 0;
    pActwk->patcnt = 0;
    pActwk->pattim = 0;
    pActwk->pattimm = 0;
    pActwk->mstno.b.h = 2;
    pActwk->actfree[0] = 120;
    --pActwk->actfree[1];
    if (!pActwk->actfree[1]) {

        pActwk->patno = 0;
        pActwk->patcnt = 0;
        pActwk->pattim = 0;
        pActwk->pattimm = 0;
        pActwk->mstno.b.h = 2;
        pActwk->actfree[0] = 0;

        pActwk->actfree[2] &= 247;
        pActwk->r_no1 = 0;

        pActwk->r_no0 = 6;
        pActwk->colino = 0;
        pActwk->colicnt = 0;
        actwk[((Sint16 *)pActwk)[27]].r_no0 = 4;

        subact = ((Sint16 *)pActwk)[26];
        actwk[subact].r_no0 = 14;
        subact = ((Sint16 *)&actwk[subact])[26];

        actwk[subact].r_no0 = 24;
        subact = ((Sint16 *)&actwk[subact])[26];
        actwk[subact].r_no0 = 14;
        actwk[subact].colino = 0;
        actwk[subact].colicnt = 0;
        subact = ((Sint16 *)&actwk[subact])[26];
        actwk[subact].r_no0 = 12;
        actwk[subact].colino = 0;
        actwk[subact].colicnt = 0;
        subact = ((Sint16 *)&actwk[((Sint16 *)pActwk)[26]])[27];

        actwk[subact].r_no0 = 24;
        subact = ((Sint16 *)&actwk[subact])[26];
        actwk[subact].r_no0 = 14;
        actwk[subact].colino = 0;
        actwk[subact].colicnt = 0;
        subact = ((Sint16 *)&actwk[subact])[26];
        actwk[subact].r_no0 = 12;
        actwk[subact].colino = 0;
        actwk[subact].colicnt = 0;
    } else {
        if (pActwk->actfree[1] != 2) {

            subact = ((Sint16 *)pActwk)[27];
            actwk[subact].actfree[2] |= 64;
            egg1coli_set_body(pActwk);
        } else {

            subact = ((Sint16 *)pActwk)[27];
            actwk[subact].actfree[2] |= 32;
            egg1coli_set_arm_l(pActwk);
        }
    }
}

void egg1coli_set_body(sprite_status *pActwk) {
    Sint16 subact;

    pActwk->colino = 252;
    pActwk->colicnt = 2;

    subact =
        ((Sint16 *)&actwk[((Sint16 *)&actwk[((Sint16 *)pActwk)[26]])[26]])[26];
    actwk[subact].colino = 189;
    actwk[subact].colicnt = 2;
    subact = ((Sint16 *)&actwk[subact])[26];
    actwk[subact].colino = 190;
    actwk[subact].colicnt = 2;

    subact =
        ((Sint16 *)&actwk[((Sint16 *)&actwk[((Sint16 *)pActwk)[26]])[27]])[26];
    actwk[subact].colino = 189;
    actwk[subact].colicnt = 2;
    subact = ((Sint16 *)&actwk[subact])[26];
    actwk[subact].colino = 190;
    actwk[subact].colicnt = 2;
}

void egg1coli_set_arm(Sint16 subact) {
    subact = ((Sint16 *)&actwk[((Sint16 *)&actwk[subact])[26]])[26];
    actwk[subact].colino = 255;
    actwk[subact].colicnt = 2;
}

void egg1coli_set_arm_l(sprite_status *pActwk) {
    egg1coli_set_arm(((Sint16 *)&actwk[((Sint16 *)pActwk)[27]])[26]);
    egg1coli_set_body(pActwk);
}

void egg1coli_set_arm_r(sprite_status *pActwk) {
    egg1coli_set_arm(((Sint16 *)&actwk[((Sint16 *)pActwk)[27]])[27]);
    egg1coli_set_arm_l(pActwk);
}

void egg1coli_set(sprite_status *pActwk) {
    if (pActwk->actfree[1] == 3)
        egg1coli_set_arm_r(pActwk);
    if (pActwk->actfree[1] == 2)
        egg1coli_set_arm_l(pActwk);
    if (pActwk->actfree[1] == 1)
        egg1coli_set_body(pActwk);
}

Sint32 egg1_ini(sprite_status *pActwk) {
    bossstart = 1;

    pActwk->cddat = 0;
    pActwk->r_no0 = 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 6;
    pActwk->sprhsize = 20;
    pActwk->sprvsize = 8;
    pActwk->sproffset = 1021;
    pActwk->patbase = egg1_pat;

    pActwk->mstno.b.h = 1;
    pActwk->patno = 0;
    pActwk->patcnt = 0;
    pActwk->pattim = 0;
    pActwk->pattimm = 0;

    pActwk->xposi.w.h = 3154;
    pActwk->yposi.w.h = 120;

    colorset2(5);
    return 1;
}

Sint32 make_act(sprite_status *pActwk, sprite_status **subActwk) {
    Sint32 ret;
    sprite_status *ppActwk;

    if (actwkchk(&ppActwk) == 0) {
        *subActwk = ppActwk;
        ppActwk->xposi.w.h = pActwk->xposi.w.h;
        ppActwk->yposi.w.h = pActwk->yposi.w.h;
        ret = 0;
    } else
        ret = 1;
    return ret;
}

void egg1_make_act(sprite_status *pActwk) {
    sprite_status *subActwk, *disActwk, *bodyActwk;
    Sint16 subact;

    if (make_act(pActwk, &subActwk) != 0)
        return;
    ((Sint16 *)pActwk)[26] = (Uint16)(Uint8)(subActwk - actwk);
    ((Sint16 *)subActwk)[25] = (Uint16)(Uint8)(pActwk - actwk);
    subActwk->sprpri = 6;
    subActwk->actno = 43;
    disActwk = subActwk;
    bodyActwk = subActwk;

    if (make_act(pActwk, &subActwk) != 0)
        return;
    ((Sint16 *)disActwk)[26] = (Uint16)(Uint8)(subActwk - actwk);
    ((Sint16 *)subActwk)[25] = (Uint16)(Uint8)(disActwk - actwk);
    subActwk->sprpri = 6;
    subActwk->actno = 44;
    disActwk = subActwk;

    if (make_act(pActwk, &subActwk) != 0)
        return;
    ((Sint16 *)disActwk)[26] = (Uint16)(Uint8)(subActwk - actwk);
    ((Sint16 *)subActwk)[25] = (Uint16)(Uint8)(disActwk - actwk);
    subActwk->sprpri = 3;
    subActwk->actno = 45;
    disActwk = subActwk;

    if (make_act(pActwk, &subActwk) != 0)
        return;
    ((Sint16 *)disActwk)[26] = (Uint16)(Uint8)(subActwk - actwk);
    ((Sint16 *)subActwk)[25] = (Uint16)(Uint8)(disActwk - actwk);
    subActwk->sprpri = 2;
    subActwk->actno = 46;

    disActwk = bodyActwk;
    ((Sint16 *)subActwk)[26] = (Uint16)(Uint8)(bodyActwk - actwk);

    if (make_act(pActwk, &subActwk) != 0)
        return;
    ((Sint16 *)disActwk)[27] = (Uint16)(Uint8)(subActwk - actwk);
    ((Sint16 *)subActwk)[25] = (Uint16)(Uint8)(disActwk - actwk);
    subActwk->sprpri = 7;
    subActwk->actno = 44;
    subActwk->actfree[2] |= 4;
    disActwk = subActwk;

    if (make_act(pActwk, &subActwk) != 0)
        return;
    ((Sint16 *)disActwk)[26] = (Uint16)(Uint8)(subActwk - actwk);
    ((Sint16 *)subActwk)[25] = (Uint16)(Uint8)(disActwk - actwk);
    subActwk->sprpri = 6;
    subActwk->actno = 45;
    subActwk->actfree[2] |= 4;
    disActwk = subActwk;

    if (make_act(pActwk, &subActwk) != 0)
        return;
    ((Sint16 *)disActwk)[26] = (Uint16)(Uint8)(subActwk - actwk);
    ((Sint16 *)subActwk)[25] = (Uint16)(Uint8)(disActwk - actwk);
    subActwk->sprpri = 5;
    subActwk->actno = 46;
    subActwk->actfree[2] |= 4;

    disActwk = bodyActwk;
    ((Sint16 *)subActwk)[26] = (Uint16)(Uint8)(bodyActwk - actwk);

    if (make_act(pActwk, &subActwk) != 0)
        return;
    ((Sint16 *)subActwk)[25] = (Uint16)(Uint8)(bodyActwk - actwk);
    ((Sint16 *)&actwk[((Sint16 *)disActwk)[25]])[27] =
        (Uint16)(Uint8)(subActwk - actwk);

    subActwk->sprpri = 3;
    subActwk->actno = 47;
    disActwk = subActwk;

    if (make_act(pActwk, &subActwk) != 0)
        return;
    ((Sint16 *)disActwk)[26] = (Uint16)(Uint8)(subActwk - actwk);
    subActwk->actfree[0] = 128;
    ((Sint16 *)subActwk)[25] = (Uint16)(Uint8)(disActwk - actwk);
    ((Sint16 *)disActwk)[26] = (Uint16)(Uint8)(subActwk - actwk);
    subActwk->sprpri = 5;
    subActwk->actno = 48;
    disActwk = subActwk;

    if (make_act(pActwk, &subActwk) != 0)
        return;
    ((Sint16 *)subActwk)[25] = (Uint16)(Uint8)(disActwk - actwk);
    ((Sint16 *)disActwk)[26] = (Uint16)(Uint8)(subActwk - actwk);
    subActwk->sprpri = 4;
    subActwk->patbase = egg1arm3_pat1;
    subActwk->actno = 49;
    disActwk = subActwk;

    if (make_act(pActwk, &subActwk) != 0)
        return;
    ((Sint16 *)subActwk)[25] = (Uint16)(Uint8)(disActwk - actwk);
    ((Sint16 *)disActwk)[26] = (Uint16)(Uint8)(subActwk - actwk);
    subActwk->sprpri = 3;
    subActwk->actno = 50;
    disActwk = subActwk;
    ((Sint16 *)subActwk)[26] = (Uint16)(Uint8)(bodyActwk - actwk);

    subact = ((Sint16 *)disActwk)[25];
    subact = ((Sint16 *)&actwk[subact])[25];
    subact = ((Sint16 *)&actwk[subact])[25];

    if (make_act(pActwk, &subActwk) != 0)
        return;
    ((Sint16 *)&actwk[subact])[27] = (Uint16)(Uint8)(subActwk - actwk);
    ((Sint16 *)subActwk)[25] = subact;
    subActwk->sprpri = 7;
    subActwk->actno = 48;
    subActwk->actfree[2] |= 4;
    disActwk = subActwk;

    if (make_act(pActwk, &subActwk) != 0)
        return;
    ((Sint16 *)subActwk)[25] = (Uint16)(Uint8)(disActwk - actwk);
    ((Sint16 *)disActwk)[26] = (Uint16)(Uint8)(subActwk - actwk);
    subActwk->sprpri = 7;
    subActwk->patbase = egg1arm3_pat2;
    subActwk->actno = 49;
    subActwk->actfree[2] |= 4;
    disActwk = subActwk;

    if (make_act(pActwk, &subActwk) != 0)
        return;
    ((Sint16 *)subActwk)[25] = (Uint16)(Uint8)(disActwk - actwk);
    ((Sint16 *)disActwk)[26] = (Uint16)(Uint8)(subActwk - actwk);
    subActwk->sprpri = 6;
    subActwk->actno = 50;
    subActwk->actfree[2] |= 4;
    disActwk = subActwk;

    ((Sint16 *)subActwk)[26] = (Uint16)(Uint8)(bodyActwk - actwk);

    pActwk->actfree[1] = 3;
    egg1coli_set(pActwk);
}

Sint32 egg1_01(sprite_status *pActwk) {
    scralim_right = 2752;
    scralim_n_right = 2752;

    if (actwk[0].xposi.w.h < 2666)
        return 1;
    if (actwk[0].xposi.w.h - 160 < scralim_left)
        return 1;

    if (actwk[0].xposi.w.h >= 2912) {
        pActwk->r_no0 = 12;
        scralim_right = 2752;
        scralim_n_right = 2752;
        scralim_left = 2752;
        scralim_n_left = 2752;
    } else {
        scralim_left = actwk[0].xposi.w.h - 160;
        scralim_n_left = actwk[0].xposi.w.h - 160;
    }
    return 1;
}

Sint32 egg1_06(sprite_status *pActwk) {

    scra_vline += 6;
    if (scra_vline >= 200) {

        sub_sync(103);

        bossflag = 1;
        pActwk->r_no0 = 14;
    }
    return 1;
}

Sint32 egg1_wait(sprite_status *pActwk) {
    ++pActwk->actfree[1];
    if (pActwk->actfree[1] == 60) {
        pActwk->actfree[1] = 0;
        pActwk->r_no0 = 4;
        pActwk->xposi.w.h = 3026;
        pActwk->yposi.w.h = 120;
        egg1_make_act(pActwk);
    }
    return 1;
}

Sint32 egg1_02(sprite_status *pActwk) {
    Sint16 subact;
    Sint16 egg1acttbl[20] = {2,  0,  4, 0,  6,  0,  8,  5,  10, 6,
                             12, 10, 8, 10, 16, 50, 10, 10, -1, -1};

    subact = ((Sint16 *)pActwk)[26];
    if (!(actwk[subact].actfree[2] & 1))
        return 1;
    actwk[subact].actfree[2] &= 254;
    if (pActwk->r_no1 == 2) {

        pActwk->mstno.w = 0;
        pActwk->patno = 0;
        pActwk->patcnt = 0;
        pActwk->pattim = 0;
        pActwk->pattimm = 0;

        pActwk->actfree[1] = 3;
        pActwk->actfree[2] |= 8;
        egg1coli_set(pActwk);
        subact = ((Sint16 *)pActwk)[26];
    }

    pActwk->r_no1 += 2;
    actwk[subact].actfree[2] &= 253;
    while (egg1acttbl[pActwk->r_no1] < 0)
        pActwk->r_no1 = 6;
    actwk[subact].r_no0 = egg1acttbl[pActwk->r_no1];
    actwk[subact].actfree[3] = egg1acttbl[pActwk->r_no1 + 1];
    return 1;
}

Sint32 egg1_03(sprite_status *pActwk) {
    Sint16 subact;

    ++pActwk->actfree[1];

    bom_set(pActwk);

    if (pActwk->actfree[1] == 94) {

        ((Sint16 *)pActwk)[33] = pActwk->xposi.w.h;
        pActwk->yspeed.w = pActwk->yposi.w.h;

        pActwk->mstno.b.h = 3;
        pActwk->patno = 0;
        pActwk->patcnt = 0;
        pActwk->pattim = 0;
        pActwk->pattimm = 0;
        subact = ((Sint16 *)pActwk)[26];
        actwk[subact].actfree[2] |= 1;
    }
    if (pActwk->actfree[1] >= 120) {
        pActwk->actfree[1] = 0;
        pActwk->r_no0 = 8;
        pActwk->sprhsize = 32;
        pActwk->sprvsize = 32;
        scoreup(100);
    }
    return 1;
}

Sint32 egg1_04(sprite_status *pActwk) {
    Sint32 ret = 1;
    Uint16 sin, cos;
    int_union sinl, cosl;
    Sint16 xposi_bak, yposi_bak;

    if (pActwk->r_no1 != 0) {
        pActwk->yposi.w.h -= ((Sint16 *)pActwk)[31];

        pActwk->actfree[1] += 3;
        sinset(pActwk->actfree[1], (Sint16 *)&sin, (Sint16 *)&cos);
        sinl.l = sin;
        cosl.l = cos;
        sinl.w.l /= 32;
        ((Sint16 *)pActwk)[31] = sinl.w.l;
        pActwk->yposi.w.h += sinl.w.l;

        pActwk->xposi.l += 163840;
        if (pActwk->xposi.w.h >= 3200) {

            pActwk->actfree[1] = 0;
            if (generate_flag == 0)
                sub_sync(17);
            else
                sub_sync(16);
            genecolor();

            bossflag = 0;
            bossstart = 0;
            pActwk->r_no0 = 10;
            ret = egg1_05(pActwk);
        }
    } else {

        ++pActwk->actfree[1];

        xposi_bak = pActwk->xposi.w.h;
        yposi_bak = pActwk->yposi.w.h;
        pActwk->xposi.w.h = ((Sint16 *)pActwk)[33];
        pActwk->yposi.w.h = pActwk->yspeed.w;
        bom_set(pActwk);
        pActwk->xposi.w.h = xposi_bak;
        pActwk->yposi.w.h = yposi_bak;

        pActwk->xposi.l += 32768;
        pActwk->yposi.l -= 0x20000;
        if (pActwk->yposi.w.h <= 344) {
            ++pActwk->r_no1;
            pActwk->patno = 0;
            pActwk->patcnt = 0;
            pActwk->pattim = 0;
            pActwk->pattimm = 0;
            pActwk->mstno.b.h = 4;
            pActwk->actfree[1] = 64;
            ((Sint16 *)pActwk)[31] = 8;
        }
    }
    return ret;
}

Sint32 egg1_05(sprite_status *pActwk) {
    Sint16 temp0, temp1;
    Sint32 ret = 1;

    temp0 = scr_dir_tbl[1];
    temp1 = scr_dir_tbl[2];
    scralim_right += 6;
    scralim_n_right += 6;
    if (temp1 > scralim_right)
        ret = 0;
    else {
        scralim_right = temp1;
        scralim_n_right = temp1;
        ret = 0;
        frameout(pActwk);
    }
    return ret;
}

void egg1body(sprite_status *pActwk) {
    Sint32 (*tbl[9])(sprite_status *) = {
        &egg1body_ini, &egg1body_01, &egg1body_02, &egg1body_03, &egg1body_04,
        &egg1body_05,  &egg1body_06, &egg1body_07, &egg1body_08};

    if (tbl[pActwk->r_no0 / 2](pActwk) == 0)
        return;
    actionsub(pActwk);
}

void spd_normal(sprite_status *pActwk) {
    Sint16 subact1, subact2;

    subact1 = ((Sint16 *)pActwk)[26];
    actwk[subact1].actfree[21] = 2;
    subact1 = ((Sint16 *)&actwk[subact1])[26];
    ((Sint32 *)&actwk[subact1])[16] = 32768;
    sprite_status_set_xspeed_yspeed(&actwk[subact1], 16384);
    subact1 = ((Sint16 *)&actwk[subact1])[26];
    ((Sint32 *)&actwk[subact1])[16] = 16384;
    sprite_status_set_xspeed_yspeed(&actwk[subact1], 32768);

    subact1 = ((Sint16 *)pActwk)[27];
    actwk[subact1].actfree[21] = 2;
    subact1 = ((Sint16 *)&actwk[subact1])[26];
    ((Sint32 *)&actwk[subact1])[16] = 32768;
    sprite_status_set_xspeed_yspeed(&actwk[subact1], 16384);
    subact1 = ((Sint16 *)&actwk[subact1])[26];
    ((Sint32 *)&actwk[subact1])[16] = 16384;
    sprite_status_set_xspeed_yspeed(&actwk[subact1], 32768);

    subact1 = ((Sint16 *)pActwk)[25];
    subact2 = ((Sint16 *)&actwk[subact1])[27];
    subact1 = ((Sint16 *)&actwk[subact2])[27];
    if (subact1) {
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].actfree[2] &= 127;
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].actfree[2] &= 127;
    }

    subact1 = ((Sint16 *)&actwk[subact2])[26];
    if (subact1) {
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].actfree[2] &= 127;
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].actfree[2] &= 127;
    }
}

Sint32 egg1body_ini(sprite_status *pActwk) {
    pActwk->cddat = 0;
    pActwk->r_no0 = 2;
    pActwk->actflg = 4;
    pActwk->sprhsize = 36;
    pActwk->sprvsize = 32;
    pActwk->sproffset = 857;
    pActwk->patbase = egg1body_pat;
    spd_normal(pActwk);
    return 1;
}

Sint32 egg1body_08(sprite_status *pActwk) {
    --pActwk->actfree[3];
    if (!pActwk->actfree[3]) {
        spd_normal(pActwk);
        pActwk->actfree[2] |= 1;
        pActwk->actfree[2] &= 253;
    }
    return 1;
}

void bup_set(sprite_status *legActwk) {
    Sint16 subact;

    legActwk->actfree[2] |= 32;
    subact = ((Sint16 *)legActwk)[26];
    actwk[subact].actfree[2] |= 32;
    subact = ((Sint16 *)&actwk[subact])[26];
    actwk[subact].actfree[2] |= 32;
}

void bup_clr(sprite_status *legActwk) {
    Sint16 subact;

    legActwk->actfree[2] &= 223;
    subact = ((Sint16 *)legActwk)[26];
    actwk[subact].actfree[2] &= 223;
    subact = ((Sint16 *)&actwk[subact])[26];
    actwk[subact].actfree[2] &= 223;
}

void grd_set(sprite_status *legActwk) {
    Sint16 subact;

    legActwk->actfree[2] |= 16;
    subact = ((Sint16 *)legActwk)[26];
    actwk[subact].actfree[2] |= 16;
    subact = ((Sint16 *)&actwk[subact])[26];
    actwk[subact].actfree[2] |= 16;
}

void grd_clr(sprite_status *legActwk) {
    Sint16 subact;

    legActwk->actfree[2] &= 239;
    subact = ((Sint16 *)legActwk)[26];
    actwk[subact].actfree[2] &= 239;
    subact = ((Sint16 *)&actwk[subact])[26];
    actwk[subact].actfree[2] &= 239;
}

Sint32 egg1body_01(sprite_status *pActwk) {
    Sint16 subact;

    subact = ((Sint16 *)pActwk)[27];
    actwk[subact].r_no0 = 16;
    subact = ((Sint16 *)&actwk[subact])[26];
    actwk[subact].r_no0 = 4;
    subact = ((Sint16 *)&actwk[subact])[26];
    actwk[subact].r_no0 = 10;

    subact = ((Sint16 *)pActwk)[26];
    bup_set(&actwk[subact]);
    if (!(actwk[subact].actfree[2] & 16)) {
        pActwk->yposi.l += 98304;
        subact = ((Sint16 *)pActwk)[25];
        actwk[subact].yposi.l += 98304;
    } else
        pActwk->actfree[2] |= 1;
    return 1;
}

Sint32 egg1body_02(sprite_status *pActwk) {
    Sint16 subact;

    subact = ((Sint16 *)pActwk)[27];
    actwk[subact].actfree[2] &= 254;
    subact = ((Sint16 *)pActwk)[26];
    if (!(actwk[subact].actfree[2] & 1)) {
        actwk[subact].actfree[2] &= 254;
        return 1;
    }
    actwk[subact].actfree[2] &= 254;
    if (actwk[subact].r_no0 == 12) {

        actwk[subact].r_no0 = 10;
        return 1;
    }
    if (actwk[subact].r_no0 == 10) {

        actwk[subact].r_no0 = 14;
        return 1;
    }

    actwk[subact].r_no0 = 2;
    bup_clr(&actwk[subact]);
    grd_clr(&actwk[subact]);

    subact = ((Sint16 *)pActwk)[27];
    actwk[subact].r_no0 = 8;
    bup_set(&actwk[subact]);
    grd_set(&actwk[subact]);
    subact = ((Sint16 *)&actwk[subact])[26];
    actwk[subact].r_no0 = 2;
    subact = ((Sint16 *)&actwk[subact])[26];
    actwk[subact].r_no0 = 4;

    pActwk->actfree[2] |= 1;
    return 1;
}

Sint32 egg1body_03(sprite_status *pActwk) {
    Sint16 subact;

    if (!(pActwk->actfree[2] & 2)) {
        pActwk->actfree[2] |= 2;
        armset_2(pActwk);
    }
    subact = ((Sint16 *)pActwk)[26];
    if (!(actwk[subact].actfree[2] & 1))
        return 1;
    subact = ((Sint16 *)pActwk)[27];
    if (!(actwk[subact].actfree[2] & 1))
        return 1;

    pActwk->actfree[2] &= 253;
    pActwk->actfree[2] |= 64;
    pActwk->actfree[2] |= 1;
    return 1;
}

Sint32 egg1body_04(sprite_status *pActwk) {
    Sint16 subact1;
    Sint16 subact2;
    Sint16 subact3;
    Sint16 subact4;

    if (!(pActwk->actfree[2] & 64)) {

        subact1 = ((Sint16 *)pActwk)[26];
        subact2 = ((Sint16 *)pActwk)[27];
    } else {

        subact1 = ((Sint16 *)pActwk)[27];
        subact2 = ((Sint16 *)pActwk)[26];
    }
    if (!(actwk[subact1].actfree[2] & 1))
        return 1;
    if (!(actwk[subact2].actfree[2] & 1))
        return 1;

    subact3 = ((Sint16 *)&actwk[subact1])[26];
    subact3 = ((Sint16 *)&actwk[subact3])[26];
    subact4 = ((Sint16 *)&actwk[subact2])[26];
    actwk[subact1].actfree[2] &= 254;
    actwk[subact3].actfree[2] &= 254;
    actwk[subact2].actfree[2] &= 254;
    actwk[subact4].actfree[2] &= 254;

    if (pActwk->xposi.w.h <= 2904) {

        pActwk->actfree[3] = 1;
    }

    --pActwk->actfree[3];
    if (!pActwk->actfree[3]) {
        pActwk->actfree[2] |= 1;
        pActwk->actfree[2] &= 253;
        return 1;
    }

    if (pActwk->actfree[2] & 64) {
        pActwk->actfree[2] ^= 64;

        subact1 = ((Sint16 *)pActwk)[26];
        actwk[subact1].r_no0 = 8;
        bup_set(&actwk[subact1]);
        grd_set(&actwk[subact1]);
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].r_no0 = 6;

        subact1 = ((Sint16 *)pActwk)[27];
        actwk[subact1].r_no0 = 2;
        bup_clr(&actwk[subact1]);
        grd_clr(&actwk[subact1]);
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].r_no0 = 8;

        armset_1(pActwk);
    } else {
        pActwk->actfree[2] ^= 64;

        subact1 = ((Sint16 *)pActwk)[26];
        actwk[subact1].r_no0 = 2;
        bup_clr(&actwk[subact1]);
        grd_clr(&actwk[subact1]);
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].r_no0 = 8;

        subact1 = ((Sint16 *)pActwk)[27];
        actwk[subact1].r_no0 = 8;
        bup_set(&actwk[subact1]);
        grd_set(&actwk[subact1]);
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].r_no0 = 6;

        armset_2(pActwk);
    }
    return 1;
}

void armset_1(sprite_status *pActwk) {
    Sint16 subact1, subact2;

    subact1 = ((Sint16 *)pActwk)[25];
    subact2 = ((Sint16 *)&actwk[subact1])[27];
    subact1 = ((Sint16 *)&actwk[subact2])[27];
    if (subact1) {
        actwk[subact1].r_no0 = 2;
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].r_no0 = 8;
        actwk[subact1].patno = 0;
    }

    subact1 = ((Sint16 *)&actwk[subact2])[26];
    if (subact1) {
        actwk[subact1].r_no0 = 6;
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].r_no0 = 6;
        actwk[subact1].patno = 0;
    }
}

void armset_2(sprite_status *pActwk) {
    Sint16 subact1, subact2;

    subact1 = ((Sint16 *)pActwk)[25];
    subact2 = ((Sint16 *)&actwk[subact1])[27];
    subact1 = ((Sint16 *)&actwk[subact2])[27];
    if (subact1) {
        actwk[subact1].r_no0 = 6;
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].r_no0 = 6;
        actwk[subact1].patno = 0;
    }

    subact1 = ((Sint16 *)&actwk[subact2])[26];
    if (subact1) {
        actwk[subact1].r_no0 = 2;
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].r_no0 = 8;
        actwk[subact1].patno = 0;
    }
}

void armset_3(sprite_status *pActwk) {
    Sint16 subact1, subact2;

    subact1 = ((Sint16 *)pActwk)[25];
    subact2 = ((Sint16 *)&actwk[subact1])[27];
    subact1 = ((Sint16 *)&actwk[subact2])[27];
    if (subact1) {
        actwk[subact1].r_no0 = 2;
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].r_no0 = 10;
        actwk[subact1].patno = 1;
    }

    subact1 = ((Sint16 *)&actwk[subact2])[26];
    if (subact1) {
        actwk[subact1].r_no0 = 2;
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].r_no0 = 10;
        actwk[subact1].patno = 1;
    }
}

void spd_up(sprite_status *pActwk) {
    Sint16 subact1, subact2;

    subact1 = ((Sint16 *)pActwk)[26];
    actwk[subact1].actfree[21] = 8;
    subact1 = ((Sint16 *)&actwk[subact1])[26];
    ((Sint32 *)&actwk[subact1])[16] = 65536;
    sprite_status_set_xspeed_yspeed(&actwk[subact1], 32768);
    subact1 = ((Sint16 *)&actwk[subact1])[26];
    ((Sint32 *)&actwk[subact1])[16] = 49152;
    sprite_status_set_xspeed_yspeed(&actwk[subact1], 98304);

    subact1 = ((Sint16 *)pActwk)[27];
    actwk[subact1].actfree[21] = 8;
    subact1 = ((Sint16 *)&actwk[subact1])[26];
    ((Sint32 *)&actwk[subact1])[16] = 65536;
    sprite_status_set_xspeed_yspeed(&actwk[subact1], 32768);
    subact1 = ((Sint16 *)&actwk[subact1])[26];
    ((Sint32 *)&actwk[subact1])[16] = 49152;
    sprite_status_set_xspeed_yspeed(&actwk[subact1], 98304);

    subact1 = ((Sint16 *)pActwk)[25];
    subact2 = ((Sint16 *)&actwk[subact1])[27];
    subact1 = ((Sint16 *)&actwk[subact2])[27];
    if (subact1) {
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].actfree[2] |= 128;
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].actfree[2] |= 128;
    }

    subact1 = ((Sint16 *)&actwk[subact2])[26];
    if (subact1) {
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].actfree[2] |= 128;
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].actfree[2] |= 128;
    }
}

Sint32 egg1body_05(sprite_status *pActwk) {
    Sint16 subact1;
    Sint16 subact2;
    Sint16 subact3;
    Sint16 subact4;

    if (!(pActwk->actfree[2] & 64)) {

        subact1 = ((Sint16 *)pActwk)[26];
        subact2 = ((Sint16 *)pActwk)[27];
    } else {

        subact1 = ((Sint16 *)pActwk)[27];
        subact2 = ((Sint16 *)pActwk)[26];
    }

    if (!(actwk[subact1].actfree[2] & 1))
        return 1;
    if (!(actwk[subact2].actfree[2] & 1))
        return 1;

    if (!(pActwk->actfree[2] & 2)) {
        pActwk->actfree[2] |= 2;
        spd_normal(pActwk);
    } else {

        subact3 = ((Sint16 *)&actwk[subact1])[26];
        subact3 = ((Sint16 *)&actwk[subact3])[26];
        subact4 = ((Sint16 *)&actwk[subact2])[26];
        actwk[subact1].actfree[2] &= 254;
        actwk[subact3].actfree[2] &= 254;
        actwk[subact2].actfree[2] &= 254;
        actwk[subact4].actfree[2] &= 254;

        if (pActwk->xposi.w.h >= 2976)
            pActwk->actfree[3] = 1;

        --pActwk->actfree[3];
        if (!pActwk->actfree[3]) {
            pActwk->actfree[2] &= 253;
            pActwk->actfree[2] |= 1;
            return 1;
        }
    }

    if (pActwk->actfree[2] & 64) {
        pActwk->actfree[2] ^= 64;

        subact1 = ((Sint16 *)pActwk)[26];
        actwk[subact1].r_no0 = 22;
        bup_set(&actwk[subact1]);
        grd_set(&actwk[subact1]);
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].r_no0 = 8;

        subact1 = ((Sint16 *)pActwk)[27];
        actwk[subact1].r_no0 = 18;
        bup_clr(&actwk[subact1]);
        grd_clr(&actwk[subact1]);
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].r_no0 = 6;

        armset_2(pActwk);
    } else {
        pActwk->actfree[2] ^= 64;

        subact1 = ((Sint16 *)pActwk)[26];
        actwk[subact1].r_no0 = 18;
        bup_clr(&actwk[subact1]);
        grd_clr(&actwk[subact1]);
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].r_no0 = 6;

        subact1 = ((Sint16 *)pActwk)[27];
        actwk[subact1].r_no0 = 22;
        bup_set(&actwk[subact1]);
        grd_set(&actwk[subact1]);
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].r_no0 = 8;

        armset_1(pActwk);
    }
    return 1;
}

Sint32 egg1body_06(sprite_status *pActwk) {
    Sint16 subact1;
    Sint16 subact2;
    Sint16 subact3;
    Sint16 subact4;

    if (!(pActwk->actfree[2] & 64)) {

        subact1 = ((Sint16 *)pActwk)[26];
        subact2 = ((Sint16 *)pActwk)[27];
    } else {

        subact1 = ((Sint16 *)pActwk)[27];
        subact2 = ((Sint16 *)pActwk)[26];
    }

    if (!(pActwk->actfree[2] & 2)) {
        pActwk->actfree[2] |= 2;
        spd_up(pActwk);
    }
    if (actwk[subact1].r_no0 == 4) {

        subact3 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact3].r_no0 = 10;
        subact3 = ((Sint16 *)&actwk[subact2])[26];
        actwk[subact3].r_no0 = 12;
    }

    if (!(actwk[subact1].actfree[2] & 1))
        return 1;

    if (!(actwk[subact2].actfree[2] & 1))
        return 1;

    subact3 = ((Sint16 *)&actwk[subact1])[26];
    subact3 = ((Sint16 *)&actwk[subact3])[26];
    subact4 = ((Sint16 *)&actwk[subact2])[26];
    actwk[subact1].actfree[2] &= 254;
    actwk[subact3].actfree[2] &= 254;
    actwk[subact2].actfree[2] &= 254;
    actwk[subact4].actfree[2] &= 254;

    --pActwk->actfree[3];
    if (!pActwk->actfree[3]) {
        pActwk->actfree[2] |= 1;
        pActwk->actfree[2] &= 253;
        return 1;
    }

    if (pActwk->actfree[2] & 64) {
        pActwk->actfree[2] ^= 64;

        subact1 = ((Sint16 *)pActwk)[26];
        actwk[subact1].actfree[2] &= 253;
        bup_set(&actwk[subact1]);
        grd_set(&actwk[subact1]);
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].r_no0 = 10;
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].r_no0 = 2;

        subact1 = ((Sint16 *)pActwk)[27];
        actwk[subact1].actfree[2] &= 253;
        actwk[subact1].r_no0 = 2;
        bup_clr(&actwk[subact1]);
        grd_clr(&actwk[subact1]);
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].r_no0 = 12;

        armset_3(pActwk);

    } else {
        pActwk->actfree[2] ^= 64;

        subact1 = ((Sint16 *)pActwk)[26];
        actwk[subact1].actfree[2] &= 253;
        actwk[subact1].r_no0 = 2;
        bup_clr(&actwk[subact1]);
        grd_clr(&actwk[subact1]);
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].r_no0 = 12;

        subact1 = ((Sint16 *)pActwk)[27];
        actwk[subact1].actfree[2] &= 253;
        bup_set(&actwk[subact1]);
        grd_set(&actwk[subact1]);
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].r_no0 = 10;
        subact1 = ((Sint16 *)&actwk[subact1])[26];
        actwk[subact1].r_no0 = 2;

        armset_3(pActwk);
    }

    return 1;
}

Sint32 egg1body_07(sprite_status *pActwk) {
    Sint32 ret = 1;
    Sint16 subact;

    if (!(pActwk->actfree[2] & 1)) {
        if (emycol_d(pActwk) > 0) {
            subact = ((Sint16 *)pActwk)[25];
            pActwk->yposi.w.h += 2;
            actwk[subact].yposi.w.h += 2;
        }
    } else {

        ret = 0;
        frameout(pActwk);
    }
    return ret;
}

void arm_bom(sprite_status *pActwk) {
    sprite_status *subActwk;

    if (actwkchk(&subActwk) == 0) {
        subActwk->r_no1 = 255;
        subActwk->actno = 24;
        subActwk->xposi.w.h = pActwk->xposi.w.h;
        subActwk->yposi.w.h = pActwk->yposi.w.h;
        soundset(158);
    }
}

void egg1arm1(sprite_status *pActwk) {
    Sint32 (*tbl[3])(sprite_status *) = {&egg1arm1_ini, &egg1arm1_01,
                                         &egg1arm1_02};

    if (tbl[pActwk->r_no0 / 2](pActwk) == 0)
        return;
    actionsub(pActwk);
}

Sint32 egg1arm1_ini(sprite_status *pActwk) {
    Sint32 ret = 1;

    pActwk->cddat = 0;
    pActwk->actflg = 4;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 12;
    pActwk->sproffset = 9049;
    pActwk->patbase = egg1arm1_pat;
    pActwk->r_no0 = 2;
    ret = egg1arm1_01(pActwk);
    return ret;
}

Sint32 egg1arm1_01(sprite_status *pActwk) {
    Sint16 subact;

    subact = ((Sint16 *)pActwk)[25];
    pActwk->xposi.w.h = actwk[subact].xposi.w.h + 24;
    pActwk->yposi.w.h = actwk[subact].yposi.w.h - 12;

    if (pActwk->actfree[2] & 64) {
        pActwk->actfree[2] &= 191;

        subact = ((Sint16 *)pActwk)[26];
        actwk[subact].r_no0 = 10;
        ((Sint16 *)pActwk)[26] = 0;
        subact = ((Sint16 *)pActwk)[27];
        subact = ((Sint16 *)&actwk[subact])[26];
        return 1;
    }
    if (pActwk->actfree[2] & 32) {
        pActwk->actfree[2] &= 223;

        subact = ((Sint16 *)pActwk)[27];
        actwk[subact].r_no0 = 10;
        ((Sint16 *)pActwk)[27] = 0;
        actwk[subact].patno = 0;
        return 1;
    }
    return 1;
}

Sint32 egg1arm1_02(sprite_status *pActwk) {
    Sint32 ret = 1;

    if (!(pActwk->actfree[2] & 2)) {

        ((Sint32 *)pActwk)[16] = -65536;
        sprite_status_set_xspeed_yspeed(pActwk, -0x20000);
    } else {

        ((Sint32 *)pActwk)[16] -= 1536;
        sprite_status_add_xspeed_yspeed(pActwk, 6144);
    }
    pActwk->actfree[2] |= 2;
    pActwk->xposi.l += ((Sint32 *)pActwk)[16];
    pActwk->yposi.l += sprite_status_get_xspeed_yspeed(pActwk);

    if (pActwk->yposi.w.h >= 576) {
        ret = 0;
        frameout(pActwk);
    } else
        tenmetu();
    return ret;
}

void egg1arm2(sprite_status *pActwk) {
    Sint32 (*tbl[6])(sprite_status *) = {&egg1arm2_ini, &egg1arm2_01,
                                         &egg1arm2_02,  &egg1arm2_03,
                                         &egg1arm2_04,  &egg1arm2_05};

    if (tbl[pActwk->r_no0 / 2](pActwk) != 0) {
        if (!(pActwk->actfree[2] & 4))
            actionsub(pActwk);
    }
}

Sint32 egg1arm2_ini(sprite_status *pActwk) {
    pActwk->cddat = 0;
    pActwk->actflg = 4;
    pActwk->sprhsize = 8;
    pActwk->sprvsize = 8;
    pActwk->sproffset = 9049;
    pActwk->patbase = egg1arm2_pat;
    pActwk->r_no0 = 6;
    egg1arm2_01(pActwk);
    return 1;
}

Sint32 egg1arm2_01(sprite_status *pActwk) {
    ((char *)pActwk)[46] -= 2;
    if (((char *)pActwk)[46] > 0)
        egg1arm2_set(pActwk);
    else {
        pActwk->r_no0 = 4;
        ((char *)pActwk)[46] = 0;
        pActwk->actfree[2] |= 1;
        egg1arm2_set(pActwk);
    }
    return 1;
}

Sint32 egg1arm2_03(sprite_status *pActwk) {
    ((char *)pActwk)[46] += 2;
    if (((char *)pActwk)[46] < 48)
        egg1arm2_set(pActwk);
    else {
        pActwk->r_no0 = 8;
        ((char *)pActwk)[46] = 48;
        pActwk->actfree[2] |= 1;
        egg1arm2_set(pActwk);
    }
    return 1;
}

Sint32 egg1arm2_02(sprite_status *pActwk) {
    egg1arm2_set(pActwk);
    return 1;
}

Sint32 egg1arm2_04(sprite_status *pActwk) {
    egg1arm2_set(pActwk);
    return 1;
}

Sint32 egg1arm2_05(sprite_status *pActwk) {
    Sint32 ret = 1;
    Sint16 subact;

    if (!(pActwk->actfree[2] & 2)) {
        pActwk->actfree[2] |= 2;

        ((Sint32 *)pActwk)[16] = 65536;
        sprite_status_set_xspeed_yspeed(pActwk, -0x20000);
        subact = ((Sint16 *)pActwk)[26];
        actwk[subact].r_no0 = 4;
    } else {

        ((Sint32 *)pActwk)[16] += 1536;
        sprite_status_add_xspeed_yspeed(pActwk, 7936);
    }
    pActwk->xposi.l += ((Sint32 *)pActwk)[16];
    pActwk->yposi.l += sprite_status_get_xspeed_yspeed(pActwk);

    if (pActwk->yposi.l >= 576) {
        ret = 0;
        frameout(pActwk);
    } else
        tenmetu();
    return ret;
}

void egg1arm2_set(sprite_status *pActwk) {
    Sint16 subact;
    Uint16 sin, cos;
    int_union sinl, cosl;

    subact = ((Sint16 *)pActwk)[25];
    pActwk->xposi.w.h = actwk[subact].xposi.w.h;
    pActwk->yposi.w.h = actwk[subact].yposi.w.h;

    sinset(pActwk->actfree[0] + 64, (Sint16 *)&sin, (Sint16 *)&cos);
    sinl.l = sin;
    cosl.l = cos;
    sinl.w.l /= 16;
    cosl.w.l /= 16;
    pActwk->xposi.w.h += cosl.w.l;
    pActwk->yposi.w.h += sinl.w.l;

    if (pActwk->actfree[2] & 4)
        pActwk->xposi.w.h -= 10;
}

void egg1arm3(sprite_status *pActwk) {
    Sint32 (*tbl[6])(sprite_status *) = {&egg1arm3_ini, &egg1arm3_01,
                                         &egg1arm3_02,  &egg1arm3_03,
                                         &egg1arm3_04,  &egg1arm3_05};

    if (tbl[pActwk->r_no0 / 2](pActwk) != 0) {
        actionsub(pActwk);
    }
}

Sint32 egg1arm3_ini(sprite_status *pActwk) {
    pActwk->cddat = 0;
    pActwk->actflg = 4;
    pActwk->sprhsize = 32;
    pActwk->sprvsize = 8;
    pActwk->sproffset = 9049;
    pActwk->r_no0 = 2;
    egg1arm3_01(pActwk);
    return 1;
}

Sint32 egg1arm3_01(sprite_status *pActwk) {
    Sint16 subact;

    subact = ((Sint16 *)pActwk)[25];
    pActwk->xposi.w.h = ((Sint16 *)pActwk)[29] + (actwk[subact].xposi.w.h - 36);

    pActwk->yposi.w.h = (Sint32)actwk[subact].yposi.w.h;

    if (!(pActwk->actfree[2] & 128)) {
        if (actwk[0].yposi.w.h <= pActwk->yposi.w.h) {

            if (((Sint16 *)pActwk)[31] > -8)
                ((Sint32 *)pActwk)[15] -= 65536;
        } else {

            if (((Sint16 *)pActwk)[31] < 8)
                ((Sint32 *)pActwk)[15] += 65536;
        }
        pActwk->yposi.w.h += ((Sint16 *)pActwk)[31];
        return 1;
    }
}

Sint32 egg1arm3_03(sprite_status *pActwk) {
    Sint32 ret = 1;

    if (((Sint16 *)pActwk)[29] > 0)
        ((Sint32 *)pActwk)[14] -= 32768;
    ret = egg1arm3_01(pActwk);
    return ret;
}

Sint32 egg1arm3_04(sprite_status *pActwk) {
    Sint32 ret = 1;

    if (((Sint16 *)pActwk)[29] < 16)
        ((Sint32 *)pActwk)[14] += 32768;
    ret = egg1arm3_01(pActwk);
    return ret;
}

Sint32 egg1arm3_05(sprite_status *pActwk) {
    Sint16 subact;

    if (((Sint16 *)pActwk)[29] < 8)
        ((Sint32 *)pActwk)[14] += 32768;
    else
        pActwk->patno = 1;
    subact = ((Sint16 *)pActwk)[25];
    pActwk->xposi.w.h = ((Sint16 *)pActwk)[29] + (actwk[subact].xposi.w.h - 36);
    pActwk->yposi.w.h = (Sint32)actwk[subact].yposi.w.h;

    if (((Sint16 *)pActwk)[31] < 8)
        ((Sint32 *)pActwk)[15] += 65536;
    pActwk->yposi.w.h += ((Sint16 *)pActwk)[31];
    return 1;
}

Sint32 egg1arm3_02(sprite_status *pActwk) {
    Sint32 ret = 1;
    Sint16 subact;

    if (!(pActwk->actfree[2] & 2)) {
        pActwk->actfree[2] |= 2;

        pActwk->actfree[1] = 1;
        pActwk->actfree[0] = 0;
        ((Sint32 *)pActwk)[16] = 0;
        sprite_status_set_xspeed_yspeed(pActwk, 65536);
        subact = ((Sint16 *)pActwk)[26];
        actwk[subact].r_no0 = 4;
    } else {

        ((Sint32 *)pActwk)[16] -= 1568;
        sprite_status_add_xspeed_yspeed(pActwk, 4640);
    }
    pActwk->xposi.l += ((Sint32 *)pActwk)[16];
    pActwk->yposi.l += sprite_status_get_xspeed_yspeed(pActwk);

    if (pActwk->yposi.w.h >= 576) {
        ret = 0;
        frameout(pActwk);
    } else {
        ++pActwk->actfree[0];
        if (((Sint16 *)pActwk)[23] % 7 == 0)
            arm_bom(pActwk);
        tenmetu();
    }
    return ret;
}

void egg1arm4(sprite_status *pActwk) {
    Sint32 (*tbl[3])(sprite_status *) = {&egg1arm4_ini, &egg1arm4_01,
                                         &egg1arm4_02};

    if (tbl[pActwk->r_no0 / 2](pActwk) == 0)
        return;
    actionsub(pActwk);
}

Sint32 egg1arm4_ini(sprite_status *pActwk) {
    pActwk->cddat = 0;
    pActwk->actflg = 4;
    pActwk->sprhsize = 12;
    pActwk->sprvsize = 16;
    pActwk->sproffset = 9049;
    pActwk->patbase = egg1arm4_pat;
    pActwk->r_no0 = 2;
    return 1;
}

Sint32 egg1arm4_01(sprite_status *pActwk) {
    Sint16 subact;

    subact = ((Sint16 *)pActwk)[25];
    pActwk->xposi.w.h = actwk[subact].xposi.w.h - 12;
    pActwk->yposi.w.h = actwk[subact].yposi.w.h;

    if (actwk[subact].patno) {
        if (actwk[subact].patno != 1)
            pActwk->xposi.w.h += 8;
        else
            pActwk->xposi.w.h += 16;
    }

    if (pActwk->actfree[2] & 16)
        pActwk->xposi.w.h -= 4;
    pActwk->actfree[2] &= 239;
    return 1;
}

Sint32 egg1arm4_02(sprite_status *pActwk) {
    Sint32 ret = 1;

    if (!(pActwk->actfree[2] & 2)) {
        pActwk->actfree[2] |= 2;

        pActwk->actfree[1] = 0;

        pActwk->colino = 0;
        pActwk->colicnt = 0;

        ((Sint32 *)pActwk)[16] = 0;
        sprite_status_set_xspeed_yspeed(pActwk, -98304);
    } else {

        ((Sint32 *)pActwk)[16] -= 1280;
        sprite_status_add_xspeed_yspeed(pActwk, 6144);
    }
    pActwk->xposi.l += ((Sint32 *)pActwk)[16];
    pActwk->yposi.l += sprite_status_get_xspeed_yspeed(pActwk);

    if (pActwk->yposi.w.h >= 576) {
        ret = 0;
        frameout(pActwk);
    } else
        tenmetu();
    return ret;
}

void egg1leg1(sprite_status *pActwk) {
    Sint32 (*tbl[13])(sprite_status *) = {
        &egg1leg1_ini, &egg1leg1_01, &egg1leg1_02, &egg1leg1_03, &egg1leg1_04,
        &egg1leg1_05,  &egg1leg1_06, &egg1leg1_07, &egg1leg1_08, &egg1leg1_09,
        &egg1leg1_10,  &egg1leg1_11, &egg1leg1_12};

    if (tbl[pActwk->r_no0 / 2](pActwk) == 0)
        return;
    actionsub(pActwk);
}

Sint32 egg1leg1_ini(sprite_status *pActwk) {
    pActwk->cddat = 0;
    pActwk->actflg = 4;
    pActwk->sprhsize = 8;
    pActwk->sprvsize = 8;
    pActwk->sproffset = 9049;
    pActwk->patbase = egg1leg1_pat;

    pActwk->r_no0 = 12;
    pActwk->actfree[0] = 88;
    pActwk->actfree[21] = 2;
    return 1;
}

Sint32 egg1leg1_01(sprite_status *pActwk) {
    Sint16 subact;

    subact = ((Sint16 *)pActwk)[26];
    subact = ((Sint16 *)&actwk[subact])[26];
    if (actwk[subact].r_no0 != 6) {
        if (actwk[subact].r_no0 != 8) {
            actwk[subact].actfree[2] &= 254;
            actwk[subact].r_no0 = 6;
        }
    }
    if (((char *)pActwk)[46] > 0) {
        pActwk->actfree[0] = pActwk->actfree[0] - ((char *)pActwk)[67];

        if (((char *)pActwk)[46] > 0) {

            egg1leg1_set(pActwk);
            return 1;
        }
    }
    pActwk->actfree[0] = 0;
    if (actwk[subact].r_no0 != 8) {
        if (!(actwk[subact].actfree[2] & 1)) {

            actwk[subact].actfree[2] &= 254;
            egg1leg1_set(pActwk);
            return 1;
        }
        actwk[subact].actfree[2] &= 254;
    }

    pActwk->r_no0 = 4;
    egg1leg1_set(pActwk);
    return 1;
}

Sint32 egg1leg1_02(sprite_status *pActwk) {
    Sint16 subact;

    subact = ((Sint16 *)pActwk)[26];
    subact = ((Sint16 *)&actwk[subact])[26];
    pActwk->actfree[0] = pActwk->actfree[0] + ((char *)pActwk)[67];
    if (((char *)pActwk)[46] >= 88) {

        pActwk->actfree[0] = 88;
        pActwk->r_no0 = 6;
        actwk[subact].r_no0 = 2;
    }
    egg1leg1_set(pActwk);
    return 1;
}

Sint32 egg1leg1_03(sprite_status *pActwk) {
    Sint16 subact;

    subact = ((Sint16 *)pActwk)[26];
    subact = ((Sint16 *)&actwk[subact])[26];
    if (actwk[subact].actfree[2] & 16)
        pActwk->actfree[2] |= 1;
    egg1leg1_set(pActwk);
    return 1;
}

Sint32 egg1leg1_04(sprite_status *pActwk) {
    if (!(pActwk->actfree[2] & 1)) {
        pActwk->actfree[0] = pActwk->actfree[0] - ((char *)pActwk)[67];
        if (((char *)pActwk)[46] < 24) {

            pActwk->actfree[0] = 24;
            pActwk->actfree[2] |= 1;
        }
    }

    egg1leg1_set(pActwk);
    return 1;
}

Sint32 egg1leg1_05(sprite_status *pActwk) {
    Sint16 subact;

    subact = ((Sint16 *)pActwk)[26];
    subact = ((Sint16 *)&actwk[subact])[26];
    if (actwk[subact].r_no0 != 8)
        actwk[subact].r_no0 = 6;
    else {

        if (((char *)pActwk)[46] == 0) {

            if (actwk[subact].actfree[2] & 1) {
                actwk[subact].actfree[2] &= 254;
                pActwk->actfree[2] |= 1;
            } else
                actwk[subact].actfree[2] &= 254;
            egg1leg1_set(pActwk);
        }
    }
    if (((char *)pActwk)[46] != 0)
        pActwk->actfree[0] = pActwk->actfree[0] - ((char *)pActwk)[67];
    egg1leg1_set(pActwk);
    return 1;
}

Sint32 egg1leg1_06(sprite_status *pActwk) {
    Sint16 subact;

    subact = ((Sint16 *)pActwk)[26];
    subact = ((Sint16 *)&actwk[subact])[26];
    if (actwk[subact].r_no0 != 4)
        actwk[subact].r_no0 = 2;
    else {

        if (((char *)pActwk)[46] >= 88) {

            if (actwk[subact].actfree[2] & 1)
                pActwk->actfree[2] |= 1;
            actwk[subact].actfree[2] &= 254;
            egg1leg1_set(pActwk);
            return 1;
        }
    }

    if (((char *)pActwk)[46] < 88)
        pActwk->actfree[0] = pActwk->actfree[0] + ((char *)pActwk)[67];
    egg1leg1_set(pActwk);
    return 1;
}

Sint32 egg1leg1_07(sprite_status *pActwk) {
    Sint32 ret = 1;
    Sint16 subact;

    if (((char *)pActwk)[46] < 24) {

        pActwk->actfree[0] = pActwk->actfree[0] + ((char *)pActwk)[67];
        subact = ((Sint16 *)pActwk)[26];
        subact = ((Sint16 *)&actwk[subact])[26];
        actwk[subact].actfree[2] |= 128;
        subact = ((Sint16 *)pActwk)[25];
        subact = ((Sint16 *)&actwk[subact])[27];
        subact = ((Sint16 *)&actwk[subact])[26];
        subact = ((Sint16 *)&actwk[subact])[26];
        actwk[subact].actfree[2] |= 128;
        ret = egg1leg1_06(pActwk);
        return ret;
    }
    if (((char *)pActwk)[46] > 24) {

        pActwk->actfree[0] = pActwk->actfree[0] - ((char *)pActwk)[67];
        ret = egg1leg1_05(pActwk);
        return ret;
    }

    subact = ((Sint16 *)pActwk)[26];
    subact = ((Sint16 *)&actwk[subact])[26];
    if (actwk[subact].actfree[2] & 1)
        pActwk->actfree[2] |= 1;
    egg1leg1_set(pActwk);
    return ret;
}

Sint32 egg1leg1_08(sprite_status *pActwk) {
    Sint16 subact;

    subact = ((Sint16 *)pActwk)[25];
    subact = ((Sint16 *)&actwk[subact])[26];
    pActwk->xposi.w.h = actwk[subact].xposi.w.h - 10;
    pActwk->yposi.w.h = actwk[subact].yposi.w.h;
    pActwk->actfree[0] = actwk[subact].actfree[0];
    pActwk->actfree[1] = actwk[subact].actfree[1];
    return 1;
}

Sint32 egg1leg1_09(sprite_status *pActwk) {
    Sint16 subact;

    subact = ((Sint16 *)pActwk)[26];
    subact = ((Sint16 *)&actwk[subact])[26];
    if (actwk[subact].r_no0 != 6) {
        if (actwk[subact].r_no0 != 8) {
            actwk[subact].actfree[2] &= 254;
            actwk[subact].r_no0 = 6;
        } else {

            if (((char *)pActwk)[46] != 32) {
                pActwk->actfree[0] = pActwk->actfree[0] - ((char *)pActwk)[67];
                if (pActwk->actfree[0] <= 0) {
                    pActwk->actfree[0] = 32;
                    actwk[subact].actfree[2] &= 254;
                    pActwk->r_no0 = 20;
                }
            } else {

                pActwk->actfree[0] = 32;
                actwk[subact].actfree[2] &= 254;
                pActwk->r_no0 = 20;
            }
        }
    }
    egg1leg1_set(pActwk);
    return 1;
}

Sint32 egg1leg1_10(sprite_status *pActwk) {
    Sint16 subact;

    subact = ((Sint16 *)pActwk)[26];
    subact = ((Sint16 *)&actwk[subact])[26];
    pActwk->actfree[0] = pActwk->actfree[0] + ((char *)pActwk)[67];
    if (((char *)pActwk)[46] >= 48) {

        pActwk->actfree[0] = 48;
        pActwk->r_no0 = 6;
        actwk[subact].r_no0 = 2;
    }
    egg1leg1_set(pActwk);
    return 1;
}

Sint32 egg1leg1_11(sprite_status *pActwk) {
    Sint16 subact;

    if (!(pActwk->actfree[2] & 1)) {
        subact = pActwk->actfree[6];
        subact = actwk[subact].actfree[6];
        if (actwk[subact].r_no0 != 2) {
            if (actwk[subact].r_no0 == 4) {

                pActwk->actfree[0] = pActwk->actfree[0] + ((char *)pActwk)[67];
                if (((char *)pActwk)[46] >= 80) {
                    pActwk->actfree[2] |= 1;
                }
                egg1leg1_set(pActwk);
                return 1;
            }

            actwk[subact].r_no0 = 2;
        }
    }
    egg1leg1_set(pActwk);
    return 1;
}

void egg1leg1_set(sprite_status *pActwk) {
    Uint16 sin, cos;
    Sint16 sin2, cos2;
    int_union sinl, cosl;
    Sint16 subact;

    sinset(pActwk->actfree[0], (Sint16 *)&sin, (Sint16 *)&cos);
    sinl.l = sin;
    cosl.l = cos;
    sinl.w.l /= 16;
    cosl.w.l /= 16;

    if (pActwk->actfree[2] & 16) {
        if (!(pActwk->actfree[2] & 32)) {

            subact = ((Sint16 *)pActwk)[25];
            cos2 = cosl.w.l + (actwk[subact].xposi.w.h + 12);
            if (pActwk->actfree[2] & 4)
                cos2 -= 10;
            pActwk->xposi.w.h = cos2;
            pActwk->yposi.w.h = sinl.w.l + (actwk[subact].yposi.w.h + 20);
        } else {
            if (!(pActwk->actfree[2] & 2)) {
                pActwk->actfree[2] |= 2;
                ((Sint16 *)pActwk)[29] = cosl.w.l;
                ((Sint16 *)pActwk)[31] = sinl.w.l;
            }

            cos2 = cosl.w.l - ((Sint16 *)pActwk)[29];
            sin2 = sinl.w.l - ((Sint16 *)pActwk)[31];

            ((Sint16 *)pActwk)[29] = cosl.w.l;
            ((Sint16 *)pActwk)[31] = sinl.w.l;

            subact = ((Sint16 *)pActwk)[25];
            actwk[subact].xposi.w.h -= cos2;
            actwk[subact].yposi.w.h -= sin2;
            subact = ((Sint16 *)&actwk[subact])[25];
            actwk[subact].xposi.w.h -= cos2;
            actwk[subact].yposi.w.h -= sin2;
        }
    } else {
        pActwk->actfree[2] &= 253;

        subact = ((Sint16 *)pActwk)[25];
        cos2 = cosl.w.l + (actwk[subact].xposi.w.h + 12);
        if (pActwk->actfree[2] & 4)
            cos2 -= 10;
        pActwk->xposi.w.h = cos2;
        pActwk->yposi.w.h = sinl.w.l + (actwk[subact].yposi.w.h + 20);
    }
}

Sint32 egg1leg1_12(sprite_status *pActwk) {
    Sint32 ret = 1;

    if (!(pActwk->actfree[2] & 2)) {
        pActwk->actfree[2] |= 2;
        if (pActwk->actfree[2] & 16) {
            ((Sint32 *)pActwk)[16] = 0;
            sprite_status_set_xspeed_yspeed(pActwk, -163840);
        } else {
            ((Sint32 *)pActwk)[16] = 0;
            sprite_status_set_xspeed_yspeed(pActwk, -163840);
        }
    } else {

        if (pActwk->actfree[2] & 16) {
            ((Sint32 *)pActwk)[16] = ((Sint32 *)pActwk)[16] - 1536;
            sprite_status_add_xspeed_yspeed(pActwk, 6240);
        } else {
            ((Sint32 *)pActwk)[16] = ((Sint32 *)pActwk)[16] + 1536;
            sprite_status_add_xspeed_yspeed(pActwk, 6240);
        }
    }

    pActwk->xposi.l += ((Sint32 *)pActwk)[16];
    pActwk->yposi.l += sprite_status_get_xspeed_yspeed(pActwk);

    if (pActwk->yposi.w.h >= 576) {
        ret = 0;
        frameout(pActwk);
    } else
        tenmetu();
    return ret;
}

void egg1leg2(sprite_status *pActwk) {
    Sint32 (*tbl[8])(sprite_status *) = {
        &egg1leg2_ini, &egg1leg2_01, &egg1leg2_02, &egg1leg2_03,
        &egg1leg2_04,  &egg1leg2_05, &egg1leg2_06, &egg1leg2_07};

    if (tbl[pActwk->r_no0 / 2](pActwk) == 0)
        return;
    actionsub(pActwk);
}

Sint32 egg1leg2_ini(sprite_status *pActwk) {
    pActwk->cddat = 0;
    pActwk->r_no0 = 2;
    pActwk->actflg = 4;
    pActwk->sprvsize = 8;
    pActwk->sprhsize = 20;
    pActwk->sproffset = 9049;
    pActwk->patbase = egg1leg2_pat;
    ((Sint32 *)pActwk)[16] = 32768;
    sprite_status_set_xspeed_yspeed(pActwk, 16384);
    return 1;
}

Sint32 egg1leg2_01(sprite_status *pActwk) {
    Sint16 subact;

    subact = ((Sint16 *)pActwk)[25];
    pActwk->xposi.w.h = actwk[subact].xposi.w.h + 4;
    pActwk->yposi.w.h = actwk[subact].yposi.w.h + 16;
    return 1;
}

Sint32 egg1leg2_02(sprite_status *pActwk) {
    Sint16 subact;

    subact = ((Sint16 *)pActwk)[25];
    subact = ((Sint16 *)&actwk[subact])[25];
    subact = ((Sint16 *)&actwk[subact])[26];
    subact = ((Sint16 *)&actwk[subact])[26];
    pActwk->xposi.w.h = actwk[subact].xposi.w.h - 10;
    pActwk->yposi.w.h = actwk[subact].yposi.w.h;
    return 1;
}

Sint32 egg1leg2_03(sprite_status *pActwk) {
    Sint32 xs;

    if (((Sint16 *)pActwk)[29] < 8) {
        ((Sint32 *)pActwk)[14] =
            ((Sint32 *)pActwk)[14] + ((Sint32 *)pActwk)[16];
        if (pActwk->actfree[2] & 16) {
            xs = ((Sint32 *)pActwk)[16];
            xs = -xs;
            leg2_set(xs, 0, pActwk);
            return 1;
        }
    }
    leg2_set2(pActwk);
    return 1;
}

Sint32 egg1leg2_04(sprite_status *pActwk) {
    ((Sint16 *)pActwk)[31] = 0;
    if (((Sint16 *)pActwk)[29] > -8) {
        ((Sint32 *)pActwk)[14] =
            ((Sint32 *)pActwk)[14] - ((Sint32 *)pActwk)[16];
        if (pActwk->actfree[2] & 16) {
            leg2_set(((Sint32 *)pActwk)[16], 0, pActwk);
            return 1;
        }
    }
    leg2_set2(pActwk);
    return 1;
}

Sint32 egg1leg2_05(sprite_status *pActwk) {
    Sint32 xs, ys;

    if (((Sint16 *)pActwk)[29] > -8) {
        ((Sint32 *)pActwk)[14] =
            ((Sint32 *)pActwk)[14] - ((Sint32 *)pActwk)[16];
        xs = ((Sint32 *)pActwk)[16];
    } else
        xs = 0;
    if (((Sint16 *)pActwk)[31] > -4) {
        ((Sint32 *)pActwk)[15] =
            ((Sint32 *)pActwk)[15] - sprite_status_get_xspeed_yspeed(pActwk);
        ys = sprite_status_get_xspeed_yspeed(pActwk);
    } else
        ys = 0;
    if (pActwk->actfree[2] & 16)
        leg2_set(xs, ys, pActwk);
    else
        leg2_set2(pActwk);
    return 1;
}

Sint32 egg1leg2_06(sprite_status *pActwk) {
    Sint32 xs, ys;

    if (((Sint16 *)pActwk)[29] > -8) {
        ((Sint32 *)pActwk)[14] =
            ((Sint32 *)pActwk)[14] - ((Sint32 *)pActwk)[16];
        xs = ((Sint32 *)pActwk)[16];
    } else
        xs = 0;
    if (((Sint16 *)pActwk)[31] < 4) {
        ((Sint32 *)pActwk)[15] =
            ((Sint32 *)pActwk)[15] + sprite_status_get_xspeed_yspeed(pActwk);
        ys = sprite_status_get_xspeed_yspeed(pActwk);
    } else
        ys = 0;
    if (pActwk->actfree[2] & 16) {
        ys = -ys;
        leg2_set(xs, ys, pActwk);
    } else
        leg2_set2(pActwk);
    return 1;
}

void leg2_set(Sint32 xs, Sint32 ys, sprite_status *pActwk) {
    Sint16 subact;

    if (pActwk->actfree[2] & 32) {
        subact = ((Sint16 *)pActwk)[25];
        actwk[subact].xposi.l += xs;
        actwk[subact].yposi.l += ys;
        subact = ((Sint16 *)&actwk[subact])[25];
        actwk[subact].xposi.l += xs;
        actwk[subact].yposi.l += ys;
        subact = ((Sint16 *)&actwk[subact])[25];
        actwk[subact].xposi.l += xs;
        actwk[subact].yposi.l += ys;
    }
}

void leg2_set2(sprite_status *pActwk) {
    Sint16 subact;

    subact = ((Sint16 *)pActwk)[25];
    pActwk->xposi.w.h = ((Sint16 *)pActwk)[29] + (actwk[subact].xposi.w.h + 4);
    pActwk->yposi.w.h = ((Sint16 *)pActwk)[31] + (actwk[subact].yposi.w.h + 16);
}

Sint32 egg1leg2_07(sprite_status *pActwk) {
    Sint32 ret = 1;

    if (!(pActwk->actfree[2] & 2)) {

        pActwk->actfree[2] |= 2;
        if (!(pActwk->actfree[2] & 16)) {
            ((Sint32 *)pActwk)[16] = 0;
            sprite_status_set_xspeed_yspeed(pActwk, -0x20000);
        } else {
            ((Sint32 *)pActwk)[16] = 0;
            sprite_status_set_xspeed_yspeed(pActwk, -0x20000);
        }
    } else {

        if (!(pActwk->actfree[2] & 16)) {
            ((Sint32 *)pActwk)[16] -= 1536;
            sprite_status_add_xspeed_yspeed(pActwk, 6752);
        } else {
            ((Sint32 *)pActwk)[16] += 1536;
            sprite_status_add_xspeed_yspeed(pActwk, 6752);
        }
    }
    pActwk->xposi.l += ((Sint32 *)pActwk)[16];
    pActwk->yposi.l += sprite_status_get_xspeed_yspeed(pActwk);
    if (pActwk->yposi.w.h >= 576) {
        ret = 0;
        frameout(pActwk);
    } else
        tenmetu();
    return ret;
}

void egg1leg3(sprite_status *pActwk) {
    Sint32 (*tbl[7])(sprite_status *) = {
        &egg1leg3_ini, &egg1leg3_01, &egg1leg3_02, &egg1leg3_03,
        &egg1leg3_04,  &egg1leg3_05, &egg1leg3_06};

    if (tbl[pActwk->r_no0 / 2](pActwk) == 0)
        return;
    actionsub(pActwk);
}

void leg3_set_not_grd(sprite_status *pActwk) {
    Sint16 subact;

    subact = ((Sint16 *)pActwk)[25];
    pActwk->xposi.w.h = ((Sint16 *)pActwk)[29] + (actwk[subact].xposi.w.h - 11);
    pActwk->yposi.w.h = ((Sint16 *)pActwk)[31] + (actwk[subact].yposi.w.h + 14);
}

Sint32 egg1leg3_ini(sprite_status *pActwk) {
    pActwk->cddat = 0;
    pActwk->actflg = 4;
    pActwk->sprhsize = 32;
    pActwk->sprvsize = 20;
    pActwk->sproffset = 9049;
    pActwk->patbase = egg1leg3_pat;

    ((Sint32 *)pActwk)[16] = 16384;
    sprite_status_set_xspeed_yspeed(pActwk, 32768);

    pActwk->r_no0 = 4;
    pActwk->actfree[2] |= 1;
    ((Sint16 *)pActwk)[29] = -8;
    ((Sint16 *)pActwk)[31] = 16;
    leg3_set_not_grd(pActwk);
    return 1;
}

void leg3_on_the_ground(sprite_status *pActwk) {
    Sint16 subact;

    pActwk->actfree[2] |= 16;
    subact = ((Sint16 *)pActwk)[25];
    actwk[subact].actfree[2] |= 16;
    subact = ((Sint16 *)&actwk[subact])[25];
    actwk[subact].actfree[2] |= 16;

    subact = ((Sint16 *)&actwk[subact])[25];
    subact = ((Sint16 *)&actwk[subact])[25];
    actwk[subact].actfree[11] = 8;
}

Sint32 egg1leg3_01(sprite_status *pActwk) {
    ((Sint32 *)pActwk)[14] = ((Sint32 *)pActwk)[14] - ((Sint32 *)pActwk)[16];
    ((Sint32 *)pActwk)[15] =
        ((Sint32 *)pActwk)[15] + sprite_status_get_xspeed_yspeed(pActwk);

    if (pActwk->actfree[2] & 16) {
        if (pActwk->actfree[2] & 128) {

            if (((Sint16 *)pActwk)[31] < 12) {

                leg3_set(pActwk);
                if (0 >= emycol_d(pActwk)) {
                    leg3_on_the_ground(pActwk);
                }

                return 1;
            } else {

                pActwk->actfree[2] |= 1;
                pActwk->r_no0 = 4;
                return 1;
            }
        }
    }

    if (((Sint16 *)pActwk)[31] < 16) {

        leg3_set(pActwk);
        if (0 >= emycol_d(pActwk)) {
            leg3_on_the_ground(pActwk);
            sub_sync(126);
        }
    } else {

        pActwk->actfree[2] |= 1;
        pActwk->r_no0 = 4;
    }
    return 1;
}

Sint32 egg1leg3_03(sprite_status *pActwk) {
    ((Sint32 *)pActwk)[14] = ((Sint32 *)pActwk)[14] + ((Sint32 *)pActwk)[16];
    ((Sint32 *)pActwk)[15] =
        ((Sint32 *)pActwk)[15] - sprite_status_get_xspeed_yspeed(pActwk);
    if (((Sint32 *)pActwk)[15] <= 0) {

        ((Sint16 *)pActwk)[29] = 0;
        ((Sint16 *)pActwk)[28] = 0;
        ((Sint16 *)pActwk)[31] = 0;
        ((Sint16 *)pActwk)[30] = 0;
        pActwk->actfree[2] |= 1;
        pActwk->r_no0 = 8;
    }
    leg3_set(pActwk);
    return 1;
}

Sint32 egg1leg3_02(sprite_status *pActwk) {

    if (!(pActwk->actfree[2] & 16)) {
        leg3_set_not_grd(pActwk);
        if (0 >= emycol_d(pActwk)) {

            leg3_on_the_ground(pActwk);
            sub_sync(126);
        }
    }
    return 1;
}

Sint32 egg1leg3_04(sprite_status *pActwk) {
    leg3_set_not_grd(pActwk);
    return 1;
}

Sint32 egg1leg3_05(sprite_status *pActwk) {
    Sint16 subact;

    subact = ((Sint16 *)pActwk)[25];
    subact = ((Sint16 *)&actwk[subact])[25];
    subact = ((Sint16 *)&actwk[subact])[25];
    subact = ((Sint16 *)&actwk[subact])[26];
    subact = ((Sint16 *)&actwk[subact])[26];
    subact = ((Sint16 *)&actwk[subact])[26];
    pActwk->xposi.w.h = actwk[subact].xposi.w.h - 10;
    pActwk->yposi.w.h = actwk[subact].yposi.w.h;
    ((Sint16 *)pActwk)[29] = ((Sint16 *)&actwk[subact])[29];
    ((Sint16 *)pActwk)[31] = ((Sint16 *)&actwk[subact])[31];
    return 1;
}

void leg3_set(sprite_status *pActwk) {
    Sint16 subact;
    Sint32 xs, ys;

    if (!(pActwk->actfree[2] & 16)) {
        leg3_set_not_grd(pActwk);
    } else if (pActwk->actfree[2] & 32) {

        xs = ((Sint32 *)pActwk)[16];
        ys = sprite_status_get_xspeed_yspeed(pActwk);

        if (pActwk->r_no0 != 6) {
            xs = -xs;
            ys = -ys;
        }

        subact = ((Sint16 *)pActwk)[25];
        actwk[subact].xposi.l -= xs;
        actwk[subact].yposi.l += ys;
        subact = ((Sint16 *)&actwk[subact])[25];
        actwk[subact].xposi.l -= xs;
        actwk[subact].yposi.l += ys;
        subact = ((Sint16 *)&actwk[subact])[25];
        actwk[subact].xposi.l -= xs;
        actwk[subact].yposi.l += ys;
        subact = ((Sint16 *)&actwk[subact])[25];
        actwk[subact].xposi.l -= xs;
        actwk[subact].yposi.l += ys;
    }
}

Sint32 egg1leg3_06(sprite_status *pActwk) {
    Sint32 ret = 1;

    if (!(pActwk->actfree[2] & 2)) {
        pActwk->actfree[2] |= 2;

        if (pActwk->actfree[2] & 16) {
            ((Sint32 *)pActwk)[16] = 0;
            sprite_status_set_xspeed_yspeed(pActwk, -143360);
        } else {
            ((Sint32 *)pActwk)[16] = 0;
            sprite_status_set_xspeed_yspeed(pActwk, -143360);
        }
    } else {

        if (!(pActwk->actfree[2] & 16)) {
            ((Sint32 *)pActwk)[16] = ((Sint32 *)pActwk)[16] - 1632;
            sprite_status_add_xspeed_yspeed(pActwk, 5728);
        } else {
            ((Sint32 *)pActwk)[16] = ((Sint32 *)pActwk)[16] + 1632;
            sprite_status_add_xspeed_yspeed(pActwk, 5728);
        }
    }

    pActwk->xposi.l += ((Sint32 *)pActwk)[16];
    pActwk->yposi.l += sprite_status_get_xspeed_yspeed(pActwk);
    if (pActwk->yposi.w.h >= 576) {
        ret = 0;
        frameout(pActwk);
    } else
        tenmetu();
    return ret;
}

void bom_set(sprite_status *pActwk) {
    Sint16 temp;
    sprite_status *subActwk;

    Sint16 bom_tbl[20] = {-48, -16, 48,  16, -16, -16, 16, 16,  -32, 0,
                          48,  -16, -48, 16, -16, 16,  16, -16, 32,  0};

    if (pActwk->actfree[1] % 4 == 0) {
        temp = (Sint16)(pActwk->actfree[1] / 4) % 10 * 2;
        if (actwkchk(&subActwk) == 0) {
            subActwk->r_no1 = 255;

            subActwk->actno = 24;
            subActwk->xposi.w.h = pActwk->xposi.w.h + bom_tbl[temp];
            subActwk->yposi.w.h = pActwk->yposi.w.h + bom_tbl[temp + 1];
            soundset(158);
        }
    }
}

void tenmetu(void) {}
