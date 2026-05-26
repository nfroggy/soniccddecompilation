#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

typedef struct {
    Sint16 x;
    Sint16 y;
    Uint16 index;
    Uint16 link;
    Uint16 reverse;
} sprite_call;

static int last_stub_id;
static sprite_status_sp *last_stub_actor;
static int stub_sets_delete;
static int mem_set_count;
static void *mem_set_ptr;
static Uint8 mem_set_value;
static Sint32 mem_set_size;
static sprite_call sprite_calls[96];
static int sprite_call_count;

void player(sprite_status_sp *work);
void ufo0(sprite_status_sp *work);
void timeufo(sprite_status_sp *work);
void item_ring(sprite_status_sp *work);
void kage(sprite_status_sp *work);
void plkage(sprite_status_sp *work);
void kemuri(sprite_status_sp *work);
void sibuki(sprite_status_sp *work);
void press(sprite_status_sp *work);
void title_logo(sprite_status_sp *work);
void title_obi(sprite_status_sp *work);
void explosion(sprite_status_sp *work);
void bara_ring(sprite_status_sp *work);
void time_stone(sprite_status_sp *work);
void tskira0(sprite_status_sp *work);
void tskira1(sprite_status_sp *work);

void (*EAsprset)(Sint16, Sint16, Uint16, Uint16, Uint16);
void (*sMemSet)(void *, Uint8, Sint32);
bmp_info SprBmp[700];

#include "src/special/sps_equ.c"
#include "src/special/act_s.c"

static void record_stub(sprite_status_sp *work, int id) {
    last_stub_id = id;
    last_stub_actor = work;
    if (stub_sets_delete) {
        work->actflg |= 1;
    }
}

void player(sprite_status_sp *work) { record_stub(work, 1); }
void ufo0(sprite_status_sp *work) { record_stub(work, 2); }
void timeufo(sprite_status_sp *work) { record_stub(work, 3); }
void item_ring(sprite_status_sp *work) { record_stub(work, 4); }
void kage(sprite_status_sp *work) { record_stub(work, 5); }
void plkage(sprite_status_sp *work) { record_stub(work, 6); }
void kemuri(sprite_status_sp *work) { record_stub(work, 7); }
void sibuki(sprite_status_sp *work) { record_stub(work, 8); }
void press(sprite_status_sp *work) { record_stub(work, 9); }
void title_logo(sprite_status_sp *work) { record_stub(work, 10); }
void title_obi(sprite_status_sp *work) { record_stub(work, 11); }
void explosion(sprite_status_sp *work) { record_stub(work, 12); }
void bara_ring(sprite_status_sp *work) { record_stub(work, 13); }
void time_stone(sprite_status_sp *work) { record_stub(work, 14); }
void tskira0(sprite_status_sp *work) { record_stub(work, 15); }
void tskira1(sprite_status_sp *work) { record_stub(work, 16); }

static void test_memset(void *ptr, Uint8 value, Sint32 size) {
    ++mem_set_count;
    mem_set_ptr = ptr;
    mem_set_value = value;
    mem_set_size = size;
    memset(ptr, value, (size_t)size);
}

static void test_easprset(Sint16 x, Sint16 y, Uint16 index, Uint16 link,
                          Uint16 reverse) {
    if (sprite_call_count < (int)(sizeof(sprite_calls) / sizeof(sprite_calls[0]))) {
        sprite_calls[sprite_call_count].x = x;
        sprite_calls[sprite_call_count].y = y;
        sprite_calls[sprite_call_count].index = index;
        sprite_calls[sprite_call_count].link = link;
        sprite_calls[sprite_call_count].reverse = reverse;
    }
    ++sprite_call_count;
}

static void reset_act_s_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(z_buffer, 0, sizeof(z_buffer));
    memset(SprBmp, 0, sizeof(SprBmp));
    memset(sprite_calls, 0, sizeof(sprite_calls));
    last_stub_id = 0;
    last_stub_actor = 0;
    stub_sets_delete = 0;
    mem_set_count = 0;
    mem_set_ptr = 0;
    mem_set_value = 255;
    mem_set_size = -1;
    sprite_call_count = 0;
    linkdata = 0;
    EAsprset = test_easprset;
    sMemSet = test_memset;
}

