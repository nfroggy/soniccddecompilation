#include <stddef.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
short_union stageno;
Uint8 switchflag[32];
int_union scra_h_posit;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int actwkchk_count;
static sprite_status *actwkchk_queue[12];
static int actwkchk_queue_count;
static int actwkchk_queue_pos;
static int frameout_count;
static sprite_status *frameout_actor;
static int ridechk_count;
static sprite_status *ridechk_actor;
static sprite_status *ridechk_player;
static Sint16 ridechk_result;
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 sinset_sin;
static Sint16 sinset_cos;

void actionsub(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void frameout(sprite_status *pActwk);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);

#include "src/r4/renketu4.c"

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

Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ridechk_count;
    ridechk_actor = pActwk;
    ridechk_player = pPlayerwk;
    return ridechk_result;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    ++sinset_count;
    sinset_angle = kakudo;
    *sin = sinset_sin;
    *cos = sinset_cos;
}

static void reset_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(&stageno, 0, sizeof(stageno));
    memset(switchflag, 0, sizeof(switchflag));
    scra_h_posit.l = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_pos = 0;
    frameout_count = 0;
    frameout_actor = 0;
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
    ridechk_result = 0;
    sinset_count = 0;
    sinset_angle = 0;
    sinset_sin = 16;
    sinset_cos = 32;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void queue_children(int first_index, int count) {
    int i;

    for (i = 0; i < count; ++i) {
        queue_actor(&actwk[first_index + i]);
    }
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

static void test_renketu4_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, renketu4pat[0] == &renketu4_pat0);
    TEST_ASSERT_TRUE(ctx, renketu4pat[1] == &renketu4_pat1);
    TEST_ASSERT_EQ_INT(ctx, 1, renketu4_pat0.cnt);
    TEST_ASSERT_EQ_INT(ctx, -32, renketu4_pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -8, renketu4_pat0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_RENKETU4_BASE, renketu4_pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -8, renketu4_pat1.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_RENKETU4_BASE + 1, renketu4_pat1.spra[0].index);
}

static void test_renketu4_init_spawns_linked_children_and_moves(
    test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    queue_children(20, 8);
    platform->actno = 62;
    platform->xposi.w.h = 100;
    platform->yposi.w.h = 200;
    platform->userflag.b.h = 0;

    renketu4(platform);

    TEST_ASSERT_EQ_INT(ctx, 2, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, platform->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 17216, platform->sproffset);
    TEST_ASSERT_TRUE(ctx, platform->patbase == renketu4pat);
    TEST_ASSERT_EQ_INT(ctx, 32, platform->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 8, platform->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 100, get_legacy_word(platform, 29));
    TEST_ASSERT_EQ_INT(ctx, 200, get_legacy_word(platform, 27));
    TEST_ASSERT_EQ_INT(ctx, 33, platform->actfree[21]);
    TEST_ASSERT_EQ_INT(ctx, 0, platform->patno);
    TEST_ASSERT_EQ_INT(ctx, 1, platform->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 256, get_legacy_word(platform, 26));
    TEST_ASSERT_EQ_INT(ctx, 8448, get_legacy_word(platform, 33));
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 33, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 112, platform->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 206, platform->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actor == platform);
    TEST_ASSERT_TRUE(ctx, ridechk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == platform);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    TEST_ASSERT_EQ_INT(ctx, 8, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 62, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[20].actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 100, get_legacy_word(&actwk[20], 29));
    TEST_ASSERT_EQ_INT(ctx, 200, get_legacy_word(&actwk[20], 27));
    TEST_ASSERT_EQ_INT(ctx, 62, actwk[27].actno);
    TEST_ASSERT_EQ_INT(ctx, 8, actwk[27].actfree[18]);
}

