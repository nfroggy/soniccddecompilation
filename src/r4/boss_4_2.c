#include "../equ.h"
#include "boss_4_2.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../goal.h"
#include "../io.h"
#include "../loader2.h"
#include "../player_work.h"
#include "../score.h"
#include "playsub4.h"

typedef struct {
    char xpos;
    char ypos;
    Sint32 xspd;
    Sint32 yspd;
} tama;

typedef struct {
    union {
        Sint16 word0;
        struct {
            Uint8 timer;
            Uint8 laugh_timer;
        };
    };
    Uint8 flags;
    Uint8 bubble_slot;
    Sint16 parent_index;
    Sint16 child_index;
    Uint8 spread_count;
    Uint8 bubble_position_index;
    union {
        Sint16 angle;
        struct {
            Uint8 angle_low;
            Uint8 angle_high;
        };
    };
    Sint16 angular_speed;
    union {
        Sint32 xy_offset;
        struct {
            Sint16 x_offset;
            Sint16 y_offset;
        };
    };
    union {
        Sint32 radial_speed;
        struct {
            Sint16 radial_speed_low;
            Sint16 radius;
        };
    };
} egg4_work;

static egg4_work *egg4_get_work(sprite_status *pActwk) {
    return (egg4_work *)pActwk->actfree;
}

static void egg4_warai_chk(sprite_status *pActwk);
static void egg4_warai(sprite_status *pActwk);
static Uint32 egg4_ini(sprite_status *pActwk);
static void make_egg4meca(sprite_status *pActwk);
static Uint32 egg4_scrset(sprite_status *pActwk);
static Uint32 egg4_scrreset(sprite_status *pActwk);
static Uint32 egg4_awademo1(sprite_status *pActwk);
static void egg4_posiini(sprite_status *pActwk);
static void make_awa(sprite_status *pActwk, sprite_status **pNewact);
static void make_awa2(sprite_status *pActwk, sprite_status **pNewact);
static Uint32 egg4_awademo2(sprite_status *pActwk);
static Uint32 egg4_awademo3(sprite_status *pActwk);
static Uint32 egg4_movel(sprite_status *pActwk);
static void egg4_hitchk(sprite_status *pActwk);
static void make_tama(sprite_status *pActwk);
static void egg4_posiset(sprite_status *pActwk);
static Uint32 egg4_mover(sprite_status *pActwk);
static Uint32 egg4_wait(sprite_status *pActwk);
static Uint32 egg4_die(sprite_status *pActwk);
static void bom_set(sprite_status *pActwk);
static Uint32 egg4_esc(sprite_status *pActwk);
static Uint32 egg4_movec(sprite_status *pActwk);
static Uint32 egg4_movec2(sprite_status *pActwk);
static Uint32 egg4_movec3(sprite_status *pActwk);
static void egg4meca_ini(sprite_status *pActwk);
static void egg4meca_01(sprite_status *pActwk);
static Uint32 egg4awa_ini(sprite_status *pActwk);
static Uint32 egg4awa_deru(sprite_status *pActwk);
static Uint32 egg4awa_tuku(sprite_status *pActwk);
static Uint32 egg4awa_hiro(sprite_status *pActwk);
static void awa_hitchk(sprite_status *pActwk);
static Uint32 egg4awa_roll(sprite_status *pActwk);
static Uint32 egg4awa_ychg(sprite_status *pActwk);
static void ychg_ret(sprite_status *pActwk, sprite_status *pEggwk);
static void ychg_rad_endproc(sprite_status *pActwk);
static Uint32 egg4awa_del(sprite_status *pActwk);
static Uint32 egg4awa_out(sprite_status *pActwk);
static Uint32 egg4awa_chi(sprite_status *pActwk);
static Uint32 egg4tama_ini(sprite_status *pActwk);
static Uint32 egg4tama_01(sprite_status *pActwk);
static Uint32 egg4tama_02(sprite_status *pActwk);
static Uint32 egg4tama_kill(sprite_status *pActwk);
static Uint32 frameout_chk(sprite_status *pActwk);

static sprite_pattern egg4_pat0 = {1, {{-32, -28, 0, 452}}};
static sprite_pattern egg4_pat1 = {1, {{-32, -28, 0, 453}}};
static sprite_pattern egg4_pat2 = {1, {{-32, -28, 0, 454}}};
static sprite_pattern egg4_pat3 = {1, {{-32, -28, 0, 455}}};
static sprite_pattern egg4_pat4 = {1, {{-32, -28, 0, 456}}};
static sprite_pattern egg4_pat5 = {1, {{-32, -44, 0, 457}}};
static sprite_pattern egg4_pat6 = {1, {{-32, -44, 0, 458}}};
sprite_pattern *egg4_pat[7] = {&egg4_pat0, &egg4_pat1, &egg4_pat2, &egg4_pat3,
                               &egg4_pat4, &egg4_pat5, &egg4_pat6};
