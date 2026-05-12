#include "../equ.h"
#include "boss_6.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../etc.h"
#include "../goal.h"
#include "../io.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../ridechk.h"
#include "../score.h"
#include "col6c.h"
#include "coli6.h"

static Uint8 stbPatOff;
static Uint8 stbStx;
static char stbRad = 1;
extern Sint16 scr_dir_tbl[];
Uint8 *egg6_pchg[];
sprite_pattern *egg6_pat[];
sprite_pattern *egg6meca0_pat[];
sprite_pattern *egg6meca1_pat[];
Uint8 *egg6meca2_pchg[];
sprite_pattern *egg6meca2_pat[];
sprite_pattern *egg6bakuha_pat[];
Uint8 *egg6bakuha_pchg[];
sprite_pattern *egg6toge_pat[];
sprite_pattern *egg6gareki_pat[];
Uint8 *egg6gareki_pchg[];
sprite_pattern *egg6beam_pat[];
sprite_pattern *egg6beam2_pat[];
sprite_pattern *egg6beam3_pat[];
sprite_pattern *egg6yuka_pat[];
sprite_pattern *egg6beamer_pat[];
sprite_pattern *egg6door_pat[];
sprite_pattern *egg6gareki4_pat[];
sprite_pattern *egg6_pat_yarare[];
Uint8 *egg6beam_pchg[];
void (*boss6_tbl[11])(sprite_status *) = {
    &egg6_start, &egg6_ini,   &egg6_down,  &egg6_hover, &egg6_up,  &egg6_event,
    &egg6_dead1, &egg6_dead2, &egg6_dead3, &egg6_dead4, &egg6_none};

void egg6boss(sprite_status *pActwk) {
    egg6_jisin(pActwk);
    egg6_hitchk(pActwk);

    stbStx = 0;
    boss6_tbl[pActwk->r_no0 / 2](pActwk);
    if (stbStx != 0)
        return;
    egg6_beam_ctrl();
    patchg(pActwk, egg6_pchg);
    actionsub(pActwk);
}

void egg6_hitchk(sprite_status *pActwk) {
    sprite_status *pActbp;

    if (!(pActwk->actfree[2] & 1))
        return;

    if (pActwk->actfree[13] != 0) {
        --pActwk->actfree[13];
        if (pActwk->actfree[13] == 0)
            egg6_coliset(pActwk);
        return;
    }

    pActbp = &actwk[((Uint16 *)pActwk)[25]];
    if (pActwk->colino != 0) {
        if (pActbp->colino != 0)
            return;
    }

    ++pActwk->actfree[15];
    pActwk->colino = 0;
    pActbp->colino = 0;
    pActwk->mstno.b.h = 1;
    pActwk->patno = 0;
    pActwk->actfree[13] = 30;

    soundset(172);
}

void egg6_coliset(sprite_status *pActwk) {
    sprite_status *pActbp;

    pActbp = &actwk[((Uint16 *)pActwk)[25]];
    pActwk->colino = 60;
    pActwk->colicnt = 2;
    pActbp->colino = 61;
    pActbp->colicnt = 2;
}

void egg6_beam_ctrl(void) {
    Uint8 byD0;

    byD0 = get_beam_pos();
    byD0 = 1 << byD0;
    if (!(bossstart & byD0)) {

        if (bossflag & 128)
            return;

        bossflag |= 64;
        return;
    }

    bossflag &= 127;
}

void egg6_start(sprite_status *pActwk) {
    if (actwk[0].xposi.w.h < 2688) {
        stbStx = 1;
        return;
    }
    pActwk->r_no0 = 2;
    egg6_ini(pActwk);
}

void egg6_ini(sprite_status *pActwk) {
    if (!(bossflag & 15)) {

        colorset(6);
        colorset2(6);

        make_meca(pActwk);

        pActwk->actflg |= 4;
        pActwk->sprpri = 4;
        pActwk->sprhsize = 24;
        pActwk->sprvsize = 24;
        pActwk->sproffset = 1004;
        pActwk->patbase = egg6_pat;

        pActwk->actfree[2] |= 1;
        egg6_coliset(pActwk);

        bossflag &= 240;
        bossflag += 6;
        bossflag |= 16;
        bossstart = 6;
        scralim_left = 2528;
        scralim_n_left = 2528;
        scralim_right = 2656;
        scralim_n_right = 2656;
        return;
    }

    ++((Sint16 *)pActwk)[23];
    if (((Sint16 *)pActwk)[23] != 120)
        return;

    ((Sint16 *)pActwk)[23] = 0;
    pActwk->r_no0 = 4;
    pActwk->xspeed.w = 0;
    pActwk->yspeed.w = 768;
    ((Sint16 *)pActwk)[27] = -1;

    sub_sync(103);
}

void egg6_none(sprite_status *pActwk) { pActwk->actno = pActwk->actno; }

void egg6_down(sprite_status *pActwk) {
    ushort_union wD0;
    Uint16 down_pos[3] = {1568, 1064, 552};

    wD0.w = 0;
    add_spd2(pActwk);
    wD0.b.l = pActwk->actfree[3];
    wD0.w = down_pos[wD0.b.l];
    if ((Sint16)wD0.w > pActwk->yposi.w.h)
        return;

    pActwk->yposi.w.h = wD0.w;
    pActwk->r_no0 = 6;
    ((Sint16 *)pActwk)[23] = 0;
    pActwk->actfree[11] = 1;
    pActwk->actfree[10] = 1;

    soundset(177);
}

void egg6_hover(sprite_status *pActwk) {
    ushort_union wD0;

    wD0.w = get_snc_floor();
    if (wD0.b.l < pActwk->actfree[3]) {

        egg6_event_retry(pActwk);
        return;
    }
    fuwafuwa(pActwk);
    if (pActwk->actfree[15] == 0) {
        ++((Sint16 *)pActwk)[23];
        if (((Sint16 *)pActwk)[23] < 300)
            return;
    }

    pActwk->actfree[11] = 0;

    pActwk->r_no0 = 8;
    pActwk->xspeed.w = 0;
    pActwk->yspeed.w = -256;
    ((Sint16 *)pActwk)[27] = -16;

    ((Sint16 *)pActwk)[23] = 0;
}

void fuwafuwa(sprite_status *pActwk) {
    Sint16 iSin;
    Sint16 iCos;
    Uint8 bRad;

    if (pActwk->actfree[3] == 3) {
        ++pActwk->actfree[18];
        bRad = pActwk->actfree[18];

        if ((bRad % 4) == 0)
            stbRad = -stbRad;
        pActwk->xposi.w.h += stbRad;

        return;
    }

    pActwk->yposi.w.h -= ((Sint16 *)pActwk)[31];

    sinset(pActwk->actfree[18], &iSin, &iCos);
    pActwk->actfree[18] += 2;
    iSin <<= 2;
    iSin >>= 8;
    ((Sint16 *)pActwk)[31] = iSin;
    pActwk->yposi.w.h += iSin;
}

