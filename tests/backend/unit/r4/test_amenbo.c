#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Sint16 waterposi;

static Sint16 enemy_suicide_result;
static int enemy_suicide_count;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int soundset_count;
static Sint16 soundset_last;

Sint16 enemy_suicide(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void soundset(Sint16 ReqNo);

#include "src/r4/amenbo.c"

Sint16 enemy_suicide(sprite_status *pActwk) {
    (void)pActwk;
    ++enemy_suicide_count;
    return enemy_suicide_result;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
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

void soundset(Sint16 ReqNo) {
    ++soundset_count;
    soundset_last = ReqNo;
}

static size_t short_alias_offset(int short_index) {
    return (size_t)short_index * sizeof(Sint16) -
           offsetof(sprite_status, actfree);
}

static size_t long_alias_offset(int long_index) {
    return (size_t)long_index * sizeof(Sint32) -
           offsetof(sprite_status, actfree);
}

static void set_actor_short_alias(sprite_status *actor, int short_index,
                                  Sint16 value) {
    size_t offset = short_alias_offset(short_index);
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static Sint16 actor_short_alias(sprite_status *actor, int short_index) {
    size_t offset = short_alias_offset(short_index);
    return (Sint16)((Uint16)actor->actfree[offset] |
                    ((Uint16)actor->actfree[offset + 1] << 8));
}

static void set_actor_long_alias(sprite_status *actor, int long_index,
                                 Sint32 value) {
    size_t offset = long_alias_offset(long_index);
    Uint32 raw = (Uint32)value;
    actor->actfree[offset] = (Uint8)raw;
    actor->actfree[offset + 1] = (Uint8)(raw >> 8);
    actor->actfree[offset + 2] = (Uint8)(raw >> 16);
    actor->actfree[offset + 3] = (Uint8)(raw >> 24);
}

static Sint32 actor_long_alias(sprite_status *actor, int long_index) {
    size_t offset = long_alias_offset(long_index);
    Uint32 raw = actor->actfree[offset] |
                 ((Uint32)actor->actfree[offset + 1] << 8) |
                 ((Uint32)actor->actfree[offset + 2] << 16) |
                 ((Uint32)actor->actfree[offset + 3] << 24);
    return (Sint32)raw;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void reset_amenbo_state(void) {
    memset(actwk, 0, sizeof(actwk));
    waterposi = 0;
    enemy_suicide_result = 0;
    enemy_suicide_count = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    soundset_count = 0;
    soundset_last = 0;
}

static void test_amenbo_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pchg_b[0] == pchg_b0);
    TEST_ASSERT_TRUE(ctx, pchg_e[0] == pchg_e0);
    TEST_ASSERT_TRUE(ctx, pchg_e[1] == pchg_e1);
    TEST_ASSERT_EQ_INT(ctx, 20, pchg_b0[0]);
    TEST_ASSERT_EQ_INT(ctx, 255, pchg_b0[3]);
    TEST_ASSERT_EQ_INT(ctx, 10, pchg_e0[0]);
    TEST_ASSERT_EQ_INT(ctx, 5, pchg_e1[0]);
    TEST_ASSERT_TRUE(ctx, pat_amenbo_b[0] == &spr_amenbo_b_00);
    TEST_ASSERT_TRUE(ctx, pat_amenbo_e[3] == &spr_amenbo_e_03);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_AMENBO_BASE, spr_amenbo_b_00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_AMENBO_BASE + 5, spr_amenbo_e_03.spra[0].index);
}

static void test_projectile_branch_uses_parent_speed_and_palette(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_amenbo_state();
    actor->userflag.b.h = -1;
    actor->xposi.l = 10 << 16;
    set_actor_long_alias(actor, 12, 0x20000);
    set_actor_long_alias(actor, 15, 0);

    amenbo(actor);

    TEST_ASSERT_EQ_INT(ctx, 12, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg_b);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);

    reset_amenbo_state();
    actor = &actwk[4];
    actor->userflag.b.h = -1;
    set_actor_long_alias(actor, 15, 1);

    amenbo(actor);

    TEST_ASSERT_TRUE(ctx, patchg_table == pchg_e);
}

