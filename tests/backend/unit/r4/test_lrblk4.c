#include <stddef.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
int_union scra_h_posit;
Uint8 time_flag;
Uint8 flagwork[766];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_x;
static int actwkchk_count;
static int next_alloc_index;
static int actwkchk_fail_after;
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 sinset_sin;
static Sint16 sinset_cos;
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint32 actwkchk(sprite_status **ppActwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r4/lrblk4.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
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

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++hitchk_count;
    hitchk_actor = pActwk;
    hitchk_player = pPlayerwk;
    return 0;
}

static size_t short_alias_offset(int short_index) {
    return (size_t)short_index * sizeof(Sint16) -
           offsetof(sprite_status, actfree);
}

static void set_actor_short_alias(sprite_status *actor, int short_index,
                                  Sint16 value) {
    size_t offset = short_alias_offset(short_index);
    Uint16 bits = (Uint16)value;
    actor->actfree[offset] = (Uint8)bits;
    actor->actfree[offset + 1] = (Uint8)(bits >> 8);
}

static Sint16 actor_short_alias(sprite_status *actor, int short_index) {
    size_t offset = short_alias_offset(short_index);
    return (Sint16)((Uint16)actor->actfree[offset] |
                    ((Uint16)actor->actfree[offset + 1] << 8));
}

static void reset_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_x = 0;
    actwkchk_count = 0;
    sinset_count = 0;
    sinset_angle = 0;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
}

static void reset_lrblk4_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(flagwork, 0, sizeof(flagwork));
    memset(&scra_h_posit, 0, sizeof(scra_h_posit));
    time_flag = 0;
    next_alloc_index = 20;
    actwkchk_fail_after = -1;
    sinset_sin = 256;
    sinset_cos = 512;
    reset_logs();
}

static void assert_action_only(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
}

static void test_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, lrblk4pat1[0] == &pat10);
    TEST_ASSERT_TRUE(ctx, lrblk4pat1[1] == &pat11);
    TEST_ASSERT_TRUE(ctx, lrblk4pat2[0] == &pat20);
    TEST_ASSERT_TRUE(ctx, lrblk4pat3[0] == &pat30);
    TEST_ASSERT_TRUE(ctx, lrblk4pat5[0] == &pat50);
    TEST_ASSERT_TRUE(ctx, lrblk4pat6[3] == &pat63);
    TEST_ASSERT_TRUE(ctx, lrblk4pat8[0] == &pat10);
    TEST_ASSERT_EQ_INT(ctx, -64, pat10.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, pat10.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_LRBLK4_BASE, pat10.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_LRBLK4_BASE + 8, pat63.spra[0].index);
}

static void test_type1_master_child_and_fout_paths(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_lrblk4_state();
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 50;

    lrblk4(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 17514, actor->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->sprpri);
    TEST_ASSERT_TRUE(ctx, actor->patbase == lrblk4pat1);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 48, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 132, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 6, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 36, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[20].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[20].actfree[19]);
    TEST_ASSERT_EQ_INT(ctx, 48, actwk[20].actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 36, actwk[25].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[25].actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 324, actwk[25].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    assert_action_only(ctx, actor);

    reset_lrblk4_state();
    actor = &actwk[3];
    actor->r_no0 = 2;
    actor->xposi.w.h = 2048;
    actor->cdsts = 2;
    time_flag = 3;
    scra_h_posit.w.h = 0;
    flagwork[9] = 255;
    set_actor_short_alias(actor, 29, 2048);

    lrblk4(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 127, flagwork[9]);

    reset_lrblk4_state();
    actor = &actwk[3];
    actor->actfree[18] = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 50;

    lrblk4(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 132, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    assert_action_only(ctx, actor);
}

static void test_type2_and_type3_paths(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_lrblk4_state();
    actor->userflag.b.h = 2;
    actor->xposi.w.h = 200;
    actor->yposi.w.h = 60;
    actor->actfree[16] = 128;

    lrblk4(actor);

    TEST_ASSERT_TRUE(ctx, actor->patbase == lrblk4pat2);
    TEST_ASSERT_EQ_INT(ctx, 168, actor_short_alias(actor, 27));
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 36, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[20].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[20].actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 264, actwk[20].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, actor_short_alias(&actwk[20], 29));
    assert_action_only(ctx, actor);

    reset_lrblk4_state();
    actor = &actwk[4];
    actor->userflag.b.h = 3;
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 100;
    actor->actfree[18] = 1;

    lrblk4(actor);

    TEST_ASSERT_TRUE(ctx, actor->patbase == lrblk4pat3);
    TEST_ASSERT_EQ_INT(ctx, 156, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 100, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -128, actor->xspeed.w);
    assert_action_only(ctx, actor);

    reset_logs();
    actor->r_no0 = 2;
    actor->actflg = 128;
    actor->xposi.w.h = 300;
    actor->xspeed.w = 128;
    set_actor_short_alias(actor, 29, 300);
    actwk[0].yposi.w.h = 120;

    lrblk4(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == actor);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);

    reset_logs();
    actor->r_no0 = 2;
    actor->actflg = 128;
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 100;
    actor->xspeed.w = 0;
    set_actor_short_alias(actor, 29, 300);
    actwk[0].yposi.w.h = 80;

    lrblk4(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == actor);

    reset_lrblk4_state();
    actor = &actwk[4];
    actor->userflag.b.h = 2;
    actor->actfree[18] = 1;
    actor->xposi.w.h = 300;
    actor->actfree[16] = 64;

    lrblk4(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 32, actor->xposi.w.h);
    assert_action_only(ctx, actor);

    reset_lrblk4_state();
    actor = &actwk[4];
    actor->userflag.b.h = 3;
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 100;

    lrblk4(actor);

    TEST_ASSERT_EQ_INT(ctx, 5, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 36, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 3, actwk[20].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[20].actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 300, actor_short_alias(&actwk[20], 29));
    TEST_ASSERT_EQ_INT(ctx, 300, actor->xposi.w.h);

    reset_logs();
    actor->r_no0 = 2;
    actor->xposi.w.h = 70;
    actor->xspeed.w = -128;
    set_actor_short_alias(actor, 29, 300);

    lrblk4(actor);

    TEST_ASSERT_EQ_INT(ctx, 524, actor->xposi.w.h);

    reset_logs();
    actor->xposi.w.h = 530;
    actor->xspeed.w = 128;
    set_actor_short_alias(actor, 29, 300);

    lrblk4(actor);

    TEST_ASSERT_EQ_INT(ctx, 76, actor->xposi.w.h);
}

