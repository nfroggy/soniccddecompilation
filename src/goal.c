#include <stddef.h>

#include "equ.h"
#include "goal.h"
#include "action.h"
#include "actset.h"
#include "dircol.h"
#include "loader2.h"
#include "playsub.h"

extern void colorset2(Sint32 ColorNo);

void (*gene_tbl[6])(sprite_status *) = {&gene_init,  &gene_move0, &gene_move1,
                                        &goal_move2, &goal_move3, &kira_move};
extern sprite_pattern *genepat[];
extern Uint8 *genechg[];
extern map_init_data mapinittbl;
extern sprite_pattern *bring2pat[];
extern Uint8 *bring2chg[];
extern sprite_pattern *bringpat[];
extern Uint8 *bringchg[];
extern sprite_pattern *goalpat[];
extern Uint8 *goalchg[];

#pragma pack(push, 1)
typedef struct {
    Uint8 timer;
    Uint8 reserved1[7];
    Sint16 score_display_delay;
} goal_work;
#pragma pack(pop)

_Static_assert(offsetof(goal_work, timer) == 0,
               "goal_work.timer must map to offset 0");
_Static_assert(offsetof(goal_work, score_display_delay) == 8,
               "goal_work.score_display_delay must map to offset 8");
_Static_assert(sizeof(goal_work) <= sizeof(((sprite_status *)0)->actfree),
               "goal_work must fit in sprite_status.actfree");

static goal_work *goal_work_get(sprite_status *pActwk) {
    return (goal_work *)pActwk->actfree;
}

void gene(sprite_status *pActwk) {
    gene_tbl[pActwk->r_no0 / 2](pActwk);
    if (pActwk->r_no0 == 0)
        return;

    if (pActwk->r_no0 != 10) {

        if ((Uint32)pActwk->r_no0 >= 6)
            return;
    }

    actionsub(pActwk);
}

void gene_init(sprite_status *pActwk) {
    pActwk->actflg |= 4;
    pActwk->r_no0 += 2;
    pActwk->sprpri = 4;
    pActwk->patbase = genepat;
    pActwk->sproffset = 9345;
    pActwk->sprhs = 32;
    pActwk->sprhsize = 32;
    pActwk->sprvsize = 24;
    gene_move0(pActwk);
}

void gene_move0(sprite_status *pActwk) {
    goal_work *work = goal_work_get(pActwk);
    Sint16 coli;
    Sint16 iD0, iD1;

    patchg(pActwk, genechg);
    coli = gene_coli(pActwk, &actwk[0]);
    if (coli == 0)
        return;

    pltime_f = 0;
    pActwk->patno = 2;
    work->timer = 120;
    pActwk->r_no0 += 2;

    iD0 = actwk[0].xposi.w.h;
    iD1 = actwk[0].sprhs;
    iD1 += 32;
    iD0 -= pActwk->xposi.w.h;
    iD0 += iD1;
    if (iD0 < 0)
        goto label1;
    iD1 += iD1;
    if (iD0 >= iD1)
        goto label1;

    iD0 = actwk[0].yspeed.w;
    iD0 *= -1;
    iD0 >>= 2;
    actwk[0].yspeed.w = iD0;
    return;
label1:
    iD0 = actwk[0].xspeed.w;
    iD0 *= -1;
    iD0 >>= 2;
    actwk[0].xspeed.w = iD0;
}

void gene_move1(sprite_status *pActwk) {
    goal_work *work = goal_work_get(pActwk);
    sprite_status *pActfree;
    Sint16 iD0, iD1, ret;
    Uint16 wOff;
    Uint8 bywk;
    char tbl[16] = {0,  0, 32,  -8, -32, 0, -24, -8,
                    24, 8, -16, 8,  16,  8, -8,  -8};

    bywk = work->timer;
    iD0 = bywk;
    --iD0;
    work->timer = iD0;
    if (iD0 >= 0) {
        iD1 = iD0;
        iD1 &= 3;
        if (iD1 != 0)
            return;
        iD0 >>= 2;
        iD0 &= 7;
        iD0 += iD0;
        wOff = iD0;
        ret = actwkchk(&pActfree);
        if (ret != 0)
            return;
        soundset(158);
        pActfree->actno = 24;
        pActfree->r_no1 = 1;
        pActfree->xposi.w.h = pActwk->xposi.w.h;
        pActfree->yposi.w.h = pActwk->yposi.w.h;
        iD0 = tbl[wOff];
        pActfree->xposi.w.h += iD0;
        iD0 = tbl[wOff + 1];
        pActfree->yposi.w.h += iD0;
        return;
    }
    kira_set(pActwk);
    pActwk->r_no0 += 2;
    work->timer = 60;
}

