#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 ta_flag;
Uint8 plpower_b;
Uint8 plpower_m;
Uint8 plpower_a;
short_union editmode;
ushort_union swdata;
ushort_union swdata1;
int_union pltime;
PALETTEENTRY zone1colora[48];
static PALETTEENTRY color_buffer[64];
PALETTEENTRY *lpcolorwk = color_buffer;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int actwkchk_count;
static sprite_status *actwkchk_actor;
static Sint32 actwkchk_result;
static int emycol_d_count;
static Sint16 emycol_results[12];
static int emycol_result_count;
static int emycol_result_index;
static int sub_sync_count;
static Sint16 sub_sync_arg;
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 sinset_sin_result;
static Sint16 sinset_cos_result;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
Sint16 emycol_d(sprite_status *pActwk);
void sub_sync(Sint16 ReqNo);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);

#include "src/emie1cg.c"
#include "src/emie1.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
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

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_result == 0) {
        *ppActwk = actwkchk_actor;
    }
    return actwkchk_result;
}

Sint16 emycol_d(sprite_status *pActwk) {
    (void)pActwk;
    ++emycol_d_count;
    if (emycol_result_index < emycol_result_count) {
        return emycol_results[emycol_result_index++];
    }
    return 0;
}

void sub_sync(Sint16 ReqNo) {
    ++sub_sync_count;
    sub_sync_arg = ReqNo;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    ++sinset_count;
    sinset_angle = kakudo;
    *sin = sinset_sin_result;
    *cos = sinset_cos_result;
}

static void queue_emycol(Sint16 value) {
    emycol_results[emycol_result_count++] = value;
}

static void set_legacy_word(sprite_status *actor, int word_index,
                            Sint16 value) {
    int offset = (word_index - 23) * 2;
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static void reset_emie1_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(&editmode, 0, sizeof(editmode));
    memset(&swdata, 0, sizeof(swdata));
    memset(&swdata1, 0, sizeof(swdata1));
    memset(&pltime, 0, sizeof(pltime));
    memset(zone1colora, 0, sizeof(zone1colora));
    memset(color_buffer, 0, sizeof(color_buffer));
    ta_flag = 0;
    plpower_b = 0;
    plpower_m = 0;
    plpower_a = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    actwkchk_count = 0;
    actwkchk_actor = &actwk[20];
    actwkchk_result = 0;
    emycol_d_count = 0;
    memset(emycol_results, 0, sizeof(emycol_results));
    emycol_result_count = 0;
    emycol_result_index = 0;
    sub_sync_count = 0;
    sub_sync_arg = 0;
    sinset_count = 0;
    sinset_angle = 0;
    sinset_sin_result = 0;
    sinset_cos_result = 0;
}

static void reset_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    actwkchk_count = 0;
    emycol_d_count = 0;
    emycol_result_count = 0;
    emycol_result_index = 0;
    sub_sync_count = 0;
    sub_sync_arg = 0;
    sinset_count = 0;
    sinset_angle = 0;
}

static void assert_entry_callbacks(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void test_emie1_tables_and_time_attack_palette(test_context *ctx) {
    sprite_status *amy = &actwk[4];
    int i;

    reset_emie1_state();
    for (i = 0; i < 16; ++i) {
        zone1colora[i].peRed = (Uint8)(10 + i);
        zone1colora[i].peGreen = (Uint8)(20 + i);
        zone1colora[i].peBlue = (Uint8)(30 + i);
        zone1colora[i].peFlags = (Uint8)(40 + i);
    }

    TEST_ASSERT_TRUE(ctx, emie1pat[0] == &pat0);
    TEST_ASSERT_TRUE(ctx, emie1pat[16] == &pat10);
    TEST_ASSERT_EQ_INT(ctx, -16, pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, 448, pat0.spra[0].index);
    TEST_ASSERT_TRUE(ctx, em_pchg[0] == stnd2_pchg);
    TEST_ASSERT_EQ_INT(ctx, 11, stnd2_pchg[0]);
    TEST_ASSERT_EQ_INT(ctx, -1, kyoro_pchg[8]);

    ta_flag = 1;
    emie1(amy);

    TEST_ASSERT_EQ_INT(ctx, 0, amy->actflg);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 10, color_buffer[16].peRed);
    TEST_ASSERT_EQ_INT(ctx, 25, color_buffer[21].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 45, color_buffer[21].peFlags);
}

