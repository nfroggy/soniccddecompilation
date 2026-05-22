#include <string.h>

#include "actset.h"
#include "support/test_runner.h"
#include "types.h"

typedef void *test_handle;
typedef unsigned long test_dword;
#define TEST_STILL_ACTIVE 259UL
__declspec(dllimport) test_handle __stdcall CreateThread(
    void *thread_attributes, size_t stack_size,
    test_dword(__stdcall *start_address)(void *), void *parameter,
    test_dword creation_flags, test_dword *thread_id);
__declspec(dllimport) void __stdcall Sleep(test_dword milliseconds);
__declspec(dllimport) int __stdcall GetExitCodeThread(test_handle thread,
                                                      test_dword *exit_code);
__declspec(dllimport) int __stdcall CloseHandle(test_handle object);

Uint8 flagworkcnt;
Uint8 flagworkcnt2;
Uint8 time_flag;
char time_item;
Sint16 asetposi;
short_union actset_rno;
aset_info *asetadr;
aset_info *asetadr2;
asetz_info *asetadrz;
asetz_info *asetadrz2;
int_union scra_h_posit;
sprite_status actwk[128];
Uint8 flagwork[766];
aset_info asettbl[4];
asetz_info asettblz[1];
Uint8 dummyarea[16];
extern aset_info *pAsetadr_dummy;
extern aset_info *pAsetadr_w;
extern asetz_info *pAsetadrz_w;

static int memset_callback_count;
static void *memset_callback_ptr;
static Uint8 memset_callback_value;
static Sint32 memset_callback_size;
static int frameout_callback_count;
static sprite_status *frameout_callback_actor;

static void memset_callback(void *ptr, Uint8 value, Sint32 size);
void (*sMemSet)(void *, Uint8, Sint32) = memset_callback;
void frameout(sprite_status *pActwk);

static void memset_callback(void *ptr, Uint8 value, Sint32 size) {
    ++memset_callback_count;
    memset_callback_ptr = ptr;
    memset_callback_value = value;
    memset_callback_size = size;
    memset(ptr, value, (size_t)size);
}

void frameout(sprite_status *pActwk) {
    ++frameout_callback_count;
    frameout_callback_actor = pActwk;
    memset(pActwk, 0, sizeof(*pActwk));
}

static void reset_actset_state(void) {
    memset(&flagworkcnt, 0, sizeof(flagworkcnt));
    memset(&flagworkcnt2, 0, sizeof(flagworkcnt2));
    memset(&time_flag, 0, sizeof(time_flag));
    memset(&time_item, 0, sizeof(time_item));
    memset(&asetposi, 0, sizeof(asetposi));
    memset(&actset_rno, 0, sizeof(actset_rno));
    memset(&scra_h_posit, 0, sizeof(scra_h_posit));
    memset(actwk, 0, sizeof(actwk));
    memset(flagwork, 0, sizeof(flagwork));
    memset(asettbl, 0, sizeof(asettbl));
    memset(asettblz, 0, sizeof(asettblz));
    memset(dummyarea, 0, sizeof(dummyarea));
    asetadr = 0;
    asetadr2 = 0;
    asetadrz = 0;
    asetadrz2 = 0;
    pAsetadr_dummy = 0;
    pAsetadr_w = 0;
    pAsetadrz_w = 0;
    memset_callback_count = 0;
    memset_callback_ptr = 0;
    memset_callback_value = 0;
    memset_callback_size = 0;
    frameout_callback_count = 0;
    frameout_callback_actor = 0;
}

static void fill_spawn_pool(void) {
    int i;

    for (i = 32; i < 128; ++i) {
        actwk[i].actno = 1;
    }
}

