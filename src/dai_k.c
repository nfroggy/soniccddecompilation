#include "equ.h"
#include "dai_k.h"
#include "action.h"
#include "actset.h"
#include "etc.h"
#include "loader2.h"
#include "player_work.h"
#include "playsub.h"
#include "ridechk.h"

static void dai_k_init(sprite_status *pActwk);
static void dai_k_move(sprite_status *pActwk);
static void k_move(sprite_status *pActwk, sprite_status *pSonicwk);
static void jumpchk_d(sprite_status *pActwk, sprite_status *pSonicwk);

#if defined(R11A)
#define SPRITE_DAIK_BASE 495
#else
#define SPRITE_DAIK_BASE 478
#endif

static sprite_pattern k_daipat0 = {1, {{-16, -8, 0, SPRITE_DAIK_BASE}}};
static sprite_pattern k_daipat1 = {1, {{-16, -8, 0, SPRITE_DAIK_BASE + 1}}};
static sprite_pattern k_daipat2 = {1, {{-16, -8, 0, SPRITE_DAIK_BASE + 2}}};
sprite_pattern *k_daipat[3] = {&k_daipat0, &k_daipat1, &k_daipat2};
static char k_daichg0[5] = {1, 0, 1, 2, -1};
static char *k_daichg = k_daichg0;
static void (*dai_k_move_tbl[2])(sprite_status *) = {&dai_k_init, &dai_k_move};

void dai_k(
    sprite_status *pActwk) { /* Line 127, Address: 0x1002090, Func Offset */
    dai_k_move_tbl[pActwk->r_no0 / 2](
        pActwk); /* Line 128, Address: 0x100209c, Func Offset */

    if (!time_stop) { /* Line 130, Address: 0x10020e0, Func Offset */
        patchg(pActwk, (Uint8 **)&k_daichg); /* Line 131, Address: 0x10020f0,
                                                Func Offset */
    }

    actionsub(pActwk);  /* Line 134, Address: 0x1002104, Func Offset */
    frameout_s(pActwk); /* Line 135, Address: 0x1002110, Func Offset */
} /* Line 136, Address: 0x100211c, Func Offset */

static void dai_k_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 4;
    pActwk->patbase = k_daipat;

    pActwk->sprhsize = 16;
    pActwk->sprvsize = 8;
    dai_k_move(pActwk);
}

static void dai_k_move(sprite_status *pActwk) {
    Sint16 lenwk, sinwk, coswk;
    player_work *player = player_work_get(&actwk[0]);
    static Uint8 pattbl[16] = {0, 0, 0, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 5};

    if (!(pActwk->actflg & 128))
        return;

    if (!ride_on_chk(pActwk, actwk))
        return;

    if (!(player->status_flags & 1)) {
        player->status_flags |= 1;
        actwk[0].mstno.b.h = 45;
        lenwk = actwk[0].xposi.w.h - pActwk->xposi.w.h;

        if (lenwk >= 0) {
            player->special_angle = 0;
        } else {
            player->special_angle = 128;
            lenwk = -lenwk;
        }

        player->orbit_radius = lenwk;
    }

    if (actwk[0].r_no0 >= 6)
        return;

    player->special_angle += 8;
    sinset(player->special_angle, &sinwk, &coswk);
    actwk[0].xposi.w.h =
        pActwk->xposi.w.h + ((Uint32)(player->orbit_radius * coswk) >> 8);

    actwk[0].patcnt = pattbl[player->special_angle >> 4];

    if (!(player->special_angle & 63)) {
        ++player->orbit_radius;
    }

    swdata = swdata1;
    k_move(pActwk, &actwk[0]);
    jumpchk_d(pActwk, &actwk[0]);
}

static void k_move(sprite_status *pActwk, sprite_status *pSonicwk) {
    player_work *player = player_work_get(pSonicwk);

    if (pSonicwk->xposi.w.h >= pActwk->xposi.w.h) {

        if (swdata.b.h & 8) {
            ++player->orbit_radius;
        } else if (swdata.b.h & 4) {
            if (player->orbit_radius) {
                --player->orbit_radius;
            }
        }
    } else {

        if (swdata.b.h & 4) {
            ++player->orbit_radius;
        } else if (swdata.b.h & 8) {
            if (player->orbit_radius) {
                --player->orbit_radius;
            }
        }
    }
}

static void jumpchk_d(sprite_status *pActwk, sprite_status *pSonicwk) {
    Sint16 jumpwk, sinwk, coswk;
    player_work *player = player_work_get(pSonicwk);

    if (!(swdata.b.l & 112))
        return;

    player->status_flags = 0;
    if (pActwk->cddat & 64) {
        jumpwk = 896;
    } else {
        jumpwk = 1664;
    }

    sinset(pSonicwk->direc.b.h - 64, &sinwk, &coswk);
    pSonicwk->xspeed.w += (coswk * jumpwk) >> 8;
    pSonicwk->yspeed.w += (sinwk * jumpwk) >> 8;

    pSonicwk->cddat |= 2;
    pSonicwk->cddat &= 223;
    player->jump_started = 1;
    player->jump_lock = 0;

    soundset(160);

    if (pSonicwk->cddat & 4) {
        if (chibi_flag) {
            pSonicwk->sprvsize = 10;
            pSonicwk->sprhs = 5;
        } else {
            pSonicwk->sprvsize = 19;
            pSonicwk->sprhs = 9;
        }

        pSonicwk->cddat |= 16;
    } else {
        if (chibi_flag) {
            pSonicwk->sprvsize = 10;
            pSonicwk->sprhs = 5;
        } else {
            pSonicwk->sprvsize = 14;
            pSonicwk->sprhs = 7;
        }

        pSonicwk->cddat |= 4;
        pSonicwk->mstno.b.h = 2;
    }
}
