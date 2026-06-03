#include <stddef.h>

#include "../equ.h"
#include "tekkyu7.h"
#include "../action.h"
#include "../actset.h"

#if defined(R71) || defined(R72)
#define SPRITE_TEKKYU7_BASE 427
#elif defined(R73C) || defined(R73D)
#define SPRITE_TEKKYU7_BASE 410
#else
#define SPRITE_TEKKYU7_BASE 462
#endif

static void tekkyu7_init(sprite_status *pActwk);
static void tekkyu7_move(sprite_status *pActwk);
static void tekkyu7_stop(sprite_status *pActwk);

#pragma pack(push, 1)
typedef struct {
    Sint16 origin_x;
    Sint16 move_duration;
    Sint16 stop_duration;
    Sint16 counter;
    Sint32 x_velocity;
    Sint32 y_velocity;
} tekkyu7_work;
#pragma pack(pop)

_Static_assert(offsetof(tekkyu7_work, origin_x) == 0,
               "tekkyu7_work.origin_x offset");
_Static_assert(offsetof(tekkyu7_work, move_duration) == 2,
               "tekkyu7_work.move_duration offset");
_Static_assert(offsetof(tekkyu7_work, stop_duration) == 4,
               "tekkyu7_work.stop_duration offset");
_Static_assert(offsetof(tekkyu7_work, counter) == 6,
               "tekkyu7_work.counter offset");
_Static_assert(offsetof(tekkyu7_work, x_velocity) == 8,
               "tekkyu7_work.x_velocity offset");
_Static_assert(offsetof(tekkyu7_work, y_velocity) == 12,
               "tekkyu7_work.y_velocity offset");
_Static_assert(sizeof(tekkyu7_work) <= sizeof(((sprite_status *)0)->actfree),
               "tekkyu7_work fits in actfree");

static tekkyu7_work *tekkyu7_get_work(sprite_status *pActwk) {
    return (tekkyu7_work *)pActwk->actfree;
}

static sprite_pattern tekkyu7_pat0 = {1, {{-16, -16, 0, SPRITE_TEKKYU7_BASE}}};
sprite_pattern *pat_tekkyu7[1] = {&tekkyu7_pat0};

void tekkyu7(sprite_status *pActwk) {
    tekkyu7_work *work = tekkyu7_get_work(pActwk);
    void (*tekkyu7_acttbl[3])(sprite_status *) = {&tekkyu7_init, &tekkyu7_move,
                                                  &tekkyu7_stop};

    tekkyu7_acttbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s00(pActwk, work->origin_x);
}

void tekkyu7_init(sprite_status *pActwk) {
    tekkyu7_work *work = tekkyu7_get_work(pActwk);
    Sint16 tekkyu7_tbl0[24] = {1024, 0, 24, 12, 0, 1024, 16, 8,
                               512,  0, 48, 24, 0, 512,  32, 16,
                               256,  0, 96, 48, 0, 256,  64, 32};

    Sint16 tekkyu7_tbl1[3] = {60, 40, 20};

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->colino = 181;
    pActwk->sprhs = 16;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 16;
    pActwk->sproffset = 902;
    pActwk->patbase = pat_tekkyu7;
    work->origin_x = pActwk->xposi.w.h;
    work->stop_duration = tekkyu7_tbl1[time_flag];

    work->x_velocity =
        tekkyu7_tbl0[time_flag * 8 + pActwk->userflag.b.h * 4] << 8;
    work->y_velocity =
        tekkyu7_tbl0[time_flag * 8 + pActwk->userflag.b.h * 4 + 1] << 8;
    work->move_duration =
        tekkyu7_tbl0[time_flag * 8 + pActwk->userflag.b.h * 4 + 2];
    work->counter =
        tekkyu7_tbl0[time_flag * 8 + pActwk->userflag.b.h * 4 + 3];

    tekkyu7_move(pActwk);
}

void tekkyu7_move(sprite_status *pActwk) {
    tekkyu7_work *work = tekkyu7_get_work(pActwk);

    pActwk->xposi.l += work->x_velocity;
    pActwk->yposi.l += work->y_velocity;
    if (!(--work->counter)) {
        pActwk->r_no0 += 2;
        work->counter = work->stop_duration;
        work->x_velocity *= -1;
        work->y_velocity *= -1;
    }
}

void tekkyu7_stop(sprite_status *pActwk) {
    tekkyu7_work *work = tekkyu7_get_work(pActwk);

    if (!(--work->counter)) {
        pActwk->r_no0 -= 2;
        work->counter = work->move_duration;
    }
}
