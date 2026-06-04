#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 flagwork[766];
Uint8 time_flag;
char time_item;

static sprite_pattern dummy_barrier = {1, {{0, 0, 0, 0}}};
sprite_pattern *bariapat[1] = {&dummy_barrier};

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int soundset_count;
static Sint16 soundset_requests[8];

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void soundset(Sint16 ReqNo);

#include "src/r1/shoot1.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 8) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

static void reset_shoot1_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(flagwork, 0, sizeof(flagwork));
    time_flag = 0;
    time_item = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void test_shoot1_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, edapat[0] == &eda00);
    TEST_ASSERT_TRUE(ctx, edapat[2] == &eda02);
    TEST_ASSERT_TRUE(ctx, kagepat[0] == &kag00);
    TEST_ASSERT_TRUE(ctx, kage1pat[0] == &ana00);
    TEST_ASSERT_TRUE(ctx, shooterpositbl[0] == shooterposi_0);
    TEST_ASSERT_TRUE(ctx, shooterpositbl[2] == shooterposi_2);
    TEST_ASSERT_EQ_INT(ctx, 288, eda00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 293, ana00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 136, shooterposi_0[0]);
    TEST_ASSERT_EQ_INT(ctx, 68, shooterposi_1[0]);
}

static void test_eda_and_kage_initialize_and_move(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_shoot1_state();
    actor->actno = 17;
    actor->userflag.b.h = 2;
    eda(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_TRUE(ctx, actor->patbase == edapat);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprvsize);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);

    reset_shoot1_state();
    actor->r_no0 = 2;
    actor->actno = 18;
    eda(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_shoot1_state();
    actor->r_no0 = 8;
    eda(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_shoot1_state();
    actor->actno = 19;
    actor->userflag.b.h = 1;
    kage(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 6, actor->sprpri);
    TEST_ASSERT_TRUE(ctx, actor->patbase == kagepat);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->patno);

    reset_shoot1_state();
    actor->r_no0 = 2;
    kage(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_shoot1_state();
    actor->r_no0 = 8;
    kage(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_ana_flag_paths_and_player_trigger(test_context *ctx) {
    sprite_status *hole = &actwk[3];
    sprite_status *player = &actwk[0];

    reset_shoot1_state();
    hole->cdsts = 2;
    time_flag = 1;
    ana(hole);
    TEST_ASSERT_EQ_INT(ctx, 2, hole->r_no0);
    TEST_ASSERT_TRUE(ctx, hole->patbase == kage1pat);
    TEST_ASSERT_EQ_INT(ctx, 4, hole->actflg);
    TEST_ASSERT_EQ_INT(ctx, 6, hole->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 0, flagwork[7] & 64);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == hole);

    reset_shoot1_state();
    hole->cdsts = 2;
    time_flag = 1;
    flagwork[7] = 64;
    ana(hole);
    TEST_ASSERT_EQ_INT(ctx, 4, hole->r_no0);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == hole);

    reset_shoot1_state();
    hole->r_no0 = 2;
    hole->xposi.w.h = 100;
    hole->yposi.w.h = 120;
    hole->cdsts = 3;
    time_flag = 2;
    player_work_get(player)->status_flags = 1;
    player->xposi.w.h = 100;
    player->yposi.w.h = 120;
    ana(hole);
    TEST_ASSERT_EQ_INT(ctx, 4, hole->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 64, flagwork[11] & 64);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 163, soundset_requests[0]);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == hole);

    reset_shoot1_state();
    hole->r_no0 = 2;
    hole->xposi.w.h = 100;
    hole->yposi.w.h = 120;
    player_work_get(player)->status_flags = 1;
    player->xposi.w.h = 10;
    player->yposi.w.h = 120;
    ana(hole);
    TEST_ASSERT_EQ_INT(ctx, 2, hole->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_shoot1_state();
    hole->r_no0 = 2;
    hole->xposi.w.h = 100;
    hole->yposi.w.h = 120;
    player_work_get(player)->status_flags = 1;
    player->xposi.w.h = 100;
    player->yposi.w.h = 40;
    ana(hole);
    TEST_ASSERT_EQ_INT(ctx, 2, hole->r_no0);

    reset_shoot1_state();
    hole->r_no0 = 4;
    ana(hole);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == hole);

    reset_shoot1_state();
    hole->r_no0 = 8;
    ana(hole);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_kage1_flag_helpers_set_and_clear_selected_bit(
    test_context *ctx) {
    sprite_status *hole = &actwk[3];

    reset_shoot1_state();
    hole->cdsts = 4;
    time_flag = 2;
    flagwork[14] = 255;
    TEST_ASSERT_EQ_INT(ctx, 64, kage1_chk(hole));
    TEST_ASSERT_EQ_INT(ctx, 255, flagwork[14]);
    kage1_clr(hole);
    TEST_ASSERT_EQ_INT(ctx, 191, flagwork[14]);
}