static Uint8 egg4_pchg0[3] = {255, 0, 255};
static Uint8 egg4_pchg1[4] = {7, 1, 2, 255};
static Uint8 egg4_pchg2[3] = {255, 3, 255};
static Uint8 egg4_pchg3[6] = {3, 5, 4, 6, 4, 255};
static Uint8 *egg4_pchg[4] = {egg4_pchg0, egg4_pchg1, egg4_pchg2, egg4_pchg3};
static Uint32 (*egg4_act_tbl[14])(sprite_status *) = {
    &egg4_ini,      &egg4_scrset,   &egg4_scrreset, &egg4_awademo1,
    &egg4_awademo2, &egg4_awademo3, &egg4_movel,    &egg4_mover,
    &egg4_wait,     &egg4_die,      &egg4_esc,      &egg4_movec,
    &egg4_movec2,   &egg4_movec3};
extern Uint16 scr_dir_tbl[];

void egg4(sprite_status *pActwk) {
    egg4_get_work(pActwk)->flags &= 191;
    if (!egg4_get_work(pActwk)->laugh_timer) {

        egg4_warai_chk(pActwk);
    } else if (--egg4_get_work(pActwk)->laugh_timer == 0) {
        pActwk->mstno.b.h = 0;
        pActwk->patno = 0;
        pActwk->patcnt = 0;
        pActwk->pattim = 0;
        pActwk->pattimm = 0;
    }

    if (egg4_act_tbl[pActwk->r_no0 / 2](pActwk) != 0) {
        patchg(pActwk, egg4_pchg);
        actionsub(pActwk);
    }
}

static void egg4_warai_chk(sprite_status *pActwk) {
    if (!pActwk->mstno.b.h) {

        if (player_work_get(&actwk[0])->damage_invulnerability_timer ||
            actwk[0].r_no0 == 6) {

            egg4_warai(pActwk);
        }
    }
}

static void egg4_warai(sprite_status *pActwk) {
    egg4_get_work(pActwk)->laugh_timer = 120;
    pActwk->mstno.b.h = 1;
    pActwk->patno = 0;
    pActwk->patcnt = 0;
    pActwk->pattim = 0;
    pActwk->pattimm = 0;
}

static Uint32 egg4_ini(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 4;
    pActwk->sprhsize = 32;
    pActwk->sprvsize = 44;
    pActwk->sproffset = 798;
    pActwk->patbase = egg4_pat;
    pActwk->colicnt = 2;

    make_egg4meca(pActwk);

    return 1;
}

static void make_egg4meca(sprite_status *pActwk) {
    sprite_status *pNewact;

    if (actwkchk(&pNewact) == 0) {
        egg4_get_work(pActwk)->child_index = (Sint16)(pNewact - actwk);
        egg4_get_work(pNewact)->parent_index = (Sint16)(pActwk - actwk);
        pNewact->actno = 77;
        pNewact->xposi.w.h = pActwk->xposi.w.h;
        pNewact->yposi.w.h = pActwk->yposi.w.h;
    }
}

static Uint32 egg4_scrset(sprite_status *pActwk) {
    Sint16 xwk;

    scralim_right = scralim_n_right = 2480;

    if ((xwk = actwk[0].xposi.w.h - 160) >= scralim_left) {
        if (actwk[0].xposi.w.h >= 2640) {

            sub_sync(103);

            pActwk->r_no0 = 6;
            scralim_right = scralim_n_right = 2480;
            xwk = 2480;
        }

        scralim_left = scralim_n_left = xwk;
    }

    return 1;
}

static Uint32 egg4_scrreset(sprite_status *pActwk) {
    Sint16 *pScrdir, sLeft, sRight, sUp, sDown, BgmNo;

    pScrdir = (Sint16 *)&scr_dir_tbl[1];
    sLeft = *pScrdir++;
    sRight = *pScrdir++;
    sUp = *pScrdir++;
    sDown = *pScrdir++;

    scralim_right += 6;
    scralim_n_right += 6;

    if (scralim_right < sRight) {
        return 0;
    }

    bossstart = 0;
    scralim_right = scralim_n_right = sRight;

    if (generate_flag) {
        BgmNo = 22;
    } else {
        BgmNo = 23;
    }

    sub_sync(BgmNo);
    genecolor();
    frameout(pActwk);

    return 0;
}

static Uint32 egg4_awademo1(sprite_status *pActwk) {
    sprite_status *pNewact;

    egg4_posiini(pActwk);
    ++egg4_get_work(pActwk)->timer;

    if (egg4_get_work(pActwk)->timer == 10) {
        make_awa2(pActwk, &pNewact);
    }

    if (egg4_get_work(pActwk)->timer == 20 ||
        egg4_get_work(pActwk)->timer >= 30) {

        soundset(182);

        egg4_get_work(pActwk)->timer = 0;
        make_awa(pActwk, &pNewact);

        egg4_get_work(pNewact)->bubble_slot =
            egg4_get_work(pActwk)->bubble_slot++;

        if (egg4_get_work(pActwk)->bubble_slot == 16) {
            pActwk->r_no0 = 8;
        }
    }

    return 1;
}

