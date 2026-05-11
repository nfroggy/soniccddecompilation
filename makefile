BUILD_DIR := build
REFERENCE_DIR := references
LINKER_SCRIPT ?= linker.lcf
CC := compiler/mwccps2.exe
LD := compiler/mwldps2.exe

VARIANTS := R11A R11B R11C R11D R12A R12B R12C R12D R13C R13D R31A R31B R31C R31D R32A R32B R32C R32D R33C R33D R41A R41B R41C R41D R42A R42B R42C R42D R43C R43D R51A R51B R51C R51D R52A R52B R52C R52D R53C R53D R61A R61B R61C R61D R62A R62B R62C R62D R63C R63D R71A R71B R71C R71D R72A R72B R72C R72D R73C R73D R81A R81B R81C R81D R82A R82B R82C R82D R83C R83D SPECIAL WARP BESTTIME OPENING PLANET SAVEDATA SOUNDTST STAGETST TA THANKS VISUALMD

R11A_NAME := r11a
R11B_NAME := r11b
R11C_NAME := r11c
R11D_NAME := r11d
R12A_NAME := r12a
R12B_NAME := r12b
R12C_NAME := r12c
R12D_NAME := r12d
R13C_NAME := r13c
R13D_NAME := r13d
R31A_NAME := r31a
R31B_NAME := r31b
R31C_NAME := r31c
R31D_NAME := r31d
R32A_NAME := r32a
R32B_NAME := r32b
R32C_NAME := r32c
R32D_NAME := r32d
R33C_NAME := r33c
R33D_NAME := r33d
R41A_NAME := r41a
R41B_NAME := r41b
R41C_NAME := r41c
R41D_NAME := r41d
R42A_NAME := r42a
R42B_NAME := r42b
R42C_NAME := r42c
R42D_NAME := r42d
R43C_NAME := r43c
R43D_NAME := r43d
R51A_NAME := r51a
R51B_NAME := r51b
R51C_NAME := r51c
R51D_NAME := r51d
R52A_NAME := r52a
R52B_NAME := r52b
R52C_NAME := r52c
R52D_NAME := r52d
R53C_NAME := r53c
R53D_NAME := r53d
R61A_NAME := r61a
R61B_NAME := r61b
R61C_NAME := r61c
R61D_NAME := r61d
R62A_NAME := r62a
R62B_NAME := r62b
R62C_NAME := r62c
R62D_NAME := r62d
R63C_NAME := r63c
R63D_NAME := r63d
R71A_NAME := r71a
R71B_NAME := r71b
R71C_NAME := r71c
R71D_NAME := r71d
R72A_NAME := r72a
R72B_NAME := r72b
R72C_NAME := r72c
R72D_NAME := r72d
R73C_NAME := r73c
R73D_NAME := r73d
R81A_NAME := r81a
R81B_NAME := r81b
R81C_NAME := r81c
R81D_NAME := r81d
R82A_NAME := r82a
R82B_NAME := r82b
R82C_NAME := r82c
R82D_NAME := r82d
R83C_NAME := r83c
R83D_NAME := r83d
SPECIAL_NAME := special
WARP_NAME := warp
BESTTIME_NAME := besttime
OPENING_NAME := opening
PLANET_NAME := planet
SAVEDATA_NAME := savedata
SOUNDTST_NAME := soundtst
STAGETST_NAME := stagetst
TA_NAME := ta
THANKS_NAME := thanks
VISUALMD_NAME := visualmd
ELF_TARGETS := $(foreach variant,$(VARIANTS),$(BUILD_DIR)/$($(variant)_NAME).elf)

R11A_DEFINES := -DR1 -DR11A -DEDIT_DATA_ENTRY_COUNT=52
R11B_DEFINES := -DR1 -DR11B -DEDIT_DATA_ENTRY_COUNT=58
R11C_DEFINES := -DR1 -DR11C -DEDIT_DATA_ENTRY_COUNT=48
R11D_DEFINES := -DR1 -DR11D -DEDIT_DATA_ENTRY_COUNT=45
R12A_DEFINES := -DR1 -DR12A -DEDIT_DATA_ENTRY_COUNT=47
R12B_DEFINES := -DR1 -DR12B -DEDIT_DATA_ENTRY_COUNT=49
R12C_DEFINES := -DR1 -DR12C -DEDIT_DATA_ENTRY_COUNT=48
R12D_DEFINES := -DR1 -DR12D -DEDIT_DATA_ENTRY_COUNT=46
R13C_DEFINES := -DR1 -DR13C -DEDIT_DATA_ENTRY_COUNT=24
R13D_DEFINES := -DR1 -DR13D -DEDIT_DATA_ENTRY_COUNT=24
R31A_DEFINES := -DR3 -DR31 -DR31A -DEDIT_DATA_ENTRY_COUNT=30
R31B_DEFINES := -DR3 -DR31 -DR31B -DEDIT_DATA_ENTRY_COUNT=26
R31C_DEFINES := -DR3 -DR31 -DR31C -DEDIT_DATA_ENTRY_COUNT=23
R31D_DEFINES := -DR3 -DR31 -DR31D -DEDIT_DATA_ENTRY_COUNT=23
R32A_DEFINES := -DR3 -DR32 -DR32A -DCG_CHANGE -DEDIT_DATA_ENTRY_COUNT=36
R32B_DEFINES := -DR3 -DR32 -DR32B -DEDIT_DATA_ENTRY_COUNT=25
R32C_DEFINES := -DR3 -DR32 -DR32C -DCG_CHANGE -DEDIT_DATA_ENTRY_COUNT=23
R32D_DEFINES := -DR3 -DR32 -DR32D -DCG_CHANGE -DEDIT_DATA_ENTRY_COUNT=23
R33C_DEFINES := -DR3 -DR33 -DR33C -DEDIT_DATA_ENTRY_COUNT=8
R33D_DEFINES := -DR3 -DR33 -DR33D -DEDIT_DATA_ENTRY_COUNT=8
R41A_DEFINES := -DR4 -DR41A -DCG_CHANGE -DEDIT_DATA_ENTRY_COUNT=53
R41B_DEFINES := -DR4 -DR41B -DEDIT_DATA_ENTRY_COUNT=34
R41C_DEFINES := -DR4 -DR41C -DEDIT_DATA_ENTRY_COUNT=25
R41D_DEFINES := -DR4 -DR41D -DEDIT_DATA_ENTRY_COUNT=25
R42A_DEFINES := -DR4 -DR42A -DEDIT_DATA_ENTRY_COUNT=55
R42B_DEFINES := -DR4 -DR42B -DEDIT_DATA_ENTRY_COUNT=62
R42C_DEFINES := -DR4 -DR42C -DEDIT_DATA_ENTRY_COUNT=66
R42D_DEFINES := -DR4 -DR42D -DEDIT_DATA_ENTRY_COUNT=66
R43C_DEFINES := -DR4 -DR43C -DEDIT_DATA_ENTRY_COUNT=11
R43D_DEFINES := -DR4 -DR43D -DEDIT_DATA_ENTRY_COUNT=11
R51A_DEFINES := -DR5 -DR51 -DR51A -DEDIT_DATA_ENTRY_COUNT=91
R51B_DEFINES := -DR5 -DR51 -DR51B -DEDIT_DATA_ENTRY_COUNT=91
R51C_DEFINES := -DR5 -DR51 -DR51C -DEDIT_DATA_ENTRY_COUNT=91
R51D_DEFINES := -DR5 -DR51 -DR51D -DEDIT_DATA_ENTRY_COUNT=91
R52A_DEFINES := -DR5 -DR52 -DR52A -DEDIT_DATA_ENTRY_COUNT=91
R52B_DEFINES := -DR5 -DR52 -DR52B -DEDIT_DATA_ENTRY_COUNT=91
R52C_DEFINES := -DR5 -DR52 -DR52C -DEDIT_DATA_ENTRY_COUNT=91
R52D_DEFINES := -DR5 -DR52 -DR52D -DEDIT_DATA_ENTRY_COUNT=91
R53C_DEFINES := -DR5 -DR53 -DR53C -DEDIT_DATA_ENTRY_COUNT=57
R53D_DEFINES := -DR5 -DR53 -DR53D -DEDIT_DATA_ENTRY_COUNT=57
R61A_DEFINES := -DR6 -DR61 -DR61A -DCG_CHANGE -DEDIT_DATA_ENTRY_COUNT=55
R61B_DEFINES := -DR6 -DR61 -DR61B -DCG_CHANGE -DEDIT_DATA_ENTRY_COUNT=54
R61C_DEFINES := -DR6 -DR61 -DR61C -DCG_CHANGE -DEDIT_DATA_ENTRY_COUNT=53
R61D_DEFINES := -DR6 -DR61 -DR61D -DCG_CHANGE -DEDIT_DATA_ENTRY_COUNT=53
R62A_DEFINES := -DR6 -DR62 -DR62A -DCG_CHANGE -DEDIT_DATA_ENTRY_COUNT=56
R62B_DEFINES := -DR6 -DR62 -DR62B -DCG_CHANGE -DEDIT_DATA_ENTRY_COUNT=56
R62C_DEFINES := -DR6 -DR62 -DR62C -DCG_CHANGE -DEDIT_DATA_ENTRY_COUNT=53
R62D_DEFINES := -DR6 -DR62 -DR62D -DCG_CHANGE -DEDIT_DATA_ENTRY_COUNT=53
R63C_DEFINES := -DR6 -DR63 -DR63C -DCG_CHANGE -DEDIT_DATA_ENTRY_COUNT=38
R63D_DEFINES := -DR6 -DR63 -DR63D -DCG_CHANGE -DEDIT_DATA_ENTRY_COUNT=38
R71A_DEFINES := -DR7 -DR71 -DR71A -DEDIT_DATA_ENTRY_COUNT=79
R71B_DEFINES := -DR7 -DR71 -DR71B -DEDIT_DATA_ENTRY_COUNT=79
R71C_DEFINES := -DR7 -DR71 -DR71C -DEDIT_DATA_ENTRY_COUNT=79
R71D_DEFINES := -DR7 -DR71 -DR71D -DEDIT_DATA_ENTRY_COUNT=79
R72A_DEFINES := -DR7 -DR72 -DR72A -DEDIT_DATA_ENTRY_COUNT=79
R72B_DEFINES := -DR7 -DR72 -DR72B -DEDIT_DATA_ENTRY_COUNT=79
R72C_DEFINES := -DR7 -DR72 -DR72C -DEDIT_DATA_ENTRY_COUNT=79
R72D_DEFINES := -DR7 -DR72 -DR72D -DEDIT_DATA_ENTRY_COUNT=79
R73C_DEFINES := -DR7 -DR73 -DR73C -DEDIT_DATA_ENTRY_COUNT=52
R73D_DEFINES := -DR7 -DR73 -DR73D -DEDIT_DATA_ENTRY_COUNT=52
R81A_DEFINES := -DR8 -DR81 -DR81A -DCG_CHANGE -DEDIT_DATA_ENTRY_COUNT=38
R81B_DEFINES := -DR8 -DR81 -DR81B -DEDIT_DATA_ENTRY_COUNT=38
R81C_DEFINES := -DR8 -DR81 -DR81C -DCG_CHANGE -DEDIT_DATA_ENTRY_COUNT=38
R81D_DEFINES := -DR8 -DR81 -DR81D -DCG_CHANGE -DEDIT_DATA_ENTRY_COUNT=38
R82A_DEFINES := -DR8 -DR82 -DR82A -DCG_CHANGE -DEDIT_DATA_ENTRY_COUNT=10
R82B_DEFINES := -DR8 -DR82 -DR82B -DCG_CHANGE -DEDIT_DATA_ENTRY_COUNT=10
R82C_DEFINES := -DR8 -DR82 -DR82C -DCG_CHANGE -DEDIT_DATA_ENTRY_COUNT=10
R82D_DEFINES := -DR8 -DR82 -DR82D -DCG_CHANGE -DEDIT_DATA_ENTRY_COUNT=10
R83C_DEFINES := -DR8 -DR83 -DR83C -DCG_CHANGE -DEDIT_DATA_ENTRY_COUNT=45
R83D_DEFINES := -DR8 -DR83 -DR83D -DCG_CHANGE -DEDIT_DATA_ENTRY_COUNT=45
SPECIAL_DEFINES :=
WARP_DEFINES :=
BESTTIME_DEFINES :=
OPENING_DEFINES := -DOPENING
PLANET_DEFINES :=
SAVEDATA_DEFINES := -DSAVEDATA
SOUNDTST_DEFINES := -DSOUNDTST
STAGETST_DEFINES := -DSTAGETST
TA_DEFINES :=
THANKS_DEFINES :=
VISUALMD_DEFINES :=

PROJECT_INCLUDES := -i "main" -i "src/r1"
INCLUDES := $(PROJECT_INCLUDES)
CFLAGS := -cwd include -nostdinc -fl divbyzerocheck -g -nodead -sdatathreshold 0
COMPILE_ONLY := -c
LDFLAGS := -nostdlib -nodead -g -main ""

R11_PREFIX_SOURCES := \
	src/dllmain.c \
	src/action.c \
	src/actset.c \
	src/equ.c \
	src/dai_k.c \
	src/dai_rd1.c \
	src/dircol.c \
	src/dummy.c \
	src/edit.c

