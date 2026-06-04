#include "../equ.h"
#include "kama.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../fcol.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../suicide.h"

typedef struct {
    Sint16 gravity;
    Sint16 timer;
    union {
        Uint8 copied_userflag;
        Uint8 parent_flags;
    };
    Uint16 parent_index;
    Sint16 origin_x;
    Sint16 display_timer;
} kama_work;

static kama_work *kama_get_work(sprite_status *pActwk) {
    return (kama_work *)pActwk->actfree;
}

#if defined(R31A) || defined(R31B) || defined(R32A)
#define SPRITE_KAMA_BASE 465
#elif defined(R33C) || defined(R33D)
#define SPRITE_KAMA_BASE 469
#else
#define SPRITE_KAMA_BASE 418
#endif

static Uint8 jmp_flg;
static Uint8 bCarry;
void (*kama_tbl[7])(sprite_status *) = {&kama_init, &kama_fall, &kama_wait,
                                        &kama_atck, &kama_pati, &kama_move,
                                        &kama_reve};
void (*wpkama_tbl[3])(sprite_status *) = {&wpkama_init, &wpkama_wait,
                                          &wpkama_move};
Uint8 kama_pchg0[6] = {7, 0, 1, 2, 3, 255};
Uint8 kama_pchg1[4] = {7, 5, 6, 255};
Uint8 kama_pchg2[4] = {7, 0, 4, 255};
Uint8 kama_pchg3[4] = {7, 7, 4, 255};
Uint8 kama_pchg4[3] = {7, 0, 255};
Uint8 *kama_pchg[5] = {kama_pchg0, kama_pchg1, kama_pchg2, kama_pchg3,
                       kama_pchg4};
sprite_pattern kama_patA = {3,
                            {{-18, -20, 0, SPRITE_KAMA_BASE + 6},
                             {-12, -4, 0, SPRITE_KAMA_BASE + 7},
                             {-2, 4, 0, SPRITE_KAMA_BASE + 8}}};
sprite_pattern kama_patB = {3,
                            {{-18, -19, 0, SPRITE_KAMA_BASE + 9},
                             {-12, -3, 0, SPRITE_KAMA_BASE + 10},
                             {-2, 4, 0, SPRITE_KAMA_BASE + 11}}};
sprite_pattern kama_patC = {3,
                            {{-18, -20, 0, SPRITE_KAMA_BASE + 12},
                             {-12, -4, 0, SPRITE_KAMA_BASE + 13},
                             {-2, 4, 0, SPRITE_KAMA_BASE + 14}}};
sprite_pattern kama_patD = {3,
                            {{-18, -19, 0, SPRITE_KAMA_BASE + 15},
                             {-12, -3, 0, SPRITE_KAMA_BASE + 16},
                             {-2, 4, 0, SPRITE_KAMA_BASE + 17}}};
sprite_pattern kama_patE = {3,
                            {{-18, -19, 0, SPRITE_KAMA_BASE + 18},
                             {-12, -3, 0, SPRITE_KAMA_BASE + 19},
                             {-2, 4, 0, SPRITE_KAMA_BASE + 20}}};
sprite_pattern kama_patF = {4,
                            {{-18, -19, 0, SPRITE_KAMA_BASE + 21},
                             {-24, -12, 0, SPRITE_KAMA_BASE + 22},
                             {-12, -4, 0, SPRITE_KAMA_BASE + 23},
                             {-2, 4, 0, SPRITE_KAMA_BASE + 24}}};
sprite_pattern kama_patG = {4,
                            {{-18, -19, 0, SPRITE_KAMA_BASE + 25},
                             {-24, -11, 0, SPRITE_KAMA_BASE + 26},
                             {-12, -3, 0, SPRITE_KAMA_BASE + 27},
                             {-2, 4, 0, SPRITE_KAMA_BASE + 28}}};
sprite_pattern kama_patH = {3,
                            {{-18, -20, 0, SPRITE_KAMA_BASE + 29},
                             {-12, -4, 0, SPRITE_KAMA_BASE + 30},
                             {-2, 4, 0, SPRITE_KAMA_BASE + 31}}};
sprite_pattern *kamapat[8] = {&kama_patA, &kama_patB, &kama_patC, &kama_patD,
                              &kama_patE, &kama_patF, &kama_patG, &kama_patH};
sprite_pattern bkama_patA = {3,
                             {{-18, -20, 0, SPRITE_KAMA_BASE + 32},
                              {-12, -4, 0, SPRITE_KAMA_BASE + 33},
                              {-2, 4, 0, SPRITE_KAMA_BASE + 34}}};
