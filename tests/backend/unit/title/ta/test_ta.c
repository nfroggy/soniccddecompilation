#include <stdarg.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

game_info keep_work;
game_info *lpKeepWork = &keep_work;
Sint32 fade_flag;
Sint32 *lpFadeFlag = &fade_flag;
int_union hscroll_buffer[8];
int_union *lphscrollbuff = hscroll_buffer;
Uint16 map_work[8];
Uint16 *pmapwk = map_work;
PALETTEENTRY colorwk[80];
PALETTEENTRY colorwk2[80];
PALETTEENTRY colorwk3[80];
PALETTEENTRY colorwk4[80];
PALETTEENTRY *lpcolorwk = colorwk;
PALETTEENTRY *lpcolorwk2 = colorwk2;
PALETTEENTRY *lpcolorwk3 = colorwk3;
PALETTEENTRY *lpcolorwk4 = colorwk4;

#include "src/title/ta/ta.c"

static draw_context draw_ctx;
static score_data score;
static int delete_ea_calls;
static int draw_ta_calls;
static int tenmetu_calls;
static int tamove_calls;
static int clwout_calls;
static int clwin_init_calls;
static int clwin_calls;
static int color_set_calls;
static Sint32 last_color_set_mode;
static int sp6_calls;
static int ea_create_calls;
static int cdplay_calls;
static Sint16 last_cdplay;
static int cdpause_calls;
static Sint16 last_cdpause;
static int set_score_date_calls;
static score_data *last_set_score_date;
static int write_score_calls;
static Sint32 last_write_index;
static int read_score_calls;
static Sint32 read_score_result;
static int strncpy_calls;
static int strncmp_calls;
static int draw_context_calls;

void clwout(void) { ++clwout_calls; }
void clwout_new(void) {}
void woutcolor(Uint16 wD1) { (void)wD1; }
void clwin_init(void) { ++clwin_init_calls; }
void clwin(void) { ++clwin_calls; }
void clwin_new(void) {}
void wincolor(Uint8 byD0, Uint16 wD1) {
    (void)byD0;
    (void)wD1;
}
Sint32 FadeProc(void) { return 0; }

void TAColorSet(Sint32 mode) {
    ++color_set_calls;
    last_color_set_mode = mode;
}
void TAPicColorSet(Sint32 mode, Sint32 round) {
    (void)mode;
    (void)round;
}
void TAColorChgSP6(void) { ++sp6_calls; }

void EAError(Sint32 ret, Sint32 line, char *str) {
    (void)ret;
    (void)line;
    (void)str;
}
void srfDraw(void) {}
Sint32 ld_load_grid_module(hmx_environment *buffer, char *file,
                           hmx_environment *env, hmx_bitmap **bitmaps,
                           Sint32 size) {
    (void)buffer;
    (void)file;
    (void)env;
    (void)bitmaps;
    (void)size;
    return 0;
}
Sint32 TAGridBmpCreate(void) { return 0; }
Sint32 TAGridCreate(void) { return 0; }
Sint32 GridInitSub(char *path, map_info *pInfo) {
    (void)path;
    (void)pInfo;
    return 0;
}
Sint32 TAGridInit(void) { return 0; }
Sint32 BkgBmpCreate_TA(void) { return 0; }
Sint32 BkgBmpDelete_TA(void) { return 0; }
Sint32 BkgCreate_TA(void) { return 0; }
Sint32 BkgDelete_TA(void) { return 0; }
Sint32 LoadPicBmp(void) { return 0; }
Uint32 CreatePic(void) { return 0; }
void SetPic(Sint32 mode, Sint32 round, Sint32 zurasi) {
    (void)mode;
    (void)round;
    (void)zurasi;
}
Uint32 TA_EACreate(void) {
    ++ea_create_calls;
    return 0;
}
void DeleteEA(void) { ++delete_ea_calls; }
void DrawTA(void) { ++draw_ta_calls; }
Uint16 GetLFont(char c) { return (Uint16)c; }
Uint16 GetSFont(char c) { return (Uint16)c; }
void TimeToAsc8(Uint32 Time, char *p) {
    (void)Time;
    (void)p;
}
void TimeToAsc9(Uint32 Time, char *p) {
    (void)Time;
    (void)p;
}
void PrintTotalTime(Sint32 mode, Uint32 b) {
    (void)mode;
    (void)b;
}
void PrintRoundName(Sint32 mode, Sint32 round, Uint32 b, Sint32 grid) {
    (void)mode;
    (void)round;
    (void)b;
    (void)grid;
}
void PrintZoneNo(Sint32 mode, Sint32 zone) {
    (void)mode;
    (void)zone;
}
void PrintZoneSelect(Sint32 mode, Sint32 round, Sint32 zone) {
    (void)mode;
    (void)round;
    (void)zone;
}
void PrintZoneRecord(Sint32 mode, Sint32 round, Sint32 zone, Sint32 rank) {
    (void)mode;
    (void)round;
    (void)zone;
    (void)rank;
}
void PrintZoneRecordAll(Sint32 mode, Sint32 round, Sint32 zone) {
    (void)mode;
    (void)round;
    (void)zone;
}
void PrintMenu2(Sint32 mode, Sint32 cur, Uint32 b) {
    (void)mode;
    (void)cur;
    (void)b;
}
void PrintRank(Sint32 mode, Sint32 rank, Uint32 b) {
    (void)mode;
    (void)rank;
    (void)b;
}
void PrintRankName(Sint32 mode, Sint32 ranky, Sint32 rankx, char c, Uint32 b) {
    (void)mode;
    (void)ranky;
    (void)rankx;
    (void)c;
    (void)b;
}
void Tenmetu(void) { ++tenmetu_calls; }
Sint32 TAMove(void) {
    ++tamove_calls;
    return 0;
}