static void test_type4_type5_type6_type8_paths(test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_lrblk4_state();
    actor->userflag.b.h = 4;
    actor->xposi.w.h = 400;
    actor->yposi.w.h = 80;
    actor->actfree[19] = 1;

    lrblk4(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_TRUE(ctx, actor->patbase == lrblk4pat8);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 272, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -512, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 36, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[20].actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 528, actwk[20].xposi.w.h);

    reset_logs();
    actor->r_no0 = 2;
    actor->xposi.w.h = 146;
    set_actor_short_alias(actor, 29, 400);

    lrblk4(actor);

    TEST_ASSERT_EQ_INT(ctx, 656, actor->xposi.w.h);

    reset_lrblk4_state();
    actor = &actwk[5];
    actor->userflag.b.h = 5;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 40;

    lrblk4(actor);

    TEST_ASSERT_TRUE(ctx, actor->patbase == lrblk4pat5);
    TEST_ASSERT_EQ_INT(ctx, 4, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 0, actor_short_alias(actor, 26));
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk_count);

    reset_logs();
    actor->r_no0 = 2;
    actor->actfree[16] = 1;
    actor->actfree[17] = 5;
    set_actor_short_alias(actor, 26, -128);

    lrblk4(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 128, actor->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 128, actor_short_alias(actor, 26));

    reset_logs();
    actor->r_no0 = 2;
    actor->actfree[16] = 1;
    actor->actfree[17] = 0;
    set_actor_short_alias(actor, 26, 128);

    lrblk4(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 128, actor->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 128, actor_short_alias(actor, 26));

    reset_lrblk4_state();
    actor = &actwk[5];
    actor->userflag.b.h = 6;
    actor->xposi.w.h = 500;
    actor->yposi.w.h = 120;
    actor->actfree[18] = 2;

    lrblk4(actor);

    TEST_ASSERT_TRUE(ctx, actor->patbase == lrblk4pat6);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 48, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 548, actor_short_alias(actor, 27));
    TEST_ASSERT_EQ_INT(ctx, 596, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);

    reset_lrblk4_state();
    actor = &actwk[5];
    actor->userflag.b.h = 6;
    actor->xposi.w.h = 500;
    actor->yposi.w.h = 120;

    lrblk4(actor);

    TEST_ASSERT_EQ_INT(ctx, 3, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 36, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[20].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[20].actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 484, actwk[20].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 152, actwk[20].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 532, actor->xposi.w.h);

    reset_lrblk4_state();
    actor = &actwk[5];
    actor->userflag.b.h = 8;
    actor->xposi.w.h = 600;
    actor->yposi.w.h = 200;

    lrblk4(actor);

    TEST_ASSERT_TRUE(ctx, actor->patbase == lrblk4pat8);
    TEST_ASSERT_EQ_INT(ctx, 728, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 512, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 36, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[20].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 8, actwk[20].actfree[19]);
    TEST_ASSERT_EQ_INT(ctx, 36, actwk[21].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[21].actfree[18]);

    reset_logs();
    actor->r_no0 = 2;
    actor->xposi.w.h = 854;
    set_actor_short_alias(actor, 29, 600);

    lrblk4(actor);

    TEST_ASSERT_EQ_INT(ctx, 344, actor->xposi.w.h);
}

TEST_MAIN_BEGIN;
test_tables_capture_literal_data(&ctx);
test_type1_master_child_and_fout_paths(&ctx);
test_type2_and_type3_paths(&ctx);
test_type4_type5_type6_type8_paths(&ctx);
TEST_MAIN_END
