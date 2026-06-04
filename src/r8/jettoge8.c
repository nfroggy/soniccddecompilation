#include "../equ.h"
#include "jettoge8.h"
#include "../action.h"
#include "../actset.h"
#include "../playsub.h"

#if defined(R83)
#define SPRITE_JETTOGE8_BASE 505
#else
#define SPRITE_JETTOGE8_BASE 475
#endif

typedef struct {
    Sint16 turn_timer;
    Sint32 x_velocity;
    Sint16 origin_x;
    Uint8 saved_colino;
} jettoge8_work;

static jettoge8_work *jettoge8_get_work(sprite_status *actionwk) {
    return (jettoge8_work *)actionwk->actfree;
}

static char p00[4] = {1, 0, 1, -1};
static char *pchg[1] = {p00};
static sprite_pattern pat00 = {1, {{-16, -76, 0, SPRITE_JETTOGE8_BASE}}};
static sprite_pattern pat01 = {1, {{-16, -76, 0, SPRITE_JETTOGE8_BASE + 1}}};
sprite_pattern *pat_jettoge[2] = {&pat00, &pat01};

void jettoge(sprite_status *actionwk) {
    switch (actionwk->r_no0) {
    case 0:
        a_init(actionwk);
    case 2:
        a_move(actionwk);
        break;
    }
}

void a_init(sprite_status *actionwk) {
    jettoge8_work *work = jettoge8_get_work(actionwk);

    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->patbase = pat_jettoge;
    actionwk->sproffset = 33856;
    work->origin_x = actionwk->xposi.w.h;
    work->saved_colino = 177;
    work->x_velocity = 20480;
    actionwk->sprvsize = 76;
    actionwk->sprpri = 1;
    actionwk->sprhsize = 16;
}

void a_move(sprite_status *actionwk) {
    jettoge8_work *work = jettoge8_get_work(actionwk);
    Sint16 t;

    actionwk->xposi.l += work->x_velocity;
    if (prio_flag == 0)
        actionwk->colino = work->saved_colino;
    else
        actionwk->colino = 0;
    t = work->turn_timer--;
    if (t <= 0) {
        work->turn_timer = 409;
        work->x_velocity = -work->x_velocity;

        actionwk->actflg ^= 1;
        actionwk->cddat ^= 1;
    }
    patchg(actionwk, (Uint8 **)pchg);
    actionsub(actionwk);
    frameout_s00(actionwk, work->origin_x);
}
