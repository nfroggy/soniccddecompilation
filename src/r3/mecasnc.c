#include "../equ.h"
#include "mecasnc.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../fcol.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../player_work.h"
#include "../ridechk.h"
#include "coli3.h"

typedef struct {
    Uint8 wait_timer;
    Sint16 base_y;
    Uint16 ride_actor_index;
    Uint8 hover_angle;
    Uint8 lift_timer;
    Uint8 ready_flag;
    Sint16 acceleration;
} mecasnc3_work;

typedef struct {
    Uint16 ride_actor_index;
    Uint8 heart_timer;
    Uint8 initialized_flag;
} emie3_work;

typedef struct {
    Uint8 timer;
    Uint8 piece_index;
    Sint16 gravity;
} hari3x_work;

typedef struct {
    Uint8 timer;
    Uint8 no_wave;
} heart3_work;

typedef struct {
    Uint16 parent_index;
} msnc3fire_work;

static inline mecasnc3_work *mecasnc3_work_get(sprite_status *pActwk) {
    return (mecasnc3_work *)pActwk->actfree;
}

static inline emie3_work *emie3_work_get(sprite_status *pActwk) {
    return (emie3_work *)pActwk->actfree;
}

static inline hari3x_work *hari3x_work_get(sprite_status *pActwk) {
    return (hari3x_work *)pActwk->actfree;
}

static inline heart3_work *heart3_work_get(sprite_status *pActwk) {
    return (heart3_work *)pActwk->actfree;
}

static inline msnc3fire_work *msnc3fire_work_get(sprite_status *pActwk) {
    return (msnc3fire_work *)pActwk->actfree;
}

static Uint8 jmp_flg;
static Uint8 bCarry;
void (*msnc_tbl[6])(sprite_status *) = {&mecasnc3_init, &mecasnc3_matu1,
                                        &mecasnc3_come, &mecasnc3_hovr,
                                        &mecasnc3_upmv, &mecasnc3_rght};
extern sprite_pattern *mecasnc3pat[];
void (*emie3_tbl[3])(sprite_status *) = {&emie3_init, &emie3_walk, &emie3_help};
extern PALETTEENTRY zone3colora[];
extern Uint8 *em_pchg[];
extern sprite_pattern *emie3pat[];
static PALETTEENTRY emie3_clr[16] = {
    {0, 0, 0, 1},       {0, 0, 0, 1},       {128, 32, 96, 1},
    {160, 64, 128, 1},  {224, 96, 224, 1},  {224, 160, 224, 1},
    {224, 224, 224, 1}, {160, 160, 160, 1}, {128, 128, 128, 1},
    {64, 64, 64, 1},    {224, 160, 128, 1}, {192, 96, 0, 1},
    {32, 192, 0, 1},    {0, 128, 0, 1},     {96, 0, 128, 1},
    {224, 0, 0, 1}};
void (*hari_tbl[3])(sprite_status *) = {&hari3x_init, &hari3x_move,
                                        &hari3x_brkn};
extern sprite_pattern *hari3xpat[];
Sint16 spd_tbl[8] = {512, -512, -256, -1024, -256, -512, 512, -1024};
void (*heart_tbl[2])(sprite_status *) = {&heart3_init, &heart3_move};
void (*msfire_tbl[2])(sprite_status *) = {&msnc3fire_init, &msnc3fire_move};
Uint8 vatck_pchg[5] = {0, 0, 1, 0, 255};
Uint8 elec_pchg[9] = {6, 1, 7, 1, 8, 1, 9, 1, 255};
Uint8 hover_pchg[3] = {14, 1, 255};
Uint8 *msc_pchg[3] = {vatck_pchg, elec_pchg, hover_pchg};
Uint8 fire_pchg0[6] = {1, 12, 15, 13, 15, 255};
Uint8 *fire_pchg[1] = {fire_pchg0};

Uint8 CSset_b(Uint8 bySrc, Uint8 byDst) {
    Uint16 wk, wSrc, wDst;

    wSrc = bySrc;
    wDst = byDst;
    wk = wSrc + wDst;
    if (wk >= 255)
        return 1;
    return 0;
}

