#include "../equ.h"
#include "boss_5.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../goal.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../ridechk.h"
#include "../score.h"
#include "scr53c.h"

static Sint32 egg5_ini(sprite_status *pActwk);
static Sint32 egg5_1(sprite_status *pActwk);
static Sint32 egg5_esc(sprite_status *pActwk);
static Sint32 egg5meca1_ini(sprite_status *pActwk);
static Sint32 egg5meca1_scrset(sprite_status *pActwk);
static Sint32 egg5meca1_scrreset(sprite_status *pActwk);
static Sint32 egg5meca1_normal(sprite_status *pActwk);
static Sint32 egg5meca2_ini(sprite_status *pActwk);
static Sint32 egg5meca2_ue(sprite_status *pActwk);
static Sint32 egg5meca2_down1(sprite_status *pActwk);
static Sint32 egg5meca2_down2(sprite_status *pActwk);
static Sint32 egg5meca2_kezu(sprite_status *pActwk);
static Sint32 egg5meca2_esc(sprite_status *pActwk);
static Sint32 egg5meca2_end(sprite_status *pActwk);
static void my_rideonchk(sprite_status *pActwk);
static void egg5meca3_ini(sprite_status *pActwk);
static void egg5meca3_1(sprite_status *pActwk);
static void egg5pipe_ini(sprite_status *pActwk);
static void egg5pipe_1(sprite_status *pActwk);
static Sint32 egg5catch_ini(sprite_status *pActwk);
static Sint32 egg5catch_1st(sprite_status *pActwk);
static Sint32 egg5catch_return(sprite_status *pActwk);
static Sint32 egg5catch_wait(sprite_status *pActwk);
static Sint32 egg5catch_go1(sprite_status *pActwk);
static Sint32 egg5catch_go2(sprite_status *pActwk);
static Sint32 egg5catch_fire(sprite_status *pActwk);
static void lever_on(sprite_status *pActwk);
static void lever_off(sprite_status *pActwk);
static Sint32 egg5bomb_ini(sprite_status *pActwk);
static Sint32 egg5bomb_1(sprite_status *pActwk);
static Sint32 egg5bomb_2(sprite_status *pActwk);
static Sint32 egg5bomb_3(sprite_status *pActwk);
static Sint32 egg5bomb_4(sprite_status *pActwk);
static Sint32 egg5bomb2_ini(sprite_status *pActwk);
static Sint32 egg5bomb2_1(sprite_status *pActwk);
static Sint32 egg5bomb2_2(sprite_status *pActwk);
static Sint32 egg5bomb_hit(sprite_status *pActwk);
static Sint32 egg5hibana_ini(sprite_status *pActwk);
static Sint32 egg5hibana_1(sprite_status *pActwk);
static Sint32 egg5hibana_2(sprite_status *pActwk);
static Sint32 hibana_delchk(sprite_status *pActwk);
static void egg5hari_ini(sprite_status *pActwk);
static void egg5hari_1(sprite_status *pActwk);
static void egg5hari_2(sprite_status *pActwk);
static void egg5hari_3(sprite_status *pActwk);
static void egg5hari_4(sprite_status *pActwk);
static void egg5hari_chk(sprite_status *pActwk);
static void egg5belt_ini(sprite_status *pActwk);
static void egg5belt_0(sprite_status *pActwk);
static void egg5belt_1(sprite_status *pActwk);
static void egg5belt_2(sprite_status *pActwk);
static void egg5belt_3(sprite_status *pActwk);
static Sint32 belt_spdset(sprite_status *pActwk);
static void belt_hitpnt(Sint16 subact, sprite_status *pActwk);
static void belt_sncspd(Sint32 wD6, sprite_status *pActwk);
static void belt_sncxmax(Sint16 subact);
static void belt_anime(sprite_status *pActwk);
static void set_belt_col(sprite_status *pActwk);
static Sint32 chk_belt_on(void);
static Sint32 egg5bakuha_ini(sprite_status *pActwk);
static Sint32 egg5bakuha_1(sprite_status *pActwk);
static Sint32 frameout_sp4(sprite_status *pActwk);
static void bom_set(sprite_status *pActwk);
static void make_meca2(sprite_status *pActwk);
static void make_egg5(sprite_status *pActwk);
static void make_catch(sprite_status *pActwk);
static void make_pipe(sprite_status *pActwk);
static void make_meca3(sprite_status *pActwk);
static void make_bomb(sprite_status *pActwk);
static void make_bomb2(sprite_status *pActwk);
static void make_belt(sprite_status *pActwk);
static void chg_mstno(Uint8 num, sprite_status *pActwk);
static void chg_mstno2(Uint8 num, Sint16 subact);
static Sint32 frameout_chk(sprite_status *pActwk);

extern void colorset2(Sint32 ColorNo);

static Uint8 egg5_pchg0[4] = {19, 0, 1, 255};
static Uint8 egg5_pchg1[6] = {3, 2, 4, 3, 5, 255};
static Uint8 egg5_pchg2[6] = {3, 6, 8, 7, 9, 255};
static Uint8 egg5_pchg3[10] = {2, 10, 13, 12, 15, 11, 14, 12, 15, 255};
static Uint8 *egg5_pchg[4] = {egg5_pchg0, egg5_pchg1, egg5_pchg2, egg5_pchg3};
static Uint8 egg5catch_pchg0[3] = {255, 0, 255};
static Uint8 egg5catch_pchg1[4] = {5, 1, 2, 255};
static Uint8 *egg5catch_pchg[2] = {egg5catch_pchg0, egg5catch_pchg1};
static Uint8 egg5hibana_pchg0[7] = {3, 0, 1, 2, 3, 4, 255};
static Uint8 *egg5hibana_pchg[1] = {egg5hibana_pchg0};
static Uint8 egg5bakuha_pat0[12] = {3, 0, 5, 6, 3, 4, 0, 1, 2, 3, 4, 252};
static Uint8 *egg5bakuha_pchg[1] = {egg5bakuha_pat0};
static sprite_pattern spr_egg5_pat00 = {1, {{-24, -28, 0, 422}}};
static sprite_pattern spr_egg5_pat01 = {1, {{-24, -28, 0, 423}}};
static sprite_pattern spr_egg5_pat02 = {1, {{-24, -48, 0, 424}}};
static sprite_pattern spr_egg5_pat03 = {1, {{-24, -48, 0, 425}}};
static sprite_pattern spr_egg5_pat04 = {1, {{-24, -32, 0, 426}}};
static sprite_pattern spr_egg5_pat05 = {1, {{-24, -32, 0, 427}}};
static sprite_pattern spr_egg5_pat06 = {1, {{-24, -44, 0, 428}}};
static sprite_pattern spr_egg5_pat07 = {1, {{-24, -44, 0, 429}}};
static sprite_pattern spr_egg5_pat08 = {1, {{-24, -28, 0, 430}}};
static sprite_pattern spr_egg5_pat09 = {1, {{-24, -28, 0, 431}}};
static sprite_pattern spr_egg5_pat10 = {1, {{-32, -52, 0, 432}}};
static sprite_pattern spr_egg5_pat11 = {1, {{-32, -48, 0, 433}}};
static sprite_pattern spr_egg5_pat12 = {1, {{-32, -50, 0, 434}}};
static sprite_pattern spr_egg5_pat13 = {1, {{-32, -36, 0, 435}}};
static sprite_pattern spr_egg5_pat14 = {1, {{-32, -32, 0, 436}}};
static sprite_pattern spr_egg5_pat15 = {1, {{-32, -34, 0, 437}}};
sprite_pattern *egg5_pat[16] = {
    &spr_egg5_pat00, &spr_egg5_pat01, &spr_egg5_pat02, &spr_egg5_pat03,
    &spr_egg5_pat04, &spr_egg5_pat05, &spr_egg5_pat06, &spr_egg5_pat07,
    &spr_egg5_pat08, &spr_egg5_pat09, &spr_egg5_pat10, &spr_egg5_pat11,
    &spr_egg5_pat12, &spr_egg5_pat13, &spr_egg5_pat14, &spr_egg5_pat15};