static void test_flagwkclr_resets_counters_and_flagwork(test_context *ctx) {
    reset_actset_state();
    flagworkcnt = 9;
    flagworkcnt2 = 8;
    memset(flagwork, 0xAA, sizeof(flagwork));

    flagwkclr();

    TEST_ASSERT_EQ_INT(ctx, 1, flagworkcnt);
    TEST_ASSERT_EQ_INT(ctx, 1, flagworkcnt2);
    TEST_ASSERT_EQ_INT(ctx, 1, memset_callback_count);
    TEST_ASSERT_TRUE(ctx, memset_callback_ptr == flagwork);
    TEST_ASSERT_EQ_INT(ctx, 0, memset_callback_value);
    TEST_ASSERT_EQ_INT(ctx, sizeof(flagwork), memset_callback_size);
    TEST_ASSERT_EQ_INT(ctx, 0, flagwork[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, flagwork[sizeof(flagwork) - 1]);
}

static void test_tm_setchk_checks_past_present_and_future_bits(
    test_context *ctx) {
    aset_info entry;
    Uint16 index;

    reset_actset_state();
    memset(&entry, 0, sizeof(entry));
    pAsetadr_w = &entry;

    entry.c = 1 << 5;
    time_flag = 0;
    index = 0xFFFF;
    TEST_ASSERT_EQ_INT(ctx, 1, tm_setchk(2, &index));
    TEST_ASSERT_EQ_INT(ctx, 6, index);

    entry.c = 2 << 5;
    time_flag = 1;
    index = 0xFFFF;
    TEST_ASSERT_EQ_INT(ctx, 1, tm_setchk(2, &index));
    TEST_ASSERT_EQ_INT(ctx, 7, index);

    entry.c = 4 << 5;
    time_flag = 2;
    index = 0xFFFF;
    TEST_ASSERT_EQ_INT(ctx, 1, tm_setchk(2, &index));
    TEST_ASSERT_EQ_INT(ctx, 8, index);

    entry.c = 0;
    time_flag = 0;
    TEST_ASSERT_EQ_INT(ctx, 0, tm_setchk(2, &index));
    time_flag = 1;
    TEST_ASSERT_EQ_INT(ctx, 0, tm_setchk(2, &index));
    time_flag = 2;
    TEST_ASSERT_EQ_INT(ctx, 0, tm_setchk(2, &index));
}

static test_dword __stdcall tm_setchk_nonreturning_thread(void *param) {
    Uint16 index;

    (void)param;
    index = 0;
    (void)tm_setchk(0, &index);
    return 0;
}

static void test_tm_setchk_hangs_for_unexpected_time_state(test_context *ctx) {
    static aset_info entry;
    test_handle thread;
    test_dword exit_code;

    reset_actset_state();
    memset(&entry, 0, sizeof(entry));
    pAsetadr_w = &entry;
    time_flag = 3;

    thread = CreateThread(0, 0, tm_setchk_nonreturning_thread, 0, 0, 0);
    TEST_ASSERT_NOT_NULL(ctx, thread);
    Sleep(50);
    exit_code = 0;
    TEST_ASSERT_TRUE(ctx, GetExitCodeThread(thread, &exit_code) != 0);
    TEST_ASSERT_EQ_INT(ctx, TEST_STILL_ACTIVE, exit_code);
    CloseHandle(thread);
}

static void test_actwkchk_finds_first_free_spawn_slot(test_context *ctx) {
    sprite_status *actor;

    reset_actset_state();
    actwk[32].actno = 3;
    actwk[33].actno = 0;
    actor = 0;

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk(&actor));
    TEST_ASSERT_TRUE(ctx, actor == &actwk[33]);
}

static void test_actwkchk_reports_full_spawn_pool(test_context *ctx) {
    sprite_status *actor;

    reset_actset_state();
    fill_spawn_pool();
    actor = 0;

    TEST_ASSERT_EQ_INT(ctx, -1, actwkchk(&actor));
    TEST_ASSERT_TRUE(ctx, actor == 0);
}

static void test_actwkchk2_finds_free_slot_after_actor(test_context *ctx) {
    sprite_status *actor;

    reset_actset_state();
    actwk[41].actno = 9;
    actwk[42].actno = 8;
    actwk[43].actno = 0;
    actor = 0;

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk2(&actwk[40], &actor));
    TEST_ASSERT_TRUE(ctx, actor == &actwk[43]);
}