sprite_pattern bkama_patB = {3,
                             {{-18, -19, 0, SPRITE_KAMA_BASE + 35},
                              {-12, -3, 0, SPRITE_KAMA_BASE + 36},
                              {-2, 4, 0, SPRITE_KAMA_BASE + 37}}};
sprite_pattern bkama_patC = {3,
                             {{-18, -20, 0, SPRITE_KAMA_BASE + 38},
                              {-12, -4, 0, SPRITE_KAMA_BASE + 39},
                              {-2, 4, 0, SPRITE_KAMA_BASE + 40}}};
sprite_pattern bkama_patD = {3,
                             {{-18, -19, 0, SPRITE_KAMA_BASE + 41},
                              {-12, -3, 0, SPRITE_KAMA_BASE + 42},
                              {-2, 4, 0, SPRITE_KAMA_BASE + 43}}};
sprite_pattern bkama_patE = {3,
                             {{-18, -19, 0, SPRITE_KAMA_BASE + 44},
                              {-12, -3, 0, SPRITE_KAMA_BASE + 45},
                              {-2, 4, 0, SPRITE_KAMA_BASE + 46}}};
sprite_pattern bkama_patF = {3,
                             {{-18, -20, 0, SPRITE_KAMA_BASE + 47},
                              {-12, -4, 0, SPRITE_KAMA_BASE + 48},
                              {-2, 4, 0, SPRITE_KAMA_BASE + 49}}};
sprite_pattern bkama_patG = {3,
                             {{-18, -19, 0, SPRITE_KAMA_BASE + 50},
                              {-12, -3, 0, SPRITE_KAMA_BASE + 51},
                              {-2, 4, 0, SPRITE_KAMA_BASE + 52}}};
sprite_pattern bkama_patH = {3,
                             {{-18, -20, 0, SPRITE_KAMA_BASE + 53},
                              {-12, -4, 0, SPRITE_KAMA_BASE + 54},
                              {-2, 4, 0, SPRITE_KAMA_BASE + 55}}};
sprite_pattern *bkamapat[8] = {&bkama_patA, &bkama_patB, &bkama_patC,
                               &bkama_patD, &bkama_patE, &bkama_patF,
                               &bkama_patG, &bkama_patH};
Uint8 wpkama_pchg0[8] = {3, 0, 1, 2, 3, 4, 5, 255};
Uint8 wpkama_pchg1[8] = {0, 0, 1, 2, 3, 4, 5, 255};
Uint8 *wpkama_pchg[2] = {wpkama_pchg0, wpkama_pchg1};
sprite_pattern bu0 = {1, {{-8, -8, 0, SPRITE_KAMA_BASE}}};
sprite_pattern bu1 = {1, {{-8, -8, 0, SPRITE_KAMA_BASE + 1}}};
sprite_pattern bu2 = {1, {{-8, -8, 0, SPRITE_KAMA_BASE + 2}}};
sprite_pattern bu3 = {1, {{-8, -8, 0, SPRITE_KAMA_BASE + 3}}};
sprite_pattern bu4 = {1, {{-8, -8, 0, SPRITE_KAMA_BASE + 4}}};
sprite_pattern bu5 = {1, {{-8, -8, 0, SPRITE_KAMA_BASE + 5}}};
sprite_pattern *wpkamapat[6] = {&bu0, &bu1, &bu2, &bu3, &bu4, &bu5};

void ene_kama(sprite_status *pActwk) {
    Uint8 byRwk;

    if (pActwk->xposi.w.h == 4800 && pActwk->yposi.w.h == 890) {

        jmp_flg = 0;
    }
    if (enemy_suicide(pActwk) == -1)
        return;

    byRwk = pActwk->r_no0 / 2;
    kama_tbl[byRwk](pActwk);

    frameout_s(pActwk);
}

void kama_init(sprite_status *pActwk) {
    kama_work *work = kama_get_work(pActwk);

    work->copied_userflag = pActwk->userflag.b.h;
    if ((char)work->copied_userflag >= 0) {
        pActwk->patno = 1;
        pActwk->patbase = kamapat;
    } else {

        pActwk->patno = 2;
        pActwk->patbase = bkamapat;
    }

    pActwk->actflg = 4;
    pActwk->sprpri = 3;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 20;
    pActwk->sproffset = 9202;
    pActwk->yspeed.w = 256;
    pActwk->colino = 6;
    pActwk->r_no0 += 2;

    kama_fall(pActwk);
}

void kama_disp(sprite_status *pActwk) {
    patchg(pActwk, kama_pchg);
    actionsub(pActwk);
    jmp_flg = 1;
}