static sprite_pattern spr_egg5meca1_pat0 = {1, {{-32, -24, 0, 438}}};
sprite_pattern *egg5meca1_pat[1] = {&spr_egg5meca1_pat0};
static sprite_pattern spr_egg5meca2_pat0 = {1, {{-32, -24, 0, 439}}};
static sprite_pattern spr_egg5meca2_pat1 = {1, {{-32, -24, 0, 440}}};
static sprite_pattern spr_egg5meca2_pat2 = {1, {{-32, -24, 0, 441}}};
static sprite_pattern spr_egg5meca2_pat3 = {1, {{-32, -24, 0, 442}}};
static sprite_pattern spr_egg5meca2_pat4 = {1, {{-32, -24, 0, 443}}};
static sprite_pattern spr_egg5meca2_pat5 = {1, {{-32, -24, 0, 444}}};
sprite_pattern *egg5meca2_pat[6] = {&spr_egg5meca2_pat0, &spr_egg5meca2_pat1,
                                    &spr_egg5meca2_pat2, &spr_egg5meca2_pat3,
                                    &spr_egg5meca2_pat4, &spr_egg5meca2_pat5};
static sprite_pattern spr_egg5meca3_pat0 = {1, {{-16, -8, 0, 445}}};
sprite_pattern *egg5meca3_pat[1] = {&spr_egg5meca3_pat0};
static sprite_pattern spr_egg5pipe_pat0 = {1, {{-28, -32, 0, 446}}};
static sprite_pattern spr_egg5pipe_pat1 = {1, {{-28, -32, 0, 447}}};
static sprite_pattern spr_egg5pipe_pat2 = {1, {{-28, -32, 0, 448}}};
static sprite_pattern spr_egg5pipe_pat3 = {1, {{-28, -32, 0, 449}}};
sprite_pattern *egg5pipe_pat[4] = {&spr_egg5pipe_pat0, &spr_egg5pipe_pat1,
                                   &spr_egg5pipe_pat2, &spr_egg5pipe_pat3};
static sprite_pattern spr_egg5catch_pat0 = {1, {{-16, -12, 0, 450}}};
static sprite_pattern spr_egg5catch_pat1 = {1, {{-16, -12, 0, 451}}};
static sprite_pattern spr_egg5catch_pat2 = {1, {{-16, -12, 0, 452}}};
sprite_pattern *egg5catch_pat[3] = {&spr_egg5catch_pat0, &spr_egg5catch_pat1,
                                    &spr_egg5catch_pat2};
static sprite_pattern spr_egg5bomb_pat0 = {1, {{-12, -12, 0, 453}}};
static sprite_pattern spr_egg5bomb_pat1 = {1, {{-8, -8, 0, 454}}};
sprite_pattern *egg5bomb_pat[2] = {&spr_egg5bomb_pat0, &spr_egg5bomb_pat1};
static sprite_pattern spr_egg5hibana_pat0 = {1, {{0, 0, 0, 455}}};
static sprite_pattern spr_egg5hibana_pat1 = {1, {{0, 0, 0, 456}}};
static sprite_pattern spr_egg5hibana_pat2 = {1, {{0, 0, 0, 457}}};
static sprite_pattern spr_egg5hibana_pat3 = {1, {{0, 0, 0, 458}}};
static sprite_pattern spr_egg5hibana_pat4 = {1, {{0, 0, 0, 459}}};
sprite_pattern *egg5hibana_pat[5] = {&spr_egg5hibana_pat0, &spr_egg5hibana_pat1,
                                     &spr_egg5hibana_pat2, &spr_egg5hibana_pat3,
                                     &spr_egg5hibana_pat4};
static sprite_pattern spr_egg5hari_pat0 = {1, {{-16, -32, 0, 460}}};
sprite_pattern *egg5hari_pat[1] = {&spr_egg5hari_pat0};
extern Sint16 scr_dir_tbl[6];
extern sprite_pattern *bakupat[7];

void egg5(sprite_status *pActwk) {
    static Sint32 (*act_tbl[3])(sprite_status *) = {&egg5_ini, &egg5_1,
                                                    &egg5_esc};

    if (act_tbl[pActwk->r_no0 / 2](pActwk) != 0) {
        if (pActwk->mstno.b.h)
            patchg(pActwk, egg5_pchg);
        actionsub(pActwk);
    } else
        frameout(pActwk);
}

static Sint32 egg5_ini(sprite_status *pActwk) {
    Sint32 ret;

    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 5;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 16;
    pActwk->sproffset = 700;
    pActwk->patbase = egg5_pat;
    make_catch(pActwk);
    ret = egg5_1(pActwk);
    return ret;
}

static Sint32 egg5_1(sprite_status *pActwk) {
    Sint16 subact;
    Sint32 ret;

    if (!(pActwk->actfree[2] & 4)) {
        subact = ((Sint16 *)pActwk)[25];
        pActwk->yposi.w.h = actwk[subact].yposi.w.h;
        if (actwk[subact].patno >= 4) {

            if (!pActwk->actfree[0]) {

                chg_mstno(2, pActwk);
                pActwk->actfree[0] = 68;
            } else {
                --pActwk->actfree[0];
                if (pActwk->actfree[0] == 20) {

                    chg_mstno(0, pActwk);
                }
            }
        }
        return 1;
    }

    pActwk->sprpri = 3;
    pActwk->actfree[0] = 0;
    pActwk->r_no0 += 2;
    chg_mstno(3, pActwk);
    pActwk->yposi.w.h += 6;
    scoreup(100);
    ret = egg5_esc(pActwk);
    return ret;
}

static Sint32 egg5_esc(sprite_status *pActwk) {
    Sint32 ret = 1;

    pActwk->xposi.l += 73728;
    if (pActwk->xposi.w.h >= 3616)
        ret = 0;
    return ret;
}