static draw_context *stub_get_draw_context(void) {
    ++draw_context_calls;
    return &draw_ctx;
}

static void stub_cdplay(Sint16 id) {
    ++cdplay_calls;
    last_cdplay = id;
}

static void stub_cdpause(Sint16 id) {
    ++cdpause_calls;
    last_cdpause = id;
}

static void stub_set_score_date(score_data *data) {
    ++set_score_date_calls;
    last_set_score_date = data;
}

static Uint32 stub_write_score_data(Sint32 index, char *data, Uint32 size) {
    ++write_score_calls;
    last_write_index = index;
    (void)data;
    (void)size;
    return 1;
}

static Sint32 stub_read_score_index(Uint32 id) {
    ++read_score_calls;
    (void)id;
    return read_score_result;
}

static char *stub_strncpy(char *dst, char *src, Sint32 count) {
    ++strncpy_calls;
    return strncpy(dst, src, (size_t)count);
}

static Sint32 stub_strncmp(char *left, char *right, Sint32 count) {
    ++strncmp_calls;
    return strncmp(left, right, (size_t)count);
}

static void stub_output_debug_string(char *message) { (void)message; }
static void stub_printf(char *buffer, const char *format, ...) {
    (void)buffer;
    (void)format;
}
static Sint32 stub_open_file(char *path) {
    (void)path;
    return 0;
}
static Sint32 stub_read_file(Sint32 file, void *buffer, Sint32 size) {
    (void)file;
    (void)buffer;
    return size;
}
static void stub_close_file(Sint32 file) { (void)file; }
static void *stub_mem_alloc(Sint32 size) {
    (void)size;
    return 0;
}
static void stub_mem_free(void *mem) { (void)mem; }
static void stub_flip_to_screen(void) {}
static void stub_ld_bitmap_4to8(void *a, void *b, Sint32 c, Sint32 d,
                                Sint32 e, Sint32 f, Sint32 g) {
    (void)a;
    (void)b;
    (void)c;
    (void)d;
    (void)e;
    (void)f;
    (void)g;
}
static void *stub_ld_load_cmpfile(hmx_environment *env, char *path) {
    (void)env;
    (void)path;
    return 0;
}
static void stub_background_set(hmx_background *bg, Sint32 color) {
    (void)bg;
    (void)color;
}
static hmx_bitmap *stub_bitmap_create(hmx_environment *env, Sint32 w,
                                      Sint32 h) {
    (void)env;
    (void)w;
    (void)h;
    return (hmx_bitmap *)1;
}
static void stub_bitmap_release(hmx_environment *env, hmx_bitmap *bmp) {
    (void)env;
    (void)bmp;
}
static void *stub_bitmap_get_scan0(hmx_bitmap *bmp) {
    (void)bmp;
    return 0;
}
static void stub_bitmap_set_transparency(hmx_bitmap *bmp, Sint32 value) {
    (void)bmp;
    (void)value;
}
static void stub_hmx_free(hmx_environment *env, void *mem) {
    (void)env;
    (void)mem;
}
static hmx_grid *stub_grid_create(hmx_environment *env, Sint32 a, Sint32 b,
                                  Sint32 c, Sint32 d) {
    (void)env;
    (void)a;
    (void)b;
    (void)c;
    (void)d;
    return (hmx_grid *)1;
}
static void stub_grid_release(hmx_environment *env, hmx_grid *grid) {
    (void)env;
    (void)grid;
}
static hmx_renderer_base *stub_grid_base(hmx_grid *grid) {
    (void)grid;
    return (hmx_renderer_base *)1;
}
static void stub_grid_set_position(hmx_grid *grid, Sint32 x, Sint32 y) {
    (void)grid;
    (void)x;
    (void)y;
}
static void stub_grid_set_view(hmx_grid *grid, Sint32 x, Sint32 y, Sint32 w,
                               Sint32 h) {
    (void)grid;
    (void)x;
    (void)y;
    (void)w;
    (void)h;
}
static void stub_grid_set_tile(hmx_grid *grid, Sint32 x, Sint32 y,
                               hmx_bitmap *bmp, Sint32 tile) {
    (void)grid;
    (void)x;
    (void)y;
    (void)bmp;
    (void)tile;
}
static void stub_renderer_add(hmx_renderer_context *ctx, Sint32 level,
                              hmx_renderer_base *base) {
    (void)ctx;
    (void)level;
    (void)base;
}
static void stub_renderer_clear(hmx_renderer_context *ctx) { (void)ctx; }
static void stub_renderer_draw(hmx_renderer_context *ctx, hmx_surface *surf) {
    (void)ctx;
    (void)surf;
}
static hmx_renderer_base *stub_sprite_base(hmx_sprite *sprite) {
    (void)sprite;
    return (hmx_renderer_base *)1;
}
static void stub_sprite_set_position(hmx_sprite *sprite, Sint32 x, Sint32 y) {
    (void)sprite;
    (void)x;
    (void)y;
}
static void stub_sprite_set_bitmap(hmx_sprite *sprite, hmx_bitmap *bmp) {
    (void)sprite;
    (void)bmp;
}

