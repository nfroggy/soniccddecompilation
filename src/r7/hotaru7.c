#include "../equ.h"
#include "hotaru7.h"
#include "../action.h"
#include "../actset.h"
#include "../playsub.h"
#include "../suicide.h"
#include <stddef.h>

#pragma pack(push, 1)
typedef struct {
    union {
        Uint8 *flag_work;
        struct {
            Uint16 parent_index;
            Sint16 origin_x;
        };
        struct {
            Uint16 partner2_index;
            Uint16 partner1_index;
        };
    };
    union {
        Sint16 *position_table;
        Sint32 speed;
    };
    union {
        Uint8 **pattern_change;
        struct {
            Sint16 child2_index;
            Sint16 child1_index;
        };
    };
    Uint8 parent_cdsts;
} hotaru7_work;
#pragma pack(pop)

_Static_assert(sizeof(Uint8 *) == 4, "hotaru7_work pointer fields are 32-bit");
_Static_assert(sizeof(Sint16 *) == 4, "hotaru7_work pointer fields are 32-bit");
_Static_assert(sizeof(Uint8 **) == 4,
               "hotaru7_work pointer fields are 32-bit");
_Static_assert(offsetof(hotaru7_work, flag_work) == 0,
               "hotaru7_work.flag_work offset");
_Static_assert(offsetof(hotaru7_work, parent_index) == 0,
               "hotaru7_work.parent_index offset");
_Static_assert(offsetof(hotaru7_work, origin_x) == 2,
               "hotaru7_work.origin_x offset");
_Static_assert(offsetof(hotaru7_work, partner2_index) == 0,
               "hotaru7_work.partner2_index offset");
_Static_assert(offsetof(hotaru7_work, partner1_index) == 2,
               "hotaru7_work.partner1_index offset");
_Static_assert(offsetof(hotaru7_work, position_table) == 4,
               "hotaru7_work.position_table offset");
_Static_assert(offsetof(hotaru7_work, speed) == 4,
               "hotaru7_work.speed offset");
_Static_assert(offsetof(hotaru7_work, pattern_change) == 8,
               "hotaru7_work.pattern_change offset");
_Static_assert(offsetof(hotaru7_work, child2_index) == 8,
               "hotaru7_work.child2_index offset");
_Static_assert(offsetof(hotaru7_work, child1_index) == 10,
               "hotaru7_work.child1_index offset");
_Static_assert(offsetof(hotaru7_work, parent_cdsts) == 12,
               "hotaru7_work.parent_cdsts offset");
_Static_assert(sizeof(hotaru7_work) <= sizeof(((sprite_status *)0)->actfree),
               "hotaru7_work fits in actfree");

static inline hotaru7_work *hotaru7_work_get(sprite_status *pActwk) {
    return (hotaru7_work *)pActwk->actfree;
}

static void a_init(sprite_status *pActwk);
static void a_wait(sprite_status *pActwk);
static void a_make(sprite_status *pActwk);
static void ini_b(sprite_status *pActwk, sprite_status *pNewact);
static void a_move(sprite_status *pActwk);
static void b_init(sprite_status *pActwk);
static void b_fly(sprite_status *pActwk);
static void b_near(sprite_status *pActwk);
static void b_fix(sprite_status *pActwk);
static void b_fix1(sprite_status *pActwk);
static void b_start(sprite_status *pActwk);
static void b_com(sprite_status *pActwk);
static void b_com1(sprite_status *pActwk);
static void b_kill_v(sprite_status *pActwk);
static void b_die_v(sprite_status *pActwk, sprite_status *pMainwk);
static void b_light(sprite_status *pActwk);
static void b_up(sprite_status *pActwk);
static void b_up1(sprite_status *pActwk);
static void b_make_c(sprite_status *pActwk);
static void c_init(sprite_status *pActwk);
static void c_move(sprite_status *pActwk);
static Uint32 area(sprite_status *pActwk, sprite_status *pPlayerwk, Uint16 Ypos,
                   Uint16 Ylen, Uint16 Xpos, Uint16 Xlen);

