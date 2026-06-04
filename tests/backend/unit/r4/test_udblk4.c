#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_x;
static int frameout_s0_count;
static sprite_status *frameout_s0_actor;
static int ride_on_chk_count;
static sprite_status *ride_on_chk_actor;
static int actwkchk_count;
static int next_alloc_index;
static int actwkchk_fail_after;
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 sinset_sin;
static Sint16 sinset_cos;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint32 frameout_s0(sprite_status *pActwk);
Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint32 actwkchk(sprite_status **ppActwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);

#include "src/r4/udblk4.c"

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

Sint32 frameout_s0(sprite_status *pActwk) {
    ++frameout_s0_count;
    frameout_s0_actor = pActwk;
    return 0;
}

Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_chk_count;
    ride_on_chk_actor = pActwk;
    (void)pPlayerwk;
    return 0;
}

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_fail_after >= 0 && actwkchk_count > actwkchk_fail_after) {
        *ppActwk = 0;
        return -1;
    }
    *ppActwk = &actwk[next_alloc_index++];
    return 0;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    ++sinset_count;
    sinset_angle = kakudo;
    *sin = sinset_sin;
    *cos = sinset_cos;
}

static void reset_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_x = 0;
    frameout_s0_count = 0;
    frameout_s0_actor = 0;
    ride_on_chk_count = 0;
    ride_on_chk_actor = 0;
    actwkchk_count = 0;
    sinset_count = 0;
    sinset_angle = 0;
}

static void reset_udblk4_state(void) {
    memset(actwk, 0, sizeof(actwk));
    next_alloc_index = 20;
    actwkchk_fail_after = -1;
    sinset_sin = 256;
    sinset_cos = 512;
    reset_logs();
}

static void assert_action_origin(test_context *ctx, sprite_status *actor,
                                 Sint16 origin) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, origin, frameout_s00_x);
}

static void test_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, udblk4pat1[0] == &pat00);
    TEST_ASSERT_TRUE(ctx, udblk4pat3[0] == &pat01);
    TEST_ASSERT_TRUE(ctx, udblk4pat4[0] == &pat02);
    TEST_ASSERT_TRUE(ctx, udblk4pat5[0] == &pat03);
    TEST_ASSERT_TRUE(ctx, udblk4pat7[0] == &pat03);
    TEST_ASSERT_TRUE(ctx, udblk4pat8[0] == &pat04);
    TEST_ASSERT_TRUE(ctx, udblk4pat6[1] == &pat06);
    TEST_ASSERT_TRUE(ctx, udblk4pat9[0] == &pat05);
    TEST_ASSERT_TRUE(ctx, udblk4patA[0] == &pat07);
    TEST_ASSERT_EQ_INT(ctx, -16, pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -64, pat00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_UDBLK4_BASE, pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_UDBLK4_BASE + 7, pat07.spra[0].index);
}

static void test_type1_type2_type3(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_udblk4_state();
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 50;

    udblk4(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 17514, actor->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->sprpri);
    TEST_ASSERT_TRUE(ctx, actor->patbase == udblk4pat1);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 64, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 114, udblk4_get_work(actor)->base_y);
    TEST_ASSERT_EQ_INT(ctx, 100, udblk4_get_work(actor)->base_x);
    TEST_ASSERT_EQ_INT(ctx, 193, udblk4_get_work(actor)->phase_high);
    TEST_ASSERT_EQ_INT(ctx, 6, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 35, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, udblk4_get_work(&actwk[20])->child_index);
    assert_action_origin(ctx, actor, 100);

    reset_udblk4_state();
    actor = &actwk[3];
    actor->userflag.b.h = 1;
    udblk4_get_work(actor)->child_index = 3;
    actor->yposi.w.h = 40;

    udblk4(actor);

    TEST_ASSERT_EQ_INT(ctx, 121, udblk4_get_work(actor)->phase_high);
    TEST_ASSERT_EQ_INT(ctx, -96, actor->xposi.w.h);

    reset_udblk4_state();
    actor = &actwk[3];
    actor->userflag.b.h = 2;
    udblk4(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_udblk4_state();
    actor = &actwk[3];
    actor->userflag.b.h = 3;
    actor->xposi.w.h = 200;
    actor->yposi.w.h = 100;

    udblk4(actor);

    TEST_ASSERT_TRUE(ctx, actor->patbase == udblk4pat3);
    TEST_ASSERT_EQ_INT(ctx, 32, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 96, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 20, udblk4_get_work(actor)->base_y);
    TEST_ASSERT_EQ_INT(ctx, 64, sinset_angle);
    assert_action_origin(ctx, actor, 200);
}

