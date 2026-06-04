#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 flagwork[766];
Uint8 flagworkcnt;
Uint8 flagworkcnt2;
Uint8 time_flag;
short_union stageno;
int_union scra_h_posit;
int_union scra_v_posit;

static Sint16 enemy_suicide_result;
static int enemy_suicide_count;
static int actionsub_count;
static sprite_status *actionsub_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_s0_count;
static sprite_status *frameout_s0_actor;
static int actwkchk_count;
static int actwkchk2_count;
static Sint32 actwkchk_result;
static Sint32 actwkchk2_result;
static sprite_status *next_spawn;
static Uint8 stable_flag_byte;
static Sint16 stable_offset_table[2];

Sint16 enemy_suicide(sprite_status *pActwk);
void actionsub(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void frameout(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint32 frameout_s0(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);

#include "src/r7/hotaru7.c"

Sint16 enemy_suicide(sprite_status *pActwk) {
    (void)pActwk;
    ++enemy_suicide_count;
    return enemy_suicide_result;
}

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
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

Sint32 frameout_s0(sprite_status *pActwk) {
    ++frameout_s0_count;
    frameout_s0_actor = pActwk;
    return 0;
}

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    *ppActwk = next_spawn;
    return actwkchk_result;
}

Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk) {
    (void)pActwk;
    ++actwkchk2_count;
    *ppNewActwk = next_spawn;
    return actwkchk2_result;
}

static void reset_hotaru7_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(flagwork, 0, sizeof(flagwork));
    flagworkcnt = 0;
    flagworkcnt2 = 0;
    time_flag = 0;
    stageno.w = 0;
    scra_h_posit.l = 0;
    scra_v_posit.l = 0;
    enemy_suicide_result = 0;
    enemy_suicide_count = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_s0_count = 0;
    frameout_s0_actor = 0;
    actwkchk_count = 0;
    actwkchk2_count = 0;
    actwkchk_result = 0;
    actwkchk2_result = 0;
    next_spawn = &actwk[20];
    stable_flag_byte = 0;
    stable_offset_table[0] = 0;
    stable_offset_table[1] = 0;
}

static void prepare_child(sprite_status *child, Sint16 parent_index) {
    child->userflag.b.l = -1;
    hotaru7_work_get(child)->parent_cdsts = 9;
    hotaru7_work_get(child)->parent_index = parent_index;
    actwk[parent_index].actno = 35;
    actwk[parent_index].cdsts = 9;
}

static void test_hotaru7_area_uses_unsigned_window_edges(test_context *ctx) {
    sprite_status actor;
    sprite_status player;

    memset(&actor, 0, sizeof(actor));
    memset(&player, 0, sizeof(player));
    actor.xposi.w.h = 100;
    actor.yposi.w.h = 100;
    player.xposi.w.h = 100;
    player.yposi.w.h = 100;

    TEST_ASSERT_EQ_INT(ctx, 1,
                       (Sint32)area(&actor, &player, 65296, 480, 65296, 480));

    player.xposi.w.h = 700;

    TEST_ASSERT_EQ_INT(ctx, 0,
                       (Sint32)area(&actor, &player, 65296, 480, 65296, 480));

    player.xposi.w.h = 100;
    player.yposi.w.h = 700;

    TEST_ASSERT_EQ_INT(ctx, 0,
                       (Sint32)area(&actor, &player, 65296, 480, 65296, 480));
}

