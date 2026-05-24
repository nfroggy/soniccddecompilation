#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
short_union stageno;
int_union scra_v_posit;
Uint8 time_flag;
Uint8 colrevflag;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_x;
static int ridechk_count;
static sprite_status *ridechk_actor;
static sprite_status *ridechk_player;
static int actwkchk2_count;
static sprite_status *actwkchk2_queue[8];
static int actwkchk2_queue_count;
static int actwkchk2_queue_index;
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 sinset_sin;
static Sint16 sinset_cos;
static int speedset2_count;
static sprite_status *speedset2_actor;
static int emycol_u_count;
static Sint16 emycol_u_result;
static int ride_on_clr_count;
static sprite_status *ride_on_clr_actor;
static sprite_status *ride_on_clr_player;
static int frameout_count;
static sprite_status *frameout_actor;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);
void speedset2(sprite_status *pActwk);
Sint16 emycol_u(sprite_status *pActwk);
Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk);
void frameout(sprite_status *pActwk);

#include "src/r5/dai_rd5.c"

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

Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ridechk_count;
    ridechk_actor = pActwk;
    ridechk_player = pPlayerwk;
    return 0;
}

Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk) {
    (void)pActwk;
    ++actwkchk2_count;
    if (actwkchk2_queue_index >= actwkchk2_queue_count) {
        *ppNewActwk = 0;
        return -1;
    }
    *ppNewActwk = actwkchk2_queue[actwkchk2_queue_index++];
    return 0;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    ++sinset_count;
    sinset_angle = kakudo;
    *sin = sinset_sin;
    *cos = sinset_cos;
}

void speedset2(sprite_status *pActwk) {
    ++speedset2_count;
    speedset2_actor = pActwk;
}

Sint16 emycol_u(sprite_status *pActwk) {
    ++emycol_u_count;
    (void)pActwk;
    return emycol_u_result;
}

Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_clr_count;
    ride_on_clr_actor = pActwk;
    ride_on_clr_player = pPlayerwk;
    return 0;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

static void reset_dair5_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(&stageno, 0, sizeof(stageno));
    memset(&scra_v_posit, 0, sizeof(scra_v_posit));
    time_flag = 0;
    colrevflag = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_x = 0;
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
    actwkchk2_count = 0;
    memset(actwkchk2_queue, 0, sizeof(actwkchk2_queue));
    actwkchk2_queue_count = 0;
    actwkchk2_queue_index = 0;
    sinset_count = 0;
    sinset_angle = 0;
    sinset_sin = 0;
    sinset_cos = 0;
    speedset2_count = 0;
    speedset2_actor = 0;
    emycol_u_count = 0;
    emycol_u_result = 0;
    ride_on_clr_count = 0;
    ride_on_clr_actor = 0;
    ride_on_clr_player = 0;
    frameout_count = 0;
    frameout_actor = 0;
}

static void reset_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_x = 0;
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
    actwkchk2_count = 0;
    actwkchk2_queue_count = 0;
    actwkchk2_queue_index = 0;
    sinset_count = 0;
    speedset2_count = 0;
    speedset2_actor = 0;
    emycol_u_count = 0;
    ride_on_clr_count = 0;
    frameout_count = 0;
}

static void queue_actor(sprite_status *actor) {
    actwkchk2_queue[actwkchk2_queue_count++] = actor;
}

static void set_actfree_word(sprite_status *actor, int offset, Sint16 value) {
    Uint16 bits = (Uint16)value;
    actor->actfree[offset] = (Uint8)(bits & 255);
    actor->actfree[offset + 1] = (Uint8)(bits >> 8);
}

static Sint16 get_actfree_word(sprite_status *actor, int offset) {
    return (Sint16)((Uint16)actor->actfree[offset] |
                    ((Uint16)actor->actfree[offset + 1] << 8));
}

static void assert_tail_callbacks(test_context *ctx, sprite_status *actor,
                                  Sint16 origin_x) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, origin_x, frameout_s00_x);
}

static void test_tables_capture_pattern_contract(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, dair5pat[0] == &pat00);
    TEST_ASSERT_TRUE(ctx, dair5pat[5] == &pat05);
    TEST_ASSERT_EQ_INT(ctx, 1, pat00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -16, pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, pat00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 468, pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 2, pat05.cnt);
    TEST_ASSERT_EQ_INT(ctx, 475, pat05.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 476, pat05.spra[1].index);
}

