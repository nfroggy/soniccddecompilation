#include "support/test_runner.h"
#include "src/types.h"

#include "src/r3/z32act.c"

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

static void test_z32act_table_sizes_and_fingerprints(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 16, (int)(sizeof(dummyarea) / sizeof(dummyarea[0])));
    TEST_ASSERT_EQ_INT(ctx, 455, (int)(sizeof(asettbl) / sizeof(asettbl[0])));
    TEST_ASSERT_EQ_INT(ctx, 1, (int)(sizeof(asettblz) / sizeof(asettblz[0])));

    assert_eq_u32(ctx, 890083012u, fnv1a32(dummyarea, sizeof(dummyarea)),
                  "dummyarea fnv1a32");
    assert_eq_u32(ctx, 1685103333u, fnv1a32(asettbl, sizeof(asettbl)),
                  "asettbl fnv1a32");
    assert_eq_u32(ctx, 3480921043u, fnv1a32(asettblz, sizeof(asettblz)),
                  "asettblz fnv1a32");
}

static void test_z32act_anchor_entries_match_existing_table(test_context *ctx) {
    assert_dummy_byte(ctx, 0, 1);
    assert_dummy_byte(ctx, 15, 0);

    assert_aset(ctx, 0, 384, 136, 47, 255, 192, 0);
    assert_aset(ctx, 1, 592, 462, 144, 85, 224, 0);
    assert_aset(ctx, 64, 2000, 1432, 10, 0, 32, 0);
    assert_aset(ctx, 128, 3104, 648, 174, 0, 32, 0);
    assert_aset(ctx, 256, 5584, 129, 53, 0, 224, 0);
    assert_aset(ctx, 384, 7856, 17776, 10, 8, 224, 0);
    assert_aset(ctx, 453, 10136, 1024, 148, 0, 224, 0);
    assert_aset(ctx, 454, 65535, 65535, 255, 255, 255, 255);

    TEST_ASSERT_EQ_INT(ctx, 65535, asettblz[0].x);
    TEST_ASSERT_EQ_INT(ctx, 0, asettblz[0].y);
    TEST_ASSERT_EQ_INT(ctx, 0, asettblz[0].a);
    TEST_ASSERT_EQ_INT(ctx, 0, asettblz[0].b);
}

TEST_MAIN_BEGIN;
test_z32act_table_sizes_and_fingerprints(&ctx);
test_z32act_anchor_entries_match_existing_table(&ctx);
TEST_MAIN_END
