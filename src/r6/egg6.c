#include "../equ.h"
#include "egg6.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../ridechk.h"

typedef struct {
    char *spawn_table;
    Uint8 spawn_timer;
} egg6_work;

typedef struct {
    Sint32 fall_speed;
    Sint16 target_y;
} egg6_bomb_work;

static egg6_work *egg6_get_work(sprite_status *eggwk) {
    return (egg6_work *)eggwk->actfree;
}

static egg6_bomb_work *egg6_get_bomb_work(sprite_status *bombwk) {
    return (egg6_bomb_work *)bombwk->actfree;
}

static void act_init(sprite_status *eggwk);
static void act_check(sprite_status *eggwk);
static void act_make0(sprite_status *eggwk);
static void act_wait(sprite_status *eggwk);
static void act_make1(sprite_status *eggwk);
static void bomb_init(sprite_status *bombwk);
static void bomb_fall(sprite_status *bombwk);
static void bomb_die(sprite_status *bombwk);

static sprite_pattern egg00 = {1, {{-20, -28, 0, 299}}};
static sprite_pattern pat00 = {1, {{-16, -20, 0, 0}}};
sprite_pattern *pat_egg6[2] = {&egg00, &pat00};
static char tbl0[28] = {1,  0,   0,  5,   -18, -10, 10, -10, 10, 15,
                        0,  -10, 20, -10, -10, 25,  13, -10, 30, -10,
                        20, 35,  13, -10, 40,  -10, 10, -1};
static char tbl1[21] = {-24, -64, -8, 64, 8,    0,   24, -128, 40,  -128, 56,
                        64,  72,  64, 88, -128, 104, 64, 120,  -64, -1};
static Uint8 stack_pointer;

void eggz6(sprite_status *eggwk) {
    void (*act_tbl[5])(sprite_status *) = {&act_init, &act_check, &act_make0,
                                           &act_wait, &act_make1};

    if (eggwk->userflag.b.h != 0) {
        bomb(eggwk);
        return;
    }

    stack_pointer = 0;
    act_tbl[eggwk->r_no0 / 2](eggwk);
    if (stack_pointer != 0)
        return;
    actionsub(eggwk);
    if (eggwk->r_no0 <= 2)
        frameout_s(eggwk);
}

static Uint8 pchg_bomb00[4] = {1, 0, 1, 255};
static Uint8 *pchg_bomb[1] = {pchg_bomb00};
static sprite_pattern bomb0 = {1, {{-8, -8, 0, 300}}};
static sprite_pattern bomb1 = {1, {{-8, -8, 0, 301}}};
sprite_pattern *pat_bomb[2] = {&bomb0, &bomb1};

static void act_init(sprite_status *eggwk) {
    egg6_work *work = egg6_get_work(eggwk);

    if (generate_flag != 0) {
        stack_pointer = 1;
        frameout(eggwk);
        return;
    }

    eggwk->r_no0 += 2;
    eggwk->actflg |= 4;
    eggwk->sprpri = 3;
    eggwk->sprhs = eggwk->sprhsize = 20;
    eggwk->sprvsize = 28;

    eggwk->patbase = pat_egg6;
    eggwk->colino = 248;
    work->spawn_table = tbl0;
}

static void act_check(sprite_status *eggwk) {
    if (eggwk->colicnt != 0) {
        eggwk->colino = eggwk->colicnt = 0;
        eggwk->r_no0 += 2;
        if (hitchk(eggwk, &actwk[0]))
            ride_on_clr(eggwk, &actwk[0]);
    } else
        hitchk(eggwk, &actwk[0]);
}

static void act_make0(sprite_status *eggwk) {
    egg6_work *work = egg6_get_work(eggwk);
    char *tbl_address, tbl_data;
    Sint16 position_data_x, position_data_y;
    sprite_status *new_actwk;

    tbl_address = work->spawn_table;
    if ((tbl_data = *tbl_address++) < 0) {
        eggwk->r_no0 += 2;
        eggwk->patno = 1;
        work->spawn_timer = 60;
        return;
    }
    if (tbl_data != ++work->spawn_timer)
        return;
    position_data_x = *tbl_address++;
    position_data_y = *tbl_address++;
    work->spawn_table = tbl_address;
    if (actwkchk(&new_actwk) != 0)
        return;
    new_actwk->actno = 24;
    new_actwk->r_no1 = 1;
    new_actwk->xposi.w.h = eggwk->xposi.w.h + position_data_x;
    new_actwk->yposi.w.h = eggwk->yposi.w.h + position_data_y;
    soundset(158);
}

static void act_wait(sprite_status *eggwk) {
    egg6_work *work = egg6_get_work(eggwk);

    if (--work->spawn_timer == 0)
        eggwk->r_no0 += 2;
}

static void act_make1(sprite_status *eggwk) {
    char *tbl_address;
    Sint16 data_x, data_y;
    sprite_status *new_actwk;

    tbl_address = tbl1;
    while ((data_x = *tbl_address++) != -1) {
        data_y = *tbl_address++;
        if (actwkchk(&new_actwk) != 0) {
            frameout(eggwk);
            return;
        }
        new_actwk->actno = eggwk->actno;
        new_actwk->userflag.b.h = -1;
        new_actwk->xposi.w.h = eggwk->xposi.w.h + data_x;
        new_actwk->yposi.w.h = eggwk->yposi.w.h + data_y - 160;
        egg6_get_bomb_work(new_actwk)->target_y = eggwk->yposi.w.h + 38;
    };
    frameout(eggwk);
}

void bomb(sprite_status *bombwk) {
    void (*bomb_tbl[3])(sprite_status *) = {&bomb_init, &bomb_fall, &bomb_die};

    bomb_tbl[bombwk->r_no0 / 2](bombwk);
    actionsub(bombwk);
}

static void bomb_init(sprite_status *bombwk) {
    egg6_bomb_work *work = egg6_get_bomb_work(bombwk);

    bombwk->r_no0 += 2;
    bombwk->actflg |= 4;
    bombwk->sprpri = 3;
    bombwk->sprvsize = bombwk->sprhs = bombwk->sprhsize = 6;

    bombwk->patbase = pat_bomb;
    bombwk->colino = 183;
    work->fall_speed = 0;
}

static void bomb_fall(sprite_status *bombwk) {
    egg6_bomb_work *work = egg6_get_bomb_work(bombwk);

    bombwk->yposi.l += work->fall_speed;
    work->fall_speed += 1024;
    if (bombwk->yposi.w.h >= work->target_y)
        bombwk->r_no0 += 2;
    patchg(bombwk, pchg_bomb);
}

static void bomb_die(sprite_status *bombwk) {
    bombwk->actno = 24;
    bombwk->r_no0 = 0;
    bombwk->r_no1 = 1;
    soundset(158);
}
