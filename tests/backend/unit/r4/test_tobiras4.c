#include <stddef.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 switchflag[32];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int actwkchk2_count;
static sprite_status *actwkchk2_source;
static sprite_status *actwkchk2_queue[4];
static int actwkchk2_queue_count;
static int actwkchk2_queue_pos;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;

void actionsub(sprite_status *pActwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
static void test_s_mem_cpy(void *dst, void *src, Sint32 size);
void (*sMemCpy)(void *, void *, Sint32) = test_s_mem_cpy;

#include "src/r4/tobiras4.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk) {
    ++actwkchk2_count;
    actwkchk2_source = pActwk;
    if (actwkchk2_queue_pos >= actwkchk2_queue_count) {
        *ppNewActwk = 0;
        return 1;
    }

    *ppNewActwk = actwkchk2_queue[actwkchk2_queue_pos++];
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
    return 0;
}

static void test_s_mem_cpy(void *dst, void *src, Sint32 size) {
    memmove(dst, src, (size_t)size);
}

static void reset_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(switchflag, 0, sizeof(switchflag));
    actionsub_count = 0;
    actionsub_actor = 0;
    actwkchk2_count = 0;
    actwkchk2_source = 0;
    memset(actwkchk2_queue, 0, sizeof(actwkchk2_queue));
    actwkchk2_queue_count = 0;
    actwkchk2_queue_pos = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
}

static void queue_actwkchk2(sprite_status *actor) {
    actwkchk2_queue[actwkchk2_queue_count++] = actor;
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

static int legacy_word_actfree_offset(int word_index) {
    return (word_index * 2) - (int)offsetof(sprite_status, actfree);
}

static int legacy_long_actfree_offset(int long_index) {
    return (long_index * 4) - (int)offsetof(sprite_status, actfree);
}

static void set_legacy_word(sprite_status *actor, int word_index,
                            Sint16 value) {
    set_actfree_word(actor, legacy_word_actfree_offset(word_index), value);
}

static Sint16 get_legacy_word(sprite_status *actor, int word_index) {
    return get_actfree_word(actor, legacy_word_actfree_offset(word_index));
}

static void set_legacy_long(sprite_status *actor, int long_index,
                            Sint32 value) {
    set_actfree_long(actor, legacy_long_actfree_offset(long_index), value);
}

static Sint32 get_legacy_long(sprite_status *actor, int long_index) {
    return get_actfree_long(actor, legacy_long_actfree_offset(long_index));
}

static void test_tobiras4_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_tobiras4[0] == &pat00);
    TEST_ASSERT_TRUE(ctx, pat_tobiras4[1] == &pat01);
    TEST_ASSERT_TRUE(ctx, pat_tobiras4[2] == &pat02);
    TEST_ASSERT_EQ_INT(ctx, 1, pat00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -64, pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -64, pat00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 509, pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -16, pat01.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, 510, pat01.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 511, pat02.spra[0].index);
}

static void test_tobiras4_init_frames_out_when_switch_already_set(
    test_context *ctx) {
    sprite_status *door = &actwk[5];

    reset_state();
    door->userflag.b.h = 3;
    switchflag[3] = 1;

    tobiras4(door);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == door);
    TEST_ASSERT_EQ_INT(ctx, 0, door->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, door->actflg);
}

static void test_tobiras4_init_sets_static_door_shape(test_context *ctx) {
    sprite_status *door = &actwk[5];

    reset_state();
    door->userflag.b.h = 4;

    tobiras4(door);

    TEST_ASSERT_EQ_INT(ctx, 2, door->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, door->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 17514, door->sproffset);
    TEST_ASSERT_TRUE(ctx, door->patbase == pat_tobiras4);
    TEST_ASSERT_EQ_INT(ctx, 64, door->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 64, door->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 3, door->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 64, door->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_tobiras4_sense_clear_switch_checks_player_and_stays(
    test_context *ctx) {
    sprite_status *door = &actwk[5];

    reset_state();
    door->userflag.b.h = 6;
    tobiras4(door);

    tobiras4(door);

    TEST_ASSERT_EQ_INT(ctx, 2, door->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == door);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == door);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == door);
}

static void test_tobiras4_sense_set_switch_spawns_three_falling_panels(
    test_context *ctx) {
    sprite_status *door = &actwk[5];
    sprite_status *panel0 = &actwk[20];
    sprite_status *panel1 = &actwk[21];
    sprite_status *panel2 = &actwk[22];

    reset_state();
    door->userflag.b.h = 7;
    door->xposi.w.h = 200;
    door->yposi.w.h = 300;
    tobiras4(door);
    switchflag[7] = 1;
    queue_actwkchk2(panel0);
    queue_actwkchk2(panel1);
    queue_actwkchk2(panel2);

    tobiras4(door);

    TEST_ASSERT_EQ_INT(ctx, 4, door->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 16, door->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, door->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 128, get_legacy_word(door, 23));
    TEST_ASSERT_EQ_INT(ctx, -65536, get_legacy_long(door, 12));
    TEST_ASSERT_EQ_INT(ctx, 1, door->patno);
    TEST_ASSERT_EQ_INT(ctx, 152, door->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 3, actwkchk2_count);
    TEST_ASSERT_TRUE(ctx, actwkchk2_source == door);
    TEST_ASSERT_EQ_INT(ctx, 184, panel0->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, panel0->patno);
    TEST_ASSERT_EQ_INT(ctx, 65536, get_legacy_long(panel0, 12));
    TEST_ASSERT_EQ_INT(ctx, 216, panel1->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, panel1->patno);
    TEST_ASSERT_EQ_INT(ctx, -65536, get_legacy_long(panel1, 12));
    TEST_ASSERT_EQ_INT(ctx, 248, panel2->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, panel2->patno);
    TEST_ASSERT_EQ_INT(ctx, 65536, get_legacy_long(panel2, 12));
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_tobiras4_sense_allocation_failure_frames_out(
    test_context *ctx) {
    sprite_status *door = &actwk[5];

    reset_state();
    door->userflag.b.h = 8;
    tobiras4(door);
    switchflag[8] = 1;
    queue_actwkchk2(&actwk[20]);

    tobiras4(door);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == door);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk_count);
}

static void test_tobiras4_down_moves_until_counter_negative(test_context *ctx) {
    sprite_status *door = &actwk[5];

    reset_state();
    door->r_no0 = 4;
    door->yposi.l = 100 << 16;
    set_legacy_word(door, 23, 1);
    set_legacy_long(door, 12, -65536);

    tobiras4(door);

    TEST_ASSERT_EQ_INT(ctx, 0, get_legacy_word(door, 23));
    TEST_ASSERT_EQ_INT(ctx, 101, door->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_state();
    door->r_no0 = 4;
    set_legacy_word(door, 23, -1);

    tobiras4(door);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == door);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_tobiras4_patterns_capture_literal_data(&ctx);
    test_tobiras4_init_frames_out_when_switch_already_set(&ctx);
    test_tobiras4_init_sets_static_door_shape(&ctx);
    test_tobiras4_sense_clear_switch_checks_player_and_stays(&ctx);
    test_tobiras4_sense_set_switch_spawns_three_falling_panels(&ctx);
    test_tobiras4_sense_allocation_failure_frames_out(&ctx);
    test_tobiras4_down_moves_until_counter_negative(&ctx);
TEST_MAIN_END
