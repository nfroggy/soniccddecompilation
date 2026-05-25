#include <string.h>

#include "support/test_runner.h"
#include "src/title/common/hmx_types.h"
#include "src/types.h"

Uint16 nSequenceNum;
Uint16 swData1;
Sint32 nTimerCunt;
draw_context context;
draw_context *s_ctx;
hmx_environment *g_env_module;
void (*hmx_sprite_set_bitmap_module)(hmx_sprite *, hmx_bitmap *);
void (*hmx_sprite_set_position_module)(hmx_sprite *, Sint32, Sint32);
void *(*hmx_bitmap_get_scan0_module)(hmx_bitmap *);
hmx_bitmap *(*hmx_bitmap_create_module)(hmx_environment *, Sint32, Sint32);
void (*hmx_bitmap_release_module)(hmx_environment *, hmx_bitmap *);
void (*CDPause)(Sint16);
void (*CDPlay)(Sint16);
void (*WaveRequest)(Sint16);

static hmx_sprite *sprite_table[1000];
static char sprite_storage[1000];
static char bitmap_storage[4];
static char scan_storage[320 * 224];
static Sint32 draw_call_count;
static Sint32 grid_create_call_count;
static Uint16 grid_create_values[8];
static Sint32 spr_create_call_count;
static Sint32 spr_create_values[64];
static Sint32 str_pcm_call_count;
static Sint32 str_pcm_values[64];
static Sint32 str_da_call_count;
static Sint32 str_da_values[64];
static Sint32 str_pcmno_call_count;
static Sint32 str_pcmno_values[64];
static Sint32 str_dano_call_count;
static Sint32 str_dano_values[64];
static Sint32 set_sequence_call_count;
static Uint16 set_sequence_values[64];
static Sint32 key_wait_call_count;
static Sint32 wait_key_off_call_count;
static Sint32 delete_ea_call_count;
static Sint32 delete_all_char_call_count;
static Sint32 cd_pause_call_count;
static Sint16 cd_pause_values[64];
static Sint32 cd_play_call_count;
static Sint16 cd_play_values[64];
static Sint32 wave_call_count;
static Sint16 wave_values[64];
static Sint32 welcome_call_count;
static Sint32 clrset_call_count;
static PALETTEENTRY *clrset_palettes[32];
static Uint16 clrset_numbers[32];
static Sint32 bitmap_create_call_count;
static Sint32 bitmap_create_widths[8];
static Sint32 bitmap_create_heights[8];
static Sint32 bitmap_release_call_count;
static hmx_bitmap *bitmap_release_values[8];
static Sint32 bitmap_get_scan_call_count;
static Sint32 bitmap_file_call_count;
static char bitmap_file_names[16][32];
static Sint32 bitmap_file2_call_count;
static char bitmap_file2_names[16][32];
static Sint32 position_call_count;
static hmx_sprite *position_sprites[16];
static Sint32 position_x[16];
static Sint32 position_y[16];
static Sint32 set_bitmap_call_count;
static hmx_sprite *set_bitmap_sprites[16];
static hmx_bitmap *set_bitmap_values[16];
static Sint32 delete_mes_call_count;

#include "src/title/soundtst/snddo.c"

void srfDraw(void) { ++draw_call_count; }

Sint32 OEGridCreate(Uint16 indx) {
    grid_create_values[grid_create_call_count] = indx;
    ++grid_create_call_count;
    return 0;
}

Sint32 OESprCreateIndx(Sint32 indx) {
    spr_create_values[spr_create_call_count] = indx;
    ++spr_create_call_count;
    return 0;
}

void StrPCM(Sint32 kind) {
    str_pcm_values[str_pcm_call_count] = kind;
    ++str_pcm_call_count;
}

void StrDA(Sint32 kind) {
    str_da_values[str_da_call_count] = kind;
    ++str_da_call_count;
}

void StrPCMNO(Sint32 num) {
    str_pcmno_values[str_pcmno_call_count] = num;
    ++str_pcmno_call_count;
}

