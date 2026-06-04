#include "../equ.h"
#include "kowasi1.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../ridechk.h"

sprite_pattern spr_kowasi00 = {1, {{-16, -24, 0, 434}}};
sprite_pattern spr_kowasi01 = {1, {{-16, -24, 0, 435}}};
sprite_pattern spr_kowasi02 = {1, {{-16, -24, 0, 436}}};
sprite_pattern spr_kowasi03 = {1, {{-16, -24, 0, 437}}};
sprite_pattern spr_kowasi04 = {1, {{-16, -24, 0, 438}}};
sprite_pattern spr_kowasi05 = {1, {{-16, -24, 0, 439}}};
sprite_pattern spr_kowasi06 = {1, {{-16, -24, 0, 440}}};
sprite_pattern spr_kowasi07 = {1, {{-16, -24, 0, 441}}};
sprite_pattern spr_kowasi08 = {1, {{-8, -8, 0, 442}}};
sprite_pattern spr_kowasi09 = {1, {{-8, -8, 0, 443}}};
sprite_pattern spr_kowasi0a = {1, {{-8, -8, 0, 444}}};
sprite_pattern spr_kowasi0b = {1, {{-8, -8, 0, 445}}};
sprite_pattern spr_kowasi0c = {1, {{-8, -8, 0, 446}}};
sprite_pattern spr_kowasi0d = {1, {{-8, -8, 0, 447}}};
sprite_pattern *pat_kowasi[14] = {
    &spr_kowasi00, &spr_kowasi01, &spr_kowasi02, &spr_kowasi03, &spr_kowasi04,
    &spr_kowasi05, &spr_kowasi06, &spr_kowasi07, &spr_kowasi08, &spr_kowasi09,
    &spr_kowasi0a, &spr_kowasi0b, &spr_kowasi0c, &spr_kowasi0d};

typedef struct {
    union {
        Sint32 x_velocity;
        Sint16 saved_player_xspeed;
    };
    union {
        Sint32 y_velocity;
        Sint16 saved_player_yspeed;
    };
} kowasi1_work;

static kowasi1_work *kowasi1_work_get(sprite_status *pActwk) {
    return (kowasi1_work *)pActwk->actfree;
}

void kowasi(sprite_status *pActwk) {
    void (*tbl[3])(sprite_status *) = {&m_init, &m_wait, &m_down};

    tbl[pActwk->r_no0 / 2](pActwk);
}

void m_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 1;
    pActwk->sprhs = 16;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 24;
    pActwk->colino = 239;
    pActwk->sproffset = 50366;
    pActwk->patbase = pat_kowasi;
    pActwk->patno = pActwk->userflag.b.h;

    m_wait(pActwk);
}

void m_wait(sprite_status *pActwk) {
    kowasi1_work *work = kowasi1_work_get(pActwk);
    sprite_status *pActwk_w;
    kowasi1_work *fragment_work;
    sprite_status *pPlayerwk;
    Uint8 *pTbl0pat;
    Sint16 *pTbl0dposi;
    Sint32 *pTbl0speed;
    Sint32 speedx;
    Sint32 i;

    Uint8 tbl0pat[64] = {8,  9,  8,  12, 13, 12, 0,  0,  8,  9,  8,  10, 11,
                         10, 0,  0,  10, 11, 10, 10, 11, 10, 0,  0,  10, 11,
                         10, 12, 13, 12, 0,  0,  9,  8,  9,  13, 12, 13, 0,
                         0,  9,  8,  9,  11, 10, 11, 0,  0,  11, 10, 11, 11,
                         10, 11, 0,  0,  11, 10, 11, 13, 12, 13, 0,  0};

    Sint16 tbl0dposi[12] = {-8, -16, 0, 16, 0, 32, 16, 0, 16, 16, 16, 32};

    Sint32 tbl0speed[12] = {-194180, -84144,  -252434, 0, -194180, 84144,
                            -194180, -113595, -223307, 0, -194180, 113595};

    pPlayerwk = &actwk[0];
    if (pActwk->colicnt) {
        pActwk->colino = pActwk->colicnt = 0;
        pActwk->r_no0 += 2;
        work->saved_player_xspeed = pPlayerwk->xspeed.w;
        work->saved_player_yspeed = pPlayerwk->yspeed.w;
    } else {
        hitchk(pActwk, pPlayerwk);
        actionsub(pActwk);
        frameout_s(pActwk);
        return;
    }

    soundset(176);
    pPlayerwk = &actwk[0];
    pPlayerwk->xspeed.w /= 2;
    pTbl0pat = &tbl0pat[pActwk->userflag.b.h * 8];

    pActwk_w = pActwk;
    pTbl0dposi = tbl0dposi;
    pTbl0speed = tbl0speed;
    for (i = 0; i <= 5; ++i) {
        if (i != 0) {
            if (actwkchk(&pActwk_w) != 0)
                break;
            pActwk_w->actno = pActwk->actno;
            pActwk_w->r_no0 = pActwk->r_no0;
            pActwk_w->xposi.w.h = pActwk->xposi.w.h;
            pActwk_w->yposi.w.h = pActwk->yposi.w.h;
            pActwk_w->actflg = pActwk->actflg;
            pActwk_w->sprpri = pActwk->sprpri;
            pActwk_w->patbase = pActwk->patbase;
            pActwk_w->sproffset = pActwk->sproffset;
        }
        pActwk_w->sprhs = 8;
        pActwk_w->sprhsize = 8;
        pActwk_w->sprvsize = 8;
        pActwk_w->patno = *pTbl0pat++;
        pActwk_w->xposi.w.h += *pTbl0dposi++;
        pActwk_w->yposi.w.h += *pTbl0dposi++;
        speedx = *pTbl0speed++;
        fragment_work = kowasi1_work_get(pActwk_w);
        fragment_work->y_velocity = *pTbl0speed++;
        if (pPlayerwk->xspeed.w < 0)
            speedx *= -1;
        fragment_work->x_velocity = speedx;
    }
    m_down(pActwk);
}

void m_down(sprite_status *pActwk) {
    kowasi1_work *work = kowasi1_work_get(pActwk);
    sprite_status *pPlayerwk;
    Sint16 y;

    work->y_velocity += 16384;
    pActwk->xposi.l += work->x_velocity;
    pActwk->yposi.l += work->y_velocity;
    pPlayerwk = &actwk[0];
    y = pPlayerwk->yposi.w.h;
    y -= pActwk->yposi.w.h;
    if (y > -224)
        actionsub(pActwk);
    else
        frameout(pActwk);
}
