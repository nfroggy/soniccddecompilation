#include "../equ.h"
#include "kdai6.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../player_work.h"
#include "../playsub.h"
#include "../ridechk.h"

typedef struct {
    Sint16 origin_y;
    Sint16 origin_x;
    Uint8 wobble_counter;
} kdai6_work;

static kdai6_work *kdai6_get_work(sprite_status *pActwk) {
    return (kdai6_work *)pActwk->actfree;
}

void (*kdai6_tbl[2])(sprite_status *) = {&kdai6_init, &kdai6_move};
extern Uint8 *kdai6pchg[1];
extern sprite_pattern *kdai6pat[6];

void kdai6(sprite_status *pActwk) {
    kdai6_work *work = kdai6_get_work(pActwk);

    kdai6_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s00(pActwk, work->origin_x);
}

Sint16 kdai6_ridechk(sprite_status *pActwk) { ridechk(pActwk, &actwk[0]); }

void kdai6_init(sprite_status *pActwk) {
    kdai6_work *work = kdai6_get_work(pActwk);

    pActwk->actflg |= 4;
    pActwk->sprpri = 1;
    pActwk->sproffset = 17258;
    pActwk->patbase = kdai6pat;
    work->origin_x = pActwk->xposi.w.h;
    work->origin_y = pActwk->yposi.w.h;
    pActwk->sprvsize = 12;
    pActwk->sprhsize = 16;
    pActwk->r_no0 += 2;
}

void kdai6_move(sprite_status *pActwk) {
    Sint16 iD0;
    sprite_status *pPlaywk;
    player_work *player;

    pPlaywk = &actwk[0];
    player = player_work_get(pPlaywk);
    dai6_move(pActwk);

    patchg(pActwk, kdai6pchg);

    iD0 = kdai6_ridechk(pActwk);
    if (iD0 == 0)
        return;

    pPlaywk->cddat |= 1;
    pPlaywk->actflg &= 252;
    pPlaywk->actflg |= 1;
    if (!(player->status_flags & 1)) {
        pPlaywk->mstno.b.h = 45;
        player->special_angle = 0;
        iD0 = pPlaywk->xposi.w.h - pActwk->xposi.w.h;
        if (iD0 < 0) {
            iD0 = -iD0;
            player->special_angle = 128;
        }

        player->orbit_radius = iD0;
    }

    player->status_flags |= 1;
    if ((Uint32)pPlaywk->r_no0 >= 6)
        return;

    kaiten_play(pActwk);
}

Sint16 kdai6_rangecheck(sprite_status *pActwk) {
    Sint16 iD0;

    iD0 = (Sint16)(Uint16)actwk[0].sprvsize + actwk[0].yposi.w.h;
    iD0 -= pActwk->yposi.w.h;
    if (iD0 < 0)
        return 0;
    if (iD0 < 16)
        return 0;
    return -1;
}

void kaiten_play(sprite_status *pActwk) {
    Uint8 tbl[16] = {0, 0, 0, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 5};
    Uint8 byD0, byD1;
    Sint16 iSin, iCos;
    sprite_status *pPlaywk;
    player_work *player;
    int_union lD0;

    pPlaywk = &actwk[0];
    player = player_work_get(pPlaywk);
    player->special_angle += 4;
    sinset(player->special_angle, &iSin, &iCos);
    byD0 = player->orbit_radius;
    lD0.w.l = (Uint16)byD0;
    lD0.l = lD0.w.l * iCos;
    lD0.l >>= 8;
    pPlaywk->xposi.w.h = pActwk->xposi.w.h + lD0.w.l;

    byD0 = player->special_angle;
    byD1 = byD0;
    byD0 &= 240;
    byD0 >>= 4;
    pPlaywk->patcnt = tbl[byD0];
    if (!(byD1 & 63))
        ++player->orbit_radius;

    swdata.w = swdata1.w;
    if (pPlaywk->actno != 1)
        swdata.w = swdata2.w;

    k_move(pActwk);
    jumpchk_d(pActwk);
}

void (*move_tbl[4])(sprite_status *) = {&kd_mv_right, &kd_mv_left, &kd_mv_up,
                                        &kd_mv_down};
Uint8 kdpat0[8] = {9, 0, 1, 2, 3, 4, 5, 255};
Uint8 *kdai6pchg[1] = {kdpat0};
sprite_pattern kdai6pat0 = {1, {{-24, -16, 0, 314}}};
sprite_pattern kdai6pat1 = {1, {{-24, -16, 0, 315}}};
sprite_pattern kdai6pat2 = {1, {{-24, -16, 0, 316}}};
sprite_pattern kdai6pat3 = {1, {{-24, -16, 0, 317}}};
sprite_pattern kdai6pat4 = {1, {{-24, -16, 0, 318}}};
sprite_pattern kdai6pat5 = {1, {{-24, -16, 0, 319}}};
sprite_pattern *kdai6pat[6] = {&kdai6pat0, &kdai6pat1, &kdai6pat2,
                               &kdai6pat3, &kdai6pat4, &kdai6pat5};

