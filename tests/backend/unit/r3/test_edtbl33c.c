#include "support/test_runner.h"

#define EDIT_DATA_ENTRY_COUNT 8
#include "src/types.h"

static sprite_pattern *kaitenban_pat[1];
static sprite_pattern *harir3pat[1];
static sprite_pattern *bobinpat[1];
static sprite_pattern *banpa_pat[1];
static sprite_pattern *frippat[1];
static sprite_pattern *springpat[1];
static sprite_pattern *itempat[1];

#include "src/r3/edtbl33c.c"

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

static void test_edtbl33c_entries_match_existing_table(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 8, edit_tbl.num);
    TEST_ASSERT_EQ_INT(ctx, 0, edit_tbl.dummy);

    assert_edit_entry(ctx, 0, 54, 1, kaitenban_pat, 17470, 254, 0, 0, 0);
    assert_edit_entry(ctx, 1, 32, 1, harir3pat, 1020, 0, 0, 0, 0);
    assert_edit_entry(ctx, 2, 29, 1, bobinpat, 1066, 0, 0, 0, 0);
    assert_edit_entry(ctx, 3, 42, 1, banpa_pat, 17507, 0, 0, 0, 0);
    assert_edit_entry(ctx, 4, 30, 1, frippat, 1181, 0, 0, 0, 0);
    assert_edit_entry(ctx, 5, 30, 1, frippat, 1181, 0, 1, 0, 0);
    assert_edit_entry(ctx, 6, 10, 1, springpat, 1312, 0, 0, 0, 0);
    assert_edit_entry(ctx, 7, 25, 4, itempat, 1448, 0, 0, 0, 0);
}

TEST_MAIN_BEGIN;
    test_edtbl33c_entries_match_existing_table(&ctx);
TEST_MAIN_END