static void test_emie1_init_far_position_copies_amy_palette(test_context *ctx) {
    sprite_status *amy = &actwk[4];

    reset_emie1_state();
    amy->actno = 47;
    amy->xposi.w.h = 200;
    amy->yposi.w.h = 100;
    actwk[0].xposi.w.h = 40;
    queue_emycol(3);
    queue_emycol(-2);
    queue_emycol(0);

    emie1(amy);

    TEST_ASSERT_EQ_INT(ctx, 5, amy->actflg);
    TEST_ASSERT_EQ_INT(ctx, 9072, amy->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 1, amy->sprpri);
    TEST_ASSERT_TRUE(ctx, amy->patbase == emie1pat);
    TEST_ASSERT_EQ_INT(ctx, 12, amy->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, amy->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 101, amy->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 5, amy->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 5, amy->mstno.b.l);
    TEST_ASSERT_EQ_INT(ctx, 11, amy->patno);
    TEST_ASSERT_EQ_INT(ctx, 29, amy->pattim);
    TEST_ASSERT_EQ_INT(ctx, 3, emycol_d_count);
    TEST_ASSERT_EQ_INT(ctx, 128, color_buffer[18].peRed);
    TEST_ASSERT_EQ_INT(ctx, 32, color_buffer[18].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 96, color_buffer[18].peBlue);
    assert_entry_callbacks(ctx, amy);
}

static void test_emie1_init_near_player_enters_wait_state(test_context *ctx) {
    sprite_status *amy = &actwk[4];

    reset_emie1_state();
    amy->xposi.w.h = 200;
    amy->yposi.w.h = 100;
    actwk[0].xposi.w.h = 250;

    emie1(amy);

    TEST_ASSERT_EQ_INT(ctx, 2, amy->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->cddat & 1);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->actflg & 1);
    TEST_ASSERT_EQ_INT(ctx, 0, color_buffer[16].peRed);
    assert_entry_callbacks(ctx, amy);
}

static void test_emie1_waits_and_kyoro_jump_charge(test_context *ctx) {
    sprite_status *amy = &actwk[4];

    reset_emie1_state();
    amy->r_no0 = 2;
    amy->xposi.w.h = 100;
    amy->yposi.w.h = 50;
    actwk[0].xposi.w.h = 108;
    queue_emycol(5);

    emie1(amy);

    TEST_ASSERT_EQ_INT(ctx, 4, amy->actfree[20]);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 55, amy->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, amy->mstno.b.h);
    assert_entry_callbacks(ctx, amy);

    reset_emie1_state();
    amy->r_no0 = 2;
    amy->xposi.w.h = 100;
    amy->yposi.w.h = 50;
    amy->actfree[20] = 4;
    actwk[0].xposi.w.h = 120;
    queue_emycol(3);

    emie1(amy);

    TEST_ASSERT_EQ_INT(ctx, 4, amy->actfree[20]);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 53, amy->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, amy->mstno.b.h);

    reset_emie1_state();
    amy->r_no0 = 2;
    amy->actfree[20] = 128;
    amy->actfree[21] = 3;
    amy->actfree[16] = 252;
    actwk[0].xspeed.w = 1;
    queue_emycol(10);

    emie1(amy);

    TEST_ASSERT_EQ_INT(ctx, 0, amy->actfree[21]);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 4, amy->mstno.b.h);

    reset_emie1_state();
    amy->r_no0 = 2;
    amy->actfree[20] = 128;
    amy->actfree[21] = 1;
    actwk[0].xspeed.w = 1;
    queue_emycol(10);
    queue_emycol(-1);

    emie1(amy);

    TEST_ASSERT_EQ_INT(ctx, 2, amy->actfree[21]);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->actfree[20] & 64);
}