static void set_actor_for_sprite(sprite_status_sp *work,
                                 sprite_patterns_sp **table) {
    work->actno = 1;
    work->pattbl = table;
    work->pattblno = 0;
    work->patno = 0;
    work->pctime = 1;
    work->sx_posi.w.h = 160;
    work->sy_posi.w.h = 180;
}

static void assert_sprite_call(test_context *ctx, int index, Sint16 x, Sint16 y,
                               Uint16 tile, Uint16 link, Uint16 reverse) {
    TEST_ASSERT_EQ_INT(ctx, x, sprite_calls[index].x);
    TEST_ASSERT_EQ_INT(ctx, y, sprite_calls[index].y);
    TEST_ASSERT_EQ_INT(ctx, tile, sprite_calls[index].index);
    TEST_ASSERT_EQ_INT(ctx, link, sprite_calls[index].link);
    TEST_ASSERT_EQ_INT(ctx, reverse, sprite_calls[index].reverse);
}

static void test_actmain_ignores_empty_actor(test_context *ctx) {
    reset_act_s_state();

    actmain(&actwk[5]);

    TEST_ASSERT_EQ_INT(ctx, 0, last_stub_id);
    TEST_ASSERT_EQ_INT(ctx, 0, mem_set_count);
}

static void test_actmain_dispatches_each_actor_number(test_context *ctx) {
    int i;

    for (i = 1; i <= 16; ++i) {
        reset_act_s_state();
        actwk[5].actno = (Uint8)i;

        actmain(&actwk[5]);

        TEST_ASSERT_EQ_INT(ctx, i, last_stub_id);
        TEST_ASSERT_TRUE(ctx, last_stub_actor == &actwk[5]);
        TEST_ASSERT_EQ_INT(ctx, 0, mem_set_count);
    }
}

static void test_actmain_clears_actor_marked_by_handler(test_context *ctx) {
    reset_act_s_state();
    actwk[5].actno = 1;
    stub_sets_delete = 1;

    actmain(&actwk[5]);

    TEST_ASSERT_EQ_INT(ctx, 1, last_stub_id);
    TEST_ASSERT_EQ_INT(ctx, 1, mem_set_count);
    TEST_ASSERT_TRUE(ctx, mem_set_ptr == &actwk[5]);
    TEST_ASSERT_EQ_INT(ctx, 0, mem_set_value);
    TEST_ASSERT_EQ_INT(ctx, (Sint32)sizeof(actwk[5]), mem_set_size);
}

static void test_action_visits_actor_bands_and_clears_unused_links(
    test_context *ctx) {
    reset_act_s_state();
    actwk[2].actno = 2;
    actwk[0].actno = 1;
    actwk[16].actno = 3;
    actwk[40].actno = 4;

    action();

    TEST_ASSERT_EQ_INT(ctx, 4, last_stub_id);
    TEST_ASSERT_TRUE(ctx, last_stub_actor == &actwk[40]);
    TEST_ASSERT_EQ_INT(ctx, 80, sprite_call_count);
    assert_sprite_call(ctx, 0, 0, 0, 0, 0, 0);
    assert_sprite_call(ctx, 79, 0, 0, 0, 79, 0);
}

