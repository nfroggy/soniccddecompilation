#include <stddef.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_pos;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static Sint16 hitchk_result;
static int ride_on_clr_count;
static sprite_status *ride_on_clr_actor;
static sprite_status *ride_on_clr_player;
static int soundset_count;
static Sint16 soundset_requests[4];

void actionsub(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk);
void soundset(Sint16 ReqNo);

#include "src/r4/kowasi4.c"

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

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++hitchk_count;
    hitchk_actor = pActwk;
    hitchk_player = pPlayerwk;
    return hitchk_result;
}

Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_clr_count;
    ride_on_clr_actor = pActwk;
    ride_on_clr_player = pPlayerwk;
    return 0;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 4) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

static void reset_state(void) {
    memset(actwk, 0, sizeof(actwk));
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
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    hitchk_result = 0;
    ride_on_clr_count = 0;
    ride_on_clr_actor = 0;
    ride_on_clr_player = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void set_actfree_long(sprite_status *actor, int offset, Sint32 value) {
    Uint32 bits = (Uint32)value;
    actor->actfree[offset] = (Uint8)(bits & 255);
    actor->actfree[offset + 1] = (Uint8)((bits >> 8) & 255);
    actor->actfree[offset + 2] = (Uint8)((bits >> 16) & 255);
    actor->actfree[offset + 3] = (Uint8)(bits >> 24);
}

static Sint32 get_actfree_long(sprite_status *actor, int offset) {
    Uint32 bits = (Uint32)actor->actfree[offset] |
                  ((Uint32)actor->actfree[offset + 1] << 8) |
                  ((Uint32)actor->actfree[offset + 2] << 16) |
                  ((Uint32)actor->actfree[offset + 3] << 24);
    return (Sint32)bits;
}

static void set_actfree_word(sprite_status *actor, int offset, Sint16 value) {
    Uint16 bits = (Uint16)value;
    actor->actfree[offset] = (Uint8)(bits & 255);
    actor->actfree[offset + 1] = (Uint8)(bits >> 8);
}

static Sint16 get_actfree_word(sprite_status *actor, int offset) {
    Uint16 bits = (Uint16)actor->actfree[offset] |
                  ((Uint16)actor->actfree[offset + 1] << 8);
    return (Sint16)bits;
}

static int legacy_word_actfree_offset(int word_index) {
    return (word_index * 2) - (int)offsetof(sprite_status, actfree);
}

static void set_legacy_word(sprite_status *actor, int word_index, Sint16 value) {
    set_actfree_word(actor, legacy_word_actfree_offset(word_index), value);
}

static Sint16 get_legacy_word(sprite_status *actor, int word_index) {
    return get_actfree_word(actor, legacy_word_actfree_offset(word_index));
}

static void test_kowasi4_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_kowasi4[0] == &pat00);
    TEST_ASSERT_TRUE(ctx, pat_kowasi4[1] == &pat01);
    TEST_ASSERT_EQ_INT(ctx, 1, pat00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -16, pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, pat00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 484, pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -8, pat01.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -8, pat01.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 485, pat01.spra[0].index);
}

static void test_kowasi4_init_sets_fields_and_waits(test_context *ctx) {
    sprite_status *block = &actwk[3];

    reset_state();

    kowasi4(block);

    TEST_ASSERT_EQ_INT(ctx, 2, block->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, block->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 1, block->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, block->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, block->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, block->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 243, block->colino);
    TEST_ASSERT_EQ_INT(ctx, 17252, block->sproffset);
    TEST_ASSERT_TRUE(ctx, block->patbase == pat_kowasi4);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == block);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == block);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == block);
}

static void test_kowasi4_wait_collision_spawns_fragments(test_context *ctx) {
    sprite_status *block = &actwk[3];

    reset_state();
    block->actno = 57;
    block->r_no0 = 2;
    block->actflg = 4;
    block->sprpri = 1;
    block->sproffset = 17252;
    block->patbase = pat_kowasi4;
    block->colino = 243;
    block->colicnt = 1;
    block->xposi.w.h = 100;
    block->yposi.w.h = 200;
    actwk[0].xspeed.w = 100;
    actwk[0].yspeed.w = -32;
    hitchk_result = 1;
    queue_actor(&actwk[40]);
    queue_actor(&actwk[41]);
    queue_actor(&actwk[42]);

    kowasi4(block);

    TEST_ASSERT_EQ_INT(ctx, 4, block->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, block->colino);
    TEST_ASSERT_EQ_INT(ctx, 0, block->colicnt);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == block);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 176, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 3, actwkchk_count);

    TEST_ASSERT_EQ_INT(ctx, 1, block->patno);
    TEST_ASSERT_EQ_INT(ctx, 8, block->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 8, block->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 8, block->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 88, block->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 190, block->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -252434, get_actfree_long(block, 0));
    TEST_ASSERT_EQ_INT(ctx, -67760, get_actfree_long(block, 4));

    TEST_ASSERT_EQ_INT(ctx, 57, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[40].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[40].patno);
    TEST_ASSERT_EQ_INT(ctx, 108, actwk[40].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 192, actwk[40].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -189326, get_actfree_long(&actwk[40], 0));
    TEST_ASSERT_EQ_INT(ctx, -189326, get_actfree_long(&actwk[40], 4));

    TEST_ASSERT_EQ_INT(ctx, 92, actwk[41].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 208, actwk[41].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 108, actwk[42].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 208, actwk[42].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == block);
}