void StrDANO(Sint32 num) {
    str_dano_values[str_dano_call_count] = num;
    ++str_dano_call_count;
}

void OESetSequenceNum(Uint16 num) {
    set_sequence_values[set_sequence_call_count] = num;
    ++set_sequence_call_count;
    nSequenceNum = num;
    nTimerCunt = -1;
}

void KeyWait(void) { ++key_wait_call_count; }

void WaitKeyOff(void) { ++wait_key_off_call_count; }

void OEDeleteEA(void) { ++delete_ea_call_count; }

void DeleteAllCharSprt(void) { ++delete_all_char_call_count; }

static void fake_cd_pause(Sint16 mode) {
    cd_pause_values[cd_pause_call_count] = mode;
    ++cd_pause_call_count;
}

static void fake_cd_play(Sint16 track) {
    cd_play_values[cd_play_call_count] = track;
    ++cd_play_call_count;
}

static void fake_wave_request(Sint16 wave) {
    wave_values[wave_call_count] = wave;
    ++wave_call_count;
}

void StrWelcome(void) { ++welcome_call_count; }

void OEClrset(PALETTEENTRY *pPalet, Uint16 nNumber) {
    clrset_palettes[clrset_call_count] = pPalet;
    clrset_numbers[clrset_call_count] = nNumber;
    ++clrset_call_count;
}

static hmx_bitmap *fake_bitmap_create(hmx_environment *env, Sint32 width,
                                      Sint32 height) {
    (void)env;
    bitmap_create_widths[bitmap_create_call_count] = width;
    bitmap_create_heights[bitmap_create_call_count] = height;
    ++bitmap_create_call_count;
    return (hmx_bitmap *)&bitmap_storage[0];
}

static void fake_bitmap_release(hmx_environment *env, hmx_bitmap *bitmap) {
    (void)env;
    bitmap_release_values[bitmap_release_call_count] = bitmap;
    ++bitmap_release_call_count;
}

static void *fake_bitmap_get_scan0(hmx_bitmap *bitmap) {
    (void)bitmap;
    ++bitmap_get_scan_call_count;
    return scan_storage;
}

void ld_bitmap_file(char *fname, void *dst, Sint32 wx, Sint32 wy, Sint32 palet,
                    Sint32 unknown) {
    (void)dst;
    (void)wx;
    (void)wy;
    (void)palet;
    (void)unknown;
    strcpy(bitmap_file_names[bitmap_file_call_count], fname);
    ++bitmap_file_call_count;
}

void ld_bitmap_file2(char *fname, void *dst, Sint32 sx, Sint32 sy, Sint32 wx,
                     Sint32 wy, Sint32 dx, Sint32 dy, Sint32 unk1,
                     Sint32 unk2) {
    (void)dst;
    (void)sx;
    (void)sy;
    (void)wx;
    (void)wy;
    (void)dx;
    (void)dy;
    (void)unk1;
    (void)unk2;
    strcpy(bitmap_file2_names[bitmap_file2_call_count], fname);
    ++bitmap_file2_call_count;
}

static hmx_sprite *sprite_handle(Sint32 handle) {
    return (hmx_sprite *)&sprite_storage[handle];
}

static void fake_sprite_set_position(hmx_sprite *sprite, Sint32 x, Sint32 y) {
    position_sprites[position_call_count] = sprite;
    position_x[position_call_count] = x;
    position_y[position_call_count] = y;
    ++position_call_count;
}

static void fake_sprite_set_bitmap(hmx_sprite *sprite, hmx_bitmap *bitmap) {
    set_bitmap_sprites[set_bitmap_call_count] = sprite;
    set_bitmap_values[set_bitmap_call_count] = bitmap;
    ++set_bitmap_call_count;
}

void DeleteCharMes(void) { ++delete_mes_call_count; }