static void egg4_posiini(sprite_status *pActwk) {
    if (pActwk->yposi.w.h >= 1312) {

        pActwk->xposi.w.h = 2736;
        pActwk->xposi.w.l = 0;
        pActwk->yposi.w.h = 1312;
        pActwk->yposi.w.l = 0;
    } else {
        pActwk->yposi.l += 18000;
    }
}

static void make_awa(sprite_status *pActwk, sprite_status **pNewact) {
    sprite_status *pMakeact;

    if (actwkchk(&pMakeact) == 0) {
        *pNewact = pMakeact;
        egg4_get_work(pMakeact)->parent_index = (Sint16)(pActwk - actwk);
        pMakeact->actno = 79;
        pMakeact->xposi.w.h = pActwk->xposi.w.h;
        pMakeact->yposi.w.h = 1464;
    }
}

static void make_awa2(sprite_status *pActwk, sprite_status **pNewact) {
    make_awa(pActwk, pNewact);
    (*pNewact)->r_no1 = 1;
}

static Uint32 egg4_awademo2(sprite_status *pActwk) {
    egg4_posiini(pActwk);

    if (++egg4_get_work(pActwk)->timer == 150) {
        egg4_get_work(pActwk)->timer = 0;
        pActwk->r_no0 = 10;
    }

    return 1;
}

static Uint32 egg4_awademo3(sprite_status *pActwk) {
    if (++egg4_get_work(pActwk)->timer == 150) {
        pActwk->r_no0 = 12;
        egg4_get_work(pActwk)->radius = 96;
        pActwk->yspeed.w = 64;
        egg4_get_work(pActwk)->angle = 0;
        egg4_get_work(pActwk)->x_offset = 96;
        egg4_get_work(pActwk)->y_offset = 0;
        egg4_get_work(pActwk)->word0 = 0;
    }

    return 1;
}

static Uint32 egg4_movel(sprite_status *pActwk) {
    egg4_hitchk(pActwk);

    egg4_get_work(pActwk)->angle += 256;
    if (egg4_get_work(pActwk)->angle == 2048) {
        make_tama(pActwk);
    }

    if ((Uint16)egg4_get_work(pActwk)->angle == 32768) {

        egg4_get_work(pActwk)->timer = 0;
        pActwk->r_no0 = 16;
        egg4_get_work(pActwk)->flags |= 16;
    } else {
        pActwk->xposi.w.h -= egg4_get_work(pActwk)->x_offset;
        pActwk->yposi.w.h -= egg4_get_work(pActwk)->y_offset;
        egg4_posiset(pActwk);
    }

    return 1;
}

static void egg4_hitchk(sprite_status *pActwk) {
    if (pActwk->colino)
        return;

    pActwk->colino = 60;
    if (pActwk->colicnt != 1)
        return;

    egg4_get_work(pActwk)->timer = 0;
    pActwk->r_no0 = 18;
    egg4_get_work(pActwk)->flags |= 128;
    pActwk->mstno.b.h = 2;
    pActwk->patno = pActwk->patcnt = pActwk->pattim = pActwk->pattimm = 0;
    egg4_die(pActwk);
}

static void make_tama(sprite_status *pActwk) {
    Sint32 i;
    sprite_status *pNewact;
    Sint16 xposwk;
    Sint32 xspdwk;
    static tama tama_tbl[4] = {{-29, 23, -81000, 81000},
                               {-6, 31, 0, 114688},
                               {16, 28, 57344, 99319},
                               {29, 17, 81000, 81000}};

    for (i = 0; i < 4; ++i) {
        if (actwkchk(&pNewact) != 0)
            break;

        egg4_get_work(pNewact)->parent_index = (Sint16)(pActwk - actwk);
        pNewact->actno = 78;
        pNewact->xposi.w.h = pActwk->xposi.w.h;
        pNewact->yposi.w.h = pActwk->yposi.w.h;

        xposwk = tama_tbl[i].xpos;
        xspdwk = tama_tbl[i].xspd;

        if (pActwk->cddat & 1) {

            xposwk = -xposwk;
            xspdwk = -xspdwk;
        }

        egg4_get_work(pNewact)->x_offset = xposwk;
        egg4_get_work(pNewact)->y_offset = tama_tbl[i].ypos;
        egg4_get_work(pNewact)->radial_speed = xspdwk;
        sprite_status_set_xspeed_yspeed(pNewact, tama_tbl[i].yspd);
    }
}

