#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
char dirstk[4];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_s0_count;
static sprite_status *frameout_s0_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_x;
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int scdchk_count;
static Sint16 scdchk_x;
static Sint16 scdchk_y;
static char scdchk_dir_value;
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static int ride_on_clr_count;
static sprite_status *ride_on_clr_actor;
static sprite_status *ride_on_clr_player;
static int soundset_count;
static Sint16 soundset_requests[8];

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint32 frameout_s0(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint32 actwkchk(sprite_status **ppActwk);
Sint16 scdchk(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi,
              Sint16 iOffset, Sint16 iBlkMsk, Sint16 iRideon,
              char *cpDirStk);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk);
void soundset(Sint16 ReqNo);

#include "src/r4/rblk4.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

Sint32 frameout_s0(sprite_status *pActwk) {
    ++frameout_s0_count;
    frameout_s0_actor = pActwk;
    return 0;
}

Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi) {
    ++frameout_s00_count;
    frameout_s00_actor = pActwk;
    frameout_s00_x = xposi;
    return 0;
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

Sint16 scdchk(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi,
              Sint16 iOffset, Sint16 iBlkMsk, Sint16 iRideon,
              char *cpDirStk) {
    (void)pActwk;
    (void)iOffset;
    (void)iBlkMsk;
    (void)iRideon;
    ++scdchk_count;
    scdchk_x = iXposi;
    scdchk_y = iYposi;
    *cpDirStk = scdchk_dir_value;
    return 0;
}

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++hitchk_count;
    hitchk_actor = pActwk;
    hitchk_player = pPlayerwk;
    return 0;
}

Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_clr_count;
    ride_on_clr_actor = pActwk;
    ride_on_clr_player = pPlayerwk;
    return 0;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 8) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void set_actor_word(sprite_status *actor, int index, Sint16 value) {
    int offset = 0;

    if (index == 26) {
        offset = 6;
    } else if (index == 28) {
        offset = 10;
    } else if (index == 29) {
        offset = 12;
    }

    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static void reset_rblk4_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(dirstk, 0, sizeof(dirstk));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_s0_count = 0;
    frameout_s0_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_x = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    scdchk_count = 0;
    scdchk_x = 0;
    scdchk_y = 0;
    scdchk_dir_value = 0;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    ride_on_clr_count = 0;
    ride_on_clr_actor = 0;
    ride_on_clr_player = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void test_rblk4_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, rblk4pat[0] == &pat00);
    TEST_ASSERT_TRUE(ctx, rblk4pat[8] == &pat08);
    TEST_ASSERT_EQ_INT(ctx, 2, pat00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -48, pat00.spra[1].xoff);
    TEST_ASSERT_EQ_INT(ctx, -48, pat00.spra[1].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_RBLK4_BASE, pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_RBLK4_BASE + 16, pat08.spra[0].index);
}

static void test_rblk4_initializes_main_and_spawned_actors(test_context *ctx) {
    sprite_status *block = &actwk[3];

    reset_rblk4_state();
    block->xposi.w.h = 320;
    block->yposi.w.h = 96;
    block->userflag.b.h = 3;
    queue_actor(&actwk[10]);
    queue_actor(&actwk[11]);
    rblk4(block);
    TEST_ASSERT_EQ_INT(ctx, 61, actwk[10].actno);
    TEST_ASSERT_EQ_INT(ctx, 255, actwk[10].actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[10].actfree[19]);
    TEST_ASSERT_EQ_INT(ctx, 61, actwk[11].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[11].actfree[19]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == block);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == block);

    reset_rblk4_state();
    rblk4(block);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s0_count);
    TEST_ASSERT_TRUE(ctx, frameout_s0_actor == block);

    reset_rblk4_state();
    queue_actor(&actwk[10]);
    rblk4(block);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s0_count);
}

static void test_rblk4_wait_paths(test_context *ctx) {
    sprite_status *block = &actwk[3];
    sprite_status *player = &actwk[0];

    reset_rblk4_state();
    block->r_no0 = 2;
    set_actor_word(block, 26, 10);
    rblk4_wait(block);
    TEST_ASSERT_EQ_INT(ctx, 0, scdchk_count);

    reset_rblk4_state();
    block->r_no0 = 2;
    block->xposi.w.h = 100;
    player->xposi.w.h = 200;
    set_actor_word(block, 26, 10);
    actwk[10].actfree[20] = 128;
    rblk4_wait(block);
    TEST_ASSERT_EQ_INT(ctx, 0, scdchk_count);

    reset_rblk4_state();
    block->r_no0 = 2;
    block->xposi.w.h = 100;
    block->yposi.w.h = 50;
    player->xposi.w.h = 70;
    player->sprhsize = 0;
    set_actor_word(block, 26, 10);
    actwk[10].actfree[20] = 128;
    scdchk_dir_value = 1;
    rblk4_wait(block);
    TEST_ASSERT_EQ_INT(ctx, 1, scdchk_count);
    TEST_ASSERT_EQ_INT(ctx, 78, scdchk_x);
    TEST_ASSERT_EQ_INT(ctx, 100, scdchk_y);

    reset_rblk4_state();
    block->r_no0 = 2;
    block->actfree[19] = 2;
    block->xposi.w.h = 100;
    block->yposi.w.h = 50;
    player->xposi.w.h = 120;
    player->sprhsize = 0;
    set_actor_word(block, 26, 10);
    actwk[10].actfree[20] = 128;
    rblk4_wait(block);

    reset_rblk4_state();
    block->r_no0 = 2;
    block->xposi.w.h = 100;
    block->yposi.w.h = 50;
    player->xposi.w.h = -1;
    set_actor_word(block, 26, 10);
    set_actor_word(block, 28, 11);
    actwk[10].actfree[20] = 128;
    actwk[11].actfree[20] = 1;
    rblk4_wait(block);
}

