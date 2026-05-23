#include "support/test_runner.h"

#define EDIT_DATA_ENTRY_COUNT 45
#include "src/types.h"

static sprite_pattern *pat_kuzure_a[1];
static sprite_pattern *koma8pat[1];
static sprite_pattern *harir8pat[1];
static sprite_pattern *pat_tobira0[1];
static sprite_pattern *pat_tobira1[1];
static sprite_pattern *pat_tobira2[1];
static sprite_pattern *pat_sw[1];
static sprite_pattern *pistonpat[1];
static sprite_pattern *bobinpat[1];
static sprite_pattern *togeitapat[1];
static sprite_pattern *pat_nokogiri[1];
static sprite_pattern *pat_konbea[1];
static sprite_pattern *pat_kabuto[1];
static sprite_pattern *hotaru8pat[1];
static sprite_pattern *togedair8pat[1];
static sprite_pattern *pat_jettoge[1];
static sprite_pattern *springpat[1];
static sprite_pattern *spring90pat[1];
static sprite_pattern *spring45pat[1];
static sprite_pattern *komapat[1];
static sprite_pattern *itempat[1];

#include "src/r8/edtbl83.c"

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

static void test_edtbl83_entries_match_existing_table(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 45, edit_tbl.num);
    TEST_ASSERT_EQ_INT(ctx, 0, edit_tbl.dummy);

    assert_edit_entry(ctx, 0, 37, 3, pat_kuzure_a, 17274, 1, 0, 0, 0);
    assert_edit_entry(ctx, 1, 37, 3, pat_kuzure_a, 17274, 2, 0, 0, 1);
    assert_edit_entry(ctx, 2, 37, 3, pat_kuzure_a, 17274, 3, 0, 0, 2);
    assert_edit_entry(ctx, 3, 37, 3, pat_kuzure_a, 17274, 4, 0, 0, 3);
    assert_edit_entry(ctx, 4, 37, 3, pat_kuzure_a, 17274, 5, 0, 0, 4);
    assert_edit_entry(ctx, 5, 37, 3, pat_kuzure_a, 17274, 6, 0, 0, 5);
    assert_edit_entry(ctx, 6, 37, 3, pat_kuzure_a, 17274, 7, 0, 0, 6);
    assert_edit_entry(ctx, 7, 37, 3, pat_kuzure_a, 17274, 8, 0, 0, 7);
    assert_edit_entry(ctx, 8, 15, 4, koma8pat, 882, 0, 0, 0, 0);
    assert_edit_entry(ctx, 9, 54, 1, harir8pat, 1160, 255, 0, 0, 0);
    assert_edit_entry(ctx, 10, 54, 1, harir8pat, 1160, 254, 0, 0, 1);
    assert_edit_entry(ctx, 11, 41, 3, pat_tobira0, 1150, 0, 0, 0, 0);
    assert_edit_entry(ctx, 12, 41, 3, pat_tobira1, 1150, 1, 0, 0, 0);
    assert_edit_entry(ctx, 13, 41, 3, pat_tobira2, 1150, 2, 0, 0, 0);
    assert_edit_entry(ctx, 14, 33, 1, pat_sw, 902, 1, 0, 0, 0);
    assert_edit_entry(ctx, 15, 52, 1, pistonpat, 1176, 0, 0, 0, 0);
    assert_edit_entry(ctx, 16, 52, 1, pistonpat, 1176, 1, 0, 0, 0);
    assert_edit_entry(ctx, 17, 52, 1, pistonpat, 1176, 2, 0, 0, 0);
    assert_edit_entry(ctx, 18, 37, 3, pat_kuzure_a, 17274, 0, 0, 0, 0);
    assert_edit_entry(ctx, 19, 37, 3, pat_kuzure_a, 17274, 17, 0, 0, 0);
    assert_edit_entry(ctx, 20, 29, 1, bobinpat, 1230, 1, 0, 0, 0);
    assert_edit_entry(ctx, 21, 51, 1, togeitapat, 1024, 0, 0, 0, 0);
    assert_edit_entry(ctx, 22, 39, 3, pat_nokogiri, 17174, 0, 0, 0, 0);
    assert_edit_entry(ctx, 23, 39, 3, pat_nokogiri, 17174, 128, 0, 0, 0);
    assert_edit_entry(ctx, 24, 42, 3, pat_konbea, 844, 1, 0, 0, 0);
    assert_edit_entry(ctx, 25, 42, 3, pat_konbea, 844, 0, 0, 0, 1);
    assert_edit_entry(ctx, 26, 42, 3, pat_konbea, 844, 0, 0, 1, 2);
    assert_edit_entry(ctx, 27, 36, 3, pat_kabuto, 9294, 0, 0, 0, 0);
    assert_edit_entry(ctx, 28, 36, 3, pat_kabuto, 9294, 0, 1, 1, 0);
    assert_edit_entry(ctx, 29, 36, 3, pat_kabuto, 9294, 1, 0, 0, 0);
    assert_edit_entry(ctx, 30, 36, 3, pat_kabuto, 9294, 1, 1, 1, 0);
    assert_edit_entry(ctx, 31, 57, 1, hotaru8pat, 9136, 0, 0, 0, 0);
    assert_edit_entry(ctx, 32, 54, 1, togedair8pat, 17392, 1, 0, 0, 0);
    assert_edit_entry(ctx, 33, 43, 3, pat_jettoge, 1088, 0, 0, 0, 0);
    assert_edit_entry(ctx, 34, 10, 1, springpat, 1312, 0, 0, 0, 0);
    assert_edit_entry(ctx, 35, 10, 1, springpat, 1312, 0, 2, 0, 0);
    assert_edit_entry(ctx, 36, 10, 1, spring90pat, 1312, 4, 0, 0, 0);
    assert_edit_entry(ctx, 37, 10, 1, spring90pat, 1312, 4, 1, 0, 0);
    assert_edit_entry(ctx, 38, 10, 1, spring45pat, 1266, 8, 0, 0, 0);
    assert_edit_entry(ctx, 39, 10, 1, spring45pat, 1266, 8, 2, 0, 0);
    assert_edit_entry(ctx, 40, 10, 1, spring45pat, 1266, 8, 1, 0, 0);
    assert_edit_entry(ctx, 41, 10, 1, spring45pat, 1266, 8, 3, 0, 0);
    assert_edit_entry(ctx, 42, 15, 4, komapat, 882, 0, 0, 0, 0);
    assert_edit_entry(ctx, 43, 25, 4, itempat, 1448, 0, 0, 0, 0);
    assert_edit_entry(ctx, 44, 25, 4, itempat, 1448, 9, 0, 0, 12);
}

TEST_MAIN_BEGIN;
    test_edtbl83_entries_match_existing_table(&ctx);
TEST_MAIN_END