R11_POST_EDIT_SOURCES := \
	src/enemy.c \
	src/et1.c \
	src/etc.c \
	src/fcol.c \
	src/game.c \
	src/goal.c \
	src/io.c \
	src/item.c \
	src/player.c \
	src/playpat1.c \
	src/playsp.c \
	src/playsub.c \
	src/plchg.c \
	src/ridechk.c \
	src/ring.c \
	src/score.c \
	src/scrchk.c \
	src/spring.c \
	src/suicide.c \
	src/taki.c \
	src/tbl0.c \
	src/tbl1.c \
	src/tree.c \
	src/zone.c \
	src/zonetbl1.c

R11C_POST_EDIT_SOURCES := \
	src/enemy.c \
	src/et1.c \
	src/etc.c \
	src/fcol.c \
	src/game.c \
	src/goal.c \
	src/io.c \
	src/item.c \
	src/player.c \
	src/playpat1.c \
	src/playsp.c \
	src/playsub.c \
	src/plchg.c \
	src/ridechk.c \
	src/ring.c \
	src/score.c \
	src/scrchk.c \
	src/spring.c \
	src/suicide.c \
	src/taki.c \
	src/tbl0.c \
	src/tbl1.c \
	src/zone.c \
	src/zonetbl1.c

R11_TAIL_SOURCES := \
	src/block.c \
	src/loader2.c \
	src/r1/coli1.c \
	src/save.c \
	src/r1/friend1.c \
	src/r1/movie1.c \
	src/bmp.c

R11A_SOURCES := \
	$(R11_PREFIX_SOURCES) \
	src/emie1.c \
	src/emie1cg.c \
	$(R11_POST_EDIT_SOURCES) \
	src/r1/act11a.c \
	src/r1/col1a.c \
	src/r1/dev11a.c \
	src/r1/edtbl11a.c \
	src/r1/kowasi1.c \
	src/r1/kuzur11a.c \
	src/r1/kuzure.c \
	src/r1/scr11a.c \
	src/r1/shoot1.c \
	src/r1/sikake.c \
	src/r1/z11act.c \
	src/block.c \
	src/loader2.c \
	src/r1/coli1.c \
	src/save.c \
	src/r1/z11atbl.c \
	src/r1/friend1.c \
	src/r1/movie1.c \
	src/bmp.c

R11B_SOURCES := \
	$(R11_PREFIX_SOURCES) \
	$(R11_POST_EDIT_SOURCES) \
	src/r1/kowasi1.c \
	src/r1/kuzure.c \
	src/r1/shoot1.c \
	src/r1/sikake.c \
	src/r1/z11act.c \
	src/block.c \
	src/loader2.c \
	src/bmp.c \
	src/r1/coli1.c \
	src/r1/act11b.c \
	src/r1/col1b.c \
	src/r1/dev11b.c \
	src/r1/edtbl11b.c \
	src/r1/kuzur11b.c \
	src/r1/scr11b.c \
	src/r1/z11btbl.c \
	src/r1/friend1.c \
	src/r1/movie1.c \
	src/save.c

R11C_SOURCES := \
	$(R11_PREFIX_SOURCES) \
	$(R11C_POST_EDIT_SOURCES) \
	src/r1/kowasi1.c \
	src/r1/kuzure.c \
	src/r1/shoot1.c \
	src/r1/sikake.c \
	src/block.c \
	src/loader2.c \
	src/bmp.c \
	src/r1/coli1.c \
	src/r1/dev11c.c \
	src/r1/scr11c.c \
	src/r1/z11act.c \
	src/r1/z11ctbl.c \
	src/r1/act11b.c \
	src/r1/edtbl11c.c \
	src/r1/kuzur11c.c \
	src/r1/col1c.c \
	src/r1/friend1.c \
	src/r1/movie1.c \
	src/save.c

R11D_SOURCES := \
	$(R11_PREFIX_SOURCES) \
	$(R11C_POST_EDIT_SOURCES) \
	src/r1/kowasi1.c \
	src/r1/kuzure.c \
	src/r1/shoot1.c \
	src/r1/sikake.c \
	src/block.c \
	src/loader2.c \
	src/bmp.c \
	src/r1/coli1.c \
	src/r1/z11act.c \
	src/r1/act11b.c \
	src/r1/dev11d.c \
	src/r1/edtbl11d.c \
	src/r1/kuzur11d.c \
	src/r1/z11dtbl.c \
	src/r1/col1d.c \
	src/r1/scr11d.c \
	src/r1/friend1.c \
	src/r1/movie1.c \
	src/save.c

R12A_SOURCES := \
	$(R11_PREFIX_SOURCES) \
	$(R11_POST_EDIT_SOURCES) \
	src/r1/col1a.c \
	src/r1/kowasi1.c \
	src/r1/kuzure.c \
	src/r1/shoot1.c \
	src/r1/sikake.c \
	src/block.c \
	src/loader2.c \
	src/bmp.c \
	src/r1/coli1.c \
	src/r1/act12a.c \
	src/r1/dev12a.c \
	src/r1/edtbl12a.c \
	src/r1/kuzur12a.c \
	src/branko1.c \
	src/r1/scr12a.c \
	src/r1/z12act.c \
	src/r1/z12atbl.c \
	src/r1/friend1.c \
	src/r1/movie1.c \
	src/save.c

R12B_SOURCES := \
	$(R11_PREFIX_SOURCES) \
	$(R11_POST_EDIT_SOURCES) \
	src/r1/kowasi1.c \
	src/r1/kuzure.c \
	src/r1/shoot1.c \
	src/r1/sikake.c \
	src/block.c \
	src/loader2.c \
	src/bmp.c \
	src/r1/coli1.c \
	src/r1/act12a.c \
	src/r1/col1b.c \
	src/r1/dev12b.c \
	src/r1/edtbl12b.c \
	src/r1/kuzur12b.c \
	src/r1/scr12b.c \
	src/r1/z12act.c \
	src/r1/z12btbl.c \
	src/branko1.c \
	src/r1/friend1.c \
	src/r1/movie1.c \
	src/save.c

R12C_SOURCES := \
	$(R11_PREFIX_SOURCES) \
	$(R11C_POST_EDIT_SOURCES) \
	src/r1/kowasi1.c \
	src/r1/kuzure.c \
	src/r1/shoot1.c \
	src/r1/sikake.c \
	src/block.c \
	src/loader2.c \
	src/bmp.c \
	src/r1/coli1.c \
	src/r1/col1c.c \
	src/r1/act12a.c \
	src/r1/dev12c.c \
	src/r1/edtbl12c.c \
	src/r1/kuzur12c.c \
	src/r1/scr12c.c \
	src/r1/z12act.c \
	src/r1/z12ctbl.c \
	src/branko1.c \
	src/r1/friend1.c \
	src/r1/movie1.c \
	src/save.c

R12D_SOURCES := \
	$(R11_PREFIX_SOURCES) \
	$(R11C_POST_EDIT_SOURCES) \
	src/r1/kowasi1.c \
	src/r1/kuzure.c \
	src/r1/shoot1.c \
	src/r1/sikake.c \
	src/block.c \
	src/loader2.c \
	src/bmp.c \
	src/r1/coli1.c \
	src/r1/act12a.c \
	src/r1/col1d.c \
	src/r1/dev12d.c \
	src/r1/edtbl12d.c \
	src/r1/kuzur12d.c \
	src/r1/scr12d.c \
	src/r1/z12act.c \
	src/r1/z12dtbl.c \
	src/branko1.c \
	src/r1/friend1.c \
	src/r1/movie1.c \
	src/save.c

R13C_SOURCES := \
	src/dllmain.c \
	src/action.c \
	src/actset.c \
	src/dai_rd1.c \
	src/dircol.c \
	src/dummy.c \
	src/edit.c \
	src/enemy.c \
	src/equ.c \
	src/etc.c \
	src/fcol.c \
	src/game.c \
	src/io.c \
	src/item.c \
	src/player.c \
	src/playpat1.c \
	src/playsp.c \
	src/playsub.c \
	src/plchg.c \
	src/ridechk.c \
	src/ring.c \
	src/score.c \
	src/scrchk.c \
	src/spring.c \
	src/suicide.c \
	src/tbl0.c \
	src/tbl1.c \
	src/tree.c \
	src/zone.c \
	src/zonetbl1.c \
	src/r1/shoot1.c \
	src/r1/sikake.c \
	src/block.c \
	src/loader2.c \
	src/bmp.c \
	src/r1/coli1.c \
	src/r1/act13c.c \
	src/r1/col1c.c \
	src/r1/dev13c.c \
	src/r1/edtbl13c.c \
	src/r1/scr13c.c \
	src/r1/z13act.c \
	src/r1/z13ctbl.c \
	src/et1.c \
	src/goal.c \
	src/r1/boss_1.c \
	src/r1/friend1.c \
	src/save.c

R13D_SOURCES := \
	src/dllmain.c \
	src/action.c \
	src/actset.c \
	src/dai_rd1.c \
	src/dircol.c \
	src/dummy.c \
	src/edit.c \
	src/enemy.c \
	src/equ.c \
	src/etc.c \
	src/fcol.c \
	src/game.c \
	src/io.c \
	src/item.c \
	src/player.c \
	src/playpat1.c \
	src/playsp.c \
	src/playsub.c \
	src/plchg.c \
	src/ridechk.c \
	src/ring.c \
	src/score.c \
	src/scrchk.c \
	src/spring.c \
	src/suicide.c \
	src/tbl0.c \
	src/tbl1.c \
	src/tree.c \
	src/zone.c \
	src/zonetbl1.c \
	src/r1/shoot1.c \
	src/r1/sikake.c \
	src/block.c \
	src/loader2.c \
	src/bmp.c \
	src/r1/coli1.c \
	src/et1.c \
	src/goal.c \
	src/r1/act13c.c \
	src/r1/col1d.c \
	src/r1/dev13d.c \
	src/r1/edtbl13c.c \
	src/r1/scr13d.c \
	src/r1/z13dtbl.c \
	src/r1/z13act.c \
	src/r1/boss_1.c \
	src/r1/friend1.c \
	src/save.c

R31A_SOURCES := \
	src/actset.c \
	src/dircol.c \
	src/dummy.c \
	src/edit.c \
	src/equ.c \
	src/etc.c \
	src/fcol.c \
	src/game.c \
	src/goal.c \
	src/io.c \
	src/item.c \
	src/player.c \
	src/playsub.c \
	src/ridechk.c \
	src/ring.c \
	src/score.c \
	src/scrchk.c \
	src/spring.c \
	src/suicide.c \
	src/zone.c \
	src/loader2.c \
	src/r3/banpa.c \
	src/r3/bobin.c \
	src/r3/bobinb.c \
	src/r3/coli3.c \
	src/r3/edtbl31a.c \
	src/r3/et3.c \
	src/r3/friend3.c \
	src/r3/ga3.c \
	src/r3/playsp3.c \
	src/r3/pocket.c \
	src/r3/togebl3a.c \
	src/r3/trap_r3.c \
	src/r3/z31atbl.c \
	src/dllmain.c \
	src/bmp.c \
	src/block.c \
	src/r3/miracle.c \
	src/tbl0.c \
	src/tbl1.c \
	src/r3/col3a.c \
	src/r3/scr31a.c \
	src/r3/z31act.c \
	src/plchg.c \
	src/r3/dev31a.c \
	src/r3/act31a.c \
	src/r3/mecasnc.c \
	src/r3/msnccg.c \
	src/r3/tentou.c \
	src/r3/kama.c \
	src/plpat6.c \
	src/r3/movie3.c \
	src/r3/zonetbl3.c \
	src/save.c \
	src/action.c

R31B_SOURCES := \
	src/actset.c \
	src/dircol.c \
	src/dummy.c \
	src/edit.c \
	src/equ.c \
	src/etc.c \
	src/fcol.c \
	src/game.c \
	src/goal.c \
	src/io.c \
	src/item.c \
	src/player.c \
	src/playsub.c \
	src/ridechk.c \
	src/ring.c \
	src/score.c \
	src/scrchk.c \
	src/spring.c \
	src/suicide.c \
	src/zone.c \
	src/loader2.c \
	src/r3/banpa.c \
	src/r3/bobin.c \
	src/r3/bobinb.c \
	src/r3/coli3.c \
	src/r3/et3.c \
	src/r3/friend3.c \
	src/r3/ga3.c \
	src/r3/playsp3.c \
	src/r3/pocket.c \
	src/r3/trap_r3.c \
	src/dllmain.c \
	src/bmp.c \
	src/block.c \
	src/r3/miracle.c \
	src/tbl0.c \
	src/tbl1.c \
	src/r3/z31act.c \
	src/plchg.c \
	src/r3/msnccg.c \
	src/r3/tentou.c \
	src/r3/kama.c \
	src/plpat6.c \
	src/r3/act31b.c \
	src/r3/col3b.c \
	src/r3/dev31b.c \
	src/r3/edtbl31b.c \
	src/r3/z31btbl.c \
	src/r3/zonetbl3.c \
	src/r3/scr31b.c \
	src/r3/movie3.c \
	src/save.c \
	src/r3/togebl3b.c \
	src/action.c

