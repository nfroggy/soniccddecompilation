#include "support/test_runner.h"
#include "src/types.h"

#include "src/r3/z33act.c"

static void test_z33act_dummy_area_matches_existing_bytes(test_context *ctx) {
    int i;

    TEST_ASSERT_EQ_INT(ctx, 1, dummyarea[0]);
    for (i = 1; i < 16; ++i) {
        TEST_ASSERT_EQ_INT(ctx, 0, dummyarea[i]);
    }
}

static void assert_aset_entry(test_context *ctx, int index,
                              const aset_info *expected) {
    TEST_ASSERT_EQ_INT(ctx, expected->x, asettbl[index].x);
    TEST_ASSERT_EQ_INT(ctx, expected->y, asettbl[index].y);
    TEST_ASSERT_EQ_INT(ctx, expected->a, asettbl[index].a);
    TEST_ASSERT_EQ_INT(ctx, expected->b, asettbl[index].b);
    TEST_ASSERT_EQ_INT(ctx, expected->c, asettbl[index].c);
    TEST_ASSERT_EQ_INT(ctx, expected->d, asettbl[index].d);
}

static void test_z33act_actor_table_matches_existing_records(
    test_context *ctx) {
    static const aset_info expected[] = {
        {288, 896, 29, 0, 128, 0},
        {352, 968, 53, 1, 128, 0},
        {410, 976, 10, 4, 128, 0},
        {416, 816, 153, 1, 128, 0},
        {431, 976, 153, 1, 128, 0},
        {432, 760, 29, 0, 128, 0},
        {488, 192, 144, 35, 128, 0},
        {520, 975, 53, 0, 128, 0},
        {528, 1424, 153, 1, 128, 0},
        {528, 1432, 10, 0, 128, 0},
        {560, 1072, 32, 1, 128, 0},
        {584, 712, 10, 4, 128, 0},
        {591, 900, 53, 1, 128, 0},
        {592, 592, 144, 34, 128, 0},
        {592, 1072, 32, 1, 128, 0},
        {616, 720, 153, 2, 128, 0},
        {624, 160, 144, 33, 128, 0},
        {624, 272, 53, 0, 128, 0},
        {648, 17248, 30, 0, 128, 0},
        {656, 1136, 53, 1, 128, 0},
        {656, 1248, 53, 1, 128, 0},
        {656, 1336, 10, 0, 128, 0},
        {688, 128, 144, 33, 128, 0},
        {712, 984, 144, 83, 128, 0},
        {712, 1296, 144, 34, 128, 0},
        {752, 832, 29, 0, 128, 0},
        {768, 112, 144, 33, 128, 0},
        {792, 17030, 30, 0, 128, 0},
        {832, 736, 29, 0, 128, 0},
        {843, 17419, 30, 0, 128, 0},
        {850, 17628, 30, 0, 128, 0},
        {864, 656, 144, 34, 128, 0},
        {864, 1296, 144, 34, 128, 0},
        {880, 1328, 32, 4, 128, 0},
        {896, 448, 177, 0, 128, 0},
        {896, 1104, 182, 254, 128, 0},
        {912, 1328, 32, 4, 128, 0},
        {941, 1244, 30, 0, 128, 0},
        {948, 1035, 30, 0, 128, 0},
        {960, 736, 29, 0, 128, 0},
        {1000, 644, 30, 0, 128, 0},
        {1040, 432, 144, 83, 128, 0},
        {1040, 519, 53, 1, 128, 0},
        {1040, 618, 10, 0, 128, 0},
        {1056, 811, 30, 0, 128, 0},
        {1072, 1344, 176, 0, 128, 0},
        {1080, 984, 144, 83, 128, 0},
        {1088, 560, 53, 0, 128, 0},
        {1112, 338, 144, 33, 128, 0},
        {1136, 784, 32, 0, 128, 0},
        {1136, 1072, 144, 66, 128, 0},
        {1136, 1144, 53, 1, 128, 0},
        {1136, 1192, 10, 0, 128, 0},
        {1184, 784, 153, 0, 128, 0},
        {1200, 976, 144, 84, 128, 0},
        {1200, 992, 53, 1, 128, 0},
        {1200, 1248, 53, 1, 128, 0},
        {1200, 1352, 10, 0, 128, 0},
        {1216, 416, 144, 82, 128, 0},
        {1216, 800, 10, 8, 128, 0},
        {1336, 224, 149, 0, 128, 0},
        {65535, 65535, 255, 255, 255, 255},
    };
    int i;

    TEST_ASSERT_EQ_INT(ctx, 62, (int)(sizeof(asettbl) / sizeof(asettbl[0])));
    TEST_ASSERT_EQ_INT(ctx, 62, (int)(sizeof(expected) / sizeof(expected[0])));
    for (i = 0; i < 62; ++i) {
        assert_aset_entry(ctx, i, &expected[i]);
    }
}

static void test_z33act_zone_actor_table_matches_existing_sentinel(
    test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 1,
                       (int)(sizeof(asettblz) / sizeof(asettblz[0])));
    TEST_ASSERT_EQ_INT(ctx, 65535, asettblz[0].x);
    TEST_ASSERT_EQ_INT(ctx, 0, asettblz[0].y);
    TEST_ASSERT_EQ_INT(ctx, 0, asettblz[0].a);
    TEST_ASSERT_EQ_INT(ctx, 0, asettblz[0].b);
}

TEST_MAIN_BEGIN;
    test_z33act_dummy_area_matches_existing_bytes(&ctx);
    test_z33act_actor_table_matches_existing_records(&ctx);
    test_z33act_zone_actor_table_matches_existing_sentinel(&ctx);
TEST_MAIN_END