static void test_type4_type5_type6_type7(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_udblk4_state();
    actor->userflag.b.h = 4;
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 200;

    udblk4(actor);

    TEST_ASSERT_TRUE(ctx, actor->patbase == udblk4pat4);
    TEST_ASSERT_EQ_INT(ctx, 18, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 48, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 192, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 3, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 34, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 4, udblk4_get_work(&actwk[20])->parent_index);
    TEST_ASSERT_EQ_INT(ctx, 35, actwk[21].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, udblk4_get_work(&actwk[21])->child_index);
    assert_action_origin(ctx, actor, 300);

    reset_logs();
    actor->r_no0 = 2;
    udblk4_get_work(actor)->parent_index = 20;
    udblk4(actor);
    TEST_ASSERT_EQ_INT(ctx, actor->yspeed.w, actwk[20].yspeed.w);

    reset_udblk4_state();
    actor = &actwk[4];
    actor->userflag.b.h = 5;
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 200;

    udblk4(actor);

    TEST_ASSERT_TRUE(ctx, actor->patbase == udblk4pat5);
    TEST_ASSERT_EQ_INT(ctx, 20, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 65, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 3, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 35, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[20].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 240, udblk4_get_work(&actwk[20])->phase_high);
    assert_action_origin(ctx, actor, 300);

    reset_udblk4_state();
    actor = &actwk[4];
    actor->userflag.b.h = 6;
    actor->xposi.w.h = 400;
    actor->yposi.w.h = 120;

    udblk4(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 7, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 35, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 9, actwk[20].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 255, udblk4_get_work(&actwk[20])->variant);
    TEST_ASSERT_EQ_INT(ctx, 4, udblk4_get_work(&actwk[20])->parent_index);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_EQ_INT(ctx, 400, frameout_s00_x);

    reset_udblk4_state();
    actor = &actwk[4];
    actor->userflag.b.h = 7;
    actor->xposi.w.h = 500;
    actor->yposi.w.h = 160;

    udblk4(actor);

    TEST_ASSERT_TRUE(ctx, actor->patbase == udblk4pat7);
    TEST_ASSERT_EQ_INT(ctx, 548, udblk4_get_work(actor)->base_x);
    TEST_ASSERT_EQ_INT(ctx, 3, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 7, actwk[20].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 224, udblk4_get_work(&actwk[20])->phase_high);
    assert_action_origin(ctx, actor, 548);
}