static void egg4_posiset(sprite_status *pActwk) {
    Sint16 sinwk, coswk;
    int_union xwk, ywk;

    sinset(egg4_get_work(pActwk)->angle_high, &sinwk, &coswk);
    xwk.l = egg4_get_work(pActwk)->radius;
    ywk.l = pActwk->yspeed.w;
    ywk.l *= sinwk;
    xwk.l *= coswk;
    ywk.l >>= 8;
    xwk.l >>= 8;

    egg4_get_work(pActwk)->x_offset = xwk.w.l;
    egg4_get_work(pActwk)->y_offset = ywk.w.l;
    pActwk->xposi.w.h += xwk.w.l;
    pActwk->yposi.w.h += ywk.w.l;
}

static Uint32 egg4_mover(sprite_status *pActwk) {
    egg4_hitchk(pActwk);

    egg4_get_work(pActwk)->angle -= 256;
    if (egg4_get_work(pActwk)->angle == 30720) {
        make_tama(pActwk);
    }

    if (egg4_get_work(pActwk)->angle == 0) {

        egg4_get_work(pActwk)->timer = 0;
        pActwk->r_no0 = 16;
        egg4_get_work(pActwk)->flags |= 16;
    } else {
        pActwk->xposi.w.h -= egg4_get_work(pActwk)->x_offset;
        pActwk->yposi.w.h -= egg4_get_work(pActwk)->y_offset;
        egg4_posiset(pActwk);
    }

    return 1;
}

static Uint32 egg4_wait(sprite_status *pActwk) {
    char awaposi_cnt_tbl[8] = {14, 12, 10, 8, 6, 4, -1, -1};
    char cnt;
    sprite_status *pChildact;

    egg4_get_work(pActwk)->flags &= 207;
    egg4_hitchk(pActwk);
    ++egg4_get_work(pActwk)->timer;

    if ((Sint32)egg4_get_work(pActwk)->timer == 48) {

        pChildact = &actwk[egg4_get_work(pActwk)->child_index];
        pChildact->actflg ^= 1;
        pChildact->cddat ^= 1;
        pActwk->actflg ^= 1;
        pActwk->cddat ^= 1;
    } else if (egg4_get_work(pActwk)->timer == 96) {

        egg4_get_work(pActwk)->timer = 0;
        if (pActwk->cddat & 1) {

            egg4_get_work(pActwk)->flags |= 8;
            pActwk->r_no0 = 14;
        } else {
            egg4_get_work(pActwk)->flags &= 247;
            pActwk->r_no0 = 12;
        }

        while ((cnt = awaposi_cnt_tbl[egg4_get_work(pActwk)
                                          ->bubble_position_index]) >= 0) {
            if (egg4_get_work(pActwk)->bubble_slot > cnt)
                break;

            ++egg4_get_work(pActwk)->bubble_position_index;
            pActwk->r_no0 = 22;
        }
    }

    return 1;
}

static Uint32 egg4_die(sprite_status *pActwk) {
    ++egg4_get_work(pActwk)->timer;

    if (egg4_get_work(pActwk)->timer < 60) {
        bom_set(pActwk);
    } else if (egg4_get_work(pActwk)->timer == 60) {

        pActwk->mstno.b.h = 3;
        egg4_get_work(pActwk)->laugh_timer = 255;
        pActwk->patno = pActwk->patcnt = pActwk->pattim = pActwk->pattimm = 0;

        pActwk->patno = 1;
    } else if (egg4_get_work(pActwk)->timer == 61) {

        egg4_get_work(pActwk)->timer = 0;
        pActwk->r_no0 = 20;
        pActwk->actflg |= 1;
        pActwk->cddat |= 1;
        actwk[egg4_get_work(pActwk)->child_index].actflg |= 1;
        actwk[egg4_get_work(pActwk)->child_index].cddat |= 1;
        scoreup(100);
    }

    return 1;
}

static void bom_set(sprite_status *pActwk) {
    int_union tmwk;
    Sint32 wk;
    Sint16 bom_tbl[20] = {-48, -16, 48,  16, -16, -16, 16, 16,  -32, 0,
                          48,  -16, -48, 16, -16, 16,  16, -16, 32,  0};
    sprite_status *pNewact;

    tmwk.l = 0;
    tmwk.b.b4 = egg4_get_work(pActwk)->timer;
    wk = tmwk.l;
    tmwk.w.l = wk % 4;
    tmwk.w.h = wk / 4;
    if (tmwk.w.l == 0) {
        tmwk.l = (Uint32)tmwk.l >> 16 & 65535 | tmwk.l << 16 & -65536;
        tmwk.l %= 10;
        tmwk.l <<= 1;
        if (actwkchk(&pNewact) == 0) {
            pNewact->r_no1 = 1;
            pNewact->actno = 24;
            pNewact->xposi.w.h = pActwk->xposi.w.h;
            pNewact->yposi.w.h = pActwk->yposi.w.h;
            pNewact->xposi.w.h += bom_tbl[tmwk.l++];
            pNewact->yposi.w.h += bom_tbl[tmwk.l];
            soundset(158);
        }
    }
}

