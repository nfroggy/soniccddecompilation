#include "../equ.h"
#include "pocket.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../player_work.h"
#include "../playsub.h"
#include "../score.h"

#if defined(R31A)
#define SPRITE_POCKET_BASE 558
#elif defined(R31B) || defined(R32A)
#define SPRITE_POCKET_BASE 552
#elif defined(R33C) || defined(R33D)
#define SPRITE_POCKET_BASE 556
#else
#define SPRITE_POCKET_BASE 505
#endif

static sprite_pattern pat_pocket1 = {1, {{0, 0, 0, 0}}};
static sprite_pattern pat_pocket2 = {1, {{-24, -8, 0, SPRITE_POCKET_BASE}}};
static sprite_pattern pat_pocket3 = {1,
                                     {{-16, -24, 0, SPRITE_POCKET_BASE + 1}}};
static sprite_pattern pat_pocket4 = {1, {{-8, -16, 0, SPRITE_POCKET_BASE + 2}}};
static sprite_pattern pat_pocket5 = {1, {{-24, -8, 0, SPRITE_POCKET_BASE + 3}}};
sprite_pattern *pat[4] = {&pat_pocket1, &pat_pocket2, &pat_pocket3,
                          &pat_pocket4};
sprite_pattern *pat_pocket[2] = {&pat_pocket5, &pat_pocket1};
static char pchg0[4] = {3, 0, 1, -1};
static char *pchg[1] = {pchg0};
void (*pocket_move_tbl[11])(sprite_status *) = {
    &a_init,  &a_move0, &a_move00, &a_move1, &a_move2, &a_move3,
    &a_move4, &a_move5, &a_move6,  &a_move7, &a_move8};

typedef struct {
    Sint16 timer;
    Uint16 child_index;
    Uint16 parent_index;
    Uint16 bonus_remaining;
    Uint8 active;
} pocket_work;

static pocket_work *pocket_get_work(sprite_status *actionwk) {
    return (pocket_work *)actionwk->actfree;
}

void pocket(sprite_status *actionwk) {
    if (actionwk->userflag.b.h < 0)
        pocket0(actionwk);
    else
        pocket1(actionwk);
}

void pocket0(sprite_status *actionwk) {
    pocket_move_tbl[actionwk->r_no0 / 2](actionwk);
    actionsub(actionwk);
    frameout_s(actionwk);
}

void a_init(sprite_status *actionwk) {
    if (time_flag == 2 && generate_flag != 0) {
        frameout(actionwk);
        return;
    }

    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->sprpri = 0;
    actionwk->sproffset = 943;
    actionwk->patbase = pat;

    pocket_get_work(actionwk)->bonus_remaining = 7;
    a_move0(actionwk);
}

void a_move0(sprite_status *actionwk) {
    sprite_status *a6;

    if (editmode.w)
        return;
    a6 = &actwk[0];
    a_check(actionwk, a6);
    if (pocket_get_work(actionwk)->active != 0)
        to_a_move1(actionwk);
}

void a_check(sprite_status *actionwk, sprite_status *a6) {
    Sint16 d0;

    if (a6->yspeed.w < 0)
        return;
    if (a6->r_no0 == 4 || a6->r_no0 == 6)
        return;
    d0 = a6->yposi.w.h - actionwk->yposi.w.h;
    d0 += 24;
    if (d0 < 0 || d0 >= 16)
        return;
    d0 = a6->xposi.w.h - actionwk->xposi.w.h;
    d0 += 24;
    if (d0 < 0 || d0 >= 48)
        return;

    pocket_get_work(actionwk)->active = 255;
    player_work_get(a6)->status_flags |= 1;
    a6->cddat |= 4;
    a6->sprvsize = 14;
    a6->sprhs = 7;
    a6->mstno.b.h = 2;
    a6->xspeed.w = 0;
    a6->yspeed.w = -2048;
    a6->xposi.w.h = actionwk->xposi.w.h;
    a6->yposi.w.h = actionwk->yposi.w.h - 16;
}

void to_a_move1(sprite_status *actionwk) {
    actionwk->r_no0 += 2;
    pocket_get_work(actionwk)->timer = 8;
    actionwk->patno = 1;
    soundset(215);
    a_move00(actionwk);
}

