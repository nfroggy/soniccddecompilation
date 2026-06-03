#include "../equ.h"
#include "coli6.h"
#include "../actset.h"
#include "../fcol.h"
#include "../loader2.h"
#include "../player6.h"
#include "../player_work.h"
#include "../score.h"

static Uint8 bCarry;
Uint16 escoretbl[4] = {10, 20, 50, 100};
Uint8 colitbl[64][2] = {
    {20, 20}, {18, 12}, {16, 16}, {4, 16},  {12, 18},  {16, 16}, {6, 6},
    {24, 12}, {12, 16}, {16, 12}, {8, 8},   {20, 16},  {20, 8},  {14, 14},
    {24, 24}, {40, 16}, {16, 24}, {8, 16},  {32, 112}, {64, 32}, {128, 32},
    {32, 32}, {8, 8},   {4, 4},   {32, 8},  {12, 12},  {8, 4},   {24, 4},
    {40, 4},  {4, 8},   {4, 24},  {4, 40},  {4, 32},   {24, 24}, {12, 24},
    {72, 8},  {8, 12},  {16, 8},  {32, 16}, {32, 16},  {0, 0},   {0, 0},
    {0, 0},   {0, 0},   {0, 0},   {0, 0},   {8, 19},   {8, 28},  {24, 12},
    {16, 16}, {4, 4},   {8, 16},  {16, 5},  {12, 12},  {8, 8},   {26, 30},
    {8, 8},   {40, 36}, {18, 17}, {30, 48}, {60, 32},  {16, 8},  {2, 12},
    {0, 0}};

Sint16 pcol(sprite_status *pActwk) {
    player_work *player = player_work_get(pActwk);
    Sint16 iXwork = pActwk->xposi.w.h, iYwork = pActwk->yposi.w.h,
           iSprvs = pActwk->sprvsize - 3;
    Sint16 iXcollichk;
    Sint16 i;
    Sint16 iRet;
    Sint16 iColino;
    sprite_status *pColliAct;

    if (player->status_flags & 64)
        return 0;

    iXwork -= 8;
    iYwork -= iSprvs;
    if (pActwk->patno == 57) {
        iYwork += 12;
        iSprvs = 10;
    }

    iXcollichk = 16;
    iSprvs += iSprvs;
    pColliAct = &actwk[32];

    for (i = 0; i < 96; ++i, ++pColliAct) {

        if (pColliAct->actflg & 128) {
            iColino = pColliAct->colino;
            if (iColino != 0) {
                iRet =
                    CollitblDataXchk(pActwk, pColliAct, iXwork, iYwork, iSprvs);
                if (iRet)
                    return iRet;
            }
        }
    }
    return 0;
}

Sint16 CollitblDataXchk(sprite_status *pActwk, sprite_status *pColliAct,
                        Sint16 iXposi, Sint16 iYposi, Sint16 iD5) {
    Sint16 iColiNo = pColliAct->colino;
    Sint16 iColiwk;
    Sint16 iColiData;

    iColiNo = (iColiNo & 63) - 1;
    iColiwk = colitbl[iColiNo][0];

    iColiData = pColliAct->xposi.w.h - iColiwk;
    bCarry = CCset(iColiData, iXposi);
    iColiData = iColiData - iXposi;
    if (bCarry == 1) {
        iColiwk += iColiwk;
        bCarry = CSset(iColiData, iColiwk);
        iColiData += iColiwk;
        if (bCarry == 1)
            return CollitblDataYchk(pActwk, pColliAct, iYposi, iColiNo, iD5);
        else
            return 0;
    }

    if (iColiData > 16)
        return 0;

    return CollitblDataYchk(pActwk, pColliAct, iYposi, iColiNo, iD5);
}

Sint16 CollitblDataYchk(sprite_status *pActwk, sprite_status *pColliAct,
                        Sint16 iYposi, Sint16 iColiNo, Sint16 iD5) {
    Sint16 iColiData;
    Sint16 iYwork;

    iColiData = colitbl[iColiNo][1];

    iYwork = pColliAct->yposi.w.h - iColiData;
    bCarry = CCset(iYwork, iYposi);
    iYwork = iYwork - iYposi;
    if (bCarry == 1) {
        iColiData += iColiData;
        bCarry = CSset(iColiData, iYwork);
        iColiData += iYwork;
        if (bCarry == 1)
            return ColliHitChk(pActwk, pColliAct);
        else
            return 0;
    }

    if (iYwork > iD5)
        return 0;

    return ColliHitChk(pActwk, pColliAct);
}

