#include <stddef.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 bossflag;
Uint8 generate_flag;
Sint16 scra_vline;
Uint8 flagwork[766];

static sprite_pattern dummy_pattern = {1, {{0, 0, 0, 1}}};
sprite_pattern *bakupat[] = {&dummy_pattern};
static Uint8 bakuchg0[] = {1, 0, 255};
char *bakuchg[] = {(char *)bakuchg0};

static int actionsub_count;
static sprite_status *actionsub_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int soundset_count;
static Sint16 soundset_requests[16];
static int frameout_count;
static sprite_status *frameout_actor;
static int actwkchk_count;
static int actwkchk2_count;
static int next_alloc_index;
static int actwkchk_fail_after;
static int colorset2_count;
static Sint32 colorset2_values[16];
static int scoreup_count;
static Uint32 scoreup_value;
static int sub_sync_count;
static Sint16 sub_sync_requests[16];
static int genecolor_count;
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 sinset_sin;
static Sint16 sinset_cos;
static int atan_sonic_count;
static Sint16 atan_sonic_x;
static Sint16 atan_sonic_y;
static Sint16 atan_sonic_result;
static Sint16 emycol_d_result;
static Sint16 emycol_u_result;
static Sint16 emycol_l_result;
static Sint16 emycol_r_result;
static Sint32 random_value;
Sint32 (*sRandom)(void);

void actionsub(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void soundset(Sint16 ReqNo);
void frameout(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);
void colorset2(Sint32 ColorNo);
void scoreup(Uint32 score);
void sub_sync(Sint16 ReqNo);
void genecolor(void);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);
Sint16 atan_sonic(Sint16 x, Sint16 y);
Sint16 emycol_d(sprite_status *pActwk);
Sint16 emycol_u(sprite_status *pActwk);
Sint16 emycol_l(sprite_status *pActwk, Uint8 iD3);
Sint16 emycol_r(sprite_status *pActwk, Uint8 iD3);
Sint32 stub_random(void);

#include "src/r3/boss_3.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < (int)(sizeof(soundset_requests) /
                               sizeof(soundset_requests[0]))) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_fail_after >= 0 && actwkchk_count > actwkchk_fail_after) {
        *ppActwk = 0;
        return 1;
    }
    *ppActwk = &actwk[next_alloc_index++];
    return 0;
}

Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk) {
    (void)pActwk;
    ++actwkchk2_count;
    if (actwkchk_fail_after >= 0 && actwkchk2_count > actwkchk_fail_after) {
        *ppNewActwk = 0;
        return 1;
    }
    *ppNewActwk = &actwk[next_alloc_index++];
    return 0;
}

void colorset2(Sint32 ColorNo) {
    if (colorset2_count < (int)(sizeof(colorset2_values) /
                                sizeof(colorset2_values[0]))) {
        colorset2_values[colorset2_count] = ColorNo;
    }
    ++colorset2_count;
}

void scoreup(Uint32 score) {
    ++scoreup_count;
    scoreup_value = score;
}

void sub_sync(Sint16 ReqNo) {
    if (sub_sync_count < (int)(sizeof(sub_sync_requests) /
                               sizeof(sub_sync_requests[0]))) {
        sub_sync_requests[sub_sync_count] = ReqNo;
    }
    ++sub_sync_count;
}

void genecolor(void) { ++genecolor_count; }

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    ++sinset_count;
    sinset_angle = kakudo;
    *sin = sinset_sin;
    *cos = sinset_cos;
}

Sint16 atan_sonic(Sint16 x, Sint16 y) {
    ++atan_sonic_count;
    atan_sonic_x = x;
    atan_sonic_y = y;
    return atan_sonic_result;
}

Sint16 emycol_d(sprite_status *pActwk) {
    (void)pActwk;
    return emycol_d_result;
}

Sint16 emycol_u(sprite_status *pActwk) {
    (void)pActwk;
    return emycol_u_result;
}

Sint16 emycol_l(sprite_status *pActwk, Uint8 iD3) {
    (void)pActwk;
    (void)iD3;
    return emycol_l_result;
}

Sint16 emycol_r(sprite_status *pActwk, Uint8 iD3) {
    (void)pActwk;
    (void)iD3;
    return emycol_r_result;
}

Sint32 stub_random(void) { return random_value; }

