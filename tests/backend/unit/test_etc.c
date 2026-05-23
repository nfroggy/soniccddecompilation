#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

Uint8 flagwork[766];
Uint8 int_flg;
int_union scra_h_posit;

static int actionsub_count;
static int frameout_count;
static sprite_status *actionsub_actor;
static sprite_status *frameout_actor;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
    pActwk->actno = 0;
}

#include "src/etc.c"

static void reset_fixture(void) {
    memset(flagwork, 0, sizeof(flagwork));
    int_flg = 0;
    memset(&scra_h_posit, 0, sizeof(scra_h_posit));
    actionsub_count = 0;
    frameout_count = 0;
    actionsub_actor = 0;
    frameout_actor = 0;
}

static void test_frameoutchk_keeps_actor_inside_rounded_screen_band(
    test_context *ctx) {
    sprite_status actor;

    reset_fixture();
    memset(&actor, 0, sizeof(actor));
    scra_h_posit.w.h = 100;
    actor.xposi.w.h = 600;
    actor.cdsts = 5;
    flagwork[5] = 255;

    frameoutchk(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 255, flagwork[5]);
}

static void test_frameoutchk_frames_out_actor_past_screen_band(
    test_context *ctx) {
    sprite_status actor;

    reset_fixture();
    memset(&actor, 0, sizeof(actor));
    scra_h_posit.w.h = 100;
    actor.xposi.w.h = 800;
    actor.actno = 47;
    actor.cdsts = 5;
    flagwork[5] = 255;

    frameoutchk(&actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 127, flagwork[5]);
    TEST_ASSERT_EQ_INT(ctx, 0, actor.actno);
}

static void test_frameoutchk_skips_flag_clear_when_no_flag_index(
    test_context *ctx) {
    sprite_status actor;

    reset_fixture();
    memset(&actor, 0, sizeof(actor));
    scra_h_posit.w.h = 100;
    actor.xposi.w.h = 800;
    actor.actno = 13;
    actor.cdsts = 0;
    flagwork[0] = 255;

    frameoutchk(&actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 255, flagwork[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, actor.actno);
}

static void test_sync_reflects_interrupt_flag(test_context *ctx) {
    reset_fixture();

    int_flg = 0;
    TEST_ASSERT_EQ_INT(ctx, 0, sync());

    int_flg = 1;
    TEST_ASSERT_EQ_INT(ctx, -1, sync());
}

static void test_sinset_uses_lookup_table_with_byte_wrap(test_context *ctx) {
    Sint16 sin_value;
    Sint16 cos_value;

    reset_fixture();

    sin_value = 1234;
    cos_value = 5678;
    sinset(0, &sin_value, &cos_value);
    TEST_ASSERT_EQ_INT(ctx, 0, sin_value);
    TEST_ASSERT_EQ_INT(ctx, 256, cos_value);

    sinset(64, &sin_value, &cos_value);
    TEST_ASSERT_EQ_INT(ctx, 256, sin_value);
    TEST_ASSERT_EQ_INT(ctx, 0, cos_value);

    sinset(128, &sin_value, &cos_value);
    TEST_ASSERT_EQ_INT(ctx, 0, sin_value);
    TEST_ASSERT_EQ_INT(ctx, -256, cos_value);

    sinset(192, &sin_value, &cos_value);
    TEST_ASSERT_EQ_INT(ctx, -256, sin_value);
    TEST_ASSERT_EQ_INT(ctx, 0, cos_value);

    sinset(255, &sin_value, &cos_value);
    TEST_ASSERT_EQ_INT(ctx, -6, sin_value);
    TEST_ASSERT_EQ_INT(ctx, 255, cos_value);
}

static void test_atan_sonic_axes_and_equal_diagonals(test_context *ctx) {
    reset_fixture();

    TEST_ASSERT_EQ_INT(ctx, 64, atan_sonic(0, 0));
    TEST_ASSERT_EQ_INT(ctx, 0, atan_sonic(256, 0));
    TEST_ASSERT_EQ_INT(ctx, 64, atan_sonic(0, 256));
    TEST_ASSERT_EQ_INT(ctx, 128, atan_sonic(-256, 0));
    TEST_ASSERT_EQ_INT(ctx, 192, atan_sonic(0, -256));

    TEST_ASSERT_EQ_INT(ctx, 32, atan_sonic(256, 256));
    TEST_ASSERT_EQ_INT(ctx, 96, atan_sonic(-256, 256));
    TEST_ASSERT_EQ_INT(ctx, 160, atan_sonic(-256, -256));
    TEST_ASSERT_EQ_INT(ctx, 224, atan_sonic(256, -256));
}

static void test_atan_sonic_uses_x_or_y_dominant_ratio(test_context *ctx) {
    reset_fixture();

    TEST_ASSERT_EQ_INT(ctx, 10, atan_sonic(256, 64));
    TEST_ASSERT_EQ_INT(ctx, 54, atan_sonic(64, 256));
    TEST_ASSERT_EQ_INT(ctx, 118, atan_sonic(-256, 64));
    TEST_ASSERT_EQ_INT(ctx, 182, atan_sonic(-64, -256));
    TEST_ASSERT_EQ_INT(ctx, 246, atan_sonic(256, -64));
}

TEST_MAIN_BEGIN;
test_frameoutchk_keeps_actor_inside_rounded_screen_band(&ctx);
test_frameoutchk_frames_out_actor_past_screen_band(&ctx);
test_frameoutchk_skips_flag_clear_when_no_flag_index(&ctx);
test_sync_reflects_interrupt_flag(&ctx);
test_sinset_uses_lookup_table_with_byte_wrap(&ctx);
test_atan_sonic_axes_and_equal_diagonals(&ctx);
test_atan_sonic_uses_x_or_y_dominant_ratio(&ctx);
TEST_MAIN_END;
