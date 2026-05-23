#include <stdio.h>

#include "support/test_runner.h"
#include "src/types.h"

#include "src/r7/z73act.c"

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

static void assert_aset(test_context *ctx, int index, Uint16 x, Uint16 y,
                        Uint8 a, Uint8 b, Uint8 c, Uint8 d) {
    TEST_ASSERT_EQ_INT(ctx, x, asettbl[index].x);
    TEST_ASSERT_EQ_INT(ctx, y, asettbl[index].y);
    TEST_ASSERT_EQ_INT(ctx, a, asettbl[index].a);
    TEST_ASSERT_EQ_INT(ctx, b, asettbl[index].b);
    TEST_ASSERT_EQ_INT(ctx, c, asettbl[index].c);
    TEST_ASSERT_EQ_INT(ctx, d, asettbl[index].d);
}

static void test_z73act_table_sizes_and_fingerprints(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 16, (int)(sizeof(dummyarea) / sizeof(dummyarea[0])));
    TEST_ASSERT_EQ_INT(ctx, 79, (int)(sizeof(asettbl) / sizeof(asettbl[0])));
    TEST_ASSERT_EQ_INT(ctx, 1, (int)(sizeof(asettblz) / sizeof(asettblz[0])));

    assert_eq_u32(ctx, 890083012u, fnv1a32(dummyarea, sizeof(dummyarea)),
                  "dummyarea fnv1a32");
    assert_eq_u32(ctx, 339529476u, fnv1a32(asettbl, sizeof(asettbl)),
                  "asettbl fnv1a32");
    assert_eq_u32(ctx, 3480921043u, fnv1a32(asettblz, sizeof(asettblz)),
                  "asettblz fnv1a32");
}

static void test_z73act_anchor_entries_match_existing_table(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 1, dummyarea[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, dummyarea[15]);

    assert_aset(ctx, 0, 22, 256, 182, 0, 128, 0);
    assert_aset(ctx, 1, 24, 400, 153, 1, 128, 0);
    assert_aset(ctx, 12, 3184, 448, 171, 4, 128, 194);
    assert_aset(ctx, 24, 5456, 656, 178, 0, 128, 0);
    assert_aset(ctx, 36, 8416, 656, 178, 0, 128, 0);
    assert_aset(ctx, 48, 11048, 272, 43, 0, 128, 0);
    assert_aset(ctx, 60, 12224, 496, 171, 3, 128, 0);
    assert_aset(ctx, 76, 16136, 384, 179, 0, 128, 0);
    assert_aset(ctx, 77, 16304, 496, 171, 2, 128, 0);
    assert_aset(ctx, 78, 65535, 0, 0, 0, 0, 0);

    TEST_ASSERT_EQ_INT(ctx, 65535, asettblz[0].x);
    TEST_ASSERT_EQ_INT(ctx, 0, asettblz[0].y);
    TEST_ASSERT_EQ_INT(ctx, 0, asettblz[0].a);
    TEST_ASSERT_EQ_INT(ctx, 0, asettblz[0].b);
}

TEST_MAIN_BEGIN;
    test_z73act_table_sizes_and_fingerprints(&ctx);
    test_z73act_anchor_entries_match_existing_table(&ctx);
TEST_MAIN_END
