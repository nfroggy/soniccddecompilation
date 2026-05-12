#include "equ.h"
#include "block.h"
#include "action.h"
#include "dummy.h"
#include "playsub.h"
#include "ridechk.h"

void (*block_tbl[7])(sprite_status *) = {
    &block_init,  &block_move,  &block_move2, &block_move3,
    &block_move4, &block_move5, &block_move6};
Uint8 blockchg0[3] = {2, 5, 255};
Uint8 blockchg1[10] = {2, 1, 5, 2, 5, 3, 5, 4, 5, 252};
Uint8 blockchg2[10] = {2, 1, 0, 2, 0, 3, 0, 4, 0, 252};
Uint8 *blockchg[3] = {blockchg0, blockchg1, blockchg2};

#if defined(R31) || defined(R32)
#define SPRITE_BLOCK_BASE 404
#elif defined(R33)
#define SPRITE_BLOCK_BASE 408
#elif defined(R6)
#define SPRITE_BLOCK_BASE 403
#else
#define SPRITE_BLOCK_BASE 294
#endif

sprite_pattern block0 = {0, {{-12, -12, 0, SPRITE_BLOCK_BASE}}};
sprite_pattern block1 = {1, {{-12, -12, 0, SPRITE_BLOCK_BASE}}};
sprite_pattern block2 = {1, {{-12, -12, 0, SPRITE_BLOCK_BASE + 1}}};
sprite_pattern block3 = {1, {{-12, -12, 0, SPRITE_BLOCK_BASE + 2}}};
sprite_pattern block4 = {1, {{-12, -12, 0, SPRITE_BLOCK_BASE + 3}}};
sprite_pattern block5 = {1, {{-12, -12, 0, SPRITE_BLOCK_BASE + 4}}};
sprite_pattern *blockpat[6] = {&block0, &block1, &block2,
                               &block3, &block4, &block5};

void block(sprite_status *pActwk) {
    Sint16 iD0, iD1;

    block_tbl[pActwk->r_no0 / 2](pActwk);
    iD0 = pActwk->xposi.w.h & 65408;
    iD1 = scra_h_posit.w.h - 128;
    iD1 &= -128;
    iD0 -= iD1;
    if ((Uint16)iD0 > 640)
        frameout(pActwk);
}

void block_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->patbase = blockpat;

    pActwk->sprpri = 1;
    pActwk->sprhsize = 12;
    pActwk->sprvsize = 12;
    pActwk->patno = 5;
    block_move(pActwk);
}

void block_move(sprite_status *pActwk) {
    block_ride(pActwk);
    if (time_flag != 0)
        if (time_flag == 2) {
            if (pActwk->cddat & 8)
                goto label1;
        } else {

            pActwk->patno = 0;
            if (pActwk->cddat & 8) {
                pActwk->r_no0 = 6;
                pActwk->mstno.b.h = 1;
            }
        }

    actionsub(pActwk);
    return;
label1:
    pActwk->r_no0 += 2;
    block_move2(pActwk);
}

void block_move2(sprite_status *pActwk) {
    Sint16 iD0;

    block_ride(pActwk);
    pActwk->yposi.w.h += 2;
    iD0 = scra_v_posit.w.h;
    iD0 += 224;
    if (iD0 < pActwk->yposi.w.h) {
        frameout(pActwk);
    } else {
        actionsub(pActwk);
    }
}

void block_move3(sprite_status *pActwk) {
    block_ride(pActwk);
    if (!(pActwk->cddat & 8)) {
        pActwk->r_no0 = 2;
        return;
    }

    patchg(pActwk, blockchg);
    actionsub(pActwk);
}

void block_move4(sprite_status *pActwk) {
    pActwk->mstno.b.h = 0;
    block_ride(pActwk);
    if (!(pActwk->cddat & 8)) {
        pActwk->r_no0 += 2;
        pActwk->mstno.b.h = 2;
        return;
    }

    patchg(pActwk, blockchg);
    actionsub(pActwk);
}

void block_move5(sprite_status *pActwk) {
    block_ride(pActwk);
    patchg(pActwk, blockchg);
    actionsub(pActwk);
}

void block_move6(sprite_status *pActwk) { pActwk->r_no0 = 2; }

void block_ride(sprite_status *pActwk) {
    ride_on_chk_b(pActwk, actwk);
    ride_on_chk_b(pActwk, actwk);
}

void ride_on_chk_b(sprite_status *pActwk, sprite_status *pPlaywk) {
    ridechk(pActwk, pPlaywk);
}
