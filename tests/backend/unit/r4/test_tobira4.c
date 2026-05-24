#include <stddef.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 switchflag[32];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static Sint16 hitchk_result;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r4/tobira4.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
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

static void reset_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(switchflag, 0, sizeof(switchflag));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    hitchk_result = 0;
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

static void assert_entry_callbacks(test_context *ctx, sprite_status *door) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == door);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == door);
}

static void test_tobira4_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, tobira4pat[0] == &pat0);
    TEST_ASSERT_TRUE(ctx, tobira4pat[1] == &pat1);
    TEST_ASSERT_TRUE(ctx, tobira4pat[2] == &pat2);
    TEST_ASSERT_EQ_INT(ctx, 506, pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 507, pat1.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 508, pat2.spra[0].index);
}

static void test_tobira4_init_captures_position_and_vertical_layout(
    test_context *ctx) {
    sprite_status *door = &actwk[5];

    reset_state();
    door->xposi.w.h = 100;
    door->yposi.w.h = 200;
    door->userflag.b.h = 0x21;

    tobira4_init(door);

    TEST_ASSERT_EQ_INT(ctx, 2, door->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, door->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 17472, door->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 2, door->sprpri);
    TEST_ASSERT_TRUE(ctx, door->patbase == tobira4pat);
    TEST_ASSERT_EQ_INT(ctx, 100, get_legacy_word(door, 29));
    TEST_ASSERT_EQ_INT(ctx, 200, get_legacy_word(door, 27));
    TEST_ASSERT_EQ_INT(ctx, 1, door->actfree[6]);
    TEST_ASSERT_EQ_INT(ctx, 2, door->actfree[7]);
    TEST_ASSERT_EQ_INT(ctx, 8, door->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 32, door->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 64, door->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 1, door->patno);
}

static void test_tobira4_init_captures_wide_layout(test_context *ctx) {
    sprite_status *door = &actwk[5];

    reset_state();
    door->xposi.w.h = 300;
    door->yposi.w.h = 400;
    door->userflag.b.h = 0x40;

    tobira4_init(door);

    TEST_ASSERT_EQ_INT(ctx, 0, door->actfree[6]);
    TEST_ASSERT_EQ_INT(ctx, 4, door->actfree[7]);
    TEST_ASSERT_EQ_INT(ctx, 8, door->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 64, door->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 128, door->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 0, door->patno);
}

static void test_tobira4_move_opens_and_enters_check_state(test_context *ctx) {
    sprite_status *door = &actwk[5];
    sprite_status *player = &actwk[0];

    reset_state();
    door->xposi.w.h = 100;
    door->yposi.w.h = 200;
    door->userflag.b.h = 0x21;
    tobira4_init(door);
    door->actfree[16] = 56;
    switchflag[1] = 128;
    player->xposi.w.h = 120;
    player->yposi.w.h = 220;

    tobira4_move(door);

    TEST_ASSERT_EQ_INT(ctx, 0, door->actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 64, door->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 136, door->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4, door->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == door);
    TEST_ASSERT_TRUE(ctx, hitchk_player == player);
}

static void test_tobira4_move_partial_open_overrides_closed_switch(
    test_context *ctx) {
    sprite_status *door = &actwk[5];

    reset_state();
    door->xposi.w.h = 100;
    door->yposi.w.h = 200;
    door->userflag.b.h = 0x01;
    tobira4_init(door);
    door->actfree[16] = 8;
    switchflag[1] = 0;

    tobira4_move(door);

    TEST_ASSERT_EQ_INT(ctx, 0, door->actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 16, door->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 84, door->xposi.w.h);
}

static void test_tobira4_move_type_four_does_not_enter_check_state(
    test_context *ctx) {
    sprite_status *door = &actwk[5];

    reset_state();
    door->xposi.w.h = 100;
    door->yposi.w.h = 200;
    door->userflag.b.h = 0x41;
    tobira4_init(door);
    door->actfree[16] = 120;
    switchflag[1] = 128;

    tobira4_move(door);

    TEST_ASSERT_EQ_INT(ctx, 128, door->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 72, door->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, door->r_no0);
}

static void test_tobira4_close_counts_down_and_returns_to_move(
    test_context *ctx) {
    sprite_status *door = &actwk[5];

    reset_state();
    door->xposi.w.h = 100;
    door->yposi.w.h = 200;
    door->userflag.b.h = 0x10;
    tobira4_init(door);
    door->r_no0 = 6;
    door->actfree[16] = 16;

    tobira4_clse(door);
    TEST_ASSERT_EQ_INT(ctx, 8, door->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 108, door->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 6, door->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);

    tobira4_clse(door);
    TEST_ASSERT_EQ_INT(ctx, 0, door->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 100, door->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, door->r_no0);
}

static void test_tobira4_type_helpers_move_from_original_position(
    test_context *ctx) {
    sprite_status *door = &actwk[5];

    reset_state();
    set_legacy_word(door, 29, 100);
    set_legacy_word(door, 27, 200);

    door->actfree[18] = 255;
    door->actfree[16] = 16;
    type1(door);
    TEST_ASSERT_EQ_INT(ctx, 8, door->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 92, door->xposi.w.h);

    door->actfree[18] = 0;
    door->actfree[16] = 4;
    door->actfree[17] = 20;
    type2(door);
    TEST_ASSERT_EQ_INT(ctx, 12, door->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 112, door->xposi.w.h);

    door->actfree[18] = 0;
    door->actfree[16] = 60;
    door->actfree[17] = 64;
    type3(door);
    TEST_ASSERT_EQ_INT(ctx, 64, door->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 136, door->yposi.w.h);

    door->actfree[18] = 255;
    door->actfree[16] = 4;
    type4(door);
    TEST_ASSERT_EQ_INT(ctx, 0, door->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 200, door->yposi.w.h);

    door->actfree[7] = 4;
    door->actfree[18] = 0;
    door->actfree[16] = 0;
    door->actfree[17] = 16;
    tobira4_cnt(door);
    TEST_ASSERT_EQ_INT(ctx, 8, door->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 192, door->yposi.w.h);
}

