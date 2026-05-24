#include <stddef.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
int_union scra_h_posit;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int actwkchk2_count;
static sprite_status *actwkchk2_source;
static sprite_status *actwkchk2_queue[4];
static int actwkchk2_queue_count;
static int actwkchk2_queue_pos;
static int emycol_d_count;
static Sint16 emycol_d_result;
static int emycol_u_count;
static Sint16 emycol_u_result;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_x;
static int ride_on_chk_count;
static sprite_status *ride_on_chk_actor;
static sprite_status *ride_on_chk_player;

void actionsub(sprite_status *pActwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);
Sint16 emycol_d(sprite_status *pActwk);
Sint16 emycol_u(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r4/swblk4.c"

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

Sint16 emycol_d(sprite_status *pActwk) {
    (void)pActwk;
    ++emycol_d_count;
    return emycol_d_result;
}

Sint16 emycol_u(sprite_status *pActwk) {
    (void)pActwk;
    ++emycol_u_count;
    return emycol_u_result;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi) {
    ++frameout_s00_count;
    frameout_s00_actor = pActwk;
    frameout_s00_x = xposi;
    return 0;
}

Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_chk_count;
    ride_on_chk_actor = pActwk;
    ride_on_chk_player = pPlayerwk;
    return 0;
}

static void reset_state(void) {
    memset(actwk, 0, sizeof(actwk));
    scra_h_posit.l = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    actwkchk2_count = 0;
    actwkchk2_source = 0;
    memset(actwkchk2_queue, 0, sizeof(actwkchk2_queue));
    actwkchk2_queue_count = 0;
    actwkchk2_queue_pos = 0;
    emycol_d_count = 0;
    emycol_d_result = 0;
    emycol_u_count = 0;
    emycol_u_result = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_x = 0;
    ride_on_chk_count = 0;
    ride_on_chk_actor = 0;
    ride_on_chk_player = 0;
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

static int legacy_word_actfree_offset(int word_index) {
    return (word_index * 2) - (int)offsetof(sprite_status, actfree);
}

static void set_legacy_word(sprite_status *actor, int word_index,
                            Sint16 value) {
    set_actfree_word(actor, legacy_word_actfree_offset(word_index), value);
}

static Sint16 get_legacy_word(sprite_status *actor, int word_index) {
    return get_actfree_word(actor, legacy_word_actfree_offset(word_index));
}

static void test_swblk4_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, switchr4pat[0] == &switchr4_pat0);
    TEST_ASSERT_TRUE(ctx, switchr4pat[1] == &switchr4_pat1);
    TEST_ASSERT_EQ_INT(ctx, -16, switchr4_pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -12, switchr4_pat0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 472, switchr4_pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 473, switchr4_pat1.spra[0].index);
    TEST_ASSERT_TRUE(ctx, swblkr4pat[0] == &swblkr4_pat0);
    TEST_ASSERT_TRUE(ctx, swblkr4pat[1] == &swblkr4_pat1);
    TEST_ASSERT_TRUE(ctx, swblkr4pat[2] == &swblkr4_pat2);
    TEST_ASSERT_EQ_INT(ctx, 474, swblkr4_pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 475, swblkr4_pat1.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 476, swblkr4_pat2.spra[0].index);
}

static void test_switchr4_init_pressed_by_player(test_context *ctx) {
    sprite_status *sw = &actwk[5];
    sprite_status *player = &actwk[0];

    reset_state();
    sw->xposi.w.h = 100;
    sw->yposi.w.h = 100;
    player->xposi.w.h = 100;
    player->yposi.w.h = 76;
    player->sprvsize = 16;

    switchr4(sw);

    TEST_ASSERT_EQ_INT(ctx, 2, sw->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, sw->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 1290, sw->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 3, sw->sprpri);
    TEST_ASSERT_TRUE(ctx, sw->patbase == switchr4pat);
    TEST_ASSERT_EQ_INT(ctx, 100, get_legacy_word(sw, 29));
    TEST_ASSERT_EQ_INT(ctx, 14, sw->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 5, sw->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, sw->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, sw->patno);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);
    TEST_ASSERT_TRUE(ctx, ride_on_chk_actor == sw);
    TEST_ASSERT_TRUE(ctx, ride_on_chk_player == player);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_EQ_INT(ctx, 100, frameout_s00_x);
}