void egg6_up(sprite_status *pActwk) {
    ushort_union wD0;
    Sint16 up_pos[3] = {1064, 552, 304};

    add_spd2(pActwk);
    wD0.w = pActwk->actfree[3];
    if (up_pos[wD0.w] < pActwk->yposi.w.h)
        return;

    pActwk->yposi.w.h = up_pos[wD0.w];
    pActwk->actfree[10] = 0;
    pActwk->r_no0 = 10;
    pActwk->actfree[14] = 240;
    ++pActwk->actfree[3];

    if (pActwk->actfree[3] != 3) {
        pActwk->actfree[11] = 1;
        soundset(163);
    }

    pActwk->actfree[15] = 0;
    pActwk->mstno.b.h = 0;
    pActwk->patno = 0;
}

void egg6_event(sprite_status *pActwk) {
    Uint16 wDemoNo;

    fuwafuwa(pActwk);

    ++((Sint16 *)pActwk)[23];
    wDemoNo = ((Sint16 *)pActwk)[23];

    switch (wDemoNo) {
    case 498:
        beamdemo_start(pActwk);
        rakkabutu(pActwk);
        break;
    case 540:
        beam_start(pActwk);
        break;
    case 570:
        bossflag &= 223;
        rakkabutu(pActwk);
        break;
    case 600:
        pActwk->actfree[12] = 1;
        rakkabutu(pActwk);
        break;
    case 630:
        bossflag |= 32;
        pActwk->actfree[12] = 0;
        rakkabutu(pActwk);
        break;
    case 840:
        event_end(pActwk);
        break;
    default:
        rakkabutu(pActwk);
        break;
    }
}

void beamdemo_start(sprite_status *pActwk) {
    Sint16 iD0;
    soundset(178);

    iD0 = get_snc_floor();
    make_beam(iD0);
    rakkabutu(pActwk);
}

void beam_start(sprite_status *pActwk) {
    Uint8 byFloor;
    Uint8 byD1;

    byD1 = bossstart & 31;
    byFloor = get_snc_floor();
    switch (byFloor) {
    case 0:
        byD1 |= 32;
        break;
    case 1:
        byD1 |= 64;
        break;
    case 2:
        byD1 |= 128;
        break;
    }
    bossstart = byD1;
    rakkabutu(pActwk);
}

void rakkabutu(sprite_status *pActwk) {
    Uint16 wD0;
    Sint16 iD0;
    if (((Sint16 *)pActwk)[23] <= 360) {
        wD0 = ((Sint16 *)pActwk)[23] - 360;
        make_gareki(pActwk, wD0);
    }

    if (((Sint16 *)pActwk)[23] >= 180) {
        wD0 = ((Sint16 *)pActwk)[23] - 180;
        make_yuka(pActwk, wD0);
    }

    if (pActwk->actfree[3] != 3)
        return;

    if (pActwk->colino != 0) {

        if (((Sint16 *)pActwk)[23] > 180)
            return;

        iD0 = ((Sint16 *)pActwk)[23] - 1;
        make_toge(pActwk, iD0);
        return;
    }

    pActwk->actfree[2] &= 254;
    ((Sint16 *)pActwk)[23] = 0;
    pActwk->r_no0 = 12;
    soundset(199);
    pActwk->xspeed.w = 768;
    pActwk->yspeed.w = 256;
    ((Sint16 *)pActwk)[27] = 16;

    make_bakuha(pActwk);
    pActwk->mstno.b.h = 2;
    pActwk->patno = 0;
    pActwk->actfree[11] = 255;
    pActwk->actfree[10] = 255;
}

void event_end(sprite_status *pActwk) {

    if (pActwk->actfree[3] == 3) {
        egg6_event_retry(pActwk);
        return;
    }

    ((Sint16 *)pActwk)[23] = 0;
    pActwk->r_no0 = 4;
    pActwk->xspeed.w = 0;
    pActwk->yspeed.w = 768;
    ((Sint16 *)pActwk)[27] = -1;
}

void egg6_event_retry(sprite_status *pActwk) {
    ((Sint16 *)pActwk)[23] = 360;
    pActwk->actfree[10] = 0;
    pActwk->r_no0 = 10;
}

void egg6_dead1(sprite_status *pActwk) {
    dead_snc_pos();
    if (pActwk->yposi.w.h < 672) {
        add_spd2(pActwk);
        pActwk->xspeed.w = -pActwk->xspeed.w;
        return;
    }

    pActwk->r_no0 = 14;
    pActwk->mstno.b.h = 3;
    pActwk->patno = 0;
    pActwk->yposi.w.h = 672;
    ((Sint16 *)pActwk)[23] = 0;
}

void egg6_dead2(sprite_status *pActwk) {
    Sint16 sUsr_tm;
    if (((Sint16 *)pActwk)[23] == 120)
        bossstart &= 31;

    if (((Sint16 *)pActwk)[23] <= 326)
        dead_snc_pos();
    else
        dead_snc_pos2();

    ++((Sint16 *)pActwk)[23];
    sUsr_tm = ((Sint16 *)pActwk)[23];
    if (sUsr_tm == 180) {
        door_open(pActwk);
        return;
    }

    if (sUsr_tm >= 240) {
        colchg_stop();
        if (sUsr_tm < 325) {
            enkei_out(pActwk);
            return;
        }
    }

    if (sUsr_tm == 325) {
        egg6_flashout();
        return;
    }

    if (sUsr_tm == 326) {
        egg6_cgchg(pActwk);
        return;
    }

    if (sUsr_tm == 383) {
        egg6_flashin();
        return;
    }

    if (sUsr_tm >= 384) {
        if (sUsr_tm < 469) {
            enkei_in(pActwk);
            return;
        }
    }
    if (sUsr_tm == 469)
        dead2_end(pActwk);
}

void enkei_in(sprite_status *pActwk) {
    Uint8 byTimer, byCnt;

    byTimer = pActwk->actfree[20];
    byCnt = pActwk->actfree[21];
    fin_boss6(&byTimer, &byCnt);
    pActwk->actfree[20] = byTimer;
    pActwk->actfree[21] = byCnt;
}

void enkei_out(sprite_status *pActwk) {
    Uint8 byTimer, byCnt;

    byTimer = pActwk->actfree[20];
    byCnt = pActwk->actfree[21];
    fout_boss6(&byTimer, &byCnt);
    pActwk->actfree[20] = byTimer;
    pActwk->actfree[21] = byCnt;
}

void egg6_flashout(void) {
    colchg_stop();
    flashout();
}

void egg6_flashin(void) {
    colchg_stop();
    flashin_boss6();
}

void egg6_cgchg(sprite_status *pActwk) {
    sprite_status *pActbp;

    pActwk->mstno.b.h = 4;
    pActwk->patno = 0;
    pActwk->sproffset = 768;
    pActwk->xposi.w.h = 2864;
    pActwk->yposi.w.h = 648;

    pActbp = &actwk[((Uint16 *)pActwk)[25]];
    pActbp->yposi.w.h += 16;
    pActbp->sprpri = 1;
    pActbp->r_no0 += 2;
}