void egg5meca1(sprite_status *pActwk) {
    Sint16 subact;
    static Sint32 (*act_tbl[4])(sprite_status *) = {
        &egg5meca1_ini, &egg5meca1_scrset, &egg5meca1_scrreset,
        &egg5meca1_normal};

    if (act_tbl[pActwk->r_no0 / 2](pActwk) != 0) {
        subact = ((Sint16 *)pActwk)[26];
        if (!(actwk[subact].actfree[2] & 8))
            actionsub(pActwk);
    }
}

static Sint32 egg5meca1_ini(sprite_status *pActwk) {
    Sint32 ret;

    colorset2(6);
    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 4;
    pActwk->sprhsize = 32;
    pActwk->sprvsize = 24;
    pActwk->sproffset = 8892;
    pActwk->patbase = egg5meca1_pat;
    make_meca2(pActwk);
    ret = egg5meca1_scrset(pActwk);
    return ret;
}

static Sint32 egg5meca1_scrset(sprite_status *pActwk) {
    Sint16 wD0;

    if (!(pActwk->actfree[2] & 2)) {
        if (scra_vline > 200) {
            bossflag = 5;
            pActwk->actfree[2] |= 2;
        }

        scra_vline += 6;
    }

    scralim_right = 3264;
    scralim_n_right = 3264;
    wD0 = actwk[0].xposi.w.h - 160;
    if (wD0 >= scralim_left) {
        if (actwk[0].xposi.w.h >= 3424) {

            pActwk->actfree[2] &= 253;
            sub_sync(103);
            bossstart = 5;
            pActwk->r_no0 = 6;
            scralim_right = 3264;
            scralim_n_right = 3264;
            wD0 = 3264;
        }

        scralim_left = wD0;
        scralim_n_left = wD0;
    }
    return 1;
}

static Sint32 egg5meca1_scrreset(sprite_status *pActwk) {
    Sint16 wD0, wD1, wD2, wD3;

    wD0 = scr_dir_tbl[1];
    wD1 = scr_dir_tbl[2];
    wD2 = scr_dir_tbl[3];
    wD3 = scr_dir_tbl[4];
    scralim_right += 6;
    scralim_n_right += 6;
    if (wD1 <= scralim_right) {

        bossstart = 0;
        scralim_right = wD1;
        scralim_n_right = wD1;
        if (!generate_flag)
            sub_sync(26);
        else
            sub_sync(25);
        genecolor();
        pActwk->r_no0 = 6;
    }
    return 1;
}

static Sint32 egg5meca1_normal(sprite_status *pActwk) {
    Sint16 subact;
    Sint16 wD0;

    subact = ((Sint16 *)pActwk)[26];
    if (actwk[subact].actfree[2] & 128)
        pActwk->actfree[20] = 8;
    actwk[subact].actfree[2] &= 127;

    if (pActwk->actfree[20]) {
        --pActwk->actfree[20];
        wD0 = 2;
        if (pActwk->actfree[20] & 2)
            wD0 *= -1;
        scralim_down += wD0;
        scralim_n_down += wD0;
    }
    return 1;
}

void egg5meca2(sprite_status *pActwk) {
    Sint16 subact;
    static Sint32 (*act_tbl[7])(sprite_status *) = {
        &egg5meca2_ini,  &egg5meca2_ue,  &egg5meca2_down1, &egg5meca2_down2,
        &egg5meca2_kezu, &egg5meca2_esc, &egg5meca2_end};

    if (act_tbl[pActwk->r_no0 / 2](pActwk) != 0) {
        if (!(pActwk->actfree[2] & 8)) {

            pActwk->xposi.w.h = 3536;
            ++pActwk->actfree[1];

            if (pActwk->actfree[1] & 2) {
                subact = ((Sint16 *)pActwk)[27];
                if (((Sint16 *)&actwk[subact])[27] >= 1024) {
                    if (((Sint16 *)&actwk[subact])[27] >= 1280) {
                        ++pActwk->xposi.w.h;
                        if (!(pActwk->actfree[10] & 1)) {
                            pActwk->actfree[10] ^= 1;
                            actionsub(pActwk);
                            return;
                        }
                        pActwk->actfree[10] ^= 1;
                        pActwk->xposi.w.h -= 2;
                        actionsub(pActwk);
                        return;
                    }

                    if (pActwk->actfree[10] & 1)
                        --pActwk->xposi.w.h;
                    pActwk->actfree[10] ^= 1;
                }
            }

            actionsub(pActwk);
        }
    }
}

static Sint32 egg5meca2_ini(sprite_status *pActwk) {
    Sint32 ret;

    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 4;
    pActwk->sprhsize = 32;
    pActwk->sprvsize = 112;
    pActwk->sproffset = 8892;
    pActwk->patbase = egg5meca2_pat;
    make_egg5(pActwk);
    make_meca3(pActwk);
    make_pipe(pActwk);
    make_belt(pActwk);
    pActwk->colino = 62;
    pActwk->colicnt = 2;
    ((Sint16 *)pActwk)[33] = 2400;
    ret = egg5meca2_ue(pActwk);
    return ret;
}

static Sint32 egg5meca2_ue(sprite_status *pActwk) {
    if (!pActwk->colino) {
        soundset(172);
        pActwk->r_no0 = 4;
        pActwk->colicnt = 0;
    }
    return 1;
}

static Sint32 egg5meca2_down1(sprite_status *pActwk) {
    Sint32 ret = 1;

    ++pActwk->actfree[0];
    if (pActwk->actfree[0] != 60) {
        if (!(pActwk->actfree[0] & 1)) {
            if (!(pActwk->actfree[0] & 2))
                pActwk->yposi.w.h -= 2;
            else
                pActwk->yposi.w.h += 2;
        }
    } else {

        pActwk->actfree[0] = 0;
        pActwk->r_no0 += 2;
        ((Sint32 *)pActwk)[4] = 77824;
        chg_mstno2(1, ((Sint16 *)pActwk)[26]);
        ((Sint16 *)pActwk)[30] = 648;
        ((Sint16 *)pActwk)[29] = 640;
        ret = egg5meca2_down2(pActwk);
    }
    return ret;
}

static Sint32 egg5meca2_down2(sprite_status *pActwk) {

    if (pActwk->actfree[3] == 1) {

        if (pActwk->yposi.w.h < ((Sint16 *)pActwk)[29]) {

            soundset(180);
            ++pActwk->actfree[3];
        } else
            pActwk->yposi.l -= 32768;
        return 1;
    }

    if (pActwk->yposi.w.h >= ((Sint16 *)pActwk)[30]) {

        soundset(180);
        pActwk->actfree[2] |= 128;
        make_hibana2(pActwk);
        if (pActwk->actfree[3]) {

            if (!pActwk->patno) {
                soundset(196);

                ++pActwk->patno;
                chg_mstno2(0, ((Sint16 *)pActwk)[26]);
            }

            pActwk->r_no0 = 8;
            pActwk->actfree[3] = 0;
            return 1;
        }
        ++pActwk->actfree[3];
    } else
        pActwk->yposi.l += 77824;
    return 1;
}

