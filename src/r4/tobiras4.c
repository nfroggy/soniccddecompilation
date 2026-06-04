#include "../equ.h"
#include "tobiras4.h"
#include "../action.h"
#include "../actset.h"
#include "../impfuncs.h"
#include "../ridechk.h"

#if defined(R42A)
#define SPRITE_TOBIRAS4_BASE 516
#else
#define SPRITE_TOBIRAS4_BASE 509
#endif

static void act_init(sprite_status *pActwk);
static void act_sense(sprite_status *pActwk);
static void act_down(sprite_status *pActwk);
static void act_disp(sprite_status *pActwk);

typedef struct {
    Sint16 drop_counter;
    Sint32 y_velocity;
    Uint8 *switch_flag;
} tobiras4_work;

static tobiras4_work *tobiras4_work_get(sprite_status *pActwk) {
    return (tobiras4_work *)pActwk->actfree;
}

static sprite_pattern pat00 = {1, {{-64, -64, 0, SPRITE_TOBIRAS4_BASE}}};
static sprite_pattern pat01 = {1, {{-16, -64, 0, SPRITE_TOBIRAS4_BASE + 1}}};
static sprite_pattern pat02 = {1, {{-16, -64, 0, SPRITE_TOBIRAS4_BASE + 2}}};
sprite_pattern *pat_tobiras4[3] = {&pat00, &pat01, &pat02};

void tobiras4(sprite_status *pActwk) {
    void (*tbl[3])(sprite_status *) = {&act_init, &act_sense, &act_down};

    tbl[pActwk->r_no0 / 2](pActwk);
}

static void act_init(sprite_status *pActwk) {
    Uint8 *p;

    p = &switchflag[pActwk->userflag.b.h];
    if (*p) {
        frameout(pActwk);
        return;
    }
    tobiras4_work_get(pActwk)->switch_flag = p;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 17514;
    pActwk->patbase = pat_tobiras4;
    pActwk->sprvsize = 64;
    pActwk->sprhsize = 64;
    pActwk->sprpri = 3;
    pActwk->sprhs = 64;
}

static void act_sense(sprite_status *pActwk) {
    sprite_status *pNewActwk;
    Uint8 *p;
    Sint16 d5, d6;
    tobiras4_work *work;

    work = tobiras4_work_get(pActwk);
    p = work->switch_flag;
    if (*p == 0) {
        hitchk(pActwk, &actwk[0]);
        actionsub(pActwk);
        frameout_s(pActwk);
        return;
    }

    pActwk->r_no0 += 2;
    pActwk->sprhs = 16;
    pActwk->sprhsize = 16;
    work->drop_counter = 128;
    work->y_velocity = -65536;
    ++pActwk->patno;
    pActwk->xposi.w.h -= 48;

    d6 = 2;
    d5 = pActwk->xposi.w.h;
    do {
        if (actwkchk2(pActwk, &pNewActwk) != 0) {
            frameout(pActwk);
            return;
        }
        sMemCpy(pNewActwk, pActwk, sizeof(*pActwk));
        d5 += 32;
        pNewActwk->xposi.w.h = d5;
        if (!(d6 & 1)) {
            tobiras4_work_get(pNewActwk)->y_velocity *= -1;
            ++pNewActwk->patno;
        }
    } while (--d6 >= 0);

    act_disp(pActwk);
}

static void act_down(sprite_status *pActwk) {
    tobiras4_work *work = tobiras4_work_get(pActwk);

    if (work->drop_counter < 0) {
        frameout(pActwk);
        return;
    }
    --work->drop_counter;
    pActwk->yposi.l -= work->y_velocity;
    act_disp(pActwk);
}

static void act_disp(sprite_status *pActwk) {
    hitchk(pActwk, &actwk[0]);
    actionsub(pActwk);
}
