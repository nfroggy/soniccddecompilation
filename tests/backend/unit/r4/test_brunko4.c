#include <stddef.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
int_union scra_h_posit;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_pos;
static int actwkchk2_count;
static sprite_status *actwkchk2_source;
static sprite_status *actwkchk2_queue[2];
static int actwkchk2_queue_count;
static int actwkchk2_queue_pos;
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
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);
void frameout(sprite_status *pActwk);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);

#include "src/r4/brunko4.c"

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
    scra_h_posit.l = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_pos = 0;
    actwkchk2_count = 0;
    actwkchk2_source = 0;
    memset(actwkchk2_queue, 0, sizeof(actwkchk2_queue));
    actwkchk2_queue_count = 0;
    actwkchk2_queue_pos = 0;
    frameout_count = 0;
    frameout_actor = 0;
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
    ridechk_result = 0;
    sinset_count = 0;
    sinset_angle = 0;
    sinset_sin = 160;
    sinset_cos = 320;
}

static void queue_actwkchk(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
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

static void test_brunko4_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, brunko4pat[0] == &pat0);
    TEST_ASSERT_TRUE(ctx, brunko4pat[1] == &pat1);
    TEST_ASSERT_TRUE(ctx, brunko4pat[2] == &pat2);
    TEST_ASSERT_EQ_INT(ctx, 1, pat0.cnt);
    TEST_ASSERT_EQ_INT(ctx, -8, pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -8, pat0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 470, pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -32, pat1.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, 471, pat1.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 472, pat2.spra[0].index);
}

static void test_brunko4_init_spawns_chain_links(test_context *ctx) {
    sprite_status *master = &actwk[5];
    int i;

    reset_state();
    master->xposi.w.h = 100;
    master->yposi.w.h = 200;
    master->userflag.b.h = 7;
    for (i = 0; i < 6; ++i) {
        queue_actwkchk(&actwk[20 + i]);
    }

    brunko4(master);

    TEST_ASSERT_EQ_INT(ctx, 2, master->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, master->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 4, master->sprpri);
    TEST_ASSERT_TRUE(ctx, master->patbase == brunko4pat);
    TEST_ASSERT_EQ_INT(ctx, 832, master->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 32, master->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 10, master->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 191, master->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 128, master->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 100, get_legacy_word(master, 29));
    TEST_ASSERT_EQ_INT(ctx, 200, get_legacy_word(master, 27));
    TEST_ASSERT_EQ_INT(ctx, 100, master->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, master->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 6, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 191, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == master);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    TEST_ASSERT_EQ_INT(ctx, 41, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[20].actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[20].patno);
    TEST_ASSERT_EQ_INT(ctx, 100, get_legacy_word(&actwk[20], 29));
    TEST_ASSERT_EQ_INT(ctx, 200, get_legacy_word(&actwk[20], 27));
    TEST_ASSERT_EQ_INT(ctx, 7, actwk[20].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 41, actwk[25].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[25].actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[25].patno);
}

static void test_brunko4_init_patno_one_uses_high_sprite_page(
    test_context *ctx) {
    sprite_status *master = &actwk[5];

    reset_state();
    master->patno = 1;
    master->xposi.w.h = 100;
    master->yposi.w.h = 200;

    brunko_init(master);

    TEST_ASSERT_EQ_INT(ctx, 17216, master->sproffset);
}

static void test_brunko4_init_platform_spawns_helper(test_context *ctx) {
    sprite_status *platform = &actwk[20];
    sprite_status *helper = &actwk[30];

    reset_state();
    platform->xposi.w.h = 100;
    platform->yposi.w.h = 200;
    platform->actfree[18] = 6;
    platform->userflag.b.h = 0x37;
    set_legacy_word(platform, 29, 100);
    set_legacy_word(platform, 27, 200);
    queue_actwkchk2(helper);

    brunko_init(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_TRUE(ctx, actwkchk2_source == platform);
    TEST_ASSERT_EQ_INT(ctx, 39, helper->actno);
    TEST_ASSERT_EQ_INT(ctx, 20, get_legacy_word(helper, 28));
    TEST_ASSERT_EQ_INT(ctx, 240, helper->actfree[15]);
    TEST_ASSERT_EQ_INT(ctx, 16, helper->actfree[14]);
    TEST_ASSERT_EQ_INT(ctx, 7, helper->userflag.b.h);
}

static void test_brunko4_platform_move_sets_speed_and_player_y(
    test_context *ctx) {
    sprite_status *platform = &actwk[20];
    sprite_status *player = &actwk[0];

    reset_state();
    platform->r_no0 = 2;
    platform->xposi.w.h = 100;
    platform->yposi.w.h = 200;
    platform->sprvsize = 10;
    platform->actfree[16] = 192;
    platform->actfree[18] = 6;
    set_legacy_word(platform, 29, 100);
    set_legacy_word(platform, 27, 200);
    player->sprvsize = 16;
    player->yposi.w.h = 999;
    ridechk_result = 1;

    brunko4(platform);

    TEST_ASSERT_EQ_INT(ctx, 220, platform->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 260, platform->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 30720, platform->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 15360, platform->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 236, player->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actor == platform);
    TEST_ASSERT_TRUE(ctx, ridechk_player == player);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_brunko4_platform_move_without_ride_leaves_player_y(
    test_context *ctx) {
    sprite_status *platform = &actwk[20];
    sprite_status *player = &actwk[0];

    reset_state();
    platform->r_no0 = 2;
    platform->xposi.w.h = 100;
    platform->yposi.w.h = 200;
    platform->sprvsize = 10;
    platform->actfree[16] = 192;
    platform->actfree[18] = 6;
    set_legacy_word(platform, 29, 100);
    set_legacy_word(platform, 27, 200);
    player->sprvsize = 16;
    player->yposi.w.h = 999;

    brunko4(platform);

    TEST_ASSERT_EQ_INT(ctx, 999, player->yposi.w.h);
}

static void test_brunko4_userflag_16_advances_angle_forward(test_context *ctx) {
    sprite_status *link = &actwk[20];

    reset_state();
    link->actfree[16] = 192;
    link->actfree[18] = 1;
    link->userflag.b.h = 16;
    set_legacy_word(link, 29, 100);
    set_legacy_word(link, 27, 200);

    brunko4_posiset(link);

    TEST_ASSERT_EQ_INT(ctx, 193, link->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 0, link->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 193, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 120, link->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 210, link->yposi.w.h);
}

static void test_brunko4_entry_frames_out_when_origin_offscreen(
    test_context *ctx) {
    sprite_status *link = &actwk[20];

    reset_state();
    link->r_no0 = 2;
    link->actfree[18] = 1;
    link->actfree[16] = 192;
    set_legacy_word(link, 29, 1024);
    set_legacy_word(link, 27, 200);
    scra_h_posit.w.h = 128;

    brunko4(link);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == link);
}

TEST_MAIN_BEGIN;
    test_brunko4_patterns_capture_literal_data(&ctx);
    test_brunko4_init_spawns_chain_links(&ctx);
    test_brunko4_init_patno_one_uses_high_sprite_page(&ctx);
    test_brunko4_init_platform_spawns_helper(&ctx);
    test_brunko4_platform_move_sets_speed_and_player_y(&ctx);
    test_brunko4_platform_move_without_ride_leaves_player_y(&ctx);
    test_brunko4_userflag_16_advances_angle_forward(&ctx);
    test_brunko4_entry_frames_out_when_origin_offscreen(&ctx);
TEST_MAIN_END