static void test_actwkchk2_reports_no_later_free_slot(test_context *ctx) {
    sprite_status *actor;
    int i;

    reset_actset_state();
    for (i = 41; i < 128; ++i) {
        actwk[i].actno = 1;
    }
    actor = 0;

    TEST_ASSERT_EQ_INT(ctx, -1, actwkchk2(&actwk[40], &actor));
    TEST_ASSERT_TRUE(ctx, actor == 0);
}

static void test_actnoset_skips_when_time_gate_is_closed(test_context *ctx) {
    aset_info entries[2];
    Uint16 index;

    reset_actset_state();
    memset(entries, 0, sizeof(entries));
    entries[0].a = 5;
    entries[0].c = 0;
    pAsetadr_w = entries;
    time_flag = 0;
    index = 0xEEEE;

    TEST_ASSERT_EQ_INT(ctx, 0, actnoset(4, &index));
    TEST_ASSERT_TRUE(ctx, pAsetadr_w == &entries[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[32].actno);
    TEST_ASSERT_EQ_INT(ctx, 12, index);
}

static void test_actnoset_skips_already_spawned_flagged_entry(
    test_context *ctx) {
    aset_info entries[2];
    Uint16 index;

    reset_actset_state();
    memset(entries, 0, sizeof(entries));
    entries[0].a = 128 | 5;
    entries[0].c = 1 << 5;
    pAsetadr_w = entries;
    time_flag = 0;
    flagwork[3] = 128;
    index = 0;

    TEST_ASSERT_EQ_INT(ctx, 0, actnoset(1, &index));
    TEST_ASSERT_TRUE(ctx, pAsetadr_w == &entries[1]);
    TEST_ASSERT_EQ_INT(ctx, 128, flagwork[3]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[32].actno);
    TEST_ASSERT_EQ_INT(ctx, 3, index);
}

static void test_actnoset_spawns_actor_from_entry(test_context *ctx) {
    aset_info entries[2];
    Uint16 index;

    reset_actset_state();
    memset(entries, 0, sizeof(entries));
    entries[0].x = 123;
    entries[0].y = (Uint16)((2 << 14) | 456);
    entries[0].a = 128 | 7;
    entries[0].b = 9;
    entries[0].c = 1 << 5;
    entries[0].d = 11;
    pAsetadr_w = entries;
    time_flag = 0;
    index = 0;

    TEST_ASSERT_EQ_INT(ctx, 0, actnoset(2, &index));
    TEST_ASSERT_TRUE(ctx, pAsetadr_w == &entries[1]);
    TEST_ASSERT_EQ_INT(ctx, 128, flagwork[6]);
    TEST_ASSERT_EQ_INT(ctx, 6, index);
    TEST_ASSERT_EQ_INT(ctx, 123, actwk[32].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 456, actwk[32].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[32].actflg);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[32].cddat);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[32].cdsts);
    TEST_ASSERT_EQ_INT(ctx, 7, actwk[32].actno);
    TEST_ASSERT_EQ_INT(ctx, 9, actwk[32].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 11, actwk[32].userflag.b.l);
}

static void test_actnoset_spawns_unflagged_actor(test_context *ctx) {
    aset_info entries[2];
    Uint16 index;

    reset_actset_state();
    memset(entries, 0, sizeof(entries));
    entries[0].x = 321;
    entries[0].y = 654;
    entries[0].a = 12;
    entries[0].b = 3;
    entries[0].c = 1 << 5;
    entries[0].d = 4;
    pAsetadr_w = entries;
    time_flag = 0;
    index = 0;

    TEST_ASSERT_EQ_INT(ctx, 0, actnoset(5, &index));
    TEST_ASSERT_EQ_INT(ctx, 321, actwk[32].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 654, actwk[32].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[32].actflg);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[32].cddat);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[32].cdsts);
    TEST_ASSERT_EQ_INT(ctx, 12, actwk[32].actno);
    TEST_ASSERT_EQ_INT(ctx, 0, flagwork[15]);
}

