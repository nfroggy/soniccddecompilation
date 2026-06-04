#include "../equ.h"
#include "screw_a.h"
#include "../action.h"
#include "../actset.h"
#include "playsub4.h"

#if defined(R41A) || defined(R42A)
#define SPRITE_SCREWA_BASE 457
#elif defined(R41B)
#define SPRITE_SCREWA_BASE 477
#elif defined(R42B)
#define SPRITE_SCREWA_BASE 528
#else
#define SPRITE_SCREWA_BASE 488
#endif

static Uint8 pchg_00[7] = {1, 0, 1, 2, 3, 4, 255};
static Uint8 *pchg[1] = {pchg_00};

typedef struct {
    Uint8 switch_latched;
    Uint8 animation_enabled;
} screw_a_work;

static screw_a_work *screw_a_work_get(sprite_status *pActwk) {
    return (screw_a_work *)pActwk->actfree;
}

static sprite_pattern screw0_pat00 = {1, {{-16, -8, 0, SPRITE_SCREWA_BASE}}};
static sprite_pattern screw0_pat01 = {1,
                                      {{-12, -8, 0, SPRITE_SCREWA_BASE + 1}}};
static sprite_pattern screw0_pat02 = {1, {{-8, -8, 0, SPRITE_SCREWA_BASE + 2}}};
static sprite_pattern screw0_pat03 = {1,
                                      {{-12, -8, 0, SPRITE_SCREWA_BASE + 3}}};
static sprite_pattern screw0_pat04 = {1,
                                      {{-16, -8, 0, SPRITE_SCREWA_BASE + 4}}};
sprite_pattern *pat_screw0[5] = {&screw0_pat00, &screw0_pat01, &screw0_pat02,
                                 &screw0_pat03, &screw0_pat04};
static sprite_pattern screw1_pat00 = {1,
                                      {{-8, -16, 0, SPRITE_SCREWA_BASE + 5}}};
static sprite_pattern screw1_pat01 = {1,
                                      {{-8, -12, 0, SPRITE_SCREWA_BASE + 6}}};
static sprite_pattern screw1_pat02 = {1, {{-8, -8, 0, SPRITE_SCREWA_BASE + 7}}};
static sprite_pattern screw1_pat03 = {1,
                                      {{-8, -12, 0, SPRITE_SCREWA_BASE + 8}}};
static sprite_pattern screw1_pat04 = {1,
                                      {{-8, -16, 0, SPRITE_SCREWA_BASE + 9}}};
sprite_pattern *pat_screw1[5] = {&screw1_pat00, &screw1_pat01, &screw1_pat02,
                                 &screw1_pat03, &screw1_pat04};

void screw(sprite_status *pActwk) {
    screw_a_work *work = screw_a_work_get(pActwk);

    if (!pActwk->r_no0) {

        pActwk->r_no0 += 2;
        pActwk->actflg |= 4;
        pActwk->sprpri = 3;
        pActwk->sproffset = 17280;

        if (pActwk->userflag.b.h & 128) {
            work->animation_enabled = 255;
        }

        if (!(pActwk->userflag.b.h & 2)) {
            pActwk->patbase = pat_screw0;
            pActwk->sprhsize = 16;
            pActwk->sprvsize = 8;
        } else {
            pActwk->patbase = pat_screw1;
            pActwk->sprhsize = 8;
            pActwk->sprvsize = 16;
        }

        if (pActwk->userflag.b.h & 1) {
            pActwk->actflg |= 3;
            pActwk->cddat |= 3;
        }
    }

    if (!(pActwk->userflag.b.l & 128) && !work->switch_latched &&
        switchflag[pActwk->userflag.b.l]) {

        work->switch_latched = 1;
        work->animation_enabled = ~work->animation_enabled;
    }

    if (!work->animation_enabled) {
        pActwk->patno = 0;
    } else {
        patchg(pActwk, pchg);
    }
    actionsub(pActwk);
    frameout_s(pActwk);
}