void dead2_end(sprite_status *pActwk) {
    colchg_start();
    pActwk->yspeed.w = 0;
    ((Sint16 *)pActwk)[27] = 0;
    pActwk->xspeed.w = 256;
    ((Sint16 *)pActwk)[26] = 12;

    pActwk->r_no0 = 16;
    ((Sint16 *)pActwk)[23] = 0;
    scralim_right = scr_dir_tbl[2];
    scralim_n_right = scr_dir_tbl[2];
    if (generate_flag == 0)
        sub_sync(29);
    else
        sub_sync(28);

    scoreup(100);
}

void door_open(sprite_status *pActwk) {
    Uint8 byActoff = 2;
    sprite_status *pActDoor;

    do {
        act_search(62, &byActoff, &pActDoor);
    } while (pActDoor->r_no0 != 4);

    pActDoor->r_no0 = 8;
}

void egg6_dead3(sprite_status *pActwk) {
    genecolor_setchk(pActwk);
    add_spd2(pActwk);
    if (pActwk->xposi.w.h >= 3824) {

        pActwk->r_no0 = 18;
        bossstart = 0;
        egg6_dead4(pActwk);
    }
}

void egg6_dead4(sprite_status *pActwk) {
    genecolor_setchk(pActwk);
    stbStx = 1;
}

void genecolor_setchk(sprite_status *pActwk) {
    if (pActwk->actfree[2] & 2)
        return;

    if (actwk[0].xposi.w.h >= 3072) {
        pActwk->actfree[2] |= 2;
        genecolor();
    }
}

void dead_snc_pos2(void) {
    actwk[0].xspeed.w = 0;
    actwk[0].mspeed.w = 0;
    actwk[0].xposi.w.h = 2752;
}

void dead_snc_pos(void) {
    if (actwk[0].cddat & 128)
        return;

    actwk[0].xspeed.w = 0;
    actwk[0].mspeed.w = 0;

    if (actwk[0].yposi.w.h > 652)
        actwk[0].yposi.w.h = 652;
    if (actwk[0].xposi.w.h < 2752)
        actwk[0].xposi.w.h = 2672;
    else
        actwk[0].xposi.w.h = 2832;
}

void (*egg6meca0_tbl[3])(sprite_status *) = {&egg6meca0_ini, &egg6meca0_01,
                                             &egg6meca0_02};

void egg6meca0(sprite_status *pActwk) {
    egg6meca0_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
}

void egg6meca0_ini(sprite_status *pActwk) {
    pActwk->r_no0 = 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 5;
    pActwk->sprhsize = 64;
    pActwk->sprvsize = 48;
    pActwk->sproffset = 8960;
    pActwk->patbase = egg6meca0_pat;
}

void egg6meca0_01(sprite_status *pActwk) {
    sprite_status *pActbp;

    pActbp = &actwk[((Uint16 *)pActwk)[25]];
    pActwk->xposi.w.h = pActbp->xposi.w.h;
    pActwk->yposi.w.h = pActbp->yposi.w.h - 16;
}

void egg6meca0_02(sprite_status *pActwk) {
    pActwk->patno = 1;
    pActwk->sprhsize = 64;
    pActwk->sprvsize = 16;
    pActwk->sproffset = 9107;
    ride_on_chk(pActwk, &actwk[0]);
}

void (*egg6meca1_tbl[2])(sprite_status *) = {&egg6meca1_ini, &egg6meca1_01};

void egg6meca1(sprite_status *pActwk) {
    egg6meca1_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
}

void egg6meca1_ini(sprite_status *pActwk) {
    pActwk->r_no0 = 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 5;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 16;
    pActwk->sproffset = 8960;
    pActwk->patbase = egg6meca1_pat;
}

void egg6meca1_01(sprite_status *pActwk) {
    Uint8 byD0;
    char egg6meca1_pchg[5] = {0, 2, 1, 3, -1};
    sprite_status *pActbp;

    pActbp = &actwk[((Uint16 *)pActwk)[25]];
    pActwk->xposi.w.h = pActbp->xposi.w.h;
    pActwk->yposi.w.h = pActbp->yposi.w.h;
    pActwk->yposi.w.h -= 64;

    if (((char *)pActbp)[56] == 0)
        return;

    if (((char *)pActbp)[56] < 0) {

        stbStx = 1;
        frameout(pActwk);
        return;
    }
    if (((char *)pActbp)[56] == 1) {

        pActwk->pattimm = 20;

        pActwk->pattim = 2;
        ++((char *)pActbp)[56];
        return;
    }

    if (--pActwk->pattim != 0)
        return;

    ++pActwk->patno;
    if (egg6meca1_pchg[pActwk->patno] < 0)
        pActwk->patno = 0;
    byD0 = pActwk->pattimm;
    byD0 -= 3;

    if ((char)byD0 < 2)
        byD0 = 2;
    pActwk->pattimm = byD0;
    pActwk->pattim = byD0;
}

void (*egg6meca2_tbl[2])(sprite_status *) = {&egg6meca2_ini, &egg6meca2_01};

void egg6meca2(sprite_status *pActwk) {
    pActwk->colino = 190;
    pActwk->colicnt = 2;
    stbStx = 0;
    egg6meca2_tbl[pActwk->r_no0 / 2](pActwk);
    if (stbStx != 0)
        return;

    patchg(pActwk, egg6meca2_pchg);
    actionsub(pActwk);
}

void egg6meca2_ini(sprite_status *pActwk) {
    pActwk->r_no0 = 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 5;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 16;
    pActwk->sproffset = 8960;
    pActwk->patbase = egg6meca2_pat;
}

void egg6meca2_01(sprite_status *pActwk) {
    sprite_status *pActbp;

    pActbp = &actwk[((Uint16 *)pActwk)[25]];
    pActwk->xposi.w.h = pActbp->xposi.w.h;
    pActwk->yposi.w.h = pActbp->yposi.w.h;
    pActwk->yposi.w.h += 64;
    pActwk->mstno.b.h = pActbp->actfree[11];
    if (pActwk->mstno.b.h < 0) {
        stbStx = 1;
        frameout(pActwk);
    }
}

void (*egg6bakuha_tbl[3])(sprite_status *) = {&egg6bakuha_ini, &egg6bakuha_01,
                                              &egg6bakuha_02};

void egg6bakuha(sprite_status *pActwk) {
    egg6bakuha_tbl[pActwk->r_no0 / 2](pActwk);
}

void egg6bakuha_ini(sprite_status *pActwk) {
    pActwk->actflg = 4;
    pActwk->sproffset = 8960;
    pActwk->patbase = egg6bakuha_pat;
    if (pActwk->mstno.b.h != 0) {

        pActwk->sprpri = 3;
        pActwk->sprhsize = 12;
        pActwk->sprvsize = 16;
        pActwk->r_no0 = 2;
        egg6bakuha_01(pActwk);
    } else {

        pActwk->sprpri = 2;
        pActwk->sprhsize = 16;
        pActwk->sprvsize = 16;
        pActwk->r_no0 = 4;
        egg6bakuha_02(pActwk);
    }
}