static void reset_boss3_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(flagwork, 0, sizeof(flagwork));
    bossflag = 0;
    generate_flag = 0;
    scra_vline = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    frameout_count = 0;
    frameout_actor = 0;
    actwkchk_count = 0;
    actwkchk2_count = 0;
    next_alloc_index = 40;
    actwkchk_fail_after = -1;
    colorset2_count = 0;
    memset(colorset2_values, 0, sizeof(colorset2_values));
    scoreup_count = 0;
    scoreup_value = 0;
    sub_sync_count = 0;
    memset(sub_sync_requests, 0, sizeof(sub_sync_requests));
    genecolor_count = 0;
    sinset_count = 0;
    sinset_angle = 0;
    sinset_sin = 128;
    sinset_cos = -64;
    atan_sonic_count = 0;
    atan_sonic_x = 0;
    atan_sonic_y = 0;
    atan_sonic_result = 64;
    emycol_d_result = 1;
    emycol_u_result = 1;
    emycol_l_result = 1;
    emycol_r_result = 1;
    random_value = 0x12345;
    sRandom = stub_random;
}

static void test_small_platform_init_hit_and_bomb_paths(test_context *ctx) {
    reset_boss3_state();
    actwk[1].xposi.w.h = 500;
    actwk[1].yposi.w.h = 700;
    egg3dai_s_ini(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].actflg);
    TEST_ASSERT_EQ_INT(ctx, 48, actwk[1].sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 62, actwk[1].colino);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].colicnt);
    TEST_ASSERT_EQ_INT(ctx, -163840, egg3_get_work(&actwk[1])->vertical_speed);
    TEST_ASSERT_EQ_INT(ctx, 652, egg3_get_work(&actwk[1])->target_position);
    TEST_ASSERT_EQ_INT(ctx, 50, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 452, actwk[40].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, egg3_get_work(&actwk[40])->child_index);
    TEST_ASSERT_EQ_INT(ctx, 40, egg3_get_work(&actwk[1])->child_index);

    reset_boss3_state();
    actwk[1].colino = 0;
    egg3dai_s_chk(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 172, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, egg3_get_work(&actwk[1])->flags);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_boss3_state();
    actwk[1].yposi.w.h = 700;
    egg3_get_work(&actwk[1])->timer = 15;
    egg3_get_work(&actwk[1])->vertical_speed = -0x400000;
    egg3_get_work(&actwk[1])->target_position = 690;
    egg3dai_s_up(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 690, actwk[1].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, egg3_get_work(&actwk[1])->flags & 4);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)egg3dai_s_pchg);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->child_index = 40;
    egg3_get_work(&actwk[1])->explosion_timer = 120;
    egg3_get_work(&actwk[1])->flags = 2;
    egg3dai_s_bom(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actwk[1]);
}

static void test_jaba_and_flash_helpers(test_context *ctx) {
    reset_boss3_state();
    actwk[0].xposi.w.h = 1152;
    actwk[0].yposi.w.h = 1307;
    jaba_s_chk(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, egg3_get_work(&actwk[1])->flags);
    TEST_ASSERT_EQ_INT(ctx, 103, sub_sync_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 63, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[40].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1056, actwk[40].xposi.w.h);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->flash_timer = 2;
    actwk[1].patno = 4;
    egg3flash(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[1].patno);
    TEST_ASSERT_EQ_INT(ctx, 1, egg3_get_work(&actwk[1])->flash_timer);
    TEST_ASSERT_EQ_INT(ctx, 5, colorset2_values[0]);

    egg3flash(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].patno);
    TEST_ASSERT_EQ_INT(ctx, 0, egg3_get_work(&actwk[1])->flash_timer);
    TEST_ASSERT_EQ_INT(ctx, 6, colorset2_values[1]);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->jaba_count = 0;
    actwk[0].xposi.w.h = 896;
    actwk[0].yposi.w.h = 260;
    TEST_ASSERT_EQ_INT(ctx, 0, jabaopen_chk(&actwk[1]));
    actwk[0].xposi.w.h = 831;
    TEST_ASSERT_EQ_INT(ctx, 1, jabaopen_chk(&actwk[1]));
    egg3_get_work(&actwk[1])->jaba_count = 4;
    TEST_ASSERT_EQ_INT(ctx, 1, jabaopen_chk(&actwk[1]));

    reset_boss3_state();
    egg3_get_work(&actwk[1])->jaba_count = 0;
    actwk[0].xposi.w.h = 896;
    actwk[0].yposi.w.h = 100;
    TEST_ASSERT_EQ_INT(ctx, 1, jabaopen_chk(&actwk[1]));
    actwk[0].yposi.w.h = 300;
    actwk[0].xposi.w.h = 961;
    TEST_ASSERT_EQ_INT(ctx, 1, jabaopen_chk(&actwk[1]));

    reset_boss3_state();
    egg3_get_work(&actwk[1])->flash_timer = 0;
    egg3flash(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, colorset2_count);

    reset_boss3_state();
    actwk[0].xposi.w.h = 1152;
    actwk[0].yposi.w.h = 1307;
    actwkchk_fail_after = 0;
    jaba_s_chk(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, egg3_get_work(&actwk[1])->flags);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[40].actno);
}

