#include "../equ.h"
#include "swgun4.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../loader2.h"
#include "../ridechk.h"
#include "../ring.h"
#include "../score.h"
#include "playsub4.h"

typedef struct {
    union {
        struct {
            Uint8 triggered_sound;
            Uint8 press_ramp;
            Uint8 previous_pressed;
            Uint8 current_pressed;
            Sint16 origin_y;
            Uint8 switch_index;
            Uint8 reward_type;
            Sint16 origin_x;
            Uint8 reward_timer;
            Uint8 reward_subtimer;
            Sint16 master_index;
            Sint16 active_switch_index;
        } swgun;
        struct {
            Sint16 spin_angle;
            Sint16 spin_speed;
            Uint8 lifetime;
            Uint8 bounce_delay;
        } ring;
    };
} swgun4_work;

static swgun4_work *swgun4_get_work(sprite_status *pActwk) {
    return (swgun4_work *)pActwk->actfree;
}

static void swgun4_init(sprite_status *pActwk);
static void swgun4_move(sprite_status *pActwk);
static void swgun4_move1(sprite_status *pActwk);
static void switch_move(sprite_status *pActwk);
static void hariset(sprite_status *pActwk);
static void awaset(sprite_status *pActwk);
static void ring4set(sprite_status *pActwk);
static void bp1000(sprite_status *pActwk);
static void ring4_init(sprite_status *pActwk);
static void ring4_move(sprite_status *pActwk);
static void ring4_get(sprite_status *pActwk);
static void ring4_die(sprite_status *pActwk);
static void ring4_erase(sprite_status *pActwk);
static void tensu4_init(sprite_status *pActwk);
static void tensu4_move(sprite_status *pActwk);

static sprite_pattern pat0 = {1, {{-16, -8, 0, 481}}};
static sprite_pattern pat1 = {1, {{-16, 0, 0, 482}}};
static sprite_pattern pat2 = {1, {{-16, 0, 0, 483}}};
sprite_pattern *swgun4pat[3] = {&pat0, &pat1, &pat2};

void swgun4(sprite_status *pActwk) {
    void (*tbl[3])(sprite_status *) = {&swgun4_init, &swgun4_move,
                                       &swgun4_move1};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s00(pActwk, swgun4_get_work(pActwk)->swgun.origin_x);
}

static void swgun4_init(sprite_status *pActwk) {
    sprite_status *pNewActwk;
    Sint16 *a2;
    Sint16 i;
    Uint8 d1;
    swgun4_work *pWork = swgun4_get_work(pActwk);
    Sint16 tbl[12] = {0, 0, -64, 64, -128, 128, 64, -64, 96, -128, 128, -128};

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 1290;
    pActwk->sprpri = 3;
    pActwk->patbase = swgun4pat;
    pActwk->sprhsize = 14;
    pActwk->sprvsize = 8;

    if (pWork->swgun.switch_index == 0) {
        pWork->swgun.origin_x = pActwk->xposi.w.h;
        pWork->swgun.origin_y = pActwk->yposi.w.h;
        pWork->swgun.master_index = (Sint16)(pActwk - actwk);

        d1 = 1;
        for (i = 0; i <= 4; ++i) {
            if (actwkchk(&pNewActwk) == 0) {
                swgun4_work *pNewWork = swgun4_get_work(pNewActwk);

                pNewActwk->actno = 63;

                pNewWork->swgun.origin_x = pWork->swgun.origin_x;

                pNewWork->swgun.master_index = (Sint16)(pActwk - actwk);
                pNewWork->swgun.switch_index = d1;
                a2 = &tbl[d1 * 2];
                pNewActwk->xposi.w.h = pActwk->xposi.w.h + *a2++;
                pNewActwk->yposi.w.h = pActwk->yposi.w.h + *a2++;
                pNewWork->swgun.origin_y = pNewActwk->yposi.w.h;
            }
            ++d1;
        }
    }
}