static Uint32 egg4_esc(sprite_status *pActwk) {
    if (++egg4_get_work(pActwk)->timer < 60) {
        return 1;
    }

    pActwk->xposi.l += 114688;
    if (pActwk->xposi.w.h >= 2864) {

        pActwk->r_no0 = 4;
        frameout(&actwk[egg4_get_work(pActwk)->child_index]);
    }

    return 1;
}

static Uint32 egg4_movec(sprite_status *pActwk) {
    int_union offswk;

    ++egg4_get_work(pActwk)->timer;

    switch (egg4_get_work(pActwk)->timer) {

    case 20:
        egg4_warai(pActwk);
        egg4_get_work(pActwk)->spread_count = 0;
        egg4_get_work(pActwk)->flags |= 32;
        break;

    case 21:
        egg4_get_work(pActwk)->flags &= 223;
        break;

    case 45:
        make_tama(pActwk);
        break;
    }

    if (egg4_get_work(pActwk)->flags & 8) {

        pActwk->xposi.l += 65536;
        if (pActwk->xposi.w.h >= 2736)
            goto label1;
        return 1;
    }

    pActwk->xposi.l -= 65536;
    if (pActwk->xposi.w.h <= 2544)
        goto label1;
    return 1;

label1:
    egg4_get_work(pActwk)->xy_offset *= -1;
    egg4_get_work(pActwk)->angle += 32768;
    pActwk->xposi.w.h = 2640;
    pActwk->xposi.w.l = 0;

    offswk.w.h = egg4_get_work(pActwk)->x_offset;
    offswk.w.l = egg4_get_work(pActwk)->y_offset;
    pActwk->xposi.l += offswk.l;

    pActwk->yposi.w.h = 1312;
    pActwk->yposi.w.l = 0;

    egg4_get_work(pActwk)->timer = 0;
    pActwk->r_no0 = 16;
    egg4_get_work(pActwk)->flags |= 16;

    return 1;
}

static Uint32 egg4_movec2(sprite_status *pActwk) {
    pActwk = pActwk;
    return 1;
}

static Uint32 egg4_movec3(sprite_status *pActwk) {
    pActwk = pActwk;
    return 1;
}

static sprite_pattern egg4meca_pat0 = {1, {{-32, -28, 0, 459}}};
static sprite_pattern egg4meca_pat1 = {1, {{-32, -28, 0, 460}}};
sprite_pattern *egg4meca_pat[2] = {&egg4meca_pat0, &egg4meca_pat1};
static void (*egg4meca_act_tbl[2])(sprite_status *) = {&egg4meca_ini,
                                                       &egg4meca_01};
static Uint8 egg4awa_pchg0[7] = {9, 0, 1, 2, 3, 4, 255};
static Uint8 egg4awa_pchg1[3] = {255, 4, 255};
static Uint8 egg4awa_pchg2[5] = {19, 4, 5, 6, 255};
static Uint8 egg4awa_pchg3[3] = {255, 6, 255};
static Uint8 egg4awa_pchg4[4] = {19, 7, 8, 252};
static Uint8 *egg4awa_pchg[5] = {egg4awa_pchg0, egg4awa_pchg1, egg4awa_pchg2,
                                 egg4awa_pchg3, egg4awa_pchg4};
static sprite_pattern egg4awa_pat0 = {1, {{-4, -4, 0, 463}}};
static sprite_pattern egg4awa_pat1 = {1, {{-4, -4, 0, 464}}};
static sprite_pattern egg4awa_pat2 = {1, {{-4, -4, 0, 465}}};
static sprite_pattern egg4awa_pat3 = {1, {{-8, -8, 0, 466}}};
static sprite_pattern egg4awa_pat4 = {1, {{-8, -8, 0, 467}}};
static sprite_pattern egg4awa_pat5 = {1, {{-12, -12, 0, 468}}};
static sprite_pattern egg4awa_pat6 = {1, {{-16, -16, 0, 469}}};
static sprite_pattern egg4awa_pat7 = {1, {{-16, -16, 0, 470}}};
static sprite_pattern egg4awa_pat8 = {1, {{-16, -16, 0, 471}}};
sprite_pattern *egg4awa_pat[9] = {&egg4awa_pat0, &egg4awa_pat1, &egg4awa_pat2,
                                  &egg4awa_pat3, &egg4awa_pat4, &egg4awa_pat5,
                                  &egg4awa_pat6, &egg4awa_pat7, &egg4awa_pat8};
static Uint32 (*egg4awa_act_tbl[9])(sprite_status *) = {
    &egg4awa_ini,  &egg4awa_deru, &egg4awa_tuku, &egg4awa_hiro, &egg4awa_roll,
    &egg4awa_ychg, &egg4awa_del,  &egg4awa_out,  &egg4awa_chi};

void egg4meca(sprite_status *pActwk) {
    egg4meca_act_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
}

