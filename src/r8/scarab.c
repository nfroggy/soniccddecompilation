#include "../equ.h"
#include "scarab.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../ridechk.h"
#include "../suicide.h"

static void c_init(sprite_status *pActwk);
static void c_wait(sprite_status *pActwk);
static void c_make(sprite_status *pActwk);
static void ini_e_i(sprite_status *pActwk, sprite_status *pNewact);
static void c_move(sprite_status *pActwk);
static void enemy(sprite_status *pActwk);
static void e_init(sprite_status *pActwk);
static void e_move(sprite_status *pActwk);
static void turn(sprite_status *pActwk);
static void e1_check(sprite_status *pActwk);
static void e1_chk_patchg(sprite_status *pActwk);
static void e1_catch(sprite_status *pActwk);
static void e1_keep(sprite_status *pActwk);
static void e1_wait(sprite_status *pActwk);
static void iteminit(sprite_status *pActwk);
static void itemmove(sprite_status *pActwk);
static void itemmove2(sprite_status *pActwk);
static void itemmove3(sprite_status *pActwk);
static void itemget(sprite_status *pActwk);
static void ride_on_chk_i(sprite_status *pActwk, sprite_status *pPlayerwk);

static Uint8 pchg_n00[4] = {9, 0, 1, 255};
static Uint8 *pchg_n[1] = {pchg_n00};
static Uint8 pchg_o00[4] = {19, 0, 1, 255};
static Uint8 *pchg_o[1] = {pchg_o00};
static sprite_pattern pat00 = {1, {{-32, -12, 0, 402}}};
static sprite_pattern pat01 = {1, {{-32, -12, 0, 403}}};
static sprite_pattern pat02 = {1, {{-40, -12, 0, 404}}};
static sprite_pattern pat03 = {1, {{-24, -12, 0, 405}}};
static sprite_pattern pat04 = {1, {{-32, -12, 0, 406}}};
static sprite_pattern pat05 = {1, {{-32, -12, 0, 407}}};
sprite_pattern *pat_scarab[6] = {&pat00, &pat01, &pat02,
                                 &pat03, &pat04, &pat05};
static void (*c_act_tbl[3])(sprite_status *) = {&c_init, &c_wait, &c_move};
static void (*item_move_tbl[5])(sprite_status *) = {
    &iteminit, &itemmove, &itemget, &itemmove2, &itemmove3};
static void (*e_act_tbl[2])(sprite_status *) = {&e_init, &e_move};
static void (*e_move_tbl[3])(sprite_status *) = {&e1_check, &e1_keep, &e1_wait};
extern sprite_pattern *itempat[];
extern Uint8 *itemchg[];

void scarab(sprite_status *pActwk) {
    if ((Uint8)pActwk->userflag.b.h == 2) {

        if (actwk[((Sint16 *)pActwk)[23]].actno != 47) {

            frameout(pActwk);
            return;
        }

        item_move_tbl[pActwk->r_no0 / 2](pActwk);
    } else if ((Uint8)pActwk->userflag.b.h < 2) {

        enemy(pActwk);
    } else {

        c_act_tbl[pActwk->r_no0 / 2](pActwk);
    }
}

static void c_init(sprite_status *pActwk) {
    Uint8 flgwk;

    pActwk->r_no0 += 2;
    flgwk = pActwk->cdsts;
    flgwk = (time_flag & 127) + flgwk * 3;
    *(Uint8 **)&pActwk->actfree[0] = &flagwork[flgwk];

    c_wait(pActwk);
}

static void c_wait(sprite_status *pActwk) {
    Uint16 xwk, ywk;

    xwk = actwk[0].xposi.w.h - pActwk->xposi.w.h + 320;
    ywk = actwk[0].yposi.w.h - pActwk->yposi.w.h + 320;

    if (ywk < 640 && xwk < 640) {
        c_make(pActwk);
    } else {
        frameout_s(pActwk);
    }
}

static void c_make(sprite_status *pActwk) {
    Uint8 *pFlagWork, flg;
    sprite_status *pNewact;

    pActwk->r_no0 += 2;
    pFlagWork = *(Uint8 **)&pActwk->actfree[0];

    if (!(*pFlagWork & 1)) {
        if (actwkchk2(pActwk, &pNewact) != 0) {
            frameout_s0(pActwk);
            return;
        }

        ini_e_i(pActwk, pNewact);
        ((Sint16 *)pActwk)[25] = pNewact - actwk;
        pNewact->userflag.b.h = (Uint8)(pActwk->userflag.b.h & 1);

        if (pActwk->userflag.b.h & 16) {
            pNewact->userflag.b.l = -1;
        } else {
            pNewact->userflag.b.l = 0;
        }
    }

    if (!(*pFlagWork & 2)) {
        flg = pActwk->userflag.b.l;

        if ((flg & 128) || flg > 7) {

            *pFlagWork |= 2;
        } else {

            if (actwkchk2(pActwk, &pNewact) != 0) {
                frameout_s0(pActwk);
                return;
            }

            ini_e_i(pActwk, pNewact);
            ((Sint16 *)pActwk)[26] = pNewact - actwk;
            pNewact->userflag.b.h = 2;
            pNewact->userflag.b.l = flg;
            pNewact->yposi.w.h -= 4;

            if (pActwk->userflag.b.h & 16) {
                pNewact->xposi.w.h += 24;
            } else {
                pNewact->xposi.w.h -= 24;
            }
        }
    }
}