static void test_enemy_suicide_skips_normal_actor(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_amenbo_state();
    enemy_suicide_result = 1;

    amenbo(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

static void test_init_variants_and_regular_acceleration(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_amenbo_state();
    actor->actno = 41;
    actor->xposi.w.h = 200;
    actor->yposi.w.h = 40;
    waterposi = 96;

    amenbo(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 9136, actor->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 20, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 20, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 12, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 49, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 119, actor_short_alias(actor, 32));
    TEST_ASSERT_EQ_INT(ctx, 200, actor_short_alias(actor, 33));
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_amenbo_e);
    TEST_ASSERT_EQ_INT(ctx, -1536, actor_long_alias(actor, 12));
    TEST_ASSERT_EQ_INT(ctx, -1536, actor_long_alias(actor, 13));
    TEST_ASSERT_EQ_INT(ctx, 256, actor_long_alias(actor, 14));
    TEST_ASSERT_EQ_INT(ctx, 1, actor_long_alias(actor, 15));
    TEST_ASSERT_EQ_INT(ctx, 88, actor->yposi.w.h);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg_e);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_amenbo_state();
    actor = &actwk[4];
    actor->userflag.b.h = 1;
    actor->xposi.w.h = 120;
    waterposi = 64;

    amenbo(actor);

    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_amenbo_b);
    TEST_ASSERT_EQ_INT(ctx, -512, actor_long_alias(actor, 12));
    TEST_ASSERT_EQ_INT(ctx, -512, actor_long_alias(actor, 13));
    TEST_ASSERT_EQ_INT(ctx, 192, actor_long_alias(actor, 14));
    TEST_ASSERT_EQ_INT(ctx, 0, actor_long_alias(actor, 15));
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg_b);
    TEST_ASSERT_EQ_INT(ctx, 56, actor->yposi.w.h);
}

static void test_accel_clamps_at_negative_and_positive_limits(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_amenbo_state();
    actor->r_no0 = 2;
    actor->userflag.b.h = 1;
    set_actor_long_alias(actor, 12, -49000);
    set_actor_long_alias(actor, 13, -512);
    waterposi = 24;

    amenbo(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, -49152, actor_long_alias(actor, 12));
    TEST_ASSERT_EQ_INT(ctx, 512, actor_long_alias(actor, 13));
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg_b);

    reset_amenbo_state();
    actor = &actwk[4];
    actor->r_no0 = 2;
    actor->userflag.b.h = 1;
    set_actor_long_alias(actor, 12, 49000);
    set_actor_long_alias(actor, 13, 512);
    waterposi = 24;

    amenbo(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 49152, actor_long_alias(actor, 12));
    TEST_ASSERT_EQ_INT(ctx, -512, actor_long_alias(actor, 13));
}

static void test_decel_crosses_zero_and_continues_otherwise(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_amenbo_state();
    actor->r_no0 = 4;
    set_actor_long_alias(actor, 12, -100);
    set_actor_long_alias(actor, 14, 192);
    waterposi = 50;

    amenbo(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor_long_alias(actor, 12));
    TEST_ASSERT_EQ_INT(ctx, -192, actor_long_alias(actor, 14));
    TEST_ASSERT_EQ_INT(ctx, 60, actor_short_alias(actor, 23));
    TEST_ASSERT_EQ_INT(ctx, 42, actor->yposi.w.h);

    reset_amenbo_state();
    actor = &actwk[4];
    actor->r_no0 = 4;
    actor->userflag.b.h = 1;
    set_actor_long_alias(actor, 12, -1000);
    set_actor_long_alias(actor, 14, 192);
    waterposi = 50;

    amenbo(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, -808, actor_long_alias(actor, 12));

    reset_amenbo_state();
    actor = &actwk[4];
    actor->r_no0 = 4;
    actor->userflag.b.h = 1;
    set_actor_long_alias(actor, 12, -100);
    set_actor_long_alias(actor, 14, 192);
    waterposi = 50;

    amenbo(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor_long_alias(actor, 12));
    TEST_ASSERT_EQ_INT(ctx, 60, actor_short_alias(actor, 23));
}