static void egg4meca_ini(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 4;
    pActwk->sprhsize = 32;
    pActwk->sprvsize = 28;
    pActwk->sproffset = 798;
    pActwk->patbase = egg4meca_pat;

    egg4meca_01(pActwk);
}

static void egg4meca_01(sprite_status *pActwk) {
    sprite_status *pEggact;

    pEggact = &actwk[egg4_get_work(pActwk)->parent_index];
    pActwk->xposi.w.h = pEggact->xposi.w.h;
    pActwk->yposi.w.h = pEggact->yposi.w.h;
}

void egg4awa(sprite_status *pActwk) {
    if (egg4awa_act_tbl[pActwk->r_no0 / 2](pActwk) != 0) {
        actionsub(pActwk);
    }
}

static Uint32 egg4awa_ini(sprite_status *pActwk) {
    int_union RndNum;
    uint_union uRndNum;

    pActwk->r_no0 = 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 3;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 16;
    pActwk->sproffset = 798;
    pActwk->patbase = egg4awa_pat;

    RndNum.l = random();
    RndNum.l = RndNum.w.l;
    RndNum.w.l = RndNum.l % 1280;
    if (RndNum.w.l < 0) {

        RndNum.w.l -= 128;
    } else {
        RndNum.w.l += 128;
    }

    egg4_get_work(pActwk)->angular_speed = RndNum.w.l;

    uRndNum.l = random();
    uRndNum.w.h = 0;
    uRndNum.w.l = (Sint16)(uRndNum.l % 16 + 10);
    egg4_get_work(pActwk)->radius = uRndNum.w.l;

    return egg4awa_deru(pActwk);
}

static Uint32 egg4awa_deru(sprite_status *pActwk) {
    sprite_status *pEggact;
    Sint16 xwk;

    pActwk->yposi.l += -98304;
    if (pActwk->r_no1) {

        if (pActwk->yposi.w.h < 1136) {
            return egg4awa_out(pActwk);
        }
    } else {
        pEggact = &actwk[egg4_get_work(pActwk)->parent_index];
        if (pEggact->yposi.w.h >= pActwk->yposi.w.h) {

            if ((xwk = pActwk->xposi.w.h - pEggact->xposi.w.h) >= 0) {

                egg4_get_work(pActwk)->angle = 32768;
            } else {
                egg4_get_work(pActwk)->angle = 0;
            }

            egg4_get_work(pActwk)->radius = xwk;
            pActwk->yspeed.w = xwk;
            egg4_get_work(pActwk)->x_offset = -xwk;
            egg4_get_work(pActwk)->y_offset = 0;
            pActwk->xposi.w.h = pEggact->xposi.w.h;
            pActwk->yposi.w.h = pEggact->yposi.w.h;
            pActwk->r_no0 = 4;
            pActwk->mstno.b.h = 2;
            return 1;
        }
    }

    egg4_get_work(pActwk)->angle += egg4_get_work(pActwk)->angular_speed;

    pActwk->xposi.w.h -= egg4_get_work(pActwk)->x_offset;
    egg4_posiset(pActwk);

    if (pActwk->patcnt == 4) {
        pActwk->mstno.b.h = 1;
        pActwk->patcnt = 0;
    }

    patchg(pActwk, egg4awa_pchg);
    return 1;
}

static Uint32 egg4awa_tuku(sprite_status *pActwk) {
    sprite_status *pEggact;
    Sint16 RadSpd;

    pEggact = &actwk[egg4_get_work(pActwk)->parent_index];
    pActwk->xposi.w.h = pEggact->xposi.w.h;
    pActwk->yposi.w.h = pEggact->yposi.w.h;
    egg4_posiset(pActwk);

    if ((RadSpd = egg4_get_work(pActwk)->angular_speed) >= 0) {
        RadSpd *= -1;
    }

    egg4_get_work(pActwk)->angle += RadSpd;
    if (pEggact->r_no0 == 10) {
        egg4_get_work(pActwk)->x_offset = 0;
        egg4_get_work(pActwk)->y_offset = 0;
        pActwk->r_no0 = 6;
        pActwk->xposi.l = pEggact->xposi.l;
        pActwk->yposi.l = pEggact->yposi.l;

        RadSpd = (char)egg4_get_work(pActwk)->bubble_slot;
        RadSpd = (Uint16)RadSpd << 12;
        egg4_get_work(pActwk)->angle = RadSpd;
        egg4_get_work(pActwk)->radius = 16;
        pActwk->yspeed.w = 16;
    }

    return 1;
}

