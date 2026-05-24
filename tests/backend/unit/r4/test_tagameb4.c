#include <stddef.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static Sint16 enemy_suicide_result;
static int enemy_suicide_count;
static int actwkchk_count;
static int actwkchk_fail_after;
static int next_alloc_index;
static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int soundset_count;
static Sint16 soundset_no;

Sint16 enemy_suicide(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void soundset(Sint16 ReqNo);

#include "src/r4/tagameb4.c"

Sint16 enemy_suicide(sprite_status *pActwk) {
    (void)pActwk;
    ++enemy_suicide_count;
    return enemy_suicide_result;
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

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

void soundset(Sint16 ReqNo) {
    ++soundset_count;
    soundset_no = ReqNo;
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
    Uint16 bits = (Uint16)value;
    actor->actfree[offset] = (Uint8)bits;
    actor->actfree[offset + 1] = (Uint8)(bits >> 8);
}

static Sint16 actor_short_alias(sprite_status *actor, int short_index) {
    size_t offset = short_alias_offset(short_index);
    return (Sint16)((Uint16)actor->actfree[offset] |
                    ((Uint16)actor->actfree[offset + 1] << 8));
}

static void set_actor_long_alias(sprite_status *actor, int long_index,
                                 Sint32 value) {
    size_t offset = long_alias_offset(long_index);
    Uint32 bits = (Uint32)value;
    actor->actfree[offset] = (Uint8)bits;
    actor->actfree[offset + 1] = (Uint8)(bits >> 8);
    actor->actfree[offset + 2] = (Uint8)(bits >> 16);
    actor->actfree[offset + 3] = (Uint8)(bits >> 24);
}

static Sint32 actor_long_alias(sprite_status *actor, int long_index) {
    size_t offset = long_alias_offset(long_index);
    Uint32 bits = actor->actfree[offset] |
                  ((Uint32)actor->actfree[offset + 1] << 8) |
                  ((Uint32)actor->actfree[offset + 2] << 16) |
                  ((Uint32)actor->actfree[offset + 3] << 24);
    return (Sint32)bits;
}

static void set_actfree_long(sprite_status *actor, int offset, Sint32 value) {
    Uint32 bits = (Uint32)value;
    actor->actfree[offset] = (Uint8)bits;
    actor->actfree[offset + 1] = (Uint8)(bits >> 8);
    actor->actfree[offset + 2] = (Uint8)(bits >> 16);
    actor->actfree[offset + 3] = (Uint8)(bits >> 24);
}

static Sint32 actfree_long(sprite_status *actor, int offset) {
    Uint32 bits = actor->actfree[offset] |
                  ((Uint32)actor->actfree[offset + 1] << 8) |
                  ((Uint32)actor->actfree[offset + 2] << 16) |
                  ((Uint32)actor->actfree[offset + 3] << 24);
    return (Sint32)bits;
}

static void reset_logs(void) {
    enemy_suicide_count = 0;
    actwkchk_count = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    soundset_count = 0;
    soundset_no = 0;
}

static void reset_tagameb_state(void) {
    memset(actwk, 0, sizeof(actwk));
    enemy_suicide_result = 0;
    actwkchk_fail_after = -1;
    next_alloc_index = 20;
    reset_logs();
}

static void assert_displayed(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void test_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pchg[0] == pchg0);
    TEST_ASSERT_EQ_INT(ctx, 30, pchg0[0]);
    TEST_ASSERT_EQ_INT(ctx, 255, pchg0[3]);
    TEST_ASSERT_TRUE(ctx, pat_tagameb_b[0] == &spr_tagame_b_00);
    TEST_ASSERT_TRUE(ctx, pat_tagameb_e[1] == &spr_tagame_e_01);
    TEST_ASSERT_TRUE(ctx, pat[0] == &spr_tagame_99);
    TEST_ASSERT_EQ_INT(ctx, -20, spr_tagame_b_00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -12, spr_tagame_b_00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 448, spr_tagame_b_00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 452, spr_tagame_99.spra[0].index);
}

static void test_suicide_short_circuits_master(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_tagameb_state();
    enemy_suicide_result = 1;

    tagameb(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

static void test_master_init_variants(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_tagameb_state();
    actor->actno = 45;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    actor->cdsts = 9;

    tagameb(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 9168, actor->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 20, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 20, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 45, actor->colino);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_tagameb_e);
    TEST_ASSERT_EQ_INT(ctx, -65536, actor_long_alias(actor, 12));
    TEST_ASSERT_EQ_INT(ctx, 200, actor_short_alias(actor, 29));
    TEST_ASSERT_EQ_INT(ctx, 100, actor_short_alias(actor, 30));
    TEST_ASSERT_EQ_INT(ctx, 3, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 20, actor_short_alias(actor, 26));
    TEST_ASSERT_EQ_INT(ctx, 21, actor_short_alias(actor, 27));
    TEST_ASSERT_EQ_INT(ctx, 22, actor_short_alias(actor, 28));
    TEST_ASSERT_EQ_INT(ctx, 45, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, -1, actwk[20].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 9, actwk[20].actfree[19]);
    TEST_ASSERT_EQ_INT(ctx, 3, actor_short_alias(&actwk[20], 33));
    TEST_ASSERT_EQ_INT(ctx, 102, actwk[20].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 190, actwk[20].yposi.w.h);
    assert_displayed(ctx, actor);

    reset_tagameb_state();
    actor = &actwk[3];
    actor->actno = 45;
    actor->userflag.b.h = 1;
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 400;

    tagameb(actor);

    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_tagameb_b);
    TEST_ASSERT_EQ_INT(ctx, -32768, actor_long_alias(actor, 12));
    TEST_ASSERT_EQ_INT(ctx, 400, actor_short_alias(actor, 29));
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    assert_displayed(ctx, actor);
}

static void test_make_toge_allocation_failures_frameout(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    for (int fail_after = 0; fail_after <= 2; ++fail_after) {
        reset_tagameb_state();
        actor = &actwk[3];
        actor->actno = 45;
        actwkchk_fail_after = fail_after;

        tagameb(actor);

        TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
        TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    }
}

static void test_master_move_check_reverse_and_patch(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_tagameb_state();
    actor->actno = 45;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    tagameb(actor);
    reset_logs();

    actwk[0].xposi.w.h = 170;
    actwk[0].yposi.w.h = 100;

    tagameb(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 60, actor_short_alias(actor, 23));
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    assert_displayed(ctx, actor);

    reset_tagameb_state();
    actor = &actwk[3];
    actor->userflag.b.h = 1;
    actor->actflg = 4;
    actor->r_no0 = 2;
    actor->xposi.l = 100 << 16;
    set_actor_long_alias(actor, 12, -32768);
    set_actor_short_alias(actor, 23, 1);
    set_actor_short_alias(actor, 29, 400);

    tagameb(actor);

    TEST_ASSERT_EQ_INT(ctx, 99, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -32768, actor_long_alias(actor, 12));
    TEST_ASSERT_EQ_INT(ctx, 0, actor_short_alias(actor, 23));
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg);

    reset_tagameb_state();
    actor = &actwk[3];
    actor->userflag.b.h = 1;
    actor->actflg = 4;
    actor->r_no0 = 2;
    set_actor_long_alias(actor, 12, -32768);
    set_actor_short_alias(actor, 23, -1);
    set_actor_short_alias(actor, 29, 400);

    tagameb(actor);

    TEST_ASSERT_EQ_INT(ctx, 400, actor_short_alias(actor, 23));
    TEST_ASSERT_EQ_INT(ctx, 32768, actor_long_alias(actor, 12));
    TEST_ASSERT_EQ_INT(ctx, 5, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->cddat);

    reset_tagameb_state();
    actor = &actwk[3];
    actor->r_no0 = 2;
    actor->xposi.l = 100 << 16;
    actor->yposi.w.h = 200;
    actor->actflg = 4;
    set_actor_short_alias(actor, 23, 1);
    set_actor_short_alias(actor, 26, 20);
    set_actor_short_alias(actor, 27, 21);
    set_actor_short_alias(actor, 28, 22);
    set_actor_long_alias(actor, 12, -65536);
    actwk[20].xposi.l = 10 << 16;
    actwk[21].xposi.l = 20 << 16;
    actwk[22].xposi.l = 30 << 16;
    actwk[0].xposi.w.h = 300;
    actwk[0].yposi.w.h = 100;

    tagameb(actor);

    TEST_ASSERT_EQ_INT(ctx, 99, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 9, actwk[20].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 19, actwk[21].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 29, actwk[22].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actor_short_alias(actor, 23));
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);

    reset_tagameb_state();
    actor = &actwk[3];
    actor->r_no0 = 2;
    actor->xposi.l = 100 << 16;
    actor->actflg = 4;
    set_actor_short_alias(actor, 23, -1);
    set_actor_short_alias(actor, 26, 20);
    set_actor_short_alias(actor, 27, 21);
    set_actor_short_alias(actor, 28, 22);
    set_actor_short_alias(actor, 29, 200);
    set_actor_long_alias(actor, 12, -65536);
    actwk[0].xposi.w.h = 300;
    actwk[0].yposi.w.h = 100;

    tagameb(actor);

    TEST_ASSERT_EQ_INT(ctx, 200, actor_short_alias(actor, 23));
    TEST_ASSERT_EQ_INT(ctx, 5, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 98, actwk[20].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 93, actwk[21].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 89, actwk[22].xposi.w.h);

    reset_tagameb_state();
    actor = &actwk[3];
    actor->r_no0 = 2;
    actor->xposi.l = 100 << 16;
    actor->actflg = 5;
    set_actor_short_alias(actor, 23, -1);
    set_actor_short_alias(actor, 26, 20);
    set_actor_short_alias(actor, 27, 21);
    set_actor_short_alias(actor, 28, 22);
    set_actor_short_alias(actor, 29, 200);
    set_actor_long_alias(actor, 12, -65536);
    actwk[0].xposi.w.h = 300;
    actwk[0].yposi.w.h = 100;

    tagameb(actor);

    TEST_ASSERT_EQ_INT(ctx, 200, actor_short_alias(actor, 23));
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 97, actwk[20].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 102, actwk[21].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 106, actwk[22].xposi.w.h);
}

