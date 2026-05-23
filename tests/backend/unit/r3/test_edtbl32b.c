#include "support/test_runner.h"

#define EDIT_DATA_ENTRY_COUNT 25
#include "src/types.h"

static sprite_pattern *bariapat[1];
static sprite_pattern *pat_movie[1];
static sprite_pattern *pat_et[1];
static sprite_pattern *pat_e_ga[1];
static sprite_pattern *pat_b_ga[1];
static sprite_pattern *pat_e_tentou[1];
static sprite_pattern *pat_ten_b_tentou[1];
static sprite_pattern *kamapat[1];
static sprite_pattern *bkamapat[1];
static sprite_pattern *harir3pat[1];
static sprite_pattern *frdr3pat[1];
static sprite_pattern *trapdr3pat[1];
static sprite_pattern *for3pat[1];
static sprite_pattern *getdair3pat[1];
static sprite_pattern *gandair3pat[1];
static sprite_pattern *drumr3pat[1];
static sprite_pattern *pat_pocket[1];
static sprite_pattern *banpa_pat[1];
static sprite_pattern *togeball_pat[1];
static sprite_pattern *bobinpat[1];
static sprite_pattern *frippat[1];
static sprite_pattern *springpat[1];
static sprite_pattern *itempat[1];

#include "src/r3/edtbl32b.c"

static void assert_edit_entry(test_context *ctx, int index, Uint8 actno,
                              Uint8 sprpri, sprite_pattern **patbase,
                              Uint16 offset, Uint8 userflag_h, Uint8 rludrv,
                              Uint8 userflag_l, Uint8 patno) {
    edit_data_entry *entry = &edit_tbl.dat[index];

    TEST_ASSERT_EQ_INT(ctx, actno, entry->actno);
    TEST_ASSERT_EQ_INT(ctx, sprpri, entry->sprpri);
    TEST_ASSERT_TRUE(ctx, patbase == entry->patbase);
    TEST_ASSERT_EQ_INT(ctx, offset, entry->offset);
    TEST_ASSERT_EQ_INT(ctx, userflag_h, entry->userflag_h);
    TEST_ASSERT_EQ_INT(ctx, rludrv, entry->rludrv);
    TEST_ASSERT_EQ_INT(ctx, userflag_l, entry->userflag_l);
    TEST_ASSERT_EQ_INT(ctx, patno, entry->patno);
}

static void test_edtbl32b_entries_match_existing_table(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 25, edit_tbl.num);
    TEST_ASSERT_EQ_INT(ctx, 0, edit_tbl.dummy);

    assert_edit_entry(ctx, 0, 53, 3, bariapat, 1348, 0, 0, 0, 1);
    assert_edit_entry(ctx, 1, 53, 3, bariapat, 1348, 1, 0, 0, 2);
    assert_edit_entry(ctx, 2, 56, 3, pat_movie, 830, 0, 0, 0, 0);
    assert_edit_entry(ctx, 3, 55, 1, pat_et, 874, 0, 0, 0, 0);
    assert_edit_entry(ctx, 4, 45, 1, pat_e_ga, 9249, 0, 0, 0, 0);
    assert_edit_entry(ctx, 5, 45, 1, pat_b_ga, 9249, 255, 0, 0, 0);
    assert_edit_entry(ctx, 6, 46, 1, pat_e_tentou, 9322, 0, 0, 0, 0);
    assert_edit_entry(ctx, 7, 46, 1, pat_ten_b_tentou, 9322, 255, 0, 0,
                      0);
    assert_edit_entry(ctx, 8, 36, 1, kamapat, 9202, 0, 0, 0, 0);
    assert_edit_entry(ctx, 9, 36, 1, bkamapat, 9202, 255, 0, 0, 0);
    assert_edit_entry(ctx, 10, 32, 1, harir3pat, 798, 0, 0, 0, 0);
    assert_edit_entry(ctx, 11, 33, 1, frdr3pat, 17214, 2, 0, 0, 0);
    assert_edit_entry(ctx, 12, 34, 1, trapdr3pat, 846, 1, 0, 0, 0);
    assert_edit_entry(ctx, 13, 35, 1, for3pat, 862, 5, 0, 0, 0);
    assert_edit_entry(ctx, 14, 38, 1, getdair3pat, 878, 0, 0, 0, 0);
    assert_edit_entry(ctx, 15, 39, 1, gandair3pat, 886, 0, 0, 0, 0);
    assert_edit_entry(ctx, 16, 40, 1, drumr3pat, 910, 0, 0, 0, 0);
    assert_edit_entry(ctx, 17, 47, 1, pat_pocket, 957, 0, 0, 0, 0);
    assert_edit_entry(ctx, 18, 42, 1, banpa_pat, 17246, 0, 0, 0, 0);
    assert_edit_entry(ctx, 19, 44, 1, togeball_pat, 957, 0, 0, 0, 0);
    assert_edit_entry(ctx, 20, 29, 1, bobinpat, 1230, 0, 0, 0, 0);
    assert_edit_entry(ctx, 21, 30, 1, frippat, 1181, 0, 0, 0, 0);
    assert_edit_entry(ctx, 22, 30, 1, frippat, 1181, 0, 1, 0, 0);
    assert_edit_entry(ctx, 23, 10, 1, springpat, 1312, 0, 0, 0, 0);
    assert_edit_entry(ctx, 24, 25, 4, itempat, 1448, 0, 0, 0, 0);
}

TEST_MAIN_BEGIN;
    test_edtbl32b_entries_match_existing_table(&ctx);
TEST_MAIN_END
