#include "../equ.h"
#include "playsp8.h"
#include "../action.h"
#include "../actset.h"
#include "../fcol.h"
#include "../ridechk.h"

static void chibi_init(sprite_status *smallwk);
static void chibi_move(sprite_status *smallwk);
static Uint8 playsp_chk(Sint16 block_no);

static sprite_pattern chb00 = {1, {{-16, -20, 0, 0}}};
sprite_pattern *chibipat[1] = {&chb00};

void chibi_coli(sprite_status *smallwk) {
    void (*tbl[2])(sprite_status *) = {&chibi_init, &chibi_move};

    tbl[smallwk->r_no0 / 2](smallwk);
    actionsub(smallwk);
    frameout_s(smallwk);
}

static void chibi_init(sprite_status *smallwk) {
    smallwk->r_no0 += 2;
    smallwk->actflg |= 4;
    smallwk->patbase = chibipat;
    smallwk->sprhs = smallwk->sprhsize = smallwk->sprvsize = 16;
    chibi_move(smallwk);
}

static void chibi_move(sprite_status *smallwk) { hitchk(smallwk, &actwk[0]); }

void playsp(void) {
    Sint16 block_no, block_tmp, x_add;

    if (stageno.b.l == 2)
        return;

    block_tmp =
        scramapad(&actwk[0], actwk[0].xposi.w.h - (Sint16)actwk[0].sprhs,
                  actwk[0].yposi.w.h + (Sint16)actwk[0].sprvsize + 2);

    block_no = block_tmp & 2047;
    if (!playsp_chk(block_no)) {
        block_tmp =
            scramapad(&actwk[0], actwk[0].xposi.w.h + (Sint16)actwk[0].sprhs,
                      actwk[0].yposi.w.h + (Sint16)actwk[0].sprvsize + 2);

        block_no = block_tmp & 2047;
        if (playsp_chk(block_no) == 0)
            return;
    }
    if (!(block_tmp & 2048))
        x_add = 1;
    else
        x_add = -1;
    if (block_no == 276)
        x_add = -x_add;
    actwk[0].xposi.w.h += x_add;
}

static Uint8 playsp_chk(Sint16 block_no) {
    Sint16 i = 0;
    Sint16 belt[7] = {270, 271, 272, 273, 274, 275, 276};

    for (; i < 7; ++i)
        if (block_no == belt[i])
            return 1;
    return 0;
}
