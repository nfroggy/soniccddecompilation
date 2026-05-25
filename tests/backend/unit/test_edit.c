#include <string.h>

#define EDIT_DATA_ENTRY_COUNT 2
#include "support/test_runner.h"
#include "src/types.h"

short_union editmode;
short_union editno;
ushort_union swdata1;
Sint16 blkno;
Uint8 edit_user;
Sint32 CKeyTogle;
sprite_pattern *sncpat[1];
edit_data edit_tbl;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int actwkchk_count;
static int scramapad_count;
static sprite_status *scramapad_actor;
static Sint16 scramapad_x;
static Sint16 scramapad_y;
static Sint16 scramapad_result;

void actionsub(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
Sint16 scramapad(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi);

#include "src/edit.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    *ppActwk = 0;
    return -1;
}

Sint16 scramapad(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi) {
    ++scramapad_count;
    scramapad_actor = pActwk;
    scramapad_x = iXposi;
    scramapad_y = iYposi;
    return scramapad_result;
}

static void reset_edit_state(sprite_status *actor) {
    static sprite_pattern pattern0 = {1, {{-1, -2, 0, 100}}};
    static sprite_pattern pattern1 = {1, {{-3, -4, 0, 200}}};
    static sprite_pattern sonic_pattern = {1, {{0, 0, 0, 300}}};
    static sprite_pattern *entry0_patbase[1];
    static sprite_pattern *entry1_patbase[1];

    memset(actor, 0, sizeof(*actor));
    memset(&editmode, 0, sizeof(editmode));
    memset(&editno, 0, sizeof(editno));
    memset(&swdata1, 0, sizeof(swdata1));
    blkno = 0;
    edit_user = 0;
    CKeyTogle = 99;

    entry0_patbase[0] = &pattern0;
    entry1_patbase[0] = &pattern1;
    sncpat[0] = &sonic_pattern;

    memset(&edit_tbl, 0, sizeof(edit_tbl));
    edit_tbl.num = 2;
    edit_tbl.dat[0].actno = 40;
    edit_tbl.dat[0].sprpri = 5;
    edit_tbl.dat[0].patbase = entry0_patbase;
    edit_tbl.dat[0].offset = 1234;
    edit_tbl.dat[0].userflag_h = 6;
    edit_tbl.dat[0].rludrv = 2;
    edit_tbl.dat[0].userflag_l = 9;
    edit_tbl.dat[0].patno = 7;
    edit_tbl.dat[1].actno = 41;
    edit_tbl.dat[1].sprpri = 3;
    edit_tbl.dat[1].patbase = entry1_patbase;
    edit_tbl.dat[1].offset = 4321;
    edit_tbl.dat[1].userflag_h = 8;
    edit_tbl.dat[1].rludrv = 1;
    edit_tbl.dat[1].userflag_l = 11;
    edit_tbl.dat[1].patno = 4;

    actionsub_count = 0;
    actionsub_actor = 0;
    actwkchk_count = 0;
    scramapad_count = 0;
    scramapad_actor = 0;
    scramapad_x = 0;
    scramapad_y = 0;
    scramapad_result = 0x1357;
}

static void test_edit_returns_immediately_when_disabled(test_context *ctx) {
    sprite_status actor;

    reset_edit_state(&actor);
    actor.xposi.w.h = 100;
    actor.yposi.w.h = 200;
    actor.patno = 12;

    edit(&actor);

    TEST_ASSERT_EQ_INT(ctx, 100, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 12, actor.patno);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, scramapad_count);
}

static void test_edit_moves_right_and_applies_selected_entry(test_context *ctx) {
    sprite_status actor;

    reset_edit_state(&actor);
    editmode.b.h = 1;
    swdata1.b.h = 8;
    actor.xposi.w.h = 100;
    actor.yposi.w.h = 200;
    actor.sprvsize = 12;
    actor.mstno.b.h = 5;

    edit(&actor);

    TEST_ASSERT_EQ_INT(ctx, 108, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, scramapad_count);
    TEST_ASSERT_TRUE(ctx, scramapad_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 108, scramapad_x);
    TEST_ASSERT_EQ_INT(ctx, 212, scramapad_y);
    TEST_ASSERT_EQ_INT(ctx, 0x1357, blkno);
    TEST_ASSERT_TRUE(ctx, actor.patbase == edit_tbl.dat[0].patbase);
    TEST_ASSERT_EQ_INT(ctx, 1234, actor.sproffset);
    TEST_ASSERT_EQ_INT(ctx, 5, actor.sprpri);
    TEST_ASSERT_EQ_INT(ctx, 7, actor.patno);
    TEST_ASSERT_EQ_INT(ctx, 9, edit_user);
    TEST_ASSERT_EQ_INT(ctx, 6, actor.actflg);
    TEST_ASSERT_EQ_INT(ctx, 0, actor.mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, CKeyTogle);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == &actor);
}