static void test_type8_type9_typeA_typeB(test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_udblk4_state();
    actor->userflag.b.h = 8;
    actor->xposi.w.h = 600;
    actor->yposi.w.h = 240;

    udblk4(actor);

    TEST_ASSERT_TRUE(ctx, actor->patbase == udblk4pat8);
    TEST_ASSERT_EQ_INT(ctx, 32, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 48, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 3, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 35, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 8, actwk[20].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);

    reset_udblk4_state();
    actor = &actwk[5];
    actor->userflag.b.h = 9;
    actor->xposi.w.h = 640;
    actor->yposi.w.h = 260;
    udblk4_get_work(actor)->variant = 128;

    udblk4(actor);

    TEST_ASSERT_TRUE(ctx, actor->patbase == udblk4pat9);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 35, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 9, actwk[20].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 160, actwk[20].yposi.w.h - actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_udblk4_state();
    actor = &actwk[5];
    actor->userflag.b.h = 10;
    actor->xposi.w.h = 700;
    actor->yposi.w.h = 300;

    udblk4(actor);

    TEST_ASSERT_TRUE(ctx, actor->patbase == udblk4patA);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 6, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[20].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 224, udblk4_get_work(&actwk[20])->phase_high);
    TEST_ASSERT_EQ_INT(ctx, 668, actwk[20].xposi.w.h);

    reset_udblk4_state();
    actor = &actwk[5];
    actor->userflag.b.h = 11;
    actor->xposi.w.h = 800;
    actor->yposi.w.h = 400;
    udblk4_get_work(actor)->child_index = 2;

    udblk4(actor);

    TEST_ASSERT_TRUE(ctx, actor->patbase == udblk4pat9);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, -80, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 32, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -256, actor->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_parent_frameout_edges_and_typeB_moves(test_context *ctx) {
    sprite_status *actor = &actwk[6];

    reset_udblk4_state();
    actor->userflag.b.h = 1;
    actor->r_no0 = 2;
    actor->yspeed.w = -1;
    udblk4_get_work(actor)->base_y = 10;
    udblk4_get_work(actor)->base_x = 20;
    udblk4(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, ride_on_chk_count);

    reset_udblk4_state();
    actor = &actwk[6];
    actor->userflag.b.h = 6;
    actor->r_no0 = 2;
    udblk4_get_work(actor)->base_x = 123;
    udblk4(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_EQ_INT(ctx, 123, frameout_s00_x);

    reset_udblk4_state();
    actor->userflag.b.h = 8;
    actor->r_no0 = 2;
    udblk4_get_work(actor)->parent_index = 20;
    actwk[20].actno = 0;
    udblk4(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s0_count);
    TEST_ASSERT_TRUE(ctx, frameout_s0_actor == actor);

    reset_udblk4_state();
    actor = &actwk[6];
    actor->userflag.b.h = 9;
    actor->r_no0 = 2;
    udblk4_get_work(actor)->parent_index = 20;
    actwk[20].actno = 0;
    udblk4(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s0_count);

    reset_udblk4_state();
    actor = &actwk[6];
    actor->userflag.b.h = 10;
    actor->r_no0 = 2;
    udblk4_get_work(actor)->parent_index = 20;
    actwk[20].actno = 0;
    udblk4(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s0_count);

    reset_udblk4_state();
    actor = &actwk[6];
    actor->userflag.b.h = 11;
    actor->xposi.w.h = 900;
    actor->yposi.w.h = 500;
    udblk4(actor);
    TEST_ASSERT_EQ_INT(ctx, 5, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 11, actwk[20].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, udblk4_get_work(&actwk[20])->child_index);
    TEST_ASSERT_EQ_INT(ctx, 500, actor->yposi.w.h);

    reset_udblk4_state();
    actor = &actwk[6];
    actor->userflag.b.h = 11;
    actor->r_no0 = 2;
    udblk4_get_work(actor)->parent_index = 20;
    actwk[20].actno = 0;
    udblk4(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s0_count);

    reset_udblk4_state();
    actor = &actwk[6];
    actor->userflag.b.h = 11;
    actor->r_no0 = 2;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 50;
    actor->yspeed.w = -256;
    udblk4_get_work(actor)->base_y = 200;

    udblk4(actor);

    TEST_ASSERT_EQ_INT(ctx, 328, actor->yposi.w.h);

    reset_udblk4_state();
    actor = &actwk[6];
    actor->userflag.b.h = 11;
    actor->r_no0 = 2;
    actor->yposi.w.h = 220;
    actor->yspeed.w = -256;
    udblk4_get_work(actor)->base_y = 200;

    udblk4(actor);

    TEST_ASSERT_EQ_INT(ctx, 219, actor->yposi.w.h);

    reset_udblk4_state();
    actor = &actwk[6];
    actor->userflag.b.h = 11;
    actor->r_no0 = 2;
    actor->yposi.w.h = 170;
    actor->yspeed.w = -256;
    udblk4_get_work(actor)->base_y = 200;

    udblk4(actor);

    TEST_ASSERT_EQ_INT(ctx, 169, actor->yposi.w.h);

    reset_udblk4_state();
    actor = &actwk[6];
    actor->userflag.b.h = 11;
    actor->r_no0 = 2;
    actor->xposi.w.h = 400;
    actor->yposi.w.h = 50;
    actor->yspeed.w = 256;
    udblk4_get_work(actor)->base_y = 200;

    udblk4(actor);

    TEST_ASSERT_EQ_INT(ctx, 72, actor->yposi.w.h);

    reset_udblk4_state();
    actor = &actwk[6];
    actor->userflag.b.h = 11;
    actor->r_no0 = 2;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 50;
    actor->yspeed.w = 256;
    udblk4_get_work(actor)->base_y = 200;

    udblk4(actor);

    TEST_ASSERT_EQ_INT(ctx, 51, actor->yposi.w.h);

    reset_udblk4_state();
    actor = &actwk[6];
    actor->userflag.b.h = 11;
    actor->r_no0 = 2;
    actor->xposi.w.h = 250;
    actor->yposi.w.h = 50;
    actor->yspeed.w = 256;
    udblk4_get_work(actor)->base_y = 200;

    udblk4(actor);

    TEST_ASSERT_EQ_INT(ctx, 51, actor->yposi.w.h);
}

TEST_MAIN_BEGIN;
test_tables_capture_literal_data(&ctx);
test_type1_type2_type3(&ctx);
test_type4_type5_type6_type7(&ctx);
test_type8_type9_typeA_typeB(&ctx);
test_parent_frameout_edges_and_typeB_moves(&ctx);
TEST_MAIN_END