void egg6bakuha_01(sprite_status *pActwk) {
    sprite_status *pActbp;

    pActbp = &actwk[((Uint16 *)pActwk)[25]];
    ++((Sint16 *)pActwk)[23];
    if (((Sint16 *)pActwk)[23] == 420) {
        frameout(pActwk);
        return;
    }
    pActwk->xposi.w.h = pActbp->xposi.w.h;
    pActwk->yposi.w.h = pActbp->yposi.w.h;
    pActwk->yposi.w.h -= 64;

    patchg(pActwk, egg6bakuha_pchg);
    if (pActwk->patno < 7) {
        make_bakuha2(pActwk);
        actionsub(pActwk);
    }
}

void egg6bakuha_02(sprite_status *pActwk) {
    ++((Sint16 *)pActwk)[23];
    if (((Sint16 *)pActwk)[23] == 24) {
        frameout(pActwk);
        return;
    }
    add_spd(pActwk);
    patchg(pActwk, egg6bakuha_pchg);
    actionsub(pActwk);
}

void (*egg6toge_tbl[3])(sprite_status *) = {&egg6toge_ini, &egg6toge_01,
                                            &egg6toge_02};

void egg6toge(sprite_status *pActwk) {
    egg6toge_tbl[pActwk->r_no0 / 2](pActwk);
}

void egg6toge_ini(sprite_status *pActwk) {
    pActwk->r_no0 = 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 3;
    pActwk->sprhsize = 4;
    pActwk->sprvsize = 12;
    pActwk->sproffset = 8960;
    pActwk->patbase = egg6toge_pat;
    pActwk->colino = 191;
    pActwk->colicnt = 2;
    egg6toge_01(pActwk);
}

void egg6toge_01(sprite_status *pActwk) {
    Sint16 iD1;

    pActwk->yposi.l += 229376;
    iD1 = emycol_d(pActwk);
    iD1 -= 3;
    if (iD1 <= 0) {
        pActwk->yposi.w.h -= iD1;
        pActwk->r_no0 += 2;
        pActwk->colino = 0;
        pActwk->colicnt = 0;
    }

    actionsub(pActwk);
}

void egg6toge_02(sprite_status *pActwk) {
    ++((char *)pActwk)[47];
    if (((char *)pActwk)[47] < 5) {
        actionsub(pActwk);
        return;
    }

    if (((char *)pActwk)[47] != 10)
        return;

    ++((char *)pActwk)[46];
    if (((char *)pActwk)[46] != 7)
        ((char *)pActwk)[47] = 0;
    else
        frameout(pActwk);
}

void (*egg6gareki_tbl[3])(sprite_status *) = {&egg6gareki_ini, &egg6gareki_01,
                                              &egg6gareki_02};

void egg6gareki(sprite_status *pActwk) {
    egg6gareki_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_chk(pActwk);
}

void egg6gareki_ini(sprite_status *pActwk) {
    pActwk->r_no0 = 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 3;

    pActwk->sprhsize = 4;
    pActwk->sprvsize = 4;

    pActwk->sproffset = 17472;
    pActwk->patbase = egg6gareki_pat;
}

void egg6gareki_01(sprite_status *pActwk) {
    Sint16 iD1;

    patchg(pActwk, egg6gareki_pchg);

    add_spd2(pActwk);
    iD1 = emycol_d(pActwk);
    --iD1;
    if (iD1 <= 0) {

        pActwk->yposi.w.h -= iD1;
        pActwk->r_no0 += 2;
        make_gareki4(pActwk);
    }
}

void egg6gareki_02(sprite_status *pActwk) { add_spd2(pActwk); }

void (*egg6beam_tbl[2])(sprite_status *) = {&egg6beam_ini, &egg6beam_01};

void egg6beam(sprite_status *pActwk) {

    egg6beam_tbl[pActwk->r_no0 / 2](pActwk);
}

static sprite_pattern **e6beam_tbl[3] = {egg6beam_pat, egg6beam2_pat,
                                         egg6beam3_pat};

void egg6beam_ini(sprite_status *pActwk) {
    pActwk->actfree[10] = 4;
    pActwk->actfree[11] = 0;

    pActwk->r_no0 = 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 1;
    pActwk->sprhsize = 208;
    pActwk->sprvsize = 8;
    pActwk->sproffset = 17520;
    pActwk->patbase = egg6beam_pat;
    egg6beam_01(pActwk);
}

void egg6beam_01(sprite_status *pActwk) {
    Uint8 byD0;
    Uint8 bywk;

    if (--pActwk->actfree[10] == 0) {

        pActwk->actfree[10] = 4;
        if (++pActwk->actfree[11] > 2)
            pActwk->actfree[11] = 0;
        bywk = pActwk->actfree[11];
        pActwk->patbase = e6beam_tbl[bywk];
    }

    ++((char *)pActwk)[47];
    byD0 = ((char *)pActwk)[47];
    byD0 -= 30;
    if ((char)byD0 >= 0) {
        byD0 /= 3;
        if (byD0 == 5) {
            frameout(pActwk);
            return;
        }
        pActwk->patno = byD0;
    }

    if (pActwk->userflag.b.h != 0)
        pActwk->actflg |= 1;
    actionsub(pActwk);
}

void (*egg6yuka_tbl[3])(sprite_status *) = {&egg6yuka_ini, &egg6yuka_01,
                                            &egg6yuka_02};

void egg6yuka(sprite_status *pActwk) {
    egg6yuka_tbl[pActwk->r_no0 / 2](pActwk);
    egg6_yuka_ridechk(pActwk);
    actionsub(pActwk);
}

void egg6yuka_ini(sprite_status *pActwk) {
    short_union iD0;

    pActwk->r_no0 = 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 3;
    pActwk->sprhsize = 14;

    pActwk->sprhs = 14;

    pActwk->sprvsize = 16;
    pActwk->sproffset = 1088;
    pActwk->patbase = egg6yuka_pat;

    iD0.w = 0;
    iD0.b.l = pActwk->userflag.b.l;
    iD0.w <<= 3;
    ((Sint16 *)pActwk)[28] = iD0.w;

    pActwk->yspeed.w = 0;
    ((Sint16 *)pActwk)[27] = 11;
}

static Sint16 yuka_tbl[24] = {1680, 1024, -1536, 11, 1680, 1024, -1536, 14,
                              1168, 736,  -1536, 11, 1168, 736,  -1536, 14,
                              656,  272,  -1536, 11, 656,  272,  -1536, 14};

