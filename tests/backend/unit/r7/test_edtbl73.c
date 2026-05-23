#include "support/test_runner.h"

#define EDIT_DATA_ENTRY_COUNT 52
#include "src/types.h"

static sprite_pattern *pat_chgwall7[1];
static sprite_pattern *emie7pat[1];
static sprite_pattern *pat_movie[1];
static sprite_pattern *pat_friend0[1];
static sprite_pattern *pat_friend1[1];
static sprite_pattern *pat_tekkyu7j[1];
static sprite_pattern *pat_tekkyu7[1];
static sprite_pattern *pat_wall7[1];
static sprite_pattern *branko7pat[1];
static sprite_pattern *springpat[1];
static sprite_pattern *spring90pat[1];
static sprite_pattern *spring45pat[1];
static sprite_pattern *itempat[1];

#include "src/r7/edtbl73.c"

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

static void test_edtbl73_entries_match_existing_table(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 52, edit_tbl.num);
    TEST_ASSERT_EQ_INT(ctx, 0, edit_tbl.dummy);

    assert_edit_entry(ctx, 0, 53, 1, pat_chgwall7, 0, 0, 0, 0, 1);
    assert_edit_entry(ctx, 1, 51, 3, emie7pat, 9163, 0, 0, 0, 17);
    assert_edit_entry(ctx, 2, 51, 3, emie7pat, 9163, 0, 0, 0, 18);
    assert_edit_entry(ctx, 3, 51, 3, emie7pat, 9163, 0, 0, 0, 19);
    assert_edit_entry(ctx, 4, 39, 3, pat_movie, 1082, 0, 0, 0, 0);
    assert_edit_entry(ctx, 5, 40, 3, pat_friend0, 918, 0, 0, 0, 0);
    assert_edit_entry(ctx, 6, 40, 3, pat_friend1, 918, 1, 0, 0, 3);
    assert_edit_entry(ctx, 7, 38, 1, pat_tekkyu7j, 910, 0, 0, 0, 0);
    assert_edit_entry(ctx, 8, 38, 1, pat_tekkyu7j, 910, 1, 0, 0, 1);
    assert_edit_entry(ctx, 9, 38, 1, pat_tekkyu7j, 910, 2, 0, 0, 2);
    assert_edit_entry(ctx, 10, 38, 1, pat_tekkyu7j, 910, 3, 0, 0, 3);
    assert_edit_entry(ctx, 11, 37, 1, pat_tekkyu7, 33670, 0, 0, 0, 0);
    assert_edit_entry(ctx, 12, 37, 1, pat_tekkyu7, 33670, 1, 0, 0, 0);
    assert_edit_entry(ctx, 13, 36, 1, pat_wall7, 50038, 0, 0, 0, 0);
    assert_edit_entry(ctx, 14, 36, 1, pat_wall7, 50038, 1, 0, 0, 1);
    assert_edit_entry(ctx, 15, 36, 1, pat_wall7, 50038, 2, 0, 0, 2);
    assert_edit_entry(ctx, 16, 36, 1, pat_wall7, 50038, 3, 0, 0, 3);
    assert_edit_entry(ctx, 17, 36, 1, pat_wall7, 50038, 4, 0, 0, 4);
    assert_edit_entry(ctx, 18, 36, 1, pat_wall7, 50038, 5, 0, 0, 5);
    assert_edit_entry(ctx, 19, 32, 1, branko7pat, 870, 26, 0, 0, 0);
    assert_edit_entry(ctx, 20, 32, 1, branko7pat, 9062, 40, 0, 0, 0);
    assert_edit_entry(ctx, 21, 32, 1, branko7pat, 17254, 56, 0, 0, 0);
    assert_edit_entry(ctx, 22, 32, 1, branko7pat, 25446, 72, 0, 0, 0);
    assert_edit_entry(ctx, 23, 32, 1, branko7pat, 870, 88, 0, 0, 0);
    assert_edit_entry(ctx, 24, 32, 1, branko7pat, 9062, 104, 0, 0, 0);
    assert_edit_entry(ctx, 25, 32, 1, branko7pat, 17254, 120, 0, 0, 0);
    assert_edit_entry(ctx, 26, 10, 1, springpat, 1312, 0, 0, 0, 0);
    assert_edit_entry(ctx, 27, 10, 1, springpat, 1312, 0, 2, 0, 0);
    assert_edit_entry(ctx, 28, 10, 1, spring90pat, 1312, 4, 0, 0, 0);
    assert_edit_entry(ctx, 29, 10, 1, spring90pat, 1312, 4, 1, 0, 0);
    assert_edit_entry(ctx, 30, 10, 1, spring45pat, 1266, 8, 0, 0, 0);
    assert_edit_entry(ctx, 31, 10, 1, spring45pat, 1266, 8, 2, 0, 0);
    assert_edit_entry(ctx, 32, 10, 1, spring45pat, 1266, 8, 1, 0, 0);
    assert_edit_entry(ctx, 33, 10, 1, spring45pat, 1266, 8, 3, 0, 0);
    assert_edit_entry(ctx, 34, 10, 1, springpat, 9504, 2, 0, 0, 0);
    assert_edit_entry(ctx, 35, 10, 1, springpat, 9504, 2, 2, 0, 0);
    assert_edit_entry(ctx, 36, 10, 1, spring90pat, 9504, 6, 0, 0, 0);
    assert_edit_entry(ctx, 37, 10, 1, spring90pat, 9504, 6, 1, 0, 0);
    assert_edit_entry(ctx, 38, 10, 1, spring45pat, 9458, 10, 0, 0, 0);
    assert_edit_entry(ctx, 39, 10, 1, spring45pat, 9458, 10, 2, 0, 0);
    assert_edit_entry(ctx, 40, 10, 1, spring45pat, 9458, 10, 1, 0, 0);
    assert_edit_entry(ctx, 41, 10, 1, spring45pat, 9458, 10, 3, 0, 0);
    assert_edit_entry(ctx, 42, 25, 4, itempat, 1448, 0, 0, 0, 0);
    assert_edit_entry(ctx, 43, 25, 4, itempat, 1448, 1, 0, 0, 1);
    assert_edit_entry(ctx, 44, 25, 4, itempat, 1448, 2, 0, 0, 2);
    assert_edit_entry(ctx, 45, 25, 4, itempat, 1448, 3, 0, 0, 3);
    assert_edit_entry(ctx, 46, 25, 4, itempat, 1448, 4, 0, 0, 4);
    assert_edit_entry(ctx, 47, 25, 4, itempat, 1448, 5, 0, 0, 5);
    assert_edit_entry(ctx, 48, 25, 4, itempat, 1448, 6, 0, 0, 6);
    assert_edit_entry(ctx, 49, 25, 4, itempat, 1448, 7, 0, 0, 7);
    assert_edit_entry(ctx, 50, 25, 4, itempat, 1448, 8, 0, 0, 10);
    assert_edit_entry(ctx, 51, 25, 4, itempat, 1448, 9, 0, 0, 12);
}

TEST_MAIN_BEGIN;
    test_edtbl73_entries_match_existing_table(&ctx);
TEST_MAIN_END
