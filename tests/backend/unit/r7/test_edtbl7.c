#include "support/test_runner.h"

#define EDIT_DATA_ENTRY_COUNT 79
#include "src/types.h"

static sprite_pattern *slight7pat[1];
static sprite_pattern *branko7pat[1];
static sprite_pattern *pat_chgwall7[1];
static sprite_pattern *bariapat[1];
static sprite_pattern *pat_et[1];
static sprite_pattern *pat_movie[1];
static sprite_pattern *pat_friend0[1];
static sprite_pattern *pat_friend1[1];
static sprite_pattern *pat_kanabun_e[1];
static sprite_pattern *pat_kanabun_b[1];
static sprite_pattern *pat_hotaru7_e[1];
static sprite_pattern *pat_hotaru7_b[1];
static sprite_pattern *pat_kabasira[1];
static sprite_pattern *pat_tekkyu7j[1];
static sprite_pattern *pat_tekkyu7[1];
static sprite_pattern *pat_wall7[1];
static sprite_pattern *springpat[1];
static sprite_pattern *spring90pat[1];
static sprite_pattern *spring45pat[1];
static sprite_pattern *itempat[1];

#include "src/r7/edtbl7.c"

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

static void test_edtbl7_entries_match_existing_table(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 79, edit_tbl.num);
    TEST_ASSERT_EQ_INT(ctx, 0, edit_tbl.dummy);

    assert_edit_entry(ctx, 0, 56, 1, slight7pat, 9391, 0, 0, 8, 0);
    assert_edit_entry(ctx, 1, 56, 1, slight7pat, 9391, 1, 0, 8, 0);
    assert_edit_entry(ctx, 2, 56, 1, slight7pat, 9391, 2, 0, 8, 0);
    assert_edit_entry(ctx, 3, 56, 1, slight7pat, 9391, 3, 0, 8, 0);
    assert_edit_entry(ctx, 4, 56, 1, slight7pat, 9391, 4, 0, 8, 0);
    assert_edit_entry(ctx, 5, 56, 1, slight7pat, 9391, 5, 0, 8, 1);
    assert_edit_entry(ctx, 6, 56, 1, slight7pat, 9391, 6, 0, 8, 2);
    assert_edit_entry(ctx, 7, 56, 1, slight7pat, 9391, 7, 0, 8, 0);
    assert_edit_entry(ctx, 8, 56, 1, slight7pat, 9391, 8, 0, 8, 0);
    assert_edit_entry(ctx, 9, 56, 1, slight7pat, 17583, 0, 0, 4, 0);
    assert_edit_entry(ctx, 10, 56, 1, slight7pat, 17583, 1, 0, 4, 0);
    assert_edit_entry(ctx, 11, 56, 1, slight7pat, 17583, 2, 0, 4, 0);
    assert_edit_entry(ctx, 12, 56, 1, slight7pat, 17583, 3, 0, 4, 0);
    assert_edit_entry(ctx, 13, 56, 1, slight7pat, 17583, 4, 0, 4, 0);
    assert_edit_entry(ctx, 14, 56, 1, slight7pat, 17583, 5, 0, 4, 1);
    assert_edit_entry(ctx, 15, 56, 1, slight7pat, 17583, 6, 0, 4, 2);
    assert_edit_entry(ctx, 16, 56, 1, slight7pat, 17583, 7, 0, 4, 0);
    assert_edit_entry(ctx, 17, 56, 1, slight7pat, 17583, 8, 0, 4, 0);
    assert_edit_entry(ctx, 18, 32, 1, branko7pat, 870, 22, 0, 0, 0);
    assert_edit_entry(ctx, 19, 32, 1, branko7pat, 9062, 38, 0, 0, 0);
    assert_edit_entry(ctx, 20, 32, 1, branko7pat, 17254, 54, 0, 0, 0);
    assert_edit_entry(ctx, 21, 32, 1, branko7pat, 25446, 70, 0, 0, 0);
    assert_edit_entry(ctx, 22, 32, 1, branko7pat, 870, 86, 0, 0, 0);
    assert_edit_entry(ctx, 23, 32, 1, branko7pat, 9062, 102, 0, 0, 0);
    assert_edit_entry(ctx, 24, 32, 1, branko7pat, 17254, 118, 0, 0, 0);
    assert_edit_entry(ctx, 25, 53, 1, pat_chgwall7, 0, 0, 0, 0, 1);
    assert_edit_entry(ctx, 26, 43, 3, bariapat, 1348, 0, 0, 0, 1);
    assert_edit_entry(ctx, 27, 43, 3, bariapat, 1348, 1, 0, 0, 2);
    assert_edit_entry(ctx, 28, 43, 3, bariapat, 1348, 2, 0, 0, 3);
    assert_edit_entry(ctx, 29, 43, 3, bariapat, 1348, 3, 0, 0, 4);
    assert_edit_entry(ctx, 30, 43, 3, bariapat, 1348, 4, 0, 0, 5);
    assert_edit_entry(ctx, 31, 41, 1, pat_et, 942, 0, 0, 0, 0);
    assert_edit_entry(ctx, 32, 39, 3, pat_movie, 1199, 0, 0, 0, 0);
    assert_edit_entry(ctx, 33, 40, 3, pat_friend0, 918, 0, 0, 0, 0);
    assert_edit_entry(ctx, 34, 40, 3, pat_friend1, 918, 1, 0, 0, 3);
    assert_edit_entry(ctx, 35, 33, 1, pat_kanabun_e, 9193, 0, 0, 0, 0);
    assert_edit_entry(ctx, 36, 33, 1, pat_kanabun_b, 9193, 1, 0, 0, 0);
    assert_edit_entry(ctx, 37, 35, 1, pat_hotaru7_e, 33885, 0, 0, 0, 0);
    assert_edit_entry(ctx, 38, 35, 1, pat_hotaru7_b, 33885, 1, 0, 0, 0);
    assert_edit_entry(ctx, 39, 34, 1, pat_kabasira, 41926, 0, 0, 0, 0);
    assert_edit_entry(ctx, 40, 34, 1, pat_kabasira, 41926, 1, 0, 0, 0);
    assert_edit_entry(ctx, 41, 38, 1, pat_tekkyu7j, 910, 0, 0, 0, 0);
    assert_edit_entry(ctx, 42, 38, 1, pat_tekkyu7j, 910, 1, 0, 0, 1);
    assert_edit_entry(ctx, 43, 38, 1, pat_tekkyu7j, 910, 2, 0, 0, 2);
    assert_edit_entry(ctx, 44, 38, 1, pat_tekkyu7j, 910, 3, 0, 0, 3);
    assert_edit_entry(ctx, 45, 37, 1, pat_tekkyu7, 33670, 0, 0, 0, 0);
    assert_edit_entry(ctx, 46, 37, 1, pat_tekkyu7, 33670, 1, 0, 0, 0);
    assert_edit_entry(ctx, 47, 36, 1, pat_wall7, 50038, 0, 0, 0, 0);
    assert_edit_entry(ctx, 48, 36, 1, pat_wall7, 50038, 1, 0, 0, 1);
    assert_edit_entry(ctx, 49, 36, 1, pat_wall7, 50038, 2, 0, 0, 2);
    assert_edit_entry(ctx, 50, 36, 1, pat_wall7, 50038, 3, 0, 0, 3);
    assert_edit_entry(ctx, 51, 36, 1, pat_wall7, 50038, 4, 0, 0, 4);
    assert_edit_entry(ctx, 52, 36, 1, pat_wall7, 50038, 5, 0, 0, 5);
    assert_edit_entry(ctx, 53, 10, 1, springpat, 1312, 0, 0, 0, 0);
    assert_edit_entry(ctx, 54, 10, 1, springpat, 1312, 0, 2, 0, 0);
    assert_edit_entry(ctx, 55, 10, 1, spring90pat, 1312, 4, 0, 0, 0);
    assert_edit_entry(ctx, 56, 10, 1, spring90pat, 1312, 4, 1, 0, 0);
    assert_edit_entry(ctx, 57, 10, 1, spring45pat, 1266, 8, 0, 0, 0);
    assert_edit_entry(ctx, 58, 10, 1, spring45pat, 1266, 8, 2, 0, 0);
    assert_edit_entry(ctx, 59, 10, 1, spring45pat, 1266, 8, 1, 0, 0);
    assert_edit_entry(ctx, 60, 10, 1, spring45pat, 1266, 8, 3, 0, 0);
    assert_edit_entry(ctx, 61, 10, 1, springpat, 9504, 2, 0, 0, 0);
    assert_edit_entry(ctx, 62, 10, 1, springpat, 9504, 2, 2, 0, 0);
    assert_edit_entry(ctx, 63, 10, 1, spring90pat, 9504, 6, 0, 0, 0);
    assert_edit_entry(ctx, 64, 10, 1, spring90pat, 9504, 6, 1, 0, 0);
    assert_edit_entry(ctx, 65, 10, 1, spring45pat, 9458, 10, 0, 0, 0);
    assert_edit_entry(ctx, 66, 10, 1, spring45pat, 9458, 10, 2, 0, 0);
    assert_edit_entry(ctx, 67, 10, 1, spring45pat, 9458, 10, 1, 0, 0);
    assert_edit_entry(ctx, 68, 10, 1, spring45pat, 9458, 10, 3, 0, 0);
    assert_edit_entry(ctx, 69, 25, 4, itempat, 1448, 0, 0, 0, 0);
    assert_edit_entry(ctx, 70, 25, 4, itempat, 1448, 1, 0, 0, 1);
    assert_edit_entry(ctx, 71, 25, 4, itempat, 1448, 2, 0, 0, 2);
    assert_edit_entry(ctx, 72, 25, 4, itempat, 1448, 3, 0, 0, 3);
    assert_edit_entry(ctx, 73, 25, 4, itempat, 1448, 4, 0, 0, 4);
    assert_edit_entry(ctx, 74, 25, 4, itempat, 1448, 5, 0, 0, 5);
    assert_edit_entry(ctx, 75, 25, 4, itempat, 1448, 6, 0, 0, 6);
    assert_edit_entry(ctx, 76, 25, 4, itempat, 1448, 7, 0, 0, 7);
    assert_edit_entry(ctx, 77, 25, 4, itempat, 1448, 8, 0, 0, 10);
    assert_edit_entry(ctx, 78, 25, 4, itempat, 1448, 9, 0, 0, 12);
}

TEST_MAIN_BEGIN;
    test_edtbl7_entries_match_existing_table(&ctx);
TEST_MAIN_END
