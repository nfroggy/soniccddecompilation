#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 generate_flag;
Uint8 pltime_f;
Uint8 plautoflag;
ushort_union swdata;
short_union editmode;
PALETTEENTRY gamecolor[64];
static PALETTEENTRY color_buffer[64];
PALETTEENTRY *lpcolorwk = color_buffer;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int actwkchk_count;
static sprite_status *actwkchk_actor;
static Sint32 actwkchk_result;
static int soundset_count;
static Sint16 soundset_arg;
static int goal_move2_count;
static sprite_status *goal_move2_actor;
static int sub_sync_count;
static Sint16 sub_sync_arg;
static Sint16 emycol_d_result;
static int emycol_d_count;
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 sinset_sin_result;
static Sint16 sinset_cos_result;

enum {
    EMIE7_TIMER_OFFSET = 6,
    EMIE7_HOME_X_OFFSET = 12,
    EMIE7_GOAL_FLAG_OFFSET = 14,
    EMIE7_FALL_FLAG_OFFSET = 18
};

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void soundset(Sint16 ReqNo);
void goal_move2(sprite_status *pActwk);
void sub_sync(Sint16 ReqNo);
Sint16 emycol_d(sprite_status *pActwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);

#include "src/r7/emie7cg.c"
#include "src/r7/emie7.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
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

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_result == 0) {
        *ppActwk = actwkchk_actor;
    }
    return actwkchk_result;
}

void soundset(Sint16 ReqNo) {
    ++soundset_count;
    soundset_arg = ReqNo;
}

void goal_move2(sprite_status *pActwk) {
    ++goal_move2_count;
    goal_move2_actor = pActwk;
}

void sub_sync(Sint16 ReqNo) {
    ++sub_sync_count;
    sub_sync_arg = ReqNo;
}

Sint16 emycol_d(sprite_status *pActwk) {
    (void)pActwk;
    ++emycol_d_count;
    return emycol_d_result;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    ++sinset_count;
    sinset_angle = kakudo;
    *sin = sinset_sin_result;
    *cos = sinset_cos_result;
}

static void write_actfree_s16(sprite_status *actor, int offset, Sint16 value) {
    memcpy(&actor->actfree[offset], &value, sizeof(value));
}

static Sint16 read_actfree_s16(sprite_status *actor, int offset) {
    Sint16 value;

    memcpy(&value, &actor->actfree[offset], sizeof(value));
    return value;
}

static void reset_emie7_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(gamecolor, 0, sizeof(gamecolor));
    memset(color_buffer, 0, sizeof(color_buffer));
    generate_flag = 0;
    pltime_f = 1;
    plautoflag = 0;
    memset(&swdata, 0, sizeof(swdata));
    memset(&editmode, 0, sizeof(editmode));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    actwkchk_count = 0;
    actwkchk_actor = &actwk[20];
    actwkchk_result = 0;
    soundset_count = 0;
    soundset_arg = 0;
    goal_move2_count = 0;
    goal_move2_actor = 0;
    sub_sync_count = 0;
    sub_sync_arg = 0;
    emycol_d_result = 0;
    emycol_d_count = 0;
    sinset_count = 0;
    sinset_angle = 0;
    sinset_sin_result = 0;
    sinset_cos_result = 0;
}

