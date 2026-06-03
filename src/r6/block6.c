#include <stddef.h>

#include "../equ.h"
#include "block6.h"
#include "../action.h"
#include "../actset.h"
#include "../ridechk.h"

#pragma pack(push, 1)
typedef struct {
    Uint16 parent_index;
    char *speed_table;
    Sint16 move_timer;
    Uint16 sequence_index;
    union {
        Uint8 trigger;
        Sint16 y_speed_direction;
    };
    Uint16 previous_child_index;
    Sint32 x_speed;
    Sint32 y_speed;
} block6_work;
#pragma pack(pop)

_Static_assert(sizeof(char *) == 4, "block6_work uses the Win32 DLL layout");
_Static_assert(offsetof(block6_work, parent_index) == 0,
               "block6_work.parent_index offset");
_Static_assert(offsetof(block6_work, speed_table) == 2,
               "block6_work.speed_table offset");
_Static_assert(offsetof(block6_work, move_timer) == 6,
               "block6_work.move_timer offset");
_Static_assert(offsetof(block6_work, sequence_index) == 8,
               "block6_work.sequence_index offset");
_Static_assert(offsetof(block6_work, trigger) == 10,
               "block6_work.trigger offset");
_Static_assert(offsetof(block6_work, y_speed_direction) == 10,
               "block6_work.y_speed_direction offset");
_Static_assert(offsetof(block6_work, previous_child_index) == 12,
               "block6_work.previous_child_index offset");
_Static_assert(offsetof(block6_work, x_speed) == 14,
               "block6_work.x_speed offset");
_Static_assert(offsetof(block6_work, y_speed) == 18,
               "block6_work.y_speed offset");
_Static_assert(sizeof(block6_work) <= sizeof(((sprite_status *)0)->actfree),
               "block6_work fits in actfree");

static block6_work *block6_get_work(sprite_status *blockwk) {
    return (block6_work *)blockwk->actfree;
}

static void main_init(sprite_status *blockwk);
static void main_move(sprite_status *blockwk);
static void sub(sprite_status *blockwk);
static void sub_go(sprite_status *blockwk);
static void sub_go1(sprite_status *blockwk);
static void sub_stop0(sprite_status *blockwk);
static void sub_stop01(sprite_status *blockwk);
static void sub_stop02(sprite_status *blockwk);
static void sub_back(sprite_status *blockwk);
static void sub_back1(sprite_status *blockwk);
static void sub_stop1(sprite_status *blockwk);
static void sub_stop11(sprite_status *blockwk);
static void sub_die(sprite_status *blockwk);
static Uint8 set_slave(sprite_status *blockwk);

static char dat00[4] = {0, 0, 0, -1};
static char dat01[4] = {1, 1, 1, -1};
static char dat02[4] = {2, 2, 2, -1};
static char dat03[4] = {3, 3, 3, -1};
static char *tbl00[4] = {dat00, dat01, dat02, dat03};
static char *tbl03[4] = {dat03, dat02, dat01, dat00};
static char tbl01_00[4] = {0, 0, 1, -1};
static char tbl01_01[4] = {1, 0, 1, -1};
static char tbl01_02[4] = {2, 2, 1, -1};
static char tbl01_03[4] = {3, 3, 3, -1};
static char *tbl01[4] = {tbl01_00, tbl01_01, tbl01_02, tbl01_03};
static char tbl02_00[3] = {0, 0, -1};
static char tbl02_01[3] = {2, 2, -1};
static char *tbl02[4] = {tbl02_00, tbl02_01, tbl02_00, tbl02_01};
static char tbl04_00[4] = {1, 1, 0, -1};
static char tbl04_01[4] = {2, 3, 3, -1};
static char tbl04_02[4] = {3, 0, 3, -1};
static char tbl04_03[4] = {0, 1, 0, -1};
static char *tbl04[4] = {tbl04_00, tbl04_01, tbl04_02, tbl04_03};
static char **tbl0[5] = {tbl00, tbl01, tbl02, tbl03, tbl04};
static sprite_pattern block = {1, {{-16, -16, 0, 298}}};
sprite_pattern *pat_block6[1] = {&block};
static char stack_pointer;

