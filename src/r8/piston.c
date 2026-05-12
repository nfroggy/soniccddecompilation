#include "../equ.h"
#include "piston.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../ridechk.h"

#if defined(R82)
#define SPRITE_PISTON_BASE 317
#elif defined(R83)
#define SPRITE_PISTON_BASE 310
#else
#define SPRITE_PISTON_BASE 308
#endif

static void piston_init(sprite_status *pistonwk);
static void init_piston0(sprite_status *pistonwk);
static void init_piston1(sprite_status *pistonwk);
static void piston_move(sprite_status *pistonwk);

static sprite_pattern pis00 = {1, {{-32, -31, 0, SPRITE_PISTON_BASE}}};
static sprite_pattern pis01 = {3,
                               {{-32, -95, 0, SPRITE_PISTON_BASE},
                                {-32, -31, 0, SPRITE_PISTON_BASE},
                                {-32, 33, 0, SPRITE_PISTON_BASE}}};
static sprite_pattern pis02 = {1, {{-32, -19, 0, SPRITE_PISTON_BASE + 1}}};
static sprite_pattern pis03 = {1, {{-16, -20, 0, 0}}};
sprite_pattern *pistonpat[4] = {&pis00, &pis01, &pis02, &pis03};

void piston(sprite_status *pistonwk) {
    void (*piston_tbl[2])(sprite_status *) = {&piston_init, &piston_move};
    piston_tbl[pistonwk->r_no0 / 2](pistonwk);
    actionsub(pistonwk);
    frameout_s(pistonwk);
}

static void piston_init(sprite_status *pistonwk) {
    char tbl_index;
    Sint16 cal_rad, cal_radius, sin_tmp, cos_tmp;
    Sint32 sin_data;
    void (*act_tbl[3])(sprite_status *) = {&init_piston0, &init_piston1,
                                           &init_piston0};
    Sint16 rad_tbl[4] = {0, 256, 512, 768};

    Sint16 radius_tbl[2] = {32, 48};

    pistonwk->r_no0 += 2;
    pistonwk->actflg |= 4;
    pistonwk->patbase = pistonpat;

    pistonwk->sprpri = 3;
    ((Sint16 *)pistonwk)[26] = pistonwk->yposi.w.h;
    pistonwk->yspeed.w = pistonwk->xspeed.w = 0;

    tbl_index = pistonwk->userflag.b.h & 48;
    tbl_index >>= 4;

    cal_rad = ((Sint16 *)pistonwk)[27] = rad_tbl[tbl_index];
    tbl_index = pistonwk->userflag.b.h & 64;
    tbl_index >>= 6;
    cal_radius = ((Sint16 *)pistonwk)[29] = radius_tbl[tbl_index];
    sinset((char)(cal_rad >> 2), &sin_tmp, &cos_tmp);
    sin_data = sin_tmp;
    sin_data *= cal_radius;
    sin_data >>= 8;
    sin_data += ((Sint16 *)pistonwk)[26];
    act_tbl[pistonwk->userflag.b.h & 3](pistonwk);
}

static Sint16 spd_tbl[4] = {4, 8, 16, 32};

static void init_piston0(sprite_status *pistonwk) {
    pistonwk->patno = 0;
    pistonwk->sprhsize = pistonwk->sprvsize = 32;
    ((Sint16 *)pistonwk)[28] = spd_tbl[(pistonwk->userflag.b.h & 12) / 4];
}

static void init_piston1(sprite_status *pistonwk) {
    Uint8 delay_time_tbl[2] = {60, 90};

    pistonwk->patno = 1;
    pistonwk->sprhsize = 32;
    pistonwk->sprvsize = 96;
    ((Sint16 *)pistonwk)[28] = spd_tbl[(pistonwk->userflag.b.h & 4) / 4];

    pistonwk->actfree[14] = delay_time_tbl[(pistonwk->userflag.b.h & 8) / 8];

    pistonwk->actfree[20] = 0;
}

static void piston_move(sprite_status *pistonwk) {
    Sint16 cal_posi, cal_rad, sin_tmp, cos_tmp;
    Sint32 stack_yposi, sin_data, cal_yposi;
    Sint16 jump_tbl[2] = {-4096, -2560};

    if (pistonwk->yspeed.w < 0)
        ride_on_chk(pistonwk, &actwk[0]);
    stack_yposi = pistonwk->yposi.l;
    if ((pistonwk->userflag.b.h & 3) == 1) {

        if (pistonwk->actfree[20] & 128) {
            cal_posi = pistonwk->yposi.w.h;
            if ((char)--pistonwk->actfree[16] >= 0)
                goto label1;

            pistonwk->actfree[20] &= 127;
        } else {
            cal_posi = ((Sint16 *)pistonwk)[27];
            if (cal_posi == 256 || cal_posi == 768) {

                pistonwk->actfree[20] |= 128;
                pistonwk->actfree[16] = pistonwk->actfree[14];
                cal_posi = pistonwk->yposi.w.h;
                goto label1;
            }
        }
    }

    cal_rad = ((Sint16 *)pistonwk)[27] + ((Sint16 *)pistonwk)[28];

    cal_rad &= 1023;
    ((Sint16 *)pistonwk)[27] = cal_rad;
    sinset((char)(cal_rad >> 2), &sin_tmp, &cos_tmp);
    sin_data = sin_tmp;
    sin_data *= ((Sint16 *)pistonwk)[29];
    sin_data >>= 8;
    sin_data += ((Sint16 *)pistonwk)[26];
    cal_posi = sin_data;
label1:
    pistonwk->yposi.w.h = cal_posi;
    cal_yposi = (cal_posi << 16) - stack_yposi;
    pistonwk->yspeed.w = cal_yposi >> 8;
    if ((pistonwk->userflag.b.h & 3) == 1) {

        if (((Sint16 *)pistonwk)[27] == 528) {

            if (ride_on_chk(pistonwk, &actwk[0]) == 0)
                return;

            actwk[0].yspeed.w =
                jump_tbl[((Uint8)pistonwk->userflag.b.h & 128) >> 7];
            actwk[0].cddat |= 2;
            actwk[0].cddat &= 247;
            actwk[0].mstno.b.h = 16;
            pistonwk->cddat &= 247;
        }
    }
    if (pistonwk->yspeed.w >= 0)
        ride_on_chk(pistonwk, &actwk[0]);
}