Sint16 ColliHitChk(sprite_status *pActwk, sprite_status *pColliAct) {
    Uint8 iColino = pColliAct->colino & 192;

    if (iColino == 0) {
        pcolnomal(pActwk, pColliAct);
        return 1;
    }
    if (iColino == 192) {
        pcolspecial(pActwk, pColliAct);
        return 1;
    }
    if ((char)iColino < 0) {
        pcolplay(pActwk, pColliAct);
        return 1;
    }

    return pcolitem(pActwk, pColliAct);
}

Sint16 pcolitem(sprite_status *pActwk, sprite_status *pColliAct) {
    player_work *player = player_work_get(pActwk);
    Sint16 iD0;

    if ((pColliAct->colino & 63) != 6) {

        if (player->damage_invulnerability_timer < 90)
            pColliAct->r_no0 += 2;
    } else {

        if (pActwk->yspeed.w < 0) {
            iD0 = pActwk->yposi.w.h - 16;
            bCarry = CCset(iD0, pColliAct->yposi.w.h);
            if (bCarry == 0) {
                pActwk->yspeed.w = -pActwk->yspeed.w;
                pColliAct->yspeed.w = -384;
                if (pColliAct->r_no1 == 0)
                    pColliAct->r_no1 += 4;
            }
        } else {

            if (pActwk->mstno.b.h == 2) {
                pActwk->yspeed.w = -pActwk->yspeed.w;
                pColliAct->r_no0 += 2;
            }
        }
    }
    return -1;
}

Sint16 pcolnomal(sprite_status *pActwk, sprite_status *pColliAct) {
    Sint16 iScoreCntwk;
    Sint16 iScoreData;
    score_marker_work *score_marker = score_marker_work_get(pColliAct);

    if (plpower_a == 0 && plpower_m == 0 && pActwk->mstno.b.h != 2) {

        pcolplay(pActwk, pColliAct);
        return -1;
    }

    if (pColliAct->colicnt != 0) {
        pActwk->xspeed.w = -pActwk->xspeed.w;
        pActwk->yspeed.w = -pActwk->yspeed.w;
        pActwk->xspeed.w /= 2;
        pActwk->yspeed.w /= 2;
        pColliAct->colino = 0;
        --pColliAct->colicnt;
        if (pColliAct->colicnt == 0)
            pColliAct->cddat |= 128;
    } else {

        pColliAct->cddat |= 128;
        iScoreCntwk = emyscorecnt;
        emyscorecnt += 2;
        if ((Uint16)iScoreCntwk >= 6) {
            iScoreCntwk = 6;
        }
        score_marker->points_index = iScoreCntwk;
        iScoreData = escoretbl[iScoreCntwk / 2];
        if (emyscorecnt >= 32) {
            iScoreData = 1000;
            score_marker->points_index = 10;
        }

        scoreup(iScoreData);
        soundset(150);
        pColliAct->actno = 24;
        pColliAct->r_no0 = 0;
        pColliAct->userflag.b.h = 1;

        if (pActwk->yspeed.w >= 0) {
            if (pActwk->yposi.w.h < pColliAct->yposi.w.h)
                pActwk->yspeed.w = -pActwk->yspeed.w;
            else
                pActwk->yspeed.w -= 256;
        } else
            pActwk->yspeed.w += 256;
    }
    return -1;
}

Sint16 pcolplay2(sprite_status *pActwk, sprite_status *pColliAct) {
    pColliAct->cddat |= 128;
    pcolplay(pActwk, pColliAct);
}

Sint16 pcolplay(sprite_status *pActwk, sprite_status *pColliAct) {
    if (plpower_a != 0)
        return -1;
    if (plpower_m == 0)
        return pcole(pActwk, pColliAct);
    else
        return -1;
}

Sint16 pcole(sprite_status *pActwk, sprite_status *pColliAct) {
    if (player_work_get(pActwk)->damage_invulnerability_timer != 0)
        return -1;
    else
        return playdamageset(pActwk, pColliAct);
}

void playdamagechk(sprite_status *pActwk, sprite_status *pColliAct) {
    player_work *player = player_work_get(pActwk);

    if (!(plpower_b & 1))
        conbine_flag = 0;
    plpower_b &= 254;

    pActwk->r_no0 = 4;
    jumpcolsub();
    pActwk->cddat |= 2;

    if (pActwk->cddat & 64) {
        pActwk->yspeed.w = -512;
        pActwk->xspeed.w = -256;
    } else {
        pActwk->yspeed.w = -1024;
        pActwk->xspeed.w = -512;
    }

    if (pColliAct->xposi.w.h <= pActwk->xposi.w.h)
        pActwk->xspeed.w = -pActwk->xspeed.w;

    pActwk->mspeed.w = 0;
    pActwk->mstno.b.h = 26;
    player->damage_invulnerability_timer = 120;
}