static void test_actnoset_reports_allocation_failure(test_context *ctx) {
    aset_info entries[2];
    Uint16 index;

    reset_actset_state();
    memset(entries, 0, sizeof(entries));
    entries[0].a = 128 | 5;
    entries[0].c = 1 << 5;
    pAsetadr_w = entries;
    time_flag = 0;
    fill_spawn_pool();
    index = 0;

    TEST_ASSERT_EQ_INT(ctx, -1, actnoset(1, &index));
    TEST_ASSERT_TRUE(ctx, pAsetadr_w == &entries[0]);
    TEST_ASSERT_EQ_INT(ctx, 128, flagwork[3]);
}

static void test_actsetinit_sets_pointers_and_calls_actset(test_context *ctx) {
    reset_actset_state();
    scra_h_posit.w.h = 96;
    asettbl[0].x = 640;
    asettblz[0].x = 65535;

    actsetinit();

    TEST_ASSERT_EQ_INT(ctx, 2, actset_rno.w);
    TEST_ASSERT_TRUE(ctx, pAsetadr_dummy == (aset_info *)dummyarea);
    TEST_ASSERT_TRUE(ctx, asetadr == asettbl);
    TEST_ASSERT_TRUE(ctx, asetadr2 == asettbl);
    TEST_ASSERT_TRUE(ctx, asetadrz == asettblz);
    TEST_ASSERT_TRUE(ctx, asetadrz2 == asettblz);
    TEST_ASSERT_TRUE(ctx, pAsetadrz_w == asettblz);
    TEST_ASSERT_EQ_INT(ctx, 1, flagworkcnt);
    TEST_ASSERT_EQ_INT(ctx, 1, flagworkcnt2);
    TEST_ASSERT_EQ_INT(ctx, 0, asetposi);
}

static void test_actsetinit_skips_entries_before_initial_window(
    test_context *ctx) {
    reset_actset_state();
    scra_h_posit.w.h = 512;
    asettbl[0].x = 0;
    asettbl[0].a = 128 | 1;
    asettbl[0].c = 1 << 5;
    asettbl[1].x = 128;
    asettbl[1].a = 2;
    asettbl[1].c = 1 << 5;
    asettbl[2].x = 256;
    asettbl[2].a = 128 | 3;
    asettbl[2].c = 1 << 5;
    asettbl[3].x = 1200;
    asettbl[3].c = 1 << 5;

    actsetinit();

    TEST_ASSERT_EQ_INT(ctx, 2, actset_rno.w);
    TEST_ASSERT_TRUE(ctx, asetadr == &asettbl[3]);
    TEST_ASSERT_TRUE(ctx, asetadr2 == &asettbl[3]);
    TEST_ASSERT_EQ_INT(ctx, 3, flagworkcnt);
    TEST_ASSERT_EQ_INT(ctx, 3, flagworkcnt2);
    TEST_ASSERT_EQ_INT(ctx, 512, asetposi);
}

static void test_actsetchk_initializes_then_reuses_state(test_context *ctx) {
    reset_actset_state();
    asettbl[0].x = 640;
    actset_rno.w = 0;

    actsetchk();
    TEST_ASSERT_EQ_INT(ctx, 2, actset_rno.w);

    scra_h_posit.w.h = 0;
    actsetchk();
    TEST_ASSERT_EQ_INT(ctx, 2, actset_rno.w);
}

static void test_actset_returns_when_screen_bucket_is_unchanged(
    test_context *ctx) {
    reset_actset_state();
    aset_info entries[1];
    memset(entries, 0, sizeof(entries));
    asetadr = entries;
    asetadr2 = entries;
    asetposi = 256;
    scra_h_posit.w.h = 300;

    actset();

    TEST_ASSERT_TRUE(ctx, asetadr == entries);
    TEST_ASSERT_TRUE(ctx, asetadr2 == entries);
    TEST_ASSERT_EQ_INT(ctx, 256, asetposi);
}