static void reset_state(void) {
    memset(&keep_work, 0, sizeof(keep_work));
    memset(&score, 0, sizeof(score));
    memset(&draw_ctx, 0, sizeof(draw_ctx));
    lpKeepWork = &keep_work;
    lpScoreData = &score;
    get_draw_context_module = stub_get_draw_context;
    CDPlay = stub_cdplay;
    CDPause = stub_cdpause;
    SetScoreDate2 = stub_set_score_date;
    WriteScoreData2 = stub_write_score_data;
    ReadScoreIndx2 = stub_read_score_index;
    sStrncpy = stub_strncpy;
    sStrncmp = stub_strncmp;
    gMove = 0;
    gRankY = 0;
    gRankX = 0;
    bExit = 0;
    KeyRepTimer = 0;
    s_ctx = 0;
    gRet = 0;
    gNewRankX = 0;
    gTimer = 0;
    gNewMenuZone = 0;
    gMenuZone = 0;
    gNewMenuRound = 0;
    gMenuRound = 0;
    gNewMenu2 = 0;
    gMenu2 = 0;
    gNewMenu1 = 0;
    gMenu1 = 0;
    swdata1.w = 0;
    swdata2.w = 0;
    delete_ea_calls = 0;
    draw_ta_calls = 0;
    tenmetu_calls = 0;
    tamove_calls = 0;
    clwout_calls = 0;
    clwin_init_calls = 0;
    clwin_calls = 0;
    color_set_calls = 0;
    last_color_set_mode = -1;
    sp6_calls = 0;
    ea_create_calls = 0;
    cdplay_calls = 0;
    last_cdplay = -1;
    cdpause_calls = 0;
    last_cdpause = -1;
    set_score_date_calls = 0;
    last_set_score_date = (score_data *)1;
    write_score_calls = 0;
    last_write_index = -99;
    read_score_calls = 0;
    read_score_result = 7;
    strncpy_calls = 0;
    strncmp_calls = 0;
    draw_context_calls = 0;
}

static void set_name(char name[4], char a, char b, char c) {
    name[0] = a;
    name[1] = b;
    name[2] = c;
    name[3] = 0;
}

static void test_swdata_set_copies_both_words(test_context *ctx) {
    ushort_union sw1;
    ushort_union sw2;

    reset_state();
    sw1.w = 0x1234;
    sw2.w = 0xabcd;

    SWdataSet(sw1, sw2);

    TEST_ASSERT_EQ_INT(ctx, 0x1234, swdata1.w);
    TEST_ASSERT_EQ_INT(ctx, 0xabcd, swdata2.w);
}