static void test_hotaru7_main_initializes_and_waits_for_player(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_hotaru7_state();
    actor->actno = 35;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 100;
    actwk[0].xposi.w.h = 700;
    actwk[0].yposi.w.h = 100;

    hotaru7(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 33885, actor->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);

    reset_hotaru7_state();
    actor = &actwk[4];
    actor->actno = 35;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 100;
    actor->userflag.b.h = 13;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 100;
    stageno.b.l = 2;

    hotaru7(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 33568, actor->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_hotaru7_child_frameout_and_suicide_gates(test_context *ctx) {
    sprite_status *child = &actwk[6];

    reset_hotaru7_state();
    child->userflag.b.l = -1;
    hotaru7_work_get(child)->parent_index = 3;

    hotaru7(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 0, enemy_suicide_count);

    reset_hotaru7_state();
    child = &actwk[6];
    prepare_child(child, 3);
    enemy_suicide_result = 1;

    hotaru7(child);

    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

static void test_hotaru7_child_initializes_variants_and_direction(
    test_context *ctx) {
    sprite_status *child = &actwk[6];

    reset_hotaru7_state();
    prepare_child(child, 3);
    child->xposi.w.h = 200;
    actwk[0].xposi.w.h = 300;

    hotaru7(child);

    TEST_ASSERT_EQ_INT(ctx, 2, child->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 5, child->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, child->cddat);
    TEST_ASSERT_EQ_INT(ctx, 1, child->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 20, child->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 12, child->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 50, child->colino);
    TEST_ASSERT_TRUE(ctx, child->patbase == pat_hotaru7_e);
    TEST_ASSERT_EQ_INT(ctx, 200, hotaru7_work_get(child)->origin_x);

    reset_hotaru7_state();
    child = &actwk[6];
    prepare_child(child, 3);
    child->userflag.b.h = 1;
    child->xposi.w.h = 200;
    actwk[0].xposi.w.h = 100;

    hotaru7(child);

    TEST_ASSERT_EQ_INT(ctx, 4, child->actflg);
    TEST_ASSERT_EQ_INT(ctx, 0, child->cddat);
    TEST_ASSERT_TRUE(ctx, child->patbase == pat_hotaru7_b);
}

static void test_hotaru7_child_fly_moves_and_reports_callbacks(
    test_context *ctx) {
    sprite_status *child = &actwk[6];

    reset_hotaru7_state();
    prepare_child(child, 3);
    child->xposi.w.h = 200;
    actwk[0].xposi.w.h = 1000;
    b_init(child);
    enemy_suicide_count = 0;

    hotaru7(child);

    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 201, child->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == child);
}

static void test_hotaru7_child_fly_reverses_and_alerts_siblings(
    test_context *ctx) {
    sprite_status *child = &actwk[6];

    reset_hotaru7_state();
    prepare_child(child, 3);
    hotaru7_work_get(&actwk[3])->child1_index = 7;
    hotaru7_work_get(&actwk[3])->child2_index = 8;
    child->xposi.w.h = 279;
    hotaru7_work_get(child)->origin_x = 200;
    hotaru7_work_get(child)->speed = 65536;
    actwk[0].xposi.w.h = 279;
    actwk[0].yposi.w.h = 0;

    b_fly(child);

    TEST_ASSERT_EQ_INT(ctx, 279, child->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, child->actflg & 1);
    TEST_ASSERT_EQ_INT(ctx, 1, child->cddat & 1);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[7].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[8].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_hotaru7_state();
    prepare_child(child, 3);
    child->xposi.w.h = 200;
    hotaru7_work_get(child)->origin_x = 200;
    hotaru7_work_get(child)->speed = -65536;

    b_fly(child);

    TEST_ASSERT_EQ_INT(ctx, 199, child->xposi.w.h);
}

static void test_hotaru7_parent_make_spawns_each_side(test_context *ctx) {
    sprite_status *main = &actwk[4];

    reset_hotaru7_state();
    main->r_no0 = 4;
    main->actno = 35;
    main->userflag.b.h = 1;
    main->cdsts = 9;
    main->xposi.w.h = 100;
    main->yposi.w.h = 200;
    main->sproffset = 1234;
    stable_flag_byte = 2;
    stable_offset_table[0] = 36;
    stable_offset_table[1] = 60;
    hotaru7_work_get(main)->flag_work = &stable_flag_byte;
    hotaru7_work_get(main)->position_table = stable_offset_table;
    next_spawn = &actwk[10];

    a_make(main);

    TEST_ASSERT_EQ_INT(ctx, 6, main->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 10, hotaru7_work_get(main)->child1_index);
    TEST_ASSERT_EQ_INT(ctx, 35, actwk[10].actno);
    TEST_ASSERT_EQ_INT(ctx, -1, actwk[10].userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 136, actwk[10].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 260, actwk[10].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 9, hotaru7_work_get(&actwk[10])->parent_cdsts);

    reset_hotaru7_state();
    main = &actwk[4];
    main->actno = 35;
    main->cdsts = 9;
    main->xposi.w.h = 100;
    main->yposi.w.h = 200;
    stable_flag_byte = 1;
    stable_offset_table[0] = 8;
    stable_offset_table[1] = -68;
    hotaru7_work_get(main)->flag_work = &stable_flag_byte;
    hotaru7_work_get(main)->position_table = stable_offset_table;
    next_spawn = &actwk[11];

    a_make(main);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 11, hotaru7_work_get(main)->child2_index);
    TEST_ASSERT_EQ_INT(ctx, 92, actwk[11].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 268, actwk[11].yposi.w.h);

    reset_hotaru7_state();
    main = &actwk[4];
    stable_flag_byte = 0;
    hotaru7_work_get(main)->flag_work = &stable_flag_byte;
    actwkchk2_result = 1;

    a_make(main);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s0_count);

    reset_hotaru7_state();
    main = &actwk[4];
    stable_flag_byte = 1;
    hotaru7_work_get(main)->flag_work = &stable_flag_byte;
    actwkchk2_result = 1;

    a_make(main);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s0_count);
}