void kira_set(sprite_status *pActwk) {
    Sint32 i, ret, iColor;
    Sint16 iD1;
    sprite_status *pActfree;
    Sint16 tbl[11] = {0, -128, 128, -256, 256, -384, 384, -512, 512, -640, 640};

    iColor = mapinittbl.colorno1;
    colorset2(iColor);

    for (i = 0, iD1 = 0; i < 7; ++i, ++iD1) {

        ret = (Sint16)actwkchk(&pActfree);
        if (ret != 0)
            break;
        pActfree->actno = 21;
        pActfree->actflg |= 4;
        pActfree->xposi.w.h = pActwk->xposi.w.h;
        pActfree->yposi.w.h = pActwk->yposi.w.h;
        pActfree->r_no0 = 10;
        pActfree->patbase = genepat;
        pActfree->sproffset = 0;
        pActfree->mstno.b.h = 1;
        pActfree->yspeed.w = -1536;
        pActfree->xspeed.w = tbl[iD1];
    }
}

void (*bring2_tbl[3])(sprite_status *) = {&bring2init, &bring2move0,
                                          &bring2move1};
void (*bigring_tbl[3])(sprite_status *) = {&bring_init, &bring_move0,
                                           &bring_move1};
void (*mosugu_tbl[3])(sprite_status *, sprite_status *) = {
    &mosug_init, &mosug_move0, &mosug_move1};

void kira_move(sprite_status *pActwk) {
    Sint16 ret;

    patchg(pActwk, genechg);
    speedset(pActwk);
    ret = emycol_d(pActwk);
    if (ret < 0) {

        pActwk->actno = 31;
        pActwk->userflag.b.h = 1;
        pActwk->r_no0 = 0;
    }
}

Sint16 gene_coli(sprite_status *pActwk, sprite_status *pPlaywk) {
    Sint16 iD0, iD1;

    if (!(pPlaywk->cddat & 4))
        return 0;
    iD1 = pPlaywk->sprhs;
    iD1 += 32;
    iD0 = pPlaywk->xposi.w.h - pActwk->xposi.w.h;
    iD0 += iD1;
    if (iD0 < 0)
        return 0;
    iD1 += iD1;
    if (iD0 >= iD1)
        return 0;

    iD1 = pPlaywk->sprvsize;
    iD1 += 28;
    iD0 = pPlaywk->yposi.w.h - pActwk->yposi.w.h;
    iD0 += iD1;
    if (iD0 < 0)
        return 0;
    iD1 += iD1;
    if (iD0 >= iD1)
        return 0;
    return 1;
}

void bring2(sprite_status *pActwk) {
    bring2_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
}

void bring2init(sprite_status *pActwk) {
    pActwk->actflg |= 4;
    pActwk->r_no0 += 2;
    pActwk->sproffset = 1007;
    pActwk->patbase = bring2pat;
    bring2move0(pActwk);
}

void bring2move0(sprite_status *pActwk) { patchg(pActwk, bring2chg); }

void bring2move1(sprite_status *pActwk) { frameout(pActwk); }

void bigring(sprite_status *pActwk) {
    if (pActwk->userflag.b.h != 0) {

        bring2(pActwk);
        return;
    }
    if (plring < 50) {

        frameout_s(pActwk);
        return;
    }

    bigring_tbl[pActwk->r_no0 / 2](pActwk);
    if (pActwk->r_no0 == 4)
        return;
    actionsub(pActwk);
}

void bring_init(sprite_status *pActwk) {
    if (clrspflg_save == 127 || ta_flag != 0) {

        frameout(pActwk);
        return;
    }

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 9352;
    pActwk->patbase = bringpat;
    pActwk->sprhs = 32;
    pActwk->sprhsize = 32;
    pActwk->sprvsize = 32;
    bring_move0(pActwk);
}

void bring_move0(sprite_status *pActwk) {
    sprite_status *pActfree, *pPlaywk;
    Sint16 iD0, ret;
    do {
        pPlaywk = &actwk[0];
        ret = bring_coli(pActwk, pPlaywk);
        if (ret == 0) {
            bring_move1(pActwk);
            return;
        }

        special_flag = 1;
        pActwk->r_no0 += 2;
        iD0 = scra_h_posit.w.h;
        iD0 += 336;
        pPlaywk->xposi.w.h = iD0;
        plautoflag |= 1;
        swdata.w = 2056;
        pPlaywk->xspeed.w = 0;
        pPlaywk->mspeed.w = 0;
        scroll_start.b.h = 1;
        soundset(175);
        ret = actwkchk(&pActfree);
    } while (ret != 0);
    pActfree->actno = 20;
    pActfree->xposi.w.h = pActwk->xposi.w.h;
    pActfree->yposi.w.h = pActwk->yposi.w.h;
    pActfree->userflag.b.h = 1;
}

