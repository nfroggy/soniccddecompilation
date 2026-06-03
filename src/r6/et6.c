#include <stddef.h>

#include "../equ.h"
#include "et6.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../ridechk.h"
#include "../score.h"

#pragma pack(push, 1)
typedef struct {
    union {
        Uint8 explosion_timer;
        Uint8 reset_timer;
        Uint16 hover_counter;
        Sint16 hover_counter_signed;
    };
    Sint16 explosion_table_offset;
    Uint8 unused4[2];
    Sint16 origin_y;
    Sint16 hover_direction;
} et6_work;
#pragma pack(pop)

_Static_assert(offsetof(et6_work, explosion_timer) == 0,
               "et6_work.explosion_timer offset");
_Static_assert(offsetof(et6_work, hover_counter) == 0,
               "et6_work.hover_counter offset");
_Static_assert(offsetof(et6_work, explosion_table_offset) == 2,
               "et6_work.explosion_table_offset offset");
_Static_assert(offsetof(et6_work, origin_y) == 6,
               "et6_work.origin_y offset");
_Static_assert(offsetof(et6_work, hover_direction) == 8,
               "et6_work.hover_direction offset");
_Static_assert(sizeof(et6_work) <= sizeof(((sprite_status *)0)->actfree),
               "et6_work fits in actfree");

static et6_work *et6_get_work(sprite_status *actionwk) {
    return (et6_work *)actionwk->actfree;
}

static void m_init(sprite_status *actionwk);
static void m_wait(sprite_status *actionwk);

static sprite_pattern tpat00 = {1, {{-40, 15, 0, 419}}};
static sprite_pattern tpat01 = {1, {{-40, 16, 0, 420}}};
static sprite_pattern tpat07 = {1, {{0, 0, 0, 0}}};
static sprite_pattern tpat02 = {2, {{-24, -32, 0, 426}, {-40, -32, 0, 427}}};
static sprite_pattern tpat03 = {
    3, {{-22, -8, 0, 428}, {-24, -32, 0, 426}, {-40, -32, 0, 427}}};
static sprite_pattern tpat04 = {4,
                                {{-22, -8, 0, 429},
                                 {-24, -32, 0, 432},
                                 {-24, -32, 0, 426},
                                 {-40, -32, 0, 427}}};
static sprite_pattern tpat05 = {4,
                                {{-22, -8, 0, 430},
                                 {-24, -32, 0, 433},
                                 {-24, -32, 0, 426},
                                 {-40, -32, 0, 427}}};
static sprite_pattern tpat06 = {4,
                                {{-22, -8, 0, 431},
                                 {-24, -32, 0, 434},
                                 {-24, -32, 0, 426},
                                 {-40, -32, 0, 427}}};
sprite_pattern *pat_et[8] = {&tpat00, &tpat01, &tpat02, &tpat03,
                             &tpat04, &tpat05, &tpat06, &tpat07};
static Uint16 tbl0sproffset[11] = {1087, 1033, 0, 0, 1108, 1024, 0, 0, 0, 0, 0};
static char tbl0[64] = {
    1,   0,  0,   2,   -40, -20, 3,   28, 10, 4,   18,  -18, 5,   -18, -10, 6,
    8,   -8, 8,   -18, 14,  10,  -10, 10, 12, 30,  -10, 15,  0,   -18, 18,  20,
    -10, 20, -10, 18,  22,  8,   23,  25, 13, -10, 26,  23,  -22, 28,  -3,  -25,
    30,  10, 20,  32,  -10, 2,   34,  30, -8, 35,  13,  -10, 40,  -10, 10,  -1};
static char pchg0[6] = {3, 3, 4, 5, 6, -1};
static char *pchg[1] = {pchg0};

void et(sprite_status *actionwk) {
    switch (actionwk->r_no0) {

    case 0:
        m_init(actionwk);
    case 2:
        m_wait(actionwk);
        break;
    case 4:
        m_die(actionwk);
        break;
    case 6:
        m1wait(actionwk);
    }

    actionsub(actionwk);
    if (actionwk->r_no0 <= 2) {
        frameout_s(actionwk);
    }
}

static void m_init(sprite_status *actionwk) {
    et6_work *work = et6_get_work(actionwk);
    Uint16 *a1;
    Uint16 d0;

    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->sprpri = 4;
    actionwk->sprhs = 34;
    actionwk->sprhsize = 34;
    actionwk->sprvsize = 32;

    a1 = tbl0sproffset;
    d0 = ((stageno.w + 1) << 2) + (Uint16)time_flag;
    actionwk->sproffset = a1[d0];

    actionwk->patbase = pat_et;

    work->explosion_table_offset = 0;
    work->origin_y = actionwk->yposi.w.h;
    work->hover_counter_signed = 4;
    work->hover_direction = 1;

    d0 = 0;
    if (generate_flag == 0)
        d0 += 2;
    if (time_flag == 0)
        ++d0;
    actionwk->patno = d0;
    if (generate_flag == 0 && time_flag == 0) {

        actionwk->colino = 250;
        actionwk->yposi.w.h -= 16;
    }
}

static void m_wait(sprite_status *actionwk) {
    et6_work *work = et6_get_work(actionwk);
    Sint32 d0;
    sprite_status *a1;

    if (generate_flag != 0 || time_flag != 0)
        return;
    a_hover(actionwk);

    if (actionwk->colicnt != 0) {
        actionwk->colino = 0;
        work->hover_counter_signed = 0;
        actionwk->patno = 7;
        actionwk->r_no0 += 2;
        generate_flag = 1;
        d0 = 150;
        scoreup(d0);
        a1 = &actwk[0];
        if (hitchk(actionwk, a1))
            ride_on_clr(actionwk, a1);
        return;
    }
    hitchk(actionwk, &actwk[0]);
    patchg(actionwk, (Uint8 **)pchg);
}

void m_die(sprite_status *actionwk) {
    et6_work *work = et6_get_work(actionwk);
    sprite_status *a1;
    char d0;
    Sint16 d5, d6, a6;

    a6 = work->explosion_table_offset;
    d0 = tbl0[a6++];
    if (d0 < 0) {

        actionwk->r_no0 += 2;
        work->reset_timer = 8;
        return;
    }
    ++work->explosion_timer;
    if (work->explosion_timer != d0)
        return;
    d5 = tbl0[a6++];
    d6 = tbl0[a6++];
    work->explosion_table_offset = a6;
    if (actwkchk(&a1) != 0)
        return;
    a1->actno = 24;
    a1->r_no1 = 1;
    a1->xposi = actionwk->xposi;
    a1->yposi = actionwk->yposi;
    a1->xposi.w.h += d5;
    a1->yposi.w.h += d6;
    baku_init(a1);
    soundset(158);
}

void m1wait(sprite_status *actionwk) {
    et6_work *work = et6_get_work(actionwk);

    if (--work->reset_timer)
        return;
    actionwk->r_no0 -= 6;
    actionwk->yposi.w.h = work->origin_y;
    soundset(217);
}

void a_hover(sprite_status *actionwk) {
    et6_work *work = et6_get_work(actionwk);
    Uint16 d0;

    ++work->hover_counter;
    d0 = work->hover_counter;
    d0 &= 7;
    if (d0 == 0) {

        d0 = work->hover_direction;
        actionwk->yposi.w.h += d0;
    }
    d0 = work->hover_counter;
    d0 &= 31;
    if (d0 == 0)
        work->hover_direction = -work->hover_direction;
}
