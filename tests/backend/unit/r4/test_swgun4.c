#include <stddef.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
int_union pltime;
Sint16 scralim_down;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_x;
static int frameout_count;
static sprite_status *frameout_actor;
static int actwkchk_count;
static int next_alloc_index;
static int actwkchk_fail_after;
static int hitchk_u_count;
static Sint16 hitchk_u_result;
static sprite_status *hitchk_u_actor;
static int soundset_count;
static Sint16 soundset_no;
static int ride_on_clr_count;
static sprite_status *ride_on_clr_actor;
static int scoreup_count;
static Uint32 scoreup_value;
static int speedset2_count;
static sprite_status *speedset2_actor;
static int emycol_d_count;
static Sint16 emycol_d_result;
static int ringgetsub_count;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
void frameout(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
Sint16 hitchk_u(sprite_status *pActwk, sprite_status *pPlayerwk);
void soundset(Sint16 ReqNo);
Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk);
void scoreup(Uint32 updata);
void speedset2(sprite_status *pActwk);
Sint16 emycol_d(sprite_status *pActwk);
void ringgetsub(void);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);

#include "src/r4/swgun4.c"

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

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
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

Sint16 hitchk_u(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++hitchk_u_count;
    hitchk_u_actor = pActwk;
    (void)pPlayerwk;
    return hitchk_u_result;
}

void soundset(Sint16 ReqNo) {
    ++soundset_count;
    soundset_no = ReqNo;
}

Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_clr_count;
    ride_on_clr_actor = pActwk;
    (void)pPlayerwk;
    return 0;
}

void scoreup(Uint32 updata) {
    ++scoreup_count;
    scoreup_value = updata;
}

void speedset2(sprite_status *pActwk) {
    ++speedset2_count;
    speedset2_actor = pActwk;
    pActwk->yposi.l += (Sint32)pActwk->yspeed.w << 8;
}

Sint16 emycol_d(sprite_status *pActwk) {
    (void)pActwk;
    ++emycol_d_count;
    return emycol_d_result;
}

void ringgetsub(void) { ++ringgetsub_count; }

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
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
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_x = 0;
    frameout_count = 0;
    frameout_actor = 0;
    actwkchk_count = 0;
    hitchk_u_count = 0;
    hitchk_u_result = 0;
    hitchk_u_actor = 0;
    soundset_count = 0;
    soundset_no = 0;
    ride_on_clr_count = 0;
    ride_on_clr_actor = 0;
    scoreup_count = 0;
    scoreup_value = 0;
    speedset2_count = 0;
    speedset2_actor = 0;
    emycol_d_count = 0;
    emycol_d_result = 1;
    ringgetsub_count = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
}

static void reset_swgun4_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(&pltime, 0, sizeof(pltime));
    scralim_down = 0;
    next_alloc_index = 20;
    actwkchk_fail_after = -1;
    reset_logs();
}

static void assert_swgun_action(test_context *ctx, sprite_status *actor,
                                Sint16 origin) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, origin, frameout_s00_x);
}

static void test_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, swgun4pat[0] == &pat0);
    TEST_ASSERT_TRUE(ctx, swgun4pat[1] == &pat1);
    TEST_ASSERT_TRUE(ctx, swgun4pat[2] == &pat2);
    TEST_ASSERT_EQ_INT(ctx, -16, pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -8, pat0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 481, pat0.spra[0].index);
    TEST_ASSERT_TRUE(ctx, ringpat[0] == &ringsp0);
    TEST_ASSERT_TRUE(ctx, ringpat[8] == &ringsp8);
    TEST_ASSERT_EQ_INT(ctx, 362, ringsp0.spra[0].index);
    TEST_ASSERT_TRUE(ctx, ringchg[0] == pchg0);
    TEST_ASSERT_EQ_INT(ctx, 5, pchg0[0]);
    TEST_ASSERT_EQ_INT(ctx, 252, pchg0[5]);
    TEST_ASSERT_TRUE(ctx, tensu4pat[0] == &pat00);
    TEST_ASSERT_EQ_INT(ctx, 484, pat00.spra[0].index);
}

static void test_swgun4_init_spawns_children(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_swgun4_state();
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 500;

    swgun4(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1290, actor->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->sprpri);
    TEST_ASSERT_TRUE(ctx, actor->patbase == swgun4pat);
    TEST_ASSERT_EQ_INT(ctx, 14, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 300, actor_short_alias(actor, 29));
    TEST_ASSERT_EQ_INT(ctx, 500, actor_short_alias(actor, 27));
    TEST_ASSERT_EQ_INT(ctx, 3, actor_short_alias(actor, 31));
    TEST_ASSERT_EQ_INT(ctx, 5, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 63, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[20].actfree[10]);
    TEST_ASSERT_EQ_INT(ctx, 236, actwk[20].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 564, actwk[20].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 63, actwk[24].actno);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[24].actfree[10]);
    TEST_ASSERT_EQ_INT(ctx, 428, actwk[24].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 372, actwk[24].yposi.w.h);
    assert_swgun_action(ctx, actor, 300);

    reset_swgun4_state();
    actor = &actwk[3];
    actor->actfree[10] = 2;

    swgun4(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
}