void a_move00(sprite_status *actionwk) {
    sprite_status *a1;

    if (--pocket_get_work(actionwk)->timer >= 0)
        return;
    actionwk->r_no0 += 2;
    actionwk->patno = 2;
    pocket_get_work(actionwk)->timer = 6;
    if (actwkchk(&a1) != 0) {
        frameout(actionwk);
        return;
    }
    pocket_get_work(actionwk)->child_index = a1 - actwk;
    pocket_get_work(a1)->parent_index = actionwk - actwk;
    a1->actno = actionwk->actno;

    a1->xposi.w.h = actionwk->xposi.w.h;
    a1->yposi.w.h = actionwk->yposi.w.h;
    player_work_get(&actwk[0])->status_flags |= 64;
}

void a_move1(sprite_status *actionwk) {
    if (--pocket_get_work(actionwk)->timer >= 0)
        return;
    actionwk->r_no0 += 2;
    pocket_get_work(actionwk)->timer = 2;
    actionwk->patno = 3;
}

void a_move2(sprite_status *actionwk) {
    if (--pocket_get_work(actionwk)->timer >= 0)
        return;
    actionwk->r_no0 += 2;
    pocket_get_work(actionwk)->timer = 30;
    actionwk->patno = 0;
}

void a_move3(sprite_status *actionwk) {
    if (--pocket_get_work(actionwk)->timer >= 0)
        return;
    actionwk->r_no0 += 2;
    pocket_get_work(actionwk)->timer = 30;
    if (pocket_get_work(actionwk)->bonus_remaining == 0)
        return;
    --pocket_get_work(actionwk)->bonus_remaining;
    scoreup(10);
    tensuu0(actionwk, 0);
}

void a_move4(sprite_status *actionwk) {
    if (--pocket_get_work(actionwk)->timer >= 0)
        return;
    actionwk->r_no0 += 2;
    pocket_get_work(actionwk)->timer = 30;
    if (pocket_get_work(actionwk)->bonus_remaining == 0)
        return;
    --pocket_get_work(actionwk)->bonus_remaining;
    scoreup(10);
    tensuu0(actionwk, 0);
}

void a_move5(sprite_status *actionwk) {
    if (--pocket_get_work(actionwk)->timer >= 0)
        return;
    actionwk->r_no0 += 2;
    pocket_get_work(actionwk)->timer = 4;
    actionwk->patno = 3;
}

void a_move6(sprite_status *actionwk) {
    if (--pocket_get_work(actionwk)->timer >= 0)
        return;
    actionwk->r_no0 += 2;
    pocket_get_work(actionwk)->timer = 6;
    actionwk->patno = 2;
}

void a_move7(sprite_status *actionwk) {
    sprite_status *a1;

    if (--pocket_get_work(actionwk)->timer >= 0)
        return;
    soundset(159);
    actionwk->r_no0 += 2;
    actionwk->patno = 0;

    pocket_get_work(actionwk)->timer = 120;
    a1 = &actwk[pocket_get_work(actionwk)->child_index];
    a1->r_no0 = 4;
    if (pocket_get_work(actionwk)->active == 0)
        return;
    pocket_get_work(actionwk)->active = 0;
    player_work_get(&actwk[0])->status_flags &= 254;
    player_work_get(&actwk[0])->status_flags &= 191;
}

void a_move8(sprite_status *actionwk) {
    if (--pocket_get_work(actionwk)->timer >= 0)
        return;
    actionwk->r_no0 = 2;
    pocket_get_work(actionwk)->active = 0;
}

void pocket1(sprite_status *actionwk) {
    switch (actionwk->r_no0) {
    case 0:
        b_init(actionwk);
        break;
    case 2:
        b_move(actionwk);
        break;
    case 4:
        b_die(actionwk);
        break;
    }
}

void b_init(sprite_status *actionwk) {
    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->sprpri = 0;
    actionwk->sprhsize = 24;
    actionwk->sprvsize = 8;
    actionwk->sproffset = 17327;
    actionwk->patbase = pat_pocket;
    b_move(actionwk);
}

void b_move(sprite_status *actionwk) {
    sprite_status *a1;

    a1 = &actwk[pocket_get_work(actionwk)->parent_index];
    if (a1->actno != 47) {
        frameout(actionwk);
        return;
    }
    patchg(actionwk, (Uint8 **)pchg);
    actionsub(actionwk);
}

void b_die(sprite_status *actionwk) { frameout(actionwk); }