void egg6yuka_01(sprite_status *pActwk) {
    Sint16 iD0, iD1;

    add_spd2(pActwk);

    iD1 = ((Sint16 *)pActwk)[28];
    iD1 += ((Sint16 *)pActwk)[30];
    iD0 = yuka_tbl[iD1];
    if (iD0 > pActwk->yposi.w.h) {
        iD0 = yuka_tbl[iD1 + 1];
        if (iD0 >= pActwk->yposi.w.h)
            pActwk->yposi.w.h = iD0;
        return;
    }

    pActwk->xspeed.w = 0;
    pActwk->yspeed.w = 0;
    pActwk->yposi.w.h = iD0;
    pActwk->r_no0 = 4;
    soundset(180);
}

void egg6yuka_02(sprite_status *pActwk) {
    Sint16 iD0, iD1;
    sprite_status *pActbp;

    pActbp = &actwk[((Uint16 *)pActwk)[25]];
    if (pActbp->actfree[12] == 0)
        return;

    pActwk->r_no0 = 2;

    iD0 = pActwk->xposi.w.h - actwk[0].xposi.w.h;
    if (iD0 < 0)
        iD0 = -iD0;

    ((Uint16 *)pActwk)[30] = 0;
    if (iD0 < 24)
        ((Uint16 *)pActwk)[30] = 4;

    iD1 = ((Uint16 *)pActwk)[28];
    iD1 += ((Uint16 *)pActwk)[30];
    pActwk->yspeed.w = yuka_tbl[iD1 + 2];
    ((Sint16 *)pActwk)[27] = yuka_tbl[iD1 + 3];
}

void egg6_yuka_ridechk(sprite_status *pActwk) {
    Sint16 iSpd_sav;

    if (pActwk->yspeed.w == 0)

        pActwk->actfree[18] = 0;
    else {
        pActwk->actfree[18] = 4;
    }
    pActwk->sprvsize = pActwk->sprvsize + pActwk->actfree[18];

    if (actwk[0].actno != 0) {
        if (egg6_yuka_hasami_chk(pActwk) == 0) {
            iSpd_sav = actwk[0].yspeed.w;
            if ((actwk[0].cddat & 8) && !(actwk[0].cddat & 2)) {

                actwk[0].yspeed.w = 0;
            }

            if (ride_on_chk(pActwk, &actwk[0]) != 0)
                egg6_yuka_ride_on(pActwk);
            else
                actwk[0].yspeed.w = iSpd_sav;
        }
    }
    pActwk->sprvsize = pActwk->sprvsize - pActwk->actfree[18];
}

void egg6_yuka_ride_on(sprite_status *pActwk) {
    Sint16 iD0;

    actwk[0].yposi.l = pActwk->yposi.l;
    iD0 = actwk[0].sprvsize;
    iD0 += 16;
    actwk[0].yposi.w.h -= iD0;
    if (pActwk->yspeed.w < 0)

        actwk[0].yspeed.w = pActwk->yspeed.w;
    else
        actwk[0].yspeed.w = 0;
}

Sint16 egg6_yuka_hasami_chk(sprite_status *pActwk) {
    Sint16 iD0, iD1;

    iD1 = pActwk->xposi.w.h - actwk[0].xposi.w.h;
    if (iD1 < 0)
        iD1 = -iD1;

    if (iD1 < 23) {
        if (actwk[0].yposi.w.h < 265) {

            mydamage(pActwk);
            actwk[0].yspeed.w = 512;
            return -1;
        }
        if (pActwk->yspeed.w > 0) {
            iD0 = (char)actwk[0].sprvsize;
            iD1 = actwk[0].yposi.w.h - iD0;

            iD1 -= pActwk->yposi.w.h;
            if (iD1 >= 0 && iD1 < 33) {
                return mydamage(pActwk);
            }
        }
    }

    if (actwk[0].yposi.w.h < pActwk->yposi.w.h)
        return 0;

    return -1;
}

Sint16 mydamage(sprite_status *pActwk) {

    if (!(actwk[0].cddat & 128)) {
        pcole(&actwk[0], pActwk);
        if (actwk[0].mstno.b.h == 24) {
            actwk[0].cddat |= 128;
        }
    }
    return -1;
}

void (*egg6door_tbl[6])(sprite_status *) = {
    &egg6door_ini,    &egg6door_closewait, &egg6door_openwait,
    &egg6door_closed, &egg6door_move,      &egg6door_beamer};

void egg6door(sprite_status *pActwk) {
    egg6door_tbl[pActwk->r_no0 / 2](pActwk);

    if (pActwk->userflag.b.h >= 0) {
        Sint16 xwk, ywk;

        xwk = ((Sint16 *)pActwk)[29];
        ywk = ((Sint16 *)pActwk)[30];
        pActwk->xposi.w.h += xwk;
        pActwk->yposi.w.h += ywk;
        ride_on_chk(pActwk, &actwk[0]);

        pActwk->xposi.w.h -= xwk;
        pActwk->yposi.w.h -= ywk;
    }

    actionsub(pActwk);
}

void egg6door_ini(sprite_status *pActwk) {
    pActwk->actflg = 4;
    pActwk->sprpri = 2;
    pActwk->sproffset = 17496;

    pActwk->patno = pActwk->userflag.b.h & 15;
    if (pActwk->userflag.b.h >= 0) {

        pActwk->patbase = egg6door_pat;
        if (pActwk->userflag.b.h & 1) {

            pActwk->yspeed.w = -512;
            pActwk->r_no0 = 4;

            ((Sint16 *)pActwk)[28] = -24576;
            ((Sint16 *)pActwk)[29] = 20;
            ((Sint16 *)pActwk)[30] = -26;
            pActwk->sprhsize = 8;
            pActwk->sprvsize = 80;
            return;
        }

        pActwk->sprhsize = 8;
        pActwk->sprvsize = 16;
        pActwk->yspeed.w = 512;
        pActwk->r_no0 = 2;

        ((Sint16 *)pActwk)[28] = 16384;
        ((Sint16 *)pActwk)[29] = -20;
        ((Sint16 *)pActwk)[30] = -10;
    } else {

        pActwk->sprhsize = 8;
        pActwk->sprvsize = 16;
        pActwk->patbase = egg6beamer_pat;
        pActwk->r_no0 = 10;
    }
}

void egg6door_closewait(sprite_status *pActwk) {
    bossflag &= 127;
    bossflag &= 223;

    if (actwk[0].xposi.w.h >= 2352)
        bossflag |= 64;

    if (actwk[0].xposi.w.h >= 2480)
        bossflag |= 32;

    if (actwk[0].xposi.w.h >= 2656)
        pActwk->r_no0 = 8;
}

void egg6door_move(sprite_status *pActwk) {
    short_union iD0;
    add_spd(pActwk);
    ((Sint16 *)pActwk)[23] += pActwk->yspeed.w;

    iD0.w = ((Sint16 *)pActwk)[28];
    if (iD0.w >= 0) {

        if (iD0.w > ((Sint16 *)pActwk)[23])
            return;

        pActwk->r_no0 = 6;
    } else {

        if (iD0.w < ((Sint16 *)pActwk)[23])
            return;

        pActwk->r_no0 = 10;
    }

    if (pActwk->userflag.b.h & 1) {

        pActwk->sprhsize = 9;
        pActwk->sprvsize = 16;
    } else {
        pActwk->sprhsize = 9;
        pActwk->sprvsize = 32;
    }
}