void bring_move1(sprite_status *pActwk) { patchg(pActwk, bringchg); }

Sint16 bring_coli(sprite_status *pActwk, sprite_status *pPlaywk) {
    Sint16 iD0, iD1;

    iD1 = pPlaywk->sprhs;
    iD1 += 16;
    iD0 = pPlaywk->xposi.w.h - pActwk->xposi.w.h;
    iD0 += iD1;
    if (iD0 < 0)
        return 0;
    iD1 += iD1;
    if (iD0 >= iD1)
        return 0;
    iD1 = pPlaywk->sprvsize;
    iD1 += 32;
    iD0 = pPlaywk->yposi.w.h - pActwk->yposi.w.h;
    iD0 += iD1;
    if (iD0 < 0)
        return 0;
    iD1 += iD1;
    if (iD0 >= iD1)
        return 0;
    return 1;
}

void mosugu(sprite_status *pActwk) {
    mosugu_tbl[pActwk->r_no0 / 2](pActwk, &actwk[0]);
    if (stageno.b.l != 2)
        actionsub(pActwk);
    frameout_s(pActwk);
}

void mosug_init(sprite_status *pActwk, sprite_status *pPlaywk) {
    if (stageno.w == 513 && time_flag == 1) {

        if (pActwk->userflag.b.h == 0) {

            pActwk->userflag.b.h = 1;

            return;
        }

        if (divdevwk[0] != 0)
            return;
    }

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 4;
    pActwk->patbase = goalpat;
    pActwk->sprhs = 16;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 32;
    pActwk->patno = 5;
    offset_set(pActwk);

    pActwk->sproffset &= 32767;

    mosug_move0(pActwk, pPlaywk);
}

void mosug_move0(sprite_status *pActwk, sprite_status *pPlaywk) {
    Sint16 iD0;
    Sint16 iD1;

    iD0 = pPlaywk->xposi.w.h + pPlaywk->sprhsize;
    iD1 = pActwk->xposi.w.h - pActwk->sprhsize;
    if (iD0 >= iD1) {

        if (!(pPlaywk->sproffset & 32768)) {
            pPlaywk->sproffset |= 32768;
        }
    }
    if (pPlaywk->xspeed.w < 0) {

        pPlaywk->sproffset &= 32767;
    }

    if (stageno.w == 1280 && scralim_left >= 11352 &&
        pPlaywk->yposi.w.h >= 620 && pPlaywk->yposi.w.h <= 652) {

        pPlaywk->sproffset &= 32767;
    }

    iD0 = pPlaywk->yposi.w.h - pActwk->yposi.w.h;
    iD0 += 128;
    if (iD0 < 0)
        return;

    if ((Uint16)iD0 >= 256)
        return;

    if ((Uint16)pPlaywk->xposi.w.h < (Uint16)pActwk->xposi.w.h)
        return;

    if (plflag == 2)
        return;

    pActwk->r_no0 += 2;
    scralim_left = scra_h_posit.w.h;
    scralim_n_left = scra_h_posit.w.h;
    backto_cnt = 0;
    time_item = 0;
    plpower_a = 0;
}

void mosug_move1(sprite_status *pActwk, sprite_status *pPlaywk) {
    Sint16 iD0;
    Sint16 iD1;

    iD0 = pPlaywk->xposi.w.h - pPlaywk->sprhsize;
    iD1 = pActwk->xposi.w.h + pActwk->sprhsize;
    if (iD0 > iD1) {

        if (pPlaywk->sproffset & 32768) {
            pPlaywk->sproffset &= 32767;
        }
    }
}

void offset_set(sprite_status *pActwk) {
    Sint16 tbl[56] = {858,  1271, 1271, 1271, 897, 1271, 1271, 1271, 768,  768,
                      768,  768,  768,  768,  768, 768,  1266, 1266, 1266, 1266,
                      1266, 1266, 1266, 1266, 698, 716,  691,  689,  698,  716,
                      691,  689,  596,  556,  660, 568,  632,  650,  700,  664,
                      942,  942,  942,  942,  942, 942,  942,  942,  544,  545,
                      588,  566,  574,  586,  605, 582};
    Sint16 iD0;
    Sint16 iD1;
    short_union stagewk;

    stagewk.w = stageno.w;
    stagewk.b.l <<= 7;
    stagewk.w >>= 4;
    iD0 = stagewk.b.l;
    iD1 = time_flag;
    if (iD1 == 2)
        iD1 += generate_flag;
    iD1 += iD1;
    iD0 += iD1;
    pActwk->sproffset = tbl[iD0 / 2];
    if (stageno.b.h != 3)
        pActwk->sproffset |= 32768;
}