static void test_zbuf_init_clears_bucket_heads_only(test_context *ctx) {
    reset_act_s_state();
    z_buffer[0] = 11;
    z_buffer[1] = 22;
    z_buffer[8] = 33;

    zbuf_init();

    TEST_ASSERT_EQ_INT(ctx, 0, z_buffer[0]);
    TEST_ASSERT_EQ_INT(ctx, 22, z_buffer[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, z_buffer[8]);
}

static void test_zbuf_set_uses_first_empty_slot_and_clamps_depth(
    test_context *ctx) {
    reset_act_s_state();
    z_buffer[504] = 7;

    zbuf_set(&actwk[9], 4096);

    TEST_ASSERT_EQ_INT(ctx, 7, z_buffer[504]);
    TEST_ASSERT_EQ_INT(ctx, 9, z_buffer[505]);
    TEST_ASSERT_EQ_INT(ctx, 0, z_buffer[506]);
}

static void test_zbuf_set_handles_first_slot_and_full_bucket(test_context *ctx) {
    int i;

    reset_act_s_state();

    zbuf_set(&actwk[8], 0);
    TEST_ASSERT_EQ_INT(ctx, 8, z_buffer[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, z_buffer[1]);

    for (i = 0; i < 7; ++i) {
        z_buffer[32 + i] = (Uint16)(20 + i);
    }

    zbuf_set(&actwk[9], 256);

    TEST_ASSERT_EQ_INT(ctx, 26, z_buffer[38]);
    TEST_ASSERT_EQ_INT(ctx, 9, z_buffer[39]);
    TEST_ASSERT_EQ_INT(ctx, 0, z_buffer[40]);
}

static void test_zpatset_call_stops_on_empty_slot(test_context *ctx) {
    reset_act_s_state();

    TEST_ASSERT_EQ_INT(ctx, 1, zpatset_call(3));
    TEST_ASSERT_EQ_INT(ctx, 0, sprite_call_count);
}

static void test_patset_animates_and_draws_visible_pattern(test_context *ctx) {
    static sprite_pattern frame0 = {1, {{3, 4, 0, 10}}};
    static sprite_pattern frame1 = {1, {{5, 6, 8, 11}}};
    static sprite_patterns_sp anim = {2, 1, {&frame0, &frame1}};
    static sprite_patterns_sp *table[] = {&anim};

    reset_act_s_state();
    SprBmp[10].xs = 12;
    SprBmp[10].ys = 14;
    SprBmp[11].xs = 16;
    SprBmp[11].ys = 18;
    set_actor_for_sprite(&actwk[4], table);

    patset(&actwk[4]);
    patset(&actwk[4]);

    TEST_ASSERT_EQ_INT(ctx, 2, sprite_call_count);
    assert_sprite_call(ctx, 0, 171, 184, 10, 0, 0);
    assert_sprite_call(ctx, 1, 147, 186, 11, 1, 1);
}

static void test_patset_wraps_out_of_range_frame_and_respects_freeze(
    test_context *ctx) {
    static sprite_pattern frame0 = {1, {{1, 2, 0, 12}}};
    static sprite_patterns_sp anim = {1, 4, {&frame0}};
    static sprite_patterns_sp *table[] = {&anim};

    reset_act_s_state();
    SprBmp[12].xs = 8;
    SprBmp[12].ys = 8;
    set_actor_for_sprite(&actwk[4], table);
    actwk[4].patno = 9;
    actwk[4].actflg = 2;

    patset(&actwk[4]);

    TEST_ASSERT_EQ_INT(ctx, 1, sprite_call_count);
    assert_sprite_call(ctx, 0, 169, 182, 12, 0, 0);
}

static void test_patset_skips_hidden_empty_and_inactive_patterns(
    test_context *ctx) {
    static sprite_pattern empty = {0, {{0, 0, 0, 0}}};
    static sprite_patterns_sp anim = {1, 1, {&empty}};
    static sprite_patterns_sp *table[] = {&anim};

    reset_act_s_state();
    set_actor_for_sprite(&actwk[4], table);
    actwk[4].actno = 0;
    patset(&actwk[4]);
    TEST_ASSERT_EQ_INT(ctx, 0, sprite_call_count);

    actwk[4].actno = 1;
    actwk[4].actflg = 4;
    patset(&actwk[4]);
    TEST_ASSERT_EQ_INT(ctx, 0, sprite_call_count);

    actwk[4].actflg = 0;
    patset(&actwk[4]);
    TEST_ASSERT_EQ_INT(ctx, 0, sprite_call_count);
}

static void test_patinit_variants_select_animation_timer(test_context *ctx) {
    static sprite_pattern frame0 = {1, {{0, 0, 0, 13}}};
    static sprite_patterns_sp anim0 = {1, 5, {&frame0}};
    static sprite_patterns_sp anim1 = {1, 7, {&frame0}};
    static sprite_patterns_sp *table[] = {&anim0, &anim1};

    reset_act_s_state();
    actwk[4].pattbl = table;
    actwk[4].patno = 3;

    patinit(&actwk[4], 1);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[4].pattblno);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[4].patno);
    TEST_ASSERT_EQ_INT(ctx, 7, actwk[4].pctime);
    TEST_ASSERT_EQ_INT(ctx, 7, actwk[4].pctime_b);

    actwk[4].patno = 3;
    patinit1(&actwk[4], 0);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[4].pattblno);
    TEST_ASSERT_EQ_INT(ctx, 3, actwk[4].patno);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[4].pctime);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[4].pctime_b);
}