void egg6door_closed(sprite_status *pActwk) {
    if (actwk[0].actno != 0) {
        if (actwk[0].xposi.w.h < 2560)
            actwk[0].xposi.w.h = 2560;
    }
    egg6door_beamer(pActwk);
}

void egg6door_openwait(sprite_status *pActwk) {
    if (actwk[0].actno != 0) {
        if (actwk[0].xposi.w.h > 2944)
            actwk[0].xposi.w.h = 2944;
    }
    egg6door_beamer(pActwk);
}

void egg6door_beamer(sprite_status *pActwk) {
    short_union iD0;
    sprite_status *pPlaywk;

    pPlaywk = &actwk[0];
    iD0.b.l = (Uint8)(1 << pActwk->userflag.b.l);
    if (!(bossstart & iD0.b.l))
        return;

    if (pPlaywk->actno != 0) {
        iD0.w = pActwk->yposi.w.h - pPlaywk->yposi.w.h;
        if (iD0.w < 0)
            iD0.w = -iD0.w;
        if (iD0.w >= 8)
            return;

        if (((Sint16 *)pPlaywk)[26] != 0)
            return;

        if (actwk[0].r_no0 != 6 && !(actwk[0].cddat & 128))
            mydamage(pActwk);
    }
}

void add_spd2(sprite_status *pActwk) {
    int_union lD0;

    pActwk->xspeed.w += ((Sint16 *)pActwk)[26];
    pActwk->yspeed.w += ((Sint16 *)pActwk)[27];

    lD0.l = pActwk->xspeed.w;
    pActwk->xposi.l += lD0.l << 8;
    lD0.l = pActwk->yspeed.w;
    pActwk->yposi.l += lD0.l << 8;
}

void add_spd(sprite_status *pActwk) {
    int_union lD0;

    lD0.l = pActwk->xspeed.w;
    pActwk->xposi.l += lD0.l << 8;
    lD0.l = pActwk->yspeed.w;
    pActwk->yposi.l += lD0.l << 8;
}

Sint16 act_search(Uint8 wActNo, Uint8 *bpActoff, sprite_status **ppActAdr) {
    while (*bpActoff < 128) {
        if (wActNo == actwk[(*bpActoff)++].actno) {
            *ppActAdr = &actwk[*bpActoff - 1];
            return 0;
        }
    }
    return -1;
}

static Sint16 yuka_xini[12] = {2712, 2792, 2632, 2872, 2608, 2680,
                               2800, 2896, 2832, 2904, 2672, 2600};
static Sint16 gareki_yini[3] = {1420, 908, 396};

Sint16 get_gareki_y(sprite_status *pActwk) {
    Sint16 iD0;
    iD0 = pActwk->actfree[3];
    --iD0;
    return gareki_yini[iD0];
}

void make_toge(sprite_status *pActwk, Sint16 iD0) {
    Sint16 iD1, iD2;
    sprite_status *pActfree;

    iD2 = iD0 / 35;
    iD0 = iD0 % 35;
    if (iD0 != 0)
        return;

    if (actwkchk(&pActfree) != 0)
        return;

    ((Uint16 *)pActfree)[25] = pActwk - actwk;
    pActfree->actno = 56;

    pActfree->yposi.w.h = get_gareki_y(pActwk);

    if ((iD2 & 65535) == 0) {
        if (actwk[0].xposi.w.h < 2752)
            pActwk->actfree[19] = 1;
        else
            pActwk->actfree[19] = 0;
    }

    iD1 = 32;
    if (pActwk->actfree[19] != 0)
        iD0 = 2592;
    else {
        iD1 = -iD1;
        iD0 = 2912;
    }
    iD0 += iD2 * iD1;
    pActfree->xposi.w.h = iD0;
}

void make_yuka(sprite_status *pActwk, Sint16 iD0) {
    Sint16 iD1, iD2, iSav;
    sprite_status *pActfree;

    iSav = iD0;
    iD2 = iD0 / 20;
    iD0 = iD0 % 20;
    if (iD0 != 0)
        return;

    if (iD2 >= 4)
        return;

    if (actwkchk(&pActfree) != 0)
        return;

    pActfree->userflag.b.h = (Uint8)iD2;

    ((Uint16 *)pActfree)[25] = pActwk - actwk;
    pActfree->actno = 61;

    pActfree->yposi.w.h = get_gareki_y(pActwk);

    iD1 = pActwk->actfree[3] - 1;
    pActfree->userflag.b.l = (Uint8)iD1;

    iD1 <<= 2;
    iD1 += iD2;
    pActfree->xposi.w.h = yuka_xini[iD1];
}

void make_gareki(sprite_status *pActwk, Sint16 iD0) {
    Sint16 iD2;
    int_union lD0;
    sprite_status *pActfree;

    iD2 = iD0 / 16;
    iD0 = iD0 % 16;
    if (iD0 != 0)
        return;

    if (actwkchk(&pActfree) != 0)
        return;

    ((Uint16 *)pActfree)[25] = pActwk - actwk;
    pActfree->actno = 57;

    pActfree->yposi.w.h = get_gareki_y(pActwk);

    lD0.l = random();
    lD0.l = lD0.w.l;

    iD0 = lD0.l % 80;
    iD0 += 2752;
    pActfree->xposi.w.h = iD0;

    iD0 = lD0.l % 640;
    pActfree->xspeed.w = iD0;

    pActfree->yspeed.w = 256;

    ((Sint16 *)pActfree)[27] = 8;
    iD0 = lD0.w.l & 15;
    ((Sint16 *)pActfree)[27] += iD0;
}

void make_gareki4(sprite_status *pActwk) {
    Sint16 i, offs;
    Sint16 para[16] = {512, -448, 0, 48, -512, -448, 0, 48,
                       256, -640, 0, 48, -256, -640, 0, 48};
    sprite_status *pActfree;

    pActfree = pActwk;
    offs = 0;
    for (i = 3; i >= 0; --i) {
        if (i != 3) {
            if (actwkchk(&pActfree) != 0)
                return;

            pActfree->actno = pActwk->actno;
            pActfree->r_no0 = pActwk->r_no0;
            pActfree->actflg = pActwk->actflg;
            pActfree->sprpri = pActwk->sprpri;
            pActfree->sprhsize = pActwk->sprhsize;
            pActfree->sprvsize = pActwk->sprvsize;
            pActfree->sproffset = pActwk->sproffset;

            pActfree->xposi.w.h = pActwk->xposi.w.h;
            pActfree->yposi.w.h = pActwk->yposi.w.h;
        }
        pActfree->patbase = egg6gareki4_pat;
        pActfree->patno = i;
        pActfree->xspeed.w = para[offs++];
        pActfree->yspeed.w = para[offs++];
        ((Sint16 *)pActfree)[26] = para[offs++];
        ((Sint16 *)pActfree)[27] = para[offs++];
    }
}