static void test_kowasi4_wait_negative_xspeed_uses_flipped_speed_table(
    test_context *ctx) {
    sprite_status *block = &actwk[3];

    reset_state();
    block->r_no0 = 2;
    block->colicnt = 1;
    block->xposi.w.h = 100;
    block->yposi.w.h = 200;
    actwk[0].xspeed.w = -100;
    actwk[0].yspeed.w = 32;
    queue_actor(&actwk[40]);
    queue_actor(&actwk[41]);
    queue_actor(&actwk[42]);

    kowasi4(block);

    TEST_ASSERT_EQ_INT(ctx, 252434, get_actfree_long(block, 0));
    TEST_ASSERT_EQ_INT(ctx, -67760, get_actfree_long(block, 4));
    TEST_ASSERT_EQ_INT(ctx, 189326, get_actfree_long(&actwk[40], 0));
    TEST_ASSERT_EQ_INT(ctx, -189326, get_actfree_long(&actwk[40], 4));
}

static void test_kowasi4_wait_vertical_speed_dominates_base_table(
    test_context *ctx) {
    sprite_status *block = &actwk[3];

    reset_state();
    block->r_no0 = 2;
    block->colicnt = 1;
    block->xposi.w.h = 100;
    block->yposi.w.h = 200;
    actwk[0].xspeed.w = 20;
    actwk[0].yspeed.w = -80;
    queue_actor(&actwk[40]);
    queue_actor(&actwk[41]);
    queue_actor(&actwk[42]);

    kowasi4(block);

    TEST_ASSERT_EQ_INT(ctx, -84144, get_actfree_long(block, 0));
    TEST_ASSERT_EQ_INT(ctx, -376832, get_actfree_long(block, 4));
    TEST_ASSERT_EQ_INT(ctx, 84144, get_actfree_long(&actwk[40], 0));
    TEST_ASSERT_EQ_INT(ctx, -393216, get_actfree_long(&actwk[40], 4));
}

static void test_kowasi4_wait_allocation_failure_keeps_first_fragment_only(
    test_context *ctx) {
    sprite_status *block = &actwk[3];

    reset_state();
    block->r_no0 = 2;
    block->colicnt = 1;
    block->xposi.w.h = 100;
    block->yposi.w.h = 200;
    actwk[0].xspeed.w = 100;

    kowasi4(block);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, block->patno);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_kowasi4_down_moves_or_frames_out_by_player_distance(
    test_context *ctx) {
    sprite_status *fragment = &actwk[3];

    reset_state();
    fragment->r_no0 = 4;
    fragment->xposi.l = 100 << 16;
    fragment->yposi.l = 200 << 16;
    set_actfree_long(fragment, 0, 2 << 16);
    set_actfree_long(fragment, 4, -1 << 16);
    actwk[0].yposi.w.h = 200;

    kowasi4(fragment);

    TEST_ASSERT_EQ_INT(ctx, 102, fragment->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 199, fragment->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, (Sint32)((-1 << 16) + 16384),
                       get_actfree_long(fragment, 4));
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == fragment);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_state();
    fragment = &actwk[3];
    fragment->r_no0 = 4;
    fragment->yposi.l = 500 << 16;
    actwk[0].yposi.w.h = 200;

    kowasi4(fragment);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == fragment);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_kowasi4_patterns_capture_literal_data(&ctx);
    test_kowasi4_init_sets_fields_and_waits(&ctx);
    test_kowasi4_wait_collision_spawns_fragments(&ctx);
    test_kowasi4_wait_negative_xspeed_uses_flipped_speed_table(&ctx);
    test_kowasi4_wait_vertical_speed_dominates_base_table(&ctx);
    test_kowasi4_wait_allocation_failure_keeps_first_fragment_only(&ctx);
    test_kowasi4_down_moves_or_frames_out_by_player_distance(&ctx);
TEST_MAIN_END