void mecasnc3(sprite_status *pActwk) {
    sprite_status *pPlaywk;

    jmp_flg = 0;
    msnc_tbl[pActwk->r_no0 / 2](pActwk);

    pPlaywk = &actwk[0];
    if (pPlaywk->xposi.w.h >= 1216)
        mecasnc3_fout(pActwk);
    else
        actionsub(pActwk);
}

void mecasnc3_init(sprite_status *pActwk) {
    mecasnc3_work *work = mecasnc3_work_get(pActwk);

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 976;
    pActwk->sprpri = 2;
    pActwk->patbase = mecasnc3pat;
    pActwk->patno = 14;
    pActwk->sprvsize = 32;
    work->hover_angle = 60;
    pActwk->colino = 0;
    mecasnc3_matu1(pActwk);
}

void mecasnc3_matu1(sprite_status *pActwk) {
    mecasnc3_work *work = mecasnc3_work_get(pActwk);

    if (work->hover_angle != 0) {

        --work->hover_angle;
        return;
    }

    dir_left(pActwk);
    work->acceleration = -16;
    pActwk->r_no0 += 2;

    if (pActwk->actflg & 128)
        soundset(202);
}

void mecasnc3_come(sprite_status *pActwk) {
    Sint16 iD0, iD1;
    mecasnc3_work *work = mecasnc3_work_get(pActwk);

    emie_speedsetx(pActwk);
    iD0 = work->acceleration;
    iD0 += pActwk->xspeed.w;
    if (iD0 <= -768)
        iD0 = -768;

    pActwk->xspeed.w = iD0;

    iD0 = 992;
    iD1 = work->ride_actor_index;
    if (iD1 != 0)
        iD0 = actwk[iD1].xposi.w.h + 32;

    if (iD0 >= pActwk->xposi.w.h) {

        pActwk->xspeed.w = 0;
        work->acceleration = 0;
        work->ready_flag = 255;
        pActwk->mstno.b.h = 2;
        setfire(pActwk);
        pActwk->colino = 0;
        work->base_y = pActwk->yposi.w.h;
        pActwk->r_no0 += 2;
    }

    empatchg(pActwk, msc_pchg);
}

void mecasnc3_hovr(sprite_status *pActwk) {
    sprite_status *pPlaywk;
    char wWk;
    mecasnc3_work *work = mecasnc3_work_get(pActwk);

    pPlaywk = &actwk[0];
    setdirect(pActwk, pPlaywk);
    yposisetsub(pActwk);

    work->hover_angle += 4;
    wWk = (Sint8)work->hover_angle;
    if (wWk < 0) {

        pActwk->r_no0 += 2;
        pActwk->yspeed.w = -704;
        work->acceleration = 11;
        work->lift_timer = 64;
        work->wait_timer = 80;
    }

    empatchg(pActwk, msc_pchg);
}

void yposisetsub(sprite_status *pActwk) {
    mecasnc3_work *work = mecasnc3_work_get(pActwk);
    Uint8 byR = work->hover_angle;
    Sint16 iSin, iCos;
    int_union lD0;

    sinset(byR, &iSin, &iCos);
    lD0.l = iSin << 2;
    if (lD0.l & 0x80000000)
        lD0.l <<= 8, lD0.l |= 0x80000000;
    else
        lD0.l <<= 8;

    lD0.w.h += work->base_y;
    pActwk->yposi.l = lD0.l;
}

void mecasnc3_upmv(sprite_status *pActwk) {
    sprite_status *pPlaywk;
    short_union iD0;
    mecasnc3_work *work = mecasnc3_work_get(pActwk);

    pPlaywk = &actwk[0];
    setdirect(pActwk, pPlaywk);
    if (work->lift_timer != 0) {

        emie_speedset(pActwk);
        pActwk->yspeed.w += work->acceleration;

        if (--work->lift_timer == 0) {

            work->acceleration = 0;
            pActwk->yspeed.w = 0;
            work->base_y = pActwk->yposi.w.h;
        } else {

            empatchg(pActwk, msc_pchg);
            return;
        }
    }

    yposisetsub(pActwk);
    work->hover_angle += 4;

    iD0.w = pActwk->xposi.w.h;
    bCarry = CCset(iD0.w, pPlaywk->xposi.w.h);
    iD0.w -= pPlaywk->xposi.w.h;
    if (bCarry == 0) {

        if ((Uint8)iD0.b.l >= 160) {

            empatchg(pActwk, msc_pchg);
            return;
        }
    }

    if (--work->wait_timer == 0) {

        dir_right(pActwk);
        pActwk->xspeed.w = 0;
        work->acceleration = 96;
        pActwk->r_no0 += 2;
    }

    empatchg(pActwk, msc_pchg);
}

