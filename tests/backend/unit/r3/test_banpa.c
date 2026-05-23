#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_x;
static int soundset_count;
static Sint16 soundset_requests[8];

void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
void soundset(Sint16 ReqNo);

#include "src/r3/banpa.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi) {
    ++frameout_s00_count;
    frameout_s00_actor = pActwk;
    frameout_s00_x = xposi;
    return 0;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 8) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

static void reset_banpa_state(void) {
    memset(actwk, 0, sizeof(actwk));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_x = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void reset_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_x = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void init_bumper(sprite_status *bumper, Sint16 x, Sint16 y,
                        Sint8 userflag_h) {
    reset_banpa_state();
    bumper->xposi.w.h = x;
    bumper->yposi.w.h = y;
    bumper->userflag.b.h = userflag_h;
    banpa(bumper);
    reset_logs();
}

static void test_banpa_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, banpa_pat[0] == &bmp00);
    TEST_ASSERT_EQ_INT(ctx, 1, bmp00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -32, bmp00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, bmp00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 0, bmp00.spra[0].etc);
    TEST_ASSERT_EQ_INT(ctx, 315, bmp00.spra[0].index);
}

static void test_banpa_initializes_small_vertical_motion(test_context *ctx) {
    sprite_status *bumper = &actwk[2];

    reset_banpa_state();
    bumper->xposi.w.h = 320;
    bumper->yposi.w.h = 100;
    bumper->actflg = 128;

    banpa(bumper);

    TEST_ASSERT_EQ_INT(ctx, 2, bumper->r_no0);
    TEST_ASSERT_TRUE(ctx, bumper->patbase == banpa_pat);
    TEST_ASSERT_EQ_INT(ctx, 4, bumper->actflg);
    TEST_ASSERT_EQ_INT(ctx, 32, bumper->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, bumper->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, bumper->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 231, bumper->colino);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == bumper);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == bumper);
    TEST_ASSERT_EQ_INT(ctx, 320, frameout_s00_x);

    reset_logs();
    banpa(bumper);

    TEST_ASSERT_EQ_INT(ctx, 99, bumper->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 320, bumper->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_EQ_INT(ctx, 320, frameout_s00_x);
}

static void test_banpa_initializes_large_horizontal_positive_motion(
    test_context *ctx) {
    sprite_status *bumper = &actwk[2];

    init_bumper(bumper, 500, 100, -64);

    banpa(bumper);

    TEST_ASSERT_EQ_INT(ctx, 501, bumper->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 100, bumper->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_EQ_INT(ctx, 500, frameout_s00_x);
}

static void test_banpa_reverses_motion_after_configured_count(
    test_context *ctx) {
    sprite_status *bumper = &actwk[2];

    init_bumper(bumper, 500, 100, -128);

    for (int i = 0; i < 96; ++i) {
        banpa(bumper);
    }

    TEST_ASSERT_EQ_INT(ctx, 404, bumper->xposi.w.h);

    banpa(bumper);

    TEST_ASSERT_EQ_INT(ctx, 405, bumper->xposi.w.h);
}

static void test_banpa_collision_zeros_player_xspeed_on_center_y(
    test_context *ctx) {
    sprite_status *bumper = &actwk[2];
    sprite_status *player = &actwk[0];

    init_bumper(bumper, 500, 100, 0);
    bumper->actflg = 128;
    bumper->colicnt = 3;
    player->xposi.w.h = 520;
    player->yposi.w.h = 96;
    player->xspeed.w = 777;
    player->yspeed.w = -333;
    player->cddat = 255;

    banpa(bumper);

    TEST_ASSERT_EQ_INT(ctx, 0, bumper->colicnt);
    TEST_ASSERT_EQ_INT(ctx, 0, player->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -333, player->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 255, player->cddat);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 181, soundset_requests[0]);
}

static void test_banpa_collision_bounces_player_vertically(test_context *ctx) {
    sprite_status *bumper = &actwk[2];
    sprite_status *player = &actwk[0];

    init_bumper(bumper, 500, 100, 0);
    bumper->colicnt = 1;
    player->xposi.w.h = 500;
    player->yposi.w.h = 116;
    player->cddat = 240;

    banpa(bumper);

    TEST_ASSERT_EQ_INT(ctx, 0, player->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1792, player->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 194, player->cddat);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    init_bumper(bumper, 500, 100, 0);
    bumper->colicnt = 1;
    player->xposi.w.h = 500;
    player->yposi.w.h = 80;
    player->cddat = 240;

    banpa(bumper);

    TEST_ASSERT_EQ_INT(ctx, -1792, player->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 194, player->cddat);
}

static void test_banpa_collision_bounces_player_diagonally(test_context *ctx) {
    sprite_status *bumper = &actwk[2];
    sprite_status *player = &actwk[0];

    init_bumper(bumper, 500, 100, 0);
    bumper->colicnt = 1;
    player->xposi.w.h = 600;
    player->yposi.w.h = 140;
    player->cddat = 240;

    banpa(bumper);

    TEST_ASSERT_EQ_INT(ctx, 1267, player->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1267, player->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 194, player->cddat);

    init_bumper(bumper, 500, 100, 0);
    bumper->colicnt = 1;
    player->xposi.w.h = 440;
    player->yposi.w.h = 40;
    player->cddat = 240;

    banpa(bumper);

    TEST_ASSERT_EQ_INT(ctx, -1267, player->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -1267, player->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 194, player->cddat);
}

TEST_MAIN_BEGIN;
    test_banpa_tables_capture_literal_data(&ctx);
    test_banpa_initializes_small_vertical_motion(&ctx);
    test_banpa_initializes_large_horizontal_positive_motion(&ctx);
    test_banpa_reverses_motion_after_configured_count(&ctx);
    test_banpa_collision_zeros_player_xspeed_on_center_y(&ctx);
    test_banpa_collision_bounces_player_vertically(&ctx);
    test_banpa_collision_bounces_player_diagonally(&ctx);
TEST_MAIN_END
