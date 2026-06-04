#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
short_union gameflag;
short_union stageno;
ushort_union swdata;
Uint8 bossstart;
Uint8 plautoflag;
Uint8 time_flag;
Uint8 clchgtim[7];
PALETTEENTRY palette_work[64];
PALETTEENTRY *lpcolorwk = palette_work;
Sint16 scralim_down;
Sint16 scralim_n_down;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int memset_count;
static void *memset_dest;
static Uint8 memset_value;
static Sint32 memset_size;

void actionsub(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
static void memset_callback(void *dest, Uint8 value, Sint32 size);
void (*sMemSet)(void *, Uint8, Sint32) = memset_callback;

#include "src/r8/emie8.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

static void memset_callback(void *dest, Uint8 value, Sint32 size) {
    ++memset_count;
    memset_dest = dest;
    memset_value = value;
    memset_size = size;
    memset(dest, value, (size_t)size);
}

static void reset_emie8_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(&gameflag, 0, sizeof(gameflag));
    memset(&stageno, 0, sizeof(stageno));
    memset(&swdata, 0, sizeof(swdata));
    memset(clchgtim, 0, sizeof(clchgtim));
    memset(palette_work, 0, sizeof(palette_work));
    bossstart = 0;
    plautoflag = 0;
    time_flag = 0;
    scralim_down = 0;
    scralim_n_down = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    memset_count = 0;
    memset_dest = 0;
    memset_value = 0;
    memset_size = 0;
}

static void reset_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    memset_count = 0;
    memset_dest = 0;
    memset_value = 0;
    memset_size = 0;
}

static void test_emie8_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, emie8_pchg[0] == pchg0);
    TEST_ASSERT_EQ_INT(ctx, 3, pchg0[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, pchg0[1]);
    TEST_ASSERT_EQ_INT(ctx, 5, pchg0[6]);
    TEST_ASSERT_EQ_INT(ctx, 255, pchg0[7]);
    TEST_ASSERT_TRUE(ctx, emie8_pat[0] == &pat00);
    TEST_ASSERT_TRUE(ctx, emie8_pat[7] == &pat07);
    TEST_ASSERT_EQ_INT(ctx, -16, pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -20, pat00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 513, pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 516, pat03.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 520, pat07.spra[0].index);
}

static void test_emie8_initializes_and_starts_walking(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_emie8_state();
    actor->xposi.w.h = 3000;
    actor->yposi.w.h = 460;
    actwk[0].xposi.w.h = 3900;

    emie8(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 20, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 8736, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == emie8_pat);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 320, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 3001, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, plautoflag);
    TEST_ASSERT_EQ_INT(ctx, 2056, swdata.w);
    TEST_ASSERT_EQ_INT(ctx, -4, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, -4, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == emie8_pchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
}

static void test_emie8_init_waits_when_bossstart_is_set(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_emie8_state();
    bossstart = 1;

    emie8(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_emie8_player_auto_control_sequence(test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *player = &actwk[0];

    reset_emie8_state();
    player->xposi.w.h = 3984;
    emie8_snc_r(actor, player);
    TEST_ASSERT_EQ_INT(ctx, 2, emie8_get_work(actor)->scripted_control_state);
    TEST_ASSERT_EQ_INT(ctx, 1028, swdata.w);

    player->xposi.w.h = 3952;
    emie8_snc_l(actor, player);
    TEST_ASSERT_EQ_INT(ctx, 4, emie8_get_work(actor)->scripted_control_state);
    TEST_ASSERT_EQ_INT(ctx, 0, swdata.w);

    actor->r_no0 = 6;
    emie8_snc_w(actor, player);
    TEST_ASSERT_EQ_INT(ctx, 6, emie8_get_work(actor)->scripted_control_state);
    TEST_ASSERT_EQ_INT(ctx, 5, player->mstno.b.h);

    player->patcnt = 3;
    emie8_snc_pata(actor, player);
    TEST_ASSERT_EQ_INT(ctx, 16, player->patcnt);
}

static void test_emie8_jump_sequence_and_landing(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_emie8_state();
    actor->r_no0 = 4;
    actor->xposi.w.h = 3743;
    actor->yposi.w.h = 450;
    actor->xspeed.w = 320;

    emie8(actor);

    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 6, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -704, actor->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 447, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_logs();
    actor->r_no0 = 8;
    actor->yposi.w.h = 460;
    actor->yspeed.w = 0;

    emie8(actor);

    TEST_ASSERT_EQ_INT(ctx, 10, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 7, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 320, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 460, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->mstno.w);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_emie8_final_walk_flashes_palette_then_finishes(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_emie8_state();
    actor->r_no0 = 14;
    actor->xposi.w.h = 3808;
    actor->xspeed.w = 320;
    emie8_get_work(actor)->flash_timer = 7;
    emie8_get_work(actor)->flash_count = 0;
    palette_work[0].peRed = 0;
    palette_work[0].peGreen = 224;
    palette_work[0].peBlue = 64;
    palette_work[1].peRed = 192;
    palette_work[1].peGreen = 128;
    palette_work[1].peBlue = 224;

    emie8(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, memset_count);
    TEST_ASSERT_TRUE(ctx, memset_dest == clchgtim);
    TEST_ASSERT_EQ_INT(ctx, 127, memset_value);
    TEST_ASSERT_EQ_INT(ctx, (int)sizeof(clchgtim), memset_size);
    TEST_ASSERT_EQ_INT(ctx, 127, clchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 32, palette_work[0].peRed);
    TEST_ASSERT_EQ_INT(ctx, 224, palette_work[0].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 96, palette_work[0].peBlue);
    TEST_ASSERT_EQ_INT(ctx, 224, palette_work[1].peRed);
    TEST_ASSERT_EQ_INT(ctx, 160, palette_work[1].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 224, palette_work[1].peBlue);
    TEST_ASSERT_EQ_INT(ctx, 1, emie8_get_work(actor)->flash_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_table == emie8_pchg);

    reset_logs();
    emie8_get_work(actor)->flash_timer = 7;
    emie8_get_work(actor)->flash_count = 7;

    emie8(actor);

    TEST_ASSERT_EQ_INT(ctx, 128, time_flag);
    TEST_ASSERT_EQ_INT(ctx, 2, gameflag.w);
    TEST_ASSERT_EQ_INT(ctx, 1792, stageno.w);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_emie8_tables_capture_literal_data(&ctx);
    test_emie8_initializes_and_starts_walking(&ctx);
    test_emie8_init_waits_when_bossstart_is_set(&ctx);
    test_emie8_player_auto_control_sequence(&ctx);
    test_emie8_jump_sequence_and_landing(&ctx);
    test_emie8_final_walk_flashes_palette_then_finishes(&ctx);
TEST_MAIN_END