void block6(sprite_status *blockwk) {
    void (*main_tbl[2])(sprite_status *) = {&main_init, &main_move};

    if (blockwk->userflag.b.h < 0) {
        sub(blockwk);
        return;
    }

    main_tbl[blockwk->r_no0 / 2](blockwk);
    hitchk(blockwk, &actwk[0]);
    actionsub(blockwk);
    frameout_s(blockwk);
}

static void main_init(sprite_status *blockwk) {
    block6_work *work = block6_get_work(blockwk);

    blockwk->r_no0 += 2;
    blockwk->actflg |= 4;
    blockwk->sprpri = 1;
    blockwk->sprhs = blockwk->sprhsize = blockwk->sprvsize = 16;

    blockwk->patbase = pat_block6;
    work->parent_index = blockwk - actwk;
    work->trigger = 255;
    work->sequence_index = 0;
}

static void main_move(sprite_status *blockwk) {
    block6_work *work = block6_get_work(blockwk);

    if (work->trigger == 0)
        return;
    work->trigger = 0;
    work->speed_table = tbl0[blockwk->userflag.b.h][work->sequence_index / 2];

    if ((work->sequence_index += 2) >= 8)
        work->sequence_index = 0;
    if (set_slave(blockwk) != 0)
        frameout(blockwk);
}

static void sub(sprite_status *blockwk) {
    sprite_status *parent_wk;
    void (*sub_tbl[10])(sprite_status *) = {
        &sub_go,   &sub_go1,   &sub_stop0, &sub_stop01, &sub_stop02,
        &sub_back, &sub_back1, &sub_stop1, &sub_stop11, &sub_die};

    parent_wk = &actwk[block6_get_work(blockwk)->parent_index];
    if (parent_wk->actno != 42) {
        frameout(blockwk);
        return;
    }
    if (blockwk->userflag.b.l != parent_wk->userflag.b.l) {
        frameout(blockwk);
        return;
    }

    stack_pointer = 0;
    sub_tbl[blockwk->r_no0 / 2](blockwk);
    if (stack_pointer != 0)
        return;
    hitchk(blockwk, &actwk[0]);
    actionsub(blockwk);
}

static void sub_go(sprite_status *blockwk) {
    block6_work *work = block6_get_work(blockwk);
    char *speed_tbl, speed_data;

    blockwk->r_no0 += 2;
    work->move_timer = 63;
    work->x_speed = work->y_speed = 0;

    speed_tbl = work->speed_table - 1;
    switch (speed_data = *speed_tbl) {
    case 0:
        work->y_speed = -32768;
        break;
    case 1:
        work->x_speed = 32768;
        break;
    case 2:
        work->y_speed = 32768;
        break;
    case 3:
        work->x_speed = -32768;
        break;
    }

    if (actwk[work->parent_index].userflag.b.h == 2) {
        if (work->y_speed >= 0)
            work->y_speed_direction = blockwk->yspeed.w = 1;
        else
            work->y_speed_direction = blockwk->yspeed.w = -1;
    }
    sub_go1(blockwk);
}

static void sub_go1(sprite_status *blockwk) {
    block6_work *work = block6_get_work(blockwk);

    blockwk->xposi.l += work->x_speed;
    blockwk->yposi.l += work->y_speed;
    if (--work->move_timer < 0)
        blockwk->r_no0 += 2;
}

static void sub_stop0(sprite_status *blockwk) {
    sprite_status *parent_wk;

    parent_wk = &actwk[block6_get_work(blockwk)->parent_index];

    blockwk->r_no0 += 2;
    blockwk->yspeed.w = 0;
    if (parent_wk->userflag.b.h != 2)
        block6_get_work(blockwk)->move_timer = 30;
    else
        block6_get_work(blockwk)->move_timer = 0;
}

static void sub_stop01(sprite_status *blockwk) {
    block6_work *work = block6_get_work(blockwk);

    if (--work->move_timer >= 0)
        return;
    blockwk->r_no0 += 2;
    if (set_slave(blockwk) == 0)
        return;
    blockwk->r_no0 += 2;
    block6_get_work(&actwk[work->parent_index])->trigger = 255;
}