static void test_emie1_chases_clamps_and_can_catch_player(test_context *ctx) {
    sprite_status *amy = &actwk[4];

    reset_emie1_state();
    amy->r_no0 = 2;
    amy->xposi.w.h = 100;
    amy->yposi.w.h = 100;
    set_legacy_word(amy, 29, 100);
    actwk[0].xposi.w.h = 116;
    actwk[0].yposi.w.h = 92;
    queue_emycol(0);

    emie1(amy);

    TEST_ASSERT_EQ_INT(ctx, 4, amy->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 129, amy->actfree[20]);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->patno);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 124, sub_sync_arg);

    reset_emie1_state();
    amy->r_no0 = 2;
    amy->xposi.w.h = 100;
    amy->xspeed.w = -700;
    set_legacy_word(amy, 29, 100);
    actwk[0].xposi.w.h = 20;
    queue_emycol(0);

    emie1(amy);

    TEST_ASSERT_EQ_INT(ctx, -512, amy->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 98, amy->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, amy->mstno.b.h);

    reset_emie1_state();
    amy->r_no0 = 2;
    amy->xposi.w.h = -210;
    amy->xspeed.w = -16;
    amy->actfree[20] = 4;
    set_legacy_word(amy, 29, 100);
    actwk[0].xposi.w.h = -260;

    emie1(amy);

    TEST_ASSERT_EQ_INT(ctx, 0, amy->actfree[20] & 4);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->xspeed.w);

    reset_emie1_state();
    amy->r_no0 = 2;
    amy->xposi.w.h = 100;
    amy->xspeed.w = 700;
    set_legacy_word(amy, 29, 100);
    actwk[0].xposi.w.h = 200;
    queue_emycol(0);

    emie1(amy);

    TEST_ASSERT_EQ_INT(ctx, 512, amy->xspeed.w);
}

static void test_emie1_daki_paths_jump_and_time_release(test_context *ctx) {
    sprite_status *amy = &actwk[4];
    sprite_status *sonic = &actwk[0];

    reset_emie1_state();
    amy->r_no0 = 4;
    amy->xspeed.w = 128;
    sonic->xposi.w.h = 200;
    sonic->yposi.w.h = 300;
    sonic->xspeed.w = 128;
    sonic->cddat = 1;

    emie1(amy);

    TEST_ASSERT_EQ_INT(ctx, 5, sonic->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 64, sonic->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 212, amy->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 300, amy->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 3, amy->mstno.b.h);

    reset_emie1_state();
    amy->r_no0 = 4;
    amy->actfree[20] = 1;
    sonic->direc.b.h = 64;
    sonic->cddat = 4;
    swdata1.b.h = 112;
    sinset_sin_result = -128;
    sinset_cos_result = 64;

    emie1(amy);

    TEST_ASSERT_EQ_INT(ctx, 6, amy->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->actfree[20] & 1);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 0, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 19, sonic->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 9, sonic->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 20, sonic->cddat & 20);

    reset_emie1_state();
    amy->r_no0 = 4;
    amy->actfree[20] = 1;
    sonic->yposi.w.h = 100;
    pltime.l = 602624;
    sinset_cos_result = 256;

    emie1(amy);

    TEST_ASSERT_EQ_INT(ctx, 2, amy->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->actfree[20] & 1);
    TEST_ASSERT_EQ_INT(ctx, 14, sonic->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 7, sonic->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 105, sonic->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, sonic->mstno.b.h);
}

static void test_emie1_jump_arc_and_landing_return(test_context *ctx) {
    sprite_status *amy = &actwk[4];

    reset_emie1_state();
    amy->r_no0 = 6;
    amy->xposi.w.h = 120;
    amy->cddat = 1;
    set_legacy_word(amy, 29, 100);
    queue_emycol(0);

    emie1(amy);

    TEST_ASSERT_EQ_INT(ctx, 8, amy->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 128, amy->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -704, amy->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 6, amy->patno);

    reset_emie1_state();
    amy->r_no0 = 6;
    amy->xposi.w.h = 300;
    set_legacy_word(amy, 29, 100);
    queue_emycol(0);

    emie1(amy);

    TEST_ASSERT_EQ_INT(ctx, 0, amy->xspeed.w);

    reset_emie1_state();
    amy->r_no0 = 6;
    amy->xposi.w.h = 80;
    set_legacy_word(amy, 29, 100);
    queue_emycol(0);

    emie1(amy);

    TEST_ASSERT_EQ_INT(ctx, -128, amy->xspeed.w);

    reset_emie1_state();
    amy->r_no0 = 8;
    amy->xposi.w.h = 100;
    amy->yposi.w.h = 100;
    amy->yspeed.w = 0;
    amy->actfree[16] = 240;
    queue_emycol(-1);

    emie1(amy);

    TEST_ASSERT_EQ_INT(ctx, 2, amy->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->yspeed.w);

    reset_emie1_state();
    amy->r_no0 = 8;
    amy->yspeed.w = -32;
    queue_emycol(0);

    emie1(amy);

    TEST_ASSERT_EQ_INT(ctx, 7, amy->patno);
    TEST_ASSERT_EQ_INT(ctx, 32, amy->yspeed.w);

    reset_emie1_state();
    amy->r_no0 = 8;
    amy->actfree[16] = 0;
    queue_emycol(-1);

    emie1(amy);

    TEST_ASSERT_EQ_INT(ctx, 16, amy->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 8, amy->r_no0);

    reset_emie1_state();
    amy->r_no0 = 2;
    amy->actfree[20] = 128 | 64;
    amy->yspeed.w = 0;
    queue_emycol(0);

    emie1(amy);

    TEST_ASSERT_EQ_INT(ctx, 4, amy->patno);
}

