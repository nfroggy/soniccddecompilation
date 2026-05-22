#include <string.h>

#include "support/test_runner.h"
#include "types.h"

sprite_status actwk[128];
game_info *lpKeepWork;
Sint16 plring;
Sint16 plring_s;
Uint8 plring_f2;
Uint8 plring_f2_s;
int_union pltime;
Uint8 water_flag;
Sint16 scralim_down;
Sint16 scralim_n_down;
int_union scra_h_posit;
int_union scra_v_posit;
int_union scrb_h_posit;
int_union scrb_v_posit;
int_union scrc_h_posit;
int_union scrc_v_posit;
int_union scrz_h_posit;
int_union scrz_v_posit;
short_union stageno;
Uint8 chibi_flag;
Uint8 plflag;
Sint16 scralim_left;
Uint8 plflag_s;
Sint16 plxposi_s;
Sint16 plyposi_s;
Sint32 pltime_s;
Uint8 water_flag_s;
Sint16 scralim_down_s;
Sint16 scra_h_posit_s;
Sint16 scra_v_posit_s;
Sint16 scrb_h_posit_s;
Sint16 scrb_v_posit_s;
Sint16 scrc_h_posit_s;
Sint16 scrc_v_posit_s;
Sint16 scrz_h_posit_s;
Sint16 scrz_v_posit_s;
Uint8 chibi_s;
Sint16 waterposi_m;
Sint16 waterposi_m_s;
Uint8 waterflag;
Uint8 waterflag_s;

static game_info keep_work;

#include "src/save.c"

static void reset_save_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(&keep_work, 0, sizeof(keep_work));
    lpKeepWork = &keep_work;
    plring = 0;
    plring_s = 0;
    plring_f2 = 0;
    plring_f2_s = 0;
    memset(&pltime, 0, sizeof(pltime));
    water_flag = 0;
    scralim_down = 0;
    scralim_n_down = 0;
    memset(&scra_h_posit, 0, sizeof(scra_h_posit));
    memset(&scra_v_posit, 0, sizeof(scra_v_posit));
    memset(&scrb_h_posit, 0, sizeof(scrb_h_posit));
    memset(&scrb_v_posit, 0, sizeof(scrb_v_posit));
    memset(&scrc_h_posit, 0, sizeof(scrc_h_posit));
    memset(&scrc_v_posit, 0, sizeof(scrc_v_posit));
    memset(&scrz_h_posit, 0, sizeof(scrz_h_posit));
    memset(&scrz_v_posit, 0, sizeof(scrz_v_posit));
    memset(&stageno, 0, sizeof(stageno));
    chibi_flag = 0;
    plflag = 0;
    scralim_left = 0;
    plflag_s = 0;
    plxposi_s = 0;
    plyposi_s = 0;
    pltime_s = 0;
    water_flag_s = 0;
    scralim_down_s = 0;
    scra_h_posit_s = 0;
    scra_v_posit_s = 0;
    scrb_h_posit_s = 0;
    scrb_v_posit_s = 0;
    scrc_h_posit_s = 0;
    scrc_v_posit_s = 0;
    scrz_h_posit_s = 0;
    scrz_v_posit_s = 0;
    chibi_s = 0;
    waterposi_m = 0;
    waterposi_m_s = 0;
    waterflag = 0;
    waterflag_s = 0;
}

static void fill_keep_work_checkpoint(void) {
    keep_work.plxposi_sb = 1234;
    keep_work.plyposi_sb = -567;
    keep_work.cddat_sb = 0xA5;
    keep_work.plmspd_sb = -111;
    keep_work.plxspd_sb = 222;
    keep_work.plyspd_sb = -333;
    keep_work.plring_s = 44;
    keep_work.plring_f2_s = 7;
    keep_work.pltime_sb = 0x01020304;
    keep_work.water_flag_sb = 9;
    keep_work.scralim_down_sb = 3456;
    keep_work.scra_h_posit_sb = -100;
    keep_work.scra_v_posit_sb = 200;
    keep_work.scrb_h_posit_sb = -300;
    keep_work.scrb_v_posit_sb = 400;
    keep_work.scrc_h_posit_sb = -500;
    keep_work.scrc_v_posit_sb = 600;
    keep_work.scrz_h_posit_sb = -700;
    keep_work.scrz_v_posit_sb = 800;
    keep_work.chibi_sb = 11;
}

static void assert_checkpoint_loaded(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 1234, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -567, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0xA5, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, -111, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 222, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -333, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 44, plring);
    TEST_ASSERT_EQ_INT(ctx, 44, plring_s);
    TEST_ASSERT_EQ_INT(ctx, 7, plring_f2);
    TEST_ASSERT_EQ_INT(ctx, 7, plring_f2_s);
    TEST_ASSERT_EQ_INT(ctx, 0x01020304, pltime.l);
    TEST_ASSERT_EQ_INT(ctx, 9, water_flag);
    TEST_ASSERT_EQ_INT(ctx, 3456, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 3456, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, -100, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -300, scrb_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 400, scrb_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -500, scrc_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 600, scrc_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -700, scrz_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 800, scrz_v_posit.w.h);
}