static Sint16 tbl_hotaru7[8] = {36, 60, 8, -68, 56, -56, 80, 16};
static Uint8 pchg_e00[12] = {9, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 252};
static Uint8 pchg_e01[14] = {3, 4, 2, 2, 2, 4, 4, 2, 2, 4, 4, 4, 2, 252};
static Uint8 pchg_e02[12] = {9, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 252};
static Uint8 pchg_e03[4] = {9, 3, 4, 255};
static Uint8 *pchg_hotaru7_e[4] = {pchg_e00, pchg_e01, pchg_e02, pchg_e03};
static Uint8 pchg_b00[12] = {19, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 252};
static Uint8 pchg_b01[14] = {9, 4, 2, 2, 2, 4, 4, 2, 2, 4, 4, 4, 2, 252};
static Uint8 pchg_b02[12] = {19, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 252};
static Uint8 pchg_b03[4] = {19, 3, 4, 255};
static Uint8 *pchg_hotaru7_b[4] = {pchg_b00, pchg_b01, pchg_b02, pchg_b03};
static Uint8 pchg_tama00[5] = {1, 0, 1, 2, 255};
static Uint8 *pchg_tama[1] = {pchg_tama00};
static sprite_pattern pat_e00 = {1, {{-20, -12, 32, 508}}};
static sprite_pattern pat_e01 = {1, {{-12, -20, 32, 509}}};
static sprite_pattern pat_e02 = {1, {{-12, -20, 32, 510}}};
static sprite_pattern pat_e03 = {2, {{-12, -20, 32, 511}, {-11, -7, 32, 521}}};
static sprite_pattern pat_e04 = {2, {{-12, -20, 32, 512}, {-11, -7, 32, 521}}};
sprite_pattern *pat_hotaru7_e[5] = {&pat_e00, &pat_e01, &pat_e02, &pat_e03,
                                    &pat_e04};
static sprite_pattern pat_b00 = {1, {{-20, -12, 32, 513}}};
static sprite_pattern pat_b01 = {1, {{-12, -20, 32, 514}}};
static sprite_pattern pat_b02 = {1, {{-12, -20, 32, 515}}};
static sprite_pattern pat_b03 = {2, {{-12, -20, 32, 516}, {-11, -7, 32, 521}}};
static sprite_pattern pat_b04 = {2, {{-12, -20, 32, 517}, {-11, -7, 32, 521}}};
sprite_pattern *pat_hotaru7_b[5] = {&pat_b00, &pat_b01, &pat_b02, &pat_b03,
                                    &pat_b04};
static sprite_pattern pat_tama00 = {1, {{-12, -12, 0, 518}}};
static sprite_pattern pat_tama01 = {1, {{-12, -12, 0, 519}}};
static sprite_pattern pat_tama02 = {1, {{-12, -12, 0, 520}}};
sprite_pattern *pat_tama[3] = {&pat_tama00, &pat_tama01, &pat_tama02};
static void (*a_act_tbl[4])(sprite_status *) = {&a_init, &a_wait, &a_make,
                                                &a_move};
static void (*b_act_tbl[11])(sprite_status *) = {
    &b_init, &b_fly,   &b_near, &b_fix, &b_fix1, &b_start,
    &b_com,  &b_light, &b_com,  &b_up,  &b_up1};
static void (*c_act_tbl[3])(sprite_status *) = {&c_init, &c_move, &frameout};

void hotaru7(sprite_status *pActwk) {
    hotaru7_work *work = hotaru7_work_get(pActwk);

    switch (pActwk->userflag.b.l) {
        sprite_status *pMainact;
        sprite_status *pBact1;
        sprite_status *pBact2;

    case -1:
        pMainact = &actwk[work->parent_index];
        if (pMainact->actno != 35 || pMainact->cdsts != work->parent_cdsts)

            frameout(pActwk);
        else {

            if (!enemy_suicide(pActwk)) {
                b_act_tbl[pActwk->r_no0 / 2](pActwk);
            }
        }
        break;

    case -2:
        pBact1 = &actwk[work->partner1_index];
        pBact2 = &actwk[work->partner2_index];

        if (pBact1->actno != 35 || pBact2->actno != 35 || pBact1->r_no0 != 16 ||
            pBact2->r_no0 != 16)
            frameout(pActwk);
        else {

            c_act_tbl[pActwk->r_no0 / 2](pActwk);
        }
        break;

    default:
        a_act_tbl[pActwk->r_no0 / 2](pActwk);
        break;
    }
}

void a_init(sprite_status *pActwk) {
    Uint16 flagwk;
    Uint8 *pFlagwork;
    Sint32 idx;
    hotaru7_work *work = hotaru7_work_get(pActwk);

    pActwk->r_no0 += 2;
    flagwk = pActwk->cdsts;
    idx = time_flag + flagwk * 3;
    pFlagwork = &flagwork[idx];
    work->flag_work = &flagwork[idx];

    idx = pActwk->userflag.b.h & 12;
    work->position_table = &tbl_hotaru7[idx / 2];

    if (pActwk->userflag.b.h & 1) {
        pActwk->userflag.b.h = 1;
    } else {
        pActwk->userflag.b.h = 0;
    }

    if (stageno.b.l != 2) {
        pActwk->sproffset = 33885;
    } else {
        pActwk->sproffset = 33568;
    }

    a_wait(pActwk);
}

