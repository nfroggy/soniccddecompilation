#include "../equ.h"
#include "et7.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../ridechk.h"
#include "../score.h"
#include <stddef.h>

static void m_init(sprite_status *pActwk);
static void m_wait(sprite_status *pActwk);
static void m_die(sprite_status *pActwk);
static void m1wait(sprite_status *pActwk);
static void a_hover(sprite_status *pActwk);

static char p_a[6] = {3, 3, 4, 5, 6, -1};
static char *pchg[1] = {p_a};
static sprite_pattern et_pat00 = {1, {{-40, 16, 0, 455}}};
static sprite_pattern et_pat01 = {1, {{-40, 16, 0, 456}}};
static sprite_pattern et_pat02 = {1, {{-40, -32, 0, 457}}};
static sprite_pattern et_pat03 = {1, {{-40, -32, 0, 458}}};
static sprite_pattern et_pat04 = {1, {{-40, -32, 0, 459}}};
static sprite_pattern et_pat05 = {1, {{-40, -32, 0, 460}}};
static sprite_pattern et_pat06 = {1, {{-40, -32, 0, 461}}};
static sprite_pattern et_pat07;
sprite_pattern *pat_et[8] = {&et_pat00, &et_pat01, &et_pat02, &et_pat03,
                             &et_pat04, &et_pat05, &et_pat06, &et_pat07};
static void (*et_jmp_tbl[4])(sprite_status *) = {&m_init, &m_wait, &m_die,
                                                 &m1wait};
static char tbl0[64] = {
    1,   0,  0,   2,   -40, -20, 3,   28, 10, 4,   18,  -18, 5,   -18, -10, 6,
    8,   -8, 8,   -18, 14,  10,  -10, 10, 12, 30,  -10, 15,  0,   -18, 18,  20,
    -10, 20, -10, 18,  22,  8,   23,  25, 13, -10, 26,  23,  -22, 28,  -3,  -25,
    30,  10, 20,  32,  -10, 2,   34,  30, -8, 35,  13,  -10, 40,  -10, 10,  -1};

#pragma pack(push, 1)
typedef struct {
    union {
        Sint16 hover_counter;
        struct {
            Uint8 delay_timer;
            Uint8 hover_counter_high;
        };
    };
    Sint16 explosion_table_index;
    Uint8 reserved4[2];
    Sint16 base_y;
    Sint16 hover_direction;
} et7_work;
#pragma pack(pop)

_Static_assert(offsetof(et7_work, hover_counter) == 0,
               "et7_work.hover_counter offset");
_Static_assert(offsetof(et7_work, delay_timer) == 0,
               "et7_work.delay_timer offset");
_Static_assert(offsetof(et7_work, explosion_table_index) == 2,
               "et7_work.explosion_table_index offset");
_Static_assert(offsetof(et7_work, base_y) == 6,
               "et7_work.base_y offset");
_Static_assert(offsetof(et7_work, hover_direction) == 8,
               "et7_work.hover_direction offset");
_Static_assert(sizeof(et7_work) <= sizeof(((sprite_status *)0)->actfree),
               "et7_work fits in actfree");

static et7_work *et7_work_get(sprite_status *pActwk) {
    return (et7_work *)pActwk->actfree;
}

void et(sprite_status *pActwk) {
    et_jmp_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);

    if (pActwk->r_no0 <= 2) {
        frameout_s(pActwk);
    }
}

static void m_init(sprite_status *pActwk) {
    et7_work *work = et7_work_get(pActwk);
    Uint8 patnowk;
    Uint16 tbl0sproffset[3][4] = {
        {942, 942, 0, 0}, {942, 942, 0, 0}, {0, 0, 0, 0}};

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 4;
    pActwk->sprhs = pActwk->sprhsize = 34;
    pActwk->sprvsize = 32;

    pActwk->sproffset = tbl0sproffset[stageno.b.l][time_flag];

    pActwk->patbase = pat_et;
    work->explosion_table_index = 0;
    work->base_y = pActwk->yposi.w.h;
    work->hover_counter = 4;
    work->hover_direction = 1;

    patnowk = 0;

    if (!generate_flag) {
        patnowk += 2;
    }

    if (time_flag == 0) {
        ++patnowk;
    }

    pActwk->patno = patnowk;

    if (!generate_flag && time_flag == 0) {
        pActwk->colino = 250;
        pActwk->yposi.w.h -= 16;
    }

    m_wait(pActwk);
}

static void m_wait(sprite_status *pActwk) {
    et7_work *work = et7_work_get(pActwk);

    if (!generate_flag && time_flag == 0) {
        a_hover(pActwk);
        if (pActwk->colicnt) {
            pActwk->colino = pActwk->colicnt = 0;
            work->hover_counter = 0;
            pActwk->patno = 7;
            pActwk->r_no0 += 2;
            generate_flag = 1;
            scoreup(150);
            if (hitchk(pActwk, &actwk[0])) {
                ride_on_clr(pActwk, &actwk[0]);
            }

        } else {
            hitchk(pActwk, &actwk[0]);
            patchg(pActwk, (Uint8 **)pchg);
        }
    }
}

static void m_die(sprite_status *pActwk) {
    et7_work *work = et7_work_get(pActwk);
    char timewk;
    Sint16 xwk, ywk;
    Sint16 table_index;
    sprite_status *pNewact;

    table_index = work->explosion_table_index;
    timewk = tbl0[table_index++];

    if (timewk < 0) {

        pActwk->r_no0 += 2;
        work->delay_timer = 8;
    } else {
        if (++work->delay_timer == (Uint8)timewk) {
            xwk = tbl0[table_index++];
            ywk = tbl0[table_index++];
            work->explosion_table_index = table_index;

            if (actwkchk(&pNewact) == 0) {
                pNewact->actno = 24;
                pNewact->r_no1 = 1;
                pNewact->xposi.w.h = pActwk->xposi.w.h + xwk;
                pNewact->yposi.w.h = pActwk->yposi.w.h + ywk;
                soundset(158);
            }
        }
    }
}

static void m1wait(sprite_status *pActwk) {
    et7_work *work = et7_work_get(pActwk);

    if (--work->delay_timer == 0) {
        pActwk->r_no0 -= 6;
        pActwk->yposi.w.h = work->base_y;
        soundset(217);
    }
}

static void a_hover(sprite_status *pActwk) {
    et7_work *work = et7_work_get(pActwk);
    Sint16 timewk;

    ++work->hover_counter;

    timewk = work->hover_counter;
    if (!(timewk & 7)) {
        pActwk->yposi.w.h += work->hover_direction;
    }

    timewk = work->hover_counter;
    if (!(timewk & 31)) {
        work->hover_direction *= -1;
    }
}