static void test_switch_move_press_release_and_fire(test_context *ctx) {
    sprite_status *actor = &actwk[4];
    sprite_status *master = &actwk[3];

    reset_swgun4_state();
    set_actor_short_alias(actor, 31, 3);
    actor->r_no0 = 2;
    actor->sprvsize = 8;
    actor->actfree[10] = 1;
    master->actno = 63;
    pltime.b.b3 = 20;
    hitchk_u_result = 1;

    swgun4(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_u_count);
    TEST_ASSERT_TRUE(ctx, hitchk_u_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 255, actor->actfree[7]);
    TEST_ASSERT_EQ_INT(ctx, 4, actor_short_alias(master, 32));
    TEST_ASSERT_EQ_INT(ctx, 3, actor->actfree[11]);
    TEST_ASSERT_EQ_INT(ctx, 3, master->actfree[11]);
    TEST_ASSERT_EQ_INT(ctx, 8, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 191, soundset_no);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->actfree[5]);
    assert_swgun_action(ctx, actor, 0);

    reset_logs();
    actor->actfree[6] = 255;
    actor->actfree[7] = 255;
    hitchk_u_result = 0;

    swgun4(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[7]);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->yposi.w.h);

    reset_swgun4_state();
    actor = &actwk[4];
    master = &actwk[3];
    set_actor_short_alias(actor, 31, 3);
    actor->r_no0 = 2;
    master->actno = 63;

    swgun4(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[5]);

    reset_swgun4_state();
    actor = &actwk[4];
    master = &actwk[3];
    set_actor_short_alias(actor, 31, 3);
    actor->r_no0 = 2;
    actor->actfree[10] = 1;
    actor->sprvsize = 8;
    master->actno = 63;
    hitchk_u_result = 1;
    actwk[0].yspeed.w = -1;

    swgun4(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor_short_alias(master, 32));
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    reset_swgun4_state();
    actor = &actwk[4];
    master = &actwk[3];
    set_actor_short_alias(actor, 31, 3);
    actor->r_no0 = 2;
    actor->actfree[4] = 255;
    actor->sprvsize = 8;
    master->actno = 63;
    hitchk_u_result = 1;

    swgun4(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor_short_alias(master, 32));
}