void make_beam(Sint16 iD0) {
    Sint16 beam_ytbl[3] = {1672, 1160, 648};
    Sint16 xposi_tbl[2] = {2584, 2920};
    Sint16 iD2, iD3, iD4;
    sprite_status *pActfree;

    iD3 = beam_ytbl[iD0];

    for (iD4 = 0; iD4 < 2; ++iD4) {
        if (actwkchk(&pActfree) != 0)
            return;
        iD2 = xposi_tbl[iD4];
        pActfree->actno = 64;
        pActfree->xposi.w.h = iD2;
        pActfree->yposi.w.h = iD3;
        pActfree->userflag.b.h = (Uint8)iD4;
    }
}

void make_bakuha(sprite_status *pActwk) {
    sprite_status *pActfree;

    if (actwkchk(&pActfree) != 0)
        return;
    soundset(158);
    pActfree->actno = 63;
    pActfree->mstno.b.h = 1;
    ((Uint16 *)pActfree)[25] = pActwk - actwk;
    pActwk->actno = pActwk->actno;
}

void make_bakuha2(sprite_status *pActwk) {
    sprite_status *pActfree;
    int_union lD0, lD1;

    if (actwkchk(&pActfree) != 0)
        return;
    soundset(158);
    pActfree->actno = 63;

    pActfree->xposi.w.h = pActwk->xposi.w.h;
    pActfree->yposi.w.h = pActwk->yposi.w.h - 8;
    lD0.l = random();
    lD1.l = lD0.l;
    lD0.l = lD0.w.l & 65535;
    lD0.l = lD0.w.l;

    lD1.l = lD1.w.h & 32767;
    lD1.l = -lD1.l;

    pActfree->xspeed.w = lD0.l % 1024;
    pActfree->yspeed.w = lD1.l % 1024;
}

void make_meca(sprite_status *pActwk) {
    sprite_status *pActfree;

    if (actwkchk(&pActfree) != 0)
        return;
    ((Uint16 *)pActfree)[25] = pActwk - actwk;
    pActfree->actno = 53;
    ((Uint16 *)pActwk)[25] = pActfree - actwk;

    if (actwkchk(&pActfree) != 0)
        return;
    ((Uint16 *)pActfree)[25] = pActwk - actwk;
    pActfree->actno = 54;

    if (actwkchk(&pActfree) != 0)
        return;
    ((Uint16 *)pActfree)[25] = pActwk - actwk;
    pActfree->actno = 55;
}

void frameout_chk(sprite_status *pActwk) {
    if (pActwk->yposi.w.h > 1776) {
        frameout(pActwk);
    }
}

Sint16 get_snc_floor(void) {
    if (actwk[0].yposi.w.h <= 664)
        return 2;
    if (actwk[0].yposi.w.h <= 1176)
        return 1;

    return 0;
}

Sint16 get_beam_pos(void) {
    if (actwk[0].yposi.w.h <= 864)
        return 7;
    if (actwk[0].yposi.w.h <= 1376)
        return 6;

    return 5;
}

void egg6_jisin(sprite_status *pActwk) {
    Sint16 iD0;

    if (pActwk->actfree[14] != 0) {
        --pActwk->actfree[14];
        if (!(pActwk->actfree[14] & 1))
            iD0 = -2;
        else {
            iD0 = -6;
        }
        scralim_down += iD0;
        scralim_n_down += iD0;
    }
}

void colchg_set(Uint8 byD0) {
    Sint16 i;
    for (i = 0; i < 6; ++i) {
        clchgtim[i] = byD0;
    }
}

void colchg_stop(void) { colchg_set(128); }

void colchg_start(void) { colchg_set(0); }

Uint8 e6pat_pchg0[22] = {4, 0, 0, 1, 1, 0, 2, 3, 3, 2, 2,
                         1, 1, 0, 0, 1, 3, 2, 2, 3, 3, 255};
Uint8 e6pat_pchg1[6] = {4, 6, 4, 7, 5, 255};
Uint8 e6pat_pchg2[3] = {255, 8, 255};
Uint8 e6pat_pchg3[4] = {20, 9, 10, 255};
Uint8 e6pat_pchg4[10] = {2, 11, 14, 13, 16, 12, 15, 13, 16, 255};
Uint8 *egg6_pchg[5] = {e6pat_pchg0, e6pat_pchg1, e6pat_pchg2, e6pat_pchg3,
                       e6pat_pchg4};
sprite_pattern e6spr_pat00 = {1, {{-24, -20, 0, 462}}};
sprite_pattern e6spr_pat01 = {1, {{-24, -20, 0, 463}}};
sprite_pattern e6spr_pat02 = {1, {{-24, -20, 0, 464}}};
sprite_pattern e6spr_pat03 = {1, {{-24, -20, 0, 465}}};
sprite_pattern e6spr_pat04 = {1, {{-24, -20, 0, 466}}};
sprite_pattern e6spr_pat05 = {1, {{-24, -20, 0, 467}}};
sprite_pattern e6spr_pat06 = {1, {{-24, -36, 0, 468}}};
sprite_pattern e6spr_pat07 = {1, {{-24, -36, 0, 469}}};
sprite_pattern e6spr_pat08 = {1, {{-28, -36, 0, 470}}};
sprite_pattern e6spr_pat09 = {1, {{-24, -28, 0, 471}}};
sprite_pattern e6spr_pat10 = {1, {{-24, -28, 0, 472}}};
sprite_pattern e6spr_pat11 = {1, {{-32, -36, 0, 473}}};
sprite_pattern e6spr_pat12 = {1, {{-32, -32, 0, 474}}};
sprite_pattern e6spr_pat13 = {1, {{-32, -34, 0, 475}}};
sprite_pattern e6spr_pat14 = {1, {{-32, -52, 0, 476}}};
sprite_pattern e6spr_pat15 = {1, {{-32, -52, 0, 477}}};
sprite_pattern e6spr_pat16 = {1, {{-32, -50, 0, 478}}};
sprite_pattern *egg6_pat[17] = {
    &e6spr_pat00, &e6spr_pat01, &e6spr_pat02, &e6spr_pat03, &e6spr_pat04,
    &e6spr_pat05, &e6spr_pat06, &e6spr_pat07, &e6spr_pat08, &e6spr_pat09,
    &e6spr_pat10, &e6spr_pat11, &e6spr_pat12, &e6spr_pat13, &e6spr_pat14,
    &e6spr_pat15, &e6spr_pat16};
sprite_pattern *egg6_pat_yarare[6] = {&e6spr_pat11, &e6spr_pat12, &e6spr_pat13,
                                      &e6spr_pat14, &e6spr_pat15, &e6spr_pat16};
