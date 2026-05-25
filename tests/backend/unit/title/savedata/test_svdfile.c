#include <string.h>

#include "support/test_runner.h"
#include "src/title/common/score_data_types.h"
#include "src/types.h"

score_data saveData[6];
Sint32 nSelectFileNumber;
score_data *lpCrntScorData;
Uint32 (*CloseScore)(Uint32);
Uint32 (*OpenScore)(Sint32);
Sint32 (*ReadIndx)(Uint32);
Uint32 (*ReadScore)(Sint32, char *, Uint32);
Uint32 (*WriteIndx)(Sint32, Uint32);
Uint32 (*WriteScore)(Sint32, char *, Uint32);
void (*sMemCpy)(void *, void *, Sint32);

static Uint32 open_score_values[8];
static Uint32 close_score_values[8];
static Sint32 read_score_slots[8];
static Uint32 read_score_modes[8];
static Uint32 read_index_handles[8];
static Sint32 write_score_slots[8];
static Uint32 write_score_modes[8];
static Uint32 written_score_saved[8];
static Uint32 written_score_round[8];
static Sint32 write_index_values[8];
static Uint32 write_index_handles[8];
static Sint32 open_score_call_count;
static Sint32 close_score_call_count;
static Sint32 read_index_call_count;
static Sint32 read_score_call_count;
static Sint32 write_index_call_count;
static Sint32 write_score_call_count;
static Sint32 mem_cpy_call_count;
static Uint32 open_score_next_handle;
static Sint32 read_index_return;
static score_data read_score_payloads[6];
static score_data current_score;

#include "src/title/savedata/svdfile.c"

static Uint32 fake_open_score(Sint32 mode) {
    open_score_values[open_score_call_count] = (Uint32)mode;
    ++open_score_call_count;
    return open_score_next_handle;
}

static Uint32 fake_close_score(Uint32 handle) {
    close_score_values[close_score_call_count] = handle;
    ++close_score_call_count;
    return 0;
}

static Sint32 fake_read_index(Uint32 handle) {
    read_index_handles[read_index_call_count] = handle;
    ++read_index_call_count;
    return read_index_return;
}

static Uint32 fake_read_score(Sint32 slot, char *data, Uint32 mode) {
    read_score_slots[read_score_call_count] = slot;
    read_score_modes[read_score_call_count] = mode;
    memcpy(data, &read_score_payloads[slot], sizeof(read_score_payloads[slot]));
    ++read_score_call_count;
    return 1;
}

static Uint32 fake_write_index(Sint32 index, Uint32 handle) {
    write_index_values[write_index_call_count] = index;
    write_index_handles[write_index_call_count] = handle;
    ++write_index_call_count;
    return 0;
}

static Uint32 fake_write_score(Sint32 slot, char *data, Uint32 mode) {
    score_data *written = (score_data *)data;

    write_score_slots[write_score_call_count] = slot;
    write_score_modes[write_score_call_count] = mode;
    written_score_saved[write_score_call_count] = written->saved;
    written_score_round[write_score_call_count] = written->roundNo;
    ++write_score_call_count;
    return 0;
}

static void fake_mem_cpy(void *dst, void *src, Sint32 bytes) {
    ++mem_cpy_call_count;
    memcpy(dst, src, (size_t)bytes);
}

static void reset_fixture(void) {
    Sint32 i;

    memset(saveData, 0, sizeof(saveData));
    memset(read_score_payloads, 0, sizeof(read_score_payloads));
    memset(&current_score, 0, sizeof(current_score));
    memset(open_score_values, 0, sizeof(open_score_values));
    memset(close_score_values, 0, sizeof(close_score_values));
    memset(read_score_slots, 0, sizeof(read_score_slots));
    memset(read_score_modes, 0, sizeof(read_score_modes));
    memset(read_index_handles, 0, sizeof(read_index_handles));
    memset(write_score_slots, 0, sizeof(write_score_slots));
    memset(write_score_modes, 0, sizeof(write_score_modes));
    memset(written_score_saved, 0, sizeof(written_score_saved));
    memset(written_score_round, 0, sizeof(written_score_round));
    memset(write_index_values, 0, sizeof(write_index_values));
    memset(write_index_handles, 0, sizeof(write_index_handles));

    for (i = 0; i < 6; ++i) {
        read_score_payloads[i].saved = 100 + i;
        read_score_payloads[i].roundNo = 200 + i;
        read_score_payloads[i].total = 300 + i;
        saveData[i].saved = 400 + i;
        saveData[i].roundNo = 500 + i;
        saveData[i].total = 600 + i;
    }

    nSelectFileNumber = 99;
    lpCrntScorData = &current_score;
    open_score_call_count = 0;
    close_score_call_count = 0;
    read_index_call_count = 0;
    read_score_call_count = 0;
    write_index_call_count = 0;
    write_score_call_count = 0;
    mem_cpy_call_count = 0;
    open_score_next_handle = 77;
    read_index_return = 2;
    OpenScore = fake_open_score;
    CloseScore = fake_close_score;
    ReadIndx = fake_read_index;
    ReadScore = fake_read_score;
    WriteIndx = fake_write_index;
    WriteScore = fake_write_score;
    sMemCpy = fake_mem_cpy;
}