static void test_init_sets_shape_and_spawns_springs(test_context *ctx) {
    sprite_status *platform = &actwk[8];

    reset_dair5_state();
    platform->xposi.w.h = 100;
    platform->yposi.w.h = 200;
    platform->userflag.b.h = 14;
    queue_actor(&actwk[20]);
    queue_actor(&actwk[21]);

    dair5(platform);

    TEST_ASSERT_EQ_INT(ctx, 2, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, platform->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, platform->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 17156, platform->sproffset);
    TEST_ASSERT_TRUE(ctx, platform->patbase == dair5pat);
    TEST_ASSERT_EQ_INT(ctx, 16, platform->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 2, platform->patno);
    TEST_ASSERT_EQ_INT(ctx, 32, platform->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 100, actwk[20].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, actwk[20].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[20].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[21].actno);
    assert_tail_callbacks(ctx, platform, 100);

    reset_dair5_state();
    stageno.b.l = 2;
    platform->xposi.w.h = 64;
    platform->yposi.w.h = 80;
    platform->userflag.b.h = 22;
    queue_actor(&actwk[21]);
    queue_actor(&actwk[22]);
    queue_actor(&actwk[23]);

    dair5(platform);

    TEST_ASSERT_EQ_INT(ctx, 17272, platform->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 3, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[21].actno);
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[22].actno);
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[23].actno);
}

static void test_move_types_update_visible_motion(test_context *ctx) {
    sprite_status *platform = &actwk[8];

    reset_dair5_state();
    type01(platform);
    TEST_ASSERT_EQ_INT(ctx, 128, platform->actfree[21]);

    reset_dair5_state();
    platform->r_no0 = 2;
    platform->userflag.b.h = 1;
    platform->xposi.l = 100 << 16;
    platform->yposi.l = 200 << 16;
    set_actfree_word(platform, 6, 4);
    set_actfree_word(platform, 8, 4);

    dair5(platform);

    TEST_ASSERT_EQ_INT(ctx, 4, platform->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, (100 << 16) + (4 << 8), platform->xposi.l);
    TEST_ASSERT_EQ_INT(ctx, 128, platform->actfree[21]);
    assert_tail_callbacks(ctx, platform, 0);

    reset_dair5_state();
    platform->r_no0 = 2;
    platform->userflag.b.h = 129;
    platform->xposi.l = 100 << 16;

    dair5(platform);

    TEST_ASSERT_EQ_INT(ctx, -4, platform->xspeed.w);

    reset_dair5_state();
    platform->r_no0 = 2;
    platform->userflag.b.h = 131;
    platform->xposi.l = 100 << 16;
    platform->yposi.l = 200 << 16;
    set_actfree_word(platform, 6, 4);
    set_actfree_word(platform, 8, 4);

    dair5(platform);

    TEST_ASSERT_EQ_INT(ctx, 0, platform->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, (200 << 16) - (4 << 8), platform->yposi.l);

    reset_dair5_state();
    platform->r_no0 = 2;
    platform->userflag.b.h = 8;
    platform->xposi.l = 100 << 16;
    set_actfree_word(platform, 6, 4);
    set_actfree_word(platform, 8, 4);

    dair5(platform);

    TEST_ASSERT_EQ_INT(ctx, 4, platform->xspeed.w);

    reset_dair5_state();
    platform->r_no0 = 2;
    platform->userflag.b.h = 136;
    platform->xposi.l = 100 << 16;

    dair5(platform);

    TEST_ASSERT_EQ_INT(ctx, -4, platform->xspeed.w);
}

static void test_drop_type_arms_waits_accelerates_and_frames_out(
    test_context *ctx) {
    sprite_status *platform = &actwk[8];

    reset_dair5_state();
    platform->r_no0 = 2;
    platform->userflag.b.h = 6;
    platform->cddat = 8;

    dair5(platform);

    TEST_ASSERT_EQ_INT(ctx, 30, platform->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 1, platform->actfree[19]);
    TEST_ASSERT_EQ_INT(ctx, 128, platform->actfree[21]);

    reset_dair5_state();
    platform->r_no0 = 2;
    platform->userflag.b.h = 6;
    platform->actfree[19] = 1;
    platform->actfree[17] = 2;
    platform->actfree[21] = 128;

    dair5(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, platform->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 128, platform->actfree[21]);

    reset_dair5_state();
    platform->r_no0 = 2;
    platform->userflag.b.h = 6;
    platform->actfree[19] = 1;
    platform->actfree[17] = 1;
    platform->yspeed.w = 512;
    platform->yposi.l = 100 << 16;
    scra_v_posit.w.h = 50;

    dair5(platform);

    TEST_ASSERT_EQ_INT(ctx, 0, platform->actfree[21]);
    TEST_ASSERT_EQ_INT(ctx, 18, platform->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 544, platform->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, (100 << 16) + (544 << 8), platform->yposi.l);

    reset_dair5_state();
    platform->r_no0 = 2;
    platform->userflag.b.h = 6;
    platform->actfree[19] = 1;
    platform->yspeed.w = 1024;
    platform->yposi.w.h = 400;
    scra_v_posit.w.h = 100;

    dair5(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == platform);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == platform);
}