static void test_actset_forward_spawns_entries_in_window(test_context *ctx) {
    aset_info entries[3];

    reset_actset_state();
    memset(entries, 0, sizeof(entries));
    entries[0].x = 100;
    entries[0].y = 200;
    entries[0].a = 128 | 10;
    entries[0].b = 1;
    entries[0].c = 1 << 5;
    entries[0].d = 2;
    entries[1].x = 700;
    entries[1].c = 1 << 5;
    entries[2].x = 900;
    asetadr = entries;
    asetadr2 = entries;
    flagworkcnt = 1;
    flagworkcnt2 = 1;
    asetposi = -1;
    scra_h_posit.w.h = 0;

    actset();

    TEST_ASSERT_EQ_INT(ctx, 0, asetposi);
    TEST_ASSERT_TRUE(ctx, asetadr == &entries[1]);
    TEST_ASSERT_TRUE(ctx, asetadr2 == entries);
    TEST_ASSERT_EQ_INT(ctx, 2, flagworkcnt);
    TEST_ASSERT_EQ_INT(ctx, 1, flagworkcnt2);
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[32].actno);
    TEST_ASSERT_EQ_INT(ctx, 100, actwk[32].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 128, flagwork[3]);
}

static void test_actset_forward_rolls_back_failed_flagged_entry(
    test_context *ctx) {
    aset_info entries[2];

    reset_actset_state();
    memset(entries, 0, sizeof(entries));
    entries[0].x = 100;
    entries[0].a = 128 | 10;
    entries[0].c = 1 << 5;
    entries[1].x = 700;
    asetadr = entries;
    asetadr2 = entries;
    flagworkcnt = 4;
    flagworkcnt2 = 1;
    asetposi = -1;
    scra_h_posit.w.h = 0;
    fill_spawn_pool();

    actset();

    TEST_ASSERT_TRUE(ctx, asetadr == entries);
    TEST_ASSERT_EQ_INT(ctx, 4, flagworkcnt);
    TEST_ASSERT_EQ_INT(ctx, 0, flagwork[12]);
}

static void test_actset_backward_spawns_entry_and_updates_windows(
    test_context *ctx) {
    aset_info entries[4];

    reset_actset_state();
    memset(entries, 0, sizeof(entries));
    entries[0].x = 0;
    entries[1].x = 300;
    entries[1].y = 44;
    entries[1].a = 128 | 13;
    entries[1].c = 1 << 5;
    entries[2].x = 1000;
    entries[2].a = 128 | 1;
    entries[3].x = 1000;
    asetadr = &entries[3];
    asetadr2 = &entries[2];
    flagworkcnt = 6;
    flagworkcnt2 = 4;
    asetposi = 512;
    scra_h_posit.w.h = 256;

    actset();

    TEST_ASSERT_EQ_INT(ctx, 256, asetposi);
    TEST_ASSERT_TRUE(ctx, asetadr2 == &entries[1]);
    TEST_ASSERT_TRUE(ctx, asetadr == &entries[2]);
    TEST_ASSERT_EQ_INT(ctx, 5, flagworkcnt);
    TEST_ASSERT_EQ_INT(ctx, 3, flagworkcnt2);
    TEST_ASSERT_EQ_INT(ctx, 13, actwk[32].actno);
    TEST_ASSERT_EQ_INT(ctx, 300, actwk[32].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 128, flagwork[9]);
}

static void test_actset_forward_advances_secondary_window(test_context *ctx) {
    aset_info entries[4];

    reset_actset_state();
    memset(entries, 0, sizeof(entries));
    entries[0].x = 64;
    entries[0].a = 128 | 1;
    entries[0].c = 1 << 5;
    entries[1].x = 128;
    entries[1].c = 1 << 5;
    entries[2].x = 900;
    entries[2].c = 1 << 5;
    entries[3].x = 1000;
    asetadr = &entries[2];
    asetadr2 = &entries[0];
    flagworkcnt = 5;
    flagworkcnt2 = 7;
    asetposi = -1;
    scra_h_posit.w.h = 256;

    actset();

    TEST_ASSERT_EQ_INT(ctx, 256, asetposi);
    TEST_ASSERT_TRUE(ctx, asetadr == &entries[2]);
    TEST_ASSERT_TRUE(ctx, asetadr2 == &entries[1]);
    TEST_ASSERT_EQ_INT(ctx, 5, flagworkcnt);
    TEST_ASSERT_EQ_INT(ctx, 8, flagworkcnt2);
}