R31C_SOURCES := \
	src/actset.c \
	src/dircol.c \
	src/dummy.c \
	src/edit.c \
	src/equ.c \
	src/etc.c \
	src/fcol.c \
	src/game.c \
	src/goal.c \
	src/io.c \
	src/item.c \
	src/player.c \
	src/playsub.c \
	src/ridechk.c \
	src/ring.c \
	src/score.c \
	src/scrchk.c \
	src/spring.c \
	src/suicide.c \
	src/zone.c \
	src/loader2.c \
	src/r3/banpa.c \
	src/r3/bobin.c \
	src/r3/bobinb.c \
	src/r3/coli3.c \
	src/r3/et3.c \
	src/r3/friend3.c \
	src/r3/ga3.c \
	src/r3/playsp3.c \
	src/r3/pocket.c \
	src/r3/trap_r3.c \
	src/dllmain.c \
	src/bmp.c \
	src/block.c \
	src/r3/miracle.c \
	src/tbl0.c \
	src/tbl1.c \
	src/r3/z31act.c \
	src/plchg.c \
	src/r3/tentou.c \
	src/r3/kama.c \
	src/plpat6.c \
	src/r3/act31b.c \
	src/r3/col3c.c \
	src/r3/dev31c.c \
	src/r3/edtbl31c.c \
	src/r3/z31ctbl.c \
	src/r3/movie3.c \
	src/r3/zonetbl3.c \
	src/r3/scr31c.c \
	src/save.c \
	src/r3/togebl3d.c \
	src/action.c

R31D_SOURCES := \
	src/actset.c \
	src/dircol.c \
	src/dummy.c \
	src/edit.c \
	src/equ.c \
	src/etc.c \
	src/fcol.c \
	src/game.c \
	src/goal.c \
	src/io.c \
	src/item.c \
	src/player.c \
	src/playsub.c \
	src/ridechk.c \
	src/ring.c \
	src/score.c \
	src/scrchk.c \
	src/spring.c \
	src/suicide.c \
	src/zone.c \
	src/loader2.c \
	src/r3/banpa.c \
	src/r3/bobin.c \
	src/r3/bobinb.c \
	src/r3/coli3.c \
	src/r3/et3.c \
	src/r3/friend3.c \
	src/r3/ga3.c \
	src/r3/playsp3.c \
	src/r3/pocket.c \
	src/r3/trap_r3.c \
	src/dllmain.c \
	src/bmp.c \
	src/block.c \
	src/r3/miracle.c \
	src/tbl0.c \
	src/tbl1.c \
	src/r3/z31act.c \
	src/plchg.c \
	src/r3/tentou.c \
	src/r3/kama.c \
	src/plpat6.c \
	src/r3/act31b.c \
	src/r3/col3d.c \
	src/r3/dev31d.c \
	src/r3/edtbl31d.c \
	src/r3/z31dtbl.c \
	src/r3/zonetbl3.c \
	src/r3/scr31d.c \
	src/r3/movie3.c \
	src/save.c \
	src/r3/togebl3d.c \
	src/action.c

R32A_SOURCES := \
	src/actset.c \
	src/dircol.c \
	src/dummy.c \
	src/edit.c \
	src/equ.c \
	src/etc.c \
	src/fcol.c \
	src/game.c \
	src/goal.c \
	src/io.c \
	src/item.c \
	src/player.c \
	src/playsub.c \
	src/ridechk.c \
	src/ring.c \
	src/score.c \
	src/scrchk.c \
	src/spring.c \
	src/suicide.c \
	src/zone.c \
	src/loader2.c \
	src/r3/banpa.c \
	src/r3/bobin.c \
	src/r3/bobinb.c \
	src/r3/coli3.c \
	src/r3/et3.c \
	src/r3/friend3.c \
	src/r3/ga3.c \
	src/r3/playsp3.c \
	src/r3/pocket.c \
	src/r3/togebl3a.c \
	src/r3/trap_r3.c \
	src/dllmain.c \
	src/bmp.c \
	src/block.c \
	src/r3/miracle.c \
	src/tbl0.c \
	src/tbl1.c \
	src/r3/col3a.c \
	src/plchg.c \
	src/r3/mecasnc.c \
	src/r3/msnccg.c \
	src/r3/tentou.c \
	src/r3/kama.c \
	src/plpat6.c \
	src/r3/dev32a.c \
	src/r3/edtbl32a.c \
	src/r3/z32act.c \
	src/r3/z32atbl.c \
	src/r3/zonetbl3.c \
	src/r3/scr32a.c \
	src/r3/act32a.c \
	src/r3/movie3.c \
	src/save.c \
	src/action.c

R32B_SOURCES := \
	src/actset.c \
	src/dircol.c \
	src/dummy.c \
	src/edit.c \
	src/equ.c \
	src/etc.c \
	src/fcol.c \
	src/game.c \
	src/goal.c \
	src/io.c \
	src/item.c \
	src/player.c \
	src/playsub.c \
	src/ridechk.c \
	src/ring.c \
	src/score.c \
	src/scrchk.c \
	src/spring.c \
	src/suicide.c \
	src/zone.c \
	src/loader2.c \
	src/r3/banpa.c \
	src/r3/bobin.c \
	src/r3/bobinb.c \
	src/r3/coli3.c \
	src/r3/et3.c \
	src/r3/friend3.c \
	src/r3/ga3.c \
	src/r3/playsp3.c \
	src/r3/pocket.c \
	src/r3/trap_r3.c \
	src/dllmain.c \
	src/bmp.c \
	src/block.c \
	src/r3/miracle.c \
	src/tbl0.c \
	src/tbl1.c \
	src/plchg.c \
	src/r3/tentou.c \
	src/r3/kama.c \
	src/plpat6.c \
	src/r3/act32a.c \
	src/r3/z32act.c \
	src/r3/col3b.c \
	src/r3/dev32b.c \
	src/r3/edtbl32b.c \
	src/r3/z32btbl.c \
	src/r3/zonetbl3.c \
	src/r3/scr32b.c \
	src/save.c \
	src/r3/movie3.c \
	src/r3/togebl3b.c \
	src/action.c

R32C_SOURCES := \
	src/actset.c \
	src/dircol.c \
	src/dummy.c \
	src/edit.c \
	src/equ.c \
	src/etc.c \
	src/fcol.c \
	src/game.c \
	src/goal.c \
	src/io.c \
	src/item.c \
	src/player.c \
	src/playsub.c \
	src/ridechk.c \
	src/ring.c \
	src/score.c \
	src/scrchk.c \
	src/spring.c \
	src/suicide.c \
	src/zone.c \
	src/loader2.c \
	src/r3/banpa.c \
	src/r3/bobin.c \
	src/r3/bobinb.c \
	src/r3/coli3.c \
	src/r3/et3.c \
	src/r3/friend3.c \
	src/r3/ga3.c \
	src/r3/playsp3.c \
	src/r3/pocket.c \
	src/r3/trap_r3.c \
	src/dllmain.c \
	src/bmp.c \
	src/block.c \
	src/r3/miracle.c \
	src/tbl0.c \
	src/tbl1.c \
	src/plchg.c \
	src/r3/tentou.c \
	src/r3/kama.c \
	src/plpat6.c \
	src/r3/act32a.c \
	src/r3/z32act.c \
	src/r3/col3c.c \
	src/r3/dev32c.c \
	src/r3/edtbl32c.c \
	src/r3/z32ctbl.c \
	src/r3/zonetbl3.c \
	src/r3/scr32c.c \
	src/save.c \
	src/r3/movie3.c \
	src/r3/togebl3d.c \
	src/action.c

R32D_SOURCES := \
	src/actset.c \
	src/dircol.c \
	src/dummy.c \
	src/edit.c \
	src/equ.c \
	src/etc.c \
	src/fcol.c \
	src/game.c \
	src/goal.c \
	src/io.c \
	src/item.c \
	src/player.c \
	src/playsub.c \
	src/ridechk.c \
	src/ring.c \
	src/score.c \
	src/scrchk.c \
	src/spring.c \
	src/suicide.c \
	src/zone.c \
	src/loader2.c \
	src/r3/banpa.c \
	src/r3/bobin.c \
	src/r3/bobinb.c \
	src/r3/coli3.c \
	src/r3/et3.c \
	src/r3/friend3.c \
	src/r3/ga3.c \
	src/r3/playsp3.c \
	src/r3/pocket.c \
	src/r3/trap_r3.c \
	src/dllmain.c \
	src/bmp.c \
	src/block.c \
	src/r3/miracle.c \
	src/tbl0.c \
	src/tbl1.c \
	src/plchg.c \
	src/r3/tentou.c \
	src/r3/kama.c \
	src/plpat6.c \
	src/r3/act32a.c \
	src/r3/z32act.c \
	src/r3/col3d.c \
	src/r3/dev32d.c \
	src/r3/edtbl32d.c \
	src/r3/z32dtbl.c \
	src/r3/zonetbl3.c \
	src/r3/scr32d.c \
	src/r3/movie3.c \
	src/save.c \
	src/r3/togebl3d.c \
	src/action.c

R33C_SOURCES := \
	src/actset.c \
	src/dircol.c \
	src/dummy.c \
	src/edit.c \
	src/equ.c \
	src/etc.c \
	src/fcol.c \
	src/game.c \
	src/goal.c \
	src/io.c \
	src/item.c \
	src/player.c \
	src/playsub.c \
	src/ridechk.c \
	src/ring.c \
	src/score.c \
	src/scrchk.c \
	src/spring.c \
	src/suicide.c \
	src/zone.c \
	src/loader2.c \
	src/r3/banpa.c \
	src/r3/bobin.c \
	src/r3/bobinb.c \
	src/r3/coli3.c \
	src/r3/et3.c \
	src/r3/friend3.c \
	src/r3/ga3.c \
	src/r3/playsp3.c \
	src/r3/pocket.c \
	src/r3/trap_r3.c \
	src/dllmain.c \
	src/bmp.c \
	src/block.c \
	src/r3/miracle.c \
	src/tbl0.c \
	src/tbl1.c \
	src/plchg.c \
	src/r3/tentou.c \
	src/r3/kama.c \
	src/plpat6.c \
	src/r3/act33c.c \
	src/r3/col3c.c \
	src/r3/dev33c.c \
	src/r3/edtbl33c.c \
	src/r3/z33ctbl.c \
	src/r3/z33act.c \
	src/r3/sikake33.c \
	src/r3/light.c \
	src/r3/stopper.c \
	src/r3/boss_3.c \
	src/r3/gate.c \
	src/r3/movie3.c \
	src/r3/rollplat.c \
	src/r3/scr33c.c \
	src/r3/zonetbl3.c \
	src/save.c \
	src/r3/togebl3d.c \
	src/action.c

R33D_SOURCES := \
	src/actset.c \
	src/dircol.c \
	src/dummy.c \
	src/edit.c \
	src/equ.c \
	src/etc.c \
	src/fcol.c \
	src/game.c \
	src/goal.c \
	src/io.c \
	src/item.c \
	src/player.c \
	src/playsub.c \
	src/ridechk.c \
	src/ring.c \
	src/score.c \
	src/scrchk.c \
	src/spring.c \
	src/suicide.c \
	src/zone.c \
	src/loader2.c \
	src/r3/banpa.c \
	src/r3/bobin.c \
	src/r3/bobinb.c \
	src/r3/coli3.c \
	src/r3/et3.c \
	src/r3/friend3.c \
	src/r3/ga3.c \
	src/r3/playsp3.c \
	src/r3/pocket.c \
	src/r3/trap_r3.c \
	src/dllmain.c \
	src/bmp.c \
	src/block.c \
	src/r3/miracle.c \
	src/tbl0.c \
	src/tbl1.c \
	src/plchg.c \
	src/r3/tentou.c \
	src/plpat6.c \
	src/r3/act33c.c \
	src/r3/z33act.c \
	src/r3/col3d.c \
	src/r3/dev33d.c \
	src/r3/kama.c \
	src/r3/z33dtbl.c \
	src/r3/zonetbl3.c \
	src/r3/scr33d.c \
	src/save.c \
	src/r3/togebl3d.c \
	src/r3/boss_3.c \
	src/r3/gate.c \
	src/r3/movie3.c \
	src/r3/sikake33.c \
	src/r3/stopper.c \
	src/r3/light.c \
	src/r3/rollplat.c \
	src/r3/edtbl33c.c \
	src/action.c

R41A_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/ridechk.c \
	src/player.c \
	src/playsp.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/r4/act41a.c \
	src/r4/col4a.c \
	src/r4/coli4.c \
	src/r4/dev41a.c \
	src/r4/edtbl41a.c \
	src/r4/et4.c \
	src/r4/scr41a.c \
	src/r4/scrchk4.c \
	src/r4/sw4.c \
	src/r4/swblk4.c \
	src/r4/z41act.c \
	src/r4/z41atbl.c \
	src/r4/zonetbl4.c \
	src/suicide.c \
	src/r4/game4.c \
	src/r4/playsub4.c \
	src/r4/awa.c \
	src/r4/harid4.c \
	src/r4/tobira4.c \
	src/r4/harir4.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/r4/walls.c \
	src/r4/wtbl41a.c \
	src/r4/funsui4.c \
	src/r4/wall1.c \
	src/r4/screw_a.c \
	src/r4/escal4.c \
	src/r4/brunko4.c \
	src/r4/tonbo.c \
	src/r4/amenbo.c \
	src/r4/tagameb4.c \
	src/r4/yago.c \
	src/r4/friend4.c \
	src/r4/tekkyu.c \
	src/r4/playpat4.c \
	src/r4/kuzure4.c