void setfire(sprite_status *pActwk) {
    sprite_status *pActfree;

    if (actwkchk(&pActfree) == 0) {

        pActfree->actno = 52;
        msnc3fire_work_get(pActfree)->parent_index = pActwk - actwk;
    }
}

void mecasnc3_rght(sprite_status *pActwk) {
    Sint16 iD0;
    mecasnc3_work *work = mecasnc3_work_get(pActwk);

    emie_speedsetx(pActwk);
    iD0 = work->acceleration;
    iD0 += pActwk->xspeed.w;
    if (iD0 >= 1024)
        iD0 = 1024;

    pActwk->xspeed.w = iD0;
    if (pActwk->xposi.w.h < 1320)
        empatchg(pActwk, msc_pchg);
    else

        mecasnc3_fout(pActwk);
}

void mecasnc3_fout(sprite_status *pActwk) {

    emie3end = 255;
    emie3clrsetx(zone3colora);
    frameout(pActwk);
    jmp_flg = 1;
}

void emie_speedset(sprite_status *pActwk) {
    emie_speedsetx(pActwk);
    emie_speedsety(pActwk);
}

void emie_speedsety(sprite_status *pActwk) {
    int_union lD0;

    lD0.l = pActwk->yspeed.w;
    if (lD0.l & 0x80000000)
        lD0.l <<= 8, lD0.l |= 0x80000000;
    else
        lD0.l <<= 8;
    pActwk->yposi.l += lD0.l;
}

void emie_speedsetx(sprite_status *pActwk) {
    int_union lD0;

    lD0.l = pActwk->xspeed.w;
    if (lD0.l & 0x80000000)
        lD0.l <<= 8, lD0.l |= 0x80000000;
    else
        lD0.l <<= 8;
    pActwk->xposi.l += lD0.l;
}

void emie3(sprite_status *pActwk) {
    sprite_status *pPlaywk;

    jmp_flg = 0;
    if (ta_flag == 0 && emie3end == 0) {

        emie3_tbl[pActwk->r_no0 / 2](pActwk);
        pPlaywk = &actwk[0];
        if (pPlaywk->xposi.w.h >= 1216)
            mecasnc3_fout(pActwk);
        else
            actionsub(pActwk);
    } else {

        mecasnc3_fout(pActwk);
    }
    jmp_flg = 0;
}

void emie3_init(sprite_status *pActwk) {
    sprite_status *pPlaywk;
    emie3_work *work = emie3_work_get(pActwk);

    if (work->initialized_flag == 0) {

        work->initialized_flag = 255;
    }

    pActwk->actflg |= 4;
    pActwk->sproffset = 9054;
    pActwk->sprpri = 1;
    pActwk->patbase = emie3pat;
    emie3clrset();
    pPlaywk = &actwk[0];
    setdirect(pActwk, pPlaywk);
    if (pPlaywk->xposi.w.h >= pActwk->xposi.w.h)
        pActwk->r_no0 += 2;

    empatchg(pActwk, em_pchg);
}

void emiewalkret(sprite_status *pActwk) {
    empatchg(pActwk, em_pchg);
    heartset(pActwk);
}

