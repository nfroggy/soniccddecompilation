#include "../equ.h"
#include "kumo.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../suicide.h"

static void kumo_init(sprite_status *pActwk);
static void kumo_jump(sprite_status *pActwk);
static void kumo_jump1(sprite_status *pActwk);
static void kumo_move(sprite_status *pActwk);
static void kumo_move1(sprite_status *pActwk);
static Sint16 kumo_check(sprite_status *pActwk0, sprite_status *pActwk1);
static void kumo_tama(sprite_status *pActwk);
static void kumo_tama1(sprite_status *pActwk);
static void tama(sprite_status *pActwk);

static Uint8 pchg_e_00[4] = {7, 1, 2, 255};
static Uint8 pchg_b_00[4] = {15, 1, 2, 255};
static Uint8 pchg_tama_00[4] = {2, 0, 1, 255};
static Uint8 *pchg_e[1] = {pchg_e_00};
static Uint8 *pchg_b[1] = {pchg_b_00};
static Uint8 *pchg_tama[1] = {pchg_tama_00};
static sprite_pattern kumo_e_pat00 = {1, {{-28, -25, 0, 409}}};
static sprite_pattern kumo_e_pat01 = {1, {{-28, -17, 0, 410}}};
static sprite_pattern kumo_e_pat02 = {1, {{-28, -13, 0, 411}}};
static sprite_pattern kumo_b_pat00 = {1, {{-28, -25, 0, 412}}};
static sprite_pattern kumo_b_pat01 = {1, {{-28, -17, 0, 413}}};
static sprite_pattern kumo_b_pat02 = {1, {{-28, -13, 0, 414}}};
static sprite_pattern tama_pat00 = {1, {{-12, -8, 0, 415}}};
static sprite_pattern tama_pat01 = {1, {{-12, -8, 0, 416}}};
sprite_pattern *pat_kumo_e[3] = {&kumo_e_pat00, &kumo_e_pat01, &kumo_e_pat02};
sprite_pattern *pat_kumo_b[3] = {&kumo_b_pat00, &kumo_b_pat01, &kumo_b_pat02};
sprite_pattern *pat_tama[2] = {&tama_pat00, &tama_pat01};

void kumo(sprite_status *pActwk) {
    void (*kumo_move_tbl[7])(sprite_status *) = {
        kumo_init,  kumo_jump, kumo_jump1, kumo_move,
        kumo_move1, kumo_tama, kumo_tama1};

    if (pActwk->userflag.b.h & 128) {

        tama(pActwk);
        return;
    }

    if (enemy_suicide(pActwk))
        return;
    kumo_move_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s(pActwk);
}

static void kumo_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->sproffset = 9236;
    pActwk->sprhs = 24;
    pActwk->sprhsize = 24;
    pActwk->sprvsize = 24;
    pActwk->colino = 53;

    if (!pActwk->userflag.b.h) {
        pActwk->patbase = pat_kumo_e;
        ((Uint8 ***)pActwk)[12] = pchg_e;
        ((Sint32 *)pActwk)[13] = -524288;
    } else {
        pActwk->patbase = pat_kumo_b;
        ((Uint8 ***)pActwk)[12] = pchg_b;
        ((Sint32 *)pActwk)[13] = -327680;
    }

    if (pActwk->userflag.b.l) {
        pActwk->actflg |= 1;
        pActwk->cddat |= 1;
    }

    pActwk->r_no0 += 2;
    kumo_jump1(pActwk);
}

static void kumo_jump(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->patno = 0;
    pActwk->yposi.w.h -= 8;
    ((Sint32 *)pActwk)[14] = ((Sint32 *)pActwk)[13];
    kumo_jump1(pActwk);
}

static void kumo_jump1(sprite_status *pActwk) {
    Sint16 wD1;

    ((Sint32 *)pActwk)[14] += 16384;
    pActwk->yposi.l += ((Sint32 *)pActwk)[14];
    if ((wD1 = emycol_d(pActwk)) < 0) {
        pActwk->yposi.w.h += wD1;
        pActwk->r_no0 += 2;
        pActwk->patno = 1;
        pActwk->yposi.w.h += 8;
    }
}