R41B_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/ridechk.c \
	src/player.c \
	src/playsp.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/r4/coli4.c \
	src/r4/et4.c \
	src/r4/sw4.c \
	src/r4/z41act.c \
	src/r4/zonetbl4.c \
	src/suicide.c \
	src/r4/game4.c \
	src/r4/playsub4.c \
	src/r4/awa.c \
	src/r4/harir4.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/r4/walls.c \
	src/r4/wall1.c \
	src/r4/escal4.c \
	src/r4/tonbo.c \
	src/r4/amenbo.c \
	src/r4/tagameb4.c \
	src/r4/yago.c \
	src/r4/friend4.c \
	src/r4/lrblk4.c \
	src/r4/col4b.c \
	src/r4/scr41b.c \
	src/scrchk.c \
	src/r4/act41b.c \
	src/r4/edtbl41b.c \
	src/r4/dev41b.c \
	src/r4/screw_a.c \
	src/r4/movie4.c \
	src/r4/rblk4.c \
	src/r4/z41btbl.c \
	src/r4/playpat4.c \
	src/r4/udblk4.c \
	src/r4/wtbl41b.c

R41C_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/ridechk.c \
	src/player.c \
	src/playsp.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/r4/coli4.c \
	src/r4/sw4.c \
	src/r4/z41act.c \
	src/r4/zonetbl4.c \
	src/suicide.c \
	src/r4/game4.c \
	src/r4/playsub4.c \
	src/r4/awa.c \
	src/r4/harid4.c \
	src/r4/harir4.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/r4/wall1.c \
	src/r4/escal4.c \
	src/scrchk.c \
	src/r4/act41c.c \
	src/r4/col4c.c \
	src/r4/dev41c.c \
	src/r4/edtbl41c.c \
	src/r4/screw_c.c \
	src/r4/tekkyu4.c \
	src/r4/wtbl41c.c \
	src/r4/renketu4.c \
	src/r4/amenbo.c \
	src/r4/friend4.c \
	src/r4/tagameb4.c \
	src/r4/tonbo.c \
	src/r4/yago.c \
	src/r4/scr41c.c \
	src/r4/lrblk4.c \
	src/r4/z41ctbl.c \
	src/r4/playpat4.c \
	src/r4/udblk4.c \
	src/r4/swgun4.c

R41D_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/ridechk.c \
	src/player.c \
	src/playsp.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/r4/coli4.c \
	src/r4/sw4.c \
	src/r4/z41act.c \
	src/r4/zonetbl4.c \
	src/suicide.c \
	src/r4/game4.c \
	src/r4/playsub4.c \
	src/r4/awa.c \
	src/r4/harid4.c \
	src/r4/harir4.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/r4/wall1.c \
	src/r4/escal4.c \
	src/scrchk.c \
	src/r4/act41c.c \
	src/r4/edtbl41c.c \
	src/r4/screw_c.c \
	src/r4/tekkyu4.c \
	src/r4/wtbl41c.c \
	src/r4/renketu4.c \
	src/r4/amenbo.c \
	src/r4/friend4.c \
	src/r4/tagameb4.c \
	src/r4/tonbo.c \
	src/r4/yago.c \
	src/r4/col4d.c \
	src/r4/scr41d.c \
	src/r4/dev41d.c \
	src/r4/z41dtbl.c \
	src/r4/playpat4.c \
	src/r4/udblk4.c \
	src/r4/swgun4.c

R42A_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/ridechk.c \
	src/player.c \
	src/playsp.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/r4/col4a.c \
	src/r4/coli4.c \
	src/r4/et4.c \
	src/r4/sw4.c \
	src/r4/swblk4.c \
	src/r4/zonetbl4.c \
	src/suicide.c \
	src/r4/game4.c \
	src/r4/playsub4.c \
	src/r4/awa.c \
	src/r4/tobira4.c \
	src/r4/harir4.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/r4/walls.c \
	src/r4/wall1.c \
	src/r4/screw_a.c \
	src/r4/escal4.c \
	src/r4/brunko4.c \
	src/r4/tonbo.c \
	src/r4/amenbo.c \
	src/r4/tagameb4.c \
	src/r4/yago.c \
	src/r4/friend4.c \
	src/r4/lrblk4.c \
	src/scrchk.c \
	src/r4/scr42a.c \
	src/r4/dev42a.c \
	src/r4/edtbl42a.c \
	src/r4/z42atbl.c \
	src/r4/wtbl42a.c \
	src/r4/tobiras4.c \
	src/r4/tekkyu.c \
	src/r4/tekkyu4.c \
	src/r4/osumizu.c \
	src/r4/wall4.c \
	src/r4/act42a.c \
	src/r4/ukidai.c \
	src/r4/playpat4.c \
	src/r4/z42act.c \
	src/r4/kuzure4.c \
	src/r4/tekkyu1.c \
	src/r4/wall42.c

R42B_SOURCES := \
	src/dircol.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/r4/col4b.c \
	src/etc.c \
	src/fcol.c \
	src/r4/game4.c \
	src/io.c \
	src/scrchk.c \
	src/r4/act42b.c \
	src/player.c \
	src/r4/playsub4.c \
	src/playsp.c \
	src/save.c \
	src/r4/coli4.c \
	src/edit.c \
	src/r4/edtbl42b.c \
	src/actset.c \
	src/r4/z42act.c \
	src/ridechk.c \
	src/spring.c \
	src/ring.c \
	src/item.c \
	src/score.c \
	src/r4/awa.c \
	src/goal.c \
	src/zone.c \
	src/r4/zonetbl4.c \
	src/r4/wtbl42b.c \
	src/r4/harir4.c \
	src/r4/sw4.c \
	src/r4/tonbo.c \
	src/r4/amenbo.c \
	src/r4/yago.c \
	src/r4/friend4.c \
	src/r4/walls.c \
	src/r4/wall1.c \
	src/r4/et4.c \
	src/suicide.c \
	src/r4/dev42b.c \
	src/r4/movie4.c \
	src/r4/z42btbl.c \
	src/r4/playpat4.c \
	src/r4/kuzure4.c \
	src/action.c \
	src/r4/rblk4.c \
	src/r4/udblk4.c \
	src/r4/scr42a.c \
	src/tbl0.c \
	src/tbl1.c \
	src/r4/screw_a.c \
	src/r4/tagameb4.c \
	src/r4/lrblk4.c \
	src/plchg.c

R42C_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/ridechk.c \
	src/player.c \
	src/playsp.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/r4/coli4.c \
	src/r4/sw4.c \
	src/r4/zonetbl4.c \
	src/suicide.c \
	src/r4/game4.c \
	src/r4/playsub4.c \
	src/r4/awa.c \
	src/r4/harir4.c \
	src/plchg.c \
	src/r4/walls.c \
	src/r4/wall1.c \
	src/r4/tonbo.c \
	src/r4/amenbo.c \
	src/r4/tagameb4.c \
	src/r4/yago.c \
	src/r4/friend4.c \
	src/r4/lrblk4.c \
	src/r4/tekkyu.c \
	src/r4/act42c.c \
	src/r4/col4c.c \
	src/r4/dev42c.c \
	src/r4/edtbl42c.c \
	src/r4/screw_c.c \
	src/r4/wtbl42c.c \
	src/r4/z42act.c \
	src/r4/z42ctbl.c \
	src/r4/playpat4.c \
	src/r4/kuzure4.c \
	src/r4/baneiwa.c \
	src/r4/daid4.c \
	src/r4/tekkyu1.c \
	src/action.c \
	src/r4/scr42c.c \
	src/r4/tobiras4.c \
	src/r4/tobira4.c \
	src/r4/tekkyu4.c \
	src/tbl0.c \
	src/tbl1.c \
	src/r4/kowasi4.c \
	src/r4/renketu4.c \
	src/r4/udblk4.c \
	src/scrchk.c

R42D_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/ridechk.c \
	src/player.c \
	src/playsp.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/r4/coli4.c \
	src/r4/sw4.c \
	src/r4/zonetbl4.c \
	src/suicide.c \
	src/r4/game4.c \
	src/r4/playsub4.c \
	src/r4/awa.c \
	src/r4/tobira4.c \
	src/r4/harir4.c \
	src/r4/walls.c \
	src/r4/wall1.c \
	src/r4/tonbo.c \
	src/r4/amenbo.c \
	src/r4/tagameb4.c \
	src/r4/yago.c \
	src/r4/friend4.c \
	src/r4/lrblk4.c \
	src/r4/tekkyu.c \
	src/r4/col4d.c \
	src/r4/dev42d.c \
	src/r4/tekkyu4.c \
	src/r4/tobiras4.c \
	src/r4/z42dtbl.c \
	src/r4/playpat4.c \
	src/action.c \
	src/plchg.c \
	src/tbl0.c \
	src/tbl1.c \
	src/r4/act42c.c \
	src/r4/edtbl42c.c \
	src/r4/scr42c.c \
	src/r4/baneiwa.c \
	src/r4/kuzure4.c \
	src/r4/kowasi4.c \
	src/r4/daid4.c \
	src/r4/screw_c.c \
	src/r4/tekkyu1.c \
	src/r4/z42act.c \
	src/r4/wtbl42c.c \
	src/scrchk.c \
	src/r4/renketu4.c \
	src/r4/udblk4.c

R43C_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/ridechk.c \
	src/player.c \
	src/playsp.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/r4/coli4.c \
	src/r4/sw4.c \
	src/r4/swblk4.c \
	src/r4/zonetbl4.c \
	src/suicide.c \
	src/r4/game4.c \
	src/r4/playsub4.c \
	src/r4/awa.c \
	src/r4/walls.c \
	src/r4/wall1.c \
	src/r4/screw_a.c \
	src/r4/tonbo.c \
	src/r4/friend4.c \
	src/r4/tekkyu.c \
	src/r4/act43c.c \
	src/r4/col4c.c \
	src/r4/dev43c.c \
	src/r4/edtbl43c.c \
	src/r4/scr43c.c \
	src/scrchk.c \
	src/r4/z43act.c \
	src/r4/wall4.c \
	src/r4/boss_4.c \
	src/r4/boss_4_2.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/plchg.c \
	src/r4/renketu4.c \
	src/r4/z43ctbl.c \
	src/r4/wtbl43.c \
	src/r4/playpat4.c

R43D_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/ridechk.c \
	src/player.c \
	src/playsp.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/r4/coli4.c \
	src/r4/sw4.c \
	src/r4/swblk4.c \
	src/r4/zonetbl4.c \
	src/suicide.c \
	src/r4/game4.c \
	src/r4/playsub4.c \
	src/r4/awa.c \
	src/r4/walls.c \
	src/r4/wall1.c \
	src/r4/screw_a.c \
	src/r4/tonbo.c \
	src/r4/friend4.c \
	src/r4/tekkyu.c \
	src/r4/act43c.c \
	src/r4/edtbl43c.c \
	src/scrchk.c \
	src/r4/z43act.c \
	src/r4/wall4.c \
	src/r4/boss_4.c \
	src/r4/boss_4_2.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/plchg.c \
	src/r4/renketu4.c \
	src/r4/wtbl43.c \
	src/r4/playpat4.c \
	src/r4/col4d.c \
	src/r4/dev43d.c \
	src/r4/scr43d.c \
	src/r4/z43dtbl.c

R51A_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/ridechk.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/r5/act51a.c \
	src/r5/col5a.c \
	src/r5/coli5.c \
	src/r5/dev51a.c \
	src/r5/edtbl5.c \
	src/r5/game5.c \
	src/r5/playsp5.c \
	src/r5/scr51a.c \
	src/r5/z51act.c \
	src/r5/z51atbl.c \
	src/playsub.c \
	src/plpat6.c \
	src/scrchk.c \
	src/r5/zonetbl5.c \
	src/r5/beltsw5.c \
	src/r5/movie5.c \
	src/r5/friend5.c \
	src/r5/kemusi.c \
	src/r5/shoot5.c \
	src/r5/kumo.c \
	src/r5/iwa5.c \
	src/r5/iwa5wave.c \
	src/r5/denden.c \
	src/r5/hashi5.c \
	src/r5/hari5f.c \
	src/r5/harir5.c \
	src/r5/iwa5roll.c \
	src/r5/kowasi5.c \
	src/r5/kuzure5.c \
	src/r5/sasori.c \
	src/r5/hasira5.c \
	src/r5/et5.c \
	src/r5/buranko5.c \
	src/r5/dai_rd5.c