static void ini_e_i(sprite_status *pActwk, sprite_status *pNewact) {
    ((Sint16 *)pNewact)[23] = pActwk - actwk;
    pNewact->actno = 47;
    pNewact->xposi.w.h = pActwk->xposi.w.h;
    pNewact->yposi.w.h = pActwk->yposi.w.h;

    ((Sint16 *)pNewact)[27] = -1;
}

static void c_move(sprite_status *pActwk) {
    Uint8 *pFlagWork;
    Sint16 idx;
    sprite_status *pChildAct;
    ushort_union flagwk;

    pFlagWork = *(Uint8 **)&pActwk->actfree[0];

    if (!(*pFlagWork & 1)) {
        if ((idx = ((Sint16 *)pActwk)[25]) != 0) {
            pChildAct = &actwk[idx];

            if (pChildAct->actno != 47) {

                *pFlagWork |= 1;
                ((Sint16 *)pActwk)[25] = 0;

                if ((idx = ((Sint16 *)pActwk)[26]) != 0) {
                    pChildAct = &actwk[idx];
                    pChildAct->xspeed.w = 0;
                }

                if ((idx = ((Sint16 *)pChildAct)[27]) >= 0) {
                    pChildAct = &actwk[idx];
                    pChildAct->actfree[2] &= 254;
                }
            }
        }
    }

    if (!(*pFlagWork & 2)) {
        if ((idx = ((Sint16 *)pActwk)[26]) != 0) {
            pChildAct = &actwk[idx];

            if (pChildAct->actno != 47) {

                *pFlagWork |= 2;
                ((Sint16 *)pActwk)[26] = 0;
            }
        }
    }

    if (*pFlagWork & 1) {
        flagwk.b.h = 255;
    } else {
        flagwk.b.h = 0;
    }

    if (*pFlagWork & 2) {
        flagwk.b.l = 255;
    } else {
        flagwk.b.l = 0;
    }

    if (flagwk.w == 65535) {
        frameout(pActwk);
        return;
    }

    flagwk.w == 0;

    if (!(((Sint16 *)pActwk)[25] | (((Sint16 *)pActwk)[26]))) {
        frameout_s0(pActwk);
    }
}

static void enemy(sprite_status *pActwk) {
    sprite_status *pCtrlact;

    if (pActwk->userflag.b.l == 1) {

        if (actwk[((Sint16 *)pActwk)[23]].actno != 47) {

            frameout(pActwk);
            return;
        }

        if (pActwk->r_no0) {
            actionsub(pActwk);
        } else {
            pActwk->r_no0 += 2;
            pActwk->sprpri = 3;
        }
    } else {
        pCtrlact = &actwk[((Sint16 *)pActwk)[23]];

        if (pCtrlact->actno != 47) {
            frameout(pActwk);
            return;
        }

        if (!enemy_suicide(pActwk)) {

            e_act_tbl[pActwk->r_no0 / 2](pActwk);
            actionsub(pActwk);
            frameout_s(pActwk);

            if (!pActwk->actno) {
                ((Sint16 *)pCtrlact)[25] = 0;
            }
        }
    }
}