static void test_special_play_check_uses_total_and_round_gate(test_context *ctx) {
    reset_state();

    score.total = 109268;
    score.roundNo = 7;
    TEST_ASSERT_EQ_INT(ctx, 1, SpecialPlayChk());

    score.total = 109269;
    TEST_ASSERT_EQ_INT(ctx, 0, SpecialPlayChk());

    score.total = 1;
    score.roundNo = 6;
    TEST_ASSERT_EQ_INT(ctx, 0, SpecialPlayChk());
}

static void test_get_play_round_defaults_and_caps(test_context *ctx) {
    reset_state();

    score.roundNo = 0;
    TEST_ASSERT_EQ_INT(ctx, 1, GetPlayRound());

    score.roundNo = 5;
    TEST_ASSERT_EQ_INT(ctx, 5, GetPlayRound());

    score.roundNo = 9;
    TEST_ASSERT_EQ_INT(ctx, 7, GetPlayRound());
}

static void test_game_restart_exits_immediately(test_context *ctx) {
    reset_state();
    keep_work.bRestart = 1;

    TEST_ASSERT_EQ_INT(ctx, -1, game());

    TEST_ASSERT_EQ_INT(ctx, 0, keep_work.bRestart);
    TEST_ASSERT_EQ_INT(ctx, 1, delete_ea_calls);
    TEST_ASSERT_EQ_INT(ctx, 1, cdpause_calls);
    TEST_ASSERT_EQ_INT(ctx, 2, last_cdpause);
    TEST_ASSERT_EQ_INT(ctx, 0, draw_ta_calls);
}

static void test_game_back_from_round_zero_starts_exit_fade(test_context *ctx) {
    reset_state();
    gMenuRound = 0;
    swdata1.b.l = 64;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 1, clwout_calls);
    TEST_ASSERT_EQ_INT(ctx, 0, delete_ea_calls);

    TEST_ASSERT_EQ_INT(ctx, -1, game());
    TEST_ASSERT_EQ_INT(ctx, 1, delete_ea_calls);
    TEST_ASSERT_EQ_INT(ctx, 2, last_cdpause);
}

static void test_game_idle_draws_and_flashes_special_palette(test_context *ctx) {
    reset_state();
    gMenu1 = 1;
    gMenuRound = 6;
    gTimer = 3;

    TEST_ASSERT_EQ_INT(ctx, 0, game());

    TEST_ASSERT_EQ_INT(ctx, 1, sp6_calls);
    TEST_ASSERT_EQ_INT(ctx, 1, tenmetu_calls);
    TEST_ASSERT_EQ_INT(ctx, 0, tamove_calls);
    TEST_ASSERT_EQ_INT(ctx, 1, draw_ta_calls);
}

static void test_game_changes_between_time_attack_and_special(test_context *ctx) {
    reset_state();
    score.total = 1;
    score.roundNo = 7;
    swdata1.b.h = 4;

    TEST_ASSERT_EQ_INT(ctx, 0, game());

    TEST_ASSERT_EQ_INT(ctx, 5, gMove);
    TEST_ASSERT_EQ_INT(ctx, 1, gNewMenu1);
    TEST_ASSERT_EQ_INT(ctx, 1, tamove_calls);
    TEST_ASSERT_EQ_INT(ctx, 1, draw_ta_calls);

    reset_state();
    gMenu1 = 1;
    swdata1.b.h = 8;

    TEST_ASSERT_EQ_INT(ctx, 0, game());

    TEST_ASSERT_EQ_INT(ctx, 6, gMove);
    TEST_ASSERT_EQ_INT(ctx, 0, gNewMenu1);
}

static void test_game_round_navigation_wraps_at_bounds(test_context *ctx) {
    reset_state();
    score.roundNo = 3;
    gMenuRound = 0;
    swdata1.b.h = 1;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 7, gMove);
    TEST_ASSERT_EQ_INT(ctx, 3, gNewMenuRound);

    reset_state();
    score.roundNo = 3;
    gMenuRound = 3;
    swdata1.b.h = 2;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 7, gMove);
    TEST_ASSERT_EQ_INT(ctx, 0, gNewMenuRound);
}

static void test_game_zone_and_menu_navigation_paths(test_context *ctx) {
    reset_state();
    gMenu2 = 2;
    gNewMenuZone = 0;
    swdata1.b.h = 1;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 8, gMove);
    TEST_ASSERT_EQ_INT(ctx, 2, gNewMenuZone);

    reset_state();
    gMenu2 = 2;
    gNewMenuZone = 2;
    swdata1.b.h = 2;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 9, gMove);
    TEST_ASSERT_EQ_INT(ctx, 0, gNewMenuZone);

    reset_state();
    gMenu2 = 1;
    swdata1.b.l = 8;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 10, gMove);
    TEST_ASSERT_EQ_INT(ctx, 2, gNewMenu2);
}

