#include <stddef.h>

#include "../equ.h"
#include "movie1.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../ridechk.h"

#if defined(R11B)
#define SPRITE_MOVIE_BASE 528
#elif defined(R11C) || defined(R11D)
#define SPRITE_MOVIE_BASE 509
#elif defined(R12A) || defined(R12B)
#define SPRITE_MOVIE_BASE 519
#elif defined(R12C) || defined(R12D)
#define SPRITE_MOVIE_BASE 510
#else
#define SPRITE_MOVIE_BASE 543
#endif

Uint8 movie1_pchg00[6] = {0, 2, 1, 3, 1, 255};
Uint8 movie1_pchg01[58] = {0, 4, 4, 1, 4, 4, 1, 4, 4, 1, 4, 4, 1,  4, 4,
                           1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1,  1, 5,
                           5, 1, 5, 5, 1, 5, 5, 1, 5, 5, 1, 5, 5,  1, 1,
                           5, 5, 1, 1, 5, 5, 1, 1, 5, 5, 1, 1, 255};
Uint8 *movie1_pchg[2] = {movie1_pchg00, movie1_pchg01};
sprite_pattern spr_movie1 = {1, {{-12, -12, 0, SPRITE_MOVIE_BASE}}};
sprite_pattern spr_movie2 = {1, {{-8, -4, 0, SPRITE_MOVIE_BASE + 1}}};
sprite_pattern spr_movie3 = {1, {{-8, -4, 0, SPRITE_MOVIE_BASE + 2}}};
sprite_pattern spr_movie4 = {1, {{-20, -24, 0, SPRITE_MOVIE_BASE + 3}}};
sprite_pattern spr_movie5 = {1, {{-20, -24, 0, SPRITE_MOVIE_BASE + 4}}};
sprite_pattern spr_movie0 = {0, {{-20, -24, 0, SPRITE_MOVIE_BASE + 4}}};
sprite_pattern *pat_movie[6] = {&spr_movie1, &spr_movie0, &spr_movie2,
                                &spr_movie3, &spr_movie4, &spr_movie5};
char tbl0[37] = {1,   0,  0,   5, -18, -10, 10,  -10, 10,  15, 0,   -18, 20,
                 -10, 18, 22,  8, 23,  25,  13,  -10, 28,  -3, -25, 30,  10,
                 20,  32, -10, 2, 35,  13,  -10, 40,  -10, 10, -1};

#pragma pack(push, 1)
typedef struct {
    Uint16 explosion_timer;
    char *explosion_script;
    Uint8 reserved_after_script[20 - 2 - sizeof(char *)];
    union {
        Uint16 parent_actor;
        struct {
            Uint8 parent_actor_low;
            Uint8 parent_destroyed;
        };
    };
} movie1_work;
#pragma pack(pop)

_Static_assert(offsetof(movie1_work, explosion_timer) == 0,
               "movie1_work.explosion_timer must map to offset 0");
_Static_assert(offsetof(movie1_work, explosion_script) == 2,
               "movie1_work.explosion_script must map to offset 2");
_Static_assert(offsetof(movie1_work, parent_actor) == 20,
               "movie1_work.parent_actor must map to offset 20");
_Static_assert(offsetof(movie1_work, parent_destroyed) == 21,
               "movie1_work.parent_destroyed must map to offset 21");
_Static_assert(sizeof(movie1_work) <= sizeof(((sprite_status *)0)->actfree),
               "movie1_work must fit in sprite_status.actfree");

static movie1_work *movie1_work_get(sprite_status *pActwk) {
    return (movie1_work *)pActwk->actfree;
}

void movie1(sprite_status *pActwk) {
    void (*tbl_m[5])(sprite_status *) = {&mm_init, &mm_wait, &mm_die, &m_baku,
                                         &mm1wait};

    if (pActwk->userflag.b.h)
        sub(pActwk);
    else {
        tbl_m[pActwk->r_no0 / 2](pActwk);
        actionsub(pActwk);
        if (pActwk->r_no0 <= 2) {
            if (frameout_s(pActwk) != 0)
                return;
        }
    }
}

void die(sprite_status *pActwk) { frameout(pActwk); }