void emie3_walk(sprite_status *pActwk) {
    Sint16 iD0, iD1, iD2;
    sprite_status *pPlaywk, *pRideAct;
    emie3_work *work = emie3_work_get(pActwk);

    pPlaywk = &actwk[0];
    iD0 = work->ride_actor_index;
    if (iD0 != 0) {

        pRideAct = &actwk[iD0];
        if (mecasnc3_work_get(pRideAct)->ready_flag != 0) {

            pActwk->r_no0 = 4;
            sub_sync(125);
            emiewalkret(pActwk);
            return;
        }
    }

    setdirect(pActwk, pPlaywk);
    if (pActwk->cddat & 1) {

        if (pActwk->xposi.w.h < 128) {

            pActwk->mstno.b.h = 0;
            pActwk->xspeed.w = 0;
            emiewalkret(pActwk);
            return;
        }
    }

    if (pActwk->xposi.w.h < 960) {

        iD0 = -32;
        if (!(pActwk->cddat & 1))
            iD0 = -iD0;

        iD0 += pActwk->xspeed.w;
        iD1 = iD0;
        iD2 = 512;
        if (iD1 < 0) {

            iD1 = -iD1;
            iD2 = -iD2;
        }

        if (iD1 >= 512)
            iD0 = iD2;

        pActwk->xspeed.w = iD0;
        emie_speedsetx(pActwk);
        pActwk->mstno.b.h = 1;
        if (pActwk->xposi.w.h < 960) {

            emiewalkret(pActwk);
            return;
        }
    }

    pActwk->mstno.b.h = 0;

    if (work->ride_actor_index == 0) {

        if (actwkchk(&pPlaywk) == 0) {

            pPlaywk->actno = 49;
            pPlaywk->xposi.w.h = 1280;
            pPlaywk->yposi.w.h = 1000;

            mecasnc3_work_get(pPlaywk)->ride_actor_index = pActwk - actwk;

            work->ride_actor_index = pPlaywk - actwk;
        }
    }
    emiewalkret(pActwk);
}

void emie3_help(sprite_status *pActwk) {
    Sint16 iD0;
    sprite_status *pRideAct;
    emie3_work *work = emie3_work_get(pActwk);

    iD0 = work->ride_actor_index;
    pRideAct = &actwk[iD0];
    if (pRideAct->actno == 49) {

        iD0 = 8;
        dir_left(pActwk);
        if (pRideAct->cddat & 1) {

            iD0 = -iD0;
            dir_right(pActwk);
        }

        iD0 += pRideAct->xposi.w.h;
        pActwk->xposi.w.h = iD0;
        pActwk->yposi.w.h = pRideAct->yposi.w.h + 4;
        pActwk->mstno.b.h = 2;
        empatchg(pActwk, em_pchg);
        return;
    }

    frameout(pActwk);
    jmp_flg = 1;
}

void setdirect(sprite_status *pActwk, sprite_status *pPlaywk) {
    dir_right(pActwk);
    if (pActwk->xposi.l >= pPlaywk->xposi.l)
        dir_left(pActwk);
}

void dir_right(sprite_status *pActwk) {
    pActwk->cddat &= 254;
    pActwk->actflg &= 254;
}

void dir_left(sprite_status *pActwk) {
    pActwk->cddat |= 1;
    pActwk->actflg |= 1;
}

void empatchg(sprite_status *pActwk, Uint8 **bppPtbl) {
    Uint8 byD0;
    Uint8 byD1;
    Uint8 byPwk;
    Uint8 *bpPchg;

    byD0 = pActwk->mstno.b.h;
    if (byD0 != pActwk->mstno.b.l) {

        pActwk->mstno.b.l = (Uint8)byD0;
        pActwk->patcnt = 0;
        pActwk->pattim = 0;
    }

    --pActwk->pattim;
    if ((char)pActwk->pattim >= 0)
        return;

    bpPchg = bppPtbl[byD0];

    do {

        byD0 = pActwk->patcnt;
        byPwk = byD0;
        byD0 = bpPchg[byPwk];
        if (byD0 != 255)
            break;
        pActwk->patcnt = 0;
    } while (1);

    byD1 = byD0;
    byD0 &= 31;
    pActwk->patno = byD0;
    byD0 = pActwk->cddat;
    byD1 >>= 5;
    byD1 ^= byD0;
    byD1 &= 3;
    pActwk->actflg &= 252;
    pActwk->actflg |= byD1;
    pActwk->pattim = bpPchg[byPwk + 1];

    if (pActwk->pattim < 2)
        ++pActwk->pattim;

    pActwk->patcnt += 2;
}

void emie3clrsetx(PALETTEENTRY *pPalet) {
    PALETTEENTRY *pColorwk;
    Sint16 i;

    pColorwk = &lpcolorwk[16];

    for (i = 0; i < 16; ++i) {
        *pColorwk = *pPalet;
        ++pColorwk;
        ++pPalet;
    }
}

void emie3clrset(void) { emie3clrsetx(emie3_clr); }

