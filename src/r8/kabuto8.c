#include "../equ.h"
#include "kabuto8.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../suicide.h"

static void kabuto0(sprite_status *kabutowk);
static void a_init(sprite_status *kabutowk);
static void a_fall(sprite_status *kabutowk);
static void a_move(sprite_status *kabutowk);
static void kabuto1(sprite_status *tunowk);
static void b_init(sprite_status *tunowk);
static void b_move(sprite_status *tunowk);

typedef struct {
    Sint16 timer;
    Sint16 offset_x;
    Sint16 offset_y;
    Sint32 x_speed;
    Sint16 base_x;
    Sint16 sound_timer;
    Uint16 parent_index;
} kabuto8_work;

static kabuto8_work *kabuto8_get_work(sprite_status *kabutowk) {
    return (kabuto8_work *)kabutowk->actfree;
}

void kabuto(sprite_status *kabutowk) {
    if (kabutowk->userflag.b.l >= 0)
        kabuto0(kabutowk);
    else
        kabuto1(kabutowk);
}

#if defined(R82)
#define SPRITE_KABUTO8_BASE 309
#elif defined(R83)
#define SPRITE_KABUTO8_BASE 302
#else
#define SPRITE_KABUTO8_BASE 300
#endif

static sprite_pattern cub00 = {1, {{-24, -16, 0, SPRITE_KABUTO8_BASE}}};
sprite_pattern *pat_kabuto[1] = {&cub00};

static void kabuto0(sprite_status *kabutowk) {
    void (*tbl[3])(sprite_status *) = {&a_init, &a_fall, &a_move};
    if (enemy_suicide(kabutowk))
        return;
    tbl[kabutowk->r_no0 / 2](kabutowk);
    actionsub(kabutowk);
    frameout_s00(kabutowk, kabuto8_get_work(kabutowk)->base_x);
}

static void a_init(sprite_status *kabutowk) {
    kabuto8_work *work = kabuto8_get_work(kabutowk);
    sprite_status *new_actwk;

    work->base_x = kabutowk->xposi.w.h;

    kabutowk->actflg |= 4;
    kabutowk->sprpri = 4;

    kabutowk->patbase = pat_kabuto;
    kabutowk->sprhsize = 24;
    kabutowk->sprvsize = 14;
    kabutowk->colino = 42;
    work->x_speed = -20480;
    kabutowk->r_no0 += 2;
    if (actwkchk2(kabutowk, &new_actwk) != 0) {
        frameout(kabutowk);
        return;
    }
    new_actwk->actno = kabutowk->actno;
    new_actwk->userflag.b.h = kabutowk->userflag.b.h;
    new_actwk->userflag.b.l = -1;
    new_actwk->xposi.w.h = kabutowk->xposi.w.h;
    new_actwk->yposi.w.h = kabutowk->yposi.w.h;
    kabuto8_get_work(new_actwk)->parent_index = kabutowk - actwk;
    if (kabutowk->userflag.b.l != 0) {
        kabutowk->actflg |= 1;
        kabutowk->cddat |= 1;
        new_actwk->actflg |= 1;
        new_actwk->cddat |= 1;
    }
    a_fall(kabutowk);
}

static void a_fall(sprite_status *kabutowk) {
    Sint16 colli_data;

    kabutowk->yposi.l += 65536;
    if ((colli_data = emycol_d(kabutowk)) < 0) {
        kabutowk->yposi.w.h += colli_data;
        kabutowk->r_no0 += 2;
    }
}

static void a_move(sprite_status *kabutowk) {
    kabuto8_work *work = kabuto8_get_work(kabutowk);

    if ((char)kabutowk->actflg < 0 && !(++work->sound_timer & 31)) {

        soundset(177);
    }
    kabutowk->xposi.l += work->x_speed;

    kabutowk->yposi.w.h += emycol_d(kabutowk);

    if (--work->timer < 0) {
        work->timer = 409;
        work->x_speed = -work->x_speed;
    }
}

static Uint8 p00[4] = {2, 0, 1, 255};
static Uint8 p01[4] = {2, 2, 3, 255};
static Uint8 *pchg[2] = {p00, p01};
static sprite_pattern hea00 = {2,
                               {{-8, -8, 0, SPRITE_KABUTO8_BASE + 2},
                                {-12, -16, 0, SPRITE_KABUTO8_BASE + 1}}};
static sprite_pattern hea01 = {2,
                               {{-8, -8, 0, SPRITE_KABUTO8_BASE + 2},
                                {-12, -17, 16, SPRITE_KABUTO8_BASE + 1}}};
static sprite_pattern hea02 = {2,
                               {{-4, -2, 0, SPRITE_KABUTO8_BASE + 3},
                                {-12, -16, 0, SPRITE_KABUTO8_BASE + 1}}};
static sprite_pattern hea03 = {2,
                               {{-4, -2, 0, SPRITE_KABUTO8_BASE + 3},
                                {-12, -17, 16, SPRITE_KABUTO8_BASE + 1}}};
static sprite_pattern *pat[4] = {&hea00, &hea01, &hea02, &hea03};

void kabuto1(sprite_status *tunowk) {
    void (*tbl[2])(sprite_status *) = {&b_init, &b_move};
    tbl[tunowk->r_no0 / 2](tunowk);
    actionsub(tunowk);
}

static void b_init(sprite_status *tunowk) {
    tunowk->r_no0 += 2;
    tunowk->actflg |= 4;
    tunowk->sprpri = 3;
    tunowk->patbase = pat;
    tunowk->sprhsize = 12;
    tunowk->sprvsize = 16;
    tunowk->colino = 171;
}

static void b_move(sprite_status *tunowk) {
    kabuto8_work *work = kabuto8_get_work(tunowk);
    sprite_status *bodywk;

    bodywk = &actwk[work->parent_index];
    if (bodywk->actno != 36) {
        frameout(tunowk);
        return;
    }
    if (tunowk->userflag.b.h)
        goto label1;

    if (--work->timer < 0) {
        work->timer = 60;
        tunowk->mstno.b.h ^= 1;
    label1:
        work->offset_x = -25;
        work->offset_y = 0;
        if (tunowk->mstno.b.h) {
            work->offset_x = -19;
            work->offset_y = -15;
        }
        if (tunowk->actflg & 1) {
            work->offset_x = -work->offset_x;
        }
    }

    tunowk->xposi.w.h = bodywk->xposi.w.h + work->offset_x;
    tunowk->yposi.w.h = bodywk->yposi.w.h + work->offset_y;

    patchg(tunowk, pchg);
}
