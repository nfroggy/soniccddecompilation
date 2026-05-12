#include "../equ.h"
#include "bobinb.h"
#include "../action.h"
#include "../playsub.h"
#include "../score.h"

static void a_init(sprite_status *bobinwk);
static void a_move(sprite_status *bobinwk);
static void a_die(sprite_status *bobinwk);

static Uint8 pat00[7] = {2, 0, 0, 0, 1, 1, 252};
static Uint8 *pchg[1] = {pat00};
static sprite_pattern bbb00 = {1, {{-16, -16, 0, 313}}};
static sprite_pattern bbb01 = {1, {{-16, -16, 0, 314}}};
sprite_pattern *pat_bobinbreak[2] = {&bbb00, &bbb01};

void bobinbreak(sprite_status *bobinwk) {
    void (*tbl[3])(sprite_status *) = {&a_init, &a_move, &a_die};

    tbl[bobinwk->r_no0 / 2](bobinwk);
    actionsub(bobinwk);
}

static void a_init(sprite_status *bobinwk) {
    bobinwk->r_no0 += 2;
    bobinwk->actflg |= 4;
    bobinwk->sprpri = 3;
    bobinwk->sprhs = 16;
    bobinwk->sprhsize = 16;
    bobinwk->sprvsize = 16;

    bobinwk->patbase = pat_bobinbreak;

    scoreup(10);
    tensuu0(bobinwk, 0);
}

static void a_move(sprite_status *bobinwk) { patchg(bobinwk, pchg); }

static void a_die(sprite_status *bobinwk) { frameout(bobinwk); }