void (*goal_tbl[5])(sprite_status *) = {&goal_init, &goal_move0, &goal_move1,
                                        &goal_move2, &goal_move3};

void goal(sprite_status *pActwk) {
    goal_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
}

void goal_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprhs = 24;
    pActwk->sprhsize = 24;
    pActwk->sprvsize = 32;
    pActwk->sprpri = 4;
    pActwk->sproffset = 1084;
    if (stageno.b.h != 0)
        pActwk->sproffset |= 128;
    pActwk->patbase = goalpat;
    goal_move0(pActwk);
}

void goal_move0(sprite_status *pActwk) {
    goal_work *work = goal_work_get(pActwk);
    sprite_status *pPlaywk;
    Sint16 iD0;

    pPlaywk = &actwk[0];
    iD0 = pPlaywk->yposi.w.h - pActwk->yposi.w.h;
    iD0 += 128;
    if (iD0 < 0)
        return;
    if ((Uint16)iD0 >= 256)
        return;
    if ((Uint16)pActwk->xposi.w.h >= (Uint16)pPlaywk->xposi.w.h)
        return;
    scralim_left = scra_h_posit.w.h;
    scralim_n_left = scra_h_posit.w.h;
    pltime_f = 0;
    work->timer = 120;
    pActwk->patno = 0;
    pActwk->r_no0 += 2;
    plpower_s = 0;
    plpower_m = 0;
    soundset(157);
}

void goal_move1(sprite_status *pActwk) {
    goal_work *work = goal_work_get(pActwk);

    patchg(pActwk, goalchg);
    --work->timer;
    if (work->timer != 0)
        return;
    pActwk->r_no0 += 2;
    pActwk->patno = 3;
    work->timer = 60;
}

void goal_move2(sprite_status *pActwk) {
    goal_work *work = goal_work_get(pActwk);
    sprite_status *pActfree;
    Sint16 iD0;
    char cTime;
    Uint16 timebonustbl[21] = {50000, 50000, 10000, 5000, 4000, 4000, 3000,
                               3000,  2000,  2000,  2000, 2000, 1000, 1000,
                               1000,  1000,  500,   500,  500,  500,  0};

    cTime = (char)work->timer - 1;
    work->timer = cTime;
    if (cTime != 0)
        return;
    if (time_flag == 0) {
        sub_sync(130);
    }
    sub_sync(107);

    plautoflag |= 1;
    swdata.w = 2056;
    if (stageno.w == 1282) {
        swdata.w = 0;
    }
    work->timer = 180;
    pActwk->r_no0 += 2;

    if (actwkchk(&pActfree) != 0) {

        pActfree = &actwk[127];
        frameout(pActfree);
    }

    pActfree->actno = 58;
    goal_work_get(pActfree)->score_display_delay = 16;

    bonus_f = 1;
    iD0 = pltime.b.b3 + pltime.b.b2 * 60;
    iD0 /= 15;
    if (iD0 > 20) {
        iD0 = 20;
    }

    iD0 += iD0;
    timebonus = timebonustbl[iD0 / 2];
    iD0 = plring;
    ringbonus = iD0 * 100;
}

void goal_move3(sprite_status *pActwk) {

    pActwk->xposi.w.h = pActwk->xposi.w.h;
}

void genecolor(void) {
    PALETTEENTRY colortbl[16] = {
        {32, 32, 160, 1},   {0, 0, 0, 1},       {64, 64, 96, 1},
        {96, 96, 160, 1},   {128, 128, 192, 1}, {160, 160, 224, 1},
        {224, 224, 224, 1}, {128, 224, 160, 1}, {96, 160, 128, 1},
        {64, 96, 96, 1},    {32, 64, 64, 1},    {0, 32, 32, 1},
        {224, 224, 0, 1},   {160, 160, 0, 1},   {64, 64, 0, 1},
        {224, 0, 0, 1}};
    Sint16 i;
    PALETTEENTRY *lpPeSrc, *lpPeDest;

    lpPeSrc = colortbl;
    lpPeDest = lpcolorwk + 16;
    for (i = 0; i < 16; ++i) {
        *lpPeDest++ = *lpPeSrc++;
    }
}