static void test_rblk4_move_sequence(test_context *ctx) {
    sprite_status *block = &actwk[3];

    reset_rblk4_state();
    block->r_no0 = 4;
    block->actfree[19] = 0;
    block->actfree[16] = 2;
    rblk4_move(block);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    reset_rblk4_state();
    block->r_no0 = 4;
    block->actfree[19] = 1;
    block->actfree[16] = 1;
    rblk4_move(block);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 191, soundset_requests[0]);

    reset_rblk4_state();
    block->r_no0 = 4;
    block->patno = 6;
    block->actfree[19] = 0;
    block->actfree[17] = 3;
    rblk4_move(block);
}

static void test_rblk4_push_sequence(test_context *ctx) {
    sprite_status *block = &actwk[3];
    sprite_status *player = &actwk[0];

    reset_rblk4_state();
    block->r_no0 = 6;
    block->yposi.w.h = 100;
    block->sprvsize = 64;
    player->sprvsize = 14;
    block->actfree[16] = 2;
    rblk4_push(block);

    reset_rblk4_state();
    block->r_no0 = 6;
    block->xposi.w.h = 320;
    block->yposi.w.h = 100;
    block->sprvsize = 64;
    player->sprvsize = 14;
    block->actfree[16] = 1;
    rblk4_push(block);

    reset_rblk4_state();
    block->r_no0 = 6;
    block->xposi.w.h = 320;
    block->yposi.w.h = 100;
    block->sprvsize = 64;
    player->sprvsize = 14;
    block->actfree[16] = 1;
    block->actfree[17] = 1;
    rblk4_push(block);

    reset_rblk4_state();
    block->r_no0 = 6;
    block->actfree[19] = 2;
    block->xposi.w.h = 320;
    block->yposi.w.h = 100;
    block->sprvsize = 64;
    player->sprvsize = 14;
    block->actfree[16] = 1;
    block->actfree[17] = 1;
    rblk4_push(block);

    reset_rblk4_state();
    block->r_no0 = 6;
    block->patno = 5;
    block->actfree[19] = 0;
    block->actfree[17] = 3;
    rblk4_push(block);
}

static void test_rblk4_ride_actor_paths(test_context *ctx) {
    sprite_status *child = &actwk[10];
    sprite_status *block = &actwk[3];

    reset_rblk4_state();
    child->actfree[18] = 255;
    child->actfree[19] = 0;
    child->cddat = 8;
    set_actor_word(child, 28, 3);
    set_actor_word(child, 29, 320);
    block->xposi.w.h = 100;
    block->yposi.w.h = 80;
    block->patno = 0;
    rblk4(child);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == child);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_EQ_INT(ctx, 320, frameout_s00_x);

    reset_rblk4_state();
    child->actfree[18] = 255;
    child->actfree[19] = 1;
    set_actor_word(child, 28, 3);
    block->xposi.w.h = 100;
    block->yposi.w.h = 80;
    block->patno = 0;
    rblk4(child);

    reset_rblk4_state();
    child->actfree[18] = 255;
    child->actfree[19] = 0;
    child->r_no0 = 2;
    child->cddat = 0;
    set_actor_word(child, 28, 3);
    block->xposi.w.h = 100;
    block->yposi.w.h = 80;
    block->patno = 1;
    rblk4_ract_move(child);

    reset_rblk4_state();
    child->actfree[19] = 1;
    child->cddat = 0;
    child->actfree[20] = 99;
    child->actfree[21] = 99;
    set_actor_word(child, 28, 3);
    block->patno = 2;
    block->xposi.w.h = 100;
    block->yposi.w.h = 80;
    rblk4_ract_move(child);

    reset_rblk4_state();
    child->actfree[19] = 1;
    child->cddat = 32;
    set_actor_word(child, 28, 3);
    block->patno = 3;
    block->xposi.w.h = 100;
    block->yposi.w.h = 80;
    rblk4_ract_move(child);

    reset_rblk4_state();
    child->actfree[19] = 1;
    child->cddat = 32;
    child->actfree[21] = 1;
    set_actor_word(child, 28, 3);
    block->patno = 0;
    block->xposi.w.h = 100;
    block->yposi.w.h = 80;
    rblk4_ract_move(child);

    reset_rblk4_state();
    child->actfree[19] = 1;
    set_actor_word(child, 28, 3);
    block->patno = 4;
    rblk4_ract_move(child);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == child);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk_count);
}

TEST_MAIN_BEGIN;
test_rblk4_patterns_capture_literal_data(&ctx);
test_rblk4_initializes_main_and_spawned_actors(&ctx);
test_rblk4_wait_paths(&ctx);
test_rblk4_move_sequence(&ctx);
test_rblk4_push_sequence(&ctx);
test_rblk4_ride_actor_paths(&ctx);
TEST_MAIN_END
