#include <stddef.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 flagwork[766];
Uint8 time_flag;
int_union scra_h_posit;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static Sint16 hitchk_result;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r4/escal4.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++hitchk_count;
    hitchk_actor = pActwk;
    hitchk_player = pPlayerwk;
    return hitchk_result;
}

static void reset_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(flagwork, 255, sizeof(flagwork));
    time_flag = 0;
    scra_h_posit.l = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    hitchk_result = 0;
}

static void test_escal4_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, escal4pat[0] == &escal4_pat0);
    TEST_ASSERT_TRUE(ctx, escal4pat[1] == &escal4_pat1);
    TEST_ASSERT_EQ_INT(ctx, 1, escal4_pat0.cnt);
    TEST_ASSERT_EQ_INT(ctx, -16, escal4_pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, escal4_pat0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 429, escal4_pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 430, escal4_pat1.spra[0].index);
}

static void test_escal4_init_sets_platform_shape(test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->xposi.w.h = 120;

    escal4(platform);

    TEST_ASSERT_EQ_INT(ctx, 2, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, platform->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 17244, platform->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 3, platform->sprpri);
    TEST_ASSERT_TRUE(ctx, platform->patbase == escal4pat);
    TEST_ASSERT_EQ_INT(ctx, 16, platform->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, platform->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == platform);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_escal4_wait_ignores_inactive_actor(test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->r_no0 = 2;
    platform->actflg = 0;
    hitchk_result = 1;

    escal4_wait(platform);

    TEST_ASSERT_EQ_INT(ctx, 2, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk_count);
}

static void test_escal4_wait_uses_hitchk_return_for_motion_table(
    test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->r_no0 = 2;
    platform->actflg = 128;
    time_flag = 0;
    hitchk_result = 1;

    escal4_wait(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == platform);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 4, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 256, platform->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -256, platform->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 160, escal4_work_get(platform)->move_timer);

    reset_state();
    platform->r_no0 = 2;
    platform->actflg = 128;
    time_flag = 2;
    hitchk_result = 1;

    escal4_wait(platform);

    TEST_ASSERT_EQ_INT(ctx, 4, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 512, platform->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -512, platform->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 80, escal4_work_get(platform)->move_timer);
}

static void test_escal4_wait_stays_put_when_hitchk_is_clear(test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->r_no0 = 2;
    platform->actflg = 128;
    hitchk_result = 0;

    escal4_wait(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 2, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, platform->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, platform->yspeed.w);
}

static void test_escal4_moveup_moves_until_counter_expires(test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->xposi.l = 100 << 16;
    platform->yposi.l = 200 << 16;
    platform->xspeed.w = 256;
    platform->yspeed.w = -256;
    escal4_work_get(platform)->move_timer = 2;

    escal4_moveup(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 101, platform->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 199, platform->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, escal4_work_get(platform)->move_timer);
    TEST_ASSERT_EQ_INT(ctx, 256, platform->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -256, platform->yspeed.w);

    escal4_moveup(platform);

    TEST_ASSERT_EQ_INT(ctx, 102, platform->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 198, platform->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, escal4_work_get(platform)->move_timer);
    TEST_ASSERT_EQ_INT(ctx, 0, platform->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, platform->yspeed.w);
}

static void test_escal4_moveup_counter_zero_only_checks_ride(test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->xposi.w.h = 100;
    platform->yposi.w.h = 200;
    platform->xspeed.w = 111;
    platform->yspeed.w = 222;
    escal4_work_get(platform)->move_timer = 0;

    escal4_moveup(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 100, platform->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, platform->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 111, platform->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 222, platform->yspeed.w);
}

static void test_escal4_entry_frames_out_and_clears_flagwork(
    test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->r_no0 = 2;
    platform->xposi.w.h = 1024;
    platform->cdsts = 4;
    time_flag = 2;
    flagwork[14] = 255;
    scra_h_posit.w.h = 128;

    escal4(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == platform);
    TEST_ASSERT_EQ_INT(ctx, 127, flagwork[14]);

    reset_state();
    platform->r_no0 = 2;
    platform->xposi.w.h = 1024;
    platform->cdsts = 0;
    flagwork[0] = 255;
    scra_h_posit.w.h = 128;

    escal4(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 255, flagwork[0]);
}

TEST_MAIN_BEGIN;
    test_escal4_patterns_capture_literal_data(&ctx);
    test_escal4_init_sets_platform_shape(&ctx);
    test_escal4_wait_ignores_inactive_actor(&ctx);
    test_escal4_wait_uses_hitchk_return_for_motion_table(&ctx);
    test_escal4_wait_stays_put_when_hitchk_is_clear(&ctx);
    test_escal4_moveup_moves_until_counter_expires(&ctx);
    test_escal4_moveup_counter_zero_only_checks_ride(&ctx);
    test_escal4_entry_frames_out_and_clears_flagwork(&ctx);
TEST_MAIN_END