static void swgun4_move(sprite_status *pActwk) {
    sprite_status *a1;
    Sint16 d0;
    swgun4_work *pWork = swgun4_get_work(pActwk);

    switch_move(pActwk);

    a1 = &actwk[pWork->swgun.master_index];
    d0 = swgun4_get_work(a1)->swgun.active_switch_index;
    if (d0 == 0)
        return;

    if (pWork->swgun.triggered_sound == 0) {
        soundset(191);
        pWork->swgun.triggered_sound = 255;
    }

    if ((Uint16)((Uint16)pWork->swgun.press_ramp + 16) < 256) {
        pWork->swgun.press_ramp += 16;
        return;
    }
    pWork->swgun.press_ramp += 16;

    pActwk->r_no0 += 2;
    ride_on_clr(pActwk, &actwk[0]);
    pWork->swgun.reward_subtimer = pWork->swgun.reward_timer = 0;
    pActwk->patno = 2;
}

static void swgun4_move1(sprite_status *pActwk) {
    sprite_status *a1;
    void (*tbl[4])(sprite_status *) = {&hariset, &bp1000, &awaset, &ring4set};

    a1 = &actwk[swgun4_get_work(pActwk)->swgun.master_index];
    tbl[swgun4_get_work(a1)->swgun.reward_type](pActwk);
}

static void switch_move(sprite_status *pActwk) {
    sprite_status *pPlayerwk;
    sprite_status *pActwk_m;
    Uint16 d0, d1;
    Sint16 r;
    swgun4_work *pWork = swgun4_get_work(pActwk);
    Uint8 tbl[42] = {2, 1, 0, 3, 3, 2, 1, 0, 3, 3, 2, 2, 0, 3,
                     3, 2, 2, 1, 2, 2, 1, 0, 3, 3, 3, 2, 2, 1,
                     0, 3, 3, 3, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0};

    pWork->swgun.previous_pressed = pWork->swgun.current_pressed;

    pPlayerwk = &actwk[0];
    r = hitchk_u(pActwk, pPlayerwk);
    if (r) {
        pWork->swgun.current_pressed = 255;
    } else {
        pWork->swgun.current_pressed = 0;
        goto label1;
    }
    if (pPlayerwk->yspeed.w < 0)
        goto label1;
    if (pWork->swgun.triggered_sound)
        goto label1;
    pActwk_m = &actwk[pWork->swgun.master_index];
    swgun4_get_work(pActwk_m)->swgun.active_switch_index =
        (Sint16)(pActwk - actwk);

    d0 = pWork->swgun.switch_index;
    d0 *= 6;
    d1 = pltime.b.b3;
    d1 &= 255;
    d1 /= 10;
    d1 &= 7;
    d0 += d1;
    pWork->swgun.reward_type = tbl[d0];

    swgun4_get_work(pActwk_m)->swgun.reward_type = pWork->swgun.reward_type;

label1:
    if (pWork->swgun.previous_pressed == 0 &&
        pWork->swgun.current_pressed == 255) {
        pPlayerwk->yposi.w.h += 8;
        pActwk->yposi.w.h += 4;
        pActwk->patno = 1;
        pActwk->sprvsize -= 4;
    }

    if (pWork->swgun.previous_pressed == 255 &&
        pWork->swgun.current_pressed == 0) {
        pPlayerwk->yposi.w.h -= 8;
        pActwk->yposi.w.h -= 4;
        pActwk->patno = 0;
        pActwk->sprvsize += 4;
    }
}

static void hariset(sprite_status *pActwk) {
    sprite_status *pMaster;
    sprite_status *pSlave;
    sprite_status *pNewActwk;
    swgun4_work *pWork = swgun4_get_work(pActwk);

    if (pWork->swgun.reward_timer) {
        frameout(pActwk);
        return;
    }
    pMaster = &actwk[pWork->swgun.master_index];
    pSlave = &actwk[swgun4_get_work(pMaster)->swgun.active_switch_index];
    if (pActwk != pSlave) {
        if (pWork->swgun.switch_index != 4) {
            if (actwkchk(&pNewActwk) == 0) {
                pNewActwk->actno = 34;
                pNewActwk->xposi.w.h = pActwk->xposi.w.h;

                swgun4_get_work(pNewActwk)->swgun.origin_x =
                    pWork->swgun.origin_x;
                pNewActwk->yposi.w.h = pWork->swgun.origin_y;
            }
        }
    }
    ++pWork->swgun.reward_timer;
}