static void test_rising_type_state_machine(test_context *ctx) {
    sprite_status *platform = &actwk[8];

    reset_dair5_state();
    platform->r_no0 = 2;
    platform->userflag.b.h = 5;
    platform->cddat = 8;

    dair5(platform);

    TEST_ASSERT_EQ_INT(ctx, 2, platform->actfree[19]);
    TEST_ASSERT_EQ_INT(ctx, 29, platform->actfree[17]);

    reset_dair5_state();
    platform->r_no0 = 2;
    platform->userflag.b.h = 5;
    platform->actfree[19] = 2;
    platform->actfree[17] = 1;
    platform->yspeed.w = -800;
    platform->yposi.l = 100 << 16;
    emycol_u_result = -4;

    dair5(platform);

    TEST_ASSERT_EQ_INT(ctx, 0, platform->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 16, platform->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 0, platform->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 4, platform->actfree[19]);
    TEST_ASSERT_EQ_INT(ctx, 1, speedset2_count);
    TEST_ASSERT_TRUE(ctx, speedset2_actor == platform);
    TEST_ASSERT_EQ_INT(ctx, 1, emycol_u_count);

    reset_dair5_state();
    platform->r_no0 = 2;
    platform->userflag.b.h = 5;
    platform->actfree[19] = 4;

    dair5(platform);

    TEST_ASSERT_EQ_INT(ctx, 4, platform->actfree[19]);
}

static void test_belt_task_moves_riding_player_by_time_and_direction(
    test_context *ctx) {
    sprite_status *platform = &actwk[8];
    sprite_status *player = &actwk[0];

    reset_dair5_state();
    platform->actfree[21] = 1;
    platform->cddat = 8;
    player->actno = 1;
    player->cddat = 8;
    player->actfree[19] = 8;
    player->xposi.l = 100 << 16;

    belttask(platform);

    TEST_ASSERT_EQ_INT(ctx, (100 << 16) + 32768, player->xposi.l);

    reset_dair5_state();
    platform->actfree[21] = 1;
    platform->cddat = 8;
    player->actno = 1;
    player->cddat = 8;
    player->actfree[19] = 8;
    player->xposi.l = 100 << 16;
    time_flag = 2;
    colrevflag = 1;

    belttask(platform);

    TEST_ASSERT_EQ_INT(ctx, (100 << 16) - 0x20000, player->xposi.l);
}

static void test_init_single_spring_dodai_and_wave_helpers(test_context *ctx) {
    sprite_status *platform = &actwk[8];

    reset_dair5_state();
    platform->xposi.w.h = 30;
    platform->yposi.w.h = 40;
    platform->userflag.b.h = 2;
    queue_actor(&actwk[24]);

    dair5(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[24].actno);
    TEST_ASSERT_EQ_INT(ctx, 30, actwk[24].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 40, actwk[24].yposi.w.h);

    reset_dair5_state();
    platform->actfree[3] = 48;
    dodai_sub(platform, 48);
    TEST_ASSERT_EQ_INT(ctx, 255, platform->actfree[18]);

    platform->actfree[3] = 47;
    dodai_sub(platform, 48);
    TEST_ASSERT_EQ_INT(ctx, 0, platform->actfree[18]);

    reset_dair5_state();
    platform->actfree[21] = 128;
    platform->actfree[20] = 8;
    set_actfree_word(platform, 10, 200);
    sinset_sin = 64;
    dai5sub(platform);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 0, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 1, platform->yposi.w.h);

    reset_dair5_state();
    platform->actfree[21] = 128;
    platform->cddat = 8;
    platform->actfree[20] = 64;
    dai5sub(platform);
    TEST_ASSERT_EQ_INT(ctx, 0, sinset_count);
}

static void test_belt_task_early_returns(test_context *ctx) {
    sprite_status *platform = &actwk[8];
    sprite_status *player = &actwk[0];

    reset_dair5_state();
    player->xposi.l = 100 << 16;
    belttask(platform);
    TEST_ASSERT_EQ_INT(ctx, 100 << 16, player->xposi.l);

    platform->actfree[21] = 1;
    belttask(platform);
    TEST_ASSERT_EQ_INT(ctx, 100 << 16, player->xposi.l);

    platform->cddat = 8;
    belttask(platform);
    TEST_ASSERT_EQ_INT(ctx, 100 << 16, player->xposi.l);

    player->actno = 1;
    belttask(platform);
    TEST_ASSERT_EQ_INT(ctx, 100 << 16, player->xposi.l);

    player->cddat = 8;
    player->actfree[19] = 7;
    belttask(platform);
    TEST_ASSERT_EQ_INT(ctx, 100 << 16, player->xposi.l);
}

TEST_MAIN_BEGIN;
    test_tables_capture_pattern_contract(&ctx);
    test_init_sets_shape_and_spawns_springs(&ctx);
    test_move_types_update_visible_motion(&ctx);
    test_drop_type_arms_waits_accelerates_and_frames_out(&ctx);
    test_rising_type_state_machine(&ctx);
    test_belt_task_moves_riding_player_by_time_and_direction(&ctx);
    test_init_single_spring_dodai_and_wave_helpers(&ctx);
    test_belt_task_early_returns(&ctx);
TEST_MAIN_END
