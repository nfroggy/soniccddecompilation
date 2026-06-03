#include <stddef.h>

#include "../equ.h"
#include "kumo.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../loader2.h"
#include "../player_work.h"
#include "../playsub.h"
#include "../suicide.h"

#pragma pack(push, 1)
typedef struct {
    Sint16 timer;
    Uint8 **change_table;
    Sint32 initial_y_speed;
    Sint32 velocity;
} kumo_work;
#pragma pack(pop)

_Static_assert(offsetof(kumo_work, timer) == 0, "kumo_work.timer offset");
_Static_assert(offsetof(kumo_work, change_table) == 2,
               "kumo_work.change_table offset");
_Static_assert(offsetof(kumo_work, initial_y_speed) == 6,
               "kumo_work.initial_y_speed offset");
_Static_assert(offsetof(kumo_work, velocity) == 10,
               "kumo_work.velocity offset");
_Static_assert(sizeof(kumo_work) <= sizeof(((sprite_status *)0)->actfree),
               "kumo_work must fit in sprite_status.actfree");

static kumo_work *kumo_get_work(sprite_status *pActwk) {
    return (kumo_work *)pActwk->actfree;
}

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
    kumo_work *work = kumo_get_work(pActwk);

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
        work->change_table = pchg_e;
        work->initial_y_speed = -524288;
    } else {
        pActwk->patbase = pat_kumo_b;
        work->change_table = pchg_b;
        work->initial_y_speed = -327680;
    }

    if (pActwk->userflag.b.l) {
        pActwk->actflg |= 1;
        pActwk->cddat |= 1;
    }

    pActwk->r_no0 += 2;
    kumo_jump1(pActwk);
}

static void kumo_jump(sprite_status *pActwk) {
    kumo_work *work = kumo_get_work(pActwk);

    pActwk->r_no0 += 2;
    pActwk->patno = 0;
    pActwk->yposi.w.h -= 8;
    work->velocity = work->initial_y_speed;
    kumo_jump1(pActwk);
}

static void kumo_jump1(sprite_status *pActwk) {
    Sint16 wD1;
    kumo_work *work = kumo_get_work(pActwk);

    work->velocity += 16384;
    pActwk->yposi.l += work->velocity;
    if ((wD1 = emycol_d(pActwk)) < 0) {
        pActwk->yposi.w.h += wD1;
        pActwk->r_no0 += 2;
        pActwk->patno = 1;
        pActwk->yposi.w.h += 8;
    }
}

static void kumo_move(sprite_status *pActwk) {
    kumo_work *work = kumo_get_work(pActwk);

    pActwk->r_no0 += 2;
    pActwk->colino = 54;
    work->timer = 180;
    kumo_move1(pActwk);
}

static void kumo_move1(sprite_status *pActwk) {
    kumo_work *work = kumo_get_work(pActwk);

    if (!pActwk->userflag.b.h) {
        if (kumo_check(pActwk, &actwk[0])) {
            pActwk->r_no0 += 2;
            pActwk->patno = 1;
            work->timer = 60;
            return;
        }
    }

    if (--work->timer == 0) {
        pActwk->r_no0 -= 6;
    }
    patchg(pActwk, work->change_table);
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
    kumo_work *work = kumo_get_work(pActwk);

    if (!(--work->timer)) {
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
        kumo_get_work(pNewactwk)->velocity = lD1;
        if (pActwk->actflg & 128) {
            soundset(160);
        }
    }
    pActwk->r_no0 -= 10;
}

static void tama(sprite_status *pActwk) {
    kumo_work *work = kumo_get_work(pActwk);
    player_work *player = player_work_get(&actwk[0]);

    if (!work->timer) {
        if (pActwk->colicnt) {
            pActwk->colicnt = 0;
            if (actwk[0].r_no0 != 4) {
                if (actwk[0].r_no0 != 6) {
                    if (!player->damage_invulnerability_timer) {

                        work->timer = 120;
                        goto label1;
                    }
                }
            }
        }

        pActwk->xposi.l += work->velocity;
        patchg(pActwk, pchg_tama);
        actionsub(pActwk);
        frameout_s(pActwk);
        return;
    }

label1:
    if (!(--work->timer)) {
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