static void test_large_platform_hit_bomb_and_end_paths(test_context *ctx) {
    reset_boss3_state();
    actwk[1].xposi.w.h = 700;
    actwk[1].yposi.w.h = 448;
    egg3dai_l_ini(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 6, colorset2_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 51, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 50, actwk[41].actno);
    TEST_ASSERT_EQ_INT(ctx, 61, actwk[42].actno);
    TEST_ASSERT_EQ_INT(ctx, 40, egg3_get_work(&actwk[1])->support_index);
    TEST_ASSERT_EQ_INT(ctx, 41, egg3_get_work(&actwk[1])->child_index);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->child_index = 40;
    actwk[0].yposi.w.h = 200;
    egg3dai_l_demo(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 220, actwk[1].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 220, egg3_get_work(&actwk[1])->target_position);
    TEST_ASSERT_EQ_INT(ctx, 4, egg3_get_work(&actwk[1])->flags);
    actwk[0].yposi.w.h = 448;
    egg3dai_l_demo(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, -163840, egg3_get_work(&actwk[1])->vertical_speed);
    TEST_ASSERT_EQ_INT(ctx, 376, egg3_get_work(&actwk[1])->target_position);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->jaba_count = 0;
    jaba_open(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 63, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 896, actwk[40].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 528, actwk[40].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, egg3_get_work(&actwk[1])->jaba_count);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->support_index = 40;
    actwk[1].colicnt = 1;
    actwk[1].yposi.w.h = 500;
    egg3dai_l_hit(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, egg3_get_work(&actwk[1])->phase);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 100, scoreup_value);
    TEST_ASSERT_EQ_INT(ctx, 20, sub_sync_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[40].r_no0);

    reset_boss3_state();
    generate_flag = 1;
    egg3_get_work(&actwk[1])->support_index = 40;
    egg3_get_work(&actwk[1])->phase = 2;
    actwk[1].colicnt = 1;
    actwk[1].yposi.w.h = 500;
    egg3dai_l_hit(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 3, egg3_get_work(&actwk[1])->phase);
    TEST_ASSERT_EQ_INT(ctx, 19, sub_sync_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[40].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1020, actwk[40].sproffset);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->child_index = 40;
    egg3_get_work(&actwk[1])->flags = 1;
    egg3_get_work(&actwk[1])->jaba_count = 4;
    egg3dai_l_end(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actwk[1]);

    reset_boss3_state();
    actwkchk_fail_after = 0;
    egg3dai_l_ini(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[40].actno);

    reset_boss3_state();
    actwkchk_fail_after = 1;
    egg3dai_l_ini(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 51, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[41].actno);

    reset_boss3_state();
    actwkchk_fail_after = 2;
    egg3dai_l_ini(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 3, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 50, actwk[41].actno);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[42].actno);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->jaba_count = 0;
    actwk[0].xposi.w.h = 896;
    actwk[0].yposi.w.h = 260;
    actwk[1].colino = 0;
    egg3dai_l_chk(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 63, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, egg3_get_work(&actwk[1])->phase);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->jaba_count = 2;
    jaba_last(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, bossflag);
    egg3_get_work(&actwk[1])->jaba_count = 3;
    actwk[0].xposi.w.h = 700;
    actwk[0].yposi.w.h = 260;
    jaba_last(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, bossflag);
    actwk[0].xposi.w.h = 896;
    actwk[0].yposi.w.h = 260;
    jaba_last(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 3, bossflag);
    TEST_ASSERT_EQ_INT(ctx, 63, actwk[40].actno);
}