static void test_game_selects_stage_and_records_keep_work_menu(test_context *ctx) {
    reset_state();
    gMenu1 = 0;
    gMenu2 = 3;
    gMenuRound = 4;
    gMenuZone = 2;
    swdata1.b.l = 64;

    TEST_ASSERT_EQ_INT(ctx, 0, game());

    TEST_ASSERT_EQ_INT(ctx, 439, gRet);
    TEST_ASSERT_EQ_INT(ctx, 0, keep_work.ta_Menu1);
    TEST_ASSERT_EQ_INT(ctx, 4, keep_work.ta_Round);
    TEST_ASSERT_EQ_INT(ctx, 2, keep_work.ta_Zone);
    TEST_ASSERT_EQ_INT(ctx, 1, clwout_calls);

    reset_state();
    gMenu1 = 1;
    gMenu2 = 3;
    gMenuRound = 6;
    swdata1.b.l = 64;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 475, gRet);
}

static void test_game_rank_edit_save_and_character_change(test_context *ctx) {
    reset_state();
    gMenu1 = 0;
    gMenu2 = 5;
    gMenuRound = 2;
    gMenuZone = 1;
    gRankY = 0;
    gRankX = 2;
    swdata1.b.l = 64;
    read_score_result = 13;

    TEST_ASSERT_EQ_INT(ctx, 0, game());

    TEST_ASSERT_EQ_INT(ctx, 1, read_score_calls);
    TEST_ASSERT_EQ_INT(ctx, 1, set_score_date_calls);
    TEST_ASSERT_EQ_INT(ctx, 0, last_set_score_date);
    TEST_ASSERT_EQ_INT(ctx, 1, write_score_calls);
    TEST_ASSERT_EQ_INT(ctx, 13, last_write_index);
    TEST_ASSERT_EQ_INT(ctx, 12, gMove);
    TEST_ASSERT_EQ_INT(ctx, 3, gNewMenu2);

    reset_state();
    gMenu1 = 0;
    gMenu2 = 5;
    gMenuRound = 1;
    gMenuZone = 0;
    gRankY = 0;
    gRankX = 1;
    set_name(score.timeattack[0][0][0].name, 'A', '0', 'Z');
    swdata1.b.h = 2;
    KeyRepTimer = 15;

    TEST_ASSERT_EQ_INT(ctx, 0, game());

    TEST_ASSERT_EQ_INT(ctx, '1', score.timeattack[0][0][0].name[1]);
    TEST_ASSERT_EQ_INT(ctx, 12, gMove);
}

static void test_game_additional_menu_and_rank_edges(test_context *ctx) {
    reset_state();
    gMenuRound = 1;
    swdata1.b.l = 64;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 1, gMove);
    TEST_ASSERT_EQ_INT(ctx, 2, gNewMenu2);
    TEST_ASSERT_EQ_INT(ctx, 0, gMenuZone);

    reset_state();
    gMenu1 = 1;
    gMenuRound = 1;
    swdata1.b.l = 64;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 2, gMove);
    TEST_ASSERT_EQ_INT(ctx, 1, gNewMenu2);

    reset_state();
    gMenu2 = 4;
    swdata1.b.l = 1;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 11, gMove);
    TEST_ASSERT_EQ_INT(ctx, 5, gNewMenu2);

    reset_state();
    gMenu2 = 5;
    gRankX = 1;
    swdata1.b.l = 64;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 12, gMove);
    TEST_ASSERT_EQ_INT(ctx, 2, gNewRankX);

    reset_state();
    gMenu2 = 5;
    gRankX = 2;
    swdata1.b.l = 4;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 12, gMove);
    TEST_ASSERT_EQ_INT(ctx, 1, gNewRankX);

    reset_state();
    gMenu2 = 5;
    gRankX = 1;
    swdata1.b.l = 8;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 12, gMove);
    TEST_ASSERT_EQ_INT(ctx, 2, gNewRankX);

    reset_state();
    gMenu2 = 5;
    swdata1.b.l = 1;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 15, KeyRepTimer);
    TEST_ASSERT_EQ_INT(ctx, 0, gMove);
}

