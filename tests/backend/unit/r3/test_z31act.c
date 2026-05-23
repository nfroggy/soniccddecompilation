#include "support/test_runner.h"
#include "src/types.h"

#include "src/r3/z31act.c"

static Uint32 fnv1a32(const void *data, unsigned int size) {
    const Uint8 *bytes = (const Uint8 *)data;
    Uint32 hash = 2166136261u;
    unsigned int i;

    for (i = 0; i < size; ++i) {
        hash ^= bytes[i];
        hash *= 16777619u;
    }
    return hash;
}

static void assert_eq_u32(test_context *ctx, Uint32 expected, Uint32 actual,
                          const char *label) {
    if (expected != actual) {
        ++ctx->failures;
        fprintf(stderr, "%s: expected %lu, got %lu\n", label,
                (unsigned long)expected, (unsigned long)actual);
    }
}

static void assert_dummy_byte(test_context *ctx, int index, Uint8 value) {
    TEST_ASSERT_EQ_INT(ctx, value, dummyarea[index]);
}

static void assert_aset(test_context *ctx, int index, Uint16 x, Uint16 y,
                        Uint8 a, Uint8 b, Uint8 c, Uint8 d) {
    TEST_ASSERT_EQ_INT(ctx, x, asettbl[index].x);
    TEST_ASSERT_EQ_INT(ctx, y, asettbl[index].y);
    TEST_ASSERT_EQ_INT(ctx, a, asettbl[index].a);
    TEST_ASSERT_EQ_INT(ctx, b, asettbl[index].b);
    TEST_ASSERT_EQ_INT(ctx, c, asettbl[index].c);
    TEST_ASSERT_EQ_INT(ctx, d, asettbl[index].d);
}

static void test_z31act_table_sizes_and_fingerprints(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 16, (int)(sizeof(dummyarea) / sizeof(dummyarea[0])));
    TEST_ASSERT_EQ_INT(ctx, 518, (int)(sizeof(asettbl) / sizeof(asettbl[0])));
    TEST_ASSERT_EQ_INT(ctx, 1, (int)(sizeof(asettblz) / sizeof(asettblz[0])));

    assert_eq_u32(ctx, 890083012u, fnv1a32(dummyarea, sizeof(dummyarea)),
                  "dummyarea fnv1a32");
    assert_eq_u32(ctx, 1709010214u, fnv1a32(asettbl, sizeof(asettbl)),
                  "asettbl fnv1a32");
    assert_eq_u32(ctx, 3480921043u, fnv1a32(asettblz, sizeof(asettblz)),
                  "asettblz fnv1a32");
}

static void test_z31act_anchor_entries_match_existing_table(test_context *ctx) {
    assert_dummy_byte(ctx, 0, 1);
    assert_dummy_byte(ctx, 15, 0);

    assert_aset(ctx, 0, 420, 944, 153, 2, 224, 0);
    assert_aset(ctx, 1, 640, 453, 144, 82, 224, 0);
    assert_aset(ctx, 64, 2256, 1240, 10, 0, 128, 0);
    assert_aset(ctx, 128, 3344, 1424, 34, 1, 192, 0);
    assert_aset(ctx, 256, 5832, 936, 153, 8, 192, 0);
    assert_aset(ctx, 384, 8592, 1416, 10, 0, 224, 0);
    assert_aset(ctx, 516, 12176, 1328, 148, 0, 224, 0);
    assert_aset(ctx, 517, 65535, 65535, 255, 255, 255, 255);

    TEST_ASSERT_EQ_INT(ctx, 65535, asettblz[0].x);
    TEST_ASSERT_EQ_INT(ctx, 0, asettblz[0].y);
    TEST_ASSERT_EQ_INT(ctx, 0, asettblz[0].a);
    TEST_ASSERT_EQ_INT(ctx, 0, asettblz[0].b);
}

TEST_MAIN_BEGIN;
test_z31act_table_sizes_and_fingerprints(&ctx);
test_z31act_anchor_entries_match_existing_table(&ctx);
TEST_MAIN_END