void a_wait(sprite_status *pActwk) {
    if (area(pActwk, &actwk[0], 65296, 480, 65296, 480) != 0) {

        pActwk->r_no0 += 2;
    }

    frameout_s(pActwk);
}

void a_make(sprite_status *pActwk) {
    Uint8 *pFlagWork;
    sprite_status *pNewact;
    Sint16 *pTbl;
    hotaru7_work *work = hotaru7_work_get(pActwk);

    pActwk->r_no0 += 2;
    pFlagWork = work->flag_work;

    if (!(*pFlagWork & 1)) {
        if (actwkchk2(pActwk, &pNewact) != 0) {
            frameout_s0(pActwk);
            return;
        }

        work->child1_index = pNewact - actwk;

        ini_b(pActwk, pNewact);
        pTbl = work->position_table;
        pNewact->xposi.w.h += *pTbl++;
        pNewact->yposi.w.h += *pTbl;
    }

    if (!(*pFlagWork & 2)) {
        if (actwkchk2(pActwk, &pNewact) != 0) {
            frameout_s0(pActwk);
            return;
        }

        work->child2_index = pNewact - actwk;

        ini_b(pActwk, pNewact);
        pTbl = work->position_table;
        pNewact->xposi.w.h -= *pTbl++;
        pNewact->yposi.w.h -= *pTbl;
    }
}

void ini_b(sprite_status *pActwk, sprite_status *pNewact) {
    hotaru7_work *new_work = hotaru7_work_get(pNewact);

    new_work->parent_index = pActwk - actwk;
    pNewact->actno = pActwk->actno;
    pNewact->userflag.b.h = pActwk->userflag.b.h;
    pNewact->userflag.b.l = -1;
    pNewact->xposi.w.h = pActwk->xposi.w.h;
    pNewact->yposi.w.h = pActwk->yposi.w.h;
    pNewact->sproffset = pActwk->sproffset;
    new_work->parent_cdsts = pActwk->cdsts;
}

void a_move(sprite_status *pActwk) {
    Uint8 *pFlagWork;
    sprite_status *pChildact;
    Sint16 idx;
    hotaru7_work *work = hotaru7_work_get(pActwk);

    pFlagWork = work->flag_work;

    if (!(*pFlagWork & 1)) {
        idx = work->child1_index;
        if (idx) {
            pChildact = &actwk[idx];
            if (pChildact->actno != pActwk->actno) {

                *pFlagWork |= 1;
                work->child1_index = 0;
            }
        }
    }

    if (!(*pFlagWork & 2)) {
        idx = work->child2_index;
        if (idx) {
            pChildact = &actwk[idx];
            if (pChildact->actno != pActwk->actno) {

                *pFlagWork |= 2;
                work->child2_index = 0;
            }
        }
    }

    if (*pFlagWork == 131)
        frameout(pActwk);
    else {
        if ((work->child2_index | work->child1_index) == 0)
            frameout_s0(pActwk);
    }
}

void b_init(sprite_status *pActwk) {
    hotaru7_work *work = hotaru7_work_get(pActwk);

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 1;
    pActwk->sprhs = pActwk->sprhsize = 20;
    pActwk->sprvsize = 12;
    pActwk->colino = 50;
    work->origin_x = pActwk->xposi.w.h;

    if (!pActwk->userflag.b.h) {

        pActwk->patbase = pat_hotaru7_e;
        work->pattern_change = pchg_hotaru7_e;
        work->speed = -65536;
    } else {
        pActwk->patbase = pat_hotaru7_b;
        work->pattern_change = pchg_hotaru7_b;
        work->speed = -32768;
    }

    if (actwk[0].xposi.w.h >= pActwk->xposi.w.h) {
        work->speed *= -1;
        pActwk->actflg ^= 1;
        pActwk->cddat ^= 1;
    }
}