void mm_init(sprite_status *pActwk) {
    movie1_work *work = movie1_work_get(pActwk);
    sprite_status *subactwk;
    movie1_work *sub_work;

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

    if (stageno.b.l)
        pActwk->sproffset = 9135;
    else
        pActwk->sproffset = 1027;

    pActwk->patbase = pat_movie;
    work->explosion_script = tbl0;

    if (actwkchk(&subactwk) != 0) {
        die(pActwk);
        return;
    }
    subactwk->actno = pActwk->actno;
    subactwk->xposi.w.h = pActwk->xposi.w.h - 21;
    subactwk->yposi.w.h = pActwk->yposi.w.h - 7;
    subactwk->userflag.b.h = -1;
    sub_work = movie1_work_get(subactwk);
    sub_work->parent_actor = (Uint16)(Uint8)(pActwk - actwk);

    if (actwkchk(&subactwk) != 0) {
        die(pActwk);
        return;
    }
    subactwk->actno = pActwk->actno;
    subactwk->xposi.w.h = pActwk->xposi.w.h - 88;
    subactwk->yposi.w.h = pActwk->yposi.w.h - 4;
    subactwk->userflag.b.h = 1;
    sub_work = movie1_work_get(subactwk);
    sub_work->parent_actor = (Uint16)(Uint8)(pActwk - actwk);

    if (actwkchk(&subactwk) != 0) {
        die(pActwk);
        return;
    }
    subactwk->actno = 36;
    subactwk->xposi.w.h = pActwk->xposi.w.h - 88;
    subactwk->yposi.w.h = pActwk->yposi.w.h - 24;
    subactwk->userflag.b.h = -128;
    sub_work = movie1_work_get(subactwk);
    sub_work->parent_actor = (Uint16)(Uint8)(pActwk - actwk);

    if (actwkchk(&subactwk) != 0) {
        die(pActwk);
        return;
    }
    subactwk->actno = 36;
    subactwk->xposi.w.h = pActwk->xposi.w.h - 100;
    subactwk->yposi.w.h = pActwk->yposi.w.h + 4;
    subactwk->userflag.b.h = -127;
    sub_work = movie1_work_get(subactwk);
    sub_work->parent_actor = (Uint16)(Uint8)(pActwk - actwk);
}

void mm_wait(sprite_status *pActwk) {
    if (pActwk->colicnt) {
        pActwk->colino = 0;
        pActwk->colicnt = 0;
        pActwk->r_no0 += 2;
    }
    hitchk(pActwk, &actwk[0]);
}

void mm_die(sprite_status *pActwk) {
    movie1_work *work = movie1_work_get(pActwk);

    pActwk->r_no0 += 2;
    pActwk->patno = 1;
    work->parent_destroyed = 255;
    if (hitchk(pActwk, &actwk[0]))
        ride_on_clr(pActwk, &actwk[0]);
}

void m_baku(sprite_status *pActwk) {
    movie1_work *work = movie1_work_get(pActwk);
    char *temp;
    Uint8 timeb;
    char xx, yy;
    sprite_status *subactwk;

    temp = work->explosion_script;
    if (*temp >= 0) {
        work->explosion_timer += 256;
        timeb = work->explosion_timer >> 8;
        if (timeb != *temp)
            return;

        ++temp;
        xx = *temp;
        ++temp;
        yy = *temp;
        ++temp;
        work->explosion_script = temp;
        if (actwkchk(&subactwk) != 0)
            return;

        subactwk->actno = 24;
        subactwk->r_no1 = 1;
        subactwk->xposi.w.h = pActwk->xposi.w.h;
        subactwk->yposi.w.h = pActwk->yposi.w.h;
        subactwk->xposi.w.h += (Uint16)xx;
        subactwk->yposi.w.h += (Uint16)yy;
        soundset(158);

        return;
    }

    pActwk->r_no0 += 2;
    work->explosion_timer = 60;
}

void mm1wait(sprite_status *pActwk) {
    movie1_work *work = movie1_work_get(pActwk);

    --work->explosion_timer;
    if (!work->explosion_timer) {
        projector_flag = 255;
        die(pActwk);
    }
}

void sub(sprite_status *pActwk) {
    movie1_work *work = movie1_work_get(pActwk);
    Sint16 subact;

    subact = work->parent_actor;
    if (actwk[subact].actno != 46) {
        die(pActwk);
        return;
    }
    if (movie1_work_get(&actwk[subact])->parent_destroyed) {
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

    if (!stageno.b.l)
        pActwk->sproffset = 1027;
    else
        pActwk->sproffset = 9135;

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