void heartset(sprite_status *pActwk) {
    Uint8 bywk;
    Sint16 iD1, wk;
    sprite_status *pActfree;
    emie3_work *work = emie3_work_get(pActwk);

    bywk = work->heart_timer;
    wk = (Sint16)bywk + 8;
    work->heart_timer += 8;
    if (wk <= 255)
        return;

    if (actwkchk(&pActfree) != 0)
        return;
    pActfree->actno = 51;
    if (!(pActwk->cddat & 1))
        iD1 = 12;
    else
        iD1 = -14;
    pActfree->xposi.w.h = pActwk->xposi.w.h + iD1;
    pActfree->yposi.w.h = pActwk->yposi.w.h - 12;
}

void hari3x_fout(sprite_status *pActwk) {
    frameout(pActwk);
    jmp_flg = 1;
}

void hari3x(sprite_status *pActwk) {
    jmp_flg = 0;
    if (ta_flag == 0) {

        hari_tbl[pActwk->r_no0 / 2](pActwk);
        if (jmp_flg == 0)
            actionsub(pActwk);
    } else
        hari3x_fout(pActwk);
    jmp_flg = 0;
}

void hari3x_ridechk(sprite_status *pActwk) {
    sprite_status *pPlaywk;
    Sint32 lD0, lD3;

    if (pActwk->userflag.b.h != 1) {

        hitchk(pActwk, &actwk[0]);
        return;
    }

    if (hitchk(pActwk, &actwk[0]) == 0)
        return;

    if (!(pActwk->cddat & 8))
        return;

    if (plpower_a != 0)
        return;
    if (plpower_m != 0)
        return;

    pPlaywk = &actwk[0];
    if (pPlaywk->r_no0 < 4) {

        if (player_work_get(pPlaywk)->damage_invulnerability_timer == 0) {

            lD3 = pPlaywk->yposi.l;
            lD0 = pPlaywk->yspeed.w;
            if (lD0 & 0x80000000)
                lD0 <<= 8, lD0 |= 0x80000000;
            else
                lD0 <<= 8;
            lD3 -= lD0;
            pPlaywk->yposi.l = lD3;
            playdamageset(pPlaywk, pActwk);
        }
    }
}

void hari3x_init(sprite_status *pActwk) {
    hari3x_work *work = hari3x_work_get(pActwk);

    if (emie3end != 0) {

        hari3x_fout(pActwk);
        return;
    }
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 798;
    pActwk->sprpri = 3;
    pActwk->patbase = hari3xpat;
    pActwk->sprhsize = 18;
    pActwk->sprvsize = 8;

    if (pActwk->userflag.b.h != 1) {

        pActwk->sprvsize = 32;
        if (pActwk->userflag.b.h != 0) {

            if (pActwk->userflag.b.h != 2)
                pActwk->patno = 14;
            else
                pActwk->patno = 9;
            pActwk->colino = 134;
            pActwk->sprhsize = 12;
        }
    }

    if (work->piece_index == 0) {

        hari3x_move(pActwk);
        return;
    }
    pActwk->colino = 0;
    pActwk->r_no0 += 2;
    hari3x_spdset(pActwk);
    hari3x_brkn(pActwk);
}

void hari3x_move(sprite_status *pActwk) {
    Sint16 i, iD0, iD1;
    sprite_status *pActmsnc;

    if (pActwk->userflag.b.h == 1) {

        hari3x_ridechk(pActwk);
        return;
    }

    pActmsnc = &actwk[32];

    for (i = 0; i < 96; ++i, ++pActmsnc) {

        if (pActmsnc->actno == 49)
            break;
    }
    if (pActmsnc->actno != 49) {

        hari3x_ridechk(pActwk);
        return;
    }

    iD1 = (char)pActwk->sprhsize;
    iD1 += 16;
    iD0 = pActmsnc->xposi.w.h - pActwk->xposi.w.h;
    iD0 += iD1;
    if (iD0 < 0) {

        hari3x_ridechk(pActwk);
        return;
    }
    iD1 += iD1;
    if (iD0 >= iD1) {

        hari3x_ridechk(pActwk);
        return;
    }

    iD1 = (char)pActwk->sprvsize;
    iD1 += 16;
    iD0 = pActmsnc->yposi.w.h - pActwk->yposi.w.h;
    iD0 += iD1;
    if (iD0 < 0) {

        hari3x_ridechk(pActwk);
        return;
    }
    iD1 += iD1;
    if (iD0 >= iD1) {

        hari3x_ridechk(pActwk);
        return;
    }
    pActwk->r_no0 += 2;
    brknset(pActwk);
    ride_on_clr(pActwk, &actwk[0]);
}