void kama_fall(sprite_status *pActwk) {
    Sint16 iD1;

    movement(pActwk);
    iD1 = emycol_d(pActwk);
    if (iD1 < 0)
        pActwk->r_no0 += 2;

    kama_disp(pActwk);
}

void kama_wait(sprite_status *pActwk) {
    Sint16 iD0, iD1;
    sprite_status *pPlaywk;

    if (pActwk->actflg & 128) {
        pActwk->actflg &= 254;
        pActwk->cddat &= 254;

        pPlaywk = &actwk[0];
        iD0 = pActwk->xposi.w.h;
        bCarry = CCset(iD0, pPlaywk->xposi.w.h);
        iD0 -= pPlaywk->xposi.w.h;
        if (bCarry == 1) {
            iD0 = -iD0;
            pActwk->actflg |= 1;
            pActwk->cddat |= 1;
        }

        if ((char)pActwk->actflg < 0) {
            iD1 = pActwk->yposi.w.h;
            bCarry = CCset(iD1, pPlaywk->yposi.w.h);
            iD1 -= pPlaywk->yposi.w.h;
            if (bCarry == 1)
                iD1 = -iD1;

            if (iD1 < 96) {

                pActwk->mstno.b.h = 2;
                set_wpkama(pActwk);
                kama_get_work(pActwk)->timer = 120;
                pActwk->r_no0 += 2;
                kama_atck(pActwk);
                return;
            }
        }
    }

    pActwk->mstno.b.h = 1;
    kama_disp(pActwk);
}

void kama_atck(sprite_status *pActwk) {
    --kama_get_work(pActwk)->timer;
    if (kama_get_work(pActwk)->timer != 0)
        kama_disp(pActwk);
    else {

        pActwk->mstno.b.h = 3;
        kama_get_work(pActwk)->timer = 60;
        pActwk->r_no0 += 2;
        kama_pati(pActwk);
    }
}

void kama_pati(sprite_status *pActwk) {
    --kama_get_work(pActwk)->timer;
    if (kama_get_work(pActwk)->timer != 0)
        kama_disp(pActwk);
    else {

        pActwk->r_no0 += 2;
        kama_move(pActwk);
    }
}

void kama_move(sprite_status *pActwk) {
    Sint16 iD0, iD1;

    if (pActwk->userflag.b.h >= 0)
        iD0 = 256;
    else
        iD0 = 128;

    pActwk->mstno.b.h = 0;
    if (!(pActwk->cddat & 1))
        iD0 = -iD0;

    pActwk->xspeed.w = iD0;

    movement(pActwk);

    iD1 = dircol_l2(pActwk);
    if (iD1 >= 0) {
        iD1 = dircol_r2(pActwk);
        if (iD1 >= 0) {
            iD1 = emycol_d(pActwk);
            if (iD1 == 0) {
                kama_disp(pActwk);
                return;
            }
            if (iD1 < 7 && iD1 > -7) {
                pActwk->yposi.w.h += iD1;
                kama_disp(pActwk);
                return;
            }
        }
    }

    kama_get_work(pActwk)->timer = 72;
    pActwk->mstno.b.h = 3;
    pActwk->r_no0 += 2;
    kama_reve(pActwk);
}

void kama_reve(sprite_status *pActwk) {
    --kama_get_work(pActwk)->timer;
    if (kama_get_work(pActwk)->timer == 0) {
        pActwk->actflg = bchg(0, pActwk->actflg);
        pActwk->cddat = bchg(0, pActwk->cddat);
        pActwk->r_no0 -= 2;
    }
    kama_disp(pActwk);
}

void movement(sprite_status *pActwk) {
    Sint32 lD0 = 0;

    lD0 = pActwk->xspeed.w;
    if (lD0 & 0x80000000)
        lD0 <<= 8, lD0 |= 0x80000000;
    else
        lD0 <<= 8;
    pActwk->xposi.l += lD0;

    lD0 = pActwk->yspeed.w;
    if (lD0 & 0x80000000)
        lD0 <<= 8, lD0 |= 0x80000000;
    else
        lD0 <<= 8;
    pActwk->yposi.l += lD0;
}