void k_move(sprite_status *pActwk) {
    sprite_status *pPlaywk;
    player_work *player;

    pPlaywk = &actwk[0];
    player = player_work_get(pPlaywk);
    if (pPlaywk->xposi.w.h - pActwk->xposi.w.h < 0) {

        if (swdata.b.h & 4) {
            ++player->orbit_radius;
            return;
        }
        if (swdata.b.h & 8) {
            --player->orbit_radius;
            if ((char)player->orbit_radius < 0)
                player->orbit_radius = 0;
        }
    } else {

        if (swdata.b.h & 8) {
            ++player->orbit_radius;
            return;
        }
        if (swdata.b.h & 4) {
            --player->orbit_radius;
            if ((char)player->orbit_radius < 0)
                player->orbit_radius = 0;
        }
    }
}

void jumpchk_d(sprite_status *pActwk) {
    sprite_status *pPlaywk;
    player_work *player;
    Sint16 iSin, iCos;
    Uint8 byD0;
    int_union lD1;

    pPlaywk = &actwk[0];
    player = player_work_get(pPlaywk);
    byD0 = pActwk->actno;
    if (!(swdata.b.l & 112))
        return;
    player->status_flags = 0;

    byD0 = pPlaywk->direc.b.h;
    byD0 -= 64;
    sinset(byD0, &iSin, &iCos);
    lD1.l = iCos * 1664;
    lD1.l >>= 8;
    pPlaywk->xspeed.w += lD1.w.l;
    lD1.l = iSin * 1664;
    lD1.l >>= 8;
    pPlaywk->yspeed.w += lD1.w.l;

    pPlaywk->cddat |= 2;
    pPlaywk->cddat &= 223;
    player->jump_started = 1;
    player->jump_lock = 0;

    if (chibi_flag != 0) {
        pPlaywk->sprvsize = 10;
        pPlaywk->sprhs = 5;
    } else {

        pPlaywk->sprvsize = 19;
        pPlaywk->sprhs = 9;
    }

    if (pPlaywk->cddat & 4) {
        pPlaywk->cddat |= 16;
        return;
    }
    if (chibi_flag != 0) {
        pPlaywk->sprvsize = 10;
        pPlaywk->sprhs = 5;
    } else {

        pPlaywk->sprvsize = 14;
        pPlaywk->sprhs = 7;
        pPlaywk->yposi.w.h += 5;
    }

    pPlaywk->cddat |= 4;
    pPlaywk->mstno.b.h = 2;
}

void dai6_move(sprite_status *pActwk) {
    move_tbl[pActwk->userflag.b.h](pActwk);
}

void kd_mv_up(sprite_status *pActwk) {
    Sint16 iD0;
    kdai6_work *work = kdai6_get_work(pActwk);

    iD0 = kd_dair6_sub2(pActwk);
    iD0 = -iD0 + work->origin_y;
    pActwk->yposi.w.h = iD0;
}

void kd_mv_down(sprite_status *pActwk) {
    Sint16 iD0;
    kdai6_work *work = kdai6_get_work(pActwk);

    iD0 = kd_dair6_sub2(pActwk);
    pActwk->yposi.w.h = iD0 + work->origin_y;
}

void kd_mv_right(sprite_status *pActwk) {
    Sint16 iD0;
    Sint32 lXsv;
    int_union lD0;
    kdai6_work *work = kdai6_get_work(pActwk);

    lXsv = pActwk->xposi.l;
    iD0 = kd_dair6_sub2(pActwk);
    pActwk->xposi.w.h = iD0 + work->origin_x;

    lD0.l = pActwk->xposi.l - lXsv;
    lD0.l >>= 8;
    pActwk->xspeed.w = lD0.w.l;
}

void kd_mv_left(sprite_status *pActwk) {
    Sint16 iD0;
    Sint32 lXsv;
    int_union lD0;
    kdai6_work *work = kdai6_get_work(pActwk);

    lXsv = pActwk->xposi.l;
    iD0 = kd_dair6_sub2(pActwk);
    pActwk->xposi.w.h = -iD0 + work->origin_x;

    lD0.l = pActwk->xposi.l - lXsv;
    lD0.l >>= 8;
    pActwk->xspeed.w = lD0.w.l;
}

Sint16 kd_dair6_sub2(sprite_status *pActwk) {
    Sint16 iSin;
    Sint16 iCos;
    Uint16 wD0;
    kdai6_work *work = kdai6_get_work(pActwk);

    wD0 = gametimer.w & 255;
    sinset(wD0, &iSin, &iCos);

    iSin += iSin;
    iSin += iSin;
    iSin /= 16;

    ++work->wobble_counter;
    return iSin;
}
