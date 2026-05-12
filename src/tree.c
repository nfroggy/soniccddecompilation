#include "equ.h"
#include "tree.h"
#include "action.h"
#include "actset.h"
#include "playsub.h"

void (*hoshi_tbl[2])(sprite_status *, sprite_status *) = {&hoshi_init,
                                                          &hoshi_move};
extern sprite_pattern *hoshipat[];

void hoshi(sprite_status *pActwk) {
    Sint16 iXposi;

    hoshi_tbl[pActwk->r_no0 / 2](pActwk, &actwk[0]);
    actionsub(pActwk);
    iXposi = ((Uint16 *)pActwk)[23];
    frameout_s00(pActwk, iXposi);
}

void hoshi_init(sprite_status *pActwk, sprite_status *pPlaywk) {
    sprite_status *pActfree;
    Sint16 i, iXwork, iLp, iOffset;
    Sint16 tbl0[4] = {64, 128, -64, -128};
    Sint16 tbl1[3] = {0, 96, -96};

    iXwork = pPlaywk->xposi.w.h;
    pActwk->actflg |= 4;
    pActwk->patbase = hoshipat;
    pActwk->sproffset = 17444;
    pActwk->sprhsize = 24;
    pActwk->sprvsize = 20;
    iXwork = pActwk->xposi.w.h;
    pActfree = pActwk;
    if (pActwk->userflag.b.h & 1)
        iLp = 2;
    else
        iLp = 4;
    pActwk->userflag.b.h &= -2;

    iOffset = 0;

    for (i = 0; i < iLp; ++i, ++iOffset) {
        if (i > 0)
            actwkchk(&pActfree);
        pActfree->r_no0 += 2;
        pActfree->actno = 44;
        pActfree->xposi.w.h = iXwork;
        pActfree->yposi.w.h = pActwk->yposi.w.h;
        ((Sint16 *)pActfree)[23] = iXwork;
        pActfree->patbase = pActwk->patbase;
        pActfree->sproffset = pActwk->sproffset;
        pActfree->sprhsize = pActwk->sprhsize;
        pActfree->sprvsize = pActwk->sprvsize;
        pActfree->actflg |= 4;
        pActfree->xposi.w.h += tbl0[iOffset];
        ((Sint16 *)pActfree)[24] = pActfree->xposi.w.h;
    }

    iLp = 3;
    iOffset = 0;

    for (i = 0; i < iLp; ++i, ++iOffset) {
        actwkchk(&pActfree);
        pActfree->r_no0 += 2;
        pActfree->actno = 44;
        pActfree->userflag.b.h = 1;
        pActfree->patno = 1;
        pActfree->sprpri = 4;
        pActfree->xposi.w.h = iXwork;
        ((Sint16 *)pActfree)[23] = iXwork;
        pActfree->yposi.w.h = pActwk->yposi.w.h;
        pActfree->patbase = pActwk->patbase;
        pActfree->sproffset = pActwk->sproffset;
        pActfree->sprhsize = 12;
        pActfree->sprvsize = 12;
        pActfree->actflg |= 4;
        pActfree->xposi.w.h += tbl1[iOffset];
    }
}

void (*k0_tbl[2])(sprite_status *, sprite_status *) = {&k0_init, &k0_move};
void (*kasoku_tbl[2])(sprite_status *, sprite_status *) = {&kasoku_init,
                                                           &kasoku_move};
sprite_pattern hoshipat0 = {1, {{-24, -20, 0, 392}}};
sprite_pattern hoshipat1 = {1, {{-12, -12, 0, 393}}};
sprite_pattern *hoshipat[2] = {&hoshipat0, &hoshipat1};
Uint8 kasokuchg0[12] = {0, 4, 4, 3, 3, 3, 5, 5, 2, 2, 2, 255};
Uint8 kasokuchg1[6] = {2, 4, 7, 5, 6, 255};
Uint8 kasokuchg2[6] = {2, 8, 0, 9, 0, 255};
Uint8 *kasokuchg[3] = {kasokuchg0, kasokuchg1, kasokuchg2};
sprite_pattern kasokupat_a = {1, {{-24, -24, 0, 382}}};
sprite_pattern kasokupat_b = {1, {{-35, -24, 0, 383}}};
sprite_pattern kasokupat_c = {1, {{-40, -24, 0, 384}}};
sprite_pattern kasokupat_d = {1, {{-43, -24, 0, 385}}};
sprite_pattern kasokupat_e = {1, {{-40, -24, 0, 386}}};
sprite_pattern kasokupat_f = {1, {{-40, -24, 0, 387}}};
sprite_pattern kasokupat_g = {1, {{-40, -24, 0, 387}}};
sprite_pattern kasokupat_h = {1, {{-40, -24, 0, 388}}};
sprite_pattern kasokupat_i = {1, {{-28, -24, 0, 389}}};
sprite_pattern kasokupat_j = {1, {{-28, -24, 0, 390}}};
sprite_pattern *kasokupat[10] = {
    &kasokupat_a, &kasokupat_b, &kasokupat_c, &kasokupat_d, &kasokupat_e,
    &kasokupat_f, &kasokupat_g, &kasokupat_h, &kasokupat_i, &kasokupat_j};