static void test_stop_and_stop1_timers(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_amenbo_state();
    actor->r_no0 = 6;
    actor->actflg = 4;
    actor->cddat = 0;
    set_actor_short_alias(actor, 23, 1);
    waterposi = 70;

    amenbo(actor);

    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->cddat);
    TEST_ASSERT_EQ_INT(ctx, 60, actor_short_alias(actor, 23));
    TEST_ASSERT_EQ_INT(ctx, 62, actor->yposi.w.h);

    reset_amenbo_state();
    actor = &actwk[4];
    actor->r_no0 = 6;
    actor->userflag.b.h = 1;
    set_actor_short_alias(actor, 23, 1);
    waterposi = 70;

    amenbo(actor);

    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 60, actor_short_alias(actor, 23));

    reset_amenbo_state();
    actor = &actwk[4];
    actor->r_no0 = 8;
    set_actor_short_alias(actor, 23, 1);
    waterposi = 70;

    amenbo(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 62, actor->yposi.w.h);
}

static void test_projectile_spawn_success_failure_and_sound(test_context *ctx) {
    sprite_status *actor = &actwk[4];
    sprite_status *shot = &actwk[5];

    reset_amenbo_state();
    actor->actno = 41;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 80;
    actor->actflg = 128;
    actor->sprpri = 3;
    actor->sproffset = 9136;
    actor->patbase = pat_amenbo_e;
    set_actor_short_alias(actor, 32, 1);
    set_actor_long_alias(actor, 15, 1);
    actwk[0].xposi.w.h = 90;
    actwk[0].yposi.w.h = 80;
    queue_actor(shot);

    act_tama(actor);

    TEST_ASSERT_EQ_INT(ctx, 120, actor_short_alias(actor, 32));
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 41, shot->actno);
    TEST_ASSERT_EQ_INT(ctx, -1, shot->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 4, actor_short_alias(shot, 44));
    TEST_ASSERT_EQ_INT(ctx, 100, shot->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 80, shot->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 128, shot->actflg);
    TEST_ASSERT_EQ_INT(ctx, 4, shot->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 9136, shot->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 4, shot->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 4, shot->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 4, shot->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 178, shot->colino);
    TEST_ASSERT_TRUE(ctx, shot->patbase == pat_amenbo_e);
    TEST_ASSERT_EQ_INT(ctx, 1, actor_long_alias(shot, 15));
    TEST_ASSERT_EQ_INT(ctx, 1, shot->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, -0x20000, actor_long_alias(shot, 12));
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 160, soundset_last);

    reset_amenbo_state();
    actor = &actwk[4];
    set_actor_short_alias(actor, 32, 1);
    actwk[0].xposi.w.h = 1000;
    actwk[0].yposi.w.h = 1000;

    act_tama(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    reset_amenbo_state();
    actor = &actwk[4];
    actor->actflg = 1;
    set_actor_short_alias(actor, 32, 1);
    actwk[0].xposi.w.h = 10;
    actwk[0].yposi.w.h = 0;
    queue_actor(&actwk[5]);

    act_tama(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0x20000, actor_long_alias(&actwk[5], 12));
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    reset_amenbo_state();
    actor = &actwk[4];
    actor->xposi.w.h = 100;
    set_actor_short_alias(actor, 32, 1);
    actwk[0].xposi.w.h = 90;
    actwk[0].yposi.w.h = 0;

    act_tama(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
}

TEST_MAIN_BEGIN;
test_amenbo_tables_capture_literal_data(&ctx);
test_projectile_branch_uses_parent_speed_and_palette(&ctx);
test_enemy_suicide_skips_normal_actor(&ctx);
test_init_variants_and_regular_acceleration(&ctx);
test_accel_clamps_at_negative_and_positive_limits(&ctx);
test_decel_crosses_zero_and_continues_otherwise(&ctx);
test_stop_and_stop1_timers(&ctx);
test_projectile_spawn_success_failure_and_sound(&ctx);
TEST_MAIN_END