static void assert_post_update_calls(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void test_emie7_init_default_position_and_palette_copy(
    test_context *ctx) {
    sprite_status *amy = &actwk[4];
    int i;

    reset_emie7_state();
    for (i = 0; i < 16; ++i) {
        gamecolor[16 + i].peRed = (Uint8)(10 + i);
        gamecolor[16 + i].peGreen = (Uint8)(20 + i);
        gamecolor[16 + i].peBlue = (Uint8)(30 + i);
        gamecolor[16 + i].peFlags = (Uint8)(40 + i);
    }

    emie7(amy);

    TEST_ASSERT_EQ_INT(ctx, 4, amy->actflg);
    TEST_ASSERT_EQ_INT(ctx, 41931, amy->sproffset);
    TEST_ASSERT_TRUE(ctx, amy->patbase == emie7pat);
    TEST_ASSERT_EQ_INT(ctx, 12, amy->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, amy->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 245, amy->colino);
    TEST_ASSERT_EQ_INT(ctx, 16195, amy->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 427, amy->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 6, amy->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 6, amy->mstno.b.l);
    TEST_ASSERT_EQ_INT(ctx, 17, amy->patno);
    TEST_ASSERT_EQ_INT(ctx, 7, amy->pattim);
    TEST_ASSERT_EQ_INT(ctx, 2, amy->patcnt);
    assert_post_update_calls(ctx, amy);
    TEST_ASSERT_EQ_INT(ctx, 10, color_buffer[16].peRed);
    TEST_ASSERT_EQ_INT(ctx, 25, color_buffer[21].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 45, color_buffer[21].peFlags);
}

static void test_emie7_init_generated_position_preserved(test_context *ctx) {
    sprite_status *amy = &actwk[4];

    reset_emie7_state();
    generate_flag = 1;
    amy->xposi.w.h = 300;
    amy->yposi.w.h = 120;

    emie7(amy);

    TEST_ASSERT_EQ_INT(ctx, 300, amy->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 120, amy->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 300, read_actfree_s16(amy, EMIE7_HOME_X_OFFSET));
}

static void test_emie7_init_collision_spawns_score_actor_and_timer(
    test_context *ctx) {
    sprite_status *amy = &actwk[4];

    reset_emie7_state();
    generate_flag = 1;
    amy->xposi.w.h = 200;
    amy->yposi.w.h = 100;
    amy->colicnt = 1;
    actwk[0].yspeed.w = 512;

    emie7(amy);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 24, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[20].r_no1);
    TEST_ASSERT_EQ_INT(ctx, 200, actwk[20].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 100, actwk[20].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 158, soundset_arg);
    TEST_ASSERT_EQ_INT(ctx, -512, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 2, amy->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 600, read_actfree_s16(amy, EMIE7_TIMER_OFFSET));
}

static void test_emie7_move0_falls_then_enters_chase(test_context *ctx) {
    sprite_status *amy = &actwk[4];

    reset_emie7_state();
    amy->r_no0 = 2;
    amy->yposi.w.h = 463;
    amy->yposi.w.l = -4096;
    amy->yspeed.w = 128;

    emie7(amy);

    TEST_ASSERT_EQ_INT(ctx, 464, amy->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, (Uint8)-1, amy->actfree[EMIE7_FALL_FLAG_OFFSET]);

    reset_emie7_state();
    amy->r_no0 = 2;
    amy->actfree[EMIE7_FALL_FLAG_OFFSET] = (Uint8)-1;
    amy->xposi.w.h = 80;
    amy->mstno.b.h = 6;
    actwk[0].xposi.w.h = 100;

    emie7(amy);

    TEST_ASSERT_EQ_INT(ctx, 4, amy->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 60, amy->actfree[21]);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->cddat & 1);
    TEST_ASSERT_EQ_INT(ctx, 6, amy->mstno.b.l);
}

static void test_emie7_move1_timer_expires_into_goal_path(test_context *ctx) {
    sprite_status *amy = &actwk[4];

    reset_emie7_state();
    amy->r_no0 = 4;
    write_actfree_s16(amy, EMIE7_TIMER_OFFSET, 1);

    emie7(amy);

    TEST_ASSERT_EQ_INT(ctx, (Uint8)-1, amy->actfree[EMIE7_GOAL_FLAG_OFFSET]);
    TEST_ASSERT_EQ_INT(ctx, 0, pltime_f);
    TEST_ASSERT_EQ_INT(ctx, 1, amy->actfree[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, goal_move2_count);
    TEST_ASSERT_TRUE(ctx, goal_move2_actor == amy);
    TEST_ASSERT_EQ_INT(ctx, 0, emycol_d_count);
}

static void test_emie7_move1_daki_and_waiting_paths(test_context *ctx) {
    sprite_status *amy = &actwk[4];

    reset_emie7_state();
    amy->r_no0 = 4;
    amy->xposi.w.h = 100;
    amy->yposi.w.h = 100;
    amy->actfree[21] = 1;
    actwk[0].xposi.w.h = 114;
    actwk[0].yposi.w.h = 92;
    actwk[0].cddat = 1;

    emie7(amy);

    TEST_ASSERT_EQ_INT(ctx, 10, amy->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 129, amy->actfree[20]);
    TEST_ASSERT_EQ_INT(ctx, 32, amy->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->patno);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 124, sub_sync_arg);

    reset_emie7_state();
    amy->r_no0 = 4;
    amy->xposi.w.h = 100;
    amy->actfree[20] = 4;
    actwk[0].xposi.w.h = 120;
    actwk[0].xspeed.w = 256;

    emie7(amy);

    TEST_ASSERT_EQ_INT(ctx, 4, amy->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, amy->mstno.b.h);

    reset_emie7_state();
    amy->r_no0 = 4;
    amy->xposi.w.h = 100;
    amy->yposi.w.h = 100;
    amy->actfree[21] = 1;
    editmode.b.h = 1;
    actwk[0].xposi.w.h = 108;
    actwk[0].yposi.w.h = 92;
    actwk[0].cddat = 1;

    emie7(amy);

    TEST_ASSERT_EQ_INT(ctx, 4, amy->r_no0);

    reset_emie7_state();
    amy->r_no0 = 4;
    amy->xposi.w.h = 100;
    amy->yposi.w.h = 100;
    amy->actfree[21] = 1;
    actwk[0].xposi.w.h = 108;
    actwk[0].yposi.w.h = 200;
    actwk[0].cddat = 1;

    emie7(amy);

    TEST_ASSERT_EQ_INT(ctx, 4, amy->r_no0);

    reset_emie7_state();
    amy->r_no0 = 4;
    amy->xposi.w.h = 100;
    amy->yposi.w.h = 100;
    amy->actfree[21] = 1;
    actwk[0].xposi.w.h = 108;
    actwk[0].yposi.w.h = 92;
    actwk[0].xspeed.w = -256;
    actwk[0].cddat = 3;

    emie7(amy);

    TEST_ASSERT_EQ_INT(ctx, 14, amy->r_no0);
}