static void test_game_rank_character_wrap_edges(test_context *ctx) {
    reset_state();
    gMenu1 = 0;
    gMenu2 = 5;
    gMenuRound = 1;
    gRankX = 0;
    set_name(score.timeattack[0][0][0].name, '0', 'A', 'Z');
    swdata1.b.h = 1;
    KeyRepTimer = 15;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 'Z', score.timeattack[0][0][0].name[0]);

    reset_state();
    gMenu1 = 0;
    gMenu2 = 5;
    gMenuRound = 1;
    gRankX = 0;
    set_name(score.timeattack[0][0][0].name, 'A', 'A', 'Z');
    swdata1.b.h = 1;
    KeyRepTimer = 15;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, '9', score.timeattack[0][0][0].name[0]);

    reset_state();
    gMenu1 = 1;
    gMenu2 = 5;
    gMenuRound = 1;
    gRankX = 0;
    set_name(score.special[0][0].name, '9', 'A', 'Z');
    swdata1.b.h = 2;
    KeyRepTimer = 15;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 'A', score.special[0][0].name[0]);

    reset_state();
    gMenu1 = 1;
    gMenu2 = 5;
    gMenuRound = 1;
    gRankX = 0;
    set_name(score.special[0][0].name, 'Z', 'A', 'Z');
    swdata1.b.h = 2;
    KeyRepTimer = 15;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, '0', score.special[0][0].name[0]);
}

static void test_game_additional_submenu_edges(test_context *ctx) {
    reset_state();
    gMenu2 = 2;
    gNewMenuZone = 1;
    swdata1.b.h = 64;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 9, gMove);
    TEST_ASSERT_EQ_INT(ctx, 2, gNewMenuZone);

    reset_state();
    gMenu1 = 0;
    gMenu2 = 1;
    swdata1.b.l = 64;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 3, gMove);
    TEST_ASSERT_EQ_INT(ctx, 0, gMenu2);
    TEST_ASSERT_EQ_INT(ctx, 0, gNewMenu2);

    reset_state();
    gMenu1 = 1;
    gMenu2 = 1;
    swdata1.b.l = 64;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 4, gMove);

    reset_state();
    gMenu1 = 0;
    gMenu2 = 2;
    swdata1.b.l = 4;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 10, gMove);
    TEST_ASSERT_EQ_INT(ctx, 1, gNewMenu2);

    reset_state();
    gMenu1 = 0;
    gMenu2 = 3;
    swdata1.b.l = 4;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 10, gMove);
    TEST_ASSERT_EQ_INT(ctx, 2, gNewMenu2);

    reset_state();
    gMenu1 = 1;
    gMenu2 = 1;
    swdata1.b.l = 4;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 10, gMove);
    TEST_ASSERT_EQ_INT(ctx, 3, gNewMenu2);

    reset_state();
    gMenu1 = 0;
    gMenu2 = 2;
    swdata1.b.l = 8;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 10, gMove);
    TEST_ASSERT_EQ_INT(ctx, 3, gNewMenu2);

    reset_state();
    gMenu1 = 1;
    gMenu2 = 3;
    gNewMenu2 = 3;
    swdata1.b.l = 8;

    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 10, gMove);
    TEST_ASSERT_EQ_INT(ctx, 1, gNewMenu2);
}

static void test_game_init_default_menu_and_callbacks(test_context *ctx) {
    reset_state();
    keep_work.ta_time = (Uint32)-2;

    game_init();

    TEST_ASSERT_EQ_INT(ctx, 1, draw_context_calls);
    TEST_ASSERT_TRUE(ctx, s_ctx == &draw_ctx);
    TEST_ASSERT_EQ_INT(ctx, 0, gMenu1);
    TEST_ASSERT_EQ_INT(ctx, 0, gMenu2);
    TEST_ASSERT_EQ_INT(ctx, 1, gMenuRound);
    TEST_ASSERT_EQ_INT(ctx, 0, gMenuZone);
    TEST_ASSERT_EQ_INT(ctx, 1, color_set_calls);
    TEST_ASSERT_EQ_INT(ctx, 0, last_color_set_mode);
    TEST_ASSERT_EQ_INT(ctx, 1, ea_create_calls);
    TEST_ASSERT_EQ_INT(ctx, 1, clwin_init_calls);
    TEST_ASSERT_EQ_INT(ctx, 1, clwin_calls);
    TEST_ASSERT_EQ_INT(ctx, 1, cdplay_calls);
    TEST_ASSERT_EQ_INT(ctx, 33, last_cdplay);
}