static void test_edit_applies_up_down_and_left_movement(test_context *ctx) {
    sprite_status actor;

    reset_edit_state(&actor);
    editmode.b.h = 1;
    swdata1.b.h = 1 | 2 | 4;
    actor.xposi.w.h = 100;
    actor.yposi.w.h = 200;
    actor.sprvsize = 10;

    edit(&actor);

    TEST_ASSERT_EQ_INT(ctx, 92, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 92, scramapad_x);
    TEST_ASSERT_EQ_INT(ctx, 210, scramapad_y);
    TEST_ASSERT_EQ_INT(ctx, 0x1357, blkno);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_edit_cycles_to_next_entry(test_context *ctx) {
    sprite_status actor;

    reset_edit_state(&actor);
    editmode.b.h = 1;
    swdata1.b.l = 64;
    actor.xposi.w.h = 50;
    actor.yposi.w.h = 75;
    actor.sprvsize = 4;

    edit(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, editno.b.h);
    TEST_ASSERT_TRUE(ctx, actor.patbase == edit_tbl.dat[1].patbase);
    TEST_ASSERT_EQ_INT(ctx, 4321, actor.sproffset);
    TEST_ASSERT_EQ_INT(ctx, 3, actor.sprpri);
    TEST_ASSERT_EQ_INT(ctx, 4, actor.patno);
    TEST_ASSERT_EQ_INT(ctx, 11, edit_user);
    TEST_ASSERT_EQ_INT(ctx, 5, actor.actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_edit_cycle_wraps_at_entry_count(test_context *ctx) {
    sprite_status actor;

    reset_edit_state(&actor);
    editmode.b.h = 1;
    editno.b.h = 1;
    swdata1.b.l = 64;

    edit(&actor);

    TEST_ASSERT_EQ_INT(ctx, 0, editno.b.h);
    TEST_ASSERT_TRUE(ctx, actor.patbase == edit_tbl.dat[0].patbase);
}

static void test_edit_spawn_key_path_remains_disabled(test_context *ctx) {
    sprite_status actor;

    reset_edit_state(&actor);
    editmode.b.h = 1;
    editno.b.h = 1;
    CKeyTogle = 1;
    actor.xposi.w.h = 140;
    actor.yposi.w.h = 220;
    actor.sprvsize = 3;

    edit(&actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, CKeyTogle);
    TEST_ASSERT_TRUE(ctx, actor.patbase == edit_tbl.dat[1].patbase);
    TEST_ASSERT_EQ_INT(ctx, 4321, actor.sproffset);
    TEST_ASSERT_EQ_INT(ctx, 4, actor.patno);
    TEST_ASSERT_EQ_INT(ctx, 11, edit_user);
    TEST_ASSERT_EQ_INT(ctx, 1, scramapad_count);
    TEST_ASSERT_EQ_INT(ctx, 140, scramapad_x);
    TEST_ASSERT_EQ_INT(ctx, 223, scramapad_y);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_edit_exit_restores_sonic_display_fields(test_context *ctx) {
    sprite_status actor;

    reset_edit_state(&actor);
    editmode.b.h = 1;
    swdata1.b.l = 16;
    actor.xposi.w.h = 20;
    actor.yposi.w.h = 30;
    actor.sprvsize = 6;

    edit(&actor);

    TEST_ASSERT_EQ_INT(ctx, 0, editmode.b.h);
    TEST_ASSERT_TRUE(ctx, actor.patbase == sncpat);
    TEST_ASSERT_EQ_INT(ctx, 1920, actor.sproffset);
    TEST_ASSERT_EQ_INT(ctx, 2, actor.sprpri);
    TEST_ASSERT_EQ_INT(ctx, 0, actor.patno);
    TEST_ASSERT_EQ_INT(ctx, 4, actor.actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, scramapad_count);
    TEST_ASSERT_EQ_INT(ctx, 0x1357, blkno);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == &actor);
}

TEST_MAIN_BEGIN;
    test_edit_returns_immediately_when_disabled(&ctx);
    test_edit_moves_right_and_applies_selected_entry(&ctx);
    test_edit_applies_up_down_and_left_movement(&ctx);
    test_edit_cycles_to_next_entry(&ctx);
    test_edit_cycle_wraps_at_entry_count(&ctx);
    test_edit_spawn_key_path_remains_disabled(&ctx);
    test_edit_exit_restores_sonic_display_fields(&ctx);
TEST_MAIN_END
