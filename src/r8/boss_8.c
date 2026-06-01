#include "../equ.h"
#include "boss_8.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../io.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../score.h"

extern void colchg_boss8(Uint8 *a3, Uint8 *a4);
extern void fadein_boss8(void);
extern void colorset2(Sint32 ColorNo);

typedef struct {
    Sint16 xOffs;
    Sint16 yOffs;
    Sint16 xDst;
    Sint16 yDst;
    Sint16 ColiNo;
} hane_offs;

typedef struct {
    Uint8 r_no;
    Uint8 Dummy;
    Sint16 param;
} egg8act;

typedef struct {
    Sint32 X_Speed;
    Sint32 Y_Speed;
    char Roll_Speed;
    char Dummy;
} egg8spd;

static void egg8_ini(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_scrset(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_open1(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_wait(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_move_r(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_move_l(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_move_d(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_move_u(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_move_c(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_move_escu(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_hane_roll(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_hane_stop(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_hane_chg1(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_hane_chg2(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_hane_chg3(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_hane_fire(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_hane_reset(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_spin_d(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_spin_l(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_spin_r(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_target(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8target_ini(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8target_roll(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8target_move(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_targetreset(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_tobi_d(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_tobi(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_tobi_u(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_dead(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_esc(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_esc2(sprite_status *pActwk, sprite_status *pMecawk);
static void egg8_warai_chk(sprite_status *pActwk);
static void egg8_colichk(sprite_status *pActwk, sprite_status *pPlayerwk,
                         sprite_status *pMecawk);
static void next_action(sprite_status *pActwk);
static void egg8meca_ini(sprite_status *pActwk, sprite_status *pEggwk);
static void egg8meca_normal(sprite_status *pActwk, sprite_status *pEggwk);
static void egg8meca_spin(sprite_status *pActwk, sprite_status *pEggwk);
static void egg8meca_chg1(sprite_status *pActwk, sprite_status *pEggwk);
static void egg8meca_chg2(sprite_status *pActwk, sprite_status *pEggwk);
static void egg8meca_chg3(sprite_status *pActwk, sprite_status *pEggwk);
static void egg8meca_fire(sprite_status *pActwk, sprite_status *pEggwk);
static void egg8meca_hane_reset(sprite_status *pActwk, sprite_status *pEggwk);
static void egg8meca_dead(sprite_status *pActwk, sprite_status *pEggwk);
static void egg8meca_tobi(sprite_status *pActwk, sprite_status *pEggwk);
static void hane_ctrl(sprite_status *pActwk);
static void hane_stop(sprite_status *pActwk);
static void hane_roll(sprite_status *pActwk);
static void futa_roll(sprite_status *pActwk);
static Sint32 egg8hane_ini(sprite_status *pActwk, sprite_status *pMecawk,
                           sprite_status *pEggwk);
static Sint32 egg8hane_demo(sprite_status *pActwk, sprite_status *pMecawk,
                            sprite_status *pEggwk);
static Sint32 egg8hane_normal(sprite_status *pActwk, sprite_status *pMecawk,
                              sprite_status *pEggwk);
static Sint32 egg8hane_fire1(sprite_status *pActwk, sprite_status *pMecawk,
                             sprite_status *pEggwk);
static Sint32 egg8hane_wait(sprite_status *pActwk, sprite_status *pMecawk,
                            sprite_status *pEggwk);
static Sint32 egg8hane_kill(sprite_status *pActwk, sprite_status *pMecawk,
                            sprite_status *pEggwk);
static void hane_no_reset(sprite_status *pActwk, sprite_status *pMecawk,
                          sprite_status *pEggwk);
static void egg8hane_posiset(sprite_status *pActwk, sprite_status *pMecawk);
static void hane_offs_set(sprite_status *pActwk, sprite_status *pMecawk);
static void hane_rol_l(sprite_status *pActwk);
static void hane_rol_r(sprite_status *pActwk);
static void rol_sound(sprite_status *pActwk);
static Uint32 egg8hibana_ini(sprite_status *pActwk);
static Uint32 egg8hibana_1(sprite_status *pActwk);
static void make_meca(sprite_status *pActwk);
static void make_hane(sprite_status *pActwk);
static void make_hibana(sprite_status *pActwk);
static void egg8_spd_set(sprite_status *pActwk, sprite_status *pMecawk);
static void tobi_set(sprite_status *pActwk);
static void bom_set(sprite_status *pActwk);

static Uint8 egg8_pchg0[3] = {255, 2, 255};
static Uint8 egg8_pchg1[6] = {3, 5, 6, 5, 7, 255};
static Uint8 egg8_pchg2[6] = {15, 8, 0, 1, 2, 255};
static Uint8 egg8_pchg3[3] = {255, 4, 255};
static Uint8 egg8_pchg4[4] = {7, 2, 3, 255};
static Uint8 egg8_pchg5[3] = {255, 8, 255};
static Uint8 *egg8_pchg[6] = {egg8_pchg0, egg8_pchg1, egg8_pchg2,
                              egg8_pchg3, egg8_pchg4, egg8_pchg5};
static sprite_pattern egg8_pat0 = {2, {{-12, -28, 0, 451}, {-24, -28, 0, 443}}};
static sprite_pattern egg8_pat1 = {2, {{-12, -28, 0, 451}, {-24, -28, 0, 444}}};
static sprite_pattern egg8_pat2 = {2, {{-12, -28, 0, 451}, {-24, -28, 0, 445}}};
static sprite_pattern egg8_pat3 = {2, {{-12, -28, 0, 451}, {-24, -28, 0, 446}}};
static sprite_pattern egg8_pat4 = {1, {{-24, -28, 0, 447}}};
static sprite_pattern egg8_pat5 = {2, {{-12, -28, 0, 451}, {-24, -28, 0, 448}}};
static sprite_pattern egg8_pat6 = {2, {{-12, -28, 0, 451}, {-24, -44, 0, 449}}};
static sprite_pattern egg8_pat7 = {2, {{-12, -28, 0, 451}, {-24, -44, 0, 450}}};
static sprite_pattern egg8_pat8 = {1, {{-12, -28, 0, 451}}};
sprite_pattern *egg8_pat[9] = {&egg8_pat0, &egg8_pat1, &egg8_pat2,
                               &egg8_pat3, &egg8_pat4, &egg8_pat5,
                               &egg8_pat6, &egg8_pat7, &egg8_pat8};
static void (*egg8_act_tbl[28])(sprite_status *, sprite_status *) = {
    &egg8_ini,         &egg8_scrset,    &egg8_open1,     &egg8_wait,
    &egg8_move_r,      &egg8_move_l,    &egg8_move_d,    &egg8_move_u,
    &egg8_move_c,      &egg8_move_escu, &egg8_hane_roll, &egg8_hane_stop,
    &egg8_hane_chg1,   &egg8_hane_chg2, &egg8_hane_chg3, &egg8_hane_fire,
    &egg8_hane_reset,  &egg8_spin_r,    &egg8_spin_l,    &egg8_target,
    &egg8_targetreset, &egg8_spin_d,    &egg8_tobi_d,    &egg8_tobi,
    &egg8_tobi_u,      &egg8_dead,      &egg8_esc,       &egg8_esc2};

void egg8(sprite_status *pActwk) {
    sprite_status *pPlayerwk, *pMecawk;
    Sint16 idx;

    pPlayerwk = &actwk[0];

    if ((idx = ((Sint16 *)pActwk)[25]) != 0) {
        pMecawk = &actwk[idx];
        pMecawk->actfree[2] &= 159;
    } else {
        pMecawk = NULL;
    }

    egg8_warai_chk(pActwk);

    if (pActwk->actfree[18] && --pActwk->actfree[18] == 0) {
        pActwk->mstno.b.h = 0;
    }

    egg8_colichk(pActwk, pPlayerwk, pMecawk);

    if (pActwk->actfree[2] & 1) {
        next_action(pActwk);
    }
    pActwk->actfree[2] &= 254;

    egg8_act_tbl[pActwk->r_no0 / 2](pActwk, pMecawk);
    patchg(pActwk, egg8_pchg);

    colchg_boss8(&pActwk->actfree[19], &pActwk->actfree[20]);

    pMecawk = &actwk[((Sint16 *)pActwk)[25]];

    if (!(pMecawk->actfree[18] / 2 & 1)) {
        actionsub(pActwk);
    }
}

static void egg8_ini(sprite_status *pActwk, sprite_status *pMecawk) {
    pActwk->r_no0 = 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 5;
    pActwk->sprhsize = 24;
    pActwk->sprvsize = 16;
    pActwk->sproffset = 990;
    pActwk->patbase = egg8_pat;

    pActwk->actfree[16] = 4;
    pActwk->yposi.w.h -= 256;
}

static void egg8_scrset(sprite_status *pActwk, sprite_status *pMecawk) {
    Sint16 limwk;

    scralim_right = scralim_n_right = 3680;
    if ((limwk = actwk[0].xposi.w.h - 160) >= scralim_left) {
        if (actwk[0].xposi.w.h >= 3840) {

            bossflag = bossstart = 8;
            make_meca(pActwk);

            pMecawk = &actwk[((Sint16 *)pActwk)[25]];
            egg8_spd_set(pActwk, pMecawk);

            pActwk->actfree[2] &= 253;
            pActwk->r_no0 = 4;

            limwk = 3680;
        }

        scralim_left = scralim_n_left = limwk;
    }
}

static void egg8_open1(sprite_status *pActwk, sprite_status *pMecawk) {
    switch (pActwk->actfree[3]) {
    case 1:
        if (++((Sint16 *)pActwk)[23] == 240) {

            pActwk->yposi.w.h += 256;

            sub_sync(104);
            bossstart |= 128;

            pActwk->actfree[0] = 0;
            pActwk->actfree[1] = 0;
            ++pActwk->actfree[3];
            pActwk->mstno.b.h = 2;
        }

        break;

    case 2:
        if ((Uint16)pActwk->actfree[0] % 3 == 0) {
            fadein_boss8();
        }

        ++pActwk->actfree[0];
        if (pActwk->actfree[0] == 63) {

            ++pActwk->actfree[3];
            pActwk->mstno.b.h = 0;
            pActwk->actfree[0] = 0;
        }

        break;

    case 3:
        if (++pActwk->actfree[0] == 40) {

            pActwk->actfree[2] |= 1;
            pActwk->actfree[0] = 0;

            pActwk->actfree[2] |= 8;
            pActwk->colino = 50;
            pActwk->colicnt = 2;
            pMecawk->colino = 51;
            pMecawk->colicnt = 2;
        }

        break;

    default:
        pActwk->mstno.b.h = 5;
        ++pActwk->actfree[3];

        colorset2(7);
        colorset2(8);
        colorset2(9);
        break;
    }
}

static void egg8_wait(sprite_status *pActwk, sprite_status *pMecawk) {
    Uint8 wk;

    ++pActwk->actfree[0];
    wk = pActwk->actfree[14];
    if (pActwk->actfree[0] == wk) {

        pActwk->actfree[0] = 0;
        pActwk->actfree[2] |= 1;
    }
}

static void egg8_move_r(sprite_status *pActwk, sprite_status *pMecawk) {
    pActwk->xposi.l += ((Sint32 *)pActwk)[14];
    if (pActwk->xposi.w.h >= ((Sint16 *)pActwk)[30]) {

        pActwk->xposi.w.h = ((Sint16 *)pActwk)[30];
        pActwk->actfree[2] |= 1;
    }
}

static void egg8_move_l(sprite_status *pActwk, sprite_status *pMecawk) {
    pActwk->xposi.l -= ((Sint32 *)pActwk)[14];
    if (pActwk->xposi.w.h <= ((Sint16 *)pActwk)[30]) {

        pActwk->xposi.w.h = ((Sint16 *)pActwk)[30];
        pActwk->actfree[2] |= 1;
    }
}

static void egg8_move_d(sprite_status *pActwk, sprite_status *pMecawk) {
    pActwk->yposi.l += ((Sint32 *)pActwk)[4];
    if (pActwk->yposi.w.h >= ((Sint16 *)pActwk)[30]) {

        pActwk->yposi.w.h = ((Sint16 *)pActwk)[30];
        pActwk->actfree[2] |= 1;
    }
}

static void egg8_move_u(sprite_status *pActwk, sprite_status *pMecawk) {
    pActwk->yposi.l -= ((Sint32 *)pActwk)[4];
    if (pActwk->yposi.w.h <= ((Sint16 *)pActwk)[30]) {

        pActwk->yposi.w.h = ((Sint16 *)pActwk)[30];
        pActwk->actfree[2] |= 1;
    }
}

static void egg8_move_c(sprite_status *pActwk, sprite_status *pMecawk) {
    Sint16 xOfs;
    char sw;

    sw = 110;

    if (!pActwk->actfree[3]) {

        pActwk->actfree[3] = 2;

        if (pActwk->xposi.w.h <= 3840) {

            sw = 114;
        } else {
            sw = 108;
        }
    } else {
        if (!(pActwk->actfree[3] & 1)) {

            pActwk->xposi.l -= ((Sint32 *)pActwk)[14];

            if ((xOfs = ((Sint16 *)pActwk)[26]) < pActwk->xposi.w.h)
                return;

        } else {
            pActwk->xposi.l += ((Sint32 *)pActwk)[14];

            if ((xOfs = ((Sint16 *)pActwk)[26]) > pActwk->xposi.w.h)
                return;
        }

        pActwk->xposi.w.h = xOfs;

        if (!((Sint16 *)pActwk)[30]) {

            pActwk->actfree[2] |= 1;
        } else if (!(--((Sint16 *)pActwk)[30])) {
            sw = 99;
        } else {
            if (!(pActwk->actfree[3] & 1)) {
                sw = 114;
            } else {
                sw = 108;
            }

            pActwk->actfree[3] ^= 1;
        }
    }

    switch (sw) {

    case 108:
        pActwk->actfree[3] &= 254;
        ((Sint16 *)pActwk)[26] = 3716;
        break;

    case 114:
        pActwk->actfree[3] |= 1;
        ((Sint16 *)pActwk)[26] = 3964;
        break;

    case 99:
        pActwk->actfree[3] ^= 1;
        ((Sint16 *)pActwk)[26] = 3840;
        break;
    }
}

static void egg8_move_escu(sprite_status *pActwk, sprite_status *pMecawk) {
    if (!pActwk->actfree[3]) {
        egg8_hane_chg3(pActwk, pMecawk);
        pActwk->actfree[2] &= 254;
        pActwk->actfree[3] = 1;
    }

    pActwk->yposi.l -= 81920;
    if (pActwk->yposi.w.h <= ((Sint16 *)pActwk)[30]) {

        pActwk->yposi.w.h = ((Sint16 *)pActwk)[30];
        pActwk->actfree[2] |= 1;
    }
}

static void egg8_hane_roll(sprite_status *pActwk, sprite_status *pMecawk) {
    if (!(pMecawk->actfree[16] & 64)) {
        pMecawk->actfree[2] |= 16;
        pMecawk->actfree[3] = 0;
        pMecawk->actfree[1] = 0;
    }

    pActwk->actfree[2] |= 1;
}

static void egg8_hane_stop(sprite_status *pActwk, sprite_status *pMecawk) {
    if (!pActwk->actfree[3]) {

        ++pActwk->actfree[3];
        pMecawk->actfree[2] |= 16;
        pMecawk->actfree[16] &= 127;
        pMecawk->actfree[3] = 0;
        pMecawk->actfree[1] = 0;

        if (pActwk->actfree[16] == 2) {

            pMecawk->actfree[12] = 8;
        } else {
            pMecawk->actfree[12] = 4;
        }
    }

    if (pMecawk->actfree[16] & 128) {

        pActwk->actfree[2] |= 1;
    }
}

static void egg8_hane_chg1(sprite_status *pActwk, sprite_status *pMecawk) {
    pMecawk->actfree[3] = 0;
    pMecawk->actfree[0] = 0;
    pMecawk->actfree[1] = 0;
    pMecawk->actfree[16] |= 160;
    pMecawk->actfree[16] &= 191;
    pMecawk->r_no0 = 4;

    pActwk->actfree[0] = 0;
    pActwk->actfree[1] = 0;
    pActwk->actfree[2] |= 1;
}

static void egg8_hane_chg2(sprite_status *pActwk, sprite_status *pMecawk) {
    soundset(201);

    pMecawk->actfree[3] = 0;
    pMecawk->actfree[0] = 0;
    pMecawk->actfree[1] = 0;
    pMecawk->r_no0 = 6;

    pActwk->actfree[2] |= 1;
}

static void egg8_hane_chg3(sprite_status *pActwk, sprite_status *pMecawk) {
    pMecawk->actfree[3] = 0;
    pMecawk->actfree[0] = 0;
    pMecawk->actfree[1] = 0;
    pMecawk->r_no0 = 8;

    pActwk->actfree[2] |= 1;
}

static void egg8_hane_fire(sprite_status *pActwk, sprite_status *pMecawk) {
    switch (pActwk->actfree[3]) {

    case 1:
        ((Sint16 *)pActwk)[30] = 316;
        egg8_move_u(pActwk, pMecawk);

        if (pActwk->actfree[2] & 1) {

            ++pActwk->actfree[3];
        }

        pActwk->actfree[2] &= 254;
        break;

    case 2:
        if (pMecawk->actfree[19] == pActwk->actfree[16] * 2) {

            pActwk->actfree[2] |= 1;
        }
        break;

    default:
        ++pActwk->actfree[3];
        pMecawk->actfree[3] = 0;
        pMecawk->actfree[0] = 0;
        pMecawk->actfree[1] = 0;
        pMecawk->r_no0 = 10;
        pMecawk->actfree[19] = 0;
        pMecawk->actfree[20] = 0;
        break;
    }
}

static void egg8_hane_reset(sprite_status *pActwk, sprite_status *pMecawk) {
    pMecawk->actfree[3] = 0;
    pMecawk->actfree[0] = 0;
    pMecawk->actfree[1] = 0;
    pMecawk->r_no0 = 12;

    pActwk->actfree[2] |= 1;
}

static void egg8_spin_d(sprite_status *pActwk, sprite_status *pMecawk) {
    Sint32 tateini = 0;

    if (!pActwk->actfree[3]) {

        if (pActwk->actfree[16] == 4) {

            pMecawk->actfree[12] = 8;

            tateini = 1;
        } else {
            if (actwk[0].xposi.w.h < pActwk->xposi.w.h) {

                pMecawk->actfree[12] = 8;

                ((Sint16 *)pActwk)[30] = 3964;
                pActwk->actfree[3] = 1;
            } else {
                if (pActwk->actfree[16] != 3) {

                    pMecawk->actfree[12] = 4;
                } else {
                    pMecawk->actfree[12] = 0;
                }

                ((Sint16 *)pActwk)[30] = 3716;
                pActwk->actfree[3] = 2;
            }
        }
    }

    if (pActwk->actfree[3] == 3) {

        tateini = 2;
    } else if (pActwk->actfree[3]) {
        if (pActwk->actfree[3] == 1) {

            egg8_move_r(pActwk, pMecawk);
        } else {
            egg8_move_l(pActwk, pMecawk);
        }

        if (pActwk->actfree[2] & 1) {
            tateini = 1;
        }

        pActwk->actfree[2] &= 254;
    }

    switch (tateini) {

    case 1:
        pMecawk->actfree[2] |= 16;
        pMecawk->actfree[16] &= 127;
        pMecawk->actfree[3] = 0;
        pMecawk->actfree[1] = 0;

        ((Sint16 *)pActwk)[30] = 416;
        pActwk->actfree[3] = 3;

    case 2:
        egg8_move_d(pActwk, pMecawk);

        if (pActwk->actfree[2] & 1) {

            pActwk->actfree[3] = 0;
        }
        break;
    }
}

static void egg8_spin_l(sprite_status *pActwk, sprite_status *pMecawk) {
    egg8_spin_r(pActwk, pMecawk);
}

static Sint16 spin_tbl1[32] = {26, -4, 26, 6,  26, -6, 26, 4,  26, -4, 26,
                               6,  26, -6, 26, 4,  26, -4, 26, 6,  26, -6,
                               26, 4,  26, -4, 26, 6,  26, -6, 26, 4};
static Sint16 spin_tbl3[32] = {26, -4, 26, 6,  26, -6, 26, 4,  26, -4, 26,
                               6,  26, -6, 26, 4,  26, -4, 26, 6,  26, -6,
                               26, 4,  26, -4, 26, 6,  21, 15, 26, 22};
static Sint16 *spin_tbl[5] = {0, spin_tbl1, spin_tbl1, spin_tbl3, spin_tbl1};

static void egg8_spin_r(sprite_status *pActwk, sprite_status *pMecawk) {
    Sint16 *pSpinTbl, xwk, ywk;
    Uint8 hanecntwk, para0wk;

    if ((char)pActwk->actfree[3] >= 5) {

        pMecawk->r_no0 = 2;
        pMecawk->actfree[3] = 0;
        pMecawk->actfree[1] = 0;

        pActwk->actfree[3] = 0;
        pActwk->actfree[0] = 0;
        pActwk->actfree[1] = 0;
        pActwk->actfree[2] |= 1;
    } else if (pActwk->actfree[3] & 128) {

        if (++pActwk->actfree[0] == 60) {
            pActwk->actfree[0] = 0;
            pActwk->actfree[3] &= 127;
        }

    } else {
        if (!pActwk->actfree[3]) {

            pActwk->actfree[0] = 0;
            pActwk->actfree[15] = 0;

            if ((pActwk->actfree[16] == 4 &&
                 actwk[0].xposi.w.h >= pActwk->xposi.w.h) ||
                (pActwk->actfree[16] != 4 && pActwk->xposi.w.h < 3840)) {
                pActwk->actfree[1] = 0;
                pActwk->actfree[3] = 2;
            } else {
                pActwk->actfree[1] = 7;
                pActwk->actfree[3] = 1;
            }

            pMecawk->r_no0 = 14;
            pMecawk->actfree[3] = 0;
            pMecawk->actfree[1] = 0;
        }

        if (++pActwk->actfree[0] == 20) {

            ++((char *)pActwk)[61];

            if (!(((char *)pActwk)[61] & 1)) {
                sub_sync(126);
            }

            pActwk->actfree[0] = 0;

            pSpinTbl = spin_tbl[pActwk->actfree[16]];
            pSpinTbl += (pActwk->actfree[1] & 15) * 2;

            xwk = *pSpinTbl++;
            ywk = *pSpinTbl++;

            if (pActwk->actfree[3] & 1) {

                pMecawk->actfree[2] |= 64;
                xwk *= -1;
                ywk *= -1;
            } else {
                pMecawk->actfree[2] |= 32;
            }

            pActwk->xposi.w.h += xwk;
            pActwk->yposi.w.h += ywk;

            hanecntwk = pActwk->actfree[16];
            para0wk = ((char *)pActwk)[61];

            if ((hanecntwk == 4 && (para0wk == 4 || para0wk == 12)) ||
                (hanecntwk != 4 && (para0wk == 4 || para0wk == 8))) {
                if ((char)pActwk->actfree[3] >= 3) {

                    pActwk->actfree[3] = 133;

                    if (pActwk->actfree[3] & 1) {

                        --pActwk->actfree[1];
                    } else {
                        ++pActwk->actfree[1];
                    }
                } else if (pActwk->actfree[3] == 1) {

                    pActwk->actfree[3] = 132;
                } else {
                    pActwk->actfree[3] = 131;
                }
            } else {
                if (pActwk->actfree[3] & 1) {

                    --pActwk->actfree[1];
                } else {
                    ++pActwk->actfree[1];
                }
            }
        }
    }
}

static void egg8_target(sprite_status *pActwk, sprite_status *pMecawk) {
    if (!pActwk->actfree[3]) {

        egg8target_ini(pActwk, pMecawk);
    }

    if (!(pActwk->actfree[3] & 2)) {

        egg8target_roll(pActwk, pMecawk);
    }

    if (!(pActwk->actfree[3] & 4)) {

        egg8target_move(pActwk, pMecawk);
    }

    if (pActwk->actfree[3] == 7) {

        pActwk->actfree[2] |= 1;
    }
}

static void egg8target_ini(sprite_status *pActwk, sprite_status *pMecawk) {
    Sint16 xPos, xSav;
    int_union xwk, ywk;

    pActwk->actfree[3] |= 1;
    xwk.w.l = xSav = xPos = actwk[0].xposi.w.h - pActwk->xposi.w.h;

    if (xSav < 0) {
        xPos *= -1;
    }

    pMecawk->actfree[2] |= 16;
    pMecawk->actfree[16] &= 127;
    pMecawk->actfree[3] = 0;
    pMecawk->actfree[1] = 0;

    if (xPos < 64) {

        xwk.l = xwk.w.l;
        xwk.l = xwk.l * 16 / 90 * 4096;
        ((Sint32 *)pActwk)[14] = xwk.l;

        ywk.l = (380 - pActwk->yposi.w.h) * 16 / 90 * 4096;
        ((Sint32 *)pActwk)[4] = ywk.l;

        if (pActwk->actfree[16] != 2) {

            pMecawk->actfree[12] = 4;
        } else {
            pMecawk->actfree[12] = 6;
        }
    } else {
        if (xSav >= 0) {

            xwk.w.l -= 16;
        } else {
            xwk.w.l += 16;
        }

        xwk.l = xwk.w.l;
        xwk.l = xwk.l * 16 / 90 * 4096;
        ((Sint32 *)pActwk)[14] = xwk.l;

        ywk.l = (394 - pActwk->yposi.w.h) * 16 / 90 * 4096;
        ((Sint32 *)pActwk)[4] = ywk.l;

        if (pActwk->actfree[16] != 2 && xSav >= 0) {
            pMecawk->actfree[12] = 2;
        } else {
            pMecawk->actfree[12] = 6;
        }
    }
}

static void egg8target_roll(sprite_status *pActwk, sprite_status *pMecawk) {
    if (pMecawk->actfree[16] & 128) {

        pActwk->actfree[3] |= 2;
    }
}

static void egg8target_move(sprite_status *pActwk, sprite_status *pMecawk) {
    Sint16 ywk;

    if (!(pMecawk->actfree[12] & 3)) {
        ywk = 380;
    } else {
        ywk = 394;
    }

    pActwk->xposi.l += ((Sint32 *)pActwk)[14];
    pActwk->yposi.l += ((Sint32 *)pActwk)[4];

    if (pActwk->yposi.w.h >= ywk) {

        pActwk->yposi.w.h = ywk;
        ((Sint32 *)pActwk)[14] *= -1;
        ((Sint32 *)pActwk)[4] *= -1;
        pActwk->actfree[3] |= 4;
    }
}

static void egg8_targetreset(sprite_status *pActwk, sprite_status *pMecawk) {
    pActwk->xposi.l += ((Sint32 *)pActwk)[14];
    pActwk->yposi.l += ((Sint32 *)pActwk)[4];

    if (pActwk->yposi.w.h <= 316) {

        pActwk->yposi.w.h = 316;
        pActwk->actfree[3] = 0;
        pActwk->actfree[2] |= 1;

        egg8_spd_set(pActwk, pMecawk);
    }
}

static void egg8_tobi_d(sprite_status *pActwk, sprite_status *pMecawk) {
    if (pActwk->actfree[3] == 0 || pActwk->actfree[3] > 5) {

        if (pActwk->xposi.w.h > actwk[0].xposi.w.h) {

            ((Sint16 *)pActwk)[30] = 3964;
            pActwk->actfree[3] = 1;
        } else {
            ((Sint16 *)pActwk)[30] = 3716;
            pActwk->actfree[3] = 2;
        }
    }

    if (pActwk->actfree[3] == 1) {

        egg8_move_r(pActwk, pMecawk);
    } else if (pActwk->actfree[3] == 2) {

        egg8_move_l(pActwk, pMecawk);
    }

    if (pActwk->actfree[3] == 1 || pActwk->actfree[3] == 2) {

        if (!(pActwk->actfree[2] & 1))
            return;

        pActwk->actfree[2] &= 254;

        pMecawk->actfree[2] |= 16;
        pMecawk->actfree[16] &= 127;
        pMecawk->actfree[12] = 8;
        pMecawk->actfree[3] = 0;
        pMecawk->actfree[1] = 0;

        pActwk->actfree[3] = 3;
    }

    if (pActwk->actfree[3] == 3) {

        if (!(pMecawk->actfree[16] & 128))
            return;

        ((Sint16 *)pActwk)[30] = 312;

        pActwk->actfree[3] = 4;
    }

    if (pActwk->actfree[3] == 4 || pActwk->actfree[3] > 5) {

        egg8_move_d(pActwk, pMecawk);

        if (!(pActwk->actfree[2] & 1))
            return;

        pActwk->actfree[2] &= 254;

        pActwk->actfree[3] = 5;
        ((Sint16 *)pActwk)[26] = 0;
        ((Sint16 *)pActwk)[27] = -104;
        ((Sint16 *)pActwk)[23] = 16384;
    }

    if (pActwk->actfree[3] == 5) {

        tobi_set(pActwk);
        pActwk->xposi.w.h -= ((Sint16 *)pActwk)[26];
        ((Sint16 *)pActwk)[26] = 0;

        if (((Sint16 *)pActwk)[23] == -32768) {

            pActwk->actfree[3] = 0;
            pActwk->actfree[2] |= 1;

            soundset(180);
        }
    }
}

static void egg8_tobi(sprite_status *pActwk, sprite_status *pMecawk) {
    Sint16 xwk;

    if ((char)pActwk->actfree[3] < 5) {
        if (!pActwk->actfree[3]) {

            ((Sint16 *)pActwk)[27] = 0;
            ((Sint16 *)pActwk)[23] = 0;

            if (pActwk->xposi.w.h < 3840) {

                ((Sint16 *)pActwk)[26] = -41;
                pActwk->actfree[3] = 2;
            } else {
                ((Sint16 *)pActwk)[26] = 41;
                pActwk->actfree[3] = 1;
            }

            pMecawk->r_no0 = 18;
            pMecawk->actfree[10] = 2;
            pMecawk->actfree[3] = 0;
            pMecawk->actfree[1] = 0;
        }

        tobi_set(pActwk);

        if (((Sint16 *)pActwk)[23] != -32768)
            return;

        soundset(180);

        ((Sint16 *)pActwk)[27] = 0;
        ((Sint16 *)pActwk)[23] = 0;

        if (pActwk->actfree[3] & 1) {

            ((Sint16 *)pActwk)[26] = 41;

            if ((xwk = pActwk->xposi.w.h - 3716) < 0) {
                xwk *= -1;
            }
        } else {
            ((Sint16 *)pActwk)[26] = -41;

            if ((xwk = pActwk->xposi.w.h - 3964) < 0) {
                xwk *= -1;
            }
        }

        if (xwk >= 41)
            return;

        ((Sint16 *)pActwk)[26] *= -1;
        if (pActwk->actfree[3] < 3) {
            if (pActwk->actfree[3] != 1) {

                pActwk->actfree[3] = 3;
            } else {
                pActwk->actfree[3] = 4;
            }

            return;
        }
    }

    pActwk->actfree[3] = 0;
    pActwk->actfree[2] |= 1;

    pMecawk->r_no0 = 2;
    pMecawk->actfree[3] = 0;
    pMecawk->actfree[1] = 0;
    pMecawk->actfree[14] = 0;
}

static void egg8_tobi_u(sprite_status *pActwk, sprite_status *pMecawk) {
    if (!pActwk->actfree[3]) {

        pActwk->actfree[3] = 1;
        ((Sint16 *)pActwk)[26] = 0;
        ((Sint16 *)pActwk)[27] = 0;
        ((Sint16 *)pActwk)[23] = 0;
    }

    tobi_set(pActwk);
    pActwk->xposi.w.h -= ((Sint16 *)pActwk)[26];
    ((Sint16 *)pActwk)[26] = 0;

    if (((Sint16 *)pActwk)[23] >= 16384) {

        pActwk->actfree[3] = 0;
        pActwk->actfree[2] |= 1;
        pActwk->actfree[0] = 0;
        pActwk->actfree[1] = 0;
        ((Sint16 *)pActwk)[27] = 0;
    }
}

static void egg8_dead(sprite_status *pActwk, sprite_status *pMecawk) {
    sprite_status *pNewact;

    if (!pActwk->actfree[0]) {

        pActwk->mstno.b.h = 3;
        pActwk->actfree[18] = 0;

        pMecawk->r_no0 = 16;
        pMecawk->patno = 3;
        pActwk->colino = 0;
        pActwk->colicnt = 0;
        pMecawk->colino = 0;
        pMecawk->colicnt = 0;
        pActwk->actfree[2] &= 247;
    }

    if (++pActwk->actfree[0] >= 90) {

        if (actwkchk(&pNewact) != 0)
            return;
        pNewact->actno = 67;
        pNewact->xposi.w.h = 3656;
        pNewact->yposi.w.h = 460;

        pActwk->r_no0 = 52;

    } else {
        bom_set(pActwk);
    }
}

static void egg8_esc(sprite_status *pActwk, sprite_status *pMecawk) {
    Uint8 dirwk;
    Sint16 sinwk, coswk;

    if (!pActwk->actfree[3]) {

        scoreup(100);

        pActwk->actfree[1] = 0;
        ((Sint16 *)pActwk)[27] = 0;
        ++pActwk->actfree[3];
    }

    ++pActwk->actfree[0];
    bom_set(pActwk);

    if ((Uint16)pActwk->actfree[0] % 60 == 0) {
        make_hibana(pActwk);
    }

    pActwk->yposi.w.h -= ((Sint16 *)pActwk)[27];
    pActwk->yposi.l += 16384;

    dirwk = pActwk->actfree[1];
    pActwk->actfree[1] += 3;
    sinset(dirwk, &sinwk, &coswk);

    sinwk = sinwk * 4 >> 8;
    ((Sint16 *)pActwk)[27] = sinwk;
    pActwk->yposi.w.h += sinwk;

    pActwk->xposi.l += ((Sint32 *)pActwk)[14];

    if (pActwk->xposi.w.h >= 4064) {

        colorset2(11);

        pActwk->r_no0 = 54;
        pActwk->actfree[0] = 0;
        bossstart = 0;
    }
}

static void egg8_esc2(sprite_status *pActwk, sprite_status *pMecawk) {
    ++pActwk->actfree[0];
    bom_set(pActwk);
}

static void egg8_warai_chk(sprite_status *pActwk) {
    if (!pActwk->mstno.b.h) {
        if (((Sint16 *)&actwk[0])[26] || actwk[0].r_no0 == 6) {

            pActwk->actfree[18] = 120;
            pActwk->mstno.b.h = 4;
        }
    }
}

static void egg8_colichk(sprite_status *pActwk, sprite_status *pPlayerwk,
                         sprite_status *pMecawk) {
    Uint8 hane_patno;

    if (!(pActwk->actfree[2] & 8))
        return;

    pMecawk->actfree[17] = 255;

    if (pMecawk->actfree[18]) {

        if (--pMecawk->actfree[18] != 0)
            return;

    } else if (!pActwk->colino || !pMecawk->colino) {

        if (pActwk->r_no0 != 30) {
            pActwk->colino = 0;
            pActwk->colicnt = 0;
            pMecawk->colino = 0;
            pMecawk->colicnt = 0;

            make_hibana(pActwk);
            pActwk->actfree[18] = 150;
            pActwk->mstno.b.h = 1;

            pMecawk->actfree[18] = 120;
            pActwk->actfree[17] = 0;
            pActwk->actfree[3] = 0;
            pActwk->actfree[2] |= 1;

            soundset(172);

            --pActwk->actfree[16];
            egg8_spd_set(pActwk, pMecawk);

            hane_patno = pMecawk->actfree[11];
            switch (pActwk->actfree[16]) {

            case 0:
                pActwk->r_no0 = 50;
                pActwk->actfree[2] &= 254;
                pActwk->actfree[0] = 0;
                pActwk->actfree[1] = 0;

                pMecawk->actfree[17] = 0;
                break;

            case 3:
                if (pPlayerwk->xposi.w.h < pActwk->xposi.w.h) {

                    if (hane_patno < 4) {

                        pMecawk->actfree[17] = 2;
                    } else if (hane_patno < 8) {

                        pMecawk->actfree[17] = 1;
                    } else if (hane_patno < 12) {

                        pMecawk->actfree[17] = 0;
                    } else {

                        pMecawk->actfree[17] = 3;
                    }
                } else {
                    if (hane_patno >= 13 || hane_patno == 0) {

                        pMecawk->actfree[17] = 2;
                    } else if (hane_patno < 5) {

                        pMecawk->actfree[17] = 1;
                    } else if (hane_patno < 9) {

                        pMecawk->actfree[17] = 0;
                    } else {

                        pMecawk->actfree[17] = 3;
                    }
                }

                break;

            case 2:
                if (pPlayerwk->xposi.w.h < pActwk->xposi.w.h) {

                    if (hane_patno >= 13 || hane_patno < 3) {

                        pMecawk->actfree[17] = 2;
                    } else if (hane_patno < 8) {

                        pMecawk->actfree[17] = 1;
                    } else {

                        pMecawk->actfree[17] = 0;
                    }
                } else {
                    if (hane_patno >= 12 || hane_patno == 0) {

                        pMecawk->actfree[17] = 2;
                    } else if (hane_patno < 5) {

                        pMecawk->actfree[17] = 1;
                    } else {

                        pMecawk->actfree[17] = 0;
                    }
                }

                break;

            case 1:
                if (pPlayerwk->xposi.w.h >= pActwk->xposi.w.h) {

                    pMecawk->actfree[17] = 0;
                } else {
                    pMecawk->actfree[17] = 1;
                }

                break;

            default:
                pActwk->actfree[16] = 0;
                break;
            }

            return;
        }
    }

    pActwk->colino = 50;
    pActwk->colicnt = 2;
    pMecawk->colino = 51;
    pMecawk->colicnt = 2;
}

static egg8act egg8acttbl1[13] = {
    {18, 0, 316}, {20, 0, 0}, {16, 0, 2}, {30, 0, 0}, {32, 0, 0},
    {6, 0, 120},  {20, 0, 0}, {16, 0, 2}, {44, 0, 0}, {46, 0, 0},
    {48, 0, 0},   {20, 0, 0}, {0, 0, 0}};
static egg8act egg8acttbl2[23] = {
    {18, 0, 316}, {20, 0, 0}, {16, 0, 2}, {22, 0, 0},  {24, 0, 0}, {6, 0, 60},
    {20, 0, 0},   {30, 0, 0}, {32, 0, 0}, {6, 0, 120}, {20, 0, 0}, {38, 0, 0},
    {26, 0, 0},   {6, 0, 60}, {28, 0, 0}, {40, 0, 0},  {20, 0, 0}, {38, 0, 0},
    {26, 0, 0},   {6, 0, 60}, {28, 0, 0}, {40, 0, 0},  {0, 0, 0}};
static egg8act egg8acttbl3[29] = {
    {18, 0, 316}, {20, 0, 0},  {16, 0, 2}, {42, 0, 0},  {34, 0, 0}, {20, 0, 0},
    {14, 0, 348}, {6, 0, 120}, {16, 0, 2}, {22, 0, 0},  {24, 0, 0}, {6, 0, 40},
    {20, 0, 0},   {30, 0, 0},  {32, 0, 0}, {6, 0, 120}, {20, 0, 0}, {38, 0, 0},
    {26, 0, 0},   {6, 0, 90},  {28, 0, 0}, {40, 0, 0},  {20, 0, 0}, {38, 0, 0},
    {26, 0, 0},   {6, 0, 90},  {28, 0, 0}, {40, 0, 0},  {0, 0, 0}};
static egg8act egg8acttbl4[29] = {
    {18, 0, 316}, {20, 0, 0},  {16, 0, 2}, {42, 0, 0},  {34, 0, 0}, {20, 0, 0},
    {14, 0, 348}, {6, 0, 120}, {16, 0, 2}, {22, 0, 0},  {24, 0, 0}, {6, 0, 40},
    {20, 0, 0},   {30, 0, 0},  {32, 0, 0}, {6, 0, 120}, {20, 0, 0}, {38, 0, 0},
    {26, 0, 0},   {6, 0, 120}, {28, 0, 0}, {40, 0, 0},  {20, 0, 0}, {38, 0, 0},
    {26, 0, 0},   {6, 0, 120}, {28, 0, 0}, {40, 0, 0},  {0, 0, 0}};
static egg8act *egg8acttbl[5] = {0, egg8acttbl1, egg8acttbl2, egg8acttbl3,
                                 egg8acttbl4};

static void next_action(sprite_status *pActwk) {
    egg8act *pEggactTbl;
    Uint8 tblno, tblidx;

    do {
        tblno = pActwk->actfree[16];
        tblidx = pActwk->actfree[17];
        pEggactTbl = &egg8acttbl[tblno][tblidx];

        if (!pEggactTbl->r_no) {
            pActwk->actfree[17] = 0;
        }
    } while (!pEggactTbl->r_no);

    ++pActwk->actfree[17];
    pActwk->r_no0 = pEggactTbl->r_no;
    ((Sint16 *)pActwk)[30] = pEggactTbl->param;
    pActwk->actfree[3] = 0;
}

static sprite_pattern meca_pat0 = {1, {{-36, -60, 0, 452}}};
static sprite_pattern meca_pat1 = {1, {{-36, -60, 0, 453}}};
static sprite_pattern meca_pat2 = {1, {{-36, -60, 0, 454}}};
static sprite_pattern meca_pat3 = {1, {{-36, -36, 0, 455}}};
sprite_pattern *egg8meca_pat[4] = {&meca_pat0, &meca_pat1, &meca_pat2,
                                   &meca_pat3};
static void (*meca_act_tbl[10])(sprite_status *, sprite_status *) = {
    &egg8meca_ini,  &egg8meca_normal, &egg8meca_chg1,       &egg8meca_chg2,
    &egg8meca_chg3, &egg8meca_fire,   &egg8meca_hane_reset, &egg8meca_spin,
    &egg8meca_dead, &egg8meca_tobi};

void egg8meca(sprite_status *pActwk) {
    sprite_status *pEggwk;

    pEggwk = &actwk[((Sint16 *)pActwk)[25]];

    meca_act_tbl[pActwk->r_no0 / 2](pActwk, pEggwk);

    if (!(pActwk->actfree[18] / 2 & 1)) {
        actionsub(pActwk);
    }
}

static void egg8meca_ini(sprite_status *pActwk, sprite_status *pEggwk) {
    pActwk->r_no0 = 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 5;
    pActwk->sprhsize = 36;
    pActwk->sprvsize = 36;
    pActwk->sproffset = 9182;
    pActwk->patbase = egg8meca_pat;
    ((Sint16 *)pActwk)[30] = 0;
    pActwk->actfree[17] = 255;
    make_hane(pActwk);
    pActwk->actfree[16] |= 160;
    pActwk->actfree[16] &= 191;
}

static void egg8meca_normal(sprite_status *pActwk, sprite_status *pEggwk) {
    pActwk->xposi.w.h = pEggwk->xposi.w.h;
    pActwk->yposi.w.h = pEggwk->yposi.w.h;
    hane_ctrl(pActwk);
}

static void egg8meca_spin(sprite_status *pActwk, sprite_status *pEggwk) {
    pActwk->xposi.w.h = pEggwk->xposi.w.h;
    pActwk->yposi.w.h = pEggwk->yposi.w.h;
    futa_roll(pActwk);
}

static void egg8meca_chg1(sprite_status *pActwk, sprite_status *pEggwk) {
    switch (pActwk->actfree[3]) {

    case 0:
        pActwk->actfree[10] = 1;
        ((Sint16 *)pActwk)[30] = 0;
        pActwk->actfree[3] = 1;

    case 1:
        if (((Sint16 *)pActwk)[30] <= 170)
            break;

    default:
        ++pActwk->actfree[3];

        if (pActwk->actfree[3] == 2) {

            pActwk->actfree[10] = 3;
        } else if (pActwk->actfree[3] == 6) {

            pActwk->actfree[10] = 4;
        }
        break;
    }

    if ((((Sint16 *)pActwk)[30] += 8) >= 256) {

        ((Sint16 *)pActwk)[30] = 256;
        pActwk->actfree[3] = 0;
        pActwk->r_no0 = 2;
        pActwk->actfree[10] = 4;
    }
}

static void egg8meca_chg2(sprite_status *pActwk, sprite_status *pEggwk) {
    if (!pActwk->actfree[3]) {

        pActwk->actfree[10] = 1;
        ++pActwk->actfree[3];
    }

    if ((((Sint16 *)pActwk)[30] += 16) >= 236) {
        pActwk->actfree[10] = 2;
    }

    if (((Sint16 *)pActwk)[30] >= 256) {

        ((Sint16 *)pActwk)[30] = 256;
        pActwk->actfree[3] = 0;
        pActwk->r_no0 = 2;
    }
}

static void egg8meca_chg3(sprite_status *pActwk, sprite_status *pEggwk) {
    pActwk->xposi.w.h = pEggwk->xposi.w.h;
    pActwk->yposi.w.h = pEggwk->yposi.w.h;

    if (!pActwk->actfree[3]) {

        pActwk->actfree[10] = 2;
        ++pActwk->actfree[3];
    }

    if ((((Sint16 *)pActwk)[30] -= 6) < 236) {
        pActwk->actfree[10] = 1;
    }

    if (((Sint16 *)pActwk)[30] <= 0) {

        ((Sint16 *)pActwk)[30] = 0;
        pActwk->actfree[3] = 0;
        pActwk->r_no0 = 2;
        pActwk->actfree[10] = 0;
    }
}

static void egg8meca_fire(sprite_status *pActwk, sprite_status *pEggwk) {
    pActwk->xposi.w.h = pEggwk->xposi.w.h;
    pActwk->yposi.w.h = pEggwk->yposi.w.h;

    if (pEggwk->actfree[16] == pActwk->actfree[19]) {

        pActwk->actfree[21] =
            (actwk[0].xposi.w.h + (Sint16)actwk[0].pattim & 3) * 8;

        pActwk->r_no0 = 2;
        pActwk->actfree[2] |= 16;
    } else {
        hane_roll(pActwk);
    }
}

static void egg8meca_hane_reset(sprite_status *pActwk, sprite_status *pEggwk) {
    pActwk->xposi.w.h = pEggwk->xposi.w.h;
    pActwk->yposi.w.h = pEggwk->yposi.w.h;

    if (pActwk->actfree[3] != 1) {

        pActwk->actfree[2] |= 8;
        ++pActwk->actfree[3];
        ((Sint16 *)pActwk)[30] = -256;
        pActwk->actfree[16] |= 160;
        pActwk->actfree[16] &= 191;
    }

    if ((((Sint16 *)pActwk)[30] += 6) >= 0) {

        pActwk->actfree[2] &= 247;
        ((Sint16 *)pActwk)[30] = 0;
        pActwk->actfree[0] = 0;
        pActwk->actfree[3] = 0;
        pActwk->r_no0 = 2;
    }
}

static void egg8meca_dead(sprite_status *pActwk, sprite_status *pEggwk) {
    pActwk->xposi.w.h = pEggwk->xposi.w.h;
    pActwk->yposi.w.h = pEggwk->yposi.w.h;
}

static void egg8meca_tobi(sprite_status *pActwk, sprite_status *pEggwk) {
    Sint16 wk;

    pActwk->xposi.w.h = pEggwk->xposi.w.h;
    pActwk->yposi.w.h = pEggwk->yposi.w.h;

    if ((wk = 416 - pEggwk->yposi.w.h) > 36) {
        wk = 36;
    }

    ((Sint16 *)pActwk)[30] = (wk << 8) / 36;
}

static void hane_ctrl(sprite_status *pActwk) {
    Uint8 FlagWk;

    FlagWk = pActwk->actfree[2];
    pActwk->actfree[2] &= 239;

    if (!(FlagWk & 16)) {
        if (!(pActwk->actfree[16] & 64)) {
            hane_stop(pActwk);
        } else {
            hane_roll(pActwk);
        }
    } else {
        pActwk->actfree[1] = 0;
        pActwk->actfree[16] &= 95;

        if (!(pActwk->actfree[16] & 64)) {
            pActwk->actfree[16] ^= 64;
            hane_roll(pActwk);
        } else {
            pActwk->actfree[16] ^= 64;
            hane_stop(pActwk);
        }
    }
}

static void hane_stop(sprite_status *pActwk) {
    if (!(pActwk->actfree[16] & 128)) {
        if (pActwk->actfree[16] & 32) {

            if (pActwk->actfree[11] != pActwk->actfree[12])
                goto label1;

            pActwk->actfree[16] |= 128;
            pActwk->patno = 0;
            return;
        }

        pActwk->patno = 1;

        if (++pActwk->actfree[1] == 10) {
            pActwk->actfree[16] |= 32;
            pActwk->actfree[1] = 0;
        }

    label1:
        if (++pActwk->actfree[0] >= pActwk->actfree[13]) {
            pActwk->actfree[0] = 0;
            pActwk->actfree[2] |= 64;
        }
    }
}

static void hane_roll(sprite_status *pActwk) {
    if (!(pActwk->actfree[16] & 32)) {

        pActwk->patno = 1;

        if (++pActwk->actfree[1] != 10)
            return;

        pActwk->actfree[1] = 0;
        pActwk->actfree[16] |= 32;
    }

    futa_roll(pActwk);

    if (++pActwk->actfree[0] >= pActwk->actfree[13]) {
        pActwk->actfree[0] = 0;
        pActwk->actfree[2] |= 64;
    }
}

static void futa_roll(sprite_status *pActwk) {
    ++pActwk->actfree[1];

    if (pActwk->actfree[1] == 6) {

        pActwk->patno = 2;
    } else if (pActwk->actfree[1] == 12) {

        pActwk->actfree[1] = 0;
        pActwk->patno = 1;
    }
}

static sprite_pattern hane0_1_pat00 = {1, {{-24, -24, 0, 456}}};
static sprite_pattern hane0_1_pat01 = {2,
                                       {{-30, -34, 0, 457}, {-30, -2, 0, 458}}};
static sprite_pattern hane0_1_pat02 = {2,
                                       {{-32, -32, 0, 459}, {-32, 0, 0, 460}}};
static sprite_pattern hane0_1_pat03 = {2,
                                       {{-32, -36, 0, 461}, {-16, -4, 0, 462}}};
static sprite_pattern hane0_1_pat04 = {1, {{-16, -24, 0, 463}}};
sprite_pattern *egg8hane0_pat1[16] = {
    &hane0_1_pat00, &hane0_1_pat01, &hane0_1_pat02, &hane0_1_pat03,
    &hane0_1_pat04, &hane0_1_pat03, &hane0_1_pat02, &hane0_1_pat01,
    &hane0_1_pat00, &hane0_1_pat01, &hane0_1_pat02, &hane0_1_pat03,
    &hane0_1_pat04, &hane0_1_pat03, &hane0_1_pat02, &hane0_1_pat01};
static sprite_pattern hane0_2_pat00 = {1, {{-24, -24, 0, 464}}};
static sprite_pattern hane0_2_pat01 = {1, {{-30, -34, 0, 465}}};
static sprite_pattern hane0_2_pat02 = {1, {{-32, -32, 0, 466}}};
static sprite_pattern hane0_2_pat03 = {1, {{-40, -36, 0, 467}}};
static sprite_pattern hane0_2_pat04 = {1, {{-40, -24, 0, 468}}};
sprite_pattern *egg8hane0_pat2[16] = {
    &hane0_2_pat00, &hane0_2_pat01, &hane0_2_pat02, &hane0_2_pat03,
    &hane0_2_pat04, &hane0_2_pat03, &hane0_2_pat02, &hane0_2_pat01,
    &hane0_2_pat00, &hane0_2_pat01, &hane0_2_pat02, &hane0_2_pat03,
    &hane0_2_pat04, &hane0_2_pat03, &hane0_2_pat02, &hane0_2_pat01};
static sprite_pattern hane0_3_pat00 = {2,
                                       {{-8, 32, 0, 470}, {-24, -24, 0, 469}}};
static sprite_pattern hane0_3_pat01 = {2,
                                       {{-26, 28, 0, 470}, {-30, -34, 0, 471}}};
static sprite_pattern hane0_3_pat02 = {2,
                                       {{-39, 24, 0, 470}, {-32, -32, 0, 472}}};
static sprite_pattern hane0_3_pat03 = {2,
                                       {{-46, 8, 0, 470}, {-40, -36, 0, 473}}};
static sprite_pattern hane0_3_pat04 = {2,
                                       {{-48, -8, 0, 470}, {-40, -24, 0, 474}}};
sprite_pattern *egg8hane0_pat3[16] = {
    &hane0_3_pat00, &hane0_3_pat01, &hane0_3_pat02, &hane0_3_pat03,
    &hane0_3_pat04, &hane0_3_pat03, &hane0_3_pat02, &hane0_3_pat01,
    &hane0_3_pat00, &hane0_3_pat01, &hane0_3_pat02, &hane0_3_pat03,
    &hane0_3_pat04, &hane0_3_pat03, &hane0_3_pat02, &hane0_3_pat01};
static sprite_pattern hane1_pat00 = {1, {{-16, -32, 0, 475}}};
static sprite_pattern hane1_pat01 = {1, {{-32, -16, 0, 476}}};
sprite_pattern *egg8hane1_pat[16] = {
    &hane1_pat00, 0, 0, 0, &hane1_pat01, 0, 0, 0,
    &hane1_pat00, 0, 0, 0, &hane1_pat01, 0, 0, 0};
static sprite_pattern hane2_1_pat00 = {1, {{-8, -32, 0, 477}}};
static sprite_pattern hane2_1_pat01 = {1, {{-20, -32, 0, 478}}};
static sprite_pattern hane2_1_pat02 = {1, {{-30, -28, 0, 479}}};
static sprite_pattern hane2_1_pat03 = {1, {{-37, -24, 0, 480}}};
static sprite_pattern hane2_1_pat04 = {1, {{-40, -8, 0, 481}}};
sprite_pattern *egg8hane2_pat1[16] = {
    &hane2_1_pat00, &hane2_1_pat01, &hane2_1_pat02, &hane2_1_pat03,
    &hane2_1_pat04, &hane2_1_pat03, &hane2_1_pat02, &hane2_1_pat01,
    &hane2_1_pat00, &hane2_1_pat01, &hane2_1_pat02, &hane2_1_pat03,
    &hane2_1_pat04, &hane2_1_pat03, &hane2_1_pat02, &hane2_1_pat01};
static sprite_pattern hane2_2_pat00 = {1, {{-8, -32, 0, 482}}};
static sprite_pattern hane2_2_pat01 = {1, {{-16, -32, 0, 483}}};
static sprite_pattern hane2_2_pat02 = {1, {{-28, -28, 0, 484}}};
static sprite_pattern hane2_2_pat03 = {1, {{-32, -24, 0, 485}}};
static sprite_pattern hane2_2_pat04 = {1, {{-32, -8, 0, 486}}};
sprite_pattern *egg8hane2_pat2[16] = {
    &hane2_2_pat00, &hane2_2_pat01, &hane2_2_pat02, &hane2_2_pat03,
    &hane2_2_pat04, &hane2_2_pat03, &hane2_2_pat02, &hane2_2_pat01,
    &hane2_2_pat00, &hane2_2_pat01, &hane2_2_pat02, &hane2_2_pat03,
    &hane2_2_pat04, &hane2_2_pat03, &hane2_2_pat02, &hane2_2_pat01};
static sprite_pattern **hane_base_tbl[6] = {egg8hane0_pat1, egg8hane0_pat2,
                                            egg8hane0_pat3, egg8hane1_pat,
                                            egg8hane2_pat1, egg8hane2_pat2};
static Sint32 (*hane_act_tbl[6])(sprite_status *, sprite_status *,
                                 sprite_status *) = {
    &egg8hane_ini,   &egg8hane_demo, &egg8hane_normal,
    &egg8hane_fire1, &egg8hane_wait, &egg8hane_kill};

void egg8hane(sprite_status *pActwk) {
    sprite_status *pMecawk, *pEggwk;
    Sint32 ret;

    pMecawk = &actwk[((Sint16 *)pActwk)[25]];
    pEggwk = &actwk[((Sint16 *)pMecawk)[25]];

    if (!(pMecawk->actfree[17] & 128)) {
        hane_no_reset(pActwk, pMecawk, pEggwk);
    }

    while ((ret = hane_act_tbl[pActwk->r_no0 / 2](pActwk, pMecawk, pEggwk)) < 0)
        ;

    if (ret == 0)
        return;

    if (pActwk->patno < 4) {

        pActwk->actflg &= 252;
    } else if (pActwk->patno < 8) {

        pActwk->actflg |= 2;
        pActwk->actflg &= 254;
    } else if (pActwk->patno < 12) {

        pActwk->actflg |= 3;
    } else {

        pActwk->actflg &= 253;
        pActwk->actflg |= 1;
    }

    actionsub(pActwk);
}

static Sint32 egg8hane_ini(sprite_status *pActwk, sprite_status *pMecawk,
                           sprite_status *pEggwk) {
    pActwk->r_no0 = 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 5;
    pActwk->sprhsize = 32;
    pActwk->sprvsize = 32;
    pActwk->sproffset = 8736;
    pActwk->patbase = egg8hane0_pat1;

    if (egg8hane_normal(pActwk, pMecawk, pEggwk) < 0) {
        return -1;
    }

    pActwk->colino = 0;
    pActwk->colicnt = 0;

    return 1;
}

static Sint32 egg8hane_demo(sprite_status *pActwk, sprite_status *pMecawk,
                            sprite_status *pEggwk) {
    egg8hane_posiset(pActwk, pMecawk);

    if (pEggwk->r_no0 == 4) {
        pActwk->colino = 0;
        pActwk->colicnt = 0;
    } else {
        pActwk->r_no0 = 4;
    }

    return 1;
}

static Sint32 egg8hane_normal(sprite_status *pActwk, sprite_status *pMecawk,
                              sprite_status *pEggwk) {
    if (pActwk->actfree[2] & 8) {
        pActwk->actfree[3] = 0;
        pActwk->r_no0 = 10;
        return -1;
    }

    if (pMecawk->r_no0 == 10 && pActwk->patno == 0) {
        pActwk->actfree[3] = 0;
        pActwk->r_no0 = 6;
        ++pMecawk->actfree[19];
        return 1;
    }

    pActwk->patbase = hane_base_tbl[pMecawk->actfree[10]];

    if (pMecawk->actfree[2] & 64) {
        hane_rol_l(pActwk);
    } else if (pMecawk->actfree[2] & 32) {
        hane_rol_r(pActwk);
    }

    if (!pActwk->actfree[14]) {
        pMecawk->actfree[11] = pActwk->patno;
    }

    egg8hane_posiset(pActwk, pMecawk);
    return 1;
}

static char reset_tbl_1mai[2] = {8, -1};
static char reset_tbl_2mai[2] = {8, 12};
static char reset_tbl_3mai[4] = {4, 8, 12, -1};
static char reset_tbl_4mai[4] = {0, 4, 8, 12};
static char *reset_pattbl[5] = {0, reset_tbl_1mai, reset_tbl_2mai,
                                reset_tbl_3mai, reset_tbl_4mai};

static Sint32 egg8hane_fire1(sprite_status *pActwk, sprite_status *pMecawk,
                             sprite_status *pEggwk) {
    Sint16 PosWk;
    char *pResetTbl;
    Sint32 lwk;
    static Sint16 hane_fire_tbl[16] = {3776, 3904, 3968, 3712, 3712, 3776,
                                       3840, 3904, 3968, 3904, 3840, 3712,
                                       3760, 3920, 3792, 3856};

    switch (pActwk->actfree[3]) {

    case 0:
        ++pActwk->actfree[3];

        if (pEggwk->actfree[16] == 1) {
            pActwk->patbase = egg8hane0_pat2;
        }

    case 1:
        pActwk->yposi.l -= 65536;
        if (pActwk->yposi.w.h <= 224) {

            ++pActwk->actfree[3];
            ++pMecawk->actfree[20];
        }

        break;

    case 2:
        if (pEggwk->actfree[16] <= pMecawk->actfree[20]) {

            pActwk->actfree[0] = 0;
            ++pActwk->actfree[3];
        }

        break;

    case 3:
        if (++pActwk->actfree[0] == (pActwk->actfree[14] + 1) * 30) {

            pActwk->sprpri = 3;
            ++pActwk->actfree[3];
            pActwk->actfree[0] = 0;

            pActwk->patno = 0;

            if (pEggwk->actfree[16] >= 3) {
                pActwk->patbase = egg8hane2_pat2;

                pActwk->xposi.w.h = hane_fire_tbl[pMecawk->actfree[21] / 2 +
                                                  pActwk->actfree[14]];

                ((Sint32 *)pActwk)[14] = 0;
                ((Sint32 *)pActwk)[4] = 229376;
            } else {
                pActwk->patbase = egg8hane0_pat2;

                if (pEggwk->actfree[16] != 2) {

                    if (actwk[0].xposi.w.h < 3840) {

                        pActwk->patno = 3;

                        lwk = 4088;
                        lwk -= actwk[0].xposi.w.h;
                        lwk *= -27146;
                        lwk >>= 16;
                        lwk &= 65535;
                        lwk += 480;
                        pActwk->yposi.w.h = lwk;

                        pActwk->xposi.w.h = 4040;
                        ((Sint32 *)pActwk)[14] = -211897;
                        ((Sint32 *)pActwk)[4] = 87759;
                    } else {
                        pActwk->patno = 13;

                        lwk = actwk[0].xposi.w.h;
                        lwk -= 3592;
                        lwk *= -27146;
                        lwk >>= 16;
                        lwk &= 65535;
                        lwk += 480;
                        pActwk->yposi.w.h = lwk;

                        pActwk->xposi.w.h = 3640;
                        ((Sint32 *)pActwk)[14] = 211897;
                        ((Sint32 *)pActwk)[4] = 87759;
                    }
                } else if (!pActwk->actfree[14]) {

                    pActwk->xposi.w.h = hane_fire_tbl[pMecawk->actfree[21] / 2 +
                                                      pActwk->actfree[14]];

                    ((Sint32 *)pActwk)[14] = 0;
                    ((Sint32 *)pActwk)[4] = 229376;
                } else {

                    pActwk->xposi.w.h = actwk[0].xposi.w.h;
                    ((Sint32 *)pActwk)[14] = 0;
                    ((Sint32 *)pActwk)[4] = 229376;
                }
            }
        }

        break;

    case 4:
        pActwk->xposi.l += ((Sint32 *)pActwk)[14];
        pActwk->yposi.l += ((Sint32 *)pActwk)[4];

        if (pActwk->patno != 0) {

            PosWk = 464;
        } else if (pActwk->patbase == egg8hane2_pat2) {
            PosWk = 448;
        } else {

            PosWk = 440;
        }

        if (pActwk->yposi.w.h >= PosWk) {

            pActwk->yposi.w.h = PosWk;
            ++pActwk->actfree[3];
            pActwk->actfree[0] = 0;

            soundset(180);
        }

        break;

    case 5:
        if (++pActwk->actfree[0] == 30) {

            pActwk->sprpri = 5;
            ++pActwk->actfree[3];
            pActwk->colino = 0;
            pActwk->colicnt = 0;
            pActwk->actfree[0] = 0;
        }

        break;

    case 6:
        if (++pActwk->actfree[0] == 60) {

            ++pMecawk->actfree[19];

            pActwk->actfree[0] = 0;
            pActwk->actfree[3] = 0;
            pActwk->r_no0 = 8;

            pResetTbl = reset_pattbl[pEggwk->actfree[16]];

            if (!pActwk->actfree[14]) {
                pMecawk->actfree[11] = pResetTbl[0];
            }

            pActwk->patno = pResetTbl[pActwk->actfree[14]];
            pMecawk->actfree[10] = 0;

        } else if (!(pActwk->actfree[0] / 2 & 1)) {

            return 1;
        }

        return 0;
    }

    return 1;
}

static Sint32 egg8hane_wait(sprite_status *pActwk, sprite_status *pMecawk,
                            sprite_status *pEggwk) {
    if (pActwk->actfree[2] & 8) {
        frameout(pActwk);
        return 0;
    }

    if (pMecawk->actfree[2] & 8) {

        pActwk->r_no0 = 4;
    }

    return 0;
}

static Sint32 egg8hane_kill(sprite_status *pActwk, sprite_status *pMecawk,
                            sprite_status *pEggwk) {
    if (!pActwk->actfree[3]) {

        pActwk->colino = 0;
        pActwk->colicnt = 0;

        ((Sint32 *)pActwk)[14] = 65536;
        ((Sint32 *)pActwk)[4] = -196608;

        if (pActwk->patno > 8) {
            ((Sint32 *)pActwk)[14] *= -1;
        }

        pActwk->actfree[0] = 0;
        pActwk->actfree[1] = 0;
        ++pActwk->actfree[3];

        if (pActwk->patbase != egg8hane1_pat) {

            pActwk->patbase = egg8hane0_pat3;
        } else {
            pActwk->patbase = egg8hane2_pat1;
        }
    }

    pActwk->xposi.l += ((Sint32 *)pActwk)[14];
    pActwk->yposi.l += ((Sint32 *)pActwk)[4];
    ((Sint32 *)pActwk)[4] += 12288;

    if (pActwk->yposi.w.h >= 480) {
        frameout(pActwk);
        return 0;
    }

    if (++pActwk->actfree[0] == 4) {
        pActwk->actfree[0] = 0;

        if (++pActwk->patno > 15) {
            pActwk->patno = 0;
        }
    }

    ++pActwk->actfree[1];

    if (pActwk->actfree[1] / 2 & 1) {

        return 0;
    }
    return 1;
}

static char hane_reset_tbl_2mai[12] = {-1, 0,  1, -1, 0,  -1,
                                       1,  -1, 0, 1,  -1, -1};
static char hane_reset_tbl_3mai[16] = {-1, 0, 1,  2, 2, -1, 0, 1,
                                       1,  2, -1, 0, 0, 1,  2, -1};
static char *hane_no_reset_tbl[4] = {0, 0, hane_reset_tbl_2mai,
                                     hane_reset_tbl_3mai};

static void hane_no_reset(sprite_status *pActwk, sprite_status *pMecawk,
                          sprite_status *pEggwk) {
    char *pResetTbl;
    Sint32 HaneNum, KillNo;

    if (pMecawk->actfree[17] == pActwk->actfree[14]) {

        pActwk->actfree[2] |= 8;
        return;
    }

    if (pEggwk->actfree[16] == 1) {

        pActwk->actfree[14] = 0;
        return;
    }

    HaneNum = pEggwk->actfree[16];
    KillNo = pMecawk->actfree[17];
    pResetTbl = hane_no_reset_tbl[HaneNum] + KillNo * 4;

    pActwk->actfree[14] = pResetTbl[pActwk->actfree[14]];
}

static void egg8hane_posiset(sprite_status *pActwk, sprite_status *pMecawk) {
    hane_offs_set(pActwk, pMecawk);
    pActwk->xposi.w.h = pMecawk->xposi.w.h + ((Sint16 *)pActwk)[26];
    pActwk->yposi.w.h = pMecawk->yposi.w.h + ((Sint16 *)pActwk)[27];
}

static hane_offs hane0_offs_tbl1[16] = {
    {0, -40, 0, -36, 180},     {19, -36, 18, -31, 181},
    {31, -32, 25, -25, 182},   {38, -16, 31, -18, 183},
    {40, 0, 36, 0, 184},       {38, 16, 31, 18, 183},
    {31, 32, 25, 25, 182},     {19, 36, 18, 31, 181},
    {0, 40, 0, 36, 180},       {-19, 36, -18, 31, 181},
    {-31, 32, -25, 25, 182},   {-38, 16, -31, 18, 183},
    {-40, 0, -36, 0, 184},     {-38, -16, -31, -18, 183},
    {-31, -32, -25, -25, 182}, {-19, -36, -18, -31, 181}};
static hane_offs hane1_offs_tbl[16] = {
    {0, -32, 0, -36, 185}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
    {32, 0, 36, 0, 186},   {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
    {0, 32, 0, 36, 185},   {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
    {-32, 0, -36, 0, 186}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};
static hane_offs hane2_offs_tbl[16] = {
    {0, -32, 0, -36, 187},     {12, -29, 18, -31, 188},
    {23, -23, 25, -25, 189},   {29, -12, 31, -18, 190},
    {32, 0, 36, 0, 191},       {29, 12, 31, 18, 190},
    {23, 23, 25, 25, 189},     {12, 29, 18, 31, 188},
    {0, 32, 0, 36, 187},       {-12, 29, -18, 31, 188},
    {-23, 23, -25, 25, 189},   {-29, 12, -31, 18, 190},
    {-32, 0, -36, 0, 191},     {-29, -12, -31, -18, 190},
    {-23, -23, -25, -25, 189}, {-12, -29, -18, -31, 188}};
static hane_offs *hane_offs_tbl[6] = {hane0_offs_tbl1, hane0_offs_tbl1,
                                      hane0_offs_tbl1, hane1_offs_tbl,
                                      hane2_offs_tbl,  hane2_offs_tbl};

static void hane_offs_set(sprite_status *pActwk, sprite_status *pMecawk) {
    hane_offs *pOffsTbl;
    Sint16 hankeiwk;

    pOffsTbl = hane_offs_tbl[pMecawk->actfree[10]] + pActwk->patno;

    hankeiwk = ((Sint16 *)pMecawk)[30];
    ((Sint16 *)pActwk)[26] = (pOffsTbl->xDst * hankeiwk >> 8) + pOffsTbl->xOffs;

    ((Sint16 *)pActwk)[27] = (pOffsTbl->yDst * hankeiwk >> 8) + pOffsTbl->yOffs;

    pActwk->colino = pOffsTbl->ColiNo;
    pActwk->colicnt = 2;
}

static void hane_rol_l(sprite_status *pActwk) {
    if ((char)--pActwk->patno < 0) {
        pActwk->patno = 15;
    }

    rol_sound(pActwk);
}

static void hane_rol_r(sprite_status *pActwk) {
    if (++pActwk->patno >= 16) {
        pActwk->patno = 0;
    }

    rol_sound(pActwk);
}

static void rol_sound(sprite_status *pActwk) {
    if (!pActwk->actfree[14] && (pActwk->patno & 3) != 2) {
        soundset(186);
    }
}

static Sint16 hibana0[6] = {-16, 0, -16, -8, -4, 0};
static Sint16 hibana1[6] = {4, 0, 16, -8, 8, 0};
static Sint16 *hibana_offs_tbl[2] = {hibana0, hibana1};
static sprite_pattern hibana_pat0 = {1, {{-8, -4, 0, 487}}};
static sprite_pattern hibana_pat1 = {1, {{-8, -4, 0, 488}}};
sprite_pattern *egg8hibana_pat[2] = {&hibana_pat0, &hibana_pat1};
static Uint32 (*hibana_act_tbl[2])(sprite_status *) = {&egg8hibana_ini,
                                                       &egg8hibana_1};

void egg8hibana(sprite_status *pActwk) {
    if (hibana_act_tbl[pActwk->r_no0 / 2](pActwk) != 0) {
        actionsub(pActwk);
    }
}

static Uint32 egg8hibana_ini(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 4;
    pActwk->sprhsize = 8;
    pActwk->sprvsize = 8;
    pActwk->sproffset = 990;
    pActwk->patbase = egg8hibana_pat;

    egg8hibana_1(pActwk);
}

static Uint32 egg8hibana_1(sprite_status *pActwk) {
    sprite_status *pMecawk;
    Sint16 *pOffsTbl;

    pMecawk = &actwk[((Sint16 *)pActwk)[25]];
    pActwk->xposi.w.h = pMecawk->xposi.w.h;
    pActwk->yposi.w.h = pMecawk->yposi.w.h;

    pOffsTbl = hibana_offs_tbl[pActwk->actfree[10]] + pActwk->actfree[3] / 2;
    pActwk->xposi.w.h += *pOffsTbl++;
    pActwk->yposi.w.h += *pOffsTbl;

    if (++pActwk->actfree[0] == 150) {
        frameout(pActwk);
        return 0;
    }

    if (++pActwk->actfree[1] == 4) {
        if ((pActwk->actfree[3] += 4) > 8) {
            pActwk->actfree[3] = 0;
        }

        pActwk->actfree[1] = 0;

        if (++pActwk->patno > 2) {
            pActwk->patno = 0;
        }
    }

    if (pActwk->patno == 2) {
        return 0;
    }
    return 1;
}

static void make_meca(sprite_status *pActwk) {
    sprite_status *pNewact;

    if (actwkchk2(pActwk, &pNewact) == 0) {
        ((Sint16 *)pNewact)[25] = pActwk - actwk;
        ((Sint16 *)pActwk)[25] = pNewact - actwk;
        pNewact->actno = 65;
        pNewact->xposi.w.h = pActwk->xposi.w.h;
        pNewact->yposi.w.h = pActwk->yposi.w.h;
    }
}

static void make_hane(sprite_status *pActwk) {
    sprite_status *pNewact;
    Uint8 hanecnt, hanepat;

    hanecnt = hanepat = 0;

    do {
        if (actwkchk2(pActwk, &pNewact) != 0)
            return;

        ((Sint16 *)pNewact)[25] = pActwk - actwk;
        pNewact->actfree[14] = hanecnt;
        pNewact->actno = 64;
        pNewact->xposi.w.h = pActwk->xposi.w.h;
        pNewact->yposi.w.h = pActwk->yposi.w.h;
        pNewact->patno = hanepat;
        ++hanecnt;
        hanepat += 4;
    } while (hanecnt != 4);
}

static void make_hibana(sprite_status *pActwk) {
    Uint8 cnt;
    sprite_status *pNewact;

    cnt = 0;

    do {
        if (actwkchk(&pNewact) != 0)
            break;

        ((Sint16 *)pNewact)[25] = pActwk - actwk;
        pNewact->actno = 66;
        pNewact->xposi.w.h = pActwk->xposi.w.h;
        pNewact->yposi.w.h = pActwk->yposi.w.h;
        pNewact->actfree[10] = cnt++;

    } while (cnt != 2);
}

static void egg8_spd_set(sprite_status *pActwk, sprite_status *pMecawk) {
    egg8spd spd_tbl[5] = {{81920, 0, 4, 0},
                          {114688, 65536, 2, 0},
                          {106496, 57344, 3, 0},
                          {98304, 49152, 4, 0},
                          {90112, 40960, 5, 0}};

    ((Sint32 *)pActwk)[14] = spd_tbl[pActwk->actfree[16]].X_Speed;

    ((Sint32 *)pActwk)[4] = spd_tbl[pActwk->actfree[16]].Y_Speed;

    pMecawk->actfree[13] = spd_tbl[pActwk->actfree[16]].Roll_Speed;
}

static void tobi_set(sprite_status *pActwk) {
    Sint16 sinwk, coswk;

    pActwk->xposi.w.h -= ((Sint16 *)pActwk)[26];
    pActwk->yposi.w.h -= ((Sint16 *)pActwk)[27];

    if ((((Uint16 *)pActwk)[23] += 560) >= 32768) {
        ((Uint16 *)pActwk)[23] = 32768;
    }

    sinset(pActwk->actfree[1], &sinwk, &coswk);

    if (!(pActwk->actfree[3] & 1)) {
        coswk *= -1;
    }

    coswk = coswk * 41 >> 8;
    sinwk = sinwk * -104 >> 8;
    ((Sint16 *)pActwk)[26] = coswk;
    ((Sint16 *)pActwk)[27] = sinwk;
    pActwk->xposi.w.h += coswk;
    pActwk->yposi.w.h += sinwk;
}

static void bom_set(sprite_status *pActwk) {
    sprite_status *pNewact;
    Sint32 rnd;
    int_union wk1, wk2;

    if (pActwk->actfree[0] % 2 == 0) {

        if (actwkchk(&pNewact) == 0) {
            pNewact->actno = 24;
            pNewact->r_no1 = 255;
            pNewact->userflag.b.l = -1;

            rnd = random();
            wk2.l = (Sint16)(rnd & 65535);
            wk1.l = (Sint16)((rnd & 4294901760) >> 16);
            pNewact->xposi.w.h = wk1.l % 128 + 3840;
            pNewact->yposi.w.h = wk2.l % 80 + 384;

            soundset(158);
        }
    }
}