static Uint32 egg4awa_hiro(sprite_status *pActwk) {
    Sint32 flg;
    sprite_status *pEggact;

    if (egg4_get_work(pActwk)->flags & 2) {
        awa_hitchk(pActwk);
    }

    flg = 0;

    pEggact = &actwk[egg4_get_work(pActwk)->parent_index];
    pActwk->xposi.l = pEggact->xposi.l;
    pActwk->yposi.l = pEggact->yposi.l;

    if ((Sint32)egg4_get_work(pActwk)->radius < 48) {
        egg4_get_work(pActwk)->radial_speed += 16384;
    } else {
        egg4_get_work(pActwk)->radius = 48;
        ++flg;
    }

    if ((Sint32)pActwk->yspeed.w < 48) {
        sprite_status_add_xspeed_yspeed(pActwk, 16384);
    } else {
        pActwk->yspeed.w = 48;
        ++flg;
    }

    if (egg4_get_work(pEggact)->flags & 8) {

        egg4_get_work(pActwk)->angle += 1152;
    } else {
        egg4_get_work(pActwk)->angle -= 1152;
    }

    egg4_posiset(pActwk);

    if (pActwk->patcnt == 2) {
        pActwk->mstno.b.h = 3;
        pActwk->patcnt = 0;
    }

    if (flg == 2) {
        pActwk->r_no0 = 8;
        flg = egg4_get_work(pActwk)->flags;

        if (!(egg4_get_work(pActwk)->flags & 2)) {
            pActwk->colino = 255;
            pActwk->colicnt = 2;
        }

        egg4_get_work(pActwk)->flags |= 2;
    }

    patchg(pActwk, egg4awa_pchg);

    return 1;
}

static void awa_hitchk(sprite_status *pActwk) {
    sprite_status *pEggact;
    Uint8 flgwk;

    pEggact = &actwk[egg4_get_work(pActwk)->parent_index];
    if (pEggact->r_no0 != 20) {
        if (pActwk->colino)
            return;

        flgwk = egg4_get_work(pEggact)->flags;
        egg4_get_work(pEggact)->flags |= 64;
        if (flgwk & 64) {

            pActwk->colino = 255;
            pActwk->colicnt = 2;
            return;
        }
    }

    pActwk->r_no0 = 12;
    pActwk->mstno.b.h = 4;
    pActwk->patno = pActwk->patcnt = pActwk->pattim = pActwk->pattimm = 0;
    --egg4_get_work(pEggact)->bubble_slot;
    egg4awa_del(pActwk);
}

static Uint32 egg4awa_roll(sprite_status *pActwk) {
    sprite_status *pEggact;

    pEggact = &actwk[egg4_get_work(pActwk)->parent_index];
    pActwk->xposi.l = pEggact->xposi.l;
    pActwk->yposi.l = pEggact->yposi.l;

    if (egg4_get_work(pEggact)->flags & 16) {
        pActwk->r_no0 = 10;
        return egg4awa_ychg(pActwk);
    }

    if (egg4_get_work(pEggact)->flags & 32) {
        pActwk->r_no0 = 16;
        pActwk->sprpri = 3;
        return egg4awa_chi(pActwk);
    }

    awa_hitchk(pActwk);
    if (egg4_get_work(pEggact)->flags & 8) {

        egg4_get_work(pActwk)->angle += 384;
    } else {
        egg4_get_work(pActwk)->angle -= 384;
    }

    egg4_posiset(pActwk);
    return 1;
}

static Uint32 egg4awa_ychg(sprite_status *pActwk) {
    sprite_status *pEggact;

    awa_hitchk(pActwk);

    pEggact = &actwk[egg4_get_work(pActwk)->parent_index];
    pActwk->xposi.l = pEggact->xposi.l;
    pActwk->yposi.l = pEggact->yposi.l;

    if (pActwk->r_no1) {

        egg4_get_work(pActwk)->radial_speed -= 65536;
        if (egg4_get_work(pActwk)->radial_speed > -3145728) {
            ychg_ret(pActwk, pEggact);
        } else {
            egg4_get_work(pActwk)->radial_speed = 3145728;
            pActwk->r_no0 = 8;

            ychg_rad_endproc(pActwk);
            ychg_ret(pActwk, pEggact);
            pActwk->r_no1 = 0;
        }
    } else {
        if ((egg4_get_work(pActwk)->radial_speed -= 65536) < 0) {

            ++pActwk->r_no1;
        }

        ychg_ret(pActwk, pEggact);
    }

    return 1;
}

static void ychg_ret(sprite_status *pActwk, sprite_status *pEggwk) {
    Sint16 spdwk;
    Uint8 radwk;

    spdwk = 1152;

    if (!(egg4_get_work(pEggwk)->flags) & 8) {
        spdwk *= -1;
    }

    egg4_get_work(pActwk)->angle += spdwk;

    radwk = egg4_get_work(pActwk)->angle_high - 64;
    if (egg4_get_work(pEggwk)->flags & 8) {
        radwk ^= 128;
    }

    if (radwk & 128) {

        pActwk->sprpri = 5;
    } else {
        pActwk->sprpri = 3;
    }

    egg4_posiset(pActwk);
}

static void ychg_rad_endproc(sprite_status *pActwk) {
    Uint16 radwk;

    radwk = egg4_get_work(pActwk)->angle;

    if (radwk < 16384) {

        egg4_get_work(pActwk)->angle = 32768 - radwk;

    } else {
        egg4_get_work(pActwk)->angle = 32768 - radwk;
    }
}