R51B_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/ridechk.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/r5/coli5.c \
	src/r5/edtbl5.c \
	src/r5/game5.c \
	src/r5/playsp5.c \
	src/r5/z51act.c \
	src/playsub.c \
	src/plpat6.c \
	src/scrchk.c \
	src/r5/zonetbl5.c \
	src/r5/beltsw5.c \
	src/r5/movie5.c \
	src/r5/friend5.c \
	src/r5/kemusi.c \
	src/r5/shoot5.c \
	src/r5/kumo.c \
	src/r5/iwa5.c \
	src/r5/iwa5wave.c \
	src/r5/denden.c \
	src/r5/hashi5.c \
	src/r5/act51a.c \
	src/r5/col5b.c \
	src/r5/dev51b.c \
	src/r5/scr51b.c \
	src/r5/z51btbl.c \
	src/r5/hari5f.c \
	src/r5/harir5.c \
	src/r5/kuzure5.c \
	src/r5/kowasi5.c \
	src/r5/hasira5.c \
	src/r5/sasori.c \
	src/r5/dai_rd5.c \
	src/r5/iwa5roll.c \
	src/r5/buranko5.c \
	src/r5/et5.c

R51C_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/ridechk.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/r5/act51a.c \
	src/r5/coli5.c \
	src/r5/edtbl5.c \
	src/r5/game5.c \
	src/r5/playsp5.c \
	src/r5/z51act.c \
	src/playsub.c \
	src/plpat6.c \
	src/scrchk.c \
	src/r5/zonetbl5.c \
	src/r5/beltsw5.c \
	src/r5/movie5.c \
	src/r5/friend5.c \
	src/r5/kemusi.c \
	src/r5/shoot5.c \
	src/r5/kumo.c \
	src/r5/iwa5.c \
	src/r5/iwa5wave.c \
	src/r5/denden.c \
	src/r5/hashi5.c \
	src/r5/col5c.c \
	src/r5/dev51c.c \
	src/r5/scr51c.c \
	src/r5/z51ctbl.c \
	src/r5/hari5f.c \
	src/r5/harir5.c \
	src/r5/kowasi5.c \
	src/r5/kuzure5.c \
	src/r5/hasira5.c \
	src/r5/sasori.c \
	src/r5/iwa5roll.c \
	src/r5/buranko5.c \
	src/r5/dai_rd5.c \
	src/r5/et5.c

R51D_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/ridechk.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/r5/act51a.c \
	src/r5/coli5.c \
	src/r5/edtbl5.c \
	src/r5/game5.c \
	src/r5/playsp5.c \
	src/r5/z51act.c \
	src/playsub.c \
	src/plpat6.c \
	src/scrchk.c \
	src/r5/zonetbl5.c \
	src/r5/beltsw5.c \
	src/r5/movie5.c \
	src/r5/friend5.c \
	src/r5/kemusi.c \
	src/r5/shoot5.c \
	src/r5/kumo.c \
	src/r5/iwa5.c \
	src/r5/iwa5wave.c \
	src/r5/denden.c \
	src/r5/hashi5.c \
	src/r5/col5d.c \
	src/r5/dev51d.c \
	src/r5/scr51d.c \
	src/r5/z51dtbl.c \
	src/r5/hari5f.c \
	src/r5/harir5.c \
	src/r5/kowasi5.c \
	src/r5/kuzure5.c \
	src/r5/hasira5.c \
	src/r5/sasori.c \
	src/r5/iwa5roll.c \
	src/r5/buranko5.c \
	src/r5/dai_rd5.c \
	src/r5/et5.c

R52A_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/ridechk.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/r5/act51a.c \
	src/r5/col5a.c \
	src/r5/coli5.c \
	src/r5/edtbl5.c \
	src/r5/game5.c \
	src/r5/playsp5.c \
	src/playsub.c \
	src/plpat6.c \
	src/scrchk.c \
	src/r5/zonetbl5.c \
	src/r5/beltsw5.c \
	src/r5/movie5.c \
	src/r5/friend5.c \
	src/r5/kemusi.c \
	src/r5/shoot5.c \
	src/r5/kumo.c \
	src/r5/iwa5.c \
	src/r5/iwa5wave.c \
	src/r5/denden.c \
	src/r5/hashi5.c \
	src/r5/hari5f.c \
	src/r5/harir5.c \
	src/r5/iwa5roll.c \
	src/r5/kowasi5.c \
	src/r5/kuzure5.c \
	src/r5/sasori.c \
	src/r5/hasira5.c \
	src/r5/et5.c \
	src/r5/buranko5.c \
	src/r5/dai_rd5.c \
	src/r5/dev52a.c \
	src/r5/scr52a.c \
	src/r5/z52act.c \
	src/r5/z52atbl.c

R52B_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/ridechk.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/r5/coli5.c \
	src/r5/edtbl5.c \
	src/r5/game5.c \
	src/r5/playsp5.c \
	src/playsub.c \
	src/plpat6.c \
	src/scrchk.c \
	src/r5/zonetbl5.c \
	src/r5/beltsw5.c \
	src/r5/movie5.c \
	src/r5/friend5.c \
	src/r5/kemusi.c \
	src/r5/shoot5.c \
	src/r5/kumo.c \
	src/r5/iwa5.c \
	src/r5/iwa5wave.c \
	src/r5/denden.c \
	src/r5/hashi5.c \
	src/r5/act51a.c \
	src/r5/col5b.c \
	src/r5/hari5f.c \
	src/r5/harir5.c \
	src/r5/kuzure5.c \
	src/r5/kowasi5.c \
	src/r5/hasira5.c \
	src/r5/sasori.c \
	src/r5/dai_rd5.c \
	src/r5/iwa5roll.c \
	src/r5/buranko5.c \
	src/r5/et5.c \
	src/r5/dev52b.c \
	src/r5/scr52b.c \
	src/r5/z52act.c \
	src/r5/z52btbl.c

R52C_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/ridechk.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/r5/act51a.c \
	src/r5/coli5.c \
	src/r5/edtbl5.c \
	src/r5/game5.c \
	src/r5/playsp5.c \
	src/playsub.c \
	src/plpat6.c \
	src/scrchk.c \
	src/r5/zonetbl5.c \
	src/r5/beltsw5.c \
	src/r5/movie5.c \
	src/r5/friend5.c \
	src/r5/kemusi.c \
	src/r5/shoot5.c \
	src/r5/kumo.c \
	src/r5/iwa5.c \
	src/r5/iwa5wave.c \
	src/r5/denden.c \
	src/r5/hashi5.c \
	src/r5/col5c.c \
	src/r5/hari5f.c \
	src/r5/harir5.c \
	src/r5/kowasi5.c \
	src/r5/kuzure5.c \
	src/r5/hasira5.c \
	src/r5/sasori.c \
	src/r5/iwa5roll.c \
	src/r5/buranko5.c \
	src/r5/dai_rd5.c \
	src/r5/et5.c \
	src/r5/dev52c.c \
	src/r5/scr52c.c \
	src/r5/z52act.c \
	src/r5/z52ctbl.c

R52D_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/ridechk.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/r5/act51a.c \
	src/r5/coli5.c \
	src/r5/edtbl5.c \
	src/r5/game5.c \
	src/r5/playsp5.c \
	src/playsub.c \
	src/plpat6.c \
	src/scrchk.c \
	src/r5/zonetbl5.c \
	src/r5/beltsw5.c \
	src/r5/movie5.c \
	src/r5/friend5.c \
	src/r5/kemusi.c \
	src/r5/shoot5.c \
	src/r5/kumo.c \
	src/r5/iwa5.c \
	src/r5/iwa5wave.c \
	src/r5/denden.c \
	src/r5/hashi5.c \
	src/r5/col5d.c \
	src/r5/hari5f.c \
	src/r5/harir5.c \
	src/r5/kowasi5.c \
	src/r5/kuzure5.c \
	src/r5/hasira5.c \
	src/r5/sasori.c \
	src/r5/iwa5roll.c \
	src/r5/buranko5.c \
	src/r5/dai_rd5.c \
	src/r5/et5.c \
	src/r5/dev52d.c \
	src/r5/scr52d.c \
	src/r5/z52act.c \
	src/r5/z52dtbl.c

R53C_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/ridechk.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/r5/coli5.c \
	src/r5/playsp5.c \
	src/playsub.c \
	src/plpat6.c \
	src/scrchk.c \
	src/r5/zonetbl5.c \
	src/r5/act53.c \
	src/r5/col5c.c \
	src/r5/dev53c.c \
	src/r5/scr53c.c \
	src/r5/z53act.c \
	src/r5/z53ctbl.c \
	src/r5/kuzure5.c \
	src/r5/shoot5.c \
	src/game.c \
	src/r5/dai_rd5.c \
	src/r5/boss_5.c \
	src/r5/edtbl53.c

R53D_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/ridechk.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/r5/coli5.c \
	src/r5/playsp5.c \
	src/playsub.c \
	src/plpat6.c \
	src/scrchk.c \
	src/r5/zonetbl5.c \
	src/r5/act53.c \
	src/r5/z53act.c \
	src/r5/kuzure5.c \
	src/r5/shoot5.c \
	src/game.c \
	src/r5/dai_rd5.c \
	src/r5/col5d.c \
	src/r5/dev53d.c \
	src/r5/scr53d.c \
	src/r5/z53dtbl.c \
	src/r5/boss_5.c \
	src/r5/edtbl53.c

R61A_SOURCES := \
	src/action.c \
	src/actset.c \
	src/dircol.c \
	src/dummy.c \
	src/edit.c \
	src/equ.c \
	src/etc.c \
	src/fcol.c \
	src/game.c \
	src/goal.c \
	src/io.c \
	src/item.c \
	src/playsub.c \
	src/ridechk.c \
	src/ring.c \
	src/score.c \
	src/scrchk.c \
	src/spring.c \
	src/suicide.c \
	src/zone.c \
	src/loader2.c \
	src/dllmain.c \
	src/bmp.c \
	src/block.c \
	src/tbl0.c \
	src/tbl1.c \
	src/plpat6.c \
	src/save.c \
	src/r6/act61a.c \
	src/r6/coli6.c \
	src/r6/edtbl61a.c \
	src/r6/sw6.c \
	src/r6/tobira6.c \
	src/r6/trap_r6.c \
	src/r6/kdai6.c \
	src/r6/beem6.c \
	src/r6/block6.c \
	src/r6/col6a.c \
	src/r6/dair6.c \
	src/r6/egg6.c \
	src/r6/et6.c \
	src/r6/friend6.c \
	src/r6/hachi6.c \
	src/r6/minomusi.c \
	src/r6/movie6.c \
	src/r6/piston6.c \
	src/r6/scr61a.c \
	src/r6/semi.c \
	src/r6/shoot6.c \
	src/r6/tobidai6.c \
	src/r6/udblk6.c \
	src/r6/z61act.c \
	src/r6/z61atbl.c \
	src/r6/dev61a.c \
	src/r6/batta.c \
	src/r6/seesaw6.c \
	src/playsp.c \
	src/player6.c \
	src/plchg6.c \
	src/r6/zonetbl6.c

R61B_SOURCES := \
	src/action.c \
	src/actset.c \
	src/dircol.c \
	src/dummy.c \
	src/edit.c \
	src/equ.c \
	src/etc.c \
	src/fcol.c \
	src/game.c \
	src/goal.c \
	src/io.c \
	src/item.c \
	src/playsub.c \
	src/ridechk.c \
	src/ring.c \
	src/score.c \
	src/scrchk.c \
	src/spring.c \
	src/suicide.c \
	src/zone.c \
	src/loader2.c \
	src/dllmain.c \
	src/bmp.c \
	src/block.c \
	src/tbl0.c \
	src/tbl1.c \
	src/plpat6.c \
	src/save.c \
	src/r6/coli6.c \
	src/r6/sw6.c \
	src/r6/tobira6.c \
	src/r6/trap_r6.c \
	src/r6/kdai6.c \
	src/r6/beem6.c \
	src/r6/block6.c \
	src/r6/dair6.c \
	src/r6/egg6.c \
	src/r6/et6.c \
	src/r6/friend6.c \
	src/r6/hachi6.c \
	src/r6/minomusi.c \
	src/r6/movie6.c \
	src/r6/piston6.c \
	src/r6/semi.c \
	src/r6/shoot6.c \
	src/r6/tobidai6.c \
	src/r6/udblk6.c \
	src/r6/batta.c \
	src/r6/seesaw6.c \
	src/playsp.c \
	src/player6.c \
	src/r6/col6b.c \
	src/r6/scr61b.c \
	src/r6/dev61b.c \
	src/r6/act61a.c \
	src/r6/edtbl61b.c \
	src/r6/z61btbl.c \
	src/r6/z61act.c \
	src/plchg6.c \
	src/r6/zonetbl6.c

R61C_SOURCES := \
	src/action.c \
	src/actset.c \
	src/dircol.c \
	src/dummy.c \
	src/edit.c \
	src/equ.c \
	src/etc.c \
	src/fcol.c \
	src/game.c \
	src/goal.c \
	src/io.c \
	src/item.c \
	src/playsub.c \
	src/ridechk.c \
	src/ring.c \
	src/score.c \
	src/scrchk.c \
	src/spring.c \
	src/suicide.c \
	src/zone.c \
	src/loader2.c \
	src/dllmain.c \
	src/bmp.c \
	src/block.c \
	src/tbl0.c \
	src/tbl1.c \
	src/plpat6.c \
	src/save.c \
	src/r6/act61a.c \
	src/r6/coli6.c \
	src/r6/sw6.c \
	src/r6/tobira6.c \
	src/r6/trap_r6.c \
	src/r6/kdai6.c \
	src/r6/beem6.c \
	src/r6/block6.c \
	src/r6/dair6.c \
	src/r6/egg6.c \
	src/r6/et6.c \
	src/r6/friend6.c \
	src/r6/hachi6.c \
	src/r6/minomusi.c \
	src/r6/movie6.c \
	src/r6/piston6.c \
	src/r6/semi.c \
	src/r6/shoot6.c \
	src/r6/tobidai6.c \
	src/r6/udblk6.c \
	src/r6/z61act.c \
	src/r6/batta.c \
	src/r6/seesaw6.c \
	src/playsp.c \
	src/player6.c \
	src/r6/col6c.c \
	src/r6/dev61c.c \
	src/r6/scr61c.c \
	src/r6/z61ctbl.c \
	src/plchg6.c \
	src/r6/edtbl61c.c \
	src/r6/zonetbl6.c

