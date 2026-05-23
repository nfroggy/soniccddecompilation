#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 time_flag;
Uint8 generate_flag;
short_union editmode;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int soundset_count;
static Sint16 soundset_requests[8];
static int scoreup_count;
static Uint32 scoreup_values[8];
static int tensuu0_count;
static sprite_status *tensuu0_actor;
static Uint8 tensuu0_userflag;
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void soundset(Sint16 ReqNo);
void scoreup(Uint32 updata);
void tensuu0(sprite_status *tensuuwk, Uint8 uf_data);
Sint32 actwkchk(sprite_status **ppActwk);

#include "src/r3/pocket.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 8) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

void scoreup(Uint32 updata) {
    if (scoreup_count < 8) {
        scoreup_values[scoreup_count] = updata;
    }
    ++scoreup_count;
}

void tensuu0(sprite_status *tensuuwk, Uint8 uf_data) {
    ++tensuu0_count;
    tensuu0_actor = tensuuwk;
    tensuu0_userflag = uf_data;
}

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_queue_index >= actwkchk_queue_count) {
        *ppActwk = 0;
        return -1;
    }
    *ppActwk = actwkchk_queue[actwkchk_queue_index++];
    return 0;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void reset_pocket_state(void) {
    memset(actwk, 0, sizeof(actwk));
    time_flag = 0;
    generate_flag = 0;
    memset(&editmode, 0, sizeof(editmode));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    scoreup_count = 0;
    memset(scoreup_values, 0, sizeof(scoreup_values));
    tensuu0_count = 0;
    tensuu0_actor = 0;
    tensuu0_userflag = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
}

static void reset_pocket_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    scoreup_count = 0;
    memset(scoreup_values, 0, sizeof(scoreup_values));
    tensuu0_count = 0;
    tensuu0_actor = 0;
    tensuu0_userflag = 0;
    actwkchk_count = 0;
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
}

static void test_pocket_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat[0] == &pat_pocket1);
    TEST_ASSERT_TRUE(ctx, pat[3] == &pat_pocket4);
    TEST_ASSERT_TRUE(ctx, pat_pocket[0] == &pat_pocket5);
    TEST_ASSERT_EQ_INT(ctx, -24, pat_pocket2.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -8, pat_pocket2.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 505, pat_pocket2.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -16, pat_pocket3.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -24, pat_pocket3.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 506, pat_pocket3.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 3, pchg0[0]);
    TEST_ASSERT_EQ_INT(ctx, -1, pchg0[3]);
    TEST_ASSERT_TRUE(ctx, pocket_move_tbl[0] == &a_init);
    TEST_ASSERT_TRUE(ctx, pocket_move_tbl[10] == &a_move8);
}

static void test_pocket_negative_init_frames_out_when_generation_blocks(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_pocket_state();
    time_flag = 2;
    generate_flag = 1;
    actor->userflag.b.h = -1;

    pocket(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void test_pocket_negative_init_catches_player_and_starts_sound(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];
    sprite_status *player = &actwk[0];

    reset_pocket_state();
    actor->userflag.b.h = -1;
    actor->actno = 47;
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 160;
    player->xposi.w.h = 300;
    player->yposi.w.h = 144;
    player->yspeed.w = 12;

    pocket(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 943, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 255, actor->actfree[20]);
    TEST_ASSERT_EQ_INT(ctx, 4, player->cddat);
    TEST_ASSERT_EQ_INT(ctx, 14, player->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 7, player->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 2, player->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, player->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -2048, player->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 300, player->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 144, player->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 215, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_pocket_negative_editmode_and_player_misses_do_not_capture(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];
    sprite_status *player = &actwk[0];

    reset_pocket_state();
    actor->r_no0 = 2;
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 160;
    editmode.w = 1;

    pocket0(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[20]);

    reset_pocket_state();
    actor = &actwk[2];
    player = &actwk[0];
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 160;
    player->xposi.w.h = 300;
    player->yposi.w.h = 144;
    player->yspeed.w = -1;

    a_check(actor, player);

    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[20]);

    player->yspeed.w = 1;
    player->r_no0 = 4;
    a_check(actor, player);

    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[20]);

    player->r_no0 = 0;
    player->yposi.w.h = 100;
    a_check(actor, player);

    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[20]);

    player->yposi.w.h = 144;
    player->xposi.w.h = 100;
    a_check(actor, player);

    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[20]);
}

static void test_pocket_move00_spawns_linked_actor_or_frames_out(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];
    sprite_status *child = &actwk[5];

    reset_pocket_state();
    actor->actno = 47;
    actor->r_no0 = 4;
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 160;
    queue_actor(child);

    a_move00(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 47, child->actno);
    TEST_ASSERT_EQ_INT(ctx, 300, child->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 160, child->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 64, actwk[0].actfree[2]);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_pocket_state();
    actor = &actwk[2];
    actor->r_no0 = 4;

    a_move00(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
}

