#include <stddef.h>

#include "../equ.h"
#include "sw8.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../ridechk.h"

#if defined(R82)
#define SPRITE_SW8_BASE 315
#elif defined(R83)
#define SPRITE_SW8_BASE 308
#else
#define SPRITE_SW8_BASE 306
#endif

static void act_init(sprite_status *swwk);
static void act_move(sprite_status *swwk);

static sprite_pattern sw00 = {
    2, {{-16, -8, 0, SPRITE_SW8_BASE}, {0, -8, 8, SPRITE_SW8_BASE}}};
static sprite_pattern sw01 = {
    2, {{-16, -4, 0, SPRITE_SW8_BASE + 1}, {0, -4, 8, SPRITE_SW8_BASE + 1}}};
sprite_pattern *pat_sw[2] = {&sw00, &sw01};

#pragma pack(push, 1)
typedef struct {
    Uint8 reserved0[18];
    Sint16 switch_index;
    Uint8 previous_pressed;
    Uint8 current_pressed;
} sw8_work;
#pragma pack(pop)

_Static_assert(offsetof(sw8_work, switch_index) == 18,
               "sw8_work.switch_index must map to offset 18");
_Static_assert(offsetof(sw8_work, previous_pressed) == 20,
               "sw8_work.previous_pressed must map to offset 20");
_Static_assert(offsetof(sw8_work, current_pressed) == 21,
               "sw8_work.current_pressed must map to offset 21");
_Static_assert(sizeof(sw8_work) <= sizeof(((sprite_status *)0)->actfree),
               "sw8_work must fit in sprite_status.actfree");

static sw8_work *sw8_work_get(sprite_status *swwk) {
    return (sw8_work *)swwk->actfree;
}

void sw(sprite_status *swwk) {
    if (swwk->r_no0 == 0)
        act_init(swwk);
    else
        act_move(swwk);
}

static void act_init(sprite_status *swwk) {
    sw8_work *work = sw8_work_get(swwk);

    swwk->r_no0 += 2;
    swwk->actflg |= 4;
    swwk->sprpri = 1;
    swwk->sprhs = swwk->sprhsize = 16;

    swwk->patbase = pat_sw;
    swwk->sprvsize = 8;

    work->switch_index = swwk->userflag.b.h;
}

static void act_move(sprite_status *swwk) {
    sw8_work *work = sw8_work_get(swwk);
    Sint16 switch_adr;

    work->previous_pressed = work->current_pressed;
    switch_adr = work->switch_index;
    if (hitchk(swwk, &actwk[0]) && actwk[0].yposi.w.h <= swwk->yposi.w.h) {
        work->current_pressed = 255;
        switchflag[switch_adr] |= 192;
    } else {
        work->current_pressed = 0;
        switchflag[switch_adr] &= 127;
    }

    if (work->previous_pressed == 0 && work->current_pressed == 255) {

        if ((char)swwk->actflg < 0)
            soundset(191);

        switchflag[switch_adr] ^= 32;
        actwk[0].yposi.w.h += 8;
        swwk->yposi.w.h += 4;
        ++swwk->patno;
        swwk->sprvsize -= 4;
    }

    if (work->previous_pressed == 255 && work->current_pressed == 0) {

        actwk[0].yposi.w.h -= 12;
        swwk->yposi.w.h -= 4;
        --swwk->patno;
        swwk->sprvsize += 4;
    }

    actionsub(swwk);
    frameout_s(swwk);
}