static void awaset(sprite_status *pActwk) {
    sprite_status *pMaster;
    sprite_status *pSlave;
    sprite_status *pNewActwk;
    swgun4_work *pWork = swgun4_get_work(pActwk);

    pMaster = &actwk[pWork->swgun.master_index];
    pSlave = &actwk[swgun4_get_work(pMaster)->swgun.active_switch_index];
    if (pActwk == pSlave) {
        if ((Uint16)((Uint16)pWork->swgun.reward_timer + 1) > 255) {
            frameout(pActwk);
        }
        ++pWork->swgun.reward_timer;
        return;
    }
    if ((Uint16)((Uint16)pWork->swgun.reward_subtimer + 4) <= 255) {
        pWork->swgun.reward_subtimer += 4;
        return;
    }
    pWork->swgun.reward_subtimer += 4;
    if (actwkchk(&pNewActwk) == 0) {
        pNewActwk->actno = 32;
        pNewActwk->xposi.w.h = pActwk->xposi.w.h;
        pNewActwk->yposi.w.h = pWork->swgun.origin_y;
        pNewActwk->userflag.b.h = 2;
    }
    ++pWork->swgun.reward_timer;
    if (pWork->swgun.reward_timer >= 2) {
        frameout(pActwk);
    }
}

static void ring4set(sprite_status *pActwk) {
    sprite_status *pMaster;
    sprite_status *pNewActwk;
    Sint16 d0;
    swgun4_work *pWork = swgun4_get_work(pActwk);
    char tbl[22] = {-32, 0, 32, 64, 96, 127, 96, 64, 32, 0, -32,
                    -32, 0, 32, 64, 96, 127, 96, 64, 32, 0, -32};

    pMaster = &actwk[pWork->swgun.master_index];
    if (pActwk != pMaster) {
        frameout(pActwk);
        return;
    }
    if ((Uint16)pWork->swgun.reward_timer + 16 < 256) {
        pWork->swgun.reward_timer += 16;
        return;
    }
    pWork->swgun.reward_timer += 16;
    if (pWork->swgun.reward_subtimer >= 21) {
        frameout(pActwk);
        return;
    }

    if (actwkchk(&pNewActwk) == 0) {
        pNewActwk->actno = 64;
        d0 = pActwk->yposi.w.h;
        d0 -= 384;
        pNewActwk->yposi.w.h = d0;
        d0 = tbl[pWork->swgun.reward_subtimer];
        d0 += pActwk->xposi.w.h;
        pNewActwk->xposi.w.h = d0;
    }
    ++pWork->swgun.reward_subtimer;
}

static void bp1000(sprite_status *pActwk) {
    sprite_status *pMaster;
    sprite_status *pSlave;
    sprite_status *pNewActwk;
    swgun4_work *pWork = swgun4_get_work(pActwk);

    if (pWork->swgun.reward_timer) {
        frameout(pActwk);
        return;
    }
    pMaster = &actwk[pWork->swgun.master_index];
    pSlave = &actwk[swgun4_get_work(pMaster)->swgun.active_switch_index];
    if (pMaster != pSlave) {
        scoreup(100);
        if (actwkchk(&pNewActwk) == 0) {
            pNewActwk->actno = 65;
            pNewActwk->xposi.w.h = pActwk->xposi.w.h;
            pNewActwk->yposi.w.h = pActwk->yposi.w.h;
        }
    }
    ++pWork->swgun.reward_timer;
}

