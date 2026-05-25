#include "support/test_runner.h"
#include "src/types.h"

static Sint32 movie_play_call_count;
static Sint32 movie_play_values[8];
static Sint32 movie_stop_call_count;
static Sint32 movie_stat_call_count;
static Sint32 movie_stat_return;
static Sint32 set_sequence_call_count;
static Uint16 set_sequence_values[8];
static Uint32 fullscreen_flag;
static Uint16 nSequenceNum;

Sint32 (*sMovieStat)(void);
Sint32 (*sMoviePlay)(Sint32);
Sint32 (*sMovieStop)(void);
Sint32 nTimerCunt;
Uint32 *lpbFullScreen;
Uint16 swData1;

#define AVIGOOD
#include "src/title/avigood/avigoddo.c"
#undef AVIGOOD

void OESetSequenceNum(Uint16 num) {
    set_sequence_values[set_sequence_call_count] = num;
    ++set_sequence_call_count;
    nSequenceNum = num;
    nTimerCunt = -1;
}

static Sint32 fake_movie_play(Sint32 movie) {
    movie_play_values[movie_play_call_count] = movie;
    ++movie_play_call_count;
    return 0;
}

static Sint32 fake_movie_stop(void) {
    ++movie_stop_call_count;
    return 0;
}

static Sint32 fake_movie_stat(void) {
    ++movie_stat_call_count;
    return movie_stat_return;
}

static void reset_fixture(void) {
    Sint32 i;

    for (i = 0; i < 8; ++i) {
        movie_play_values[i] = 0;
        set_sequence_values[i] = 0;
    }

    movie_play_call_count = 0;
    movie_stop_call_count = 0;
    movie_stat_call_count = 0;
    movie_stat_return = 0;
    set_sequence_call_count = 0;
    fullscreen_flag = 0;
    nSequenceNum = 0;
    nTimerCunt = 0;
    nPlayFileNum = 0;
    nDispBMPFileNum = 0;
    swData1 = 0;
    lpbFullScreen = &fullscreen_flag;
    sMoviePlay = fake_movie_play;
    sMovieStop = fake_movie_stop;
    sMovieStat = fake_movie_stat;
}

static void test_read_and_free_dib_are_noops(test_context *ctx) {
    reset_fixture();
    nDispBMPFileNum = 7;

    ReadDIB();
    FreeDIB();

    TEST_ASSERT_EQ_INT(ctx, 0, movie_play_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, movie_stop_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, movie_stat_call_count);
    TEST_ASSERT_EQ_INT(ctx, 7, nDispBMPFileNum);
}

static void test_opening_move_starts_first_good_movie_on_first_tick(
    test_context *ctx) {
    reset_fixture();

    AVIOpeningMove();

    TEST_ASSERT_EQ_INT(ctx, 1, movie_play_call_count);
    TEST_ASSERT_EQ_INT(ctx, 4, movie_play_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, movie_stat_call_count);
}

static void test_opening_move_advances_only_when_movie_stat_is_three(
    test_context *ctx) {
    reset_fixture();
    nTimerCunt = 1;
    movie_stat_return = 2;

    AVIOpeningMove();

    TEST_ASSERT_EQ_INT(ctx, 1, movie_stat_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, movie_stop_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, movie_play_call_count);

    movie_stat_return = 3;
    AVIOpeningMove();

    TEST_ASSERT_EQ_INT(ctx, 2, movie_stat_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, movie_stop_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, movie_play_call_count);
    TEST_ASSERT_EQ_INT(ctx, 5, movie_play_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, nPlayFileNum);
}

static void test_play_next_avi_stops_and_finishes_after_second_movie(
    test_context *ctx) {
    reset_fixture();
    nPlayFileNum = 1;
    nTimerCunt = 44;

    PlayNextAVI();

    TEST_ASSERT_EQ_INT(ctx, 2, nPlayFileNum);
    TEST_ASSERT_EQ_INT(ctx, 1, movie_stop_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, movie_play_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, set_sequence_call_count);
    TEST_ASSERT_EQ_INT(ctx, 2, set_sequence_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, nSequenceNum);
    TEST_ASSERT_EQ_INT(ctx, -1, nTimerCunt);
}

static void test_avi_end_always_stops_movie(test_context *ctx) {
    reset_fixture();

    AVIEnd();

    TEST_ASSERT_EQ_INT(ctx, 1, movie_stop_call_count);
}

static void test_avi_end_wait_fullscreen_and_timeout_paths(test_context *ctx) {
    reset_fixture();

    fullscreen_flag = 1;
    nTimerCunt = 5;
    AVIEndWait();
    TEST_ASSERT_EQ_INT(ctx, 1, set_sequence_call_count);
    TEST_ASSERT_EQ_INT(ctx, 3, set_sequence_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, nDispBMPFileNum);

    reset_fixture();
    nTimerCunt = 120;
    AVIEndWait();
    TEST_ASSERT_EQ_INT(ctx, 0, set_sequence_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, nDispBMPFileNum);

    nTimerCunt = 121;
    AVIEndWait();
    TEST_ASSERT_EQ_INT(ctx, 1, set_sequence_call_count);
    TEST_ASSERT_EQ_INT(ctx, 3, set_sequence_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, nDispBMPFileNum);
}

static void test_avi_end_key_wait_returns_good_ending_on_input_or_fullscreen(
    test_context *ctx) {
    reset_fixture();

    TEST_ASSERT_EQ_INT(ctx, 0, AVIEndKeyWait());

    swData1 = 16;
    TEST_ASSERT_EQ_INT(ctx, 16, AVIEndKeyWait());

    swData1 = 0;
    fullscreen_flag = 1;
    TEST_ASSERT_EQ_INT(ctx, 16, AVIEndKeyWait());
}

TEST_MAIN_BEGIN;
test_read_and_free_dib_are_noops(&ctx);
test_opening_move_starts_first_good_movie_on_first_tick(&ctx);
test_opening_move_advances_only_when_movie_stat_is_three(&ctx);
test_play_next_avi_stops_and_finishes_after_second_movie(&ctx);
test_avi_end_always_stops_movie(&ctx);
test_avi_end_wait_fullscreen_and_timeout_paths(&ctx);
test_avi_end_key_wait_returns_good_ending_on_input_or_fullscreen(&ctx);
TEST_MAIN_END