static Uint32 egg4awa_del(sprite_status *pActwk) {
    patchg(pActwk, egg4awa_pchg);

    return 1;
}

static Uint32 egg4awa_out(sprite_status *pActwk) {
    frameout(pActwk);

    return 0;
}

static Uint32 egg4awa_chi(sprite_status *pActwk) {
    sprite_status *pEggact;
    Sint16 cntwk, radwk;
    static Sint16 rad_tbl[17] = {0,     0,    -32768, 21845, 16384, 13107,
                                 10922, 9362, 8192,   7281,  6553,  5957,
                                 5461,  5041, 4681,   4369,  4096};

    awa_hitchk(pActwk);

    pEggact = &actwk[egg4_get_work(pActwk)->parent_index];
    pActwk->xposi.l = pEggact->xposi.l;
    pActwk->yposi.l = pEggact->yposi.l;
    egg4_get_work(pActwk)->radial_speed -= 65536;
    sprite_status_add_xspeed_yspeed(pActwk, -65536);

    if (egg4_get_work(pActwk)->radial_speed <= 0x100000) {

        cntwk = egg4_get_work(pEggact)->bubble_slot;
        radwk = rad_tbl[cntwk];
        radwk *= egg4_get_work(pEggact)->spread_count;
        egg4_get_work(pActwk)->angle = radwk;

        ++egg4_get_work(pEggact)->spread_count;

        egg4_get_work(pActwk)->radial_speed = 0x100000;
        sprite_status_set_xspeed_yspeed(pActwk, 0x100000);
        pActwk->r_no0 = 6;
    }

    awa_hitchk(pActwk);
    if (egg4_get_work(pEggact)->flags & 8) {

        egg4_get_work(pActwk)->angle += 1152;
    } else {
        egg4_get_work(pActwk)->angle -= 1152;
    }

    egg4_posiset(pActwk);

    return 1;
}

static sprite_pattern egg4tama_pat0 = {1, {{-4, -4, 0, 461}}};
static sprite_pattern egg4tama_pat1 = {1, {{-4, -4, 0, 462}}};
sprite_pattern *egg4tama_pat[2] = {&egg4tama_pat0, &egg4tama_pat1};
static Uint8 egg4tama_pchg0[4] = {1, 0, 1, 255};
static Uint8 *egg4tama_pchg[1] = {egg4tama_pchg0};
static Uint32 (*egg4tama_act_tbl[3])(sprite_status *) = {
    &egg4tama_ini, &egg4tama_01, &egg4tama_02};

void egg4tama(sprite_status *pActwk) {
    if (egg4tama_act_tbl[pActwk->r_no0 / 2](pActwk) != 0) {
        patchg(pActwk, egg4tama_pchg);
        actionsub(pActwk);
    }
}

static Uint32 egg4tama_ini(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 6;
    pActwk->sprhsize = 4;
    pActwk->sprvsize = 4;
    pActwk->sproffset = 798;
    pActwk->patbase = egg4tama_pat;

    return egg4tama_01(pActwk);
}

static Uint32 egg4tama_01(sprite_status *pActwk) {
    sprite_status *pEggact;

    pEggact = &actwk[egg4_get_work(pActwk)->parent_index];
    if (egg4_get_work(pEggact)->flags & 128) {
        return egg4tama_kill(pActwk);
    }

    pActwk->xposi.w.h = pEggact->xposi.w.h;
    pActwk->yposi.w.h = pEggact->yposi.w.h;
    pActwk->xposi.w.h += egg4_get_work(pActwk)->x_offset;
    pActwk->yposi.w.h += egg4_get_work(pActwk)->y_offset;

    if (++egg4_get_work(pActwk)->timer == 45) {
        pActwk->colino = 254;
        pActwk->colicnt = 2;
        pActwk->r_no0 += 2;
    }

    return 1;
}

static Uint32 egg4tama_02(sprite_status *pActwk) {
    sprite_status *pEggact;

    pEggact = &actwk[egg4_get_work(pActwk)->parent_index];
    if (egg4_get_work(pEggact)->flags & 128) {
        return egg4tama_kill(pActwk);
    }

    pActwk->xposi.l += egg4_get_work(pActwk)->radial_speed;
    pActwk->yposi.l += sprite_status_get_xspeed_yspeed(pActwk);

    if (frameout_chk(pActwk) != 0) {
        return egg4tama_kill(pActwk);
    }

    return 1;
}

static Uint32 egg4tama_kill(sprite_status *pActwk) {
    frameout(pActwk);

    return 0;
}

static Uint32 frameout_chk(sprite_status *pActwk) {
    if (pActwk->xposi.w.h < 2464 || pActwk->xposi.w.h > 2816 ||
        pActwk->yposi.w.h > 1488) {
        return 1;
    }

    return 0;
}