static void test_pocket_scripted_reward_sequence(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_pocket_state();
    actor->r_no0 = 6;
    ((Uint16 *)actor)[26] = 7;

    a_move1(actor);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->patno);

    ((Sint16 *)actor)[23] = 0;
    a_move2(actor);
    TEST_ASSERT_EQ_INT(ctx, 10, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->patno);

    ((Sint16 *)actor)[23] = 0;
    a_move3(actor);
    TEST_ASSERT_EQ_INT(ctx, 12, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, scoreup_count);
    TEST_ASSERT_EQ_INT(ctx, 10, scoreup_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, tensuu0_count);
    TEST_ASSERT_TRUE(ctx, tensuu0_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, tensuu0_userflag);

    ((Sint16 *)actor)[23] = 0;
    a_move4(actor);
    TEST_ASSERT_EQ_INT(ctx, 14, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, scoreup_count);

    ((Sint16 *)actor)[23] = 0;
    a_move5(actor);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->patno);

    ((Sint16 *)actor)[23] = 0;
    a_move6(actor);
    TEST_ASSERT_EQ_INT(ctx, 18, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->patno);
}

static void test_pocket_reward_sequence_skips_scoring_when_empty(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_pocket_state();
    actor->r_no0 = 10;

    a_move3(actor);
    ((Sint16 *)actor)[23] = 0;
    a_move4(actor);

    TEST_ASSERT_EQ_INT(ctx, 14, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, scoreup_count);
    TEST_ASSERT_EQ_INT(ctx, 0, tensuu0_count);
}

static void test_pocket_scripted_states_wait_while_timers_are_positive(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_pocket_state();

    actor->r_no0 = 6;
    ((Sint16 *)actor)[23] = 1;
    a_move1(actor);
    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);

    actor->r_no0 = 8;
    ((Sint16 *)actor)[23] = 1;
    a_move2(actor);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);

    actor->r_no0 = 10;
    ((Sint16 *)actor)[23] = 1;
    a_move3(actor);
    TEST_ASSERT_EQ_INT(ctx, 10, actor->r_no0);

    actor->r_no0 = 12;
    ((Sint16 *)actor)[23] = 1;
    a_move4(actor);
    TEST_ASSERT_EQ_INT(ctx, 12, actor->r_no0);

    actor->r_no0 = 14;
    ((Sint16 *)actor)[23] = 1;
    a_move5(actor);
    TEST_ASSERT_EQ_INT(ctx, 14, actor->r_no0);

    actor->r_no0 = 16;
    ((Sint16 *)actor)[23] = 1;
    a_move6(actor);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->r_no0);

    actor->r_no0 = 18;
    ((Sint16 *)actor)[23] = 1;
    a_move7(actor);
    TEST_ASSERT_EQ_INT(ctx, 18, actor->r_no0);

    actor->r_no0 = 20;
    ((Sint16 *)actor)[23] = 1;
    a_move8(actor);
    TEST_ASSERT_EQ_INT(ctx, 20, actor->r_no0);

    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 0, scoreup_count);
    TEST_ASSERT_EQ_INT(ctx, 0, tensuu0_count);
}

static void test_pocket_release_clears_player_flags_and_recycles(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];
    sprite_status *child = &actwk[5];

    reset_pocket_state();
    actor->r_no0 = 18;
    actor->actfree[20] = 255;
    actwk[0].actfree[2] = 255;
    child->r_no0 = 2;
    ((Uint16 *)actor)[24] = 5;

    a_move7(actor);

    TEST_ASSERT_EQ_INT(ctx, 20, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 4, child->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 190, actwk[0].actfree[2]);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[20]);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 159, soundset_requests[0]);

    ((Sint16 *)actor)[23] = 0;
    a_move8(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[20]);
}

static void test_pocket_release_without_capture_keeps_player_flags(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_pocket_state();
    actor->r_no0 = 18;
    actor->actfree[20] = 0;
    actwk[0].actfree[2] = 255;

    a_move7(actor);

    TEST_ASSERT_EQ_INT(ctx, 20, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 255, actwk[0].actfree[2]);
}

static void test_pocket_positive_init_and_linked_move_paths(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_pocket_state();
    actor->userflag.b.h = 0;
    actor->r_no0 = 0;

    pocket(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 24, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 17327, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_pocket);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);

    reset_pocket_state();
    actor = &actwk[3];
    actor->r_no0 = 2;
    ((Uint16 *)actor)[25] = 5;
    actwk[5].actno = 47;

    pocket(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)pchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_pocket_state();
    actor = &actwk[3];
    actor->r_no0 = 4;

    pocket(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
}

TEST_MAIN_BEGIN;
    test_pocket_tables_capture_literal_data(&ctx);
    test_pocket_negative_init_frames_out_when_generation_blocks(&ctx);
    test_pocket_negative_init_catches_player_and_starts_sound(&ctx);
    test_pocket_negative_editmode_and_player_misses_do_not_capture(&ctx);
    test_pocket_move00_spawns_linked_actor_or_frames_out(&ctx);
    test_pocket_scripted_reward_sequence(&ctx);
    test_pocket_reward_sequence_skips_scoring_when_empty(&ctx);
    test_pocket_scripted_states_wait_while_timers_are_positive(&ctx);
    test_pocket_release_clears_player_flags_and_recycles(&ctx);
    test_pocket_release_without_capture_keeps_player_flags(&ctx);
    test_pocket_positive_init_and_linked_move_paths(&ctx);
TEST_MAIN_END