static void test_bomb_setters_spawn_and_mark_completion(test_context *ctx) {
    char one_bomb_tbl[4] = {1, 1, 5, -3};

    reset_boss3_state();
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    egg3_get_work(&actwk[1])->explosion_timer = 0;
    bom_set(&actwk[1], one_bomb_tbl);
    TEST_ASSERT_EQ_INT(ctx, 24, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 105, actwk[40].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 197, actwk[40].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 255, actwk[40].r_no1);
    TEST_ASSERT_EQ_INT(ctx, 158, soundset_requests[0]);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->child_index = 40;
    egg3_get_work(&actwk[1])->explosion_timer = 119;
    bom_set_dai(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, egg3_get_work(&actwk[1])->flags & 1);
    TEST_ASSERT_EQ_INT(ctx, 1, egg3_get_work(&actwk[40])->flags & 1);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    reset_boss3_state();
    actwk[1].xposi.w.h = 300;
    actwk[1].yposi.w.h = 400;
    egg3_get_work(&actwk[1])->explosion_timer = 0;
    bom_set_cannon(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 24, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 268, actwk[40].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 400, actwk[40].yposi.w.h);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->explosion_timer = 2;
    bom_set_ctrl2(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    reset_boss3_state();
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    egg3_get_work(&actwk[1])->explosion_timer = 1;
    bom_set_ctrl2(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 24, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 84, actwk[40].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 144, actwk[40].yposi.w.h);

    reset_boss3_state();
    actwkchk_fail_after = 0;
    egg3_get_work(&actwk[1])->explosion_timer = 0;
    bom_set(&actwk[1], one_bomb_tbl);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
}

static void test_egg3_and_attachments(test_context *ctx) {
    reset_boss3_state();
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    egg3_ini(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].actflg);
    TEST_ASSERT_EQ_INT(ctx, 184, egg3_get_work(&actwk[1])->target_position);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->child_index = 40;
    actwk[40].yposi.w.h = 333;
    egg3_norm(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 333, actwk[1].yposi.w.h);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->child_index = 40;
    actwk[1].yposi.w.h = 100;
    actwk[40].yposi.w.h = 120;
    egg3_get_work(&actwk[1])->vertical_speed = 0x200000;
    egg3_tobi(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 120, actwk[1].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].mstno.w);

    reset_boss3_state();
    actwk[1].yposi.w.h = 200;
    egg3_get_work(&actwk[1])->target_position = 196;
    egg3_esc1(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 8, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3, actwk[1].sprpri);
    TEST_ASSERT_EQ_INT(ctx, actwk[1].xposi.w.h + 560,
                       egg3_get_work(&actwk[1])->target_position);

    reset_boss3_state();
    actwk[1].xposi.w.h = 100;
    egg3_get_work(&actwk[1])->target_position = 103;
    egg3_esc2(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->child_index = 40;
    actwk[40].yposi.w.h = 600;
    egg3_get_work(&actwk[40])->flags = 4;
    egg3haguruma(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 600, actwk[1].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->child_index = 40;
    egg3_get_work(&actwk[40])->phase = 1;
    egg3cannon(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->child_index = 40;
    actwk[1].yposi.w.h = 100;
    actwk[40].yposi.w.h = 200;
    egg3_get_work(&actwk[1])->vertical_speed = 0;
    egg3_tobi(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].r_no0);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->child_index = 40;
    actwk[1].yposi.w.h = 200;
    actwk[40].yposi.w.h = 200;
    egg3_get_work(&actwk[40])->phase = 2;
    egg3_tobi(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[40].patno);
    TEST_ASSERT_EQ_INT(ctx, 1020, actwk[1].sproffset);

    reset_boss3_state();
    actwk[1].xposi.w.h = 100;
    egg3_get_work(&actwk[1])->target_position = 200;
    egg3_esc2(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->flags = 1;
    egg3haguruma(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_cannon_and_punch_bomb_paths(test_context *ctx) {
    reset_boss3_state();
    egg3cannon_ini(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 24, actwk[1].sprhsize);
    TEST_ASSERT_TRUE(ctx, actwk[1].patbase == egg3cannon_pat);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->child_index = 40;
    actwk[1].xposi.w.h = 500;
    actwk[1].yposi.w.h = 600;
    egg3_get_work(&actwk[1])->timer = 119;
    actwk[1].patcnt = 0;
    actwk[0].yposi.w.h = 900;
    random_value = 0x3456;
    egg3cannon_01(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 62, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 488, actwk[40].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 608, actwk[40].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -(0x3456 % 1024), actwk[40].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 768, actwk[40].yspeed.w);

    reset_boss3_state();
    actwk[1].r_no0 = 4;
    actwk[1].pattim = 0;
    actwk[1].patcnt = 3;
    egg3cannon_fire(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].patcnt);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].patno);

    reset_boss3_state();
    actwk[1].pattim = 2;
    egg3cannon_fire(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].pattim);

    reset_boss3_state();
    actwk[1].pattim = 0;
    actwk[1].patcnt = 7;
    egg3cannon_fire(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].patcnt);

    reset_boss3_state();
    actwk[1].pattim = 0;
    actwk[1].patcnt = 1;
    egg3cannon_fire(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].pattim);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].patcnt);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].patno);

    reset_boss3_state();
    punchbom_ini(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 215, actwk[1].colino);
    TEST_ASSERT_EQ_INT(ctx, 32, egg3_get_work(&actwk[1])->y_acceleration);
    TEST_ASSERT_EQ_INT(ctx, 1536, egg3_get_work(&actwk[1])->max_y_speed);

    reset_boss3_state();
    actwk[1].xspeed.w = 256;
    actwk[1].yspeed.w = 0;
    egg3_get_work(&actwk[1])->x_acceleration = 0;
    egg3_get_work(&actwk[1])->y_acceleration = 32;
    egg3_get_work(&actwk[1])->max_y_speed = 64;
    add_spd3(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 32, actwk[1].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].xposi.w.h);

    reset_boss3_state();
    actwk[1].xposi.w.h = 200;
    actwk[1].yposi.w.h = 300;
    actwk[0].xposi.w.h = 210;
    actwk[0].yposi.w.h = 280;
    actwk[1].colicnt = 1;
    actwk[1].cdsts = 3;
    flagwork[3] = 137;
    punchbom_mov(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, atan_sonic_count);
    TEST_ASSERT_EQ_INT(ctx, -10, atan_sonic_x);
    TEST_ASSERT_EQ_INT(ctx, 20, atan_sonic_y);
    TEST_ASSERT_EQ_INT(ctx, 64, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, -896, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 138, flagwork[3]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 158, soundset_requests[0]);

    reset_boss3_state();
    punchbom_fireini(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].mstno.w);
    TEST_ASSERT_TRUE(ctx, actwk[1].patbase == bakupat);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);

    reset_boss3_state();
    punchbom_del(&actwk[1]);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actwk[1]);
}

