#include <string.h>

#include "support/test_runner.h"
#include "types.h"

#include "src/plchg.c"

static const Uint8 expected_plchg00[] = {255, 53, 54, 55, 56, 51, 52, 255};
static const Uint8 expected_plchg01[] = {255, 75, 76, 77, 78, 255, 255, 255};
static const Uint8 expected_plchg02[] = {254, 45, 46, 47, 48, 49, 255, 255};
static const Uint8 expected_plchg03[] = {254, 45, 46, 49, 47, 48, 49, 255};
static const Uint8 expected_plchg04[] = {253, 100, 101, 102, 103, 255, 255,
                                         255};
static const Uint8 expected_plchg05[] = {
    23, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 2, 2, 2, 3, 4, 254, 2};
static const Uint8 expected_plchg06[] = {15, 109, 110, 131, 132, 255};
static const Uint8 expected_plchg07[] = {63, 5, 255};
static const Uint8 expected_plchg08[] = {63, 96, 255};
static const Uint8 expected_plchg09[] = {63, 51, 255};
static const Uint8 expected_plchg10[] = {63, 52, 255};
static const Uint8 expected_plchg11[] = {63, 53, 255};
static const Uint8 expected_plchg12[] = {63, 54, 255};
static const Uint8 expected_plchg13[] = {7, 91, 92, 127, 255};
static const Uint8 expected_plchg14[] = {7, 60, 63, 255};
static const Uint8 expected_plchg15[] = {7, 97, 98, 185, 99, 186, 255};
static const Uint8 expected_plchg16[] = {3, 50, 144, 145, 146, 147, 255};
static const Uint8 expected_plchg17[] = {4, 107, 108, 255};
static const Uint8 expected_plchg18[] = {15, 67, 67, 67, 254, 1};
static const Uint8 expected_plchg19[] = {15, 67, 68, 254, 1};
static const Uint8 expected_plchg20[] = {63, 73, 255};
static const Uint8 expected_plchg21[] = {11, 95, 95, 55, 56, 253, 0};
static const Uint8 expected_plchg22[] = {32, 104, 255};
static const Uint8 expected_plchg23[] = {47, 105, 255};
static const Uint8 expected_plchg24[] = {3, 106, 255};
static const Uint8 expected_plchg25[] = {3, 78, 79, 80, 81, 82, 0, 254, 1};
static const Uint8 expected_plchg26[] = {3, 93, 255};
static const Uint8 expected_plchg27[] = {7, 93, 94, 255};
static const Uint8 expected_plchg28[] = {119, 0, 253, 0};
static const Uint8 expected_plchg29[] = {3, 60, 61, 83, 62, 84, 255};
static const Uint8 expected_plchg30[] = {3, 60, 253, 0};
static const Uint8 expected_plchg31[] = {
    23, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111,
    111, 111, 112, 112, 112, 113, 112, 113, 254, 2};
static const Uint8 expected_plchg32[] = {63, 114, 255};
static const Uint8 expected_plchg33[] = {255, 115, 116, 117, 116, 255};
static const Uint8 expected_plchg34[] = {255, 118, 119, 255, 255, 255};
static const Uint8 expected_plchg35[] = {254, 124, 125, 126, 255, 255};
static const Uint8 expected_plchg36[] = {7, 120, 120, 255};
static const Uint8 expected_plchg37[] = {3, 121, 255};
static const Uint8 expected_plchg38[] = {31, 122, 123, 255};
static const Uint8 expected_plchg39[] = {253, 115, 116, 117, 255, 255, 255};
static const Uint8 expected_plchg40[] = {63, 111, 255};
static const Uint8 expected_plchg41[] = {63, 6, 255};
static const Uint8 expected_plchg42[] = {3, 7, 7, 7, 7, 7, 9, 9,
                                         8, 8, 8, 1, 10, 10, 253, 5};
static const Uint8 expected_plchg43[] = {
    4,  17, 18, 18, 19, 19, 18, 18, 19, 19, 18, 18, 19, 19,
    17, 17, 17, 17, 20, 20, 20, 20, 21, 21, 22, 22, 22, 22,
    22, 22, 23, 23, 23, 23, 23, 23, 164, 165, 254, 2};
