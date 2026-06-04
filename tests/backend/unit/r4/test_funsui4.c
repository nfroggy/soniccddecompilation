#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Sint16 waterposi;
PALETTEENTRY colorwk[64];
PALETTEENTRY *lpcolorwk = colorwk;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int actwkchk_count;
static sprite_status *actwkchk_queue[4];
static int actwkchk_queue_count;
static int actwkchk_queue_pos;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int ridechk_count;
static sprite_status *ridechk_actor;
static sprite_status *ridechk_player;

void actionsub(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r4/funsui4.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_queue_pos >= actwkchk_queue_count) {
        *ppActwk = 0;
        return 1;
    }

    *ppActwk = actwkchk_queue[actwkchk_queue_pos++];
    return 0;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ridechk_count;
    ridechk_actor = pActwk;
    ridechk_player = pPlayerwk;
    return 0;
}

static void reset_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(colorwk, 0, sizeof(colorwk));
    waterposi = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_pos = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
}

static void queue_actwkchk(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void test_funsui4_patterns_and_animation_tables(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, funsui4pchg[0] == pchg0);
    TEST_ASSERT_TRUE(ctx, funsui4pchg[1] == pchg1);
    TEST_ASSERT_EQ_INT(ctx, 0, pchg0[0]);
    TEST_ASSERT_EQ_INT(ctx, 255, pchg0[6]);
    TEST_ASSERT_EQ_INT(ctx, 5, pchg1[1]);
    TEST_ASSERT_EQ_INT(ctx, 255, pchg1[5]);
    TEST_ASSERT_TRUE(ctx, funsui4pat[0] == &pat0);
    TEST_ASSERT_TRUE(ctx, funsui4pat[8] == &pat8);
    TEST_ASSERT_EQ_INT(ctx, 2, pat0.cnt);
    TEST_ASSERT_EQ_INT(ctx, -56, pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -80, pat0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 498, pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 503, pat0.spra[1].index);
    TEST_ASSERT_EQ_INT(ctx, 1, pat8.cnt);
    TEST_ASSERT_EQ_INT(ctx, 511, pat8.spra[0].index);
}

static void test_funsui4_init_sets_palette_shape_and_waterline(
    test_context *ctx) {
    sprite_status *fountain = &actwk[5];

    reset_state();
    waterposi = 320;
    fountain->xposi.w.h = 100;
    fountain->yposi.w.h = 500;

    funsui4(fountain);

    TEST_ASSERT_EQ_INT(ctx, 96, colorwk[55].peRed);
    TEST_ASSERT_EQ_INT(ctx, 64, colorwk[55].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 96, colorwk[55].peBlue);
    TEST_ASSERT_EQ_INT(ctx, 1, colorwk[55].peFlags);
    TEST_ASSERT_EQ_INT(ctx, 160, colorwk[56].peRed);
    TEST_ASSERT_EQ_INT(ctx, 128, colorwk[56].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 160, colorwk[56].peBlue);
    TEST_ASSERT_EQ_INT(ctx, 224, colorwk[57].peRed);
    TEST_ASSERT_EQ_INT(ctx, 224, colorwk[57].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 224, colorwk[57].peBlue);

    TEST_ASSERT_EQ_INT(ctx, 2, fountain->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, fountain->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 25344, fountain->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 1, fountain->sprpri);
    TEST_ASSERT_TRUE(ctx, fountain->patbase == funsui4pat);
    TEST_ASSERT_EQ_INT(ctx, 32, fountain->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 60, fountain->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 16, funsui4_work_get(fountain)->timer);
    TEST_ASSERT_EQ_INT(ctx, 272, fountain->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == fountain);
    TEST_ASSERT_TRUE(ctx, patchg_table == funsui4pchg);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actor == fountain);
    TEST_ASSERT_TRUE(ctx, ridechk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_funsui4_move_spawns_splash_when_timer_wraps(
    test_context *ctx) {
    sprite_status *fountain = &actwk[5];
    sprite_status *splash = &actwk[20];

    reset_state();
    waterposi = 300;
    fountain->xposi.w.h = 200;
    fountain->yposi.w.h = 260;
    funsui4_work_get(fountain)->splash_index = 2;
    funsui4_work_get(fountain)->timer = 240;
    queue_actwkchk(splash);

    funsui4_move(fountain);

    TEST_ASSERT_EQ_INT(ctx, 0, funsui4_work_get(fountain)->timer);
    TEST_ASSERT_EQ_INT(ctx, 3, funsui4_work_get(fountain)->splash_index);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, fountain->actno, splash->actno);
    TEST_ASSERT_EQ_INT(ctx, -1, splash->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 5, splash->patno);
    TEST_ASSERT_EQ_INT(ctx, 2, funsui4_work_get(splash)->splash_index);
    TEST_ASSERT_EQ_INT(ctx, 240, splash->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 188, splash->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 64, splash->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -128, splash->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 252, fountain->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
}

static void test_funsui4_move_timer_advances_without_spawning(
    test_context *ctx) {
    sprite_status *fountain = &actwk[5];

    reset_state();
    waterposi = 260;
    fountain->yposi.w.h = 100;
    funsui4_work_get(fountain)->timer = 32;

    funsui4_move(fountain);

    TEST_ASSERT_EQ_INT(ctx, 48, funsui4_work_get(fountain)->timer);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 212, fountain->yposi.w.h);
}