Sint16 playdamageset(sprite_status *pActwk, sprite_status *pColliAct) {
    sprite_status *pFreeAct;
    Sint32 iRet;
    player_work *player = player_work_get(pActwk);

    player->spin_dash_counter = 0;
    player->status_flags &= 231;
    player->mode_word = 0;

    if (plpower_b != 0) {
        playdamagechk(pActwk, pColliAct);
        return -1;
    }
    if (plring != 0) {
        iRet = actwkchk(&pFreeAct);
        if (iRet == 0) {
            pFreeAct->actno = 17;
            pFreeAct->xposi.w.h = pActwk->xposi.w.h;
            pFreeAct->yposi.w.h = pActwk->yposi.w.h;
        }

        playdamagechk(pActwk, pColliAct);
        return -1;
    }

    if (debugflag.w != 0) {
        playdamagechk(pActwk, pColliAct);
        return -1;
    }
    playdieset(pActwk);
}

Sint16 playdieset(sprite_status *pActwk) {
    player_work *player = player_work_get(pActwk);

    if (editmode.w != 0)
        return -1;
    plpower_m = 0;
    pActwk->r_no0 = 6;
    jumpcolsub();
    pActwk->cddat |= 2;
    pActwk->yspeed.w = -1792;
    pActwk->xspeed.w = 0;
    pActwk->mspeed.w = 0;
    player->death_y = pActwk->yposi.w.h;
    pActwk->mstno.b.h = 24;
    pActwk->sproffset |= 32768;
    pActwk->sprpri = 0;
    soundset(147);
    return -1;
}

Sint16 pcolspecial(sprite_status *pActwk, sprite_status *pColliAct) {
    char cColiNo = pColliAct->colino & 63;

    switch (cColiNo) {
    case 31:
    case 35:
        ++pColliAct->colicnt;
        break;
    case 56:
    case 58:
    case 59:
        main_attack(pActwk, pColliAct);
        break;
    default:
        eggman_chk(pActwk, pColliAct, cColiNo);
        break;
    }
    return -1;
}

Sint16 eggman_chk(sprite_status *pActwk, sprite_status *pColliAct,
                  char cColiNo) {

    if (bossstart != 1)
        return -1;
    switch (bossstart) {
    case 1:
        boss_1(pActwk, pColliAct, cColiNo);
        break;
    case 4:
        boss_4(pActwk, pColliAct, cColiNo);
        break;
    case 5:
        boss_5(pActwk, pColliAct, cColiNo);
        break;
    default:
        boss_1(pActwk, pColliAct, cColiNo);
        break;
    }
    return 1;
}

void boss_1(sprite_status *pActwk, sprite_status *pColliAct, char cColiNo) {
    if (cColiNo < 60 || cColiNo > 63)
        return;

    pcolnomal(pActwk, pColliAct);
    if (pColliAct->colino == 0)
        pColliAct->colicnt += 3;
    pColliAct->colino = 0;

    ++pColliAct->colicnt;
}

void boss_4(sprite_status *pActwk, sprite_status *pColliAct, char cColiNo) {
    if (cColiNo == 63) {

        if (!(pActwk->cddat & 20))
            return;

        pActwk->cddat &= 251;
        pActwk->cddat &= 239;
        pActwk->colino = 0;
        pActwk->mstno.b.h = 21;

        pActwk->yspeed.w = 1024;
        pActwk->xspeed.w = -512;
        if (pActwk->xposi.w.h >= pColliAct->xposi.w.h)
            pActwk->xspeed.w = -pActwk->xspeed.w;
    } else if (cColiNo == 62) {
        pcolplay(pActwk, pColliAct);
        ++pColliAct->colicnt;
    }
}

void boss_5(sprite_status *pActwk, sprite_status *pColliAct, char cColiNo) {
    if (cColiNo == 61) {
        pcolplay(pActwk, pColliAct);
        ++pColliAct->colicnt;
    }
}

void main_attack(sprite_status *pActwk, sprite_status *pColliAct) {
    if (pActwk->mstno.b.h == 2)
        ++pColliAct->colicnt;
}