static const Uint8 expected_plchg44[] = {252, 24, 25, 166, 25, 255};
static const Uint8 expected_plchg45[] = {252, 26, 27, 28, 31, 29, 30, 255};
static const Uint8 expected_plchg46[] = {255, 13, 14, 15, 16, 11, 12, 255};
static const Uint8 expected_plchg47[] = {255, 97, 98, 99, 255};
static const Uint8 expected_plchg48[] = {19, 112, 111, 112, 121, 254, 1};
static const Uint8 expected_plchg49[] = {255, 148, 149, 150, 151, 255, 255,
                                         255};
static const Uint8 expected_plchg50[] = {15, 133, 134, 135, 136, 255};
static const Uint8 expected_plchg51[] = {3, 1, 181, 181, 182, 182,
                                         183, 183, 187, 253, 5};
static const Uint8 expected_plchg52[] = {3, 1, 181, 181, 1, 1,
                                         184, 184, 187, 253, 5};
static const Uint8 expected_plchg53[] = {255, 166, 167, 168, 169, 255, 255,
                                         255};
static const Uint8 expected_plchg54[] = {254, 32, 33, 35, 36, 37, 49, 255};
static const Uint8 expected_plchg55[] = {1, 164, 165, 255};
static const Uint8 expected_plchg56[] = {
    1,   121, 171, 172, 171, 172, 171, 172, 169, 170, 169,
    170, 169, 170, 167, 168, 167, 168, 167, 168, 253, 26};
static const Uint8 expected_plchg57[] = {
    1,  93,  167, 168, 167, 168, 167, 168, 169, 170, 169,
    170, 169, 170, 171, 172, 171, 172, 171, 172, 253, 26};

static void check_bytes(test_context *ctx, const Uint8 *actual,
                        size_t actual_size, const Uint8 *expected,
                        size_t expected_size) {
    TEST_ASSERT_EQ_INT(ctx, expected_size, actual_size);
    if (actual_size == expected_size) {
        TEST_ASSERT_TRUE(ctx, memcmp(actual, expected, actual_size) == 0);
    }
}

#define CHECK_BYTES(ctx, actual, expected)                                     \
    check_bytes((ctx), (actual), sizeof(actual), (expected), sizeof(expected))