static void reset_fixture(void) {
    Sint32 i;

    memset(&context, 0, sizeof(context));
    memset(sprite_table, 0, sizeof(sprite_table));
    memset(spr_create_values, 0, sizeof(spr_create_values));
    memset(str_pcm_values, 0, sizeof(str_pcm_values));
    memset(str_da_values, 0, sizeof(str_da_values));
    memset(str_pcmno_values, 0, sizeof(str_pcmno_values));
    memset(str_dano_values, 0, sizeof(str_dano_values));
    memset(set_sequence_values, 0, sizeof(set_sequence_values));
    memset(cd_pause_values, 0, sizeof(cd_pause_values));
    memset(cd_play_values, 0, sizeof(cd_play_values));
    memset(wave_values, 0, sizeof(wave_values));
    memset(clrset_palettes, 0, sizeof(clrset_palettes));
    memset(clrset_numbers, 0, sizeof(clrset_numbers));
    memset(bitmap_create_widths, 0, sizeof(bitmap_create_widths));
    memset(bitmap_create_heights, 0, sizeof(bitmap_create_heights));
    memset(bitmap_release_values, 0, sizeof(bitmap_release_values));
    memset(bitmap_file_names, 0, sizeof(bitmap_file_names));
    memset(bitmap_file2_names, 0, sizeof(bitmap_file2_names));
    memset(position_sprites, 0, sizeof(position_sprites));
    memset(position_x, 0, sizeof(position_x));
    memset(position_y, 0, sizeof(position_y));
    memset(set_bitmap_sprites, 0, sizeof(set_bitmap_sprites));
    memset(set_bitmap_values, 0, sizeof(set_bitmap_values));

    for (i = 0; i < 1000; ++i) {
        sprite_table[i] = sprite_handle(i);
    }
    context.sprites = sprite_table;
    s_ctx = &context;
    g_env_module = (hmx_environment *)&bitmap_storage[1];
    hmx_bitmap_create_module = fake_bitmap_create;
    hmx_bitmap_release_module = fake_bitmap_release;
    hmx_bitmap_get_scan0_module = fake_bitmap_get_scan0;
    hmx_sprite_set_position_module = fake_sprite_set_position;
    hmx_sprite_set_bitmap_module = fake_sprite_set_bitmap;
    CDPause = fake_cd_pause;
    CDPlay = fake_cd_play;
    WaveRequest = fake_wave_request;

    nSequenceNum = 0;
    nTimerCunt = 0;
    swData1 = 0;
    nMenuKind = 1;
    nPCMNumberIndx = 0;
    nDANumberIndx = 0;
    bKakusi = 0;
    hmapwk = 0;
    hBkg = 0;
    hBmpBkg = 0;
    HorizInit = 0;
    draw_call_count = 0;
    grid_create_call_count = 0;
    spr_create_call_count = 0;
    str_pcm_call_count = 0;
    str_da_call_count = 0;
    str_pcmno_call_count = 0;
    str_dano_call_count = 0;
    set_sequence_call_count = 0;
    key_wait_call_count = 0;
    wait_key_off_call_count = 0;
    delete_ea_call_count = 0;
    delete_all_char_call_count = 0;
    cd_pause_call_count = 0;
    cd_play_call_count = 0;
    wave_call_count = 0;
    welcome_call_count = 0;
    clrset_call_count = 0;
    bitmap_create_call_count = 0;
    bitmap_release_call_count = 0;
    bitmap_get_scan_call_count = 0;
    bitmap_file_call_count = 0;
    bitmap_file2_call_count = 0;
    position_call_count = 0;
    set_bitmap_call_count = 0;
    delete_mes_call_count = 0;
}

