#include "../equ.h"
#include "awa.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../io.h"
#include "playsub4.h"

Uint8 awachg0[5] = {14, 0, 1, 2, 252};
Uint8 awachg1[6] = {14, 1, 2, 3, 4, 252};
Uint8 awachg2[7] = {14, 2, 3, 4, 5, 6, 252};
Uint8 awachg4[2] = {4, 252};
Uint8 awachg5[5] = {4, 6, 7, 8, 252};
Uint8 awachg6[5] = {15, 19, 20, 21, 255};
Uint8 *awachg[7] = {awachg0, awachg1, awachg2, awachg4,
                    awachg4, awachg5, awachg6};
extern sprite_pattern *awapat[];
extern Uint8 awasintbl[];

void awa(sprite_status *pActwk) {
    void (*awa_move_tbl[6])(sprite_status *) = {
        &awainit, &awamove, &awamove2, &awamove3, &awamove4, &awamaster};

    awa_move_tbl[pActwk->r_no0 / 2](pActwk);
}

void awainit(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->patbase = awapat;
    pActwk->sproffset = 33930;
    pActwk->actflg = 132;
    pActwk->sprhsize = 16;
    pActwk->sprpri = 1;
    if (pActwk->userflag.b.h & 128) {

        pActwk->r_no0 += 8;
        pActwk->actfree[8] = pActwk->userflag.b.h & 127;
        pActwk->actfree[9] = pActwk->userflag.b.h & 127;
        pActwk->mstno.b.h = 6;
        awamaster(pActwk);
    } else {

        pActwk->mstno.b.h = pActwk->userflag.b.h;
        ((Sint16 *)pActwk)[26] = pActwk->xposi.w.h;
        pActwk->yspeed.w = -136;
        pActwk->direc.b.h = random() & 255;
        awamove(pActwk);
    }
}

void awamove(sprite_status *pActwk) {
    patchg(pActwk, awachg);
    if (pActwk->patno == 6)
        pActwk->actfree[4] = 1;

    awamove2(pActwk);
}

void awamove2(sprite_status *pActwk) {
    if (waterposi >= pActwk->yposi.w.h) {

        pActwk->r_no0 = 6;
        pActwk->mstno.b.h += 3;
        awamove3(pActwk);
        return;
    }

    pActwk->xposi.w.h = (Sint16)(char)awasintbl[pActwk->direc.b.h++ & 127] +
                        ((Sint16 *)pActwk)[26];

    if (pActwk->actfree[4]) {
        if (awacoli(pActwk) != 0) {
            plairset();
            soundset(173);

            actwk[0].xspeed.w = 0;
            actwk[0].yspeed.w = 0;
            actwk[0].mspeed.w = 0;
            actwk[0].mstno.b.h = 21;
            ((Sint16 *)&actwk[0])[33] = 35;
            actwk[0].actfree[18] = 0;
            actwk[0].cddat &= 223;
            actwk[0].cddat &= 239;

            if (actwk[0].cddat & 4) {
                actwk[0].cddat &= 251;
                actwk[0].sprvsize = 19;
                actwk[0].sprhs = 9;
                actwk[0].yposi.w.h -= 5;
            }
            pActwk->r_no0 = 6;
            pActwk->mstno.b.h += 3;
            awamove3(pActwk);
            return;
        }
    }

    speedset2(pActwk);
    if (pActwk->actflg & 128)
        actionsub(pActwk);
    else
        frameout(pActwk);
}

void awamove3(sprite_status *pActwk) {
    patchg(pActwk, awachg);

    if (pActwk->actflg & 128)
        actionsub(pActwk);
    else
        frameout(pActwk);
}

void awamove4(sprite_status *pActwk) { frameout(pActwk); }

void awamaster(sprite_status *pActwk) {
    Uint8 awatbl[18] = {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0};
    sprite_status *pNewactwk;
    Uint8 bD0;
    Sint16 wD1;

    if (!((Uint16 *)pActwk)[29]) {
        if (waterposi >= pActwk->yposi.w.h) {
            awafoutchk(pActwk);
            return;
        }
        if (!(pActwk->actflg & 128)) {
            awafoutchk(pActwk);
            return;
        }
        if (--((Sint16 *)pActwk)[30] >= 0)
            goto label3;

        ((Sint16 *)pActwk)[29] = 1;

        do {
            wD1 = random();
            bD0 = wD1 & 7;
        } while (bD0 >= 6);

        pActwk->actfree[10] = bD0;
        wD1 &= 12;

        pActwk->actfree[18] = wD1;
        if (--pActwk->actfree[8] & 128) {
            pActwk->actfree[8] = pActwk->actfree[9];
            pActwk->actfree[12] |= 128;
        }

        goto label1;
    }
    if (--((Sint16 *)pActwk)[30] >= 0)
        goto label3;

label1:
    ((Sint16 *)pActwk)[30] = random() & 31;

    if (pActwk->actflg & 128) {
        if (actwkchk(&pNewactwk) == 0) {
            pNewactwk->actno = 32;
            pNewactwk->xposi.w.h =
                pActwk->xposi.w.h + (Sint16)((random() & 15) - 8);
            pNewactwk->yposi.w.h = pActwk->yposi.w.h;
            pNewactwk->userflag.b.h =
                awatbl[pActwk->actfree[18] + pActwk->actfree[10]];

            if (pActwk->actfree[12] & 128) {
                if ((random() & 3) == 0) {
                    if (pActwk->actfree[12] & 64)
                        goto label2;

                    pActwk->actfree[12] |= 64;
                    pNewactwk->userflag.b.h = 2;
                }

                if (!pActwk->actfree[10]) {
                    if (!(pActwk->actfree[12] & 64)) {
                        pActwk->actfree[12] |= 64;
                        pNewactwk->userflag.b.h = 2;
                    }
                }
            }
        }
    }

label2:
    if (--pActwk->actfree[10] & 128) {
        ((Sint16 *)pActwk)[30] += (random() & 127) + 128;
        ((Sint16 *)pActwk)[29] = 0;
    }

label3:
    patchg(pActwk, awachg);
    awafoutchk(pActwk);
}

void awafoutchk(sprite_status *pActwk) {
    if (!(pActwk->actflg & 128)) {
        if ((pActwk->xposi.w.h & 65408) - (scra_h_posit.w.h - 128 & 65408) >
            640) {
            frameout(pActwk);
            return;
        }
    }

    if (waterposi < pActwk->yposi.w.h)
        actionsub(pActwk);
    else
        pActwk->actflg &= 127;
}

Sint32 awacoli(sprite_status *pActwk) {
    if (!(actwk[0].actfree[2] & 128)) {
        if (pActwk->xposi.w.h - 16 < actwk[0].xposi.w.h) {
            if (pActwk->xposi.w.h + 16 >= actwk[0].xposi.w.h) {
                if (pActwk->yposi.w.h < actwk[0].yposi.w.h) {
                    if (pActwk->yposi.w.h + 16 >= actwk[0].yposi.w.h)
                        return 1;
                }
            }
        }
    }
    return 0;
}