static void test_n_patset_handles_flip_modes_clipping_and_link_limit(
    test_context *ctx) {
    static sprite_pattern many = {
        5,
        {{0, 0, 0, 20}, {4, 5, 8, 21}, {6, 7, 16, 22}, {8, 9, 24, 23},
         {127, 0, 8, 24}}};

    reset_act_s_state();
    actwk[3].sx_posi.w.h = 200;
    actwk[3].sy_posi.w.h = 190;
    SprBmp[20].xs = 16;
    SprBmp[20].ys = 16;
    SprBmp[21].xs = 18;
    SprBmp[21].ys = 20;
    SprBmp[22].xs = 22;
    SprBmp[22].ys = 24;
    SprBmp[23].xs = 26;
    SprBmp[23].ys = 28;
    SprBmp[24].xs = 250;
    SprBmp[24].ys = 16;

    n_patset(&actwk[3], &many);

    TEST_ASSERT_EQ_INT(ctx, 4, sprite_call_count);
    assert_sprite_call(ctx, 0, 218, 190, 20, 0, 0);
    assert_sprite_call(ctx, 1, 196, 195, 21, 1, 1);
    assert_sprite_call(ctx, 2, 224, 159, 22, 2, 2);
    assert_sprite_call(ctx, 3, 184, 153, 23, 3, 3);

    linkdata = 80;
    n_patset(&actwk[3], &many);
    TEST_ASSERT_EQ_INT(ctx, 4, sprite_call_count);
}

static void test_zpatset_draws_until_first_empty_z_slot(test_context *ctx) {
    static sprite_pattern frame0 = {1, {{0, 0, 0, 30}}};
    static sprite_patterns_sp anim = {1, 1, {&frame0}};
    static sprite_patterns_sp *table[] = {&anim};

    reset_act_s_state();
    SprBmp[30].xs = 10;
    SprBmp[30].ys = 10;
    set_actor_for_sprite(&actwk[6], table);
    set_actor_for_sprite(&actwk[7], table);
    z_buffer[16] = 6;
    z_buffer[17] = 7;
    z_buffer[18] = 0;

    zpatset(16);

    TEST_ASSERT_EQ_INT(ctx, 2, sprite_call_count);
    assert_sprite_call(ctx, 0, 168, 180, 30, 0, 0);
    assert_sprite_call(ctx, 1, 168, 180, 30, 1, 0);
}

TEST_MAIN_BEGIN;
test_actmain_ignores_empty_actor(&ctx);
test_actmain_dispatches_each_actor_number(&ctx);
test_actmain_clears_actor_marked_by_handler(&ctx);
test_action_visits_actor_bands_and_clears_unused_links(&ctx);
test_zbuf_init_clears_bucket_heads_only(&ctx);
test_zbuf_set_uses_first_empty_slot_and_clamps_depth(&ctx);
test_zbuf_set_handles_first_slot_and_full_bucket(&ctx);
test_zpatset_call_stops_on_empty_slot(&ctx);
test_patset_animates_and_draws_visible_pattern(&ctx);
test_patset_wraps_out_of_range_frame_and_respects_freeze(&ctx);
test_patset_skips_hidden_empty_and_inactive_patterns(&ctx);
test_patinit_variants_select_animation_timer(&ctx);
test_n_patset_handles_flip_modes_clipping_and_link_limit(&ctx);
test_zpatset_draws_until_first_empty_z_slot(&ctx);
TEST_MAIN_END
