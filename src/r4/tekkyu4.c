#include <stddef.h>

#include "../equ.h"
#include "tekkyu4.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"

#if defined(R41C)
#define SPRITE_TEKKYU4_BASE 449
#elif defined(R41D)
#define SPRITE_TEKKYU4_BASE 435
#elif defined(R42A)
#define SPRITE_TEKKYU4_BASE 469
#else
#define SPRITE_TEKKYU4_BASE 474
#endif

sprite_pattern tekkyu4_pat0 = {1, {{-16, -16, 0, SPRITE_TEKKYU4_BASE}}};
sprite_pattern *pat_tekkyu4[1] = {&tekkyu4_pat0};

#pragma pack(push, 1)
typedef struct {
    Sint16 base_x;
    Sint16 base_y;
    Uint16 angle;
    Sint16 angular_speed;
    Uint16 shift;
    Sint16 origin_x;
    Uint8 unused12[8];
    Sint16 parent_index;
} tekkyu4_work;
#pragma pack(pop)

_Static_assert(offsetof(tekkyu4_work, base_x) == 0,
               "tekkyu4_work.base_x offset");
_Static_assert(offsetof(tekkyu4_work, base_y) == 2,
               "tekkyu4_work.base_y offset");
_Static_assert(offsetof(tekkyu4_work, angle) == 4,
               "tekkyu4_work.angle offset");
_Static_assert(offsetof(tekkyu4_work, angular_speed) == 6,
               "tekkyu4_work.angular_speed offset");
_Static_assert(offsetof(tekkyu4_work, shift) == 8,
               "tekkyu4_work.shift offset");
_Static_assert(offsetof(tekkyu4_work, origin_x) == 10,
               "tekkyu4_work.origin_x offset");
_Static_assert(offsetof(tekkyu4_work, parent_index) == 20,
               "tekkyu4_work.parent_index offset");
_Static_assert(sizeof(tekkyu4_work) <= sizeof(((sprite_status *)0)->actfree),
               "tekkyu4_work fits in actfree");

static tekkyu4_work *tekkyu4_get_work(sprite_status *pActwk) {
    return (tekkyu4_work *)pActwk->actfree;
}

void tekkyu4(sprite_status *pActwk) {
    tekkyu4_work *work = tekkyu4_get_work(pActwk);
    sprite_status *pMainactwk;

    void (*tekkyu4_acttbl[2])(sprite_status *) = {&tekkyu4init, &tekkyu4move};

    if (work->parent_index) {
        pMainactwk = &actwk[work->parent_index];
        if (pMainactwk->actno != 56) {
            frameout(pActwk);
            return;
        }
    }
    tekkyu4_acttbl[pActwk->r_no0 / 2](pActwk);
}

void tekkyu4init(sprite_status *pActwk) {
    tekkyu4_work *work = tekkyu4_get_work(pActwk);
    sprite_status *pNewactwk1, *pNewactwk2, *pNewactwk3;
    Sint16 wD0;

    work->origin_x = pActwk->xposi.w.h;

    if (actwkchk(&pNewactwk3) != 0) {
        frameout(pActwk);
        return;
    }

    pNewactwk3->actno = pActwk->actno;
    pNewactwk3->r_no0 = 2;
    tekkyu4_get_work(pNewactwk3)->parent_index = pActwk - actwk;

    if (actwkchk(&pNewactwk2) != 0) {
        frameout(pActwk);
        return;
    }

    pNewactwk2->actno = pActwk->actno;
    pNewactwk2->r_no0 = 2;
    tekkyu4_get_work(pNewactwk2)->parent_index = pActwk - actwk;

    if (actwkchk(&pNewactwk1) != 0) {
        frameout(pActwk);
        return;
    }

    pNewactwk1->actno = pActwk->actno;
    pNewactwk1->r_no0 = 2;
    tekkyu4_get_work(pNewactwk1)->parent_index = pActwk - actwk;

    wD0 = -2048;

    if (pActwk->userflag.b.h & 128)
        wD0 *= -1;

    tekkyu4_get_work(pNewactwk1)->angle = (Sint32)wD0;
    tekkyu4_get_work(pNewactwk2)->angle = wD0 * 2;
    tekkyu4_get_work(pNewactwk3)->angle = wD0 * 3;

    work->angular_speed = 256;
    if (pActwk->userflag.b.h & 128)
        work->angular_speed *= -1;

    if ((wD0 = pActwk->userflag.b.h) & 128)
        wD0 *= -1;

    work->shift = wD0 & 127;

    pActwk->sprpri = 1;
    pNewactwk1->sprpri = 2;
    pNewactwk2->sprpri = 3;
    pNewactwk3->sprpri = 4;

    act_init_sub(pActwk, pActwk);
    act_init_sub(pActwk, pNewactwk1);
    act_init_sub(pActwk, pNewactwk2);
    act_init_sub(pActwk, pNewactwk3);
}

void act_init_sub(sprite_status *pActwk, sprite_status *pNewactwk) {
    tekkyu4_work *work = tekkyu4_get_work(pNewactwk);
    tekkyu4_work *source_work = tekkyu4_get_work(pActwk);

    pNewactwk->actflg |= 4;
    pNewactwk->colino = 169;
    pNewactwk->sprhs = 16;
    pNewactwk->sprhsize = 16;
    pNewactwk->sprvsize = 16;
    pNewactwk->sproffset = 880;
    pNewactwk->patbase = pat_tekkyu4;
    pNewactwk->r_no0 = 2;
    work->base_x = pActwk->xposi.w.h;
    work->base_y = pActwk->yposi.w.h;
    work->angular_speed = source_work->angular_speed;
    work->shift = source_work->shift;
}

void tekkyu4move(sprite_status *pActwk) {
    tekkyu4_work *work = tekkyu4_get_work(pActwk);
    Uint16 wD0, wD1;

    work->angle += work->angular_speed;

    sinset((Sint16)work->angle >> 8, (Sint16 *)&wD0, (Sint16 *)&wD1);

    wD0 = (Sint16)wD0 >> work->shift;
    wD1 = (Sint16)wD1 >> work->shift;

    pActwk->xposi.w.h = work->base_x + (Sint16)wD1;
    pActwk->yposi.w.h = work->base_y + (Sint16)wD0;

    actionsub(pActwk);

    if (!work->parent_index)
        frameout_s(pActwk);
}