static void test_sibuki_init_sets_shape_and_moves_once(test_context *ctx) {
    sprite_status *splash = &actwk[20];

    reset_state();
    splash->userflag.b.h = -1;
    splash->xposi.l = 100 << 16;
    splash->yposi.l = 200 << 16;
    splash->xspeed.w = 32;
    splash->yspeed.w = -64;

    funsui4(splash);

    TEST_ASSERT_EQ_INT(ctx, 2, splash->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, splash->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 25344, splash->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 3, splash->sprpri);
    TEST_ASSERT_TRUE(ctx, splash->patbase == funsui4pat);
    TEST_ASSERT_EQ_INT(ctx, 16, splash->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, splash->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, splash->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 2, funsui4_work_get(splash)->timer);
    TEST_ASSERT_EQ_INT(ctx, 100, splash->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 199, splash->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -56, splash->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_sibuki_move_frames_out_after_lifetime_wrap(test_context *ctx) {
    sprite_status *splash = &actwk[20];

    reset_state();
    splash->r_no0 = 2;
    funsui4_work_get(splash)->timer = 254;
    splash->xposi.w.h = 100;
    splash->yposi.w.h = 200;

    sibuki(splash);

    TEST_ASSERT_EQ_INT(ctx, 0, funsui4_work_get(splash)->timer);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == splash);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

static void test_non_splash_r_no4_uses_splash_move_table_entry(
    test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_state();
    actor->r_no0 = 4;
    actor->xposi.l = 10 << 16;
    actor->yposi.l = 20 << 16;
    actor->xspeed.w = 16;
    actor->yspeed.w = 8;

    funsui4(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, funsui4_work_get(actor)->timer);
    TEST_ASSERT_EQ_INT(ctx, 10, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 20, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 2, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 2, frameout_s_count);
}

TEST_MAIN_BEGIN;
    test_funsui4_patterns_and_animation_tables(&ctx);
    test_funsui4_init_sets_palette_shape_and_waterline(&ctx);
    test_funsui4_move_spawns_splash_when_timer_wraps(&ctx);
    test_funsui4_move_timer_advances_without_spawning(&ctx);
    test_sibuki_init_sets_shape_and_moves_once(&ctx);
    test_sibuki_move_frames_out_after_lifetime_wrap(&ctx);
    test_non_splash_r_no4_uses_splash_move_table_entry(&ctx);
TEST_MAIN_END