static void test_player_animation_streams_match_literals(test_context *ctx) {
    CHECK_BYTES(ctx, plchg00, expected_plchg00);
    CHECK_BYTES(ctx, plchg01, expected_plchg01);
    CHECK_BYTES(ctx, plchg02, expected_plchg02);
    CHECK_BYTES(ctx, plchg03, expected_plchg03);
    CHECK_BYTES(ctx, plchg04, expected_plchg04);
    CHECK_BYTES(ctx, plchg05, expected_plchg05);
    CHECK_BYTES(ctx, plchg06, expected_plchg06);
    CHECK_BYTES(ctx, plchg07, expected_plchg07);
    CHECK_BYTES(ctx, plchg08, expected_plchg08);
    CHECK_BYTES(ctx, plchg09, expected_plchg09);
    CHECK_BYTES(ctx, plchg10, expected_plchg10);
    CHECK_BYTES(ctx, plchg11, expected_plchg11);
    CHECK_BYTES(ctx, plchg12, expected_plchg12);
    CHECK_BYTES(ctx, plchg13, expected_plchg13);
    CHECK_BYTES(ctx, plchg14, expected_plchg14);
    CHECK_BYTES(ctx, plchg15, expected_plchg15);
    CHECK_BYTES(ctx, plchg16, expected_plchg16);
    CHECK_BYTES(ctx, plchg17, expected_plchg17);
    CHECK_BYTES(ctx, plchg18, expected_plchg18);
    CHECK_BYTES(ctx, plchg19, expected_plchg19);
    CHECK_BYTES(ctx, plchg20, expected_plchg20);
    CHECK_BYTES(ctx, plchg21, expected_plchg21);
    CHECK_BYTES(ctx, plchg22, expected_plchg22);
    CHECK_BYTES(ctx, plchg23, expected_plchg23);
    CHECK_BYTES(ctx, plchg24, expected_plchg24);
    CHECK_BYTES(ctx, plchg25, expected_plchg25);
    CHECK_BYTES(ctx, plchg26, expected_plchg26);
    CHECK_BYTES(ctx, plchg27, expected_plchg27);
    CHECK_BYTES(ctx, plchg28, expected_plchg28);
    CHECK_BYTES(ctx, plchg29, expected_plchg29);
    CHECK_BYTES(ctx, plchg30, expected_plchg30);
    CHECK_BYTES(ctx, plchg31, expected_plchg31);
    CHECK_BYTES(ctx, plchg32, expected_plchg32);
    CHECK_BYTES(ctx, plchg33, expected_plchg33);
    CHECK_BYTES(ctx, plchg34, expected_plchg34);
    CHECK_BYTES(ctx, plchg35, expected_plchg35);
    CHECK_BYTES(ctx, plchg36, expected_plchg36);
    CHECK_BYTES(ctx, plchg37, expected_plchg37);
    CHECK_BYTES(ctx, plchg38, expected_plchg38);
    CHECK_BYTES(ctx, plchg39, expected_plchg39);
    CHECK_BYTES(ctx, plchg40, expected_plchg40);
    CHECK_BYTES(ctx, plchg41, expected_plchg41);
    CHECK_BYTES(ctx, plchg42, expected_plchg42);
    CHECK_BYTES(ctx, plchg43, expected_plchg43);
    CHECK_BYTES(ctx, plchg44, expected_plchg44);
    CHECK_BYTES(ctx, plchg45, expected_plchg45);
    CHECK_BYTES(ctx, plchg46, expected_plchg46);
    CHECK_BYTES(ctx, plchg47, expected_plchg47);
    CHECK_BYTES(ctx, plchg48, expected_plchg48);
    CHECK_BYTES(ctx, plchg49, expected_plchg49);
    CHECK_BYTES(ctx, plchg50, expected_plchg50);
    CHECK_BYTES(ctx, plchg51, expected_plchg51);
    CHECK_BYTES(ctx, plchg52, expected_plchg52);
    CHECK_BYTES(ctx, plchg53, expected_plchg53);
    CHECK_BYTES(ctx, plchg54, expected_plchg54);
    CHECK_BYTES(ctx, plchg55, expected_plchg55);
    CHECK_BYTES(ctx, plchg56, expected_plchg56);
    CHECK_BYTES(ctx, plchg57, expected_plchg57);
}

static void test_playchg_pointer_table_matches_literals(test_context *ctx) {
    Uint8 *expected[] = {
        plchg00, plchg01, plchg02, plchg03, plchg04, plchg05, plchg06, plchg07,
        plchg08, plchg09, plchg10, plchg11, plchg12, plchg13, plchg14, plchg15,
        plchg16, plchg17, plchg18, plchg19, plchg20, plchg21, plchg22, plchg23,
        plchg24, plchg25, plchg26, plchg27, plchg28, plchg29, plchg30, plchg31,
        plchg32, plchg33, plchg34, plchg35, plchg36, plchg37, plchg38, plchg39,
        plchg40, plchg41, plchg42, plchg43, plchg44, plchg45, plchg46, plchg47,
        plchg48, plchg49, plchg50, plchg51, plchg52, plchg53, plchg54, plchg55,
        plchg56, plchg57, plchg54, plchg55};

    TEST_ASSERT_EQ_INT(ctx, 60, sizeof(playchg) / sizeof(playchg[0]));
    TEST_ASSERT_EQ_INT(ctx, 60, sizeof(expected) / sizeof(expected[0]));
    for (int i = 0; i < 60; ++i) {
        TEST_ASSERT_TRUE(ctx, playchg[i] == expected[i]);
    }
}

TEST_MAIN_BEGIN;
    test_player_animation_streams_match_literals(&ctx);
    test_playchg_pointer_table_matches_literals(&ctx);
TEST_MAIN_END