static void test_actset_backward_rolls_back_failed_flagged_entry(
    test_context *ctx) {
    aset_info entries[3];

    reset_actset_state();
    memset(entries, 0, sizeof(entries));
    entries[0].x = 0;
    entries[1].x = 300;
    entries[1].a = 128 | 13;
    entries[1].c = 1 << 5;
    entries[2].x = 500;
    asetadr = &entries[2];
    asetadr2 = &entries[2];
    flagworkcnt = 5;
    flagworkcnt2 = 4;
    asetposi = 512;
    scra_h_posit.w.h = 256;
    fill_spawn_pool();

    actset();

    TEST_ASSERT_TRUE(ctx, asetadr2 == &entries[2]);
    TEST_ASSERT_EQ_INT(ctx, 4, flagworkcnt2);
    TEST_ASSERT_EQ_INT(ctx, 0, flagwork[9]);
}

static void test_actset_backward_skips_when_prior_window_is_negative(
    test_context *ctx) {
    aset_info entries[2];

    reset_actset_state();
    memset(entries, 0, sizeof(entries));
    entries[0].x = 0;
    entries[1].x = 800;
    asetadr = &entries[1];
    asetadr2 = &entries[1];
    flagworkcnt = 7;
    flagworkcnt2 = 8;
    asetposi = 128;
    scra_h_posit.w.h = 0;

    actset();

    TEST_ASSERT_EQ_INT(ctx, 0, asetposi);
    TEST_ASSERT_TRUE(ctx, asetadr2 == &entries[1]);
    TEST_ASSERT_TRUE(ctx, asetadr == &entries[1]);
    TEST_ASSERT_EQ_INT(ctx, 7, flagworkcnt);
    TEST_ASSERT_EQ_INT(ctx, 8, flagworkcnt2);
}

static void test_frameout_s00_returns_for_visible_actor(test_context *ctx) {
    sprite_status actor;

    reset_actset_state();
    memset(&actor, 0, sizeof(actor));
    actor.xposi.w.h = 256;
    scra_h_posit.w.h = 256;

    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s00(&actor, actor.xposi.w.h));
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_callback_count);
}

static void test_frameout_s00_returns_for_forced_actor_without_time_warp(
    test_context *ctx) {
    sprite_status actor;

    reset_actset_state();
    memset(&actor, 0, sizeof(actor));
    actor.actflg = 128;
    time_flag = 0;

    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s00(&actor, 9999));
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_callback_count);
}

static void test_frameout_s00_clears_current_time_flag_and_frames_out(
    test_context *ctx) {
    sprite_status actor;

    reset_actset_state();
    memset(&actor, 0, sizeof(actor));
    actor.xposi.w.h = 2048;
    actor.cdsts = 2;
    scra_h_posit.w.h = 0;
    time_flag = 1;
    flagwork[7] = 255;

    TEST_ASSERT_EQ_INT(ctx, -1, frameout_s00(&actor, actor.xposi.w.h));
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_callback_count);
    TEST_ASSERT_TRUE(ctx, frameout_callback_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 127, flagwork[7]);
}

static void test_frameout_s00_clamps_time_warp_index_low(test_context *ctx) {
    sprite_status actor;

    reset_actset_state();
    memset(&actor, 0, sizeof(actor));
    actor.actflg = 128;
    actor.cdsts = 3;
    time_flag = 128 | 0;
    time_item = 1;
    flagwork[9] = 255;

    TEST_ASSERT_EQ_INT(ctx, -1, frameout_s00(&actor, 0));
    TEST_ASSERT_EQ_INT(ctx, 127, flagwork[9]);
}

static void test_frameout_s00_clamps_time_warp_index_high(test_context *ctx) {
    sprite_status actor;

    reset_actset_state();
    memset(&actor, 0, sizeof(actor));
    actor.actflg = 128;
    actor.cdsts = 3;
    time_flag = 128 | 2;
    time_item = -1;
    flagwork[11] = 255;

    TEST_ASSERT_EQ_INT(ctx, -1, frameout_s00(&actor, 0));
    TEST_ASSERT_EQ_INT(ctx, 127, flagwork[11]);
}