static void test_hotaru7_parent_move_tracks_child_completion(
    test_context *ctx) {
    sprite_status *main = &actwk[4];

    reset_hotaru7_state();
    main->actno = 35;
    stable_flag_byte = 0;
    hotaru7_work_get(main)->flag_work = &stable_flag_byte;
    hotaru7_work_get(main)->child1_index = 10;
    hotaru7_work_get(main)->child2_index = 11;
    actwk[10].actno = 0;
    actwk[11].actno = 0;

    a_move(main);

    TEST_ASSERT_EQ_INT(ctx, 3, stable_flag_byte);
    TEST_ASSERT_EQ_INT(ctx, 0, hotaru7_work_get(main)->child1_index);
    TEST_ASSERT_EQ_INT(ctx, 0, hotaru7_work_get(main)->child2_index);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s0_count);

    reset_hotaru7_state();
    main = &actwk[4];
    stable_flag_byte = 131;
    hotaru7_work_get(main)->flag_work = &stable_flag_byte;

    a_move(main);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == main);
}

static void test_hotaru7_near_moves_children_and_fixes_missing_pairs(
    test_context *ctx) {
    sprite_status *child = &actwk[6];

    reset_hotaru7_state();
    prepare_child(child, 3);
    hotaru7_work_get(&actwk[3])->child1_index = 7;
    hotaru7_work_get(&actwk[3])->child2_index = 8;
    hotaru7_work_get(child)->speed = -65536;
    actwk[7].xposi.w.h = 100;
    actwk[8].xposi.w.h = 120;
    actwk[7].yposi.w.h = 50;
    actwk[8].yposi.w.h = 70;

    b_near(child);

    TEST_ASSERT_EQ_INT(ctx, 101, actwk[7].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 121, actwk[8].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 51, actwk[7].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 71, actwk[8].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[7].cddat & 1);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_hotaru7_state();
    prepare_child(child, 3);
    hotaru7_work_get(&actwk[3])->child1_index = 7;
    hotaru7_work_get(&actwk[3])->child2_index = 8;
    hotaru7_work_get(child)->speed = 65536;
    actwk[7].actflg = actwk[8].actflg = 1;
    actwk[7].cddat = actwk[8].cddat = 1;
    actwk[7].xposi.w.h = 200;
    actwk[8].xposi.w.h = 220;
    actwk[7].yposi.w.h = 150;
    actwk[8].yposi.w.h = 170;

    b_near(child);

    TEST_ASSERT_EQ_INT(ctx, 199, actwk[7].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 219, actwk[8].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 149, actwk[7].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 169, actwk[8].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[7].cddat & 1);

    reset_hotaru7_state();
    prepare_child(child, 3);
    hotaru7_work_get(&actwk[3])->child1_index = 7;
    hotaru7_work_get(&actwk[3])->child2_index = 8;
    hotaru7_work_get(child)->speed = -65536;
    actwk[7].actflg = actwk[8].actflg = 1;
    actwk[7].cddat = actwk[8].cddat = 1;
    actwk[7].xposi.w.h = 200;
    actwk[8].xposi.w.h = 220;
    actwk[7].yposi.w.h = 150;
    actwk[8].yposi.w.h = 170;

    b_near(child);

    TEST_ASSERT_EQ_INT(ctx, 199, actwk[7].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 149, actwk[7].yposi.w.h);

    reset_hotaru7_state();
    prepare_child(child, 3);
    hotaru7_work_get(&actwk[3])->child1_index = 7;
    hotaru7_work_get(&actwk[3])->child2_index = 8;
    hotaru7_work_get(child)->speed = 65536;
    actwk[7].xposi.w.h = 156;
    actwk[8].xposi.w.h = 164;
    actwk[7].yposi.w.h = 108;
    actwk[8].yposi.w.h = 116;

    b_near(child);

    TEST_ASSERT_EQ_INT(ctx, 6, actwk[7].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[8].r_no0);

    reset_hotaru7_state();
    prepare_child(child, 3);
    hotaru7_work_get(&actwk[3])->child1_index = 0;

    b_near(child);

    TEST_ASSERT_EQ_INT(ctx, 4, child->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 51, child->colino);
}