void brknset(sprite_status *pActwk) {
    Sint16 iD1, iD2;
    sprite_status *pActfree;

    iD1 = 3;
    pActwk->patno = 4;
    iD2 = 0;
    if (pActwk->userflag.b.h != 0) {

        pActwk->patno = 8;
        iD2 = 4;
        if (pActwk->userflag.b.h != 2) {

            pActwk->patno = 13;
            iD2 = 9;
        }
    }

    while (iD1 > 0) {

        if (actwkchk(&pActfree) == 0) {

            pActfree->actno = 48;
            pActfree->xposi.w.h = pActwk->xposi.w.h;
            pActfree->yposi.w.h = pActwk->yposi.w.h;
            pActfree->patno = iD1 + iD2;
            hari3x_work_get(pActfree)->piece_index = iD1;
        }

        --iD1;
    }
    if (pActwk->actflg & 128) {

        soundset(163);
    }
    hari3x_spdset(pActwk);
}

void hari3x_spdset(sprite_status *pActwk) {
    Uint8 byD0;
    hari3x_work *work = hari3x_work_get(pActwk);

    byD0 = work->piece_index;
    byD0 *= 2;
    pActwk->xspeed.w = spd_tbl[byD0];
    pActwk->yspeed.w = spd_tbl[byD0 + 1];
    work->gravity = 96;
    work->timer = 120;
}

void hari3x_brkn(sprite_status *pActwk) {
    Sint16 iD0;
    hari3x_work *work = hari3x_work_get(pActwk);

    emie_speedset(pActwk);
    iD0 = work->gravity;
    pActwk->yspeed.w += iD0;
    --work->timer;
    if (work->timer == 0) {

        frameout(pActwk);
        jmp_flg = 1;
    }
}

void heart3(sprite_status *pActwk) {
    jmp_flg = 0;
    heart_tbl[pActwk->r_no0 / 2](pActwk);
    if (jmp_flg == 0) {

        actionsub(pActwk);
        frameout_s(pActwk);
    }
    jmp_flg = 0;
}

void heart3_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 862;
    pActwk->patbase = emie3pat;
    pActwk->sprpri = 3;
    pActwk->yspeed.w = -96;
    pActwk->patno = 8;
    heart3_move(pActwk);
}

void heart3_move(sprite_status *pActwk) {
    Uint8 byR;
    Sint16 iD0, iD1;
    heart3_work *work = heart3_work_get(pActwk);

    if (work->no_wave == 0) {

        byR = work->timer;
        byR *= 3;
        sinset(byR, &iD0, &iD1);
        iD0 /= 4;
        pActwk->xspeed.w = iD0;
    }

    emie_speedset(pActwk);
    ++work->timer;
    if (work->timer == 20)
        ++pActwk->patno;

    if (work->timer == 110) {

        ++pActwk->patno;
        pActwk->yspeed.w = 0;
        pActwk->xspeed.w = 0;
        work->no_wave = 255;
    }

    if (work->timer == 120) {

        frameout(pActwk);
        jmp_flg = 1;
    }
}

void msnc3fire(sprite_status *pActwk) {
    msfire_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
}

void msnc3fire_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 976;
    pActwk->patbase = mecasnc3pat;
    pActwk->sprpri = 3;
    msnc3fire_move(pActwk);
}

void msnc3fire_move(sprite_status *pActwk) {
    Sint16 iD0;
    sprite_status *pActmsnc;
    msnc3fire_work *work = msnc3fire_work_get(pActwk);

    iD0 = work->parent_index;
    pActmsnc = &actwk[iD0];
    if (pActmsnc->actno != 49) {

        frameout(pActwk);
        jmp_flg = 1;
        return;
    }
    iD0 = pActmsnc->xposi.w.h;
    iD0 -= 16;
    if (pActmsnc->cddat & 1)
        iD0 += 32;

    pActwk->xposi.w.h = iD0;
    pActwk->yposi.w.h = pActmsnc->yposi.w.h;

    patchg(pActwk, fire_pchg);
    jmp_flg = 1;
}
