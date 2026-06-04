#include "../equ.h"
#include "walls.h"
#include "../action.h"
#include "../actset.h"
#include "../ridechk.h"

#if defined(R41A) || defined(R42A)
#define SPRITE_WALLS_BASE 446
#elif defined(R41B)
#define SPRITE_WALLS_BASE 466
#elif defined(R42B)
#define SPRITE_WALLS_BASE 462
#elif defined(R42C) || defined(R42D)
#define SPRITE_WALLS_BASE 461
#else
#define SPRITE_WALLS_BASE 477
#endif

static void main_init(sprite_status *pActwk);
static void main_move(sprite_status *pActwk);
static void opt_check(sprite_status *pActwk);

typedef struct {
    Sint16 timer;
    Sint32 x_velocity;
    Sint16 origin_x;
    Sint16 parent_actor;
    Sint16 data_index;
    Sint16 child_actors[8];
} walls_work;

static walls_work *walls_work_get(sprite_status *pActwk) {
    return (walls_work *)pActwk->actfree;
}

static sprite_pattern pat00 = {1, {{-16, -16, 0, SPRITE_WALLS_BASE}}};
static sprite_pattern pat01 = {1, {{-32, -16, 0, SPRITE_WALLS_BASE + 1}}};
static sprite_pattern pat02 = {1, {{-48, -16, 0, SPRITE_WALLS_BASE + 2}}};
static sprite_pattern pat03 = {1, {{-64, -16, 0, SPRITE_WALLS_BASE + 3}}};
static sprite_pattern pat04;
sprite_pattern *pat_walls[5] = {&pat00, &pat01, &pat02, &pat03, &pat04};
static Uint8 patno_tbl[9][8] = {
    {2, 1, 1, 2, 2, 1, 1, 2}, {0, 3, 1, 2, 2, 1, 3, 0},
    {3, 0, 0, 3, 3, 0, 0, 3}, {1, 2, 0, 0, 0, 0, 2, 1},
    {2, 1, 1, 2, 1, 2, 2, 1}, {0, 3, 2, 0, 2, 0, 0, 3},
    {3, 0, 0, 0, 0, 0, 0, 3}, {2, 1, 1, 2, 3, 0, 0, 3},
    {1, 2, 1, 1, 1, 1, 2, 1}};
static Sint32 data_tbl[8] = {60, 0, 48, 131072, 60, 0, 192, -32768};

void walls(sprite_status *pActwk) {
    if (pActwk->userflag.b.h & 128) {

        opt_check(pActwk);
        return;
    } else if (!pActwk->r_no0) {
        main_init(pActwk);
    }

    main_move(pActwk);
}

static void main_init(sprite_status *pActwk) {
    walls_work *work = walls_work_get(pActwk);
    Sint16 *pOptwk;
    Uint8 *pPatno, patnowk;
    Sint32 i, j;
    sprite_status *pNewact;

    pActwk->r_no0 += 2;
    work->origin_x = pActwk->xposi.w.h;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->sprhs = 16;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 16;
    pActwk->sproffset = 17514;
    pActwk->patbase = pat_walls;
    pActwk->patno = 4;
    work->parent_actor = (Sint16)(pActwk - actwk);
    work->timer = 192;
    work->x_velocity = -32768;

    pOptwk = work->child_actors;
    pPatno = patno_tbl[pActwk->userflag.b.h + 1];

    for (i = 3; i >= 0; --i) {

        for (j = 1; j >= 0; --j) {

            if (actwkchk2(pActwk, &pNewact) != 0) {
                frameout(pActwk);
                return;
            }

            *pOptwk++ = (Sint16)(pNewact - actwk);
            pNewact->userflag.b.h = (j - 2) & 255;
            pNewact->yposi.w.h = pActwk->yposi.w.h - 48 + (Sint16)(i * 32);

            pNewact->patno = patnowk = *--pPatno;
            pNewact->sprhsize = pNewact->sprhs = (patnowk + 1) * 16;
            if (j != 0) {
                pNewact->xposi.w.h =
                    pActwk->xposi.w.h + 64 - (Sint16)(patnowk * 16);

            } else {
                pNewact->xposi.w.h =
                    pActwk->xposi.w.h - 64 + (Sint16)(patnowk * 16);
            }

            walls_work_get(pNewact)->parent_actor = (Sint16)(pActwk - actwk);
            pNewact->actno = pActwk->actno;
            pNewact->actflg = pActwk->actflg;
            pNewact->sprpri = pActwk->sprpri;
            pNewact->sprvsize = pActwk->sprvsize;
            pNewact->sproffset = pActwk->sproffset;
            pNewact->patbase = pActwk->patbase;
        }
    }

    main_move(pActwk);
}

static void main_move(sprite_status *pActwk) {
    walls_work *work = walls_work_get(pActwk);
    Sint16 *pOptwk;
    Sint32 i;
    Sint32 spdwk;
    sprite_status *pChildact;

    pOptwk = work->child_actors;
    spdwk = work->x_velocity;

    for (i = 3; i >= 0; --i) {

        pChildact = &actwk[*pOptwk++];
        pChildact->xposi.l -= spdwk;
        pChildact->xspeed.w = -(spdwk >> 8);

        pChildact = &actwk[*pOptwk++];
        pChildact->xposi.l += spdwk;
        pChildact->xspeed.w = spdwk >> 8;
    }

    if (--work->timer == 0) {
        work->timer = data_tbl[work->data_index / 4];

        work->x_velocity = data_tbl[work->data_index / 4 + 1];

        work->data_index += 8;
        work->data_index &= 31;
    }

    frameout_s00(pActwk, work->origin_x);
}

static void opt_check(sprite_status *pActwk) {
    walls_work *work = walls_work_get(pActwk);

    if (actwk[work->parent_actor].actno != 51) {
        frameout(pActwk);
    } else {
        hitchk(pActwk, &actwk[0]);
        actionsub(pActwk);
    }
}