static void sub_stop02(sprite_status *blockwk) {
    blockwk->actno = blockwk->actno;
}

static void sub_back(sprite_status *blockwk) {
    blockwk->r_no0 += 2;
    block6_get_work(blockwk)->move_timer = 63;
    blockwk->yspeed.w = -block6_get_work(blockwk)->y_speed_direction;
}

static void sub_back1(sprite_status *blockwk) {
    block6_work *work = block6_get_work(blockwk);

    blockwk->xposi.l -= work->x_speed;
    blockwk->yposi.l -= work->y_speed;
    if (--work->move_timer < 0)
        blockwk->r_no0 += 2;
}

static void sub_stop1(sprite_status *blockwk) {
    sprite_status *parent_wk;

    parent_wk = &actwk[block6_get_work(blockwk)->parent_index];

    blockwk->r_no0 += 2;
    blockwk->yspeed.w = 0;
    if (parent_wk->userflag.b.h != 2)
        block6_get_work(blockwk)->move_timer = 30;
    else
        block6_get_work(blockwk)->move_timer = 0;
}

static void sub_stop11(sprite_status *blockwk) {
    sprite_status *parent_wk;

    if (--block6_get_work(blockwk)->move_timer >= 0)
        return;
    parent_wk = &actwk[block6_get_work(blockwk)->previous_child_index];
    if (parent_wk->userflag.b.h < 0)
        parent_wk->r_no0 += 2;
    blockwk->r_no0 += 2;
}

static void sub_die(sprite_status *blockwk) {
    if (hitchk(blockwk, &actwk[0]) != 0)
        ride_on_clr(blockwk, &actwk[0]);
    stack_pointer = -1;
    frameout(blockwk);
}

static Uint8 set_slave(sprite_status *blockwk) {
    block6_work *work = block6_get_work(blockwk);
    char *speed_tbl, counter = 0;
    sprite_status *new_actwk;

    speed_tbl = work->speed_table;
    if (speed_tbl[counter++] < 0)
        return 1;
    if (actwkchk(&new_actwk) != 0)
        return 1;
    new_actwk->actno = blockwk->actno;
    new_actwk->actflg = blockwk->actflg;
    new_actwk->sproffset = blockwk->sproffset;
    new_actwk->patbase = blockwk->patbase;
    new_actwk->xposi.l = blockwk->xposi.l;
    new_actwk->yposi.l = blockwk->yposi.l;
    new_actwk->xspeed.w = blockwk->xspeed.w;
    new_actwk->yspeed.w = blockwk->yspeed.w;
    new_actwk->mspeed.w = blockwk->mspeed.w;
    new_actwk->sprhsize = blockwk->sprhsize;
    new_actwk->sprvsize = blockwk->sprvsize;
    new_actwk->sprhs = blockwk->sprhs;
    new_actwk->sprpri = (char)(blockwk->sprpri + 1);
    new_actwk->patno = blockwk->patno;
    new_actwk->mstno.w = blockwk->mstno.w;
    new_actwk->patcnt = blockwk->patcnt;
    new_actwk->pattim = blockwk->pattim;
    new_actwk->pattimm = blockwk->pattimm;
    new_actwk->colino = blockwk->colino;
    new_actwk->colicnt = blockwk->colicnt;
    new_actwk->cddat = blockwk->cddat;
    new_actwk->cdsts = blockwk->cdsts;
    new_actwk->r_no0 = 0;
    new_actwk->r_no1 = blockwk->r_no1;
    new_actwk->direc.w = blockwk->direc.w;
    new_actwk->userflag.w = blockwk->userflag.w;
    block6_get_work(new_actwk)->parent_index = work->parent_index;

    new_actwk->userflag.b.h = -1;
    block6_get_work(new_actwk)->previous_child_index = blockwk - actwk;
    block6_get_work(new_actwk)->speed_table = &speed_tbl[counter];
    return 0;
}