void b_fly(sprite_status *pActwk) {
    Sint16 xPos, idx;
    sprite_status *pMainwk, *pSubwk;
    hotaru7_work *work = hotaru7_work_get(pActwk);

    pActwk->xposi.l += work->speed;

    xPos = pActwk->xposi.w.h - work->origin_x;
    if (xPos < 0) {
        xPos *= -1;
    }

    if ((Sint32)xPos >= 80) {
        pActwk->xposi.l -= work->speed;
        work->speed *= -1;
        pActwk->actflg ^= 1;
        pActwk->cddat ^= 1;
    }

    if (area(pActwk, &actwk[0], 65480, 112, 65456, 160) != 0) {

        pMainwk = &actwk[work->parent_index];

        idx = hotaru7_work_get(pMainwk)->child1_index;
        if (idx) {
            pSubwk = &actwk[idx];
            pSubwk->r_no0 = 4;
        }

        idx = hotaru7_work_get(pMainwk)->child2_index;
        if (idx) {
            pSubwk = &actwk[idx];
            pSubwk->r_no0 = 4;
        }
    }

    b_com1(pActwk);
}

void b_near(sprite_status *pActwk) {
    sprite_status *pMainwk, *pSubwk1, *pSubwk2;
    Sint16 idx, xwk, ywk;
    Uint32 flag1, flag2;
    Sint32 speedwk;
    hotaru7_work *work = hotaru7_work_get(pActwk);
    hotaru7_work *main_work;

    idx = work->parent_index;
    pMainwk = &actwk[idx];
    main_work = hotaru7_work_get(pMainwk);

    if ((idx = main_work->child1_index) != 0) {
        pSubwk1 = &actwk[idx];

        if ((idx = main_work->child2_index) != 0) {
            pSubwk2 = &actwk[idx];
            flag1 = 0;

            xwk = (pSubwk1->xposi.w.h + pSubwk2->xposi.w.h >> 1) -
                  scra_h_posit.w.h;

            if (xwk < 152) {

                pSubwk1->actflg |= 1;
                pSubwk2->actflg |= 1;
                pSubwk1->cddat |= 1;
                pSubwk2->cddat |= 1;

                if ((speedwk = work->speed) < 0) {
                    speedwk = -speedwk;
                }

                pSubwk1->xposi.l += speedwk;
                pSubwk2->xposi.l += speedwk;
            } else if (xwk > 168) {

                pSubwk1->actflg &= 254;
                pSubwk2->actflg &= 254;
                pSubwk1->cddat &= 254;
                pSubwk2->cddat &= 254;

                if ((speedwk = work->speed) < 0) {
                    speedwk = -speedwk;
                }

                pSubwk1->xposi.l -= speedwk;
                pSubwk2->xposi.l -= speedwk;
            } else {
                flag1 = 1;
            }

            flag2 = 0;

            ywk = (pSubwk1->yposi.w.h + pSubwk2->yposi.w.h >> 1) -
                  scra_v_posit.w.h;

            if (ywk < 104) {

                if ((speedwk = work->speed) < 0) {
                    speedwk = -speedwk;
                }

                pSubwk1->yposi.l += speedwk;
                pSubwk2->yposi.l += speedwk;
            } else if (ywk > 120) {

                if ((speedwk = work->speed) < 0) {
                    speedwk = -speedwk;
                }

                pSubwk1->yposi.l -= speedwk;
                pSubwk2->yposi.l -= speedwk;
            } else {
                flag2 = 1;
            }

            if (flag1 != 0 && flag2 != 0) {
                pSubwk1->r_no0 = pSubwk2->r_no0 = 6;
            }

            b_com1(pActwk);
            return;
        }
    }

    pActwk->r_no0 += 2;
    b_fix(pActwk);
}

void b_fix(sprite_status *pActwk) {
    Sint16 hwk, vwk;

    pActwk->r_no0 += 2;
    pActwk->sprhs = pActwk->sprhsize = 12;
    pActwk->sprvsize = 20;
    pActwk->colino = 51;

    pActwk->actflg = 1;
    hwk = pActwk->xposi.w.h - scra_h_posit.w.h + 128;
    vwk = pActwk->yposi.w.h - scra_v_posit.w.h + 128;

    pActwk->xposi.w.h = hwk;
    pActwk->yposi.w.h = vwk;

    b_fix1(pActwk);
}

void b_fix1(sprite_status *pActwk) {
    patchg(pActwk, hotaru7_work_get(pActwk)->pattern_change);
    actionsub(pActwk);
}

void b_start(sprite_status *pActwk) {
    Sint16 xwk, ywk;

    pActwk->r_no0 += 2;
    pActwk->mstno.w = 511;

    pActwk->actflg = 4;

    xwk = pActwk->xposi.w.h - 128 + scra_h_posit.w.h;
    ywk = pActwk->yposi.w.h - 128 + scra_v_posit.w.h;

    pActwk->xposi.w.h = xwk;
    pActwk->yposi.w.h = ywk;

    b_com(pActwk);
}