static void test_draw_start_and_menu_highlight(test_context *ctx) {
    Sint32 i;

    reset_fixture();
    nSequenceNum = 1;
    SNDDraw();
    TEST_ASSERT_EQ_INT(ctx, 0, draw_call_count);
    nSequenceNum = 2;
    SNDDraw();
    TEST_ASSERT_EQ_INT(ctx, 1, draw_call_count);

    SNDStart();

    TEST_ASSERT_EQ_INT(ctx, 1, grid_create_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, grid_create_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 25, spr_create_call_count);
    for (i = 0; i < 16; ++i) {
        TEST_ASSERT_EQ_INT(ctx, 83 + i, spr_create_values[i]);
    }
    for (i = 0; i < 9; ++i) {
        TEST_ASSERT_EQ_INT(ctx, i, spr_create_values[16 + i]);
    }
    TEST_ASSERT_EQ_INT(ctx, 1, nMenuKind);
    TEST_ASSERT_EQ_INT(ctx, 2, str_pcm_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, str_da_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, str_pcmno_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, str_dano_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, set_sequence_values[0]);
}

static void test_menu_vertical_navigation_and_boundaries(test_context *ctx) {
    reset_fixture();
    swData1 = 2048;
    nMenuKind = 1;
    SNDMenu();
    TEST_ASSERT_EQ_INT(ctx, 2, nMenuKind);
    TEST_ASSERT_EQ_INT(ctx, 1, str_pcm_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, str_da_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, key_wait_call_count);

    SNDMenu();
    TEST_ASSERT_EQ_INT(ctx, 2, nMenuKind);
    TEST_ASSERT_EQ_INT(ctx, 1, key_wait_call_count);

    swData1 = 1024;
    SNDMenu();
    TEST_ASSERT_EQ_INT(ctx, 1, nMenuKind);
    TEST_ASSERT_EQ_INT(ctx, 2, str_pcm_values[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, str_da_values[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, key_wait_call_count);

    SNDMenu();
    TEST_ASSERT_EQ_INT(ctx, 1, nMenuKind);
    TEST_ASSERT_EQ_INT(ctx, 2, key_wait_call_count);

    reset_fixture();
    swData1 = 2048;
    nMenuKind = 0;
    SNDMenu();
    TEST_ASSERT_EQ_INT(ctx, 1, nMenuKind);
    TEST_ASSERT_EQ_INT(ctx, 1, key_wait_call_count);

    reset_fixture();
    swData1 = 1024;
    nMenuKind = 3;
    SNDMenu();
    TEST_ASSERT_EQ_INT(ctx, 2, nMenuKind);
    TEST_ASSERT_EQ_INT(ctx, 1, key_wait_call_count);
}

static void test_menu_number_navigation_wraps_pcm_and_da(test_context *ctx) {
    reset_fixture();
    swData1 = 256;
    nMenuKind = 1;
    nPCMNumberIndx = 0;
    SNDMenu();
    TEST_ASSERT_EQ_INT(ctx, 79, nPCMNumberIndx);
    TEST_ASSERT_EQ_INT(ctx, 79, str_pcmno_values[0]);

    nPCMNumberIndx = 10;
    SNDMenu();
    TEST_ASSERT_EQ_INT(ctx, 9, nPCMNumberIndx);
    TEST_ASSERT_EQ_INT(ctx, 9, str_pcmno_values[1]);

    swData1 = 512;
    nPCMNumberIndx = 79;
    SNDMenu();
    TEST_ASSERT_EQ_INT(ctx, 0, nPCMNumberIndx);
    TEST_ASSERT_EQ_INT(ctx, 0, str_pcmno_values[2]);

    nPCMNumberIndx = 10;
    SNDMenu();
    TEST_ASSERT_EQ_INT(ctx, 11, nPCMNumberIndx);
    TEST_ASSERT_EQ_INT(ctx, 11, str_pcmno_values[3]);

    reset_fixture();
    nMenuKind = 2;
    swData1 = 256;
    nDANumberIndx = 0;
    SNDMenu();
    TEST_ASSERT_EQ_INT(ctx, 33, nDANumberIndx);
    TEST_ASSERT_EQ_INT(ctx, 33, str_dano_values[0]);

    nDANumberIndx = 10;
    SNDMenu();
    TEST_ASSERT_EQ_INT(ctx, 9, nDANumberIndx);
    TEST_ASSERT_EQ_INT(ctx, 9, str_dano_values[1]);

    swData1 = 512;
    nDANumberIndx = 33;
    SNDMenu();
    TEST_ASSERT_EQ_INT(ctx, 0, nDANumberIndx);
    TEST_ASSERT_EQ_INT(ctx, 0, str_dano_values[2]);

    nDANumberIndx = 10;
    SNDMenu();
    TEST_ASSERT_EQ_INT(ctx, 11, nDANumberIndx);
    TEST_ASSERT_EQ_INT(ctx, 11, str_dano_values[3]);
}

static void assert_hidden_trigger(test_context *ctx, Sint32 pcm, Sint32 da,
                                  Sint32 cg, Sint32 cd, Uint16 seq,
                                  Sint32 kakusi) {
    reset_fixture();
    swData1 = 16384;
    nPCMNumberIndx = (Uint16)pcm;
    nDANumberIndx = (Uint16)da;

    SNDMenu();

    TEST_ASSERT_EQ_INT(ctx, 1, delete_ea_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, delete_all_char_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, bitmap_create_call_count);
    TEST_ASSERT_EQ_INT(ctx, cd, cd_play_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, cd_pause_values[0]);
    TEST_ASSERT_EQ_INT(ctx, seq, nSequenceNum);
    TEST_ASSERT_EQ_INT(ctx, kakusi, bKakusi);
    TEST_ASSERT_EQ_INT(ctx, cg == 4 ? 2 : 1, clrset_call_count);
}

static void test_menu_accept_hidden_and_normal_actions(test_context *ctx) {
    assert_hidden_trigger(ctx, 3, 1, 0, 21, 4, 1);
    assert_hidden_trigger(ctx, 12, 25, 1, 24, 4, 1);
    assert_hidden_trigger(ctx, 4, 21, 2, 25, 4, 1);
    assert_hidden_trigger(ctx, 11, 9, 3, 4, 4, 1);
    assert_hidden_trigger(ctx, 12, 11, 4, 33, 4, 2);

    reset_fixture();
    swData1 = 16384;
    nPCMNumberIndx = 7;
    nDANumberIndx = 7;
    SNDMenu();
    TEST_ASSERT_EQ_INT(ctx, 2, cd_pause_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, welcome_call_count);
    TEST_ASSERT_EQ_INT(ctx, 5, nSequenceNum);

    reset_fixture();
    swData1 = 16384;
    nMenuKind = 1;
    nPCMNumberIndx = 5;
    SNDMenu();
    TEST_ASSERT_EQ_INT(ctx, 5, wave_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, wait_key_off_call_count);

    reset_fixture();
    swData1 = 16384;
    nMenuKind = 2;
    nDANumberIndx = 4;
    SNDMenu();
    TEST_ASSERT_EQ_INT(ctx, 2, cd_pause_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 4, cd_play_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, wait_key_off_call_count);
}

static void test_menu_cancel_and_end_returns(test_context *ctx) {
    reset_fixture();
    swData1 = 4096;
    SNDMenu();
    TEST_ASSERT_EQ_INT(ctx, 2, cd_pause_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 999, nSequenceNum);

    nTimerCunt = 1;
    TEST_ASSERT_EQ_INT(ctx, 0, SNDEnd());
    TEST_ASSERT_EQ_INT(ctx, 0, SNDEnd2());
    TEST_ASSERT_EQ_INT(ctx, 0, SNDEnd3());
    nTimerCunt = 0;
    TEST_ASSERT_EQ_INT(ctx, 9, SNDEnd());
    TEST_ASSERT_EQ_INT(ctx, 18, SNDEnd2());
    TEST_ASSERT_EQ_INT(ctx, 19, SNDEnd3());
}

static void test_mc_sonic_create_and_delete_all_variants(test_context *ctx) {
    static const char *names[] = {"HOSHINO3.BMP", "NISIMUR3.BMP",
                                  "MIYAKE3.BMP",  "SANPEI3.BMP"};
    Sint32 i;

    for (i = 0; i < 4; ++i) {
        reset_fixture();
        TEST_ASSERT_EQ_INT(ctx, 0, MC_SONICCreate((Sint16)i));
        TEST_ASSERT_EQ_INT(ctx, 1, clrset_call_count);
        TEST_ASSERT_EQ_INT(ctx, 0, clrset_numbers[0]);
        TEST_ASSERT_EQ_INT(ctx, 1, bitmap_create_call_count);
        TEST_ASSERT_EQ_INT(ctx, 320, bitmap_create_widths[0]);
        TEST_ASSERT_EQ_INT(ctx, 224, bitmap_create_heights[0]);
        TEST_ASSERT_EQ_INT(ctx, 1, bitmap_file_call_count);
        TEST_ASSERT_TRUE(ctx, strcmp(bitmap_file_names[0], names[i]) == 0);
        TEST_ASSERT_TRUE(ctx, position_sprites[0] == sprite_handle(0));
        TEST_ASSERT_EQ_INT(ctx, 0, position_x[0]);
        TEST_ASSERT_EQ_INT(ctx, 0, position_y[0]);
        TEST_ASSERT_TRUE(ctx, set_bitmap_values[0] == context.spr_bitmaps[0]);
        TEST_ASSERT_EQ_INT(ctx, 1, context.spr_level[0]);

        TEST_ASSERT_EQ_INT(ctx, 0, MC_SONICDelete());
        TEST_ASSERT_EQ_INT(ctx, 1, bitmap_release_call_count);
        TEST_ASSERT_EQ_INT(ctx, 0, context.spr_bitmaps[0]);
        TEST_ASSERT_EQ_INT(ctx, 0, context.spr_level[0]);
    }

    reset_fixture();
    TEST_ASSERT_EQ_INT(ctx, 0, MC_SONICCreate(4));
    TEST_ASSERT_EQ_INT(ctx, 2, clrset_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, clrset_numbers[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, clrset_numbers[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, bitmap_file_call_count);
    TEST_ASSERT_EQ_INT(ctx, 2, bitmap_file2_call_count);
    TEST_ASSERT_TRUE(ctx, strcmp(bitmap_file2_names[0], "YAMA_L3.BMP") == 0);
    TEST_ASSERT_TRUE(ctx, strcmp(bitmap_file2_names[1], "YAMA_R3.BMP") == 0);
}

static void test_kakusi_and_special_message_exit_paths(test_context *ctx) {
    reset_fixture();
    bKakusi = 1;
    swData1 = 0;
    SNDKakusi();
    TEST_ASSERT_EQ_INT(ctx, 5, bKakusi);
    TEST_ASSERT_EQ_INT(ctx, 0, set_sequence_call_count);
    swData1 = 16384;
    SNDKakusi();
    TEST_ASSERT_EQ_INT(ctx, 999, nSequenceNum);

    reset_fixture();
    bKakusi = 2;
    swData1 = 0;
    SNDKakusi();
    swData1 = 4096;
    SNDKakusi();
    TEST_ASSERT_EQ_INT(ctx, 7, nSequenceNum);

    reset_fixture();
    bKakusi = 0;
    swData1 = 0;
    SNDSpecialMes();
    TEST_ASSERT_EQ_INT(ctx, 4, bKakusi);
    TEST_ASSERT_EQ_INT(ctx, 0, delete_mes_call_count);
    swData1 = 16384;
    SNDSpecialMes();
    TEST_ASSERT_EQ_INT(ctx, 1, delete_mes_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, bKakusi);
    TEST_ASSERT_EQ_INT(ctx, 6, nSequenceNum);
}

TEST_MAIN_BEGIN;
test_draw_start_and_menu_highlight(&ctx);
test_menu_vertical_navigation_and_boundaries(&ctx);
test_menu_number_navigation_wraps_pcm_and_da(&ctx);
test_menu_accept_hidden_and_normal_actions(&ctx);
test_menu_cancel_and_end_returns(&ctx);
test_mc_sonic_create_and_delete_all_variants(&ctx);
test_kakusi_and_special_message_exit_paths(&ctx);
TEST_MAIN_END