static void test_stop_stop1_and_dash_paths(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_tagameb_state();
    actor->userflag.b.h = 1;
    actor->r_no0 = 4;
    actor->actflg = 128;
    set_actor_short_alias(actor, 23, -1);

    tagameb(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 60, actor_short_alias(actor, 23));
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    assert_displayed(ctx, actor);

    reset_tagameb_state();
    actor = &actwk[3];
    actor->r_no0 = 4;
    set_actor_short_alias(actor, 23, -1);
    set_actor_short_alias(actor, 26, 20);
    set_actor_short_alias(actor, 27, 21);
    set_actor_short_alias(actor, 28, 22);

    tagameb(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, -0x20000, actfree_long(&actwk[20], 0));
    TEST_ASSERT_EQ_INT(ctx, -196608, actfree_long(&actwk[20], 4));
    TEST_ASSERT_EQ_INT(ctx, 0, actfree_long(&actwk[20], 8));
    TEST_ASSERT_EQ_INT(ctx, 8192, actfree_long(&actwk[20], 12));
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[20].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 65536, actfree_long(&actwk[21], 0));
    TEST_ASSERT_EQ_INT(ctx, 0x20000, actfree_long(&actwk[22], 0));

    reset_tagameb_state();
    actor = &actwk[3];
    actor->userflag.b.h = 1;
    actor->r_no0 = 6;
    actor->actflg = 0;
    set_actor_short_alias(actor, 23, -1);

    tagameb(actor);

    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, -98304, actor_long_alias(actor, 12));

    reset_tagameb_state();
    actor = &actwk[3];
    actor->userflag.b.h = 0;
    actor->r_no0 = 6;
    actor->actflg = 1;
    set_actor_short_alias(actor, 23, -1);

    tagameb(actor);

    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 262144, actor_long_alias(actor, 12));

    reset_tagameb_state();
    actor = &actwk[3];
    actor->r_no0 = 8;
    actor->xposi.l = 50 << 16;
    set_actor_long_alias(actor, 12, 0x40000);

    tagameb(actor);

    TEST_ASSERT_EQ_INT(ctx, 54, actor->xposi.w.h);
    assert_displayed(ctx, actor);
}

