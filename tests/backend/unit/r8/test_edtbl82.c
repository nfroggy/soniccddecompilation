#include "support/test_runner.h"

#define EDIT_DATA_ENTRY_COUNT 10
#include "src/types.h"

static sprite_pattern *pat_kuzure_a[1];
static sprite_pattern *koma8pat[1];
static sprite_pattern *pat_et[1];

#include "src/r8/edtbl82.c"

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

static void test_edtbl82_entries_match_existing_table(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 10, edit_tbl.num);
    TEST_ASSERT_EQ_INT(ctx, 0, edit_tbl.dummy);

    assert_edit_entry(ctx, 0, 37, 3, pat_kuzure_a, 17624, 1, 0, 0, 0);
    assert_edit_entry(ctx, 1, 37, 3, pat_kuzure_a, 17624, 2, 0, 0, 1);
    assert_edit_entry(ctx, 2, 37, 3, pat_kuzure_a, 17624, 3, 0, 0, 2);
    assert_edit_entry(ctx, 3, 37, 3, pat_kuzure_a, 17624, 4, 0, 0, 3);
    assert_edit_entry(ctx, 4, 37, 3, pat_kuzure_a, 17624, 5, 0, 0, 4);
    assert_edit_entry(ctx, 5, 37, 3, pat_kuzure_a, 17624, 6, 0, 0, 5);
    assert_edit_entry(ctx, 6, 37, 3, pat_kuzure_a, 17624, 7, 0, 0, 6);
    assert_edit_entry(ctx, 7, 37, 3, pat_kuzure_a, 17624, 8, 0, 0, 7);
    assert_edit_entry(ctx, 8, 15, 4, koma8pat, 1258, 0, 0, 0, 0);
    assert_edit_entry(ctx, 9, 67, 1, pat_et, 768, 0, 0, 0, 0);
}

TEST_MAIN_BEGIN;
    test_edtbl82_entries_match_existing_table(&ctx);
TEST_MAIN_END