static Sint32 egg5meca2_kezu(sprite_status *pActwk) {
    Sint16 subact;
    Sint16 wD0;
    Sint16 meca2_ytbl[4] = {0, 8, 8, 8};

    my_rideonchk(pActwk);
    if (((Sint16 *)pActwk)[33] % 6 == 0) {
        if (!(pActwk->actfree[2] & 2))
            make_hibana1();
        pActwk->actfree[2] |= 2;
    } else
        pActwk->actfree[2] &= 253;

    if (((Sint16 *)pActwk)[33] <= 0) {

        pActwk->actfree[2] |= 4;
        subact = ((Sint16 *)pActwk)[26];
        actwk[subact].actfree[2] |= 4;
        pActwk->r_no0 = 10;
        ++pActwk->patno;

        subact = ((Sint16 *)pActwk)[25];
        actwk[subact].r_no0 = 4;
        return 1;
    }
    if (((Sint16 *)pActwk)[33] != 1800) {
        if (((Sint16 *)pActwk)[33] != 1200) {
            if (((Sint16 *)pActwk)[33] != 600)
                return 1;
        }
    }

    make_hibana2(pActwk);
    wD0 = meca2_ytbl[pActwk->patno];
    ((Sint16 *)pActwk)[30] = pActwk->yposi.w.h + wD0;
    wD0 /= 2;
    ((Sint16 *)pActwk)[29] = pActwk->yposi.w.h + wD0;
    pActwk->r_no0 = 6;
    ++pActwk->patno;
    return 1;
}

static Sint32 egg5meca2_esc(sprite_status *pActwk) {
    my_rideonchk(pActwk);
    ++pActwk->actfree[0];
    if (pActwk->actfree[0] == 60) {

        pActwk->actfree[0] = 0;
        pActwk->actfree[1] = 0;
        pActwk->r_no0 = 12;

        return 1;
    }

    if (pActwk->actfree[0] % 18 == 0)
        make_hibana2(pActwk);
    return 1;
}

static Sint32 egg5meca2_end(sprite_status *pActwk) {
    if (pActwk->actfree[0] >= 120) {

        pActwk->actfree[2] |= 8;
        return 1;
    }
    my_rideonchk(pActwk);
    ++pActwk->actfree[0];
    bom_set(pActwk);
    if (!(pActwk->actfree[0] & 2)) {

        pActwk->actfree[2] |= 8;
        return 1;
    }

    pActwk->actfree[2] &= 247;
    return 1;
}

static void my_rideonchk(sprite_status *pActwk) {
    ride_on_chk(pActwk, &actwk[0]);
}

void egg5meca3(sprite_status *pActwk) {
    Sint16 subact;
    static void (*act_tbl[2])(sprite_status *) = {&egg5meca3_ini, &egg5meca3_1};

    act_tbl[pActwk->r_no0 / 2](pActwk);
    subact = ((Sint16 *)pActwk)[25];
    if (!(actwk[subact].actfree[2] & 8))
        actionsub(pActwk);
}

static void egg5meca3_ini(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 5;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 16;
    pActwk->sproffset = 8892;
    pActwk->patbase = egg5meca3_pat;
    egg5meca3_1(pActwk);
}

static void egg5meca3_1(sprite_status *pActwk) {
    Sint16 subact;

    subact = ((Sint16 *)pActwk)[25];
    pActwk->xposi.w.h = actwk[subact].xposi.w.h;
    pActwk->yposi.w.h = actwk[subact].yposi.w.h;
}

void egg5pipe(sprite_status *pActwk) {
    Sint16 subact;
    static void (*act_tbl[2])(sprite_status *) = {&egg5pipe_ini, &egg5pipe_1};

    act_tbl[pActwk->r_no0 / 2](pActwk);
    subact = ((Sint16 *)pActwk)[25];
    if (!(actwk[subact].actfree[2] & 8))
        actionsub(pActwk);
}

static void egg5pipe_ini(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 4;
    pActwk->sprhsize = 8;
    pActwk->sprvsize = 48;
    pActwk->sproffset = 8892;
    pActwk->patbase = egg5pipe_pat;
    egg5pipe_1(pActwk);
}

static void egg5pipe_1(sprite_status *pActwk) {
    Sint16 subact;
    Sint16 temp;

    subact = ((Sint16 *)pActwk)[25];
    temp = (actwk[subact].yposi.w.h - pActwk->yposi.w.h) / 32;
    temp &= 255;
    pActwk->patno = temp;
}

void egg5catch(sprite_status *pActwk) {
    static Sint32 (*act_tbl[7])(sprite_status *) = {
        &egg5catch_ini, &egg5catch_1st, &egg5catch_return, &egg5catch_wait,
        &egg5catch_go1, &egg5catch_go2, egg5catch_fire};

    if (act_tbl[pActwk->r_no0 / 2](pActwk) != 0) {
        patchg(pActwk, egg5catch_pchg);
        actionsub(pActwk);
    } else
        frameout(pActwk);
}

static Sint32 egg5catch_ini(sprite_status *pActwk) {
    Sint32 ret;

    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 4;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 12;
    pActwk->sproffset = 700;
    pActwk->patbase = egg5catch_pat;
    ret = egg5catch_1st(pActwk);
    return ret;
}

static Sint32 egg5catch_1st(sprite_status *pActwk) {
    Sint16 subact;
    Sint32 ret = 1;

    subact = ((Sint16 *)pActwk)[25];
    subact = ((Sint16 *)&actwk[subact])[25];
    if (actwk[subact].r_no0 != 2) {

        pActwk->r_no0 = 4;
        ret = egg5catch_return(pActwk);
    }
    return ret;
}

static Sint32 egg5catch_return(sprite_status *pActwk) {
    if (pActwk->xposi.w.h < 3520)
        pActwk->xposi.l += 122880;
    else
        pActwk->r_no0 = 6;
    return 1;
}

static Sint32 egg5catch_wait(sprite_status *pActwk) {
    Sint16 subact;
    Sint32 ret = 1;

    subact = ((Sint16 *)pActwk)[25];
    subact = ((Sint16 *)&actwk[subact])[25];
    if (((Sint16 *)&actwk[subact])[33] <= 210)
        ret = 0;
    else {
        ++pActwk->actfree[0];
        if (pActwk->actfree[0] == 180) {

            pActwk->actfree[0] = 0;
            soundset(203);
            pActwk->r_no0 = 8;
            lever_on(pActwk);
            make_bomb(pActwk);
            chg_mstno(1, pActwk);
        }
    }
    return ret;
}

static Sint32 egg5catch_go1(sprite_status *pActwk) {
    pActwk->xposi.l -= 49152;
    if (pActwk->xposi.w.h <= 3488)
        pActwk->r_no0 = 10;
    return 1;
}