static void test_read_data_file_rejects_negative_and_large_index(
    test_context *ctx) {
    reset_fixture();
    read_index_return = -1;

    ReadDataFile();

    TEST_ASSERT_EQ_INT(ctx, 1, open_score_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, open_score_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, read_index_call_count);
    TEST_ASSERT_EQ_INT(ctx, 77, read_index_handles[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, close_score_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, read_score_call_count);
    TEST_ASSERT_EQ_INT(ctx, 99, nSelectFileNumber);

    reset_fixture();
    read_index_return = 6;

    ReadDataFile();

    TEST_ASSERT_EQ_INT(ctx, 1, open_score_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, read_index_call_count);
    TEST_ASSERT_EQ_INT(ctx, 77, read_index_handles[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, close_score_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, read_score_call_count);
    TEST_ASSERT_EQ_INT(ctx, 99, nSelectFileNumber);
}

static void test_read_data_file_accepts_index_and_loads_six_slots(
    test_context *ctx) {
    Sint32 i;

    reset_fixture();
    read_index_return = 4;

    ReadDataFile();

    TEST_ASSERT_EQ_INT(ctx, 1, open_score_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, open_score_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, read_index_call_count);
    TEST_ASSERT_EQ_INT(ctx, 77, read_index_handles[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, close_score_call_count);
    TEST_ASSERT_EQ_INT(ctx, 77, close_score_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 4, nSelectFileNumber);
    TEST_ASSERT_EQ_INT(ctx, 6, read_score_call_count);
    for (i = 0; i < 6; ++i) {
        TEST_ASSERT_EQ_INT(ctx, i, read_score_slots[i]);
        TEST_ASSERT_EQ_INT(ctx, 0, read_score_modes[i]);
        TEST_ASSERT_EQ_INT(ctx, 100 + i, saveData[i].saved);
        TEST_ASSERT_EQ_INT(ctx, 200 + i, saveData[i].roundNo);
        TEST_ASSERT_EQ_INT(ctx, 300 + i, saveData[i].total);
    }
}

static void test_write_data_file_writes_index_slots_and_current_score(
    test_context *ctx) {
    Sint32 i;

    reset_fixture();
    nSelectFileNumber = 3;

    WriteDataFile();

    TEST_ASSERT_EQ_INT(ctx, 1, open_score_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, open_score_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, write_index_call_count);
    TEST_ASSERT_EQ_INT(ctx, 3, write_index_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 77, write_index_handles[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, close_score_call_count);
    TEST_ASSERT_EQ_INT(ctx, 77, close_score_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 6, write_score_call_count);
    for (i = 0; i < 6; ++i) {
        TEST_ASSERT_EQ_INT(ctx, i, write_score_slots[i]);
        TEST_ASSERT_EQ_INT(ctx, 0, write_score_modes[i]);
        TEST_ASSERT_EQ_INT(ctx, saveData[i].saved, written_score_saved[i]);
        TEST_ASSERT_EQ_INT(ctx, saveData[i].roundNo, written_score_round[i]);
    }
    TEST_ASSERT_EQ_INT(ctx, 1, mem_cpy_call_count);
    TEST_ASSERT_EQ_INT(ctx, saveData[3].saved, current_score.saved);
    TEST_ASSERT_EQ_INT(ctx, saveData[3].roundNo, current_score.roundNo);
    TEST_ASSERT_EQ_INT(ctx, saveData[3].total, current_score.total);
}

TEST_MAIN_BEGIN;
test_read_data_file_rejects_negative_and_large_index(&ctx);
test_read_data_file_accepts_index_and_loads_six_slots(&ctx);
test_write_data_file_writes_index_slots_and_current_score(&ctx);
TEST_MAIN_END