static void test_emie7_move1_chases_and_respects_left_home_limit(
    test_context *ctx) {
    sprite_status *amy = &actwk[4];

    reset_emie7_state();
    amy->r_no0 = 4;
    amy->xposi.w.h = 200;
    amy->yposi.w.h = 100;
    write_actfree_s16(amy, EMIE7_HOME_X_OFFSET, 300);
    actwk[0].xposi.w.h = 100;
    emycol_d_result = 3;

    emie7(amy);

    TEST_ASSERT_EQ_INT(ctx, -32, amy->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 199, amy->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 103, amy->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, amy->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);

    reset_emie7_state();
    generate_flag = 1;
    amy->r_no0 = 4;
    amy->xposi.w.h = 200;
    amy->cddat = 1;
    amy->xspeed.w = -640;
    write_actfree_s16(amy, EMIE7_HOME_X_OFFSET, 300);
    actwk[0].xposi.w.h = 100;

    emie7(amy);

    TEST_ASSERT_EQ_INT(ctx, 0, amy->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, amy->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, emycol_d_count);
}

static void test_emie7_jump_and_landing_paths(test_context *ctx) {
    sprite_status *amy = &actwk[4];

    reset_emie7_state();
    amy->r_no0 = 14;
    amy->xposi.w.h = 120;
    amy->cddat = 1;
    write_actfree_s16(amy, EMIE7_HOME_X_OFFSET, 100);

    emie7(amy);

    TEST_ASSERT_EQ_INT(ctx, 16, amy->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 128, amy->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -704, amy->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 6, amy->patno);

    reset_emie7_state();
    amy->r_no0 = 14;
    amy->xposi.w.h = 80;
    write_actfree_s16(amy, EMIE7_HOME_X_OFFSET, 100);

    emie7(amy);

    TEST_ASSERT_EQ_INT(ctx, 16, amy->r_no0);
    TEST_ASSERT_EQ_INT(ctx, -128, amy->xspeed.w);

    reset_emie7_state();
    amy->r_no0 = 14;
    amy->xposi.w.h = 300;
    write_actfree_s16(amy, EMIE7_HOME_X_OFFSET, 100);

    emie7(amy);

    TEST_ASSERT_EQ_INT(ctx, 0, amy->xspeed.w);

    reset_emie7_state();
    amy->r_no0 = 8;
    amy->xposi.w.h = 100;
    amy->yposi.w.h = 464;
    amy->yspeed.w = 128;
    amy->actfree[16] = 240;

    emie7(amy);

    TEST_ASSERT_EQ_INT(ctx, 464, amy->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 4, amy->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->actfree[16]);

    reset_emie7_state();
    amy->r_no0 = 8;
    amy->yposi.w.h = 464;
    amy->actfree[16] = 0;

    emie7(amy);

    TEST_ASSERT_EQ_INT(ctx, 8, amy->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 16, amy->actfree[16]);
}