static void test_switchr4_move_clears_when_player_is_outside(test_context *ctx) {
    sprite_status *sw = &actwk[5];
    sprite_status *player = &actwk[0];

    reset_state();
    sw->r_no0 = 2;
    sw->userflag.b.h = 1;
    sw->xposi.w.h = 100;
    sw->yposi.w.h = 100;
    sw->sprhsize = 14;
    sw->sprvsize = 5;
    set_legacy_word(sw, 29, 100);
    player->xposi.w.h = 200;
    player->yposi.w.h = 76;
    player->sprvsize = 16;

    switchr4(sw);

    TEST_ASSERT_EQ_INT(ctx, 0, sw->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, sw->patno);
}

static void test_switchr4_colichk_rejects_player_too_high(test_context *ctx) {
    sprite_status *sw = &actwk[5];
    sprite_status *player = &actwk[0];

    reset_state();
    sw->xposi.w.h = 100;
    sw->yposi.w.h = 100;
    sw->sprhsize = 14;
    player->xposi.w.h = 100;
    player->yposi.w.h = 40;
    player->sprvsize = 16;

    TEST_ASSERT_EQ_INT(ctx, 0, switchr4_colichk(sw, player));
}

static void test_switchr4_follows_ride_actor_offset(test_context *ctx) {
    sprite_status *sw = &actwk[5];
    sprite_status *ride = &actwk[30];

    reset_state();
    sw->r_no0 = 2;
    sw->sprhsize = 14;
    sw->sprvsize = 5;
    sw->actfree[14] = (Uint8)-7;
    sw->actfree[15] = 9;
    set_legacy_word(sw, 28, 30);
    set_legacy_word(sw, 29, 77);
    ride->xposi.w.h = 400;
    ride->yposi.w.h = 500;

    switchr4(sw);

    TEST_ASSERT_EQ_INT(ctx, 393, sw->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 509, sw->yposi.w.h);
}

static void test_swblkr4_init_spawns_block_and_switch(test_context *ctx) {
    sprite_status *block = &actwk[5];
    sprite_status *child = &actwk[20];
    sprite_status *sw = &actwk[21];

    reset_state();
    block->actno = 48;
    block->xposi.w.h = 100;
    block->yposi.w.h = 200;
    queue_actwkchk2(child);
    queue_actwkchk2(sw);

    swblkr4(block);

    TEST_ASSERT_EQ_INT(ctx, 2, block->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, block->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 17514, block->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 3, block->sprpri);
    TEST_ASSERT_TRUE(ctx, block->patbase == swblkr4pat);
    TEST_ASSERT_EQ_INT(ctx, 100, get_legacy_word(block, 29));
    TEST_ASSERT_EQ_INT(ctx, 200, get_legacy_word(block, 27));
    TEST_ASSERT_EQ_INT(ctx, 116, block->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 16, block->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 64, block->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, block->patno);
    TEST_ASSERT_EQ_INT(ctx, 20, get_legacy_word(block, 28));
    TEST_ASSERT_EQ_INT(ctx, 21, get_legacy_word(block, 26));
    TEST_ASSERT_EQ_INT(ctx, 8, block->yspeed.w);

    TEST_ASSERT_EQ_INT(ctx, 48, child->actno);
    TEST_ASSERT_EQ_INT(ctx, 1, child->actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 84, child->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 232, child->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 5, get_legacy_word(child, 28));
    TEST_ASSERT_EQ_INT(ctx, 2, child->patno);
    TEST_ASSERT_EQ_INT(ctx, 0, child->yspeed.w);

    TEST_ASSERT_EQ_INT(ctx, 49, sw->actno);
    TEST_ASSERT_EQ_INT(ctx, 5, get_legacy_word(sw, 28));
    TEST_ASSERT_EQ_INT(ctx, 188, sw->actfree[15]);
    TEST_ASSERT_EQ_INT(ctx, 100, get_legacy_word(sw, 29));
    TEST_ASSERT_EQ_INT(ctx, 200, get_legacy_word(sw, 27));
    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk2_count);
    TEST_ASSERT_TRUE(ctx, actwkchk2_source == block);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_swblkr4_init_frames_out_on_allocation_failures(
    test_context *ctx) {
    sprite_status *block = &actwk[5];

    reset_state();
    block->actno = 48;
    block->xposi.w.h = 100;
    block->yposi.w.h = 200;

    swblkr4_init(block);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == block);

    reset_state();
    block->actno = 48;
    block->xposi.w.h = 100;
    block->yposi.w.h = 200;
    queue_actwkchk2(&actwk[20]);

    swblkr4_init(block);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == block);
}