sprite_pattern e6meca_pat00 = {1, {{-64, -32, 0, 479}}};
sprite_pattern e6meca_pat01 = {1, {{-64, -16, 0, 480}}};
sprite_pattern *egg6meca0_pat[2] = {&e6meca_pat00, &e6meca_pat01};
sprite_pattern e6hari_pat00 = {1, {{-16, -16, 0, 481}}};
sprite_pattern e6hari_pat01 = {1, {{-16, -16, 0, 482}}};
sprite_pattern e6hari_pat02 = {1, {{-16, -16, 0, 483}}};
sprite_pattern e6hari_pat03 = {1, {{-16, -16, 0, 484}}};
sprite_pattern *egg6meca1_pat[4] = {&e6hari_pat00, &e6hari_pat01, &e6hari_pat02,
                                    &e6hari_pat03};
Uint8 egg6_fire_pchg00[6] = {2, 0, 2, 1, 3, 255};
Uint8 egg6_fire_pchg01[4] = {1, 2, 3, 255};
Uint8 *egg6meca2_pchg[2] = {egg6_fire_pchg00, egg6_fire_pchg01};
sprite_pattern e6fire_pat00 = {1, {{-16, -16, 0, 485}}};
sprite_pattern e6fire_pat01 = {1, {{-16, -16, 0, 486}}};
sprite_pattern e6fire_pat02 = {1, {{-16, -16, 0, 487}}};
sprite_pattern e6fire_pat03 = {1, {{-16, -16, 0, 488}}};
sprite_pattern *egg6meca2_pat[4] = {&e6fire_pat00, &e6fire_pat01, &e6fire_pat02,
                                    &e6fire_pat03};
sprite_pattern e6toge_pat00 = {1, {{-4, -12, 0, 489}}};
sprite_pattern *egg6toge_pat[1] = {&e6toge_pat00};
Uint8 egg6bakuha_pchg00[5] = {8, 0, 1, 2, 255};
Uint8 egg6bakuha_pchg01[12] = {2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 7, 255};
Uint8 *egg6bakuha_pchg[2] = {egg6bakuha_pchg00, egg6bakuha_pchg01};
sprite_pattern e6bakuha_pat00 = {1, {{-12, -12, 0, 490}}};
sprite_pattern e6bakuha_pat01 = {1, {{-16, -16, 0, 491}}};
sprite_pattern e6bakuha_pat02 = {1, {{-16, -16, 0, 492}}};
sprite_pattern e6bakuha_pat03 = {1, {{-12, -16, 0, 493}}};
sprite_pattern e6bakuha_pat04 = {1, {{-12, -16, 0, 494}}};
sprite_pattern e6bakuha_pat05 = {1, {{-12, -16, 0, 495}}};
sprite_pattern e6bakuha_pat06 = {1, {{-12, -16, 0, 496}}};
sprite_pattern *egg6bakuha_pat[8] = {
    &e6bakuha_pat00, &e6bakuha_pat01, &e6bakuha_pat02, &e6bakuha_pat03,
    &e6bakuha_pat04, &e6bakuha_pat05, &e6bakuha_pat06, &e6bakuha_pat06};
sprite_pattern e6beamer_pat00 = {1, {{-8, -16, 0, 497}}};
sprite_pattern e6beamer_pat01 = {1, {{-8, -16, 0, 498}}};
sprite_pattern *egg6beamer_pat[2] = {&e6beamer_pat00, &e6beamer_pat01};
sprite_pattern e6door_pat00 = {1, {{-40, -42, 0, 499}}};
sprite_pattern e6door_pat01 = {1, {{-8, -74, 0, 500}}};
sprite_pattern *egg6door_pat[2] = {&e6door_pat00, &e6door_pat01};
Uint8 egg6gareki_pchg00[4] = {8, 0, 1, 255};
Uint8 *egg6gareki_pchg[1] = {egg6gareki_pchg00};
sprite_pattern e6gareki_pat00 = {1, {{-8, -8, 0, 501}}};
sprite_pattern e6gareki_pat01 = {1, {{-8, -8, 0, 502}}};
sprite_pattern *egg6gareki_pat[2] = {&e6gareki_pat00, &e6gareki_pat01};
sprite_pattern e6gareki4_pat00 = {1, {{-8, -8, 0, 503}}};
sprite_pattern e6gareki4_pat01 = {1, {{-8, -8, 0, 504}}};
sprite_pattern e6gareki4_pat02 = {1, {{-8, -8, 0, 505}}};
sprite_pattern e6gareki4_pat03 = {1, {{-8, -8, 0, 506}}};
sprite_pattern *egg6gareki4_pat[4] = {&e6gareki4_pat00, &e6gareki4_pat01,
                                      &e6gareki4_pat02, &e6gareki4_pat03};
sprite_pattern e6yuka_pat00 = {2, {{-16, -16, 0, 507}, {-16, 0, 0, 508}}};
sprite_pattern *egg6yuka_pat[1] = {&e6yuka_pat00};
Uint8 egg6beam_pchg00[18] = {1, 0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 1, 2, 3, 4, 5};
Uint8 *egg6beam_pchg[1] = {egg6beam_pchg00};
sprite_pattern e6beam_pat00 = {1, {{-8, -8, 0, 509}}};
sprite_pattern e6beam_pat01 = {1, {{-8, -8, 0, 510}}};
sprite_pattern e6beam_pat02 = {1, {{-8, -8, 0, 511}}};
sprite_pattern e6beam_pat03 = {1, {{-8, -8, 0, 512}}};
sprite_pattern e6beam_pat04 = {1, {{-8, -8, 0, 513}}};
sprite_pattern *egg6beam_pat[5] = {&e6beam_pat00, &e6beam_pat01, &e6beam_pat02,
                                   &e6beam_pat03, &e6beam_pat04};
sprite_pattern e6beam2_pat00 = {1, {{-8, -8, 0, 514}}};
sprite_pattern e6beam2_pat01 = {1, {{-8, -8, 0, 515}}};
sprite_pattern e6beam2_pat02 = {1, {{-8, -8, 0, 516}}};
sprite_pattern e6beam2_pat03 = {1, {{-8, -8, 0, 517}}};
sprite_pattern e6beam2_pat04 = {1, {{-8, -8, 0, 518}}};
sprite_pattern *egg6beam2_pat[5] = {&e6beam2_pat00, &e6beam2_pat01,
                                    &e6beam2_pat02, &e6beam2_pat03,
                                    &e6beam2_pat04};
sprite_pattern e6beam3_pat00 = {1, {{-8, -8, 0, 519}}};
sprite_pattern e6beam3_pat01 = {1, {{-8, -8, 0, 520}}};
sprite_pattern e6beam3_pat02 = {1, {{-8, -8, 0, 521}}};
sprite_pattern e6beam3_pat03 = {1, {{-8, -8, 0, 522}}};
sprite_pattern e6beam3_pat04 = {1, {{-8, -8, 0, 523}}};
sprite_pattern *egg6beam3_pat[5] = {&e6beam3_pat00, &e6beam3_pat01,
                                    &e6beam3_pat02, &e6beam3_pat03,
                                    &e6beam3_pat04};