R61D_SOURCES := \
	src/action.c \
	src/actset.c \
	src/dircol.c \
	src/dummy.c \
	src/edit.c \
	src/equ.c \
	src/etc.c \
	src/fcol.c \
	src/game.c \
	src/goal.c \
	src/io.c \
	src/item.c \
	src/playsub.c \
	src/ridechk.c \
	src/ring.c \
	src/score.c \
	src/scrchk.c \
	src/spring.c \
	src/suicide.c \
	src/zone.c \
	src/loader2.c \
	src/dllmain.c \
	src/bmp.c \
	src/block.c \
	src/tbl0.c \
	src/tbl1.c \
	src/plpat6.c \
	src/save.c \
	src/r6/act61a.c \
	src/r6/coli6.c \
	src/r6/sw6.c \
	src/r6/tobira6.c \
	src/r6/trap_r6.c \
	src/r6/kdai6.c \
	src/r6/beem6.c \
	src/r6/block6.c \
	src/r6/dair6.c \
	src/r6/egg6.c \
	src/r6/et6.c \
	src/r6/friend6.c \
	src/r6/hachi6.c \
	src/r6/minomusi.c \
	src/r6/movie6.c \
	src/r6/piston6.c \
	src/r6/semi.c \
	src/r6/shoot6.c \
	src/r6/tobidai6.c \
	src/r6/udblk6.c \
	src/r6/z61act.c \
	src/r6/batta.c \
	src/r6/seesaw6.c \
	src/playsp.c \
	src/player6.c \
	src/r6/col6d.c \
	src/r6/dev61d.c \
	src/r6/scr61d.c \
	src/r6/z61dtbl.c \
	src/plchg6.c \
	src/r6/edtbl61c.c \
	src/r6/zonetbl6.c

R62A_SOURCES := \
	src/action.c \
	src/actset.c \
	src/dircol.c \
	src/dummy.c \
	src/edit.c \
	src/equ.c \
	src/etc.c \
	src/fcol.c \
	src/game.c \
	src/goal.c \
	src/io.c \
	src/item.c \
	src/playsub.c \
	src/ridechk.c \
	src/ring.c \
	src/score.c \
	src/scrchk.c \
	src/spring.c \
	src/suicide.c \
	src/zone.c \
	src/loader2.c \
	src/dllmain.c \
	src/bmp.c \
	src/block.c \
	src/tbl0.c \
	src/tbl1.c \
	src/plpat6.c \
	src/save.c \
	src/r6/coli6.c \
	src/r6/sw6.c \
	src/r6/tobira6.c \
	src/r6/trap_r6.c \
	src/r6/kdai6.c \
	src/r6/beem6.c \
	src/r6/block6.c \
	src/r6/dair6.c \
	src/r6/egg6.c \
	src/r6/et6.c \
	src/r6/friend6.c \
	src/r6/hachi6.c \
	src/r6/minomusi.c \
	src/r6/movie6.c \
	src/r6/piston6.c \
	src/r6/semi.c \
	src/r6/shoot6.c \
	src/r6/tobidai6.c \
	src/r6/udblk6.c \
	src/r6/batta.c \
	src/r6/seesaw6.c \
	src/playsp.c \
	src/player6.c \
	src/r6/edtbl62a.c \
	src/r6/z62act.c \
	src/r6/z62atbl.c \
	src/r6/col6a.c \
	src/r6/togebl6a.c \
	src/r6/dev62a.c \
	src/r6/scr62a.c \
	src/plchg6.c \
	src/r6/act62a.c \
	src/r6/zonetbl6.c

R62B_SOURCES := \
	src/action.c \
	src/actset.c \
	src/dircol.c \
	src/dummy.c \
	src/edit.c \
	src/equ.c \
	src/etc.c \
	src/fcol.c \
	src/game.c \
	src/goal.c \
	src/io.c \
	src/item.c \
	src/playsub.c \
	src/ridechk.c \
	src/ring.c \
	src/score.c \
	src/scrchk.c \
	src/spring.c \
	src/suicide.c \
	src/zone.c \
	src/loader2.c \
	src/dllmain.c \
	src/bmp.c \
	src/block.c \
	src/tbl0.c \
	src/tbl1.c \
	src/plpat6.c \
	src/save.c \
	src/r6/coli6.c \
	src/r6/sw6.c \
	src/r6/tobira6.c \
	src/r6/trap_r6.c \
	src/r6/kdai6.c \
	src/r6/beem6.c \
	src/r6/block6.c \
	src/r6/dair6.c \
	src/r6/egg6.c \
	src/r6/et6.c \
	src/r6/friend6.c \
	src/r6/hachi6.c \
	src/r6/minomusi.c \
	src/r6/movie6.c \
	src/r6/piston6.c \
	src/r6/semi.c \
	src/r6/shoot6.c \
	src/r6/tobidai6.c \
	src/r6/udblk6.c \
	src/r6/batta.c \
	src/r6/seesaw6.c \
	src/playsp.c \
	src/player6.c \
	src/r6/act62a.c \
	src/r6/col6b.c \
	src/r6/dev62b.c \
	src/r6/edtbl62b.c \
	src/r6/scr62b.c \
	src/r6/z62btbl.c \
	src/r6/z62act.c \
	src/r6/togebl6b.c \
	src/plchg6.c \
	src/r6/zonetbl6.c

R62C_SOURCES := \
	src/action.c \
	src/actset.c \
	src/dircol.c \
	src/dummy.c \
	src/edit.c \
	src/equ.c \
	src/etc.c \
	src/fcol.c \
	src/game.c \
	src/goal.c \
	src/io.c \
	src/item.c \
	src/playsub.c \
	src/ridechk.c \
	src/ring.c \
	src/score.c \
	src/scrchk.c \
	src/spring.c \
	src/suicide.c \
	src/zone.c \
	src/loader2.c \
	src/dllmain.c \
	src/bmp.c \
	src/block.c \
	src/tbl0.c \
	src/tbl1.c \
	src/plpat6.c \
	src/save.c \
	src/r6/coli6.c \
	src/r6/sw6.c \
	src/r6/tobira6.c \
	src/r6/trap_r6.c \
	src/r6/kdai6.c \
	src/r6/beem6.c \
	src/r6/block6.c \
	src/r6/dair6.c \
	src/r6/egg6.c \
	src/r6/et6.c \
	src/r6/friend6.c \
	src/r6/hachi6.c \
	src/r6/minomusi.c \
	src/r6/movie6.c \
	src/r6/piston6.c \
	src/r6/semi.c \
	src/r6/shoot6.c \
	src/r6/tobidai6.c \
	src/r6/udblk6.c \
	src/r6/batta.c \
	src/r6/seesaw6.c \
	src/playsp.c \
	src/player6.c \
	src/r6/act62a.c \
	src/r6/col6c.c \
	src/r6/dev62c.c \
	src/r6/edtbl62c.c \
	src/r6/z62act.c \
	src/r6/z62ctbl.c \
	src/r6/togebl6d.c \
	src/r6/scr62c.c \
	src/plchg6.c \
	src/r6/zonetbl6.c

R62D_SOURCES := \
	src/action.c \
	src/actset.c \
	src/dircol.c \
	src/dummy.c \
	src/edit.c \
	src/equ.c \
	src/etc.c \
	src/fcol.c \
	src/game.c \
	src/goal.c \
	src/io.c \
	src/item.c \
	src/playsub.c \
	src/ridechk.c \
	src/ring.c \
	src/score.c \
	src/scrchk.c \
	src/spring.c \
	src/suicide.c \
	src/zone.c \
	src/loader2.c \
	src/dllmain.c \
	src/bmp.c \
	src/block.c \
	src/tbl0.c \
	src/tbl1.c \
	src/plpat6.c \
	src/save.c \
	src/r6/coli6.c \
	src/r6/sw6.c \
	src/r6/tobira6.c \
	src/r6/trap_r6.c \
	src/r6/kdai6.c \
	src/r6/beem6.c \
	src/r6/block6.c \
	src/r6/dair6.c \
	src/r6/egg6.c \
	src/r6/et6.c \
	src/r6/friend6.c \
	src/r6/hachi6.c \
	src/r6/minomusi.c \
	src/r6/movie6.c \
	src/r6/piston6.c \
	src/r6/semi.c \
	src/r6/shoot6.c \
	src/r6/tobidai6.c \
	src/r6/udblk6.c \
	src/r6/batta.c \
	src/r6/seesaw6.c \
	src/playsp.c \
	src/player6.c \
	src/r6/col6d.c \
	src/r6/act62a.c \
	src/r6/dev62d.c \
	src/r6/edtbl62d.c \
	src/r6/scr62d.c \
	src/r6/togebl6d.c \
	src/r6/z62act.c \
	src/r6/z62dtbl.c \
	src/plchg6.c \
	src/r6/zonetbl6.c

R63C_SOURCES := \
	src/action.c \
	src/actset.c \
	src/dircol.c \
	src/dummy.c \
	src/edit.c \
	src/equ.c \
	src/etc.c \
	src/fcol.c \
	src/game.c \
	src/goal.c \
	src/io.c \
	src/item.c \
	src/playsub.c \
	src/ridechk.c \
	src/ring.c \
	src/score.c \
	src/scrchk.c \
	src/spring.c \
	src/suicide.c \
	src/zone.c \
	src/loader2.c \
	src/dllmain.c \
	src/bmp.c \
	src/block.c \
	src/tbl0.c \
	src/tbl1.c \
	src/plpat6.c \
	src/save.c \
	src/r6/coli6.c \
	src/r6/sw6.c \
	src/r6/tobira6.c \
	src/r6/trap_r6.c \
	src/r6/kdai6.c \
	src/r6/beem6.c \
	src/r6/block6.c \
	src/r6/dair6.c \
	src/r6/egg6.c \
	src/r6/et6.c \
	src/r6/friend6.c \
	src/r6/movie6.c \
	src/r6/piston6.c \
	src/r6/shoot6.c \
	src/r6/tobidai6.c \
	src/r6/udblk6.c \
	src/r6/seesaw6.c \
	src/playsp.c \
	src/player6.c \
	src/r6/col6c.c \
	src/r6/togebl6d.c \
	src/plchg6.c \
	src/r6/act63.c \
	src/r6/dev63c.c \
	src/r6/edtbl63c.c \
	src/r6/scr63c.c \
	src/r6/z63ctbl.c \
	src/r6/z63act.c \
	src/r6/boss_6.c \
	src/r6/zonetbl6.c

R63D_SOURCES := \
	src/action.c \
	src/actset.c \
	src/dircol.c \
	src/dummy.c \
	src/edit.c \
	src/equ.c \
	src/etc.c \
	src/fcol.c \
	src/game.c \
	src/goal.c \
	src/io.c \
	src/item.c \
	src/playsub.c \
	src/ridechk.c \
	src/ring.c \
	src/score.c \
	src/scrchk.c \
	src/spring.c \
	src/suicide.c \
	src/zone.c \
	src/loader2.c \
	src/dllmain.c \
	src/bmp.c \
	src/block.c \
	src/tbl0.c \
	src/tbl1.c \
	src/plpat6.c \
	src/save.c \
	src/r6/coli6.c \
	src/r6/sw6.c \
	src/r6/tobira6.c \
	src/r6/trap_r6.c \
	src/r6/kdai6.c \
	src/r6/beem6.c \
	src/r6/block6.c \
	src/r6/dair6.c \
	src/r6/egg6.c \
	src/r6/et6.c \
	src/r6/friend6.c \
	src/r6/movie6.c \
	src/r6/piston6.c \
	src/r6/shoot6.c \
	src/r6/tobidai6.c \
	src/r6/udblk6.c \
	src/r6/seesaw6.c \
	src/playsp.c \
	src/player6.c \
	src/r6/togebl6d.c \
	src/plchg6.c \
	src/r6/act63.c \
	src/r6/z63act.c \
	src/r6/boss_6.c \
	src/r6/col6d.c \
	src/r6/dev63d.c \
	src/r6/scr63d.c \
	src/r6/edtbl63c.c \
	src/r6/z63dtbl.c \
	src/r6/zonetbl6.c