static Sint32 egg5catch_go2(sprite_status *pActwk) {
    if (actwk[0].xposi.w.h < pActwk->xposi.w.h) {
        if (pActwk->xposi.w.h > 3296) {
            pActwk->xposi.l -= 49152;
            return 1;
        }
    }

    soundset(196);
    lever_off(pActwk);
    pActwk->r_no0 = 12;
    return 1;
}

static Sint32 egg5catch_fire(sprite_status *pActwk) {
    ++pActwk->actfree[0];
    if (pActwk->actfree[0] == 30) {

        lever_on(pActwk);
        pActwk->actfree[2] |= 64;
        chg_mstno(0, pActwk);
        return 1;
    }
    if (pActwk->actfree[0] == 120) {

        pActwk->actfree[0] = 0;
        pActwk->r_no0 = 4;
        lever_off(pActwk);
    }
    return 1;
}

static void lever_on(sprite_status *pActwk) {
    Sint16 subact;

    subact = ((Sint16 *)pActwk)[25];
    if (!actwk[subact].mstno.b.h) {
        chg_mstno2(0, subact);
        actwk[subact].patno = 1;
    }
}

static void lever_off(sprite_status *pActwk) {
    Sint16 subact;

    subact = ((Sint16 *)pActwk)[25];
    if (!actwk[subact].mstno.b.h) {
        chg_mstno2(0, subact);
        actwk[subact].patno = 0;
    }
}

void egg5bomb(sprite_status *pActwk) {
    Sint32 ret;
    static Sint32 (*act_tbl[5])(sprite_status *) = {
        &egg5bomb_ini, &egg5bomb_1, &egg5bomb_2, &egg5bomb_3, &egg5bomb_4};

    ret = act_tbl[pActwk->r_no0 / 2](pActwk);
    if (ret != 0) {
        if (ret != 99)
            actionsub(pActwk);
    } else
        frameout(pActwk);
}

static Sint32 egg5bomb_ini(sprite_status *pActwk) {
    Sint32 ret;

    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 4;
    pActwk->sprhsize = 12;
    pActwk->sprvsize = 12;
    pActwk->sproffset = 8892;
    pActwk->patbase = egg5bomb_pat;
    pActwk->colino = 253;
    pActwk->colicnt = 2;
    ((Sint16 *)pActwk)[30] = -3;
    ((Uint16 *)pActwk)[27] = 49152;
    ret = egg5bomb_1(pActwk);
    return ret;
}

static Sint32 egg5bomb_1(sprite_status *pActwk) {
    Sint16 subact;
    Sint32 ret = 1;

    subact = ((Sint16 *)pActwk)[25];
    pActwk->xposi.w.h = actwk[subact].xposi.w.h;
    if (actwk[subact].r_no0 == 10) {
        pActwk->r_no0 += 2;
        ret = egg5bomb_2(pActwk);
    }
    return ret;
}

static Sint32 egg5bomb_2(sprite_status *pActwk) {
    Sint16 subact;
    Sint16 sin, cos;
    Sint32 temp;
    Sint32 ret = 1;

    subact = ((Sint16 *)pActwk)[25];
    pActwk->xposi.w.h = actwk[subact].xposi.w.h;
    if (actwk[subact].actfree[2] & 64) {
        actwk[subact].actfree[2] &= 191;

        pActwk->r_no0 += 2;
        ret = egg5bomb_3(pActwk);
    } else {
        actwk[subact].actfree[2] &= 191;
        pActwk->yposi.w.h -= ((Sint16 *)pActwk)[30];
        ((Uint16 *)pActwk)[27] += 1152;
        sinset(pActwk->actfree[9], &sin, &cos);
        temp = sin;

        temp *= 3;
        temp >>= 8;
        sin = (Uint16)temp;
        ((Sint16 *)pActwk)[30] = sin;
        pActwk->yposi.w.h += sin;
    }
    return ret;
}

static Sint32 egg5bomb_3(sprite_status *pActwk) {
    Sint16 subact;
    Sint32 ret = 1;

    if (pActwk->colicnt > 2)
        ret = egg5bomb_hit(pActwk);
    else {
        subact = ((Sint16 *)pActwk)[25];
        if (actwk[subact].actfree[2] & 4)
            ret = 0;
        else {
            pActwk->yposi.l += 139264;
            if (pActwk->yposi.w.h >= 696) {

                soundset(180);
                ret = egg5bomb_4(pActwk);
            }
        }
    }
    return ret;
}

static Sint32 egg5bomb_4(sprite_status *pActwk) {
    Sint16 subact;
    Sint32 ret = 1;

    if (pActwk->colicnt > 2)
        ret = egg5bomb_hit(pActwk);
    else {
        subact = ((Sint16 *)pActwk)[25];
        if (actwk[subact].actfree[2] & 4)
            ret = 0;
        else {
            ++pActwk->actfree[0];
            if (pActwk->actfree[0] == 8) {

                make_bomb2(pActwk);
                ret = 0;
            }
        }
    }
    return ret;
}

void egg5bomb2(sprite_status *pActwk) {
    Sint32 ret;
    static Sint32 (*act_tbl[3])(sprite_status *) = {&egg5bomb2_ini,
                                                    &egg5bomb2_1, &egg5bomb2_2};

    ret = act_tbl[pActwk->r_no0 / 2](pActwk);
    if (ret != 0) {
        if (ret != 99)
            actionsub(pActwk);
    } else
        frameout(pActwk);
}

static Sint32 bomb2_tbl[24] = {
    4,  -12, 40960,  -229376, 0, 12288, 4,  -8, 81920,  -114688, 0, 6144,
    -4, -12, -40960, -229376, 0, 12288, -4, -8, -81920, -114688, 0, 6144};

static Sint32 egg5bomb2_ini(sprite_status *pActwk) {
    Sint32 ret;
    Sint16 wD0;

    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 1;
    pActwk->sprhsize = 8;
    pActwk->sprvsize = 8;
    pActwk->sproffset = 8892;
    pActwk->patbase = egg5bomb_pat;
    pActwk->colino = 253;
    pActwk->colicnt = 2;
    pActwk->patno = 1;

    wD0 = pActwk->userflag.b.h;
    wD0 *= 6;
    pActwk->xposi.w.h += bomb2_tbl[wD0++];
    pActwk->yposi.w.h += bomb2_tbl[wD0++];
    *(Sint32 *)&pActwk->actfree[16] = bomb2_tbl[wD0++];
    ((Sint32 *)pActwk)[4] = bomb2_tbl[wD0];

    ret = egg5bomb2_1(pActwk);
    return ret;
}

static Sint32 egg5bomb2_1(sprite_status *pActwk) {
    Sint32 ret = 1;

    if (pActwk->colicnt > 2)
        ret = egg5bomb_hit(pActwk);
    else {
        ++pActwk->actfree[0];
        if (pActwk->actfree[0] == 4) {

            pActwk->actfree[0] = 0;
            pActwk->r_no0 += 2;
            ret = egg5bomb2_2(pActwk);
        }
    }
    return ret;
}