static void test_emie1_heart_spawn_and_allocation_failure(test_context *ctx) {
    sprite_status *amy = &actwk[4];

    reset_emie1_state();
    amy->xposi.w.h = 100;
    amy->yposi.w.h = 50;
    amy->cddat = 1;
    amy->actfree[17] = 250;
    amy->actfree[20] = 1;
    actwkchk_actor = &actwk[20];

    heartset(amy);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 48, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 110, actwk[20].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 38, actwk[20].yposi.w.h);

    reset_emie1_state();
    amy->actfree[17] = 250;
    actwkchk_result = -1;

    heartset(amy);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[20].actno);

    reset_emie1_state();
    amy->xposi.w.h = 100;
    amy->yposi.w.h = 50;
    amy->actfree[17] = 250;
    actwkchk_actor = &actwk[20];

    heartset(amy);

    TEST_ASSERT_EQ_INT(ctx, 48, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 108, actwk[20].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 38, actwk[20].yposi.w.h);
}

static void test_heart1_initializes_moves_and_frames_out(test_context *ctx) {
    sprite_status *heart = &actwk[10];

    reset_emie1_state();
    sinset_sin_result = 64;
    sinset_cos_result = 100;

    heart1(heart);

    TEST_ASSERT_EQ_INT(ctx, 2, heart->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, heart->actflg);
    TEST_ASSERT_EQ_INT(ctx, 880, heart->sproffset);
    TEST_ASSERT_TRUE(ctx, heart->patbase == emie1pat);
    TEST_ASSERT_EQ_INT(ctx, 8, heart->patno);
    TEST_ASSERT_EQ_INT(ctx, -96, heart->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 16, heart->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, heart->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 0, sinset_angle);
    assert_entry_callbacks(ctx, heart);

    reset_emie1_state();
    heart->r_no0 = 2;
    heart->actfree[16] = 19;
    heart->patno = 8;

    heart1(heart);

    TEST_ASSERT_EQ_INT(ctx, 9, heart->patno);

    reset_emie1_state();
    heart->r_no0 = 2;
    heart->actfree[16] = 109;
    heart->patno = 8;
    heart->xspeed.w = 32;
    heart->yspeed.w = -16;

    heart1(heart);

    TEST_ASSERT_EQ_INT(ctx, 9, heart->patno);
    TEST_ASSERT_EQ_INT(ctx, 0, heart->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, heart->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, heart->actfree[18]);

    reset_emie1_state();
    heart->r_no0 = 2;
    heart->actfree[16] = 119;

    heart1(heart);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == heart);
}

static void test_static_helpers_cover_deceleration_and_animation_wrap(
    test_context *ctx) {
    sprite_status *amy = &actwk[4];
    sprite_status *sonic = &actwk[0];

    reset_emie1_state();
    amy->xspeed.w = 1;
    sonic->xspeed.w = -100;
    queue_emycol(2);

    emie_play(amy, sonic);

    TEST_ASSERT_EQ_INT(ctx, -36, sonic->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 2, sonic->yposi.w.h);

    reset_emie1_state();
    amy->xspeed.w = 1;
    sonic->xspeed.w = -10;
    emie_play(amy, sonic);
    TEST_ASSERT_EQ_INT(ctx, 0, sonic->xspeed.w);

    reset_emie1_state();
    amy->xspeed.w = 1;
    sonic->xspeed.w = 10;
    emie_play(amy, sonic);
    TEST_ASSERT_EQ_INT(ctx, 0, sonic->xspeed.w);

    reset_emie1_state();
    amy->mstno.b.h = 5;
    amy->mstno.b.l = 5;
    amy->patcnt = 8;
    amy->pattim = 1;

    empatchg(amy, em_pchg);

    TEST_ASSERT_EQ_INT(ctx, 11, amy->patno);
    TEST_ASSERT_EQ_INT(ctx, 2, amy->patcnt);
}