static void test_swgun4_fire_threshold_and_dispatches(test_context *ctx) {
    sprite_status *actor = &actwk[4];
    sprite_status *master = &actwk[3];

    reset_swgun4_state();
    set_actor_short_alias(actor, 31, 3);
    actor->r_no0 = 2;
    actor->actfree[4] = 255;
    actor->actfree[5] = 240;
    set_actor_short_alias(master, 32, 4);

    swgun4(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[14]);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[15]);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->patno);

    reset_logs();
    master->actfree[11] = 0;
    set_actor_short_alias(master, 32, 5);
    actor->r_no0 = 4;

    swgun4(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 34, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[14]);

    reset_swgun4_state();
    actor = &actwk[4];
    master = &actwk[3];
    set_actor_short_alias(actor, 31, 3);
    set_actor_short_alias(master, 32, 4);
    master->actfree[11] = 1;
    actor->r_no0 = 4;

    swgun4(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, scoreup_count);
    TEST_ASSERT_EQ_INT(ctx, 100, scoreup_value);
    TEST_ASSERT_EQ_INT(ctx, 65, actwk[20].actno);

    reset_swgun4_state();
    actor = &actwk[4];
    master = &actwk[3];
    set_actor_short_alias(actor, 31, 3);
    set_actor_short_alias(master, 32, 5);
    master->actfree[11] = 2;
    actor->r_no0 = 4;
    actor->actfree[15] = 252;

    swgun4(actor);

    TEST_ASSERT_EQ_INT(ctx, 32, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[14]);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_swgun4_state();
    actor = &actwk[3];
    set_actor_short_alias(actor, 31, 3);
    actor->r_no0 = 4;
    actor->xposi.w.h = 500;
    actor->yposi.w.h = 600;
    actor->actfree[14] = 240;
    actor->actfree[15] = 0;
    actor->actfree[11] = 3;

    swgun4(actor);

    TEST_ASSERT_EQ_INT(ctx, 64, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 468, actwk[20].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 216, actwk[20].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[15]);

    reset_swgun4_state();
    actor = &actwk[4];
    master = &actwk[3];
    set_actor_short_alias(actor, 31, 3);
    set_actor_short_alias(master, 32, 4);
    master->actfree[11] = 2;
    actor->r_no0 = 4;
    actor->actfree[14] = 255;

    swgun4(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[14]);

    reset_swgun4_state();
    actor = &actwk[4];
    master = &actwk[3];
    set_actor_short_alias(actor, 31, 3);
    set_actor_short_alias(master, 32, 4);
    master->actfree[11] = 2;
    actor->r_no0 = 4;

    swgun4(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[14]);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_swgun4_state();
    actor = &actwk[4];
    master = &actwk[3];
    set_actor_short_alias(actor, 31, 3);
    set_actor_short_alias(master, 32, 5);
    master->actfree[11] = 2;
    actor->r_no0 = 4;
    actor->actfree[14] = 1;
    actor->actfree[15] = 252;

    swgun4(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->actfree[14]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_swgun4_state();
    actor = &actwk[4];
    master = &actwk[3];
    set_actor_short_alias(actor, 31, 3);
    set_actor_short_alias(master, 32, 5);
    master->actfree[11] = 2;
    actor->r_no0 = 4;
    actor->actfree[15] = 0;

    swgun4(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->actfree[15]);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[14]);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_swgun4_state();
    actor = &actwk[4];
    master = &actwk[3];
    set_actor_short_alias(actor, 31, 3);
    master->actfree[11] = 3;
    actor->r_no0 = 4;

    swgun4(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_swgun4_state();
    actor = &actwk[3];
    set_actor_short_alias(actor, 31, 3);
    actor->r_no0 = 4;
    actor->actfree[11] = 3;
    actor->actfree[14] = 0;

    swgun4(actor);

    TEST_ASSERT_EQ_INT(ctx, 16, actor->actfree[14]);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_swgun4_frameout_dispatch_paths(test_context *ctx) {
    sprite_status *actor = &actwk[4];
    sprite_status *master = &actwk[3];

    reset_swgun4_state();
    set_actor_short_alias(actor, 31, 3);
    master->actfree[11] = 0;
    actor->r_no0 = 4;
    actor->actfree[14] = 1;

    swgun4(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);

    reset_swgun4_state();
    actor = &actwk[4];
    master = &actwk[3];
    set_actor_short_alias(actor, 31, 3);
    master->actfree[11] = 1;
    actor->r_no0 = 4;
    actor->actfree[14] = 1;

    swgun4(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);

    reset_swgun4_state();
    actor = &actwk[3];
    set_actor_short_alias(actor, 31, 3);
    actor->r_no0 = 4;
    actor->actfree[11] = 3;
    actor->actfree[14] = 240;
    actor->actfree[15] = 21;

    swgun4(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
}

static void test_ring4_lifecycle(test_context *ctx) {
    sprite_status *ring = &actwk[6];

    reset_swgun4_state();
    ring->yposi.w.h = 100;

    ring4(ring);

    TEST_ASSERT_EQ_INT(ctx, 2, ring->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 8, ring->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 8, ring->sprhs);
    TEST_ASSERT_TRUE(ctx, ring->patbase == ringpat);
    TEST_ASSERT_EQ_INT(ctx, 42926, ring->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 4, ring->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, ring->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 71, ring->colino);
    TEST_ASSERT_EQ_INT(ctx, 8, ring->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 280, ring->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 254, ring->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 47, ring->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 1, speedset2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_logs();
    ring->r_no0 = 2;
    ring->actfree[17] = 1;
    ring->actfree[16] = 2;
    ring->yspeed.w = 256;
    ring->yposi.w.h = 100;
    emycol_d_result = -4;

    ring4(ring);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    TEST_ASSERT_EQ_INT(ctx, 97, ring->yposi.w.h);
    TEST_ASSERT_TRUE(ctx, ring->yspeed.w < 0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_logs();
    ring->r_no0 = 4;

    ring4(ring);

    TEST_ASSERT_EQ_INT(ctx, 6, ring->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, ring->colino);
    TEST_ASSERT_EQ_INT(ctx, 1, ring->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 1, ringgetsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == ring);
    TEST_ASSERT_TRUE(ctx, patchg_table == ringchg);

    reset_logs();
    ring->r_no0 = 8;
    ring4(ring);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == ring);

    reset_logs();
    ring->r_no0 = 2;
    ring->actfree[16] = 1;
    ring->actfree[17] = 10;

    ring4(ring);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_logs();
    ring->r_no0 = 2;
    ring->actfree[16] = 2;
    ring->actfree[17] = 10;
    ring->yposi.w.h = 300;
    scralim_down = 0;

    ring4(ring);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
}

static void test_tensu4_lifecycle(test_context *ctx) {
    sprite_status *score = &actwk[7];

    reset_swgun4_state();

    tensu4(score);

    TEST_ASSERT_EQ_INT(ctx, 2, score->r_no0);
    TEST_ASSERT_TRUE(ctx, score->patbase == tensu4pat);
    TEST_ASSERT_EQ_INT(ctx, 878, score->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 4, score->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, score->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 8, score->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, -744, score->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, speedset2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_logs();
    score->r_no0 = 2;
    score->yspeed.w = 0;

    tensu4(score);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == score);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

TEST_MAIN_BEGIN;
test_tables_capture_literal_data(&ctx);
test_swgun4_init_spawns_children(&ctx);
test_switch_move_press_release_and_fire(&ctx);
test_swgun4_fire_threshold_and_dispatches(&ctx);
test_swgun4_frameout_dispatch_paths(&ctx);
test_ring4_lifecycle(&ctx);
test_tensu4_lifecycle(&ctx);
TEST_MAIN_END