static Sint32 egg5bomb2_2(sprite_status *pActwk) {
    Sint16 subact;
    Sint32 ret = 1;
    Sint16 wD0;

    if (pActwk->colicnt > 2)
        ret = egg5bomb_hit(pActwk);
    else {
        subact = ((Sint16 *)pActwk)[25];
        if (actwk[subact].actfree[2] & 4)
            ret = 0;
        else {
            pActwk->xposi.l += *(Sint32 *)&pActwk->actfree[16];
            pActwk->yposi.l += ((Sint32 *)pActwk)[4];
            wD0 = pActwk->userflag.b.h;
            wD0 *= 6;
            wD0 += 4;
            *(Sint32 *)&pActwk->actfree[16] += bomb2_tbl[wD0++];
            ((Sint32 *)pActwk)[4] += bomb2_tbl[wD0];

            if (pActwk->yposi.w.h >= 712)
                ret = 0;
            if (!pActwk->colino)
                ret = 0;
        }
    }
    return ret;
}

static Sint32 egg5bomb_hit(sprite_status *pActwk) {
    Sint16 xback, yback;

    xback = pActwk->xposi.w.h;
    yback = pActwk->yposi.w.h;
    frameout(pActwk);
    pActwk->r_no1 = 255;
    pActwk->actno = 24;
    pActwk->xposi.w.h = xback;
    pActwk->yposi.w.h = yback;
    soundset(158);
    return 99;
}

void egg5hibana(sprite_status *pActwk) {
    static Sint32 (*act_tbl[3])(sprite_status *) = {
        &egg5hibana_ini, &egg5hibana_1, &egg5hibana_2};

    if (act_tbl[pActwk->r_no0 / 2](pActwk) != 0) {
        patchg(pActwk, egg5hibana_pchg);
        actionsub(pActwk);
    } else
        frameout(pActwk);
}

static Sint32 egg5hibana_ini(sprite_status *pActwk) {
    Sint32 ret;

    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 2;
    pActwk->sprhsize = 8;
    pActwk->sprvsize = 4;
    pActwk->sproffset = 8892;
    pActwk->patbase = egg5hibana_pat;
    *(Sint32 *)&pActwk->actfree[16] = -0x20000;
    ((Sint32 *)pActwk)[4] = -0x20000;
    if (!pActwk->actfree[3])
        ret = egg5hibana_1(pActwk);
    else {
        pActwk->r_no0 += 2;
        ret = egg5hibana_2(pActwk);
    }
    return ret;
}

static Sint32 egg5hibana_1(sprite_status *pActwk) {
    Sint32 ret = 1;

    *(Sint32 *)&pActwk->actfree[16] -= 4096;
    pActwk->xposi.l += *(Sint32 *)&pActwk->actfree[16];
    if (pActwk->xposi.w.h <= 3448)
        ret = 0;
    else
        ret = hibana_delchk(pActwk);
    return ret;
}

static Sint32 egg5hibana_2(sprite_status *pActwk) {
    Sint32 ret;

    *(Sint32 *)&pActwk->actfree[16] -= 4096;
    ((Sint32 *)pActwk)[4] += 8192;
    pActwk->xposi.l += *(Sint32 *)&pActwk->actfree[16];
    pActwk->yposi.l += ((Sint32 *)pActwk)[4];
    ret = hibana_delchk(pActwk);
    return ret;
}

static Sint32 hibana_delchk(sprite_status *pActwk) {
    Sint32 ret = 1;

    if (frameout_chk(pActwk) != 0)
        ret = 0;
    return ret;
}

void egg5hari(sprite_status *pActwk) {
    static void (*act_tbl[5])(sprite_status *) = {
        &egg5hari_ini, &egg5hari_1, &egg5hari_2, &egg5hari_3, &egg5hari_4};

    act_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
}

static void egg5hari_ini(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 4;
    pActwk->sprhsize = 32;
    pActwk->sprvsize = 32;
    pActwk->sproffset = 17084;
    pActwk->patbase = egg5hari_pat;
    pActwk->colino = 191;
    pActwk->colicnt = 2;
    egg5hari_1(pActwk);
}

static void egg5hari_1(sprite_status *pActwk) {
    if (actwk[0].xposi.w.h >= 3152) {

        divdevset();
        pActwk->r_no0 += 2;
        egg5hari_2(pActwk);
    }
}

static void egg5hari_2(sprite_status *pActwk) {
    pActwk->yposi.l -= 0x20000;
    if (pActwk->yposi.w.h > 672)
        egg5hari_chk(pActwk);
    else {

        pActwk->yposi.w.h = 672;
        pActwk->r_no0 += 2;
        egg5hari_3(pActwk);
    }
}

static void egg5hari_3(sprite_status *pActwk) {
    pActwk->xposi.l += 0x20000;
    if (pActwk->xposi.w.h < 3280)
        egg5hari_chk(pActwk);
    else {

        pActwk->r_no0 += 2;
        pActwk->xposi.w.h = 3280;
        egg5hari_4(pActwk);
    }
}

static void egg5hari_4(sprite_status *pActwk) { egg5hari_chk(pActwk); }

static void egg5hari_chk(sprite_status *pActwk) {
    pActwk->sprhsize = 12;
    pActwk->sprvsize = 30;
    my_rideonchk(pActwk);
    pActwk->sprhsize = 32;
    pActwk->sprvsize = 32;
}

void egg5belt(sprite_status *pActwk) {
    static void (*act_tbl[5])(sprite_status *) = {
        &egg5belt_ini, &egg5belt_0, &egg5belt_1, &egg5belt_2, &egg5belt_3};

    act_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
}

static void egg5belt_ini(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    set_belt_col(pActwk);
}

static void egg5belt_0(sprite_status *pActwk) {
    Sint16 subact;

    subact = ((Sint16 *)pActwk)[25];
    if (actwk[subact].r_no0 != 2) {

        pActwk->r_no0 += 2;
        egg5belt_1(pActwk);
    }
}

static void egg5belt_1(sprite_status *pActwk) {
    Sint16 wD1 = 0;

    ++pActwk->actfree[1];

    if (pActwk->actfree[1] & 1)
        belt_anime(pActwk);

    if (actwk[0].xposi.w.h > 3464) {
        actwk[0].xposi.w.h -= 4;
        if (actwk[0].xspeed.w >= 0)
            actwk[0].xspeed.w = 0;
    } else
        ++wD1;

    if (wD1 == 1) {

        ((Sint16 *)pActwk)[27] = 192;
        pActwk->r_no0 += 2;
    }
}

static void egg5belt_2(sprite_status *pActwk) {
    Sint16 subact;
    Sint32 wD6;

    subact = ((Sint16 *)pActwk)[25];
    if (actwk[subact].actfree[2] & 4) {

        ((Sint16 *)pActwk)[27] -= 16;
        if (((Sint16 *)pActwk)[27] <= 0) {

            pActwk->r_no0 += 2;

            return;
        }
    }

    wD6 = chk_belt_on();
    if (!(actwk[subact].actfree[2] & 4))
        wD6 = belt_spdset(pActwk);

    belt_sncspd(wD6, pActwk);
    belt_sncxmax(subact);

    belt_hitpnt(subact, pActwk);
    ++pActwk->actfree[1];

    if (pActwk->actfree[1] & 1)
        belt_anime(pActwk);
}

