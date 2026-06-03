#include "../equ.h"
#include "movie7.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../ridechk.h"
#include <stddef.h>

#if defined(R71) || defined(R72)
#define SPRITE_MOVIE7_BASE 503
#elif defined(R73C) || defined(R73D)
#define SPRITE_MOVIE7_BASE 447
#else
#define SPRITE_MOVIE7_BASE 462
#endif

#pragma pack(push, 1)
typedef struct {
    union {
        Uint16 explosion_timer;
        Sint16 wait_timer;
    };
    char *script;
    Uint8 reserved0[20 - 2 - sizeof(char *)];
    union {
        Sint16 parent_actor;
        struct {
            Uint8 parent_actor_low;
            Uint8 parent_destroyed;
        };
    };
} movie7_work;
#pragma pack(pop)

_Static_assert(offsetof(movie7_work, explosion_timer) == 0,
               "movie7_work.explosion_timer offset");
_Static_assert(offsetof(movie7_work, wait_timer) == 0,
               "movie7_work.wait_timer offset");
_Static_assert(offsetof(movie7_work, script) == 2,
               "movie7_work.script offset");
_Static_assert(offsetof(movie7_work, parent_actor) == 20,
               "movie7_work.parent_actor offset");
_Static_assert(offsetof(movie7_work, parent_destroyed) == 21,
               "movie7_work.parent_destroyed offset");
_Static_assert(sizeof(movie7_work) <= sizeof(((sprite_status *)0)->actfree),
               "movie7_work fits in actfree");

static movie7_work *movie7_work_get(sprite_status *pActwk) {
    return (movie7_work *)pActwk->actfree;
}

static void die(sprite_status *pActwk);
static void m_init(sprite_status *pActwk);
static void m_wait(sprite_status *pActwk);
static void m_die(sprite_status *pActwk);
static void m_baku(sprite_status *pActwk);
static void m1wait(sprite_status *pActwk);
static void sub(sprite_status *pActwk);
static void s_init(sprite_status *pActwk);
static void s_move(sprite_status *pActwk);

Uint8 movie1_pchg00[6] = {0, 2, 1, 3, 1, 255};
Uint8 movie1_pchg01[58] = {0, 4, 4, 1, 4, 4, 1, 4, 4, 1, 4, 4, 1,  4, 4,
                           1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1,  1, 5,
                           5, 1, 5, 5, 1, 5, 5, 1, 5, 5, 1, 5, 5,  1, 1,
                           5, 5, 1, 1, 5, 5, 1, 1, 5, 5, 1, 1, 255};
Uint8 *movie1_pchg[2] = {movie1_pchg00, movie1_pchg01};
sprite_pattern spr_movie1 = {1, {{-12, -12, 0, SPRITE_MOVIE7_BASE}}};
sprite_pattern spr_movie2 = {1, {{-8, -4, 0, SPRITE_MOVIE7_BASE + 1}}};
sprite_pattern spr_movie3 = {1, {{-8, -4, 0, SPRITE_MOVIE7_BASE + 2}}};
sprite_pattern spr_movie4 = {1, {{-20, -24, 0, SPRITE_MOVIE7_BASE + 3}}};
sprite_pattern spr_movie5 = {1, {{-20, -24, 0, SPRITE_MOVIE7_BASE + 4}}};
sprite_pattern spr_movie0 = {0, {{-20, -24, 0, SPRITE_MOVIE7_BASE + 4}}};
sprite_pattern *pat_movie[6] = {&spr_movie1, &spr_movie0, &spr_movie2,
                                &spr_movie3, &spr_movie4, &spr_movie5};
char tbl0[37] = {1,   0,  0,   5, -18, -10, 10,  -10, 10,  15, 0,   -18, 20,
                 -10, 18, 22,  8, 23,  25,  13,  -10, 28,  -3, -25, 30,  10,
                 20,  32, -10, 2, 35,  13,  -10, 40,  -10, 10, -1};

void movie(sprite_status *pActwk) {
    void (*tbl_m[5])(sprite_status *) = {&m_init, &m_wait, &m_die, &m_baku,
                                         &m1wait};

    if (pActwk->userflag.b.h)
        sub(pActwk);
    else {
        tbl_m[pActwk->r_no0 / 2](pActwk);
        actionsub(pActwk);
        if (pActwk->r_no0 <= 2)
            frameout_s(pActwk);
    }
}

void die(sprite_status *pActwk) { frameout(pActwk); }

