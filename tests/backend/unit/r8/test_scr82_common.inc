static void test_scr82_scr_set_saved_position_clamps_to_scroll_limits(
    test_context *ctx) {
    SCR82_RESET();
    plflag = 1;
    actwk[0].xposi.w.h = 9000;
    actwk[0].yposi.w.h = 3000;

    scr_set();

    TEST_ASSERT_EQ_INT(ctx, 1, playload_count);
    TEST_ASSERT_EQ_INT(ctx, 7831, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1808, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1467, scrb_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 226, scrb_v_posit.w.h);
}

static void test_scr82_scr_set_uses_end_demo_position(test_context *ctx) {
    SCR82_RESET();
    demoflag.w = -32768;
    enddemono = 2;

    scr_set();

    TEST_ASSERT_EQ_INT(ctx, 3744, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1132, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 3584, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1036, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 672, scrb_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 129, scrb_v_posit.w.h);
}

static void test_scr82_mapadrset_clamps_high_offsets(test_context *ctx) {
    Sint32 index = 0;

    SCR82_RESET();
    mapwka[7][63] = 3;
    map_work[(3 - 1) * 256 + 15 + 15 * 16] = 0x2222;

    TEST_ASSERT_EQ_INT(ctx, 1, mapadrset(0, 0, 16384, 2048, (Uint8 *)mapwka,
                                         &index));
    TEST_ASSERT_EQ_INT(ctx, 0x2222, index);
}

static void test_scr82_mapadrset2_reports_block_pointer_and_empty_screen(
    test_context *ctx) {
    Sint32 index = 123;
    Uint16 *block = (Uint16 *)1;

    SCR82_RESET();

    TEST_ASSERT_EQ_INT(ctx, 0,
                       mapadrset2(32, 48, (Uint8 *)mapwka, &index, &block));
    TEST_ASSERT_EQ_INT(ctx, 0, index);

    mapwka[0][0] = 2;
    map_work[256 + 2 + 3 * 16] = 0x3456;

    TEST_ASSERT_EQ_INT(ctx, 1,
                       mapadrset2(32, 48, (Uint8 *)mapwka, &index, &block));
    TEST_ASSERT_EQ_INT(ctx, 0x3456, index);
    TEST_ASSERT_TRUE(ctx, block == &map_work[256 + 2 + 3 * 16]);
}

static void test_scr82_vram_address_extra_entry_points(test_context *ctx) {
    POINT tile = {0, 0};

    SCR82_RESET();

    vramadrset0(48, 64, &tile);

    TEST_ASSERT_EQ_INT(ctx, 6, tile.x);
    TEST_ASSERT_EQ_INT(ctx, 8, tile.y);

    vramadrset2(16, 32, 48, 64, &tile);

    TEST_ASSERT_EQ_INT(ctx, 8, tile.x);
    TEST_ASSERT_EQ_INT(ctx, 12, tile.y);
}

static void test_scr82_scroll_h_exact_center_and_left_limit(test_context *ctx) {
    SCR82_RESET();
    scralim_left = 0;
    scralim_right = 7831;
    scra_hline = 160;
    scra_h_posit.w.h = 100;
    scra_h_count = 16;
    actwk[0].xposi.w.h = 260;

    scroll_h();

    TEST_ASSERT_EQ_INT(ctx, 100, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_hz);
    TEST_ASSERT_EQ_INT(ctx, 16, scra_h_count);
    TEST_ASSERT_EQ_INT(ctx, 0, scrflaga.b.h);

    scra_h_posit.w.h = 8;
    actwk[0].xposi.w.h = -100;

    scroll_h();

    TEST_ASSERT_EQ_INT(ctx, 0, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -2048, scra_hz);
}

static void test_scr82_scroll_negative_parallax_flags(test_context *ctx) {
    SCR82_RESET();
    scralim_left = 0;
    scralim_right = 7831;
    scralim_up = 0;
    scralim_down = 1808;
    scra_hline = 160;
    scra_vline = 96;
    scra_h_posit.w.h = 100;
    scra_v_posit.w.h = 100;
    scra_h_count = 16;
    scrb_h_count = 16;
    scrc_h_count = 16;
    scrz_h_count = 16;
    actwk[0].xposi.w.h = 120;
    actwk[0].yposi.w.h = 196;

    scroll();

    TEST_ASSERT_EQ_INT(ctx, 1, scrchk_count);
    TEST_ASSERT_EQ_INT(ctx, 84, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -4096, scra_hz);
    TEST_ASSERT_TRUE(ctx, scrflagb.b.h != 0);
}

static void test_scr82_scrollb_v_flags_up_and_down_edges(test_context *ctx) {
    SCR82_RESET();
    scrb_v_posit.w.h = 32;
    scrb_v_count = 16;

    scrollb_v(16);

    TEST_ASSERT_EQ_INT(ctx, 0, scrb_v_count);
    TEST_ASSERT_EQ_INT(ctx, 1, scrflagb.b.h);

    scrflagb.b.h = 0;
    scrb_v_posit.w.h = 0;
    scrb_v_count = 16;

    scrollb_v(16);

    TEST_ASSERT_EQ_INT(ctx, 0, scrb_v_count);
    TEST_ASSERT_EQ_INT(ctx, 2, scrflagb.b.h);
}