static void egg5belt_3(sprite_status *pActwk) { pActwk + 1; }

static Sint32 belt_spdset(sprite_status *pActwk) {
    Sint32 wD6;

    wD6 = chk_belt_on();
    if (wD6 <= 0) {
        if (wD6 == 0) {
            if (actwk[0].xspeed.w < ((Sint16 *)pActwk)[27]) {

                ((Sint16 *)pActwk)[27] -= 7;
                goto label1;
            }

            if (actwk[0].xspeed.w != ((Sint16 *)pActwk)[27]) {

                ((Sint16 *)pActwk)[27] += 8;
            }

            goto label1;
        }

        ((Sint16 *)pActwk)[27] = 192;

        goto label1;
    }

    --((Sint16 *)pActwk)[27];

label1:
    if (((Sint16 *)pActwk)[27] <= 192)
        ((Sint16 *)pActwk)[27] = 192;
    else {

        if (((Sint16 *)pActwk)[27] >= 1488)
            ((Sint16 *)pActwk)[27] = 1488;
    }
    return wD6;
}

static void belt_hitpnt(Sint16 subact, sprite_status *pActwk) {
    ((Sint16 *)pActwk)[28] += ((Sint16 *)pActwk)[27];
    if (((Sint16 *)pActwk)[28] >= 1488) {

        --((Sint16 *)&actwk[subact])[33];
        ((Sint16 *)pActwk)[28] -= 1488;
    }
}

static void belt_sncspd(Sint32 wD6, sprite_status *pActwk) {
    int_union wD0;

    if (wD6 == 1) {

        if (!(pActwk->actfree[2] & 16)) {
            actwk[0].xspeed.w -= ((Sint16 *)pActwk)[29];
            actwk[0].yspeed.w -= ((Sint16 *)pActwk)[30];
        }
        pActwk->actfree[2] |= 16;
    } else {

        pActwk->actfree[2] &= 239;
        ((Sint16 *)pActwk)[29] = actwk[0].xspeed.w;
        ((Sint16 *)pActwk)[30] = actwk[0].yspeed.w;

        if (wD6 >= 0) {

            wD0.l = 0;
            wD0.w.l = ((Sint16 *)pActwk)[27];

            if (wD0.w.l - actwk[0].xspeed.w > 768) {
                wD0.l = 0;
                wD0.l = actwk[0].xspeed.w + 768;
            }

            wD0.l <<= 8;
            actwk[0].xposi.l -= wD0.l;
        }
    }
}

static void belt_sncxmax(Sint16 subact) {
    if (!(actwk[subact].actfree[2] & 4)) {
        if (actwk[0].xposi.w.h >= 3464)
            actwk[0].xposi.w.h = 3464;
    }
}

static void belt_anime(sprite_status *pActwk) {
    Uint8 wD0;

    if (((Sint16 *)pActwk)[27] > 1024) {

        wD0 = 1;
    } else {
        if (((Sint16 *)pActwk)[27] > 768) {

            wD0 = 1;
        } else {
            if (((Sint16 *)pActwk)[27] > 512) {

                wD0 = 2;
            } else {

                wD0 = 4;
            }
        }
    }

    ++pActwk->actfree[0];
    if (wD0 <= pActwk->actfree[0]) {
        pActwk->actfree[0] = 0;
        set_belt_col(pActwk);
    }
}

static void set_belt_col(sprite_status *pActwk) {
    PALETTEENTRY *pColorwk;
    char bA3;
    PALETTEENTRY belt_col_tblD[9] = {
        {224, 224, 192, 1}, {96, 96, 64, 1},    {0, 0, 0, 1},
        {0, 0, 0, 1},       {224, 224, 192, 1}, {96, 96, 64, 1},
        {96, 96, 64, 1},    {0, 0, 0, 1},       {224, 224, 192, 1}};
    PALETTEENTRY belt_col_tblC[9] = {
        {0, 32, 0, 1},    {0, 128, 0, 1},   {32, 192, 96, 1},
        {0, 128, 0, 1},   {32, 192, 96, 1}, {0, 32, 0, 1},
        {32, 192, 96, 1}, {0, 32, 0, 1},    {0, 128, 0, 1}};

    ((char *)pActwk)[66] += 3;
    if (((char *)pActwk)[66] >= 9)
        ((char *)pActwk)[66] = 0;

    bA3 = ((char *)pActwk)[66];
    pColorwk = &lpcolorwk[42];
    if (!generate_flag) {
        *pColorwk = belt_col_tblD[bA3];
        ++pColorwk;
        ++bA3;
        *pColorwk = belt_col_tblD[bA3];
        ++pColorwk;
        ++bA3;
        *pColorwk = belt_col_tblD[bA3];
    } else {
        *pColorwk = belt_col_tblC[bA3];
        ++pColorwk;
        ++bA3;
        *pColorwk = belt_col_tblC[bA3];
        ++pColorwk;
        ++bA3;
        *pColorwk = belt_col_tblC[bA3];
    }
}

static Sint32 chk_belt_on(void) {
    if (actwk[0].mstno.b.h != 26) {
        if (actwk[0].mstno.b.h != 25) {
            if (actwk[0].mstno.b.h != 24) {
                if (!(actwk[0].cddat & 128)) {
                    if (actwk[0].cddat & 2)
                        return 1;
                    else
                        return 0;
                }
            }
        }
    }
    return -1;
}

void egg5bakuha(sprite_status *pActwk) {
    static Sint32 (*act_tbl[3])(sprite_status *) = {
        &egg5bakuha_ini, &egg5bakuha_1, &frameout_sp4};

    if (act_tbl[pActwk->r_no0 / 2](pActwk) != 0) {
        patchg(pActwk, egg5bakuha_pchg);
        actionsub(pActwk);
    } else
        frameout(pActwk);
}

static Sint32 egg5bakuha_ini(sprite_status *pActwk) {
    Sint32 ret;

    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 3;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 16;
    pActwk->sproffset = 1664;
    pActwk->patbase = bakupat;
    ret = egg5bakuha_1(pActwk);
    return ret;
}

static Sint32 egg5bakuha_1(sprite_status *pActwk) {
    pActwk->yposi.l -= 196608;
    return 1;
}

static Sint32 frameout_sp4(sprite_status *pActwk) {
    pActwk + 1;
    return 0;
}

void make_hibana1(void) {
    sprite_status *ppActwk;

    if (actwkchk(&ppActwk) == 0) {

        ppActwk->actno = 32;
        ppActwk->xposi.w.h = 3560;
        ppActwk->yposi.w.h = 696;
    }
}