static void test_hotaru7_fix_start_light_and_up_states(test_context *ctx) {
    sprite_status *child = &actwk[6];

    reset_hotaru7_state();
    prepare_child(child, 3);
    child->xposi.w.h = 200;
    child->yposi.w.h = 180;
    scra_h_posit.w.h = 20;
    scra_v_posit.w.h = 40;

    b_fix(child);

    TEST_ASSERT_EQ_INT(ctx, 2, child->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, child->actflg);
    TEST_ASSERT_EQ_INT(ctx, 51, child->colino);
    TEST_ASSERT_EQ_INT(ctx, 308, child->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 268, child->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_hotaru7_state();
    prepare_child(child, 3);
    child->r_no0 = 10;
    child->xposi.w.h = 308;
    child->yposi.w.h = 268;
    scra_h_posit.w.h = 20;
    scra_v_posit.w.h = 40;

    b_start(child);

    TEST_ASSERT_EQ_INT(ctx, 12, child->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 511, child->mstno.w);
    TEST_ASSERT_EQ_INT(ctx, 200, child->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 180, child->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_hotaru7_state();
    prepare_child(child, 3);

    b_up(child);

    TEST_ASSERT_EQ_INT(ctx, 2, child->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1023, child->mstno.w);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_hotaru7_state();
    prepare_child(child, 3);

    b_light(child);

    TEST_ASSERT_EQ_INT(ctx, 2, child->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 767, child->mstno.w);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);

    reset_hotaru7_state();
    prepare_child(child, 3);
    child->yposi.w.h = 100;
    actwk[0].yposi.w.h = 400;

    b_up1(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);

    reset_hotaru7_state();
    prepare_child(child, 3);
    child->actflg = 128;
    child->yposi.w.h = 100;

    b_up1(child);

    TEST_ASSERT_EQ_INT(ctx, 98, child->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_hotaru7_make_c_and_die_vector_paths(test_context *ctx) {
    sprite_status *main = &actwk[3];
    sprite_status *left = &actwk[7];
    sprite_status *right = &actwk[8];
    sprite_status *shot = &actwk[20];

    reset_hotaru7_state();
    hotaru7_work_get(main)->child1_index = 7;
    hotaru7_work_get(main)->child2_index = 8;
    hotaru7_work_get(left)->parent_index = 3;
    left->actno = 35;
    left->actflg = 128;
    left->sproffset = 300;
    left->xposi.w.h = 100;
    left->yposi.w.h = 200;
    right->actflg = 128;
    right->xposi.w.h = 140;
    right->yposi.w.h = 240;
    next_spawn = shot;

    b_make_c(left);

    TEST_ASSERT_EQ_INT(ctx, 3, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 35, shot->actno);
    TEST_ASSERT_EQ_INT(ctx, -2, shot->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 300, shot->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 130, shot->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 237, shot->yposi.w.h);

    reset_hotaru7_state();
    hotaru7_work_get(main)->child1_index = 7;
    hotaru7_work_get(main)->child2_index = 8;
    hotaru7_work_get(left)->parent_index = 3;
    left->actflg = 128;
    right->actflg = 0;

    b_make_c(left);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    reset_hotaru7_state();
    hotaru7_work_get(main)->child1_index = 7;
    hotaru7_work_get(main)->child2_index = 8;
    hotaru7_work_get(left)->parent_index = 3;
    left->actflg = 128;
    right->actflg = 128;
    actwkchk_result = 1;

    b_make_c(left);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);

    reset_hotaru7_state();
    hotaru7_work_get(main)->child1_index = 7;
    hotaru7_work_get(main)->child2_index = 8;
    b_die_v(&actwk[7], main);

    TEST_ASSERT_EQ_INT(ctx, 0, hotaru7_work_get(main)->child1_index);

    hotaru7_work_get(main)->child1_index = 9;
    hotaru7_work_get(main)->child2_index = 8;
    b_die_v(&actwk[7], main);

    TEST_ASSERT_EQ_INT(ctx, 0, hotaru7_work_get(main)->child2_index);
}

static void test_hotaru7_projectile_initializes_and_rejects_missing_links(
    test_context *ctx) {
    sprite_status *shot = &actwk[9];

    reset_hotaru7_state();
    shot->userflag.b.l = -2;
    hotaru7_work_get(shot)->partner1_index = 7;
    hotaru7_work_get(shot)->partner2_index = 8;

    hotaru7(shot);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == shot);

    reset_hotaru7_state();
    shot = &actwk[9];
    shot->userflag.b.l = -2;
    hotaru7_work_get(shot)->partner1_index = 7;
    hotaru7_work_get(shot)->partner2_index = 8;
    actwk[7].actno = 35;
    actwk[7].r_no0 = 16;
    actwk[8].actno = 35;
    actwk[8].r_no0 = 16;

    hotaru7(shot);

    TEST_ASSERT_EQ_INT(ctx, 2, shot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, shot->actflg);
    TEST_ASSERT_EQ_INT(ctx, 2, shot->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 12, shot->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 180, shot->colino);
    TEST_ASSERT_TRUE(ctx, shot->patbase == pat_tama);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg_tama);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_hotaru7_area_uses_unsigned_window_edges(&ctx);
    test_hotaru7_main_initializes_and_waits_for_player(&ctx);
    test_hotaru7_child_frameout_and_suicide_gates(&ctx);
    test_hotaru7_child_initializes_variants_and_direction(&ctx);
    test_hotaru7_child_fly_moves_and_reports_callbacks(&ctx);
    test_hotaru7_child_fly_reverses_and_alerts_siblings(&ctx);
    test_hotaru7_parent_make_spawns_each_side(&ctx);
    test_hotaru7_parent_move_tracks_child_completion(&ctx);
    test_hotaru7_near_moves_children_and_fixes_missing_pairs(&ctx);
    test_hotaru7_fix_start_light_and_up_states(&ctx);
    test_hotaru7_make_c_and_die_vector_paths(&ctx);
    test_hotaru7_projectile_initializes_and_rejects_missing_links(&ctx);
TEST_MAIN_END;
