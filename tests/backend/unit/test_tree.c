#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_x;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int actwkchk_count;
static sprite_status *actwkchk_queue[12];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint32 frameout_s(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);

#include "src/tree.c"

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

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
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

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void set_legacy_word(sprite_status *actor, int word_index,
                            Sint16 value) {
    int offset = (word_index - 23) * 2;
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static Sint16 get_legacy_word(sprite_status *actor, int word_index) {
    int offset = (word_index - 23) * 2;
    return (Sint16)(Uint16)(actor->actfree[offset] |
                            ((Uint16)actor->actfree[offset + 1] << 8));
}

static void reset_tree_state(void) {
    memset(actwk, 0, sizeof(actwk));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_x = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
}

static void reset_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_x = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    actwkchk_count = 0;
    actwkchk_queue_index = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
}

static void assert_action_frameout_s00(test_context *ctx,
                                       sprite_status *actor,
                                       Sint16 origin_x) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, origin_x, frameout_s00_x);
}

static void test_tree_tables_and_patterns(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, hoshi_tbl[0] == hoshi_init);
    TEST_ASSERT_TRUE(ctx, hoshi_tbl[1] == hoshi_move);
    TEST_ASSERT_TRUE(ctx, k0_tbl[0] == k0_init);
    TEST_ASSERT_TRUE(ctx, kasoku_tbl[1] == kasoku_move);
    TEST_ASSERT_TRUE(ctx, hoshipat[0] == &hoshipat0);
    TEST_ASSERT_EQ_INT(ctx, -24, hoshipat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, 392, hoshipat0.spra[0].index);
    TEST_ASSERT_TRUE(ctx, kasokupat[9] == &kasokupat_j);
    TEST_ASSERT_EQ_INT(ctx, 0, kasokuchg0[0]);
    TEST_ASSERT_EQ_INT(ctx, 255, kasokuchg2[5]);
}

static void test_hoshi_init_spawns_full_layout(test_context *ctx) {
    sprite_status *star = &actwk[4];
    int i;

    reset_tree_state();
    star->xposi.w.h = 1000;
    star->yposi.w.h = 200;
    for (i = 5; i <= 10; ++i) {
        queue_actor(&actwk[i]);
    }

    hoshi(star);

    TEST_ASSERT_EQ_INT(ctx, 6, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 44, star->actno);
    TEST_ASSERT_EQ_INT(ctx, 2, star->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1064, star->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1000, get_legacy_word(star, 23));
    TEST_ASSERT_EQ_INT(ctx, 1064, get_legacy_word(star, 24));
    TEST_ASSERT_EQ_INT(ctx, 1128, actwk[5].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 936, actwk[6].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 872, actwk[7].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1000, actwk[8].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1096, actwk[9].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 904, actwk[10].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[8].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[8].patno);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[8].sprpri);
    TEST_ASSERT_EQ_INT(ctx, 12, actwk[8].sprhsize);
    TEST_ASSERT_TRUE(ctx, actwk[8].patbase == hoshipat);
    assert_action_frameout_s00(ctx, star, 1000);
}

static void test_hoshi_init_short_layout_clears_low_userflag(test_context *ctx) {
    sprite_status *star = &actwk[4];
    int i;

    reset_tree_state();
    star->xposi.w.h = 500;
    star->yposi.w.h = 90;
    star->userflag.b.h = 3;
    for (i = 5; i <= 8; ++i) {
        queue_actor(&actwk[i]);
    }

    hoshi(star);

    TEST_ASSERT_EQ_INT(ctx, 4, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 2, star->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 564, star->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 628, actwk[5].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 500, actwk[6].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 596, actwk[7].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 404, actwk[8].xposi.w.h);
}

static void test_hoshi_move_tracks_player_progress(test_context *ctx) {
    sprite_status *star = &actwk[4];
    sprite_status *player = &actwk[0];

    reset_tree_state();
    star->r_no0 = 2;
    set_legacy_word(star, 23, 1000);
    set_legacy_word(star, 24, 1128);

    hoshi(star);

    TEST_ASSERT_EQ_INT(ctx, 1128, star->xposi.w.h);
    assert_action_frameout_s00(ctx, star, 1000);

    reset_logs();
    player->actfree[2] = 2;
    player->xposi.w.h = 1120;
    hoshi(star);
    TEST_ASSERT_EQ_INT(ctx, 1176, star->xposi.w.h);

    reset_logs();
    player->xposi.w.h = 900;
    hoshi(star);
    TEST_ASSERT_EQ_INT(ctx, 1067, star->xposi.w.h);

    reset_logs();
    player->xposi.w.h = 1240;
    hoshi(star);
    TEST_ASSERT_EQ_INT(ctx, 1223, star->xposi.w.h);

    reset_logs();
    player->xposi.w.h = 1270;
    hoshi(star);
    TEST_ASSERT_EQ_INT(ctx, 1128, star->xposi.w.h);

    reset_logs();
    star->userflag.b.h = 1;
    star->xposi.w.h = 777;
    player->xposi.w.h = 1120;
    hoshi(star);
    TEST_ASSERT_EQ_INT(ctx, 777, star->xposi.w.h);
}