static void test_tobira4_check_type1_waits_and_then_advances(
    test_context *ctx) {
    sprite_status *door = &actwk[5];
    sprite_status *player = &actwk[0];

    reset_state();
    door->xposi.w.h = 100;
    door->yposi.w.h = 100;
    door->userflag.b.h = 0x00;
    tobira4_init(door);
    player->yposi.w.h = 200;
    tobira4_move(door);
    door->r_no0 = 4;
    player->sprvsize = 10;
    player->yposi.w.h = 95;
    cl_type1(door);
    TEST_ASSERT_EQ_INT(ctx, 4, door->r_no0);

    player->yposi.w.h = 85;
    cl_type1(door);
    TEST_ASSERT_EQ_INT(ctx, 4, door->r_no0);

    player->yposi.w.h = 70;
    cl_type1(door);
    TEST_ASSERT_EQ_INT(ctx, 6, door->r_no0);
}

static void test_tobira4_check_type1_other_side_returns(test_context *ctx) {
    sprite_status *door = &actwk[5];
    sprite_status *player = &actwk[0];

    reset_state();
    door->xposi.w.h = 100;
    door->yposi.w.h = 100;
    door->userflag.b.h = 0x00;
    tobira4_init(door);
    player->yposi.w.h = 80;
    tobira4_move(door);
    door->r_no0 = 4;
    player->sprvsize = 10;
    player->yposi.w.h = 120;

    cl_type1(door);

    TEST_ASSERT_EQ_INT(ctx, 4, door->r_no0);

    player->sprvsize = 0;
    cl_type1(door);

    TEST_ASSERT_EQ_INT(ctx, 4, door->r_no0);
}

static void test_tobira4_check_type2_covers_both_sides(test_context *ctx) {
    sprite_status *door = &actwk[5];
    sprite_status *player = &actwk[0];

    reset_state();
    door->xposi.w.h = 100;
    door->yposi.w.h = 200;
    door->userflag.b.h = 0x20;
    tobira4_init(door);
    player->xposi.w.h = 200;
    tobira4_move(door);
    door->r_no0 = 4;
    player->sprhs = 10;
    player->xposi.w.h = 95;
    cl_type2(door);
    TEST_ASSERT_EQ_INT(ctx, 4, door->r_no0);

    player->sprhs = 0;
    player->xposi.w.h = 93;
    cl_type2(door);
    TEST_ASSERT_EQ_INT(ctx, 4, door->r_no0);

    player->sprhs = 10;
    player->xposi.w.h = 70;
    cl_type2(door);
    TEST_ASSERT_EQ_INT(ctx, 6, door->r_no0);

    door->r_no0 = 4;
    player->xposi.w.h = 50;
    tobira4_move(door);
    door->r_no0 = 4;
    player->sprhs = 8;
    player->xposi.w.h = 104;
    cl_type2(door);
    TEST_ASSERT_EQ_INT(ctx, 4, door->r_no0);

    player->sprhs = 0;
    player->xposi.w.h = 104;
    cl_type2(door);
    TEST_ASSERT_EQ_INT(ctx, 4, door->r_no0);

    player->sprhs = 8;
    player->xposi.w.h = 120;
    cl_type2(door);
    TEST_ASSERT_EQ_INT(ctx, 6, door->r_no0);
}

static void test_tobira4_check_dispatches_and_calls_hitchk(test_context *ctx) {
    sprite_status *door = &actwk[5];
    sprite_status *player = &actwk[0];

    reset_state();
    door->xposi.w.h = 100;
    door->yposi.w.h = 200;
    door->userflag.b.h = 0x20;
    tobira4_init(door);
    player->xposi.w.h = 40;
    tobira4_move(door);
    door->r_no0 = 4;
    player->sprhs = 8;
    player->xposi.w.h = 120;
    hitchk_count = 0;

    tobira4_chek(door);

    TEST_ASSERT_EQ_INT(ctx, 6, door->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == door);
    TEST_ASSERT_TRUE(ctx, hitchk_player == player);
}

static void test_tobira4_entry_dispatches_callbacks(test_context *ctx) {
    sprite_status *door = &actwk[5];

    reset_state();
    door->xposi.w.h = 100;
    door->yposi.w.h = 200;

    tobira4(door);

    TEST_ASSERT_EQ_INT(ctx, 2, door->r_no0);
    assert_entry_callbacks(ctx, door);
}

TEST_MAIN_BEGIN;
    test_tobira4_patterns_capture_literal_data(&ctx);
    test_tobira4_init_captures_position_and_vertical_layout(&ctx);
    test_tobira4_init_captures_wide_layout(&ctx);
    test_tobira4_move_opens_and_enters_check_state(&ctx);
    test_tobira4_move_partial_open_overrides_closed_switch(&ctx);
    test_tobira4_move_type_four_does_not_enter_check_state(&ctx);
    test_tobira4_close_counts_down_and_returns_to_move(&ctx);
    test_tobira4_type_helpers_move_from_original_position(&ctx);
    test_tobira4_check_type1_waits_and_then_advances(&ctx);
    test_tobira4_check_type1_other_side_returns(&ctx);
    test_tobira4_check_type2_covers_both_sides(&ctx);
    test_tobira4_check_dispatches_and_calls_hitchk(&ctx);
    test_tobira4_entry_dispatches_callbacks(&ctx);
TEST_MAIN_END