static void test_scr82_vertical_edge_branches(test_context *ctx) {
    SCR82_RESET();
    scralim_up = 0;
    scralim_down = 1808;
    scra_vline = 96;
    scra_v_posit.w.h = 100;
    actwk[0].cddat = 4;
    actwk[0].yposi.w.h = 202;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 101, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 256, scra_vz);

    actwk[0].cddat = 2;
    limmoveflag = 0;
    actwk[0].yposi.w.h = 228;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 0, scra_vz);

    limmoveflag = 1;
    actwk[0].cddat = 0;
    actwk[0].yposi.w.h = scra_v_posit.w.h + scra_vline;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 0, limmoveflag);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_vz);

    scra_v_posit.w.h = 100;
    scra_vline = 96;
    actwk[0].mspeed.w = -2048;
    actwk[0].yposi.w.h = 213;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 116, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4096, scra_vz);

    scra_v_posit.w.h = 100;
    scra_vline = 80;
    actwk[0].mspeed.w = 0;
    actwk[0].yposi.w.h = 177;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 98, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -512, scra_vz);
}

static void test_scr82_direct_vertical_helpers_cover_small_moves(
    test_context *ctx) {
    int_union lSpd;

    SCR82_RESET();
    scralim_up = 0;
    scralim_down = 1808;
    scra_v_posit.w.h = 100;
    scra_v_count = 16;

    sv_move_main2(4);

    TEST_ASSERT_EQ_INT(ctx, 104, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1024, scra_vz);

    scra_vline = 80;
    scra_v_posit.w.h = 100;
    scra_v_count = 16;

    sv_move_main(1);

    TEST_ASSERT_EQ_INT(ctx, 101, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 256, scra_vz);

    scra_v_posit.w.h = 100;
    scra_v_count = 0;

    sv_move_sub(-4);

    TEST_ASSERT_EQ_INT(ctx, 96, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -1024, scra_vz);
    TEST_ASSERT_EQ_INT(ctx, 1, scrflaga.b.h);

    memset(&lSpd, 0, sizeof(lSpd));
    lSpd.w.l = -1;
    scra_v_posit.w.h = 8;

    scrv_up_ch(lSpd);

    TEST_ASSERT_EQ_INT(ctx, 0, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -2048, scra_vz);
}

static void test_scr82_scrollwrtb_bit2_and_empty_paths(test_context *ctx) {
    Uint8 flag;

    SCR82_RESET();
    seed_screen_zero((Uint8 *)mapwkb);
    scrb_v_posit.w.h = 240;
    flag = 1;

    scrollwrtb(&flag, (Uint8 *)mapwkb, 24576);

    TEST_ASSERT_EQ_INT(ctx, 0, flag);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 4);

    SCR82_RESET();
    seed_screen_zero((Uint8 *)mapwkb);
    scrb_v_posit.w.h = 16;
    flag = 2;

    scrollwrtb(&flag, (Uint8 *)mapwkb, 24576);

    TEST_ASSERT_EQ_INT(ctx, 0, flag);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 4);

    SCR82_RESET();
    flag = 0;

    scrollwrtb(&flag, (Uint8 *)mapwkb, 24576);

    TEST_ASSERT_EQ_INT(ctx, 0, flag);
    TEST_ASSERT_EQ_INT(ctx, 0, setgrid_count);
}

static void test_scr82_block_wrt_visible_region_emits_grid_update(
    test_context *ctx) {
    SCR82_RESET();
    mapwka[0][0] = 1;
    blockwk[12][0] = 90;
    blockwk[12][1] = 91;
    blockwk[12][2] = 92;
    blockwk[12][3] = 93;
    scra_h_posit.w.h = -16;
    scra_v_posit.w.h = -256;

    TEST_ASSERT_EQ_INT(ctx, 0, block_chk(64, 0));

    block_wrt(12, 64, 0);

    TEST_ASSERT_EQ_INT(ctx, 12, map_work[4]);
    TEST_ASSERT_EQ_INT(ctx, 4, setgrid_count);
    TEST_ASSERT_EQ_INT(ctx, 8, setgrid_x[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, setgrid_y[0]);
    TEST_ASSERT_EQ_INT(ctx, 90, setgrid_block[0]);
}

static void test_scr82_noop_entry_points_are_callable(test_context *ctx) {
    SCR82_RESET();
    mapinittbl.colorno2 = 9;
    plflag = 1;
    play_start = 2;

    mapinit();
    scrollwrtc();
    scrollwrtz();
    mapset();
    divdevset();
    enecginit();

    TEST_ASSERT_EQ_INT(ctx, 1, colorset2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, colorset_count);
}