static void test_kasoku0_catches_left_moving_player(test_context *ctx) {
    sprite_status *bumper = &actwk[4];
    sprite_status *player = &actwk[0];

    reset_tree_state();
    bumper->xposi.w.h = 100;
    bumper->yposi.w.h = 200;
    bumper->userflag.b.l = 5;
    player->xposi.w.h = 90;
    player->yposi.w.h = 200;
    player->xspeed.w = -128;

    kasoku0(bumper);

    TEST_ASSERT_EQ_INT(ctx, 2, bumper->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, bumper->actflg);
    TEST_ASSERT_EQ_INT(ctx, 100, player->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, player->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, player->mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 55, player->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, player->actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 14, bumper->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 7, bumper->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 205, bumper->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4, player->cddat & 4);
    TEST_ASSERT_EQ_INT(ctx, 0, bumper->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == bumper);
}

static void test_k0_move_documents_guard_paths(test_context *ctx) {
    sprite_status *bumper = &actwk[4];
    sprite_status *player = &actwk[0];

    reset_tree_state();
    bumper->xposi.w.h = 100;
    bumper->yposi.w.h = 200;
    player->mstno.b.h = 43;
    k0_move(bumper, player);
    TEST_ASSERT_EQ_INT(ctx, 43, player->mstno.b.h);

    reset_tree_state();
    bumper->xposi.w.h = 100;
    bumper->yposi.w.h = 200;
    player->xposi.w.h = 90;
    player->yposi.w.h = 20;
    k0_move(bumper, player);
    TEST_ASSERT_EQ_INT(ctx, 0, player->mstno.b.h);

    reset_tree_state();
    bumper->xposi.w.h = 100;
    bumper->yposi.w.h = 200;
    player->xposi.w.h = 10;
    player->yposi.w.h = 200;
    k0_move(bumper, player);
    TEST_ASSERT_EQ_INT(ctx, 0, player->mstno.b.h);

    reset_tree_state();
    bumper->xposi.w.h = 100;
    bumper->yposi.w.h = 200;
    player->xposi.w.h = 90;
    player->yposi.w.h = 200;
    player->xspeed.w = 1;
    k0_move(bumper, player);
    TEST_ASSERT_EQ_INT(ctx, 0, player->mstno.b.h);

    reset_tree_state();
    bumper->xposi.w.h = 80;
    bumper->yposi.w.h = 200;
    player->xposi.w.h = 90;
    player->yposi.w.h = 200;
    player->xspeed.w = -1;
    k0_move(bumper, player);
    TEST_ASSERT_EQ_INT(ctx, 0, player->mstno.b.h);
}

static void test_kasoku_init_and_entry_callbacks(test_context *ctx) {
    sprite_status *booster = &actwk[4];
    sprite_status *player = &actwk[0];

    reset_tree_state();
    booster->xposi.w.h = 600;
    booster->yposi.w.h = 100;
    player->xposi.w.h = 100;

    kasoku(booster);

    TEST_ASSERT_EQ_INT(ctx, 2, booster->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, booster->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, booster->sprpri);
    TEST_ASSERT_TRUE(ctx, booster->patbase == kasokupat);
    TEST_ASSERT_EQ_INT(ctx, 1089, booster->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 32, booster->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 32, booster->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 600, get_legacy_word(booster, 23));
    assert_action_frameout_s00(ctx, booster, 600);

    reset_tree_state();
    booster->xposi.w.h = 600;
    booster->userflag.b.h = 1;
    player->xposi.w.h = 1000;

    kasoku(booster);

    TEST_ASSERT_EQ_INT(ctx, 5, booster->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, booster->cddat & 1);
}