static void e_init(sprite_status *pActwk) {
    sprite_status *pNewact;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprhs = pActwk->sprhsize = 32;
    pActwk->sprvsize = 11;
    pActwk->colino = 240;
    pActwk->sprpri = 1;
    pActwk->sproffset = 9279;
    pActwk->patbase = pat_scarab;
    ((Sint16 *)pActwk)[24] = pActwk->xposi.w.h;
    ((Sint16 *)pActwk)[28] = 60;
    pActwk->yposi.w.h += emycol_d(pActwk);

    if (actwkchk2(pActwk, &pNewact) != 0) {
        frameout_s0(pActwk);
        return;
    }

    pNewact->actno = 47;
    pNewact->userflag.b.l = 1;
    pNewact->xposi.w.h = pActwk->xposi.w.h;
    pNewact->yposi.w.h = pActwk->yposi.w.h;
    pNewact->actflg = pActwk->actflg;
    pNewact->sprhs = pActwk->sprhs;
    pNewact->sprhsize = pActwk->sprhsize;
    pNewact->sprvsize = pActwk->sprvsize;
    pNewact->sproffset = pActwk->sproffset;
    pNewact->patbase = pActwk->patbase;
    ((Sint16 *)pActwk)[32] = pNewact - actwk;
    ((Sint16 *)pNewact)[23] = pActwk - actwk;

    if (pActwk->userflag.b.l) {

        pActwk->actflg |= 1;
        pNewact->actflg |= 1;
        pActwk->cddat |= 1;
        pNewact->cddat |= 1;
    }

    if (!pActwk->userflag.b.h) {

        ((Uint8 ***)pActwk)[15] = pchg_n;
        *(Sint32 *)&pActwk->actfree[4] = 24576;
    } else {
        ((Uint8 ***)pActwk)[15] = pchg_o;
        *(Sint32 *)&pActwk->actfree[4] = 12288;
    }

    e_move(pActwk);
}

static void e_move(sprite_status *pActwk) {
    Sint16 xSav, ySav, wk;
    sprite_status *pSubact;

    ySav = pActwk->yposi.w.h;
    xSav = pActwk->xposi.w.h;

    if (((Sint16 *)pActwk)[28] >= 0) {
        pActwk->xposi.l += *(Sint32 *)&pActwk->actfree[4];

        if ((wk = pActwk->xposi.w.h - ((Sint16 *)pActwk)[24]) < 0) {
            wk *= -1;
        }

        if (wk >= 64) {
            turn(pActwk);
        }

        wk = emycol_d(pActwk);
        if (wk + 7 >= 14) {

            turn(pActwk);
        } else {
            pActwk->yposi.w.h += wk;
        }
    }

    xSav -= pActwk->xposi.w.h;
    ySav -= pActwk->yposi.w.h;

    pSubact = &actwk[((Sint16 *)pActwk)[23]];
    if ((wk = ((Sint16 *)pSubact)[26]) != 0) {
        pSubact = &actwk[wk];
        pSubact->xposi.w.h -= xSav;
        pSubact->yposi.w.h -= ySav;
        pSubact->xspeed.w = -xSav << 8;
    }

    if ((wk = ((Sint16 *)pActwk)[27]) >= 0) {
        pSubact = &actwk[wk];
        pSubact->xposi.w.h -= xSav;
        pSubact->yposi.w.h -= ySav;
    }

    e_move_tbl[pActwk->r_no1 / 2](pActwk);
    pSubact = &actwk[((Sint16 *)pActwk)[32]];
    pSubact->xposi.w.h = pActwk->xposi.w.h;
    pSubact->yposi.w.h = pActwk->yposi.w.h;
    pSubact->patno = pActwk->patno + 3;
}

static void turn(sprite_status *pActwk) {
    pActwk->xposi.l -= *(Sint32 *)&pActwk->actfree[4];
    *(Sint32 *)&pActwk->actfree[4] *= -1;
}

static void e1_check(sprite_status *pActwk) {
    if (!pActwk->colicnt)
        goto label1;
    if (actwk[0].r_no0 == 4 || actwk[0].r_no0 == 6 ||
        ((Sint16 *)&actwk[0])[26] ||
        ((Sint16 *)&actwk[((Sint16 *)pActwk)[23]])[26]) {
        pActwk->colicnt = 0;
        goto label1;
    }
    e1_catch(pActwk);
    return;

label1:
    e1_chk_patchg(pActwk);
}

static void e1_chk_patchg(sprite_status *pActwk) {
    if (--((Sint16 *)pActwk)[28] >= 0) {

        patchg(pActwk, ((Uint8 ***)pActwk)[15]);
    } else if (((Sint16 *)pActwk)[28] <= -30) {
        ((Sint16 *)pActwk)[28] = 60;
    }
}

static void e1_catch(sprite_status *pActwk) {
    ((Sint16 *)pActwk)[27] = &actwk[0] - actwk;
    actwk[0].actfree[2] |= 1;
    actwk[0].cddat |= 4;
    actwk[0].mstno.b.h = 2;
    actwk[0].sprvsize = 14;
    actwk[0].sprhs = 7;
    actwk[0].yposi.w.h = pActwk->yposi.w.h - 3;

    if (!pActwk->userflag.b.l) {

        actwk[0].xposi.w.h = pActwk->xposi.w.h - 24;
    } else {
        actwk[0].xposi.w.h = pActwk->xposi.w.h + 24;
    }

    pActwk->r_no1 += 2;
    ((Sint16 *)pActwk)[29] = 120;
    pActwk->colino = 240, pActwk->colicnt = 0;
}

