#include <stdio.h>

#include "support/test_runner.h"
#include "src/types.h"

#include "src/r7/z71atbl.c"

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

static void test_z71atbl_table_sizes_and_fingerprints(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 256,
                       (int)(sizeof(scddirtbl) / sizeof(scddirtbl[0])));
    TEST_ASSERT_EQ_INT(ctx, 257,
                       (int)(sizeof(scdtblwk) / sizeof(scdtblwk[0])));
    TEST_ASSERT_EQ_INT(ctx, 16,
                       (int)(sizeof(scdtblwk[0]) / sizeof(scdtblwk[0][0])));
    TEST_ASSERT_EQ_INT(ctx, 257,
                       (int)(sizeof(scdtblwk2) / sizeof(scdtblwk2[0])));
    TEST_ASSERT_EQ_INT(ctx, 16,
                       (int)(sizeof(scdtblwk2[0]) / sizeof(scdtblwk2[0][0])));
    TEST_ASSERT_EQ_INT(ctx, 1100,
                       (int)(sizeof(zone1scd) / sizeof(zone1scd[0])));
    TEST_ASSERT_EQ_INT(ctx, 8, (int)(sizeof(mapwka) / sizeof(mapwka[0])));
    TEST_ASSERT_EQ_INT(ctx, 64,
                       (int)(sizeof(mapwka[0]) / sizeof(mapwka[0][0])));
    TEST_ASSERT_EQ_INT(ctx, 8, (int)(sizeof(mapwkb) / sizeof(mapwkb[0])));
    TEST_ASSERT_EQ_INT(ctx, 64,
                       (int)(sizeof(mapwkb[0]) / sizeof(mapwkb[0][0])));

    assert_eq_u32(ctx, 3982722238u, fnv1a32(scddirtbl, sizeof(scddirtbl)),
                  "scddirtbl fnv1a32");
    assert_eq_u32(ctx, 1959850999u, fnv1a32(scdtblwk, sizeof(scdtblwk)),
                  "scdtblwk fnv1a32");
    assert_eq_u32(ctx, 2204738079u, fnv1a32(scdtblwk2, sizeof(scdtblwk2)),
                  "scdtblwk2 fnv1a32");
    assert_eq_u32(ctx, 2843559557u, fnv1a32(zone1scd, sizeof(zone1scd)),
                  "zone1scd fnv1a32");
    assert_eq_u32(ctx, 2310417589u, fnv1a32(mapwka, sizeof(mapwka)),
                  "mapwka fnv1a32");
    assert_eq_u32(ctx, 3608158758u, fnv1a32(mapwkb, sizeof(mapwkb)),
                  "mapwkb fnv1a32");
}

static void test_z71atbl_direction_and_collision_anchors(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 255, scddirtbl[0]);
    TEST_ASSERT_EQ_INT(ctx, 252, scddirtbl[16]);
    TEST_ASSERT_EQ_INT(ctx, 204, scddirtbl[127]);
    TEST_ASSERT_EQ_INT(ctx, 98, scddirtbl[254]);
    TEST_ASSERT_EQ_INT(ctx, 255, scddirtbl[255]);

    TEST_ASSERT_EQ_INT(ctx, 0, scdtblwk[0][0]);
    TEST_ASSERT_EQ_INT(ctx, 1, scdtblwk[1][15]);
    TEST_ASSERT_EQ_INT(ctx, 4, scdtblwk[24][15]);
    TEST_ASSERT_EQ_INT(ctx, 241, scdtblwk[57][8]);
    TEST_ASSERT_EQ_INT(ctx, 0, scdtblwk[256][0]);

    TEST_ASSERT_EQ_INT(ctx, 16, scdtblwk2[1][15]);
    TEST_ASSERT_EQ_INT(ctx, 8, scdtblwk2[16][14]);
    TEST_ASSERT_EQ_INT(ctx, 16, scdtblwk2[24][15]);
    TEST_ASSERT_EQ_INT(ctx, 242, scdtblwk2[57][14]);
    TEST_ASSERT_EQ_INT(ctx, 0, scdtblwk2[256][0]);
}

static void test_z71atbl_map_anchors(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 0, zone1scd[0]);
    TEST_ASSERT_EQ_INT(ctx, 81, zone1scd[1]);
    TEST_ASSERT_EQ_INT(ctx, 81, zone1scd[13]);
    TEST_ASSERT_EQ_INT(ctx, 0, zone1scd[46]);
    TEST_ASSERT_EQ_INT(ctx, 0, zone1scd[1099]);

    TEST_ASSERT_EQ_INT(ctx, 0, mapwka[0][0]);
    TEST_ASSERT_EQ_INT(ctx, 129, mapwka[0][2]);
    TEST_ASSERT_EQ_INT(ctx, 167, mapwka[0][15]);
    TEST_ASSERT_EQ_INT(ctx, 160, mapwka[1][2]);
    TEST_ASSERT_EQ_INT(ctx, 158, mapwka[2][0]);
    TEST_ASSERT_EQ_INT(ctx, 0, mapwka[7][63]);

    TEST_ASSERT_EQ_INT(ctx, 100, mapwkb[0][0]);
    TEST_ASSERT_EQ_INT(ctx, 105, mapwkb[0][5]);
    TEST_ASSERT_EQ_INT(ctx, 106, mapwkb[1][0]);
    TEST_ASSERT_EQ_INT(ctx, 111, mapwkb[1][8]);
    TEST_ASSERT_EQ_INT(ctx, 112, mapwkb[2][0]);
    TEST_ASSERT_EQ_INT(ctx, 117, mapwkb[2][5]);
    TEST_ASSERT_EQ_INT(ctx, 0, mapwkb[7][63]);
}

TEST_MAIN_BEGIN;
    test_z71atbl_table_sizes_and_fingerprints(&ctx);
    test_z71atbl_direction_and_collision_anchors(&ctx);
    test_z71atbl_map_anchors(&ctx);
TEST_MAIN_END