static void test_frameout_s_wraps_current_x_position(test_context *ctx) {
    sprite_status actor;

    reset_actset_state();
    memset(&actor, 0, sizeof(actor));
    actor.xposi.w.h = 4096;

    TEST_ASSERT_EQ_INT(ctx, -1, frameout_s(&actor));
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_callback_count);
}

static void test_frameout_s0_always_frames_out_and_clears_flag(
    test_context *ctx) {
    sprite_status actor;

    reset_actset_state();
    memset(&actor, 0, sizeof(actor));
    actor.cdsts = 4;
    time_flag = 2;
    flagwork[14] = 255;

    TEST_ASSERT_EQ_INT(ctx, -1, frameout_s0(&actor));
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_callback_count);
    TEST_ASSERT_TRUE(ctx, frameout_callback_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 127, flagwork[14]);
}

static void test_frameout_s0_clamps_time_warp_index(test_context *ctx) {
    sprite_status actor;

    reset_actset_state();
    memset(&actor, 0, sizeof(actor));
    actor.cdsts = 4;
    time_flag = 128 | 0;
    time_item = 2;
    flagwork[12] = 255;

    TEST_ASSERT_EQ_INT(ctx, -1, frameout_s0(&actor));
    TEST_ASSERT_EQ_INT(ctx, 127, flagwork[12]);
}

static void test_frameout_s0_clamps_time_warp_index_high(test_context *ctx) {
    sprite_status actor;

    reset_actset_state();
    memset(&actor, 0, sizeof(actor));
    actor.cdsts = 4;
    time_flag = 128 | 2;
    time_item = -1;
    flagwork[14] = 255;

    TEST_ASSERT_EQ_INT(ctx, -1, frameout_s0(&actor));
    TEST_ASSERT_EQ_INT(ctx, 127, flagwork[14]);
}

TEST_MAIN_BEGIN;
    test_flagwkclr_resets_counters_and_flagwork(&ctx);
    test_tm_setchk_checks_past_present_and_future_bits(&ctx);
    test_actwkchk_finds_first_free_spawn_slot(&ctx);
    test_actwkchk_reports_full_spawn_pool(&ctx);
    test_actwkchk2_finds_free_slot_after_actor(&ctx);
    test_actwkchk2_reports_no_later_free_slot(&ctx);
    test_actnoset_skips_when_time_gate_is_closed(&ctx);
    test_actnoset_skips_already_spawned_flagged_entry(&ctx);
    test_actnoset_spawns_actor_from_entry(&ctx);
    test_actnoset_spawns_unflagged_actor(&ctx);
    test_actnoset_reports_allocation_failure(&ctx);
    test_actsetinit_sets_pointers_and_calls_actset(&ctx);
    test_actsetinit_skips_entries_before_initial_window(&ctx);
    test_actsetchk_initializes_then_reuses_state(&ctx);
    test_actset_returns_when_screen_bucket_is_unchanged(&ctx);
    test_actset_forward_spawns_entries_in_window(&ctx);
    test_actset_forward_rolls_back_failed_flagged_entry(&ctx);
    test_actset_backward_spawns_entry_and_updates_windows(&ctx);
    test_actset_backward_rolls_back_failed_flagged_entry(&ctx);
    test_actset_backward_skips_when_prior_window_is_negative(&ctx);
    test_actset_forward_advances_secondary_window(&ctx);
    test_frameout_s00_returns_for_visible_actor(&ctx);
    test_frameout_s00_returns_for_forced_actor_without_time_warp(&ctx);
    test_frameout_s00_clears_current_time_flag_and_frames_out(&ctx);
    test_frameout_s00_clamps_time_warp_index_low(&ctx);
    test_frameout_s00_clamps_time_warp_index_high(&ctx);
    test_frameout_s_wraps_current_x_position(&ctx);
    test_frameout_s0_always_frames_out_and_clears_flag(&ctx);
    test_frameout_s0_clamps_time_warp_index(&ctx);
    test_frameout_s0_clamps_time_warp_index_high(&ctx);
    test_tm_setchk_hangs_for_unexpected_time_state(&ctx);
TEST_MAIN_END;