static void test_game_init_inserts_time_attack_record_and_recomputes_total(
    test_context *ctx) {
    int round;
    int zone;

    reset_state();
    for (round = 0; round < 7; ++round) {
        for (zone = 0; zone < 3; ++zone) {
            score.timeattack[round][zone][0].time = 1000;
            score.timeattack[round][zone][1].time = 2000;
            score.timeattack[round][zone][2].time = 3000;
            set_name(score.timeattack[round][zone][0].name, 'A', 'A', 'A');
            set_name(score.timeattack[round][zone][1].name, 'B', 'B', 'B');
            set_name(score.timeattack[round][zone][2].name, 'C', 'C', 'C');
        }
    }
    keep_work.ta_time = 500;
    keep_work.ta_Menu1 = 0;
    keep_work.ta_Round = 2;
    keep_work.ta_Zone = 1;

    game_init();

    TEST_ASSERT_EQ_INT(ctx, 4, gMenu2);
    TEST_ASSERT_EQ_INT(ctx, 0, gRankY);
    TEST_ASSERT_EQ_INT(ctx, 500, score.timeattack[1][1][0].time);
    TEST_ASSERT_EQ_INT(ctx, 'Y', score.timeattack[1][1][0].name[0]);
    TEST_ASSERT_EQ_INT(ctx, 'O', score.timeattack[1][1][0].name[1]);
    TEST_ASSERT_EQ_INT(ctx, 'U', score.timeattack[1][1][0].name[2]);
    TEST_ASSERT_EQ_INT(ctx, 1000, score.timeattack[1][1][1].time);
    TEST_ASSERT_TRUE(ctx, score.total > 0);
    TEST_ASSERT_TRUE(ctx, strncpy_calls >= 3);
    TEST_ASSERT_TRUE(ctx, strncmp_calls >= 1);
}

static void test_game_init_inserts_special_record(test_context *ctx) {
    reset_state();
    score.special[2][0].time = 4000;
    score.special[2][1].time = 5000;
    score.special[2][2].time = 6000;
    set_name(score.special[2][0].name, 'A', 'A', 'A');
    set_name(score.special[2][1].name, 'A', 'A', 'A');
    set_name(score.special[2][2].name, 'C', 'C', 'C');
    keep_work.ta_time = 4500;
    keep_work.ta_Menu1 = 1;
    keep_work.ta_Round = 3;
    keep_work.ta_Zone = 0;

    game_init();

    TEST_ASSERT_EQ_INT(ctx, 4, gMenu2);
    TEST_ASSERT_EQ_INT(ctx, 1, gRankY);
    TEST_ASSERT_EQ_INT(ctx, 4500, score.special[2][1].time);
    TEST_ASSERT_EQ_INT(ctx, 'Y', score.special[2][1].name[0]);
    TEST_ASSERT_EQ_INT(ctx, 'O', score.special[2][1].name[1]);
    TEST_ASSERT_EQ_INT(ctx, 'U', score.special[2][1].name[2]);
    TEST_ASSERT_EQ_INT(ctx, 5000, score.special[2][2].time);
}