static void test_shooter_wrapper_time_flag_and_gate_paths(test_context *ctx) {
    sprite_status *shoot = &actwk[3];
    sprite_status *player = &actwk[0];

    reset_shoot1_state();
    shoot->cdsts = 4;
    time_flag = 128 | 2;
    time_item = 0;
    flagwork[14] = 255;
    player->mstno.b.h = 43;
    shooter(shoot);
    TEST_ASSERT_EQ_INT(ctx, 127, flagwork[14]);
    TEST_ASSERT_EQ_INT(ctx, 0, shoot->r_no0);

    reset_shoot1_state();
    shoot->cdsts = 4;
    time_flag = 128 | 5;
    time_item = 0;
    flagwork[14] = 255;
    player->mstno.b.h = 43;
    shooter(shoot);
    TEST_ASSERT_EQ_INT(ctx, 127, flagwork[14]);

    reset_shoot1_state();
    shoot->cdsts = 0;
    time_flag = 128 | 1;
    flagwork[0] = 255;
    player->r_no0 = 6;
    shooter(shoot);
    TEST_ASSERT_EQ_INT(ctx, 255, flagwork[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, shoot->r_no0);

    reset_shoot1_state();
    shoot->cdsts = 4;
    time_flag = 128 | 0;
    time_item = 3;
    flagwork[12] = 255;
    shooter(shoot);
    TEST_ASSERT_EQ_INT(ctx, 127, flagwork[12]);
}

static void test_shooter_init_sets_actor_and_first_route_target(
    test_context *ctx) {
    sprite_status *shoot = &actwk[3];
    shooter_work *work;

    reset_shoot1_state();
    shoot->userflag.b.h = 0;
    shoot->xposi.w.h = 10;
    shoot->yposi.w.h = 20;
    actwk[0].xposi.w.h = 500;
    actwk[0].yposi.w.h = 500;
    shooterinit(shoot);
    work = shooter_get_work(shoot);
    TEST_ASSERT_EQ_INT(ctx, 2, shoot->r_no0);
    TEST_ASSERT_TRUE(ctx, shoot->patbase == bariapat);
    TEST_ASSERT_EQ_INT(ctx, 4, shoot->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, shoot->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, shoot->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 0, work->move_index);
    TEST_ASSERT_EQ_INT(ctx, 136, work->move_limit);
    TEST_ASSERT_TRUE(ctx, work->move_table == &shooterposi_0[1]);
    TEST_ASSERT_EQ_INT(ctx, 5184, work->target_x);
    TEST_ASSERT_EQ_INT(ctx, 240, work->target_y);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
}

static void test_shootermove_enters_pipe_and_rejects_misses(test_context *ctx) {
    sprite_status *shoot = &actwk[3];
    sprite_status *player = &actwk[0];
    player_work *pwork;

    reset_shoot1_state();
    shoot->r_no0 = 2;
    shoot->xposi.w.h = 100;
    shoot->yposi.w.h = 120;
    shoot->userflag.b.l = 1;
    player->xposi.w.h = 100;
    player->yposi.w.h = 120;
    player->r_no0 = 4;
    player->cddat = 32;
    player->mspeed.w = 3000;
    shoot->cddat = 32;
    shooter_get_work(shoot)->target_x = 160;
    shooter_get_work(shoot)->target_y = 120;
    shootermove(shoot);
    pwork = player_work_get(player);
    TEST_ASSERT_EQ_INT(ctx, 4, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, player->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 193, pwork->status_flags);
    TEST_ASSERT_EQ_INT(ctx, 2, player->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, player->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, player->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, shoot->cddat & 32);
    TEST_ASSERT_EQ_INT(ctx, 0, player->cddat & 32);
    TEST_ASSERT_EQ_INT(ctx, 2, player->cddat & 2);
    TEST_ASSERT_EQ_INT(ctx, 0, pwork->jump_started);
    TEST_ASSERT_EQ_INT(ctx, 100, player->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 120, player->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 145, soundset_requests[0]);

    reset_shoot1_state();
    shoot->xposi.w.h = 100;
    shoot->yposi.w.h = 100;
    player->xposi.w.h = 200;
    player->yposi.w.h = 100;
    shootermove(shoot);
    TEST_ASSERT_EQ_INT(ctx, 0, shoot->r_no0);

    reset_shoot1_state();
    shoot->xposi.w.h = 100;
    shoot->yposi.w.h = 100;
    player->xposi.w.h = 100;
    player->yposi.w.h = 200;
    shootermove(shoot);
    TEST_ASSERT_EQ_INT(ctx, 0, shoot->r_no0);

    reset_shoot1_state();
    shoot->xposi.w.h = 100;
    shoot->yposi.w.h = 100;
    player->xposi.w.h = 100;
    player->yposi.w.h = 100;
    player_work_get(player)->status_flags = 1;
    shootermove(shoot);
    TEST_ASSERT_EQ_INT(ctx, 0, shoot->r_no0);
}

static void test_speed_helpers_cover_axis_dominance_and_direction(
    test_context *ctx) {
    sprite_status *shoot = &actwk[3];
    sprite_status *player = &actwk[0];

    reset_shoot1_state();
    player->mspeed.w = 1024;
    player->xposi.w.h = 0;
    player->yposi.w.h = 0;
    shooter_get_work(shoot)->target_x = 0;
    shooter_get_work(shoot)->target_y = 2048;
    shooterspdset(shoot);
    TEST_ASSERT_EQ_INT(ctx, 0, player->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1024, player->yspeed.w);

    reset_shoot1_state();
    player->mspeed.w = 1024;
    player->xposi.w.h = 0;
    player->yposi.w.h = 0;
    shooter_get_work(shoot)->target_x = 0;
    shooter_get_work(shoot)->target_y = -2048;
    shooterspdset(shoot);
    TEST_ASSERT_EQ_INT(ctx, 0, player->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -1024, player->yspeed.w);

    reset_shoot1_state();
    player->mspeed.w = 1024;
    player->xposi.w.h = 0;
    player->yposi.w.h = 0;
    shooter_get_work(shoot)->target_x = -2048;
    shooter_get_work(shoot)->target_y = 1024;
    shooterspdset(shoot);
    TEST_ASSERT_EQ_INT(ctx, -1024, player->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 512, player->yspeed.w);

    reset_shoot1_state();
    player->mspeed.w = -1024;
    player->xposi.w.h = 0;
    player->yposi.w.h = 0;
    shooter_get_work(shoot)->target_x = 0;
    shooter_get_work(shoot)->target_y = -2048;
    shooterspdset(shoot);
    TEST_ASSERT_EQ_INT(ctx, 0, player->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1024, player->yspeed.w);

    reset_shoot1_state();
    player->mspeed.w = -1024;
    player->xposi.w.h = 0;
    player->yposi.w.h = 0;
    shooter_get_work(shoot)->target_x = -2048;
    shooter_get_work(shoot)->target_y = 1024;
    shooterspdset(shoot);
    TEST_ASSERT_EQ_INT(ctx, 1024, player->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -512, player->yspeed.w);

    reset_shoot1_state();
    player->mspeed.w = 3000;
    shoot->userflag.b.h = -1;
    mspd_set(shoot);
    TEST_ASSERT_EQ_INT(ctx, 4096, player->mspeed.w);

    reset_shoot1_state();
    player->mspeed.w = 3000;
    shoot->userflag.b.h = 3;
    mspd_set(shoot);
    TEST_ASSERT_EQ_INT(ctx, 4096, player->mspeed.w);
}

static void test_shootermove2_and_move3_route_progression(test_context *ctx) {
    static Uint16 route[] = {100, 200, 300, 400, 500, 600};
    sprite_status *shoot = &actwk[3];
    sprite_status *player = &actwk[0];
    shooter_work *work;

    reset_shoot1_state();
    player->mspeed.w = 1024;
    player->xposi.w.h = 0;
    player->yposi.w.h = 0;
    shooter_get_work(shoot)->target_x = 0;
    shooter_get_work(shoot)->target_y = 1024;
    shootermove2(shoot);
    TEST_ASSERT_EQ_INT(ctx, 2, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1024, player->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 145, soundset_requests[0]);

    reset_shoot1_state();
    player->xposi.l = 100 << 16;
    player->yposi.l = 200 << 16;
    player->xspeed.w = 2;
    player->yspeed.w = -3;
    shooter_get_work(shoot)->move_timer_high = 2;
    shootermove3(shoot);
    TEST_ASSERT_EQ_INT(ctx, (100 << 16) + (2 << 8), player->xposi.l);
    TEST_ASSERT_EQ_INT(ctx, (200 << 16) + (-3 << 8), player->yposi.l);

    reset_shoot1_state();
    player->mspeed.w = 1024;
    player->xposi.w.h = 0;
    player->yposi.w.h = 0;
    work = shooter_get_work(shoot);
    work->move_timer_high = 0;
    work->move_index = 0;
    work->move_limit = 8;
    work->target_x = 10;
    work->target_y = 20;
    work->move_table = route;
    shootermove3(shoot);
    TEST_ASSERT_EQ_INT(ctx, 10, player->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 20, player->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4, work->move_index);
    TEST_ASSERT_EQ_INT(ctx, 781, player->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1024, player->yspeed.w);

    reset_shoot1_state();
    shoot->r_no0 = 6;
    work = shooter_get_work(shoot);
    work->move_timer_high = 0;
    work->move_index = 8;
    work->move_limit = 8;
    work->target_x = 12;
    work->target_y = 4097;
    player_work_get(player)->status_flags = 129;
    shootermove3(shoot);
    TEST_ASSERT_EQ_INT(ctx, 0, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(player)->status_flags);
    TEST_ASSERT_EQ_INT(ctx, 1, player->yposi.w.h);
}

static void test_shooter_wrapper_dispatches_states(test_context *ctx) {
    sprite_status *shoot = &actwk[3];
    sprite_status *player = &actwk[0];

    reset_shoot1_state();
    shoot->r_no0 = 0;
    shoot->userflag.b.h = 0;
    player->xposi.w.h = 500;
    player->yposi.w.h = 500;
    shooter(shoot);
    TEST_ASSERT_EQ_INT(ctx, 2, shoot->r_no0);

    reset_shoot1_state();
    shoot->r_no0 = 2;
    shoot->xposi.w.h = 100;
    shoot->yposi.w.h = 100;
    player->xposi.w.h = 200;
    player->yposi.w.h = 100;
    shooter(shoot);
    TEST_ASSERT_EQ_INT(ctx, 2, shoot->r_no0);

    reset_shoot1_state();
    shoot->r_no0 = 4;
    player->mspeed.w = 1024;
    shooter_get_work(shoot)->target_x = 0;
    shooter_get_work(shoot)->target_y = 1024;
    shooter(shoot);
    TEST_ASSERT_EQ_INT(ctx, 6, shoot->r_no0);

    reset_shoot1_state();
    shoot->r_no0 = 6;
    shooter_get_work(shoot)->move_timer_high = 2;
    player->xspeed.w = 1;
    player->yspeed.w = 1;
    shooter(shoot);
    TEST_ASSERT_EQ_INT(ctx, 6, shoot->r_no0);
}

TEST_MAIN_BEGIN;
    test_shoot1_tables_capture_literal_data(&ctx);
    test_eda_and_kage_initialize_and_move(&ctx);
    test_ana_flag_paths_and_player_trigger(&ctx);
    test_kage1_flag_helpers_set_and_clear_selected_bit(&ctx);
    test_shooter_wrapper_time_flag_and_gate_paths(&ctx);
    test_shooter_init_sets_actor_and_first_route_target(&ctx);
    test_shootermove_enters_pipe_and_rejects_misses(&ctx);
    test_speed_helpers_cover_axis_dominance_and_direction(&ctx);
    test_shootermove2_and_move3_route_progression(&ctx);
    test_shooter_wrapper_dispatches_states(&ctx);
TEST_MAIN_END