static void kumo_move(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->colino = 54;
    ((Sint16 *)pActwk)[23] = 180;
    kumo_move1(pActwk);
}

static void kumo_move1(sprite_status *pActwk) {
    if (!pActwk->userflag.b.h) {
        if (kumo_check(pActwk, &actwk[0])) {
            pActwk->r_no0 += 2;
            pActwk->patno = 1;
            ((Sint16 *)pActwk)[23] = 60;
            return;
        }
    }

    if (--((Sint16 *)pActwk)[23] == 0) {
        pActwk->r_no0 -= 6;
    }
    patchg(pActwk, ((Uint8 ***)pActwk)[12]);
}

static Sint16 kumo_check(sprite_status *pActwk0, sprite_status *pActwk1) {
    Sint16 wD0, wD1;

    if (pActwk1->yposi.w.h - pActwk0->yposi.w.h + 80 < 160) {
        wD0 = pActwk1->xposi.w.h - pActwk0->xposi.w.h;
        wD1 = wD0 - 80;
        if (wD0 + 80 < 160) {
            if (wD0 < 0)
                wD0 = -1;
            else
                wD0 = 0;
            if (pActwk0->actflg & 1)
                wD1 = 0;
            else
                wD1 = -1;
            if (!(wD0 ^ wD1)) {
                pActwk0->actflg ^= 1;
                pActwk0->cddat ^= 1;
                return -1;
            }
        }
    }

    return 0;
}

static void kumo_tama(sprite_status *pActwk) {
    if (!(--((Sint16 *)pActwk)[23])) {
        pActwk->r_no0 += 2;
    }
}

static void kumo_tama1(sprite_status *pActwk) {
    sprite_status *pNewactwk;
    Sint16 wD0;
    Sint32 lD1;

    if (actwkchk(&pNewactwk) == 0) {
        pNewactwk->actno = pActwk->actno;
        pNewactwk->userflag.b.h = -1;
        pNewactwk->xposi.w.h = pActwk->xposi.w.h;
        pNewactwk->yposi.w.h = pActwk->yposi.w.h;
        pNewactwk->actflg = pActwk->actflg;
        pNewactwk->sprpri = 1;
        pNewactwk->sproffset = pActwk->sproffset;
        pNewactwk->sprhs = 12;
        pNewactwk->sprhsize = 12;
        pNewactwk->sprvsize = 8;
        pNewactwk->colino = 247;
        pNewactwk->patbase = pat_tama;
        wD0 = 24;
        lD1 = 196608;
        if (pNewactwk->actflg & 1) {
            wD0 *= -1;
            lD1 *= -1;
        }

        pNewactwk->xposi.w.h += wD0;
        ((Sint32 *)pNewactwk)[14] = lD1;
        if (pActwk->actflg & 128) {
            soundset(160);
        }
    }
    pActwk->r_no0 -= 10;
}

static void tama(sprite_status *pActwk) {
    if (!((Sint16 *)pActwk)[23]) {
        if (pActwk->colicnt) {
            pActwk->colicnt = 0;
            if (actwk[0].r_no0 != 4) {
                if (actwk[0].r_no0 != 6) {
                    if (!((Sint16 *)&actwk[0])[26]) {

                        ((Sint16 *)pActwk)[23] = 120;
                        goto label1;
                    }
                }
            }
        }

        pActwk->xposi.l += ((Sint32 *)pActwk)[14];
        patchg(pActwk, pchg_tama);
        actionsub(pActwk);
        frameout_s(pActwk);
        return;
    }

label1:
    if (!(--((Sint16 *)pActwk)[23])) {
        frameout(pActwk);
        return;
    }
    pActwk->xposi.w.h = actwk[0].xposi.w.h;
    pActwk->yposi.w.h = actwk[0].yposi.w.h;
    actwk[0].xspeed.w = 0;
    actwk[0].mspeed.w = 0;

    patchg(pActwk, pchg_tama);
    actionsub(pActwk);
    frameout_s(pActwk);
}