static void test_playloadb_restores_checkpoint_without_optional_flags(
    test_context *ctx) {
    reset_save_state();
    fill_keep_work_checkpoint();
    stageno.b.h = 5;
    chibi_flag = 99;
    plflag = 0;
    scralim_left = 2222;

    playloadb();

    assert_checkpoint_loaded(ctx);
    TEST_ASSERT_EQ_INT(ctx, 99, chibi_flag);
    TEST_ASSERT_EQ_INT(ctx, 2222, scralim_left);
}

static void test_playloadb_restores_chibi_and_negative_flag_limit(
    test_context *ctx) {
    reset_save_state();
    fill_keep_work_checkpoint();
    stageno.b.h = 6;
    plflag = 128;
    scralim_left = 0;

    playloadb();

    assert_checkpoint_loaded(ctx);
    TEST_ASSERT_EQ_INT(ctx, 11, chibi_flag);
    TEST_ASSERT_EQ_INT(ctx, 1074, scralim_left);
}

static void test_playload_uses_checkpoint_when_plflag_is_two(
    test_context *ctx) {
    reset_save_state();
    fill_keep_work_checkpoint();
    plflag = 2;
    stageno.b.h = 6;

    playload();

    assert_checkpoint_loaded(ctx);
    TEST_ASSERT_EQ_INT(ctx, 11, chibi_flag);
}

static void test_playload_restores_stage_start_state(test_context *ctx) {
    reset_save_state();
    plflag = 0;
    plflag_s = 3;
    plxposi_s = 333;
    plyposi_s = -444;
    plring = 55;
    plring_f2 = 66;
    pltime_s = 0;
    water_flag_s = 7;
    scralim_down_s = 888;
    scra_h_posit_s = 10;
    scra_v_posit_s = 20;
    scrb_h_posit_s = 30;
    scrb_v_posit_s = 40;
    scrc_h_posit_s = 50;
    scrc_v_posit_s = 60;
    scrz_h_posit_s = 70;
    scrz_v_posit_s = 80;
    stageno.b.h = 1;
    chibi_flag = 99;
    scralim_left = 123;

    playload();

    TEST_ASSERT_EQ_INT(ctx, 3, plflag);
    TEST_ASSERT_EQ_INT(ctx, 333, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -444, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, plring);
    TEST_ASSERT_EQ_INT(ctx, 0, plring_f2);
    TEST_ASSERT_EQ_INT(ctx, 59, pltime.b.b4);
    TEST_ASSERT_EQ_INT(ctx, -1, pltime.b.b3);
    TEST_ASSERT_EQ_INT(ctx, 0, pltime.b.b2);
    TEST_ASSERT_EQ_INT(ctx, 0, pltime.b.b1);
    TEST_ASSERT_EQ_INT(ctx, 7, water_flag);
    TEST_ASSERT_EQ_INT(ctx, 888, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 888, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 10, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 20, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 30, scrb_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 40, scrb_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 50, scrc_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 60, scrc_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 70, scrz_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 80, scrz_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 99, chibi_flag);
    TEST_ASSERT_EQ_INT(ctx, 123, scralim_left);
}

static void test_playload_restores_stage_six_chibi(test_context *ctx) {
    reset_save_state();
    plflag = 0;
    plflag_s = 4;
    stageno.b.h = 6;
    chibi_s = 12;

    playload();

    TEST_ASSERT_EQ_INT(ctx, 4, plflag);
    TEST_ASSERT_EQ_INT(ctx, 12, chibi_flag);
}

static void test_playload_restores_stage_two_water_state(test_context *ctx) {
    reset_save_state();
    plflag = 0;
    plflag_s = 5;
    stageno.b.h = 2;
    waterposi_m_s = -123;
    water_flag_s = 14;
    waterflag_s = 15;

    playload();

    TEST_ASSERT_EQ_INT(ctx, -123, waterposi_m);
    TEST_ASSERT_EQ_INT(ctx, 14, water_flag);
    TEST_ASSERT_EQ_INT(ctx, 15, waterflag);
}

static void test_playload_negative_saved_flag_updates_left_limit(
    test_context *ctx) {
    reset_save_state();
    plflag = 0;
    plflag_s = 128;
    plxposi_s = 900;
    scralim_left = 55;

    playload();

    TEST_ASSERT_EQ_INT(ctx, 128, plflag);
    TEST_ASSERT_EQ_INT(ctx, 740, scralim_left);
}

TEST_MAIN_BEGIN;
    test_playloadb_restores_checkpoint_without_optional_flags(&ctx);
    test_playloadb_restores_chibi_and_negative_flag_limit(&ctx);
    test_playload_uses_checkpoint_when_plflag_is_two(&ctx);
    test_playload_restores_stage_start_state(&ctx);
    test_playload_restores_stage_six_chibi(&ctx);
    test_playload_restores_stage_two_water_state(&ctx);
    test_playload_negative_saved_flag_updates_left_limit(&ctx);
TEST_MAIN_END;
