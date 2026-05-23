#include "support/test_runner.h"

#define EDIT_DATA_ENTRY_COUNT 38
#include "src/types.h"

static sprite_pattern *pat_kuzure_a[1];
static sprite_pattern *koma8pat[1];
static sprite_pattern *pat_et[1];
static sprite_pattern *pat_konbea[1];
static sprite_pattern *pat_scarab[1];
static sprite_pattern *pat_hachi8_e[1];
static sprite_pattern *pat_hachi8_b[1];
static sprite_pattern *pat_dai8[1];
static sprite_pattern *pat_kabuto[1];
static sprite_pattern *pat_nokogiri[1];
static sprite_pattern *pat_propera[1];
static sprite_pattern *pat_tobira0[1];
static sprite_pattern *pat_tobira1[1];
static sprite_pattern *pat_tobira2[1];

#include "src/r8/edtbl81.c"

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

static void test_edtbl81_entries_match_existing_table(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 38, edit_tbl.num);
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
    assert_edit_entry(ctx, 10, 42, 3, pat_konbea, 844, 1, 0, 0, 0);
    assert_edit_entry(ctx, 11, 42, 3, pat_konbea, 844, 0, 0, 0, 1);
    assert_edit_entry(ctx, 12, 42, 3, pat_konbea, 844, 0, 0, 1, 2);
    assert_edit_entry(ctx, 13, 47, 3, pat_scarab, 9279, 0, 0, 157, 0);
    assert_edit_entry(ctx, 14, 47, 3, pat_scarab, 9279, 0, 1, 99, 0);
    assert_edit_entry(ctx, 15, 47, 3, pat_scarab, 9279, 0, 0, 2, 0);
    assert_edit_entry(ctx, 16, 47, 3, pat_scarab, 9279, 0, 1, 253, 0);
    assert_edit_entry(ctx, 17, 46, 3, pat_hachi8_e, 9301, 0, 0, 0, 0);
    assert_edit_entry(ctx, 18, 46, 3, pat_hachi8_b, 9301, 1, 0, 0, 0);
    assert_edit_entry(ctx, 19, 44, 3, pat_dai8, 920, 0, 0, 0, 0);
    assert_edit_entry(ctx, 20, 44, 3, pat_dai8, 920, 1, 0, 0, 0);
    assert_edit_entry(ctx, 21, 44, 3, pat_dai8, 920, 2, 0, 0, 0);
    assert_edit_entry(ctx, 22, 36, 3, pat_kabuto, 9294, 0, 0, 0, 0);
    assert_edit_entry(ctx, 23, 36, 3, pat_kabuto, 9294, 0, 1, 1, 0);
    assert_edit_entry(ctx, 24, 36, 3, pat_kabuto, 9294, 1, 0, 0, 0);
    assert_edit_entry(ctx, 25, 36, 3, pat_kabuto, 9294, 1, 1, 1, 0);
    assert_edit_entry(ctx, 26, 39, 3, pat_nokogiri, 17174, 0, 0, 0, 0);
    assert_edit_entry(ctx, 27, 39, 3, pat_nokogiri, 17174, 128, 0, 0, 0);
    assert_edit_entry(ctx, 28, 39, 3, pat_nokogiri, 17174, 1, 1, 0, 0);
    assert_edit_entry(ctx, 29, 39, 3, pat_nokogiri, 17174, 129, 1, 0, 0);
    assert_edit_entry(ctx, 30, 39, 3, pat_nokogiri, 17174, 2, 2, 0, 0);
    assert_edit_entry(ctx, 31, 39, 3, pat_nokogiri, 17174, 130, 2, 0, 0);
    assert_edit_entry(ctx, 32, 39, 3, pat_nokogiri, 17174, 3, 3, 0, 0);
    assert_edit_entry(ctx, 33, 39, 3, pat_nokogiri, 17174, 131, 3, 0, 0);
    assert_edit_entry(ctx, 34, 40, 3, pat_propera, 874, 0, 0, 0, 0);
    assert_edit_entry(ctx, 35, 41, 3, pat_tobira0, 1150, 0, 0, 0, 0);
    assert_edit_entry(ctx, 36, 41, 3, pat_tobira1, 1150, 1, 0, 0, 0);
    assert_edit_entry(ctx, 37, 41, 3, pat_tobira2, 1150, 2, 0, 0, 0);
}

TEST_MAIN_BEGIN;
    test_edtbl81_entries_match_existing_table(&ctx);
TEST_MAIN_END