static sprite_pattern ringsp0 = {1, {{-8, -8, 0, 362}}};
static sprite_pattern ringsp1 = {1, {{-8, -8, 0, 363}}};
static sprite_pattern ringsp2 = {1, {{-4, -8, 0, 364}}};
static sprite_pattern ringsp3 = {1, {{-8, -8, 0, 365}}};
static sprite_pattern ringsp4 = {1, {{-8, -8, 0, 366}}};
static sprite_pattern ringsp5 = {1, {{-8, -8, 0, 367}}};
static sprite_pattern ringsp6 = {1, {{-8, -8, 0, 368}}};
static sprite_pattern ringsp7 = {1, {{-8, -8, 0, 369}}};
static sprite_pattern ringsp8;
static sprite_pattern *ringpat[9] = {&ringsp0, &ringsp1, &ringsp2,
                                     &ringsp3, &ringsp4, &ringsp5,
                                     &ringsp6, &ringsp7, &ringsp8};
static Uint8 pchg0[6] = {5, 4, 5, 6, 7, 252};
static Uint8 *ringchg[1] = {pchg0};

void ring4(sprite_status *pActwk) {
    void (*tbl[5])(sprite_status *) = {&ring4_init, &ring4_move, &ring4_get,
                                       &ring4_die, &ring4_erase};

    tbl[pActwk->r_no0 / 2](pActwk);
}

static void ring4_init(sprite_status *pActwk) {
    swgun4_work *pWork = swgun4_get_work(pActwk);

    pActwk->r_no0 += 2;
    pActwk->sprvsize = 8;
    pActwk->sprhs = 8;
    pActwk->patbase = ringpat;
    pActwk->sproffset = 42926;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->colino = 71;
    pActwk->sprhsize = 8;
    pWork->ring.spin_speed = 2048;
    pActwk->yspeed.w = 256;
    pWork->ring.lifetime = 255;
    pWork->ring.bounce_delay = 48;

    ring4_move(pActwk);
}

static void ring4_move(sprite_status *pActwk) {
    Sint16 d0, d1;
    swgun4_work *pWork = swgun4_get_work(pActwk);

    speedset2(pActwk);
    pActwk->yspeed.w += 24;

    if (pWork->ring.bounce_delay) {
        --pWork->ring.bounce_delay;
        if (pWork->ring.bounce_delay)
            goto label1;
    }

    d1 = emycol_d(pActwk);
    if (d1 < 0) {
        pActwk->yposi.w.h += d1;
        d0 = pActwk->yspeed.w;
        d0 >>= 2;
        pActwk->yspeed.w -= d0;
        pActwk->yspeed.w *= -1;
    }
label1:
    d0 = pWork->ring.spin_angle;
    d0 += pWork->ring.spin_speed;
    pWork->ring.spin_angle = d0;
    d0 >>= 12;
    d0 &= 3;
    pActwk->patno = (Uint8)d0;
    pWork->ring.spin_speed -= 8;

    --pWork->ring.lifetime;
    if (pWork->ring.lifetime == 0) {
        ring4_erase(pActwk);
        return;
    }
    d0 = scralim_down;
    d0 += 224;
    if (d0 < pActwk->yposi.w.h) {
        ring4_erase(pActwk);
        return;
    }
    actionsub(pActwk);
}

static void ring4_get(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->colino = 0;
    pActwk->sprpri = 1;
    ringgetsub();

    ring4_die(pActwk);
}

static void ring4_die(sprite_status *pActwk) {
    patchg(pActwk, ringchg);
    actionsub(pActwk);
}

static void ring4_erase(sprite_status *pActwk) { frameout(pActwk); }

static sprite_pattern pat00 = {1, {{-8, -4, 0, 484}}};
sprite_pattern *tensu4pat[1] = {&pat00};

void tensu4(sprite_status *pActwk) {
    void (*tbl[2])(sprite_status *) = {&tensu4_init, &tensu4_move};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
}

static void tensu4_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->patbase = tensu4pat;
    pActwk->sproffset = 878;
    pActwk->actflg = 4;
    pActwk->sprpri = 1;
    pActwk->sprhsize = 8;
    pActwk->yspeed.w = -768;

    tensu4_move(pActwk);
}

static void tensu4_move(sprite_status *pActwk) {
    if (pActwk->yspeed.w >= 0) {
        frameout(pActwk);
    } else {
        speedset2(pActwk);
        pActwk->yspeed.w += 24;
    }
}