static void e1_keep(sprite_status *pActwk) {
    sprite_status *pPlayerwk;

    if (--((Sint16 *)pActwk)[29] != 0) {
        pActwk->colino = 240, pActwk->colicnt = 0;
        e1_chk_patchg(pActwk);
    } else {
        pPlayerwk = &actwk[((Sint16 *)pActwk)[27]];

        ((Sint16 *)pActwk)[27] = -1;
        pPlayerwk->actfree[2] &= 254;
        pPlayerwk->yspeed.w = 0;

        if (!pActwk->userflag.b.h) {

            pPlayerwk->xspeed.w = -2560;
        } else {
            pPlayerwk->xspeed.w = -1280;
        }

        if (!pActwk->userflag.b.l) {

            pPlayerwk->cddat |= 1;
        } else {
            pPlayerwk->xspeed.w *= -1;
            pPlayerwk->cddat &= 254;
        }

        ((Sint16 *)pPlayerwk)[33] = 15;
        pPlayerwk->mspeed.w = pPlayerwk->xspeed.w;
        pPlayerwk->direc.b.h = 0;
        pPlayerwk->cddat &= 223;

        pActwk->r_no1 += 2;
        pActwk->colino = pActwk->colicnt = 0;
        pActwk->patno = 2;
        ((Sint16 *)pActwk)[29] = 30;
    }
}

static void e1_wait(sprite_status *pActwk) {
    if (!(--((Sint16 *)pActwk)[29])) {

        pActwk->r_no1 = 0;
        pActwk->mstno.b.l = -1;
        pActwk->colino = 240, pActwk->colicnt = 0;
    }
}

static void iteminit(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->sprvsize = 14;
    pActwk->sprhs = 14;
    pActwk->patbase = itempat;
    pActwk->sproffset = 1448;
    pActwk->actflg = 4;
    pActwk->sprpri = 2;
    pActwk->sprhsize = 15;
    pActwk->colino = 198;
    pActwk->mstno.b.h = pActwk->userflag.b.l;
    pActwk->yposi.w.h += emycol_d(pActwk);

    itemmove(pActwk);
}

static void itemmove(sprite_status *pActwk) {
    Sint16 col;

    if (!(pActwk->actflg & 128)) {
        itemmove3(pActwk);
    } else {
        if (pActwk->r_no1) {
            speedset(pActwk);

            if ((col = emycol_d(pActwk)) < 0) {
                pActwk->yposi.w.h += col;
                pActwk->yspeed.w = 0;
                pActwk->r_no1 = 0;
            }
        } else {

            ride_on_chk_i(pActwk, &actwk[0]);
        }

        itemmove2(pActwk);
    }
}

static void itemmove2(sprite_status *pActwk) {
    if (!time_stop) {
        patchg(pActwk, itemchg);
    }

    itemmove3(pActwk);
}

static void itemmove3(sprite_status *pActwk) {
    sprite_status *pCtrlact;

    actionsub(pActwk);
    pCtrlact = &actwk[((Sint16 *)pActwk)[23]];
    frameout_s(pActwk);

    if (!pActwk->actno) {
        ((Sint16 *)pCtrlact)[26] = 0;
    }
}

static void itemget(sprite_status *pActwk) {
    sprite_status *pCtrlact, *pNewact;
    Sint16 idx;

    pCtrlact = &actwk[((Sint16 *)pActwk)[23]];

    if ((idx = ((Sint16 *)pCtrlact)[25]) != 0) {
        actwk[idx].colicnt = 1;
    }

    ride_on_chk_i(pActwk, &actwk[0]);
    ride_on_clr(pActwk, &actwk[0]);

    soundset(150);
    pActwk->r_no0 += 4;
    pActwk->colino = 0;

    if (actwkchk(&pNewact) == 0) {
        pNewact->actno = 26;
        pNewact->xposi.w.h = pActwk->xposi.w.h;
        pNewact->yposi.w.h = pActwk->yposi.w.h;
        pNewact->mstno.b.h = pActwk->mstno.b.h;

        if (actwkchk(&pNewact) == 0) {
            pNewact->actno = 24;
            pNewact->xposi.w.h = pActwk->xposi.w.h;
            pNewact->yposi.w.h = pActwk->yposi.w.h;
            pNewact->r_no1 = 1;
            pNewact->userflag.b.h = 1;
        }
    }

    frameout(pActwk);
}

static void ride_on_chk_i(sprite_status *pActwk, sprite_status *pPlayerwk) {
    pActwk->actno = 25;
    ride_on_chk(pActwk, pPlayerwk);
    pActwk->actno = 47;
}