R71A_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/playsub.c \
	src/plpat6.c \
	src/scrchk.c \
	src/game.c \
	src/r7/act71a.c \
	src/r7/chgban.c \
	src/r7/chgwall7.c \
	src/r7/col7a.c \
	src/r7/coli7.c \
	src/r7/dev71a.c \
	src/r7/edtbl7.c \
	src/r7/playsp7.c \
	src/r7/scr71a.c \
	src/r7/z71act.c \
	src/r7/z71atbl.c \
	src/r7/zonetbl7.c \
	src/ridechk.c \
	src/r7/wall7.c \
	src/r7/tekkyu7.c \
	src/r7/tekkyu7j.c \
	src/r7/slight7.c \
	src/r7/friend7.c \
	src/r7/branko7.c \
	src/r7/et7.c \
	src/r7/kanabun.c \
	src/r7/dango7.c \
	src/r7/movie7.c \
	src/r7/kabasira.c \
	src/r7/hotaru7.c

R71B_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/playsub.c \
	src/plpat6.c \
	src/scrchk.c \
	src/game.c \
	src/r7/act71a.c \
	src/r7/chgban.c \
	src/r7/chgwall7.c \
	src/r7/coli7.c \
	src/r7/edtbl7.c \
	src/r7/playsp7.c \
	src/r7/z71act.c \
	src/r7/zonetbl7.c \
	src/ridechk.c \
	src/r7/wall7.c \
	src/r7/tekkyu7.c \
	src/r7/tekkyu7j.c \
	src/r7/slight7.c \
	src/r7/friend7.c \
	src/r7/branko7.c \
	src/r7/et7.c \
	src/r7/kanabun.c \
	src/r7/dango7.c \
	src/r7/movie7.c \
	src/r7/col7b.c \
	src/r7/scr71b.c \
	src/r7/dev71b.c \
	src/r7/z71btbl.c \
	src/r7/hotaru7.c \
	src/r7/kabasira.c

R71C_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/playsub.c \
	src/plpat6.c \
	src/scrchk.c \
	src/game.c \
	src/r7/act71a.c \
	src/r7/chgban.c \
	src/r7/chgwall7.c \
	src/r7/coli7.c \
	src/r7/edtbl7.c \
	src/r7/playsp7.c \
	src/r7/z71act.c \
	src/r7/zonetbl7.c \
	src/ridechk.c \
	src/r7/wall7.c \
	src/r7/tekkyu7.c \
	src/r7/tekkyu7j.c \
	src/r7/slight7.c \
	src/r7/friend7.c \
	src/r7/branko7.c \
	src/r7/et7.c \
	src/r7/kanabun.c \
	src/r7/dango7.c \
	src/r7/movie7.c \
	src/r7/col7c.c \
	src/r7/z71ctbl.c \
	src/r7/scr71c.c \
	src/r7/dev71c.c \
	src/r7/kabasira.c \
	src/r7/hotaru7.c

R71D_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/playsub.c \
	src/plpat6.c \
	src/scrchk.c \
	src/game.c \
	src/r7/act71a.c \
	src/r7/chgban.c \
	src/r7/chgwall7.c \
	src/r7/coli7.c \
	src/r7/edtbl7.c \
	src/r7/playsp7.c \
	src/r7/z71act.c \
	src/r7/zonetbl7.c \
	src/ridechk.c \
	src/r7/wall7.c \
	src/r7/tekkyu7.c \
	src/r7/tekkyu7j.c \
	src/r7/slight7.c \
	src/r7/friend7.c \
	src/r7/branko7.c \
	src/r7/et7.c \
	src/r7/kanabun.c \
	src/r7/dango7.c \
	src/r7/movie7.c \
	src/r7/col7d.c \
	src/r7/dev71d.c \
	src/r7/scr71d.c \
	src/r7/z71dtbl.c \
	src/r7/hotaru7.c \
	src/r7/kabasira.c

R72A_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/playsub.c \
	src/plpat6.c \
	src/scrchk.c \
	src/game.c \
	src/r7/act71a.c \
	src/r7/chgban.c \
	src/r7/chgwall7.c \
	src/r7/col7a.c \
	src/r7/coli7.c \
	src/r7/edtbl7.c \
	src/r7/playsp7.c \
	src/r7/zonetbl7.c \
	src/ridechk.c \
	src/r7/wall7.c \
	src/r7/tekkyu7.c \
	src/r7/tekkyu7j.c \
	src/r7/slight7.c \
	src/r7/friend7.c \
	src/r7/branko7.c \
	src/r7/et7.c \
	src/r7/kanabun.c \
	src/r7/dango7.c \
	src/r7/movie7.c \
	src/r7/scr72a.c \
	src/r7/z72act.c \
	src/r7/dev72a.c \
	src/r7/z72atbl.c \
	src/r7/hotaru7.c \
	src/r7/kabasira.c

R72B_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/playsub.c \
	src/plpat6.c \
	src/scrchk.c \
	src/game.c \
	src/r7/act71a.c \
	src/r7/chgban.c \
	src/r7/chgwall7.c \
	src/r7/coli7.c \
	src/r7/edtbl7.c \
	src/r7/playsp7.c \
	src/r7/zonetbl7.c \
	src/ridechk.c \
	src/r7/wall7.c \
	src/r7/tekkyu7.c \
	src/r7/tekkyu7j.c \
	src/r7/slight7.c \
	src/r7/friend7.c \
	src/r7/branko7.c \
	src/r7/et7.c \
	src/r7/kanabun.c \
	src/r7/dango7.c \
	src/r7/movie7.c \
	src/r7/z72act.c \
	src/r7/col7b.c \
	src/r7/dev72b.c \
	src/r7/scr72b.c \
	src/r7/z72btbl.c \
	src/r7/hotaru7.c \
	src/r7/kabasira.c

R72C_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/playsub.c \
	src/plpat6.c \
	src/scrchk.c \
	src/game.c \
	src/r7/act71a.c \
	src/r7/chgban.c \
	src/r7/chgwall7.c \
	src/r7/coli7.c \
	src/r7/edtbl7.c \
	src/r7/playsp7.c \
	src/r7/zonetbl7.c \
	src/ridechk.c \
	src/r7/wall7.c \
	src/r7/tekkyu7.c \
	src/r7/tekkyu7j.c \
	src/r7/slight7.c \
	src/r7/friend7.c \
	src/r7/branko7.c \
	src/r7/et7.c \
	src/r7/kanabun.c \
	src/r7/dango7.c \
	src/r7/movie7.c \
	src/r7/z72act.c \
	src/r7/col7c.c \
	src/r7/dev72c.c \
	src/r7/scr72c.c \
	src/r7/z72ctbl.c \
	src/r7/hotaru7.c \
	src/r7/kabasira.c

R72D_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/playsub.c \
	src/plpat6.c \
	src/scrchk.c \
	src/game.c \
	src/r7/act71a.c \
	src/r7/chgban.c \
	src/r7/chgwall7.c \
	src/r7/coli7.c \
	src/r7/edtbl7.c \
	src/r7/playsp7.c \
	src/r7/zonetbl7.c \
	src/ridechk.c \
	src/r7/wall7.c \
	src/r7/tekkyu7.c \
	src/r7/tekkyu7j.c \
	src/r7/slight7.c \
	src/r7/friend7.c \
	src/r7/branko7.c \
	src/r7/et7.c \
	src/r7/kanabun.c \
	src/r7/dango7.c \
	src/r7/movie7.c \
	src/r7/z72act.c \
	src/r7/col7d.c \
	src/r7/dev72d.c \
	src/r7/scr72d.c \
	src/r7/z72dtbl.c \
	src/r7/hotaru7.c \
	src/r7/kabasira.c

R73C_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/playsub.c \
	src/plpat6.c \
	src/scrchk.c \
	src/game.c \
	src/r7/chgban.c \
	src/r7/chgwall7.c \
	src/r7/coli7.c \
	src/r7/playsp7.c \
	src/r7/zonetbl7.c \
	src/ridechk.c \
	src/r7/wall7.c \
	src/r7/tekkyu7.c \
	src/r7/tekkyu7j.c \
	src/r7/friend7.c \
	src/r7/branko7.c \
	src/r7/movie7.c \
	src/r7/scr73c.c \
	src/r7/act73.c \
	src/r7/edtbl73.c \
	src/r7/z73act.c \
	src/r7/gaitou73.c \
	src/r7/wall73.c \
	src/r7/emie7.c \
	src/r7/col7c.c \
	src/r7/z73ctbl.c \
	src/r7/dev73c.c \
	src/r7/hari73.c \
	src/r7/emie7cg.c \
	src/r7/boss_7.c

R73D_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/playsub.c \
	src/plpat6.c \
	src/scrchk.c \
	src/game.c \
	src/r7/chgban.c \
	src/r7/chgwall7.c \
	src/r7/coli7.c \
	src/r7/playsp7.c \
	src/r7/zonetbl7.c \
	src/ridechk.c \
	src/r7/wall7.c \
	src/r7/tekkyu7.c \
	src/r7/tekkyu7j.c \
	src/r7/friend7.c \
	src/r7/branko7.c \
	src/r7/movie7.c \
	src/r7/act73.c \
	src/r7/edtbl73.c \
	src/r7/z73act.c \
	src/r7/gaitou73.c \
	src/r7/wall73.c \
	src/r7/emie7.c \
	src/r7/col7d.c \
	src/r7/scr73d.c \
	src/r7/dev73d.c \
	src/r7/z73dtbl.c \
	src/r7/hari73.c \
	src/r7/emie7cg.c \
	src/r7/boss_7.c

R81A_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/playsub.c \
	src/game.c \
	src/ridechk.c \
	src/plpat8.c \
	src/r8/act81a.c \
	src/r8/col8a.c \
	src/r8/coli8.c \
	src/r8/dev81a.c \
	src/r8/edtbl81.c \
	src/playsp.c \
	src/r8/scr81a.c \
	src/r8/z81act.c \
	src/r8/z81atbl.c \
	src/r8/zonetbl8.c \
	src/r8/trap_r81.c \
	src/r8/kabuto8.c \
	src/r8/koma8.c \
	src/r8/sw8.c \
	src/r8/okusieso.c \
	src/r8/tobira.c \
	src/r8/konbea.c \
	src/r8/scarab.c \
	src/r8/kuzure8.c \
	src/r8/nokogiri.c \
	src/r8/propera8.c \
	src/r8/dai8.c \
	src/r8/hachi8.c \
	src/r8/et8.c \
	src/r8/friend8.c \
	src/r8/piston.c \
	src/r8/haguruma.c \
	src/r8/scrchk8.c

R81B_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/playsub.c \
	src/game.c \
	src/ridechk.c \
	src/plpat8.c \
	src/r8/act81a.c \
	src/r8/coli8.c \
	src/r8/edtbl81.c \
	src/playsp.c \
	src/r8/z81act.c \
	src/r8/zonetbl8.c \
	src/r8/trap_r81.c \
	src/r8/kabuto8.c \
	src/r8/koma8.c \
	src/r8/sw8.c \
	src/r8/okusieso.c \
	src/r8/tobira.c \
	src/r8/konbea.c \
	src/r8/scarab.c \
	src/r8/kuzure8.c \
	src/r8/nokogiri.c \
	src/r8/propera8.c \
	src/r8/dai8.c \
	src/r8/hachi8.c \
	src/r8/et8.c \
	src/r8/friend8.c \
	src/r8/piston.c \
	src/r8/haguruma.c \
	src/r8/col8b.c \
	src/r8/scr81b.c \
	src/r8/z81btbl.c \
	src/r8/dev81b.c \
	src/r8/scrchk8.c

R81C_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/playsub.c \
	src/game.c \
	src/ridechk.c \
	src/plpat8.c \
	src/r8/act81a.c \
	src/r8/coli8.c \
	src/r8/edtbl81.c \
	src/playsp.c \
	src/r8/z81act.c \
	src/r8/zonetbl8.c \
	src/r8/trap_r81.c \
	src/r8/kabuto8.c \
	src/r8/koma8.c \
	src/r8/sw8.c \
	src/r8/okusieso.c \
	src/r8/tobira.c \
	src/r8/konbea.c \
	src/r8/scarab.c \
	src/r8/kuzure8.c \
	src/r8/nokogiri.c \
	src/r8/propera8.c \
	src/r8/dai8.c \
	src/r8/hachi8.c \
	src/r8/et8.c \
	src/r8/friend8.c \
	src/r8/piston.c \
	src/r8/haguruma.c \
	src/r8/col8c.c \
	src/r8/scr81c.c \
	src/r8/z81ctbl.c \
	src/r8/dev81c.c \
	src/r8/scrchk8.c

R81D_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/playsub.c \
	src/game.c \
	src/ridechk.c \
	src/plpat8.c \
	src/r8/act81a.c \
	src/r8/coli8.c \
	src/r8/edtbl81.c \
	src/playsp.c \
	src/r8/z81act.c \
	src/r8/zonetbl8.c \
	src/r8/trap_r81.c \
	src/r8/kabuto8.c \
	src/r8/koma8.c \
	src/r8/sw8.c \
	src/r8/okusieso.c \
	src/r8/tobira.c \
	src/r8/konbea.c \
	src/r8/scarab.c \
	src/r8/kuzure8.c \
	src/r8/nokogiri.c \
	src/r8/propera8.c \
	src/r8/dai8.c \
	src/r8/hachi8.c \
	src/r8/et8.c \
	src/r8/friend8.c \
	src/r8/piston.c \
	src/r8/haguruma.c \
	src/r8/col8d.c \
	src/r8/scr81d.c \
	src/r8/z81dtbl.c \
	src/r8/dev81d.c \
	src/r8/scrchk8.c

