#include "../equ.h"
#include "kanabun.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../playsub.h"
#include "../suicide.h"

typedef struct {
    Sint32 y_base;
    Sint16 wave_angle;
    Sint16 wave_angle_step;
    Uint8 **patch_data;
    Sint16 origin_x;
    Sint32 x_speed;
} kanabun_work;

static kanabun_work *kanabun_get_work(sprite_status *pActwk) {
    return (kanabun_work *)pActwk->actfree;
}

static sprite_pattern pat_e00 = {1, {{-16, -16, 128, 415}}};
static sprite_pattern pat_e01 = {1, {{-16, -16, 128, 416}}};
static sprite_pattern pat_e02 = {1, {{-12, -12, 128, 417}}};
static sprite_pattern pat_e03 = {1, {{-12, -12, 128, 418}}};
static sprite_pattern pat_e04 = {1, {{-12, -12, 0, 419}}};
static sprite_pattern pat_e05 = {1, {{-12, -12, 0, 420}}};
static sprite_pattern pat_e06 = {1, {{-12, -12, 0, 421}}};
static sprite_pattern pat_e07 = {1, {{-12, -12, 0, 422}}};
sprite_pattern *pat_kanabun_e[8] = {&pat_e00, &pat_e01, &pat_e02, &pat_e03,
                                    &pat_e04, &pat_e05, &pat_e06, &pat_e07};
static sprite_pattern pat_b00 = {1, {{-16, -16, 128, 407}}};
static sprite_pattern pat_b01 = {1, {{-16, -16, 128, 408}}};
static sprite_pattern pat_b02 = {1, {{-12, -12, 128, 409}}};
static sprite_pattern pat_b03 = {1, {{-12, -12, 128, 410}}};
static sprite_pattern pat_b04 = {1, {{-12, -12, 0, 411}}};
static sprite_pattern pat_b05 = {1, {{-12, -12, 0, 412}}};
static sprite_pattern pat_b06 = {1, {{-12, -12, 0, 413}}};
static sprite_pattern pat_b07 = {1, {{-12, -12, 0, 414}}};
sprite_pattern *pat_kanabun_b[8] = {&pat_b00, &pat_b01, &pat_b02, &pat_b03,
                                    &pat_b04, &pat_b05, &pat_b06, &pat_b07};
static Uint8 pchg_e00[66] = {1, 6, 7, 6, 7, 6, 7, 6, 7, 4, 5, 4, 5, 4, 5,  4, 5,
                             2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 0,  1, 0,
                             1, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3,  4, 5,
                             4, 5, 4, 5, 4, 5, 6, 7, 6, 7, 6, 7, 6, 7, 255};
static Uint8 *pchg_e[1] = {pchg_e00};
static Uint8 pchg_b00[66] = {3, 6, 7, 6, 7, 6, 7, 6, 7, 4, 5, 4, 5, 4, 5,  4, 5,
                             2, 3, 2, 3, 2, 3, 2, 3, 0, 1, 0, 1, 0, 1, 0,  1, 0,
                             1, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3,  4, 5,
                             4, 5, 4, 5, 4, 5, 6, 7, 6, 7, 6, 7, 6, 7, 255};
static Uint8 *pchg_b[1] = {pchg_b00};
static char tbl0[4] = {47, 48, 48, 0};

void kanabun(sprite_status *pActwk) {
    int_union poswk;
    Sint32 spdwk;
    Sint16 sinwk, coswk;
    kanabun_work *pWork = kanabun_get_work(pActwk);

    if (enemy_suicide(pActwk))
        return;

    if (!pActwk->r_no0) {

        pActwk->r_no0 += 2;
        pActwk->actflg |= 4;
        pActwk->sprpri = 3;
        pActwk->sproffset = 9193;
        pActwk->sprhs = 16;
        pActwk->sprhsize = 16;
        pActwk->sprvsize = 16;
        pWork->origin_x = pActwk->xposi.w.h;
        pWork->y_base =
            (Sint32)(((Uint32)(Uint16)pActwk->yposi.w.h << 16) |
                     (Uint16)-32768);

        if (!pActwk->userflag.b.h) {
            pActwk->patbase = pat_kanabun_e;
            pWork->patch_data = pchg_e;
            pWork->wave_angle_step = -512;
            pWork->x_speed = -16384;
        } else {
            pActwk->patbase = pat_kanabun_b;
            pWork->patch_data = pchg_b;
            pWork->wave_angle_step = -256;
            pWork->x_speed = -16384;
        }
    }

    poswk.l = spdwk = pActwk->xposi.l + pWork->x_speed;
    poswk.l = (Uint32)poswk.l >> 16 & 65535 | poswk.l << 16 & -65536;

    if ((poswk.w.l -= pWork->origin_x) < 0) {
        poswk.w.l *= -1;
    }

    if (poswk.w.l > 64) {
        pWork->x_speed *= -1;
        spdwk += pWork->x_speed;
        pActwk->actflg ^= 1;
        pActwk->cddat ^= 1;
        pWork->wave_angle = 0;
        pActwk->mstno.w = 255;
    }

    pActwk->xposi.l = spdwk;
    poswk.w.l = pWork->wave_angle + pWork->wave_angle_step;

    pWork->wave_angle = poswk.w.l;
    poswk.w.l = (Uint16)poswk.w.l >> 8;
    sinset(poswk.w.l, &sinwk, &coswk);
    poswk.w.l = sinwk;
    poswk.l = (Uint32)poswk.l >> 16 & 65535 | poswk.l << 16 & -65536;
    poswk.l >>= 2;
    poswk.l += pWork->y_base;
    pActwk->yposi.l = poswk.l;

    patchg(pActwk, pWork->patch_data);
    pActwk->colino = tbl0[pActwk->patno >> 1];
    actionsub(pActwk);
    frameout_s00(pActwk, pWork->origin_x);
}
