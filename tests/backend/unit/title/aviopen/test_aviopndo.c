#include "support/test_runner.h"
#include "src/types.h"

static Sint32 movie_play_call_count;
static Sint32 movie_play_values[4];
static Sint32 movie_stop_call_count;

Sint32 (*sMoviePlay)(Sint32);
Sint32 (*sMovieStop)(void);
Sint32 nTimerCunt;
Uint16 wMCIDeviceID;

#define AVIOPEN
#include "src/title/aviopen/aviopndo.c"
#undef AVIOPEN

static Sint32 fake_movie_play(Sint32 movie) {
    movie_play_values[movie_play_call_count] = movie;
    ++movie_play_call_count;
    return 0;
}

static Sint32 fake_movie_stop(void) {
    ++movie_stop_call_count;
    return 0;
}

static void reset_fixture(void) {
    movie_play_call_count = 0;
    movie_play_values[0] = 0;
    movie_play_values[1] = 0;
    movie_play_values[2] = 0;
    movie_play_values[3] = 0;
    movie_stop_call_count = 0;
    sMoviePlay = fake_movie_play;
    sMovieStop = fake_movie_stop;
    nTimerCunt = 0;
    wMCIDeviceID = 123;
    bAVIStart = 99;
}

static void test_read_free_and_paint_are_noops(test_context *ctx) {
    reset_fixture();

    ReadDIB("opening.bmp");
    FreeDIB();
    AVIPaint(55);

    TEST_ASSERT_EQ_INT(ctx, 0, movie_play_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, movie_stop_call_count);
    TEST_ASSERT_EQ_INT(ctx, 99, bAVIStart);
    TEST_ASSERT_EQ_INT(ctx, 123, wMCIDeviceID);
}

static void test_opening_move_starts_movie_only_on_first_tick(
    test_context *ctx) {
    reset_fixture();

    AVIOpeningMove();

    TEST_ASSERT_EQ_INT(ctx, 1, movie_play_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, movie_play_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, bAVIStart);

    nTimerCunt = 1;
    AVIOpeningMove();

    TEST_ASSERT_EQ_INT(ctx, 1, movie_play_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, bAVIStart);
}

static void test_avi_end_stops_movie_and_clears_started_flag(
    test_context *ctx) {
    reset_fixture();
    bAVIStart = 1;

    AVIEnd();

    TEST_ASSERT_EQ_INT(ctx, 1, movie_stop_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, bAVIStart);
}

TEST_MAIN_BEGIN;
test_read_free_and_paint_are_noops(&ctx);
test_opening_move_starts_movie_only_on_first_tick(&ctx);
test_avi_end_stops_movie_and_clears_started_flag(&ctx);
TEST_MAIN_END