R82A_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/playsub.c \
	src/game.c \
	src/ridechk.c \
	src/plpat8.c \
	src/r8/col8a.c \
	src/r8/coli8.c \
	src/r8/zonetbl8.c \
	src/r8/kabuto8.c \
	src/r8/koma8.c \
	src/r8/sw8.c \
	src/r8/okusieso.c \
	src/r8/tobira.c \
	src/r8/kuzure8.c \
	src/r8/dai8.c \
	src/r8/et8.c \
	src/r8/friend8.c \
	src/r8/piston.c \
	src/r8/haguruma.c \
	src/r8/playsp8.c \
	src/r8/act82a.c \
	src/r8/edtbl82.c \
	src/r8/trap_r82.c \
	src/r8/z82act.c \
	src/r8/z82atbl.c \
	src/r8/beam.c \
	src/r8/shut.c \
	src/r8/dev82a.c \
	src/r8/shoot.c \
	src/r8/dango8.c \
	src/r8/legmeca8.c \
	src/r8/bigbom8.c \
	src/r8/scr82a.c \
	src/r8/scrchk8.c \
	src/r8/jettoge8.c \
	src/r8/pipe8.c

R82B_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/playsub.c \
	src/game.c \
	src/ridechk.c \
	src/plpat8.c \
	src/r8/coli8.c \
	src/r8/zonetbl8.c \
	src/r8/kabuto8.c \
	src/r8/koma8.c \
	src/r8/sw8.c \
	src/r8/okusieso.c \
	src/r8/tobira.c \
	src/r8/kuzure8.c \
	src/r8/dai8.c \
	src/r8/et8.c \
	src/r8/friend8.c \
	src/r8/piston.c \
	src/r8/haguruma.c \
	src/r8/playsp8.c \
	src/r8/act82a.c \
	src/r8/edtbl82.c \
	src/r8/trap_r82.c \
	src/r8/z82act.c \
	src/r8/beam.c \
	src/r8/shut.c \
	src/r8/shoot.c \
	src/r8/dango8.c \
	src/r8/legmeca8.c \
	src/r8/bigbom8.c \
	src/r8/scrchk8.c \
	src/r8/jettoge8.c \
	src/r8/pipe8.c \
	src/r8/col8b.c \
	src/r8/dev82b.c \
	src/r8/scr82b.c \
	src/r8/z82btbl.c

R82C_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/playsub.c \
	src/game.c \
	src/ridechk.c \
	src/plpat8.c \
	src/r8/coli8.c \
	src/r8/zonetbl8.c \
	src/r8/kabuto8.c \
	src/r8/koma8.c \
	src/r8/sw8.c \
	src/r8/okusieso.c \
	src/r8/tobira.c \
	src/r8/kuzure8.c \
	src/r8/dai8.c \
	src/r8/et8.c \
	src/r8/friend8.c \
	src/r8/piston.c \
	src/r8/haguruma.c \
	src/r8/playsp8.c \
	src/r8/act82a.c \
	src/r8/edtbl82.c \
	src/r8/trap_r82.c \
	src/r8/z82act.c \
	src/r8/beam.c \
	src/r8/shut.c \
	src/r8/shoot.c \
	src/r8/dango8.c \
	src/r8/legmeca8.c \
	src/r8/bigbom8.c \
	src/r8/scrchk8.c \
	src/r8/jettoge8.c \
	src/r8/pipe8.c \
	src/r8/col8c.c \
	src/r8/dev82c.c \
	src/r8/scr82c.c \
	src/r8/z82ctbl.c

R82D_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/playsub.c \
	src/game.c \
	src/ridechk.c \
	src/plpat8.c \
	src/r8/coli8.c \
	src/r8/zonetbl8.c \
	src/r8/kabuto8.c \
	src/r8/koma8.c \
	src/r8/sw8.c \
	src/r8/okusieso.c \
	src/r8/tobira.c \
	src/r8/kuzure8.c \
	src/r8/dai8.c \
	src/r8/et8.c \
	src/r8/friend8.c \
	src/r8/piston.c \
	src/r8/haguruma.c \
	src/r8/playsp8.c \
	src/r8/act82a.c \
	src/r8/edtbl82.c \
	src/r8/trap_r82.c \
	src/r8/z82act.c \
	src/r8/beam.c \
	src/r8/shut.c \
	src/r8/shoot.c \
	src/r8/dango8.c \
	src/r8/legmeca8.c \
	src/r8/bigbom8.c \
	src/r8/scrchk8.c \
	src/r8/jettoge8.c \
	src/r8/pipe8.c \
	src/r8/col8d.c \
	src/r8/dev82d.c \
	src/r8/scr82d.c \
	src/r8/z82dtbl.c

R83C_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/playsub.c \
	src/game.c \
	src/ridechk.c \
	src/plpat8.c \
	src/r8/coli8.c \
	src/r8/zonetbl8.c \
	src/r8/col8c.c \
	src/r8/edtbl83.c \
	src/r8/scr83c.c \
	src/r8/scrchk8.c \
	src/r8/z83act.c \
	src/r8/z83ctbl.c \
	src/r8/haguruma.c \
	src/r8/boss_8.c \
	src/r8/piston.c \
	src/r8/kabuto8.c \
	src/r8/kuzure8.c \
	src/r8/nokogiri.c \
	src/r8/tobira.c \
	src/r8/jettoge8.c \
	src/r8/dai8.c \
	src/r8/friend8.c \
	src/r8/sw8.c \
	src/r8/koma8.c \
	src/r8/dev83c.c \
	src/r8/act83.c \
	src/r8/playsp8.c \
	src/r3/bobin.c \
	src/r8/shut.c \
	src/r8/emie8.c \
	src/r8/konbea83.c \
	src/r8/trap_r83.c \
	src/r8/hota8c.c

R83D_SOURCES := \
	src/actset.c \
	src/etc.c \
	src/fcol.c \
	src/io.c \
	src/spring.c \
	src/score.c \
	src/save.c \
	src/ring.c \
	src/player.c \
	src/dircol.c \
	src/edit.c \
	src/goal.c \
	src/item.c \
	src/zone.c \
	src/equ.c \
	src/loader2.c \
	src/bmp.c \
	src/dummy.c \
	src/dllmain.c \
	src/suicide.c \
	src/plchg.c \
	src/action.c \
	src/tbl0.c \
	src/tbl1.c \
	src/playsub.c \
	src/game.c \
	src/ridechk.c \
	src/plpat8.c \
	src/r8/coli8.c \
	src/r8/zonetbl8.c \
	src/r8/edtbl83.c \
	src/r8/scrchk8.c \
	src/r8/z83act.c \
	src/r8/haguruma.c \
	src/r8/boss_8.c \
	src/r8/piston.c \
	src/r8/kabuto8.c \
	src/r8/kuzure8.c \
	src/r8/nokogiri.c \
	src/r8/tobira.c \
	src/r8/jettoge8.c \
	src/r8/dai8.c \
	src/r8/friend8.c \
	src/r8/sw8.c \
	src/r8/koma8.c \
	src/r8/act83.c \
	src/r8/playsp8.c \
	src/r3/bobin.c \
	src/r8/shut.c \
	src/r8/trap_r83.c \
	src/r8/konbea83.c \
	src/r8/emie8.c \
	src/r8/col8d.c \
	src/r8/dev83d.c \
	src/r8/scr83d.c \
	src/r8/z83dtbl.c \
	src/r8/hota8d.c

SPECIAL_SOURCES := \
	src/special/act_s.c \
	src/special/bact.c \
	src/special/bmp.c \
	src/special/coli_s.c \
	src/special/common.c \
	src/special/data_s1.c \
	src/special/ens.c \
	src/special/etc_m.c \
	src/special/etc_s.c \
	src/special/fadein.c \
	src/special/game.c \
	src/special/kaiten.c \
	src/special/pls.c \
	src/special/spm_equ.c \
	src/special/sps_equ.c \
	src/dllmain.c \
	src/special/sin.c

WARP_SOURCES := \
	src/warp/warp.c \
	src/action.c \
	src/dllmain.c \
	src/equ.c \
	src/bmp.c \
	src/io.c

BESTTIME_SOURCES := \
	src/title/besttime/besentry.c \
	src/title/besttime/bestbmp.c \
	src/title/besttime/bestdo.c \
	src/title/besttime/bestgrid.c \
	src/title/besttime/bestitem.c \
	src/title/besttime/bestpalt.c \
	src/title/besttime/bestsprt.c \
	src/title/common/hmx_oeeactl.c

OPENING_SOURCES := \
	src/title/opening/opnbmp.c \
	src/title/opening/opndo.c \
	src/title/opening/opnentry.c \
	src/title/opening/opngrid.c \
	src/title/common/hmx_oeeactl.c \
	src/title/opening/opnpalt.c

PLANET_SOURCES := \
	src/title/planet/actm.c \
	src/title/planet/asciiset.c \
	src/title/planet/ascmap.c \
	src/title/planet/chamov.c \
	src/title/planet/data_m.c \
	src/title/planet/etc.c \
	src/title/planet/flash.c \
	src/title/planet/kaiten.c \
	src/title/planet/lplmain.c \
	src/title/planet/palet.c \
	src/title/planet/plm.c \
	src/title/planet/pls.c \
	src/title/planet/spm_equ.c \
	src/title/planet/sps_equ.c

SAVEDATA_SOURCES := \
	src/title/savedata/svdbmp.c \
	src/title/savedata/svddo.c \
	src/title/savedata/svdedit.c \
	src/title/savedata/svdentry.c \
	src/title/savedata/svdfile.c \
	src/title/savedata/svdgrid.c \
	src/title/savedata/svdmenu.c \
	src/title/savedata/svdpalt.c \
	src/title/savedata/svdsprt.c \
	src/title/common/hmx_oeeactl.c

SOUNDTST_SOURCES := \
	src/title/soundtst/sndentry.c \
	src/title/soundtst/sndbmp.c \
	src/title/soundtst/snddo.c \
	src/title/soundtst/sndgrid.c \
	src/title/soundtst/sndpalt.c \
	src/title/soundtst/sndsprt.c \
	src/title/common/hmx_oeeactl.c

STAGETST_SOURCES := \
	src/title/stagetst/stgentry.c \
	src/title/stagetst/stgbmp.c \
	src/title/stagetst/stgdo.c \
	src/title/stagetst/stggrid.c \
	src/title/stagetst/stgitem.c \
	src/title/stagetst/stgpalt.c \
	src/title/stagetst/stgsprt.c \
	src/title/common/hmx_oeeactl.c

TA_SOURCES := \
	src/title/ta/ta.c \
	src/title/ta/fadein.c \
	src/title/ta/tacolor.c \
	src/title/ta/taeactrl.c

THANKS_SOURCES := \
	src/title/thanks/act.c \
	src/title/thanks/bm_m.c \
	src/title/thanks/chrset.c \
	src/title/thanks/data_sp.c \
	src/title/thanks/etc_m.c \
	src/title/thanks/fade.c \
	src/title/thanks/game.c \
	src/title/thanks/io.c \
	src/title/thanks/spm.c \
	src/title/thanks/sprmove.c

VISUALMD_SOURCES := \
	src/title/visualmd/vmdbmp.c \
	src/title/visualmd/vmddo.c \
	src/title/visualmd/vmdentry.c \
	src/title/visualmd/vmdgrid.c \
	src/title/common/hmx_oeeactl.c \
	src/title/visualmd/vmdpalt.c \
	src/title/visualmd/vmdsprt.c

.PHONY: all elf clean compare $(VARIANTS:%=compare-%)

elf: $(ELF_TARGETS)

compare: $(VARIANTS:%=compare-%)

define BUILD_VARIANT
$(1)_OUTPUT := $$(BUILD_DIR)/$$($(1)_NAME).elf
$(1)_OBJECT_DIR := $$(BUILD_DIR)/obj/$$($(1)_NAME)
$(1)_OBJECTS := $$($(1)_SOURCES:%.c=$$($(1)_OBJECT_DIR)/%.o)

$$($(1)_OUTPUT): $$($(1)_OBJECTS) $$(LINKER_SCRIPT) makefile
	mkdir -p $$(dir $$@)
	"$$(LD)" $$(LDFLAGS) -o $$@ $$(LINKER_SCRIPT) $$($(1)_OBJECTS)

$$($(1)_OBJECT_DIR)/%.o: %.c
	mkdir -p $$(dir $$@)
	"$$(CC)" $$(COMPILE_ONLY) $$(CFLAGS) $$($(1)_DEFINES) $$(INCLUDES) -o $$@ $$<

compare-$(1): $$($(1)_OUTPUT)
	python tools/compare_elf.py $$(REFERENCE_DIR)/$$($(1)_NAME).elf $$($(1)_OUTPUT)
endef

$(foreach variant,$(VARIANTS),$(eval $(call BUILD_VARIANT,$(variant))))

clean:
	rm -rf $(BUILD_DIR)/obj $(ELF_TARGETS) $(ELF_TARGETS:.elf=.map) $(BUILD_DIR)/probe.o $(MSVC_BUILD_DIR)