static void test_dispatchers_and_remaining_movement_paths(test_context *ctx) {
    reset_boss3_state();
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    egg3dai_s(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_boss3_state();
    actwk[1].r_no0 = 2;
    actwk[1].colino = 0;
    egg3dai_s(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);

    reset_boss3_state();
    actwk[1].r_no0 = 4;
    actwk[1].yposi.w.h = 200;
    egg3_get_work(&actwk[1])->vertical_speed = -0x400000;
    egg3_get_work(&actwk[1])->target_position = 190;
    egg3dai_s(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[1].r_no0);

    reset_boss3_state();
    actwk[1].r_no0 = 6;
    egg3_get_work(&actwk[1])->explosion_timer = 120;
    egg3dai_s(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[1].r_no0);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->phase = 1;
    scra_vline = 170;
    egg3dai_l(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 176, scra_vline);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->flags = 1;
    actwk[1].r_no0 = 10;
    egg3dai_l(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_boss3_state();
    actwk[1].r_no0 = 6;
    egg3_get_work(&actwk[1])->phase = 1;
    actwk[1].yposi.w.h = 100;
    egg3_get_work(&actwk[1])->vertical_speed = 0;
    egg3_get_work(&actwk[1])->target_position = 100;
    egg3dai_l_up(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 63, actwk[1].colino);

    reset_boss3_state();
    actwk[1].r_no0 = 6;
    egg3_get_work(&actwk[1])->phase = 3;
    actwk[1].colicnt = 1;
    actwk[1].yposi.w.h = 100;
    egg3_get_work(&actwk[1])->vertical_speed = 0;
    egg3_get_work(&actwk[1])->target_position = 100;
    egg3dai_l_up(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 8, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, genecolor_count);

    reset_boss3_state();
    actwk[1].r_no0 = 8;
    egg3_get_work(&actwk[1])->timer = 3;
    actwk[1].yposi.w.h = 100;
    egg3_get_work(&actwk[1])->vertical_speed = 0;
    egg3_get_work(&actwk[1])->target_position = 100;
    egg3dai_l_bom(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[1].patno);

    reset_boss3_state();
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    egg3(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->child_index = 40;
    actwk[1].r_no0 = 4;
    actwk[1].yposi.w.h = 200;
    actwk[40].yposi.w.h = 200;
    egg3_get_work(&actwk[1])->vertical_speed = 0;
    egg3(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);

    reset_boss3_state();
    actwk[1].r_no0 = 6;
    actwk[1].yposi.w.h = 200;
    egg3_get_work(&actwk[1])->target_position = 196;
    egg3(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 8, actwk[1].r_no0);

    reset_boss3_state();
    actwk[1].r_no0 = 8;
    actwk[1].xposi.w.h = 100;
    egg3_get_work(&actwk[1])->target_position = 103;
    egg3(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->child_index = 40;
    egg3cannon(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->child_index = 40;
    actwk[1].r_no0 = 2;
    egg3_get_work(&actwk[1])->timer = 119;
    actwk[0].yposi.w.h = 900;
    egg3cannon(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);

    reset_boss3_state();
    actwk[1].r_no0 = 4;
    actwk[1].pattim = 0;
    egg3cannon(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_boss3_state();
    actwk[1].r_no0 = 0;
    punchbom(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);

    reset_boss3_state();
    actwk[1].r_no0 = 4;
    punchbom(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);

    reset_boss3_state();
    actwk[1].r_no0 = 6;
    punchbom(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_boss3_state();
    actwk[1].r_no0 = 8;
    punchbom(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);

    reset_boss3_state();
    actwk[1].r_no0 = 10;
    punchbom(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].mstno.w);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->timer = 239;
    punchbom_mov(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);

    reset_boss3_state();
    actwk[1].xspeed.w = 256;
    actwk[1].yspeed.w = 256;
    egg3_get_work(&actwk[1])->flash_timer = 2;
    egg3_get_work(&actwk[1])->y_acceleration = 0;
    emycol_d_result = -5;
    emycol_r_result = -7;
    punchbom_mov(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, -256, actwk[1].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -256, actwk[1].xspeed.w);

    reset_boss3_state();
    actwk[1].xspeed.w = -256;
    actwk[1].yspeed.w = -256;
    egg3_get_work(&actwk[1])->flash_timer = 2;
    egg3_get_work(&actwk[1])->y_acceleration = 0;
    emycol_u_result = -4;
    emycol_l_result = -6;
    punchbom_mov(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 256, actwk[1].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 256, actwk[1].xspeed.w);

    reset_boss3_state();
    actwk[1].xspeed.w = 128;
    actwk[1].yspeed.w = 256;
    egg3_get_work(&actwk[1])->x_acceleration = 0;
    egg3_get_work(&actwk[1])->y_acceleration = 0;
    add_spd3(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].yposi.w.h);

    reset_boss3_state();
    actwk[1].xspeed.w = 128;
    actwk[1].yspeed.w = -100;
    egg3_get_work(&actwk[1])->x_acceleration = 0;
    egg3_get_work(&actwk[1])->y_acceleration = -32;
    egg3_get_work(&actwk[1])->max_y_speed = -64;
    add_spd3(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, -64, actwk[1].yspeed.w);

    reset_boss3_state();
    actwk[1].xspeed.w = 256;
    actwk[1].yspeed.w = 512;
    egg3_get_work(&actwk[1])->x_acceleration = 16;
    egg3_get_work(&actwk[1])->y_acceleration = -32;
    add_spd2(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 272, actwk[1].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 480, actwk[1].yspeed.w);
}

static void test_final_boss3_branch_edges(test_context *ctx) {
    reset_boss3_state();
    actwkchk_fail_after = 0;
    egg3dai_s_ini(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[40].actno);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->phase = 1;
    egg3_get_work(&actwk[1])->flags = 1;
    actwk[1].r_no0 = 10;
    scra_vline = 175;
    egg3dai_l(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 178, scra_vline);

    reset_boss3_state();
    actwk[1].r_no0 = 2;
    egg3_get_work(&actwk[1])->child_index = 40;
    actwk[0].yposi.w.h = 448;
    egg3dai_l(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);

    reset_boss3_state();
    actwk[1].r_no0 = 4;
    actwk[1].colino = 1;
    actwk[0].xposi.w.h = 700;
    egg3dai_l(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);

    reset_boss3_state();
    actwk[1].r_no0 = 6;
    egg3_get_work(&actwk[1])->phase = 1;
    actwk[1].yposi.w.h = 100;
    egg3_get_work(&actwk[1])->vertical_speed = 0;
    egg3_get_work(&actwk[1])->target_position = 100;
    egg3dai_l(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);

    reset_boss3_state();
    actwk[1].r_no0 = 8;
    actwk[1].yposi.w.h = 100;
    egg3_get_work(&actwk[1])->vertical_speed = 0;
    egg3_get_work(&actwk[1])->target_position = 99;
    egg3dai_l(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 8, actwk[1].r_no0);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->child_index = 40;
    actwk[0].yposi.w.h = 220;
    egg3_get_work(&actwk[1])->target_position = 220;
    egg3dai_l_demo(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, egg3_get_work(&actwk[1])->flags & 4);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->jaba_count = 0;
    actwk[0].xposi.w.h = 896;
    actwk[0].yposi.w.h = 497;
    TEST_ASSERT_EQ_INT(ctx, 1, jabaopen_chk(&actwk[1]));

    reset_boss3_state();
    actwkchk_fail_after = 0;
    jaba_open(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[40].actno);

    reset_boss3_state();
    actwk[1].yposi.w.h = 100;
    egg3_get_work(&actwk[1])->vertical_speed = 0;
    egg3_get_work(&actwk[1])->target_position = 99;
    egg3dai_l_bom(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].r_no0);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->explosion_timer = 120;
    egg3dai_l_end(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->child_index = 40;
    egg3cannon(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->child_index = 40;
    actwk[0].yposi.w.h = 1024;
    egg3cannon_01(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, egg3_get_work(&actwk[1])->timer);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->child_index = 40;
    egg3_get_work(&actwk[40])->jaba_count = 1;
    actwk[0].yposi.w.h = 900;
    egg3cannon_01(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, egg3_get_work(&actwk[1])->timer);

    reset_boss3_state();
    egg3_get_work(&actwk[1])->child_index = 40;
    egg3_get_work(&actwk[1])->timer = 119;
    actwk[0].yposi.w.h = 900;
    actwkchk_fail_after = 0;
    egg3cannon_01(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[40].actno);

    reset_boss3_state();
    actwk[1].pattim = 1;
    egg3_get_work(&actwk[1])->y_acceleration = 0;
    punchbom_mov(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].patno);

    reset_boss3_state();
    actwk[1].yspeed.w = 64;
    egg3_get_work(&actwk[1])->y_acceleration = 32;
    egg3_get_work(&actwk[1])->max_y_speed = 64;
    add_spd3(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 64, actwk[1].yspeed.w);

    reset_boss3_state();
    actwk[1].yspeed.w = 0;
    egg3_get_work(&actwk[1])->y_acceleration = -32;
    egg3_get_work(&actwk[1])->max_y_speed = -64;
    add_spd3(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, -32, actwk[1].yspeed.w);
}

TEST_MAIN_BEGIN;
    test_small_platform_init_hit_and_bomb_paths(&ctx);
    test_jaba_and_flash_helpers(&ctx);
    test_large_platform_hit_bomb_and_end_paths(&ctx);
    test_bomb_setters_spawn_and_mark_completion(&ctx);
    test_egg3_and_attachments(&ctx);
    test_cannon_and_punch_bomb_paths(&ctx);
    test_dispatchers_and_remaining_movement_paths(&ctx);
    test_final_boss3_branch_edges(&ctx);
TEST_MAIN_END