static void test_kasoku_move_animates_and_pushes_player(test_context *ctx) {
    sprite_status *booster = &actwk[4];
    sprite_status *player = &actwk[0];

    reset_tree_state();
    booster->r_no0 = 2;
    booster->xposi.w.h = 1000;
    booster->yposi.w.h = 100;
    booster->sprhsize = 32;
    booster->sprvsize = 32;
    set_legacy_word(booster, 23, 1000);
    player->actfree[2] = 2;
    player->xposi.w.h = 1034;
    player->yposi.w.h = 100;
    player->yspeed.w = -10;

    kasoku(booster);

    TEST_ASSERT_EQ_INT(ctx, 0, booster->patno);
    TEST_ASSERT_EQ_INT(ctx, 1003, booster->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 60, booster->actfree[4]);
    TEST_ASSERT_EQ_INT(ctx, -3072, player->yspeed.w);
    assert_action_frameout_s00(ctx, booster, 1000);

    reset_logs();
    player->yspeed.w = 10;

    kasoku(booster);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == booster);
    TEST_ASSERT_TRUE(ctx, patchg_table == kasokuchg);
    TEST_ASSERT_EQ_INT(ctx, 1, booster->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 59, booster->actfree[4]);
    TEST_ASSERT_EQ_INT(ctx, 3072, player->yspeed.w);

    reset_tree_state();
    booster->r_no0 = 2;
    booster->xposi.w.h = 1000;
    booster->yposi.w.h = 100;
    booster->sprhsize = 32;
    booster->sprvsize = 32;
    booster->actfree[4] = 2;
    set_legacy_word(booster, 23, 1000);
    player->xposi.w.h = 1000;
    player->yposi.w.h = 100;

    kasoku(booster);

    TEST_ASSERT_EQ_INT(ctx, 2, booster->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, booster->actfree[4]);
}

static void test_kasoku_move_direction_and_guard_paths(test_context *ctx) {
    sprite_status *booster = &actwk[4];
    sprite_status *player = &actwk[0];

    reset_tree_state();
    booster->r_no0 = 2;
    booster->userflag.b.h = 1;
    set_legacy_word(booster, 23, 1000);
    player->actfree[2] = 2;
    player->xposi.w.h = 1120;
    player->yposi.w.h = 300;

    kasoku(booster);

    TEST_ASSERT_EQ_INT(ctx, 942, booster->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 3, booster->patno);
    TEST_ASSERT_EQ_INT(ctx, 0, player->yspeed.w);

    reset_tree_state();
    booster->r_no0 = 2;
    booster->xposi.w.h = 1000;
    booster->yposi.w.h = 100;
    booster->sprhsize = 32;
    booster->sprvsize = 32;
    set_legacy_word(booster, 23, 1000);
    player->actfree[2] = 2;
    player->xposi.w.h = 1270;
    player->yposi.w.h = 100;
    kasoku(booster);
    TEST_ASSERT_EQ_INT(ctx, 1000, booster->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, booster->patno);
    TEST_ASSERT_EQ_INT(ctx, 0, player->yspeed.w);

    reset_tree_state();
    booster->r_no0 = 2;
    booster->xposi.w.h = 1000;
    booster->yposi.w.h = 100;
    booster->sprhsize = 32;
    booster->sprvsize = 32;
    set_legacy_word(booster, 23, 1000);
    player->xposi.w.h = 900;
    player->yposi.w.h = 100;
    kasoku(booster);
    TEST_ASSERT_EQ_INT(ctx, 0, player->yspeed.w);

    reset_tree_state();
    booster->r_no0 = 2;
    booster->xposi.w.h = 1000;
    booster->yposi.w.h = 100;
    booster->sprhsize = 32;
    booster->sprvsize = 32;
    set_legacy_word(booster, 23, 1000);
    player->xposi.w.h = 1000;
    player->yposi.w.h = 60;
    kasoku(booster);
    TEST_ASSERT_EQ_INT(ctx, 0, player->yspeed.w);

    reset_tree_state();
    booster->r_no0 = 2;
    booster->xposi.w.h = 1000;
    booster->yposi.w.h = 100;
    booster->sprhsize = 32;
    booster->sprvsize = 32;
    set_legacy_word(booster, 23, 1000);
    player->xposi.w.h = 1000;
    player->yposi.w.h = 200;
    kasoku(booster);
    TEST_ASSERT_EQ_INT(ctx, 0, player->yspeed.w);

    reset_tree_state();
    booster->r_no0 = 2;
    booster->xposi.w.h = 1000;
    booster->yposi.w.h = 100;
    booster->sprhsize = 32;
    booster->sprvsize = 32;
    set_legacy_word(booster, 23, 1000);
    player->xposi.w.h = 1000;
    player->yposi.w.h = 100;
    player->mstno.b.h = 43;
    kasoku(booster);
    TEST_ASSERT_EQ_INT(ctx, 0, player->yspeed.w);

    reset_tree_state();
    booster->userflag.b.l = 1;
    kasoku(booster);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

TEST_MAIN_BEGIN;
test_tree_tables_and_patterns(&ctx);
test_hoshi_init_spawns_full_layout(&ctx);
test_hoshi_init_short_layout_clears_low_userflag(&ctx);
test_hoshi_move_tracks_player_progress(&ctx);
test_kasoku0_catches_left_moving_player(&ctx);
test_k0_move_documents_guard_paths(&ctx);
test_kasoku_init_and_entry_callbacks(&ctx);
test_kasoku_move_animates_and_pushes_player(&ctx);
test_kasoku_move_direction_and_guard_paths(&ctx);
TEST_MAIN_END