static void test_emie7_daki_and_end_follow_sonic(test_context *ctx) {
    sprite_status *amy = &actwk[4];

    reset_emie7_state();
    amy->r_no0 = 10;
    actwk[0].xposi.w.h = 200;
    actwk[0].yposi.w.h = 300;
    actwk[0].cddat = 1;
    amy->actfree[14] = 0;

    emie7(amy);

    TEST_ASSERT_EQ_INT(ctx, 1, plautoflag);
    TEST_ASSERT_EQ_INT(ctx, 0, swdata.w);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[0].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 212, amy->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 300, amy->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 14, amy->patno);
    TEST_ASSERT_EQ_INT(ctx, 1, goal_move2_count);

    reset_emie7_state();
    amy->r_no0 = 10;
    amy->actfree[14] = 1;
    actwk[0].xposi.w.h = 200;
    actwk[0].yposi.w.h = 300;

    emie7(amy);

    TEST_ASSERT_EQ_INT(ctx, 188, amy->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, goal_move2_count);

    reset_emie7_state();
    amy->r_no0 = 12;
    actwk[0].xposi.w.h = 200;
    actwk[0].yposi.w.h = 300;

    emie7(amy);

    TEST_ASSERT_EQ_INT(ctx, 188, amy->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 14, amy->patno);

    reset_emie7_state();
    amy->r_no0 = 12;
    actwk[0].xposi.w.h = 200;
    actwk[0].cddat = 1;

    emie7(amy);

    TEST_ASSERT_EQ_INT(ctx, 212, amy->xposi.w.h);
}

static void test_emie7_heart_spawn_and_animation_wrap(test_context *ctx) {
    sprite_status *amy = &actwk[4];

    reset_emie7_state();
    amy->r_no0 = 12;
    amy->xposi.w.h = 100;
    amy->yposi.w.h = 50;
    amy->cddat = 1;
    amy->actfree[17] = 250;
    amy->actfree[20] = 1;

    emie7(amy);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 52, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 110, actwk[20].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 38, actwk[20].yposi.w.h);

    reset_emie7_state();
    amy->mstno.b.h = 6;
    amy->mstno.b.l = 6;
    amy->patcnt = 8;
    amy->pattim = 1;

    empatchg(amy, em_pchg);

    TEST_ASSERT_EQ_INT(ctx, 17, amy->patno);
    TEST_ASSERT_EQ_INT(ctx, 2, amy->patcnt);
}

static void test_heart7_initializes_moves_and_frames_out(test_context *ctx) {
    sprite_status *heart = &actwk[10];

    reset_emie7_state();
    sinset_sin_result = 64;
    sinset_cos_result = 100;

    heart7(heart);

    TEST_ASSERT_EQ_INT(ctx, 2, heart->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, heart->actflg);
    TEST_ASSERT_EQ_INT(ctx, 971, heart->sproffset);
    TEST_ASSERT_TRUE(ctx, heart->patbase == emie7pat);
    TEST_ASSERT_EQ_INT(ctx, 8, heart->patno);
    TEST_ASSERT_EQ_INT(ctx, -96, heart->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 16, heart->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, heart->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 0, sinset_angle);

    reset_emie7_state();
    heart->r_no0 = 2;
    heart->actfree[16] = 19;
    heart->patno = 8;

    heart7(heart);

    TEST_ASSERT_EQ_INT(ctx, 9, heart->patno);

    reset_emie7_state();
    heart->r_no0 = 2;
    heart->actfree[16] = 119;

    heart7(heart);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == heart);

    reset_emie7_state();
    heart->r_no0 = 2;
    heart->actfree[16] = 109;
    heart->patno = 8;
    heart->xspeed.w = 32;
    heart->yspeed.w = -16;

    heart7(heart);

    TEST_ASSERT_EQ_INT(ctx, 9, heart->patno);
    TEST_ASSERT_EQ_INT(ctx, 0, heart->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, heart->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, heart->actfree[18]);
}

TEST_MAIN_BEGIN;
test_emie7_init_default_position_and_palette_copy(&ctx);
test_emie7_init_generated_position_preserved(&ctx);
test_emie7_init_collision_spawns_score_actor_and_timer(&ctx);
test_emie7_move0_falls_then_enters_chase(&ctx);
test_emie7_move1_timer_expires_into_goal_path(&ctx);
test_emie7_move1_daki_and_waiting_paths(&ctx);
test_emie7_move1_chases_and_respects_left_home_limit(&ctx);
test_emie7_jump_and_landing_paths(&ctx);
test_emie7_daki_and_end_follow_sonic(&ctx);
test_emie7_heart_spawn_and_animation_wrap(&ctx);
test_heart7_initializes_moves_and_frames_out(&ctx);
TEST_MAIN_END