void make_hibana2(sprite_status *pActwk) {
    sprite_status *ppActwk;
    Sint16 wD4;
    Sint32 i, j;
    Sint16 hibana_tbl[16] = {-24, 0,  -6, 0,  6,   0, 24, 0,
                             -12, -8, 12, -8, -12, 8, 12, 8};
    Sint16 hibana_ytbl[6] = {52, 52, 44, 32, 28, 28};

    wD4 = hibana_ytbl[pActwk->patno];
    j = 0;
    for (i = 0; i < 8; ++i) {
        if (actwkchk(&ppActwk) == 0) {
            ppActwk->actfree[3] = 1;
            ((Sint16 *)ppActwk)[25] = (Uint16)(Uint8)(pActwk - actwk);
            ppActwk->actno = 32;
            ppActwk->xposi.w.h = pActwk->xposi.w.h;
            ppActwk->yposi.w.h = pActwk->yposi.w.h;
            ppActwk->xposi.w.h += hibana_tbl[j++];
            ppActwk->yposi.w.h += wD4 + hibana_tbl[j++];
        } else
            i = 99;
    }
}

static void bom_set(sprite_status *pActwk) {
    sprite_status *ppActwk;
    Uint8 wD2, wD3;
    Sint16 bom_tbl[8] = {-16, -8, -32, 8, -16, 24, 32, 8};

    if (pActwk->actfree[0] % 4 == 0) {

        wD3 = pActwk->actfree[0];
        wD3 /= 4;
        wD3 %= 4;
        wD2 = wD3 * 2;
        if (actwkchk(&ppActwk) == 0) {

            ppActwk->actfree[0] = wD3;
            ppActwk->actno = 35;
            ppActwk->xposi.w.h = pActwk->xposi.w.h;
            ppActwk->yposi.w.h = pActwk->yposi.w.h;
            ppActwk->xposi.w.h += bom_tbl[wD2++];
            ppActwk->yposi.w.h += bom_tbl[wD2];
            soundset(158);
        }
    }
}

static void make_meca2(sprite_status *pActwk) {
    sprite_status *ppActwk;

    if (actwkchk(&ppActwk) == 0) {
        ((Sint16 *)ppActwk)[25] = (Uint16)(Uint8)(pActwk - actwk);
        ((Sint16 *)pActwk)[26] = (Uint16)(Uint8)(ppActwk - actwk);
        ppActwk->actno = 52;
        ppActwk->xposi.w.h = pActwk->xposi.w.h;
        ppActwk->yposi.w.h = pActwk->yposi.w.h + 56;
    }
}

static void make_egg5(sprite_status *pActwk) {
    sprite_status *ppActwk;

    if (actwkchk(&ppActwk) == 0) {
        ((Sint16 *)ppActwk)[25] = (Uint16)(Uint8)(pActwk - actwk);
        ((Sint16 *)pActwk)[26] = (Uint16)(Uint8)(ppActwk - actwk);
        ppActwk->actno = 50;
        ppActwk->xposi.w.h = pActwk->xposi.w.h;
        ppActwk->yposi.w.h = pActwk->yposi.w.h;
    }
}

static void make_catch(sprite_status *pActwk) {
    sprite_status *ppActwk;

    if (actwkchk(&ppActwk) == 0) {
        ((Sint16 *)ppActwk)[25] = (Uint16)(Uint8)(pActwk - actwk);
        ((Sint16 *)pActwk)[26] = (Uint16)(Uint8)(ppActwk - actwk);
        ppActwk->actno = 55;
        ppActwk->xposi.w.h = pActwk->xposi.w.h - 128;
        ppActwk->yposi.w.h = pActwk->yposi.w.h - 68;
    }
}

static void make_pipe(sprite_status *pActwk) {
    sprite_status *ppActwk;

    if (actwkchk(&ppActwk) == 0) {
        ((Sint16 *)ppActwk)[25] = (Uint16)(Uint8)(pActwk - actwk);
        ppActwk->actno = 54;
        ppActwk->xposi.w.h = pActwk->xposi.w.h;
        ppActwk->yposi.w.h = pActwk->yposi.w.h;
    }
}

static void make_meca3(sprite_status *pActwk) {
    sprite_status *ppActwk;

    if (actwkchk(&ppActwk) == 0) {
        ((Sint16 *)ppActwk)[25] = (Uint16)(Uint8)(pActwk - actwk);
        ppActwk->actno = 53;
        ppActwk->xposi.w.h = pActwk->xposi.w.h;
        ppActwk->yposi.w.h = pActwk->yposi.w.h + 56;
    }
}

static void make_bomb(sprite_status *pActwk) {
    sprite_status *ppActwk;

    if (actwkchk(&ppActwk) == 0) {
        ((Sint16 *)ppActwk)[25] = (Uint16)(Uint8)(pActwk - actwk);
        ppActwk->actno = 56;
        ppActwk->xposi.w.h = pActwk->xposi.w.h;
        ppActwk->yposi.w.h = pActwk->yposi.w.h + 24;
    }
}

static void make_bomb2(sprite_status *pActwk) {
    sprite_status *ppActwk;
    Sint32 i;

    for (i = 3; i >= 0; --i) {
        if (actwkchk(&ppActwk) == 0) {
            ((Sint16 *)ppActwk)[25] = (Uint16)(Uint8)(pActwk - actwk);
            ppActwk->actno = 57;
            ppActwk->xposi.w.h = pActwk->xposi.w.h;
            ppActwk->yposi.w.h = pActwk->yposi.w.h;
            ppActwk->userflag.b.h = i;
        } else
            i = -99;
    }
}

static void make_belt(sprite_status *pActwk) {
    sprite_status *ppActwk;

    if (actwkchk(&ppActwk) == 0) {
        ((Sint16 *)ppActwk)[25] = (Uint16)(Uint8)(pActwk - actwk);
        ((Sint16 *)pActwk)[27] = (Uint16)(Uint8)(ppActwk - actwk);
        ppActwk->actno = 34;
    }
}

static void chg_mstno(Uint8 num, sprite_status *pActwk) {
    chg_mstno2(num, (Uint16)(Uint8)(pActwk - actwk));
}

static void chg_mstno2(Uint8 num, Sint16 subact) {
    actwk[subact].mstno.b.h = num;
    actwk[subact].patno = 0;
    actwk[subact].patcnt = 0;
    actwk[subact].pattim = 0;
    actwk[subact].pattimm = 0;
}

static Sint32 frameout_chk(sprite_status *pActwk) {
    Sint32 ret = -1;
    Sint16 wD3;
    short_union wD0;

    wD0.w = 0;
    pActwk->sprhsize = wD0.b.l;
    wD3 = pActwk->xposi.w.h - scra_h_posit.w.h;
    if (wD0.w + wD3 >= 0) {
        if (wD3 - wD0.w < 320) {
            wD0.w = 0;
            pActwk->sprvsize = wD0.b.l;
            wD3 = pActwk->yposi.w.h - scra_v_posit.w.h;
            if (wD0.w + wD3 >= 0) {
                if (wD3 - wD0.w < 224)
                    ret = 0;
            }
        }
    }
    return ret;
}