static void test_dll_meminit_wires_callbacks_and_buffers(test_context *ctx) {
    void *funcs[49];
    char *map_slot = (char *)map_work;
    char *color_slot = (char *)colorwk;
    char *color2_slot = (char *)colorwk2;
    char *color3_slot = (char *)colorwk3;
    char *color4_slot = (char *)colorwk4;
    char *hscroll_slot = (char *)hscroll_buffer;
    char env_storage;
    char loader_storage;
    char *env_slot = &env_storage;
    char *loader_slot = &loader_storage;
    char **bufs[13];
    int i;

    reset_state();
    for (i = 0; i < 49; ++i) {
        funcs[i] = 0;
    }
    funcs[4] = stub_cdplay;
    funcs[5] = stub_cdpause;
    funcs[7] = stub_read_score_index;
    funcs[8] = stub_write_score_data;
    funcs[9] = stub_set_score_date;
    funcs[11] = stub_mem_alloc;
    funcs[12] = stub_mem_free;
    funcs[18] = stub_strncpy;
    funcs[19] = stub_strncmp;
    funcs[20] = stub_printf;
    funcs[21] = stub_output_debug_string;
    funcs[22] = stub_open_file;
    funcs[23] = stub_read_file;
    funcs[24] = stub_close_file;
    funcs[26] = stub_get_draw_context;
    funcs[27] = stub_flip_to_screen;
    funcs[28] = stub_ld_bitmap_4to8;
    funcs[29] = stub_ld_load_cmpfile;
    funcs[30] = stub_background_set;
    funcs[31] = stub_bitmap_create;
    funcs[32] = stub_bitmap_release;
    funcs[33] = stub_bitmap_get_scan0;
    funcs[34] = stub_bitmap_set_transparency;
    funcs[35] = stub_hmx_free;
    funcs[36] = stub_grid_create;
    funcs[37] = stub_grid_release;
    funcs[38] = stub_grid_base;
    funcs[39] = stub_grid_set_position;
    funcs[40] = stub_grid_set_view;
    funcs[41] = stub_grid_set_tile;
    funcs[43] = stub_renderer_add;
    funcs[44] = stub_renderer_clear;
    funcs[45] = stub_renderer_draw;
    funcs[46] = stub_sprite_base;
    funcs[47] = stub_sprite_set_position;
    funcs[48] = stub_sprite_set_bitmap;

    bufs[0] = &map_slot;
    bufs[1] = &color_slot;
    bufs[2] = &color2_slot;
    bufs[3] = &color3_slot;
    bufs[4] = &color4_slot;
    bufs[5] = &hscroll_slot;
    bufs[6] = (char **)&fade_flag;
    bufs[7] = (char **)&keep_work;
    bufs[8] = 0;
    bufs[9] = 0;
    bufs[10] = (char **)&score;
    bufs[11] = &env_slot;
    bufs[12] = &loader_slot;

    DLL_meminit(bufs, funcs);

    TEST_ASSERT_TRUE(ctx, CDPlay == stub_cdplay);
    TEST_ASSERT_TRUE(ctx, CDPause == stub_cdpause);
    TEST_ASSERT_TRUE(ctx, sStrncpy == stub_strncpy);
    TEST_ASSERT_TRUE(ctx, get_draw_context_module == stub_get_draw_context);
    TEST_ASSERT_TRUE(ctx, hmx_sprite_set_bitmap_module == stub_sprite_set_bitmap);
    TEST_ASSERT_TRUE(ctx, pmapwk == map_work);
    TEST_ASSERT_TRUE(ctx, lpcolorwk == colorwk);
    TEST_ASSERT_TRUE(ctx, lphscrollbuff == hscroll_buffer);
    TEST_ASSERT_TRUE(ctx, lpFadeFlag == &fade_flag);
    TEST_ASSERT_TRUE(ctx, lpKeepWork == &keep_work);
    TEST_ASSERT_TRUE(ctx, lpScoreData == &score);
    TEST_ASSERT_TRUE(ctx, g_env_module == (hmx_environment *)&env_storage);
    TEST_ASSERT_TRUE(ctx,
                     g_loader_module == (hmx_environment *)&loader_storage);
}

static void test_export_table_points_at_entry_points(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, ExportedFunctions.game_init == game_init);
    TEST_ASSERT_TRUE(ctx, ExportedFunctions.dll_meminit == DLL_meminit);
    TEST_ASSERT_TRUE(ctx, ExportedFunctions.dll_memfree == DLL_memfree);
    TEST_ASSERT_NOT_NULL(ctx, ExportedFunctions.game);
    TEST_ASSERT_NOT_NULL(ctx, ExportedFunctions.SWdataSet);
    TEST_ASSERT_TRUE(ctx, ExportedFunctions.FadeProc == FadeProc);
}

TEST_MAIN_BEGIN;
test_swdata_set_copies_both_words(&ctx);
test_special_play_check_uses_total_and_round_gate(&ctx);
test_get_play_round_defaults_and_caps(&ctx);
test_game_restart_exits_immediately(&ctx);
test_game_back_from_round_zero_starts_exit_fade(&ctx);
test_game_idle_draws_and_flashes_special_palette(&ctx);
test_game_changes_between_time_attack_and_special(&ctx);
test_game_round_navigation_wraps_at_bounds(&ctx);
test_game_zone_and_menu_navigation_paths(&ctx);
test_game_selects_stage_and_records_keep_work_menu(&ctx);
test_game_rank_edit_save_and_character_change(&ctx);
test_game_additional_menu_and_rank_edges(&ctx);
test_game_rank_character_wrap_edges(&ctx);
test_game_additional_submenu_edges(&ctx);
test_game_init_default_menu_and_callbacks(&ctx);
test_game_init_inserts_time_attack_record_and_recomputes_total(&ctx);
test_game_init_inserts_special_record(&ctx);
test_dll_meminit_wires_callbacks_and_buffers(&ctx);
test_export_table_points_at_entry_points(&ctx);
DLL_memfree();
TEST_MAIN_END