static void test_dakicheck_documents_guard_paths(test_context *ctx) {
    sprite_status *amy = &actwk[4];
    sprite_status *sonic = &actwk[0];

    reset_emie1_state();
    amy->xposi.w.h = 300;
    amy->xspeed.w = 1;
    set_legacy_word(amy, 29, 100);
    dakicheck(amy);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->r_no0);

    reset_emie1_state();
    amy->xposi.w.h = -300;
    amy->xspeed.w = -1;
    set_legacy_word(amy, 29, 100);
    dakicheck(amy);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->r_no0);

    reset_emie1_state();
    amy->xposi.w.h = 100;
    amy->xspeed.w = 1;
    set_legacy_word(amy, 29, 100);
    pltime.l = 602624;
    dakicheck(amy);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->r_no0);

    reset_emie1_state();
    amy->xposi.w.h = 100;
    amy->xspeed.w = 1;
    set_legacy_word(amy, 29, 100);
    editmode.b.h = 1;
    dakicheck(amy);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->r_no0);

    reset_emie1_state();
    amy->xposi.w.h = 100;
    amy->xspeed.w = 1;
    set_legacy_word(amy, 29, 100);
    sonic->xposi.w.h = 120;
    sonic->cddat = 1;
    dakicheck(amy);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->r_no0);

    reset_emie1_state();
    amy->xposi.w.h = 100;
    amy->yposi.w.h = 100;
    amy->xspeed.w = 1;
    set_legacy_word(amy, 29, 100);
    sonic->xposi.w.h = 150;
    dakicheck(amy);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->r_no0);

    reset_emie1_state();
    amy->xposi.w.h = 100;
    amy->yposi.w.h = 100;
    amy->xspeed.w = 1;
    set_legacy_word(amy, 29, 100);
    sonic->xposi.w.h = 116;
    sonic->yposi.w.h = 200;
    dakicheck(amy);
    TEST_ASSERT_EQ_INT(ctx, 0, amy->r_no0);

    reset_emie1_state();
    amy->xposi.w.h = 100;
    amy->yposi.w.h = 100;
    amy->xspeed.w = 1;
    set_legacy_word(amy, 29, 100);
    sonic->xposi.w.h = 116;
    sonic->yposi.w.h = 92;
    sonic->xspeed.w = -20;
    dakicheck(amy);
    TEST_ASSERT_EQ_INT(ctx, 4, amy->r_no0);

    reset_emie1_state();
    amy->xposi.w.h = 100;
    amy->yposi.w.h = 100;
    amy->xspeed.w = 1;
    set_legacy_word(amy, 29, 100);
    sonic->xposi.w.h = 116;
    sonic->yposi.w.h = 92;
    sonic->cddat = 2;
    dakicheck(amy);
    TEST_ASSERT_EQ_INT(ctx, 6, amy->r_no0);
}

TEST_MAIN_BEGIN;
test_emie1_tables_and_time_attack_palette(&ctx);
test_emie1_init_far_position_copies_amy_palette(&ctx);
test_emie1_init_near_player_enters_wait_state(&ctx);
test_emie1_waits_and_kyoro_jump_charge(&ctx);
test_emie1_chases_clamps_and_can_catch_player(&ctx);
test_emie1_daki_paths_jump_and_time_release(&ctx);
test_emie1_jump_arc_and_landing_return(&ctx);
test_emie1_heart_spawn_and_allocation_failure(&ctx);
test_heart1_initializes_moves_and_frames_out(&ctx);
test_static_helpers_cover_deceleration_and_animation_wrap(&ctx);
test_dakicheck_documents_guard_paths(&ctx);
TEST_MAIN_END