void b_com(sprite_status *pActwk) {
    patchg(pActwk, hotaru7_work_get(pActwk)->pattern_change);
    b_com1(pActwk);
}

void b_com1(sprite_status *pActwk) {
    sprite_status *pMainwk;

    actionsub(pActwk);
    pMainwk = &actwk[hotaru7_work_get(pActwk)->parent_index];
    frameout_s(pActwk);

    if (!pActwk->actno) {
        b_die_v(pActwk, pMainwk);
    }
}

void b_kill_v(sprite_status *pActwk) {
    sprite_status *pMainwk;

    pMainwk = &actwk[hotaru7_work_get(pActwk)->parent_index];
    frameout(pActwk);
    b_die_v(pActwk, pMainwk);
}

void b_die_v(sprite_status *pActwk, sprite_status *pMainwk) {
    Sint16 idx;
    hotaru7_work *main_work = hotaru7_work_get(pMainwk);

    idx = main_work->child1_index;
    if (idx == (Sint16)(pActwk - actwk)) {
        main_work->child1_index = 0;
    } else {
        main_work->child2_index = 0;
    }
}

void b_light(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->mstno.w = 767;
    if (!pActwk->userflag.b.h) {
        b_make_c(pActwk);
    }

    b_com(pActwk);
}

void b_up(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->mstno.w = 1023;
    b_com(pActwk);
}

void b_up1(sprite_status *pActwk) {
    pActwk->yposi.w.h -= 2;
    if (!(pActwk->actflg & 128)) {
        if ((actwk[0].yposi.w.h - pActwk->yposi.w.h) > 160) {
            b_kill_v(pActwk);
            return;
        }
    }

    b_com(pActwk);
}

void b_make_c(sprite_status *pActwk) {
    Sint16 idx1, idx2, xPos, xWk, yPos, yWk;
    sprite_status *pMainwk, *pSubwk1, *pSubwk2, *pNewact;
    Sint32 i;
    hotaru7_work *main_work;

    pMainwk = &actwk[hotaru7_work_get(pActwk)->parent_index];
    main_work = hotaru7_work_get(pMainwk);
    idx1 = main_work->child1_index;
    idx2 = main_work->child2_index;
    if (!idx1 || !idx2) {

        return;
    }

    pSubwk1 = &actwk[idx1];
    pSubwk2 = &actwk[idx2];
    if (pActwk == pSubwk1) {

        if (!(pSubwk1->actflg & 128) || !(pSubwk2->actflg & 128)) {

            return;
        }

        xPos = pSubwk1->xposi.w.h;
        yPos = pSubwk1->yposi.w.h;
        xWk = pSubwk2->xposi.w.h;
        yWk = pSubwk2->yposi.w.h;
        xWk = xWk - xPos >> 2;
        yWk = yWk - yPos >> 2;
        yPos += 7;
        for (i = 0; i < 3; ++i) {
            if (actwkchk(&pNewact) != 0)
                return;

            hotaru7_work_get(pNewact)->partner1_index = pSubwk1 - actwk;

            hotaru7_work_get(pNewact)->partner2_index = pSubwk2 - actwk;

            pNewact->actno = pSubwk1->actno;
            pNewact->userflag.b.h = pSubwk1->userflag.b.h;
            pNewact->userflag.b.l = -2;
            pNewact->sproffset = pSubwk1->sproffset;
            xPos += xWk;
            yPos += yWk;
            pNewact->xposi.w.h = xPos;
            pNewact->yposi.w.h = yPos;
        }
    }
}

void c_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 2;
    pActwk->sprhs = pActwk->sprhsize = 12;
    pActwk->sprvsize = 12;
    pActwk->colino = 180;
    pActwk->patbase = pat_tama;
    c_move(pActwk);
}

void c_move(sprite_status *pActwk) {
    patchg(pActwk, pchg_tama);
    actionsub(pActwk);
}

Uint32 area(sprite_status *pActwk, sprite_status *pPlayerwk, Uint16 Ypos,
            Uint16 Ylen, Uint16 Xpos, Uint16 Xlen) {
    Uint16 len;

    len = (Uint16)pPlayerwk->yposi.w.h - (Uint16)pActwk->yposi.w.h - Ypos;
    if (len >= Ylen) {
        return 0;
    }

    len = (Uint16)pPlayerwk->xposi.w.h - (Uint16)pActwk->xposi.w.h - Xpos;
    if (len >= Xlen) {
        return 0;
    }

    return 1;
}