void set_wpkama(sprite_status *pActwk) {
    sprite_status *pActfree;

    if (pActwk->userflag.b.h >= 0) {

        if (actwkchk(&pActfree) == 0) {
            pActfree->actno = 37;

            kama_get_work(pActfree)->parent_index = pActwk - actwk;

            kama_get_work(pActfree)->timer = 64;
            pActfree->userflag.b.h = pActwk->userflag.b.h;

            kama_get_work(pActfree)->parent_flags = pActwk->actflg;

            pActfree->yposi.w.h = pActwk->yposi.w.h - 4;
            if (!(pActwk->actflg & 1))
                pActfree->xposi.w.h = pActwk->xposi.w.h - 17;
            else
                pActfree->xposi.w.h = pActwk->xposi.w.h + 17;
        }

        if (actwkchk(&pActfree) == 0) {
            pActfree->actno = 37;

            kama_get_work(pActfree)->parent_index = pActwk - actwk;

            kama_get_work(pActfree)->timer = 20;
            pActfree->userflag.b.h = pActwk->userflag.b.h;

            kama_get_work(pActfree)->parent_flags = pActwk->actflg;

            pActfree->yposi.w.h = pActwk->yposi.w.h - 6;
            if (!(pActwk->actflg & 1))
                pActfree->xposi.w.h = pActwk->xposi.w.h - 9;
            else
                pActfree->xposi.w.h = pActwk->xposi.w.h + 9;
        }
    }
}

void tama_kama(sprite_status *pActwk) {
    jmp_flg = 0;
    wpkama_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s00(pActwk, kama_get_work(pActwk)->origin_x);
    jmp_flg = 0;
}

void wpkama_init(sprite_status *pActwk) {
    Sint16 iD1;

    pActwk->actflg = 4;
    pActwk->sprpri = 1;
    pActwk->sprhs = 4;
    pActwk->sprvsize = 4;
    pActwk->sproffset = 9202;
    pActwk->patbase = wpkamapat;
    kama_get_work(pActwk)->origin_x = pActwk->xposi.w.h;

    iD1 = 768;

    if (!(kama_get_work(pActwk)->parent_flags & 1)) {
        pActwk->actflg |= 1;
        pActwk->cddat |= 1;
        iD1 = -iD1;
    }

    pActwk->xspeed.w = iD1;
    pActwk->r_no0 += 2;
}

void wpkama_wait(sprite_status *pActwk) {
    Uint16 wD0;
    sprite_status *pActfree;

    --kama_get_work(pActwk)->timer;
    if (kama_get_work(pActwk)->timer == 0) {
        pActwk->r_no0 += 2;
        pActwk->colino = 135;
        kama_get_work(pActwk)->display_timer = 60;
    }

    wD0 = kama_get_work(pActwk)->parent_index;
    pActfree = &actwk[wD0];
    if (pActfree->actno != 36)
        frameout(pActwk);
    else

        patchg(pActwk, wpkama_pchg);
    jmp_flg = 1;
}

void wpkama_move(sprite_status *pActwk) {
    sprite_status *pPlaywk;

    movement(pActwk);
    pActwk->yspeed.w += kama_get_work(pActwk)->gravity;

    if (pActwk->patno == 0) {
        if (pActwk->actflg & 128)
            soundset(185);
    }

    pPlaywk = &actwk[0];
    if (wpkama_colicheck(pActwk, pPlaywk) != 0)
        wpkama_reve(pActwk, pPlaywk);
    wpkama_disp(pActwk);
}

void wpkama_disp(sprite_status *pActwk) {
    --kama_get_work(pActwk)->display_timer;
    if (kama_get_work(pActwk)->display_timer == 0) {
        ++kama_get_work(pActwk)->display_timer;
        pActwk->colino = 0;
    }

    patchg(pActwk, wpkama_pchg);
    jmp_flg = 1;
}

Sint16 wpkama_colicheck(sprite_status *pActwk, sprite_status *pPlaywk) {
    Uint8 byFlg;
    Sint16 iD0, iD1;

    if (plpower_m == 0) {
        if (!(pPlaywk->cddat & 4))
            return 0;
    }

    iD0 = pPlaywk->xposi.w.h - pActwk->xposi.w.h;
    iD1 = 28;
    iD0 += iD1;
    if (iD0 < 0)
        return 0;
    iD1 += iD1;
    byFlg = CCset(iD0, iD1);
    if (byFlg == 0)
        return 0;

    iD0 = pPlaywk->yposi.w.h - pActwk->yposi.w.h;
    iD1 = 28;
    iD0 += iD1;
    if (iD0 < 0)
        return 0;
    iD1 += iD1;
    byFlg = CCset(iD0, iD1);
    if (byFlg == 0)
        return 0;

    return 1;
}

void wpkama_reve(sprite_status *pActwk, sprite_status *pPlaywk) {
    pActwk->colino = 0;
    pActwk->mstno.b.h = 1;

    pActwk->xspeed.w += pPlaywk->xspeed.w;
    pActwk->yspeed.w = -2048;
    kama_get_work(pActwk)->gravity = 64;
}