static void test_renketu4_init_uses_stage_two_offset_and_piece_sprite(
    test_context *ctx) {
    sprite_status *piece = &actwk[5];

    reset_state();
    stageno.b.l = 2;
    piece->actfree[18] = 3;
    piece->xposi.w.h = 100;
    piece->yposi.w.h = 200;
    piece->userflag.b.h = 16;
    set_legacy_word(piece, 29, 100);
    set_legacy_word(piece, 27, 200);

    renketu4(piece);

    TEST_ASSERT_EQ_INT(ctx, 2, piece->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1024, piece->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 23, piece->actfree[21]);
    TEST_ASSERT_EQ_INT(ctx, 1, piece->patno);
    TEST_ASSERT_EQ_INT(ctx, 3, piece->sprpri);
    TEST_ASSERT_EQ_INT(ctx, -256, get_legacy_word(piece, 26));
    TEST_ASSERT_EQ_INT(ctx, 5888, get_legacy_word(piece, 33));
    TEST_ASSERT_EQ_INT(ctx, 0, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_renketu4_move_switch_flips_direction_once(test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->r_no0 = 2;
    platform->userflag.b.h = 3;
    platform->actfree[21] = 32;
    set_legacy_word(platform, 26, 256);
    set_legacy_word(platform, 27, 200);
    set_legacy_word(platform, 29, 100);
    switchflag[3] = 128;

    renketu4(platform);

    TEST_ASSERT_EQ_INT(ctx, -256, get_legacy_word(platform, 26));
    TEST_ASSERT_EQ_INT(ctx, 7936, get_legacy_word(platform, 33));
    TEST_ASSERT_EQ_INT(ctx, 1, platform->actfree[19]);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 31, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    sinset_count = 0;
    actionsub_count = 0;
    renketu4(platform);

    TEST_ASSERT_EQ_INT(ctx, -256, get_legacy_word(platform, 26));
    TEST_ASSERT_EQ_INT(ctx, 7680, get_legacy_word(platform, 33));
    TEST_ASSERT_EQ_INT(ctx, 1, platform->actfree[19]);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_renketu4_move_switch_release_rearms_flip(test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->r_no0 = 2;
    platform->userflag.b.h = 3;
    platform->actfree[19] = 1;
    set_legacy_word(platform, 26, -256);
    set_legacy_word(platform, 27, 200);
    set_legacy_word(platform, 29, 100);
    switchflag[3] = 0;

    renketu4(platform);

    TEST_ASSERT_EQ_INT(ctx, 0, platform->actfree[19]);
    TEST_ASSERT_EQ_INT(ctx, -256, get_legacy_word(platform, 26));
}

static void test_renketu4_move_high_userflag_ignores_switch(test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->r_no0 = 2;
    platform->userflag.b.h = (Sint8)131;
    platform->actfree[19] = 7;
    set_legacy_word(platform, 26, 256);
    set_legacy_word(platform, 27, 200);
    set_legacy_word(platform, 29, 100);
    switchflag[3] = 128;

    renketu4(platform);

    TEST_ASSERT_EQ_INT(ctx, 7, platform->actfree[19]);
    TEST_ASSERT_EQ_INT(ctx, 256, get_legacy_word(platform, 26));
    TEST_ASSERT_EQ_INT(ctx, 256, get_legacy_word(platform, 33));
}

static void test_renketu4_ridechk_places_player_on_platform(test_context *ctx) {
    sprite_status *platform = &actwk[5];
    sprite_status *player = &actwk[0];

    reset_state();
    platform->yposi.w.h = 300;
    platform->sprvsize = 8;
    player->sprvsize = 16;
    player->yposi.w.h = 999;
    ridechk_result = 1;

    renketu4_ridechk(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actor == platform);
    TEST_ASSERT_TRUE(ctx, ridechk_player == player);
    TEST_ASSERT_EQ_INT(ctx, 278, player->yposi.w.h);

    ridechk_result = 0;
    ridechk_count = 0;
    player->yposi.w.h = 999;

    renketu4_ridechk(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 999, player->yposi.w.h);
}

static void test_renketu4_entry_frames_out_when_offscreen(test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->r_no0 = 2;
    set_legacy_word(platform, 26, 256);
    set_legacy_word(platform, 27, 200);
    set_legacy_word(platform, 29, 1024);
    scra_h_posit.w.h = 0;

    renketu4(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == platform);
}

TEST_MAIN_BEGIN;
    test_renketu4_patterns_capture_literal_data(&ctx);
    test_renketu4_init_spawns_linked_children_and_moves(&ctx);
    test_renketu4_init_uses_stage_two_offset_and_piece_sprite(&ctx);
    test_renketu4_move_switch_flips_direction_once(&ctx);
    test_renketu4_move_switch_release_rearms_flip(&ctx);
    test_renketu4_move_high_userflag_ignores_switch(&ctx);
    test_renketu4_ridechk_places_player_on_platform(&ctx);
    test_renketu4_entry_frames_out_when_offscreen(&ctx);
TEST_MAIN_END