static void test_swblkr4_child_follows_or_frames_with_parent(test_context *ctx) {
    sprite_status *child = &actwk[20];
    sprite_status *parent = &actwk[5];

    reset_state();
    child->r_no0 = 2;
    child->actfree[18] = 1;
    set_legacy_word(child, 28, 5);
    parent->actno = 48;
    parent->xposi.w.h = 200;
    parent->yposi.w.h = 300;

    swblkr4(child);

    TEST_ASSERT_EQ_INT(ctx, 168, child->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 332, child->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_state();
    child->r_no0 = 2;
    child->actfree[18] = 1;
    set_legacy_word(child, 28, 5);
    parent->actno = 47;

    swblkr4(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_swblkr4_move_collision_and_switch_speed_limits(
    test_context *ctx) {
    sprite_status *block = &actwk[5];
    sprite_status *child = &actwk[20];
    sprite_status *sw = &actwk[21];

    reset_state();
    block->yspeed.w = -128;
    block->yposi.w.h = 300;
    set_legacy_word(block, 28, 20);
    set_legacy_word(block, 26, 21);
    child->actno = 48;
    sw->actno = 49;
    sw->userflag.b.h = 1;
    emycol_u_result = -1;

    swblkr4_move(block);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_u_count);
    TEST_ASSERT_EQ_INT(ctx, 0, emycol_d_count);
    TEST_ASSERT_EQ_INT(ctx, -8, block->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, child->yspeed.w);

    block->yspeed.w = -256;
    emycol_u_result = 0;
    swblkr4_move(block);
    TEST_ASSERT_EQ_INT(ctx, -256, block->yspeed.w);

    reset_state();
    block->yspeed.w = 128;
    set_legacy_word(block, 28, 20);
    set_legacy_word(block, 26, 21);
    child->actno = 48;
    sw->actno = 49;
    sw->userflag.b.h = 0;
    emycol_d_result = -1;

    swblkr4_move(block);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    TEST_ASSERT_EQ_INT(ctx, 8, block->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, child->yspeed.w);

    block->yspeed.w = 256;
    emycol_d_result = 0;
    swblkr4_move(block);
    TEST_ASSERT_EQ_INT(ctx, 256, block->yspeed.w);
}

static void test_swblkr4_move_returns_for_secondary_or_missing_switch(
    test_context *ctx) {
    sprite_status *block = &actwk[5];

    reset_state();
    block->actfree[18] = 1;
    swblkr4_move(block);
    TEST_ASSERT_EQ_INT(ctx, 0, emycol_d_count);
    TEST_ASSERT_EQ_INT(ctx, 0, emycol_u_count);

    reset_state();
    set_legacy_word(block, 28, 20);
    set_legacy_word(block, 26, 21);
    actwk[20].actno = 0;
    actwk[21].actno = 48;
    block->yspeed.w = 40;

    swblkr4_move(block);

    TEST_ASSERT_EQ_INT(ctx, 40, block->yspeed.w);
}

static void test_swblkr4_entry_frames_out_when_origin_offscreen(
    test_context *ctx) {
    sprite_status *block = &actwk[5];

    reset_state();
    block->r_no0 = 2;
    set_legacy_word(block, 29, 1024);
    scra_h_posit.w.h = 0;

    swblkr4(block);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == block);
}

TEST_MAIN_BEGIN;
    test_swblk4_patterns_capture_literal_data(&ctx);
    test_switchr4_init_pressed_by_player(&ctx);
    test_switchr4_move_clears_when_player_is_outside(&ctx);
    test_switchr4_colichk_rejects_player_too_high(&ctx);
    test_switchr4_follows_ride_actor_offset(&ctx);
    test_swblkr4_init_spawns_block_and_switch(&ctx);
    test_swblkr4_init_frames_out_on_allocation_failures(&ctx);
    test_swblkr4_child_follows_or_frames_with_parent(&ctx);
    test_swblkr4_move_collision_and_switch_speed_limits(&ctx);
    test_swblkr4_move_returns_for_secondary_or_missing_switch(&ctx);
    test_swblkr4_entry_frames_out_when_origin_offscreen(&ctx);
TEST_MAIN_END