void m_init(sprite_status *pActwk) {
    sprite_status *subactwk;

    if (projector_flag) {
        die(pActwk);
        return;
    }

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 4;
    pActwk->sprhs = 12;
    pActwk->sprhsize = 12;
    pActwk->sprvsize = 12;
    pActwk->colino = 251;
    pActwk->sproffset = 1199;
    pActwk->patbase = pat_movie;
    movie7_work_get(pActwk)->script = tbl0;

    if (actwkchk(&subactwk) != 0) {
        die(pActwk);
        return;
    }
    subactwk->actno = pActwk->actno;
    subactwk->xposi.w.h = pActwk->xposi.w.h - 21;
    subactwk->yposi.w.h = pActwk->yposi.w.h - 7;
    subactwk->userflag.b.h = -1;
    movie7_work_get(subactwk)->parent_actor =
        (Sint16)(Uint8)(pActwk - actwk);

    if (actwkchk(&subactwk) != 0) {
        die(pActwk);
        return;
    }
    subactwk->actno = pActwk->actno;
    subactwk->xposi.w.h = pActwk->xposi.w.h - 88;
    subactwk->yposi.w.h = pActwk->yposi.w.h - 4;
    subactwk->userflag.b.h = 1;
    movie7_work_get(subactwk)->parent_actor =
        (Sint16)(Uint8)(pActwk - actwk);

    if (actwkchk(&subactwk) != 0) {
        die(pActwk);
        return;
    }
    subactwk->actno = 40;
    subactwk->xposi.w.h = pActwk->xposi.w.h - 88;
    subactwk->yposi.w.h = pActwk->yposi.w.h - 24;
    subactwk->userflag.b.h = -128;
    movie7_work_get(subactwk)->parent_actor =
        (Sint16)(Uint8)(pActwk - actwk);

    if (actwkchk(&subactwk) != 0) {
        die(pActwk);
        return;
    }
    subactwk->actno = 40;
    subactwk->xposi.w.h = pActwk->xposi.w.h - 98;
    subactwk->yposi.w.h = (Sint32)pActwk->yposi.w.h;
    subactwk->userflag.b.h = -127;
    movie7_work_get(subactwk)->parent_actor =
        (Sint16)(Uint8)(pActwk - actwk);
}

void m_wait(sprite_status *pActwk) {
    if (pActwk->colicnt) {
        pActwk->colino = 0;
        pActwk->colicnt = 0;
        pActwk->r_no0 += 2;
    }
    hitchk(pActwk, &actwk[0]);
}

void m_die(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->patno = 1;
    movie7_work_get(pActwk)->parent_destroyed = 255;
    if (hitchk(pActwk, &actwk[0]))
        ride_on_clr(pActwk, &actwk[0]);
}

void m_baku(sprite_status *pActwk) {
    char *temp;
    Uint8 timeb;
    char xx, yy;
    sprite_status *subactwk;
    movie7_work *work = movie7_work_get(pActwk);

    temp = work->script;
    if (*temp >= 0) {
        work->explosion_timer += 256;
        timeb = work->explosion_timer >> 8;
        if (timeb == *temp) {
            ++temp;
            xx = *temp;
            ++temp;
            yy = *temp;
            ++temp;
            work->script = temp;
            if (actwkchk(&subactwk) == 0) {
                subactwk->actno = 24;
                subactwk->r_no1 = 1;
                subactwk->xposi.w.h = pActwk->xposi.w.h;
                subactwk->yposi.w.h = pActwk->yposi.w.h;
                subactwk->xposi.w.h += (Uint16)xx;
                subactwk->yposi.w.h += (Uint16)yy;
                soundset(158);
            }
        }
    } else {
        pActwk->r_no0 += 2;
        work->wait_timer = 60;
    }
}

void m1wait(sprite_status *pActwk) {
    --movie7_work_get(pActwk)->explosion_timer;
    if (!movie7_work_get(pActwk)->explosion_timer) {
        projector_flag = 255;
        die(pActwk);
    }
}

void sub(sprite_status *pActwk) {
    Sint16 subact;

    subact = movie7_work_get(pActwk)->parent_actor;
    if (actwk[subact].actno != 39) {
        die(pActwk);
        return;
    }
    if (movie7_work_get(&actwk[subact])->parent_destroyed) {
        die(pActwk);
        return;
    }
    if (pActwk->r_no0) {
        s_move(pActwk);
        return;
    }
    s_init(pActwk);
}

void s_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 4;
    pActwk->sproffset = 1199;
    pActwk->patbase = pat_movie;
    if (pActwk->userflag.b.h < 0) {

        pActwk->sprhs = 8;
        pActwk->sprhsize = 8;
        pActwk->sprvsize = 4;
        pActwk->mstno.b.h = 0;
    } else {

        pActwk->sprhs = 20;
        pActwk->sprhsize = 20;
        pActwk->sprvsize = 24;
        pActwk->mstno.b.h = 1;
    }
    s_move(pActwk);
}

void s_move(sprite_status *pActwk) {

    patchg(pActwk, movie1_pchg);
    actionsub(pActwk);
}