void hoshi_move(sprite_status *pActwk, sprite_status *pPlaywk) {
    Sint16 iXwork = 0;

    if (pActwk->userflag.b.h != 0)
        return;
    if (pPlaywk->actfree[2] & 2) {
        iXwork = pPlaywk->xposi.w.h;
        iXwork &= 255;
        if (pPlaywk->xposi.w.h < ((Sint16 *)pActwk)[23])
            iXwork = 255 - iXwork;

        if (iXwork >= 192) {
            if (iXwork >= 240)
                iXwork = 0;
            else
                iXwork = 191;
        }

        iXwork >>= 1;
        if (pPlaywk->xposi.w.h < ((Sint16 *)pActwk)[23])
            iXwork *= -1;
    }
    iXwork += ((Uint16 *)pActwk)[24];
    pActwk->xposi.w.h = iXwork;
}

void kasoku0(sprite_status *pActwk) {
    k0_tbl[pActwk->r_no0 / 2](pActwk, &actwk[0]);
    frameout_s(pActwk);
}

void k0_init(sprite_status *pActwk, sprite_status *pPlaywk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    k0_move(pActwk, pPlaywk);
}

void k0_move(sprite_status *pActwk, sprite_status *pPlaywk) {
    Sint16 iD0;

    if (pPlaywk->mstno.b.h == 43)
        return;
    iD0 = pActwk->yposi.w.h - pPlaywk->yposi.w.h;
    iD0 += 64;
    if ((Uint16)iD0 >= 128)
        return;

    iD0 = pActwk->xposi.w.h - pPlaywk->xposi.w.h;
    iD0 += 32;
    if ((Uint16)iD0 >= 64)
        return;

    if (pPlaywk->xspeed.w >= 0)
        return;
    if (pActwk->xposi.w.h < pPlaywk->xposi.w.h)
        return;

    pPlaywk->xposi.w.h = pActwk->xposi.w.h;
    pPlaywk->xspeed.w = 0;
    pPlaywk->mspeed.w = 0;
    pPlaywk->mstno.b.h = 55;
    pPlaywk->actfree[18] = 1;
    pPlaywk->actfree[14] = 0;
    pActwk->sprvsize = 14;
    pActwk->sprhs = 7;
    pActwk->yposi.w.h += 5;
    pPlaywk->cddat |= 4;

    pActwk->userflag.b.l = 0;
}

void kasoku(sprite_status *pActwk) {
    Sint16 iXwork;

    if (pActwk->userflag.b.l != 0) {
        kasoku0(pActwk);
        return;
    }
    kasoku_tbl[pActwk->r_no0 / 2](pActwk, &actwk[0]);
    actionsub(pActwk);
    iXwork = ((Uint16 *)pActwk)[23];
    frameout_s00(pActwk, iXwork);
}

void kasoku_init(sprite_status *pActwk, sprite_status *pPlaywk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 1;
    pActwk->patbase = kasokupat;
    pActwk->sproffset = 1089;
    pActwk->sprhsize = 32;
    pActwk->sprvsize = 32;
    ((Sint16 *)pActwk)[23] = pActwk->xposi.w.h;
    if (pActwk->userflag.b.h != 0) {
        pActwk->actflg |= 1;
        pActwk->cddat |= 1;
    }
    kasoku_move(pActwk, pPlaywk);
}

void kasoku_move(sprite_status *pActwk, sprite_status *pPlaywk) {
    Sint16 iD0, iD1, iD2;
    Sint32 lD0wk;

    if (pActwk->actfree[4] != 0) {
        pActwk->mstno.b.h = 1;
        if (!(pPlaywk->actfree[2] & 2))
            ++pActwk->mstno.b.h;

        patchg(pActwk, kasokuchg);
    } else {
        pActwk->patno = 0;
        iD1 = 0;
        if (!(pPlaywk->actfree[2] & 2))
            goto label1;
    }

    iD0 = pPlaywk->xposi.w.h & 255;
    if (pActwk->userflag.b.h != 0)
        iD0 = 255 - iD0;

    if (iD0 >= 192)
        if (iD0 >= 240)
            iD0 = 0;
        else
            iD0 = 191;

    lD0wk = iD0;
    iD1 = iD0;
    if (pActwk->actfree[4] == 0) {
        lD0wk /= 48;
        pActwk->patno = lD0wk & 255;
    }

    iD1 >>= 2;
    iD2 = iD1 >> 1;
    iD1 += iD2;
    if (pActwk->userflag.b.h != 0)
        iD1 *= -1;
label1:
    pActwk->xposi.w.h = ((Sint16 *)pActwk)[23] + iD1;

    if (pActwk->actfree[4] != 0)
        --pActwk->actfree[4];

    iD1 = pActwk->sprhsize;
    iD0 = pPlaywk->xposi.w.h - pActwk->xposi.w.h;
    iD0 += iD1;
    if (iD0 < 0)
        return;
    iD1 += iD1;
    if (iD0 >= iD1)
        return;

    iD1 = pActwk->sprvsize;
    iD0 = pPlaywk->yposi.w.h - pActwk->yposi.w.h;
    iD0 += iD1;
    if (iD0 < 0)
        return;
    iD1 += iD1;
    if (iD0 >= iD1)
        return;
    if (pPlaywk->mstno.b.h == 43)
        return;

    if (pActwk->actfree[4] == 0)
        pActwk->actfree[4] = 60;

    if (pPlaywk->yspeed.w < 0) {
        pPlaywk->yspeed.w = -3072;
    } else

        pPlaywk->yspeed.w = 3072;
}