static void test_child_follow_move_and_frameout_paths(test_context *ctx) {
    sprite_status *child = &actwk[5];
    sprite_status *parent = &actwk[3];

    reset_tagameb_state();
    parent->actno = 0;
    child->userflag.b.h = -1;
    set_actor_short_alias(child, 33, 3);

    tagameb(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);

    reset_tagameb_state();
    child = &actwk[5];
    parent = &actwk[3];
    parent->actno = 45;
    parent->cdsts = 7;
    child->userflag.b.h = -1;
    child->actfree[19] = 8;
    set_actor_short_alias(child, 33, 3);

    tagameb(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);

    reset_tagameb_state();
    child = &actwk[5];
    parent = &actwk[3];
    parent->actno = 45;
    parent->cdsts = 7;
    child->userflag.b.h = -1;
    child->actfree[19] = 7;
    set_actor_short_alias(child, 33, 3);

    tagameb(child);

    TEST_ASSERT_EQ_INT(ctx, 2, child->r_no0);
    TEST_ASSERT_TRUE(ctx, child->patbase == pat);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == child);

    reset_logs();
    child->r_no0 = 4;
    child->xposi.l = 10 << 16;
    child->yposi.l = 20 << 16;
    set_actfree_long(child, 0, 65536);
    set_actfree_long(child, 4, 131072);
    set_actfree_long(child, 8, 4096);
    set_actfree_long(child, 12, 8192);
    actwk[0].yposi.w.h = 0;

    tagameb(child);

    TEST_ASSERT_EQ_INT(ctx, 11, child->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 22, child->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 69632, actfree_long(child, 0));
    TEST_ASSERT_EQ_INT(ctx, 139264, actfree_long(child, 4));
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_logs();
    child->yposi.w.h = 300;
    actwk[0].yposi.w.h = 0;

    tagameb(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
}

TEST_MAIN_BEGIN;
test_tables_capture_literal_data(&ctx);
test_suicide_short_circuits_master(&ctx);
test_master_init_variants(&ctx);
test_make_toge_allocation_failures_frameout(&ctx);
test_master_move_check_reverse_and_patch(&ctx);
test_stop_stop1_and_dash_paths(&ctx);
test_child_follow_move_and_frameout_paths(&ctx);
TEST_MAIN_END
