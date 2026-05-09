BUILD_DIR := build
REFERENCE_DIR := references
LINKER_SCRIPT ?= linker.lcf
R11A_LINKER_SCRIPT := $(LINKER_SCRIPT)
R11B_LINKER_SCRIPT := linker-r11b.lcf
R11C_LINKER_SCRIPT := linker-r11b.lcf
R11D_LINKER_SCRIPT := linker-r11b.lcf
R12A_LINKER_SCRIPT := linker-r11b.lcf
R12B_LINKER_SCRIPT := linker-r11b.lcf
R12C_LINKER_SCRIPT := linker-r11b.lcf
R12D_LINKER_SCRIPT := linker-r11b.lcf
R13C_LINKER_SCRIPT := linker-r11b.lcf
R13D_LINKER_SCRIPT := linker-r11b.lcf
R31A_LINKER_SCRIPT := linker-r11b.lcf
R31B_LINKER_SCRIPT := linker-r11b.lcf
R31C_LINKER_SCRIPT := linker-r11b.lcf
R31D_LINKER_SCRIPT := linker-r11b.lcf
R32A_LINKER_SCRIPT := linker-r11b.lcf
R32B_LINKER_SCRIPT := linker-r11b.lcf
R32C_LINKER_SCRIPT := $(LINKER_SCRIPT)
R32D_LINKER_SCRIPT := $(LINKER_SCRIPT)
R33C_LINKER_SCRIPT := linker-r33c.lcf
R33D_LINKER_SCRIPT := linker-r11b.lcf
R41A_LINKER_SCRIPT := linker-r11b.lcf
R41B_LINKER_SCRIPT := linker-r11b.lcf
R41C_LINKER_SCRIPT := linker-r11b.lcf
R41D_LINKER_SCRIPT := linker-r11b.lcf
R42A_LINKER_SCRIPT := linker-r11b.lcf
R42B_LINKER_SCRIPT := linker-r11b.lcf
R42C_LINKER_SCRIPT := linker-r11b.lcf
R42D_LINKER_SCRIPT := linker-r11b.lcf
R43C_LINKER_SCRIPT := linker-r11b.lcf
R43D_LINKER_SCRIPT := linker-r11b.lcf
R51A_LINKER_SCRIPT := linker-r11b.lcf
R51B_LINKER_SCRIPT := $(LINKER_SCRIPT)
R51C_LINKER_SCRIPT := $(LINKER_SCRIPT)
R51D_LINKER_SCRIPT := linker-r11b.lcf
R52A_LINKER_SCRIPT := linker-r11b.lcf
R52B_LINKER_SCRIPT := linker-r11b.lcf
R52C_LINKER_SCRIPT := $(LINKER_SCRIPT)
R52D_LINKER_SCRIPT := linker-r11b.lcf
R53C_LINKER_SCRIPT := linker-r11b.lcf
R53D_LINKER_SCRIPT := linker-r11b.lcf
R61A_LINKER_SCRIPT := linker-r11b.lcf
R61B_LINKER_SCRIPT := linker-r11b.lcf
R61C_LINKER_SCRIPT := linker-r11b.lcf
R61D_LINKER_SCRIPT := linker-r11b.lcf
R62A_LINKER_SCRIPT := linker-r11b.lcf
R62B_LINKER_SCRIPT := linker-r11b.lcf
R62C_LINKER_SCRIPT := linker-r11b.lcf
R62D_LINKER_SCRIPT := linker-r11b.lcf
R63C_LINKER_SCRIPT := linker-r11b.lcf
R63D_LINKER_SCRIPT := linker-r11b.lcf
R71A_LINKER_SCRIPT := linker-r11b.lcf
R71B_LINKER_SCRIPT := linker-r11b.lcf
R71C_LINKER_SCRIPT := linker-r11b.lcf
R71D_LINKER_SCRIPT := linker-r11b.lcf
R72A_LINKER_SCRIPT := linker-r11b.lcf
R72B_LINKER_SCRIPT := linker-r11b.lcf
R72C_LINKER_SCRIPT := linker-r11b.lcf
R72D_LINKER_SCRIPT := linker-r11b.lcf
R73C_LINKER_SCRIPT := linker-r11b.lcf
R73D_LINKER_SCRIPT := linker-r11b.lcf
R81A_LINKER_SCRIPT := linker-r11b.lcf
R81B_LINKER_SCRIPT := linker-r11b.lcf
R81C_LINKER_SCRIPT := linker-r11b.lcf
R81D_LINKER_SCRIPT := linker-r11b.lcf
R82A_LINKER_SCRIPT := linker-r11b.lcf
R82B_LINKER_SCRIPT := linker-r11b.lcf
R82C_LINKER_SCRIPT := linker-r11b.lcf
R82D_LINKER_SCRIPT := linker-r11b.lcf
R83C_LINKER_SCRIPT := linker-r11b.lcf
R83D_LINKER_SCRIPT := linker-r11b.lcf
SPECIAL_LINKER_SCRIPT := linker-r11b.lcf
WARP_LINKER_SCRIPT := linker-r11b.lcf
BESTTIME_LINKER_SCRIPT := linker-r11b.lcf
OPENING_LINKER_SCRIPT := linker-r11b.lcf
PLANET_LINKER_SCRIPT := linker-r11b.lcf
SAVEDATA_LINKER_SCRIPT := linker-r11b.lcf
SOUNDTST_LINKER_SCRIPT := linker-r11b.lcf
STAGETST_LINKER_SCRIPT := linker-r11b.lcf
TA_LINKER_SCRIPT := linker-r11b.lcf
THANKS_LINKER_SCRIPT := linker-r11b.lcf
VISUALMD_LINKER_SCRIPT := linker-r11b.lcf
MSYS2_ROOT ?= C:/msys64
SHELL := $(MSYS2_ROOT)/usr/bin/sh.exe
MKDIR_P := $(MSYS2_ROOT)/usr/bin/mkdir.exe -p
RM_RF := $(MSYS2_ROOT)/usr/bin/rm.exe -rf

COMPILER_DIR ?= $(firstword $(wildcard compiler compilers))
ifeq ($(COMPILER_DIR),)
COMPILER_DIR := compiler
endif

CC := $(COMPILER_DIR)/mwccps2.exe
LD := $(COMPILER_DIR)/mwldps2.exe

VARIANTS := R11A R11B R11C R11D R12A R12B R12C R12D R13C R13D R31A R31B R31C R31D R32A R32B R32C R32D R33C R33D R41A R41B R41C R41D R42A R42B R42C R42D R43C R43D R51A R51B R51C R51D R52A R52B R52C R52D R53C R53D R61A R61B R61C R61D R62A R62B R62C R62D R63C R63D R71A R71B R71C R71D R72A R72B R72C R72D R73C R73D R81A R81B R81C R81D R82A R82B R82C R82D R83C R83D SPECIAL WARP BESTTIME OPENING PLANET SAVEDATA SOUNDTST STAGETST TA THANKS VISUALMD
TARGETS := $(VARIANTS:%=$(BUILD_DIR)/%.ELF)

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

MSL_DIR ?= C:/Program Files (x86)/Metrowerks/CodeWarrior/PS2 Support/Msl/MSL_C
RUNTIME_LIB_DIR ?= C:/Program Files (x86)/Metrowerks/CodeWarrior/PS2 Support/Runtime/Libraries
PROJECT_INCLUDES := -i "main" -i "src/R1"
MSL_INCLUDES := -i "$(MSL_DIR)"
INCLUDES := $(PROJECT_INCLUDES) $(MSL_INCLUDES)
export MWCIncludes := $(MSL_INCLUDES)
export MWLibraries := "$(RUNTIME_LIB_DIR)"
CFLAGS := -cwd include -fl divbyzerocheck -g -nodead -sdatathreshold 0
COMPILE_ONLY := -c
LDFLAGS := -nodead -g -main ""

R11_PREFIX_SOURCES := \
	src/DLLMAIN.C \
	src/ACTION.C \
	src/ACTSET.C \
	src/EQU.C \
	src/DAI_K.C \
	src/DAI_RD1.C \
	src/DIRCOL.C \
	src/DUMMY.C \
	src/EDIT.C

R11_POST_EDIT_SOURCES := \
	src/ENEMY.C \
	src/ET1.C \
	src/ETC.C \
	src/FCOL.C \
	src/GAME.C \
	src/GOAL.C \
	src/IO.C \
	src/ITEM.C \
	src/PLAYER.C \
	src/PLAYPAT1.C \
	src/PLAYSP.C \
	src/PLAYSUB.C \
	src/PLCHG.C \
	src/RIDECHK.C \
	src/RING.C \
	src/SCORE.C \
	src/SCRCHK.C \
	src/SPRING.C \
	src/SUICIDE.C \
	src/TAKI.C \
	src/TBL0.C \
	src/TBL1.C \
	src/TREE.C \
	src/ZONE.C \
	src/ZONETBL1.C

R11C_POST_EDIT_SOURCES := \
	src/ENEMY.C \
	src/ET1.C \
	src/ETC.C \
	src/FCOL.C \
	src/GAME.C \
	src/GOAL.C \
	src/IO.C \
	src/ITEM.C \
	src/PLAYER.C \
	src/PLAYPAT1.C \
	src/PLAYSP.C \
	src/PLAYSUB.C \
	src/PLCHG.C \
	src/RIDECHK.C \
	src/RING.C \
	src/SCORE.C \
	src/SCRCHK.C \
	src/SPRING.C \
	src/SUICIDE.C \
	src/TAKI.C \
	src/TBL0.C \
	src/TBL1.C \
	src/ZONE.C \
	src/ZONETBL1.C

R11_TAIL_SOURCES := \
	src/BLOCK.C \
	src/LOADER2.C \
	src/R1/COLI1.C \
	src/SAVE.C \
	src/R1/FRIEND1.C \
	src/R1/MOVIE1.C \
	src/BMP.C

R11A_SOURCES := \
	$(R11_PREFIX_SOURCES) \
	src/EMIE1.C \
	src/EMIE1CG.C \
	$(R11_POST_EDIT_SOURCES) \
	src/R1/ACT11A.C \
	src/R1/COL1A.C \
	src/R1/DEV11A.C \
	src/R1/EDTBL11A.C \
	src/R1/KOWASI1.C \
	src/R1/KUZUR11A.C \
	src/R1/KUZURE.C \
	src/R1/SCR11A.C \
	src/R1/SHOOT1.C \
	src/R1/SIKAKE.C \
	src/R1/Z11ACT.C \
	src/BLOCK.C \
	src/LOADER2.C \
	src/R1/COLI1.C \
	src/SAVE.C \
	src/R1/Z11ATBL.C \
	src/R1/FRIEND1.C \
	src/R1/MOVIE1.C \
	src/BMP.C

R11B_SOURCES := \
	$(R11_PREFIX_SOURCES) \
	$(R11_POST_EDIT_SOURCES) \
	src/R1/KOWASI1.C \
	src/R1/KUZURE.C \
	src/R1/SHOOT1.C \
	src/R1/SIKAKE.C \
	src/R1/Z11ACT.C \
	src/BLOCK.C \
	src/LOADER2.C \
	src/BMP.C \
	src/R1/COLI1.C \
	src/R1/ACT11B.C \
	src/R1/COL1B.C \
	src/R1/DEV11B.C \
	src/R1/EDTBL11B.C \
	src/R1/KUZUR11B.C \
	src/R1/SCR11B.C \
	src/R1/Z11BTBL.C \
	src/R1/FRIEND1.C \
	src/R1/MOVIE1.C \
	src/SAVE.C

R11C_SOURCES := \
	$(R11_PREFIX_SOURCES) \
	$(R11C_POST_EDIT_SOURCES) \
	src/R1/KOWASI1.C \
	src/R1/KUZURE.C \
	src/R1/SHOOT1.C \
	src/R1/SIKAKE.C \
	src/BLOCK.C \
	src/LOADER2.C \
	src/BMP.C \
	src/R1/COLI1.C \
	src/R1/DEV11C.C \
	src/R1/SCR11C.C \
	src/R1/Z11ACT.C \
	src/R1/Z11CTBL.C \
	src/R1/ACT11B.C \
	src/R1/EDTBL11C.C \
	src/R1/KUZUR11C.C \
	src/R1/COL1C.C \
	src/R1/FRIEND1.C \
	src/R1/MOVIE1.C \
	src/SAVE.C

R11D_SOURCES := \
	$(R11_PREFIX_SOURCES) \
	$(R11C_POST_EDIT_SOURCES) \
	src/R1/KOWASI1.C \
	src/R1/KUZURE.C \
	src/R1/SHOOT1.C \
	src/R1/SIKAKE.C \
	src/BLOCK.C \
	src/LOADER2.C \
	src/BMP.C \
	src/R1/COLI1.C \
	src/R1/Z11ACT.C \
	src/R1/ACT11B.C \
	src/R1/DEV11D.C \
	src/R1/EDTBL11D.C \
	src/R1/KUZUR11D.C \
	src/R1/Z11DTBL.C \
	src/R1/COL1D.C \
	src/R1/SCR11D.C \
	src/R1/FRIEND1.C \
	src/R1/MOVIE1.C \
	src/SAVE.C

R12A_SOURCES := \
	$(R11_PREFIX_SOURCES) \
	$(R11_POST_EDIT_SOURCES) \
	src/R1/COL1A.C \
	src/R1/KOWASI1.C \
	src/R1/KUZURE.C \
	src/R1/SHOOT1.C \
	src/R1/SIKAKE.C \
	src/BLOCK.C \
	src/LOADER2.C \
	src/BMP.C \
	src/R1/COLI1.C \
	src/R1/ACT12A.C \
	src/R1/DEV12A.C \
	src/R1/EDTBL12A.C \
	src/R1/KUZUR12A.C \
	src/BRANKO1.C \
	src/R1/SCR12A.C \
	src/R1/Z12ACT.C \
	src/R1/Z12ATBL.C \
	src/R1/FRIEND1.C \
	src/R1/MOVIE1.C \
	src/SAVE.C

R12B_SOURCES := \
	$(R11_PREFIX_SOURCES) \
	$(R11_POST_EDIT_SOURCES) \
	src/R1/KOWASI1.C \
	src/R1/KUZURE.C \
	src/R1/SHOOT1.C \
	src/R1/SIKAKE.C \
	src/BLOCK.C \
	src/LOADER2.C \
	src/BMP.C \
	src/R1/COLI1.C \
	src/R1/ACT12A.C \
	src/R1/COL1B.C \
	src/R1/DEV12B.C \
	src/R1/EDTBL12B.C \
	src/R1/KUZUR12B.C \
	src/R1/SCR12B.C \
	src/R1/Z12ACT.C \
	src/R1/Z12BTBL.C \
	src/BRANKO1.C \
	src/R1/FRIEND1.C \
	src/R1/MOVIE1.C \
	src/SAVE.C

R12C_SOURCES := \
	$(R11_PREFIX_SOURCES) \
	$(R11C_POST_EDIT_SOURCES) \
	src/R1/KOWASI1.C \
	src/R1/KUZURE.C \
	src/R1/SHOOT1.C \
	src/R1/SIKAKE.C \
	src/BLOCK.C \
	src/LOADER2.C \
	src/BMP.C \
	src/R1/COLI1.C \
	src/R1/COL1C.C \
	src/R1/ACT12A.C \
	src/R1/DEV12C.C \
	src/R1/EDTBL12C.C \
	src/R1/KUZUR12C.C \
	src/R1/SCR12C.C \
	src/R1/Z12ACT.C \
	src/R1/Z12CTBL.C \
	src/BRANKO1.C \
	src/R1/FRIEND1.C \
	src/R1/MOVIE1.C \
	src/SAVE.C

R12D_SOURCES := \
	$(R11_PREFIX_SOURCES) \
	$(R11C_POST_EDIT_SOURCES) \
	src/R1/KOWASI1.C \
	src/R1/KUZURE.C \
	src/R1/SHOOT1.C \
	src/R1/SIKAKE.C \
	src/BLOCK.C \
	src/LOADER2.C \
	src/BMP.C \
	src/R1/COLI1.C \
	src/R1/ACT12A.C \
	src/R1/COL1D.C \
	src/R1/DEV12D.C \
	src/R1/EDTBL12D.C \
	src/R1/KUZUR12D.C \
	src/R1/SCR12D.C \
	src/R1/Z12ACT.C \
	src/R1/Z12DTBL.C \
	src/BRANKO1.C \
	src/R1/FRIEND1.C \
	src/R1/MOVIE1.C \
	src/SAVE.C

R13C_SOURCES := \
	src/DLLMAIN.C \
	src/ACTION.C \
	src/ACTSET.C \
	src/DAI_RD1.C \
	src/DIRCOL.C \
	src/DUMMY.C \
	src/EDIT.C \
	src/ENEMY.C \
	src/EQU.C \
	src/ETC.C \
	src/FCOL.C \
	src/GAME.C \
	src/IO.C \
	src/ITEM.C \
	src/PLAYER.C \
	src/PLAYPAT1.C \
	src/PLAYSP.C \
	src/PLAYSUB.C \
	src/PLCHG.C \
	src/RIDECHK.C \
	src/RING.C \
	src/SCORE.C \
	src/SCRCHK.C \
	src/SPRING.C \
	src/SUICIDE.C \
	src/TBL0.C \
	src/TBL1.C \
	src/TREE.C \
	src/ZONE.C \
	src/ZONETBL1.C \
	src/R1/SHOOT1.C \
	src/R1/SIKAKE.C \
	src/BLOCK.C \
	src/LOADER2.C \
	src/BMP.C \
	src/R1/COLI1.C \
	src/R1/ACT13C.C \
	src/R1/COL1C.C \
	src/R1/DEV13C.C \
	src/R1/EDTBL13C.C \
	src/R1/SCR13C.C \
	src/R1/Z13ACT.C \
	src/R1/Z13CTBL.C \
	src/ET1.C \
	src/GOAL.C \
	src/R1/BOSS_1.C \
	src/R1/FRIEND1.C \
	src/SAVE.C

R13D_SOURCES := \
	src/DLLMAIN.C \
	src/ACTION.C \
	src/ACTSET.C \
	src/DAI_RD1.C \
	src/DIRCOL.C \
	src/DUMMY.C \
	src/EDIT.C \
	src/ENEMY.C \
	src/EQU.C \
	src/ETC.C \
	src/FCOL.C \
	src/GAME.C \
	src/IO.C \
	src/ITEM.C \
	src/PLAYER.C \
	src/PLAYPAT1.C \
	src/PLAYSP.C \
	src/PLAYSUB.C \
	src/PLCHG.C \
	src/RIDECHK.C \
	src/RING.C \
	src/SCORE.C \
	src/SCRCHK.C \
	src/SPRING.C \
	src/SUICIDE.C \
	src/TBL0.C \
	src/TBL1.C \
	src/TREE.C \
	src/ZONE.C \
	src/ZONETBL1.C \
	src/R1/SHOOT1.C \
	src/R1/SIKAKE.C \
	src/BLOCK.C \
	src/LOADER2.C \
	src/BMP.C \
	src/R1/COLI1.C \
	src/ET1.C \
	src/GOAL.C \
	src/R1/ACT13C.C \
	src/R1/COL1D.C \
	src/R1/DEV13D.C \
	src/R1/EDTBL13C.C \
	src/R1/SCR13D.C \
	src/R1/Z13DTBL.C \
	src/R1/Z13ACT.C \
	src/R1/BOSS_1.C \
	src/R1/FRIEND1.C \
	src/SAVE.C

R31A_SOURCES := \
	src/ACTSET.C \
	src/DIRCOL.C \
	src/DUMMY.C \
	src/EDIT.C \
	src/EQU.C \
	src/ETC.C \
	src/FCOL.C \
	src/GAME.C \
	src/GOAL.C \
	src/IO.C \
	src/ITEM.C \
	src/PLAYER.C \
	src/PLAYSUB.C \
	src/RIDECHK.C \
	src/RING.C \
	src/SCORE.C \
	src/SCRCHK.C \
	src/SPRING.C \
	src/SUICIDE.C \
	src/ZONE.C \
	src/LOADER2.C \
	src/R3/BANPA.C \
	src/R3/BOBIN.C \
	src/R3/BOBINB.C \
	src/R3/COLI3.C \
	src/R3/EDTBL31A.C \
	src/R3/ET3.C \
	src/R3/FRIEND3.C \
	src/R3/GA3.C \
	src/R3/PLAYSP3.C \
	src/R3/POCKET.C \
	src/R3/TOGEBL3A.C \
	src/R3/TRAP_R3.C \
	src/R3/Z31ATBL.C \
	src/DLLMAIN.C \
	src/BMP.C \
	src/BLOCK.C \
	src/R3/MIRACLE.C \
	src/TBL0.C \
	src/TBL1.C \
	src/R3/COL3A.C \
	src/R3/SCR31A.C \
	src/R3/Z31ACT.C \
	src/PLCHG.C \
	src/R3/DEV31A.C \
	src/R3/ACT31A.C \
	src/R3/MECASNC.C \
	src/R3/MSNCCG.C \
	src/R3/TENTOU.C \
	src/R3/KAMA.C \
	src/PLPAT6.C \
	src/R3/MOVIE3.C \
	src/R3/ZONETBL3.C \
	src/SAVE.C \
	src/ACTION.C

R31B_SOURCES := \
	src/ACTSET.C \
	src/DIRCOL.C \
	src/DUMMY.C \
	src/EDIT.C \
	src/EQU.C \
	src/ETC.C \
	src/FCOL.C \
	src/GAME.C \
	src/GOAL.C \
	src/IO.C \
	src/ITEM.C \
	src/PLAYER.C \
	src/PLAYSUB.C \
	src/RIDECHK.C \
	src/RING.C \
	src/SCORE.C \
	src/SCRCHK.C \
	src/SPRING.C \
	src/SUICIDE.C \
	src/ZONE.C \
	src/LOADER2.C \
	src/R3/BANPA.C \
	src/R3/BOBIN.C \
	src/R3/BOBINB.C \
	src/R3/COLI3.C \
	src/R3/ET3.C \
	src/R3/FRIEND3.C \
	src/R3/GA3.C \
	src/R3/PLAYSP3.C \
	src/R3/POCKET.C \
	src/R3/TRAP_R3.C \
	src/DLLMAIN.C \
	src/BMP.C \
	src/BLOCK.C \
	src/R3/MIRACLE.C \
	src/TBL0.C \
	src/TBL1.C \
	src/R3/Z31ACT.C \
	src/PLCHG.C \
	src/R3/MSNCCG.C \
	src/R3/TENTOU.C \
	src/R3/KAMA.C \
	src/PLPAT6.C \
	src/R3/ACT31B.C \
	src/R3/COL3B.C \
	src/R3/DEV31B.C \
	src/R3/EDTBL31B.C \
	src/R3/Z31BTBL.C \
	src/R3/ZONETBL3.C \
	src/R3/SCR31B.C \
	src/R3/MOVIE3.C \
	src/SAVE.C \
	src/R3/TOGEBL3B.C \
	src/ACTION.C

R31C_SOURCES := \
	src/ACTSET.C \
	src/DIRCOL.C \
	src/DUMMY.C \
	src/EDIT.C \
	src/EQU.C \
	src/ETC.C \
	src/FCOL.C \
	src/GAME.C \
	src/GOAL.C \
	src/IO.C \
	src/ITEM.C \
	src/PLAYER.C \
	src/PLAYSUB.C \
	src/RIDECHK.C \
	src/RING.C \
	src/SCORE.C \
	src/SCRCHK.C \
	src/SPRING.C \
	src/SUICIDE.C \
	src/ZONE.C \
	src/LOADER2.C \
	src/R3/BANPA.C \
	src/R3/BOBIN.C \
	src/R3/BOBINB.C \
	src/R3/COLI3.C \
	src/R3/ET3.C \
	src/R3/FRIEND3.C \
	src/R3/GA3.C \
	src/R3/PLAYSP3.C \
	src/R3/POCKET.C \
	src/R3/TRAP_R3.C \
	src/DLLMAIN.C \
	src/BMP.C \
	src/BLOCK.C \
	src/R3/MIRACLE.C \
	src/TBL0.C \
	src/TBL1.C \
	src/R3/Z31ACT.C \
	src/PLCHG.C \
	src/R3/TENTOU.C \
	src/R3/KAMA.C \
	src/PLPAT6.C \
	src/R3/ACT31B.C \
	src/R3/COL3C.C \
	src/R3/DEV31C.C \
	src/R3/EDTBL31C.C \
	src/R3/Z31CTBL.C \
	src/R3/MOVIE3.C \
	src/R3/ZONETBL3.C \
	src/R3/SCR31C.C \
	src/SAVE.C \
	src/R3/TOGEBL3D.C \
	src/ACTION.C

R31D_SOURCES := \
	src/ACTSET.C \
	src/DIRCOL.C \
	src/DUMMY.C \
	src/EDIT.C \
	src/EQU.C \
	src/ETC.C \
	src/FCOL.C \
	src/GAME.C \
	src/GOAL.C \
	src/IO.C \
	src/ITEM.C \
	src/PLAYER.C \
	src/PLAYSUB.C \
	src/RIDECHK.C \
	src/RING.C \
	src/SCORE.C \
	src/SCRCHK.C \
	src/SPRING.C \
	src/SUICIDE.C \
	src/ZONE.C \
	src/LOADER2.C \
	src/R3/BANPA.C \
	src/R3/BOBIN.C \
	src/R3/BOBINB.C \
	src/R3/COLI3.C \
	src/R3/ET3.C \
	src/R3/FRIEND3.C \
	src/R3/GA3.C \
	src/R3/PLAYSP3.C \
	src/R3/POCKET.C \
	src/R3/TRAP_R3.C \
	src/DLLMAIN.C \
	src/BMP.C \
	src/BLOCK.C \
	src/R3/MIRACLE.C \
	src/TBL0.C \
	src/TBL1.C \
	src/R3/Z31ACT.C \
	src/PLCHG.C \
	src/R3/TENTOU.C \
	src/R3/KAMA.C \
	src/PLPAT6.C \
	src/R3/ACT31B.C \
	src/R3/COL3D.C \
	src/R3/DEV31D.C \
	src/R3/EDTBL31D.C \
	src/R3/Z31DTBL.C \
	src/R3/ZONETBL3.C \
	src/R3/SCR31D.C \
	src/R3/MOVIE3.C \
	src/SAVE.C \
	src/R3/TOGEBL3D.C \
	src/ACTION.C

R32A_SOURCES := \
	src/ACTSET.C \
	src/DIRCOL.C \
	src/DUMMY.C \
	src/EDIT.C \
	src/EQU.C \
	src/ETC.C \
	src/FCOL.C \
	src/GAME.C \
	src/GOAL.C \
	src/IO.C \
	src/ITEM.C \
	src/PLAYER.C \
	src/PLAYSUB.C \
	src/RIDECHK.C \
	src/RING.C \
	src/SCORE.C \
	src/SCRCHK.C \
	src/SPRING.C \
	src/SUICIDE.C \
	src/ZONE.C \
	src/LOADER2.C \
	src/R3/BANPA.C \
	src/R3/BOBIN.C \
	src/R3/BOBINB.C \
	src/R3/COLI3.C \
	src/R3/ET3.C \
	src/R3/FRIEND3.C \
	src/R3/GA3.C \
	src/R3/PLAYSP3.C \
	src/R3/POCKET.C \
	src/R3/TOGEBL3A.C \
	src/R3/TRAP_R3.C \
	src/DLLMAIN.C \
	src/BMP.C \
	src/BLOCK.C \
	src/R3/MIRACLE.C \
	src/TBL0.C \
	src/TBL1.C \
	src/R3/COL3A.C \
	src/PLCHG.C \
	src/R3/MECASNC.C \
	src/R3/MSNCCG.C \
	src/R3/TENTOU.C \
	src/R3/KAMA.C \
	src/PLPAT6.C \
	src/R3/DEV32A.C \
	src/R3/EDTBL32A.C \
	src/R3/Z32ACT.C \
	src/R3/Z32ATBL.C \
	src/R3/ZONETBL3.C \
	src/R3/SCR32A.C \
	src/R3/ACT32A.C \
	src/R3/MOVIE3.C \
	src/SAVE.C \
	src/ACTION.C

R32B_SOURCES := \
	src/ACTSET.C \
	src/DIRCOL.C \
	src/DUMMY.C \
	src/EDIT.C \
	src/EQU.C \
	src/ETC.C \
	src/FCOL.C \
	src/GAME.C \
	src/GOAL.C \
	src/IO.C \
	src/ITEM.C \
	src/PLAYER.C \
	src/PLAYSUB.C \
	src/RIDECHK.C \
	src/RING.C \
	src/SCORE.C \
	src/SCRCHK.C \
	src/SPRING.C \
	src/SUICIDE.C \
	src/ZONE.C \
	src/LOADER2.C \
	src/R3/BANPA.C \
	src/R3/BOBIN.C \
	src/R3/BOBINB.C \
	src/R3/COLI3.C \
	src/R3/ET3.C \
	src/R3/FRIEND3.C \
	src/R3/GA3.C \
	src/R3/PLAYSP3.C \
	src/R3/POCKET.C \
	src/R3/TRAP_R3.C \
	src/DLLMAIN.C \
	src/BMP.C \
	src/BLOCK.C \
	src/R3/MIRACLE.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLCHG.C \
	src/R3/TENTOU.C \
	src/R3/KAMA.C \
	src/PLPAT6.C \
	src/R3/ACT32A.C \
	src/R3/Z32ACT.C \
	src/R3/COL3B.C \
	src/R3/DEV32B.C \
	src/R3/EDTBL32B.C \
	src/R3/Z32BTBL.C \
	src/R3/ZONETBL3.C \
	src/R3/SCR32B.C \
	src/SAVE.C \
	src/R3/MOVIE3.C \
	src/R3/TOGEBL3B.C \
	src/ACTION.C

R32C_SOURCES := \
	src/ACTSET.C \
	src/DIRCOL.C \
	src/DUMMY.C \
	src/EDIT.C \
	src/EQU.C \
	src/ETC.C \
	src/FCOL.C \
	src/GAME.C \
	src/GOAL.C \
	src/IO.C \
	src/ITEM.C \
	src/PLAYER.C \
	src/PLAYSUB.C \
	src/RIDECHK.C \
	src/RING.C \
	src/SCORE.C \
	src/SCRCHK.C \
	src/SPRING.C \
	src/SUICIDE.C \
	src/ZONE.C \
	src/LOADER2.C \
	src/R3/BANPA.C \
	src/R3/BOBIN.C \
	src/R3/BOBINB.C \
	src/R3/COLI3.C \
	src/R3/ET3.C \
	src/R3/FRIEND3.C \
	src/R3/GA3.C \
	src/R3/PLAYSP3.C \
	src/R3/POCKET.C \
	src/R3/TRAP_R3.C \
	src/DLLMAIN.C \
	src/BMP.C \
	src/BLOCK.C \
	src/R3/MIRACLE.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLCHG.C \
	src/R3/TENTOU.C \
	src/R3/KAMA.C \
	src/PLPAT6.C \
	src/R3/ACT32A.C \
	src/R3/Z32ACT.C \
	src/R3/COL3C.C \
	src/R3/DEV32C.C \
	src/R3/EDTBL32C.C \
	src/R3/Z32CTBL.C \
	src/R3/ZONETBL3.C \
	src/R3/SCR32C.C \
	src/SAVE.C \
	src/R3/MOVIE3.C \
	src/R3/TOGEBL3D.C \
	src/ACTION.C

R32D_SOURCES := \
	src/ACTSET.C \
	src/DIRCOL.C \
	src/DUMMY.C \
	src/EDIT.C \
	src/EQU.C \
	src/ETC.C \
	src/FCOL.C \
	src/GAME.C \
	src/GOAL.C \
	src/IO.C \
	src/ITEM.C \
	src/PLAYER.C \
	src/PLAYSUB.C \
	src/RIDECHK.C \
	src/RING.C \
	src/SCORE.C \
	src/SCRCHK.C \
	src/SPRING.C \
	src/SUICIDE.C \
	src/ZONE.C \
	src/LOADER2.C \
	src/R3/BANPA.C \
	src/R3/BOBIN.C \
	src/R3/BOBINB.C \
	src/R3/COLI3.C \
	src/R3/ET3.C \
	src/R3/FRIEND3.C \
	src/R3/GA3.C \
	src/R3/PLAYSP3.C \
	src/R3/POCKET.C \
	src/R3/TRAP_R3.C \
	src/DLLMAIN.C \
	src/BMP.C \
	src/BLOCK.C \
	src/R3/MIRACLE.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLCHG.C \
	src/R3/TENTOU.C \
	src/R3/KAMA.C \
	src/PLPAT6.C \
	src/R3/ACT32A.C \
	src/R3/Z32ACT.C \
	src/R3/COL3D.C \
	src/R3/DEV32D.C \
	src/R3/EDTBL32D.C \
	src/R3/Z32DTBL.C \
	src/R3/ZONETBL3.C \
	src/R3/SCR32D.C \
	src/R3/MOVIE3.C \
	src/SAVE.C \
	src/R3/TOGEBL3D.C \
	src/ACTION.C

R33C_SOURCES := \
	src/ACTSET.C \
	src/DIRCOL.C \
	src/DUMMY.C \
	src/EDIT.C \
	src/EQU.C \
	src/ETC.C \
	src/FCOL.C \
	src/GAME.C \
	src/GOAL.C \
	src/IO.C \
	src/ITEM.C \
	src/PLAYER.C \
	src/PLAYSUB.C \
	src/RIDECHK.C \
	src/RING.C \
	src/SCORE.C \
	src/SCRCHK.C \
	src/SPRING.C \
	src/SUICIDE.C \
	src/ZONE.C \
	src/LOADER2.C \
	src/R3/BANPA.C \
	src/R3/BOBIN.C \
	src/R3/BOBINB.C \
	src/R3/COLI3.C \
	src/R3/ET3.C \
	src/R3/FRIEND3.C \
	src/R3/GA3.C \
	src/R3/PLAYSP3.C \
	src/R3/POCKET.C \
	src/R3/TRAP_R3.C \
	src/DLLMAIN.C \
	src/BMP.C \
	src/BLOCK.C \
	src/R3/MIRACLE.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLCHG.C \
	src/R3/TENTOU.C \
	src/R3/KAMA.C \
	src/PLPAT6.C \
	src/R3/ACT33C.C \
	src/R3/COL3C.C \
	src/R3/DEV33C.C \
	src/R3/EDTBL33C.C \
	src/R3/Z33CTBL.C \
	src/R3/Z33ACT.C \
	src/R3/SIKAKE33.C \
	src/R3/LIGHT.C \
	src/R3/STOPPER.C \
	src/R3/BOSS_3.C \
	src/R3/GATE.C \
	src/R3/MOVIE3.C \
	src/R3/ROLLPLAT.C \
	src/R3/SCR33C.C \
	src/R3/ZONETBL3.C \
	src/SAVE.C \
	src/R3/TOGEBL3D.C \
	src/ACTION.C

R33D_SOURCES := \
	src/ACTSET.C \
	src/DIRCOL.C \
	src/DUMMY.C \
	src/EDIT.C \
	src/EQU.C \
	src/ETC.C \
	src/FCOL.C \
	src/GAME.C \
	src/GOAL.C \
	src/IO.C \
	src/ITEM.C \
	src/PLAYER.C \
	src/PLAYSUB.C \
	src/RIDECHK.C \
	src/RING.C \
	src/SCORE.C \
	src/SCRCHK.C \
	src/SPRING.C \
	src/SUICIDE.C \
	src/ZONE.C \
	src/LOADER2.C \
	src/R3/BANPA.C \
	src/R3/BOBIN.C \
	src/R3/BOBINB.C \
	src/R3/COLI3.C \
	src/R3/ET3.C \
	src/R3/FRIEND3.C \
	src/R3/GA3.C \
	src/R3/PLAYSP3.C \
	src/R3/POCKET.C \
	src/R3/TRAP_R3.C \
	src/DLLMAIN.C \
	src/BMP.C \
	src/BLOCK.C \
	src/R3/MIRACLE.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLCHG.C \
	src/R3/TENTOU.C \
	src/PLPAT6.C \
	src/R3/ACT33C.C \
	src/R3/Z33ACT.C \
	src/R3/COL3D.C \
	src/R3/DEV33D.C \
	src/R3/KAMA.C \
	src/R3/Z33DTBL.C \
	src/R3/ZONETBL3.C \
	src/R3/SCR33D.C \
	src/SAVE.C \
	src/R3/TOGEBL3D.C \
	src/R3/BOSS_3.C \
	src/R3/GATE.C \
	src/R3/MOVIE3.C \
	src/R3/SIKAKE33.C \
	src/R3/STOPPER.C \
	src/R3/LIGHT.C \
	src/R3/ROLLPLAT.C \
	src/R3/EDTBL33C.C \
	src/ACTION.C

R41A_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/RIDECHK.C \
	src/PLAYER.C \
	src/PLAYSP.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/R4/ACT41A.C \
	src/R4/COL4A.C \
	src/R4/COLI4.C \
	src/R4/DEV41A.C \
	src/R4/EDTBL41A.C \
	src/R4/ET4.C \
	src/R4/SCR41A.C \
	src/R4/SCRCHK4.C \
	src/R4/SW4.C \
	src/R4/SWBLK4.C \
	src/R4/Z41ACT.C \
	src/R4/Z41ATBL.C \
	src/R4/ZONETBL4.C \
	src/SUICIDE.C \
	src/R4/GAME4.C \
	src/R4/PLAYSUB4.C \
	src/R4/AWA.C \
	src/R4/HARID4.C \
	src/R4/TOBIRA4.C \
	src/R4/HARIR4.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/R4/WALLS.C \
	src/R4/WTBL41A.C \
	src/R4/FUNSUI4.C \
	src/R4/WALL1.C \
	src/R4/SCREW_A.C \
	src/R4/ESCAL4.C \
	src/R4/BRUNKO4.C \
	src/R4/TONBO.C \
	src/R4/AMENBO.C \
	src/R4/TAGAMEB4.C \
	src/R4/YAGO.C \
	src/R4/FRIEND4.C \
	src/R4/TEKKYU.C \
	src/R4/PLAYPAT4.C \
	src/R4/KUZURE4.C

R41B_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/RIDECHK.C \
	src/PLAYER.C \
	src/PLAYSP.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/R4/COLI4.C \
	src/R4/ET4.C \
	src/R4/SW4.C \
	src/R4/Z41ACT.C \
	src/R4/ZONETBL4.C \
	src/SUICIDE.C \
	src/R4/GAME4.C \
	src/R4/PLAYSUB4.C \
	src/R4/AWA.C \
	src/R4/HARIR4.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/R4/WALLS.C \
	src/R4/WALL1.C \
	src/R4/ESCAL4.C \
	src/R4/TONBO.C \
	src/R4/AMENBO.C \
	src/R4/TAGAMEB4.C \
	src/R4/YAGO.C \
	src/R4/FRIEND4.C \
	src/R4/LRBLK4.C \
	src/R4/COL4B.C \
	src/R4/SCR41B.C \
	src/SCRCHK.C \
	src/R4/ACT41B.C \
	src/R4/EDTBL41B.C \
	src/R4/DEV41B.C \
	src/R4/SCREW_A.C \
	src/R4/MOVIE4.C \
	src/R4/RBLK4.C \
	src/R4/Z41BTBL.C \
	src/R4/PLAYPAT4.C \
	src/R4/UDBLK4.C \
	src/R4/WTBL41B.C

R41C_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/RIDECHK.C \
	src/PLAYER.C \
	src/PLAYSP.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/R4/COLI4.C \
	src/R4/SW4.C \
	src/R4/Z41ACT.C \
	src/R4/ZONETBL4.C \
	src/SUICIDE.C \
	src/R4/GAME4.C \
	src/R4/PLAYSUB4.C \
	src/R4/AWA.C \
	src/R4/HARID4.C \
	src/R4/HARIR4.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/R4/WALL1.C \
	src/R4/ESCAL4.C \
	src/SCRCHK.C \
	src/R4/ACT41C.C \
	src/R4/COL4C.C \
	src/R4/DEV41C.C \
	src/R4/EDTBL41C.C \
	src/R4/SCREW_C.C \
	src/R4/TEKKYU4.C \
	src/R4/WTBL41C.C \
	src/R4/RENKETU4.C \
	src/R4/AMENBO.C \
	src/R4/FRIEND4.C \
	src/R4/TAGAMEB4.C \
	src/R4/TONBO.C \
	src/R4/YAGO.C \
	src/R4/SCR41C.C \
	src/R4/LRBLK4.C \
	src/R4/Z41CTBL.C \
	src/R4/PLAYPAT4.C \
	src/R4/UDBLK4.C \
	src/R4/SWGUN4.C

R41D_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/RIDECHK.C \
	src/PLAYER.C \
	src/PLAYSP.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/R4/COLI4.C \
	src/R4/SW4.C \
	src/R4/Z41ACT.C \
	src/R4/ZONETBL4.C \
	src/SUICIDE.C \
	src/R4/GAME4.C \
	src/R4/PLAYSUB4.C \
	src/R4/AWA.C \
	src/R4/HARID4.C \
	src/R4/HARIR4.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/R4/WALL1.C \
	src/R4/ESCAL4.C \
	src/SCRCHK.C \
	src/R4/ACT41C.C \
	src/R4/EDTBL41C.C \
	src/R4/SCREW_C.C \
	src/R4/TEKKYU4.C \
	src/R4/WTBL41C.C \
	src/R4/RENKETU4.C \
	src/R4/AMENBO.C \
	src/R4/FRIEND4.C \
	src/R4/TAGAMEB4.C \
	src/R4/TONBO.C \
	src/R4/YAGO.C \
	src/R4/COL4D.C \
	src/R4/SCR41D.C \
	src/R4/DEV41D.C \
	src/R4/Z41DTBL.C \
	src/R4/PLAYPAT4.C \
	src/R4/UDBLK4.C \
	src/R4/SWGUN4.C

R42A_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/RIDECHK.C \
	src/PLAYER.C \
	src/PLAYSP.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/R4/COL4A.C \
	src/R4/COLI4.C \
	src/R4/ET4.C \
	src/R4/SW4.C \
	src/R4/SWBLK4.C \
	src/R4/ZONETBL4.C \
	src/SUICIDE.C \
	src/R4/GAME4.C \
	src/R4/PLAYSUB4.C \
	src/R4/AWA.C \
	src/R4/TOBIRA4.C \
	src/R4/HARIR4.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/R4/WALLS.C \
	src/R4/WALL1.C \
	src/R4/SCREW_A.C \
	src/R4/ESCAL4.C \
	src/R4/BRUNKO4.C \
	src/R4/TONBO.C \
	src/R4/AMENBO.C \
	src/R4/TAGAMEB4.C \
	src/R4/YAGO.C \
	src/R4/FRIEND4.C \
	src/R4/LRBLK4.C \
	src/SCRCHK.C \
	src/R4/SCR42A.C \
	src/R4/DEV42A.C \
	src/R4/EDTBL42A.C \
	src/R4/Z42ATBL.C \
	src/R4/WTBL42A.C \
	src/R4/TOBIRAS4.C \
	src/R4/TEKKYU.C \
	src/R4/TEKKYU4.C \
	src/R4/OSUMIZU.C \
	src/R4/WALL4.C \
	src/R4/ACT42A.C \
	src/R4/UKIDAI.C \
	src/R4/PLAYPAT4.C \
	src/R4/Z42ACT.C \
	src/R4/KUZURE4.C \
	src/R4/TEKKYU1.C \
	src/R4/WALL42.C

R42B_SOURCES := \
	src/DIRCOL.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/R4/COL4B.C \
	src/ETC.C \
	src/FCOL.C \
	src/R4/GAME4.C \
	src/IO.C \
	src/SCRCHK.C \
	src/R4/ACT42B.C \
	src/PLAYER.C \
	src/R4/PLAYSUB4.C \
	src/PLAYSP.C \
	src/SAVE.C \
	src/R4/COLI4.C \
	src/EDIT.C \
	src/R4/EDTBL42B.C \
	src/ACTSET.C \
	src/R4/Z42ACT.C \
	src/RIDECHK.C \
	src/SPRING.C \
	src/RING.C \
	src/ITEM.C \
	src/SCORE.C \
	src/R4/AWA.C \
	src/GOAL.C \
	src/ZONE.C \
	src/R4/ZONETBL4.C \
	src/R4/WTBL42B.C \
	src/R4/HARIR4.C \
	src/R4/SW4.C \
	src/R4/TONBO.C \
	src/R4/AMENBO.C \
	src/R4/YAGO.C \
	src/R4/FRIEND4.C \
	src/R4/WALLS.C \
	src/R4/WALL1.C \
	src/R4/ET4.C \
	src/SUICIDE.C \
	src/R4/DEV42B.C \
	src/R4/MOVIE4.C \
	src/R4/Z42BTBL.C \
	src/R4/PLAYPAT4.C \
	src/R4/KUZURE4.C \
	src/ACTION.C \
	src/R4/RBLK4.C \
	src/R4/UDBLK4.C \
	src/R4/SCR42A.C \
	src/TBL0.C \
	src/TBL1.C \
	src/R4/SCREW_A.C \
	src/R4/TAGAMEB4.C \
	src/R4/LRBLK4.C \
	src/PLCHG.C

R42C_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/RIDECHK.C \
	src/PLAYER.C \
	src/PLAYSP.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/R4/COLI4.C \
	src/R4/SW4.C \
	src/R4/ZONETBL4.C \
	src/SUICIDE.C \
	src/R4/GAME4.C \
	src/R4/PLAYSUB4.C \
	src/R4/AWA.C \
	src/R4/HARIR4.C \
	src/PLCHG.C \
	src/R4/WALLS.C \
	src/R4/WALL1.C \
	src/R4/TONBO.C \
	src/R4/AMENBO.C \
	src/R4/TAGAMEB4.C \
	src/R4/YAGO.C \
	src/R4/FRIEND4.C \
	src/R4/LRBLK4.C \
	src/R4/TEKKYU.C \
	src/R4/ACT42C.C \
	src/R4/COL4C.C \
	src/R4/DEV42C.C \
	src/R4/EDTBL42C.C \
	src/R4/SCREW_C.C \
	src/R4/WTBL42C.C \
	src/R4/Z42ACT.C \
	src/R4/Z42CTBL.C \
	src/R4/PLAYPAT4.C \
	src/R4/KUZURE4.C \
	src/R4/BANEIWA.C \
	src/R4/DAID4.C \
	src/R4/TEKKYU1.C \
	src/ACTION.C \
	src/R4/SCR42C.C \
	src/R4/TOBIRAS4.C \
	src/R4/TOBIRA4.C \
	src/R4/TEKKYU4.C \
	src/TBL0.C \
	src/TBL1.C \
	src/R4/KOWASI4.C \
	src/R4/RENKETU4.C \
	src/R4/UDBLK4.C \
	src/SCRCHK.C

R42D_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/RIDECHK.C \
	src/PLAYER.C \
	src/PLAYSP.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/R4/COLI4.C \
	src/R4/SW4.C \
	src/R4/ZONETBL4.C \
	src/SUICIDE.C \
	src/R4/GAME4.C \
	src/R4/PLAYSUB4.C \
	src/R4/AWA.C \
	src/R4/TOBIRA4.C \
	src/R4/HARIR4.C \
	src/R4/WALLS.C \
	src/R4/WALL1.C \
	src/R4/TONBO.C \
	src/R4/AMENBO.C \
	src/R4/TAGAMEB4.C \
	src/R4/YAGO.C \
	src/R4/FRIEND4.C \
	src/R4/LRBLK4.C \
	src/R4/TEKKYU.C \
	src/R4/COL4D.C \
	src/R4/DEV42D.C \
	src/R4/TEKKYU4.C \
	src/R4/TOBIRAS4.C \
	src/R4/Z42DTBL.C \
	src/R4/PLAYPAT4.C \
	src/ACTION.C \
	src/PLCHG.C \
	src/TBL0.C \
	src/TBL1.C \
	src/R4/ACT42C.C \
	src/R4/EDTBL42C.C \
	src/R4/SCR42C.C \
	src/R4/BANEIWA.C \
	src/R4/KUZURE4.C \
	src/R4/KOWASI4.C \
	src/R4/DAID4.C \
	src/R4/SCREW_C.C \
	src/R4/TEKKYU1.C \
	src/R4/Z42ACT.C \
	src/R4/WTBL42C.C \
	src/SCRCHK.C \
	src/R4/RENKETU4.C \
	src/R4/UDBLK4.C

R43C_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/RIDECHK.C \
	src/PLAYER.C \
	src/PLAYSP.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/R4/COLI4.C \
	src/R4/SW4.C \
	src/R4/SWBLK4.C \
	src/R4/ZONETBL4.C \
	src/SUICIDE.C \
	src/R4/GAME4.C \
	src/R4/PLAYSUB4.C \
	src/R4/AWA.C \
	src/R4/WALLS.C \
	src/R4/WALL1.C \
	src/R4/SCREW_A.C \
	src/R4/TONBO.C \
	src/R4/FRIEND4.C \
	src/R4/TEKKYU.C \
	src/R4/ACT43C.C \
	src/R4/COL4C.C \
	src/R4/DEV43C.C \
	src/R4/EDTBL43C.C \
	src/R4/SCR43C.C \
	src/SCRCHK.C \
	src/R4/Z43ACT.C \
	src/R4/WALL4.C \
	src/R4/BOSS_4.C \
	src/R4/BOSS_4_2.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLCHG.C \
	src/R4/RENKETU4.C \
	src/R4/Z43CTBL.C \
	src/R4/WTBL43.C \
	src/R4/PLAYPAT4.C

R43D_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/RIDECHK.C \
	src/PLAYER.C \
	src/PLAYSP.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/R4/COLI4.C \
	src/R4/SW4.C \
	src/R4/SWBLK4.C \
	src/R4/ZONETBL4.C \
	src/SUICIDE.C \
	src/R4/GAME4.C \
	src/R4/PLAYSUB4.C \
	src/R4/AWA.C \
	src/R4/WALLS.C \
	src/R4/WALL1.C \
	src/R4/SCREW_A.C \
	src/R4/TONBO.C \
	src/R4/FRIEND4.C \
	src/R4/TEKKYU.C \
	src/R4/ACT43C.C \
	src/R4/EDTBL43C.C \
	src/SCRCHK.C \
	src/R4/Z43ACT.C \
	src/R4/WALL4.C \
	src/R4/BOSS_4.C \
	src/R4/BOSS_4_2.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLCHG.C \
	src/R4/RENKETU4.C \
	src/R4/WTBL43.C \
	src/R4/PLAYPAT4.C \
	src/R4/COL4D.C \
	src/R4/DEV43D.C \
	src/R4/SCR43D.C \
	src/R4/Z43DTBL.C

R51A_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/RIDECHK.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/R5/ACT51A.C \
	src/R5/COL5A.C \
	src/R5/COLI5.C \
	src/R5/DEV51A.C \
	src/R5/EDTBL5.C \
	src/R5/GAME5.C \
	src/R5/PLAYSP5.C \
	src/R5/SCR51A.C \
	src/R5/Z51ACT.C \
	src/R5/Z51ATBL.C \
	src/PLAYSUB.C \
	src/PLPAT6.C \
	src/SCRCHK.C \
	src/R5/ZONETBL5.C \
	src/R5/BELTSW5.C \
	src/R5/MOVIE5.C \
	src/R5/FRIEND5.C \
	src/R5/KEMUSI.C \
	src/R5/SHOOT5.C \
	src/R5/KUMO.C \
	src/R5/IWA5.C \
	src/R5/IWA5WAVE.C \
	src/R5/DENDEN.C \
	src/R5/HASHI5.C \
	src/R5/HARI5F.C \
	src/R5/HARIR5.C \
	src/R5/IWA5ROLL.C \
	src/R5/KOWASI5.C \
	src/R5/KUZURE5.C \
	src/R5/SASORI.C \
	src/R5/HASIRA5.C \
	src/R5/ET5.C \
	src/R5/BURANKO5.C \
	src/R5/DAI_RD5.C

R51B_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/RIDECHK.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/R5/COLI5.C \
	src/R5/EDTBL5.C \
	src/R5/GAME5.C \
	src/R5/PLAYSP5.C \
	src/R5/Z51ACT.C \
	src/PLAYSUB.C \
	src/PLPAT6.C \
	src/SCRCHK.C \
	src/R5/ZONETBL5.C \
	src/R5/BELTSW5.C \
	src/R5/MOVIE5.C \
	src/R5/FRIEND5.C \
	src/R5/KEMUSI.C \
	src/R5/SHOOT5.C \
	src/R5/KUMO.C \
	src/R5/IWA5.C \
	src/R5/IWA5WAVE.C \
	src/R5/DENDEN.C \
	src/R5/HASHI5.C \
	src/R5/ACT51A.C \
	src/R5/COL5B.C \
	src/R5/DEV51B.C \
	src/R5/SCR51B.C \
	src/R5/Z51BTBL.C \
	src/R5/HARI5F.C \
	src/R5/HARIR5.C \
	src/R5/KUZURE5.C \
	src/R5/KOWASI5.C \
	src/R5/HASIRA5.C \
	src/R5/SASORI.C \
	src/R5/DAI_RD5.C \
	src/R5/IWA5ROLL.C \
	src/R5/BURANKO5.C \
	src/R5/ET5.C

R51C_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/RIDECHK.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/R5/ACT51A.C \
	src/R5/COLI5.C \
	src/R5/EDTBL5.C \
	src/R5/GAME5.C \
	src/R5/PLAYSP5.C \
	src/R5/Z51ACT.C \
	src/PLAYSUB.C \
	src/PLPAT6.C \
	src/SCRCHK.C \
	src/R5/ZONETBL5.C \
	src/R5/BELTSW5.C \
	src/R5/MOVIE5.C \
	src/R5/FRIEND5.C \
	src/R5/KEMUSI.C \
	src/R5/SHOOT5.C \
	src/R5/KUMO.C \
	src/R5/IWA5.C \
	src/R5/IWA5WAVE.C \
	src/R5/DENDEN.C \
	src/R5/HASHI5.C \
	src/R5/COL5C.C \
	src/R5/DEV51C.C \
	src/R5/SCR51C.C \
	src/R5/Z51CTBL.C \
	src/R5/HARI5F.C \
	src/R5/HARIR5.C \
	src/R5/KOWASI5.C \
	src/R5/KUZURE5.C \
	src/R5/HASIRA5.C \
	src/R5/SASORI.C \
	src/R5/IWA5ROLL.C \
	src/R5/BURANKO5.C \
	src/R5/DAI_RD5.C \
	src/R5/ET5.C

R51D_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/RIDECHK.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/R5/ACT51A.C \
	src/R5/COLI5.C \
	src/R5/EDTBL5.C \
	src/R5/GAME5.C \
	src/R5/PLAYSP5.C \
	src/R5/Z51ACT.C \
	src/PLAYSUB.C \
	src/PLPAT6.C \
	src/SCRCHK.C \
	src/R5/ZONETBL5.C \
	src/R5/BELTSW5.C \
	src/R5/MOVIE5.C \
	src/R5/FRIEND5.C \
	src/R5/KEMUSI.C \
	src/R5/SHOOT5.C \
	src/R5/KUMO.C \
	src/R5/IWA5.C \
	src/R5/IWA5WAVE.C \
	src/R5/DENDEN.C \
	src/R5/HASHI5.C \
	src/R5/COL5D.C \
	src/R5/DEV51D.C \
	src/R5/SCR51D.C \
	src/R5/Z51DTBL.C \
	src/R5/HARI5F.C \
	src/R5/HARIR5.C \
	src/R5/KOWASI5.C \
	src/R5/KUZURE5.C \
	src/R5/HASIRA5.C \
	src/R5/SASORI.C \
	src/R5/IWA5ROLL.C \
	src/R5/BURANKO5.C \
	src/R5/DAI_RD5.C \
	src/R5/ET5.C

R52A_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/RIDECHK.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/R5/ACT51A.C \
	src/R5/COL5A.C \
	src/R5/COLI5.C \
	src/R5/EDTBL5.C \
	src/R5/GAME5.C \
	src/R5/PLAYSP5.C \
	src/PLAYSUB.C \
	src/PLPAT6.C \
	src/SCRCHK.C \
	src/R5/ZONETBL5.C \
	src/R5/BELTSW5.C \
	src/R5/MOVIE5.C \
	src/R5/FRIEND5.C \
	src/R5/KEMUSI.C \
	src/R5/SHOOT5.C \
	src/R5/KUMO.C \
	src/R5/IWA5.C \
	src/R5/IWA5WAVE.C \
	src/R5/DENDEN.C \
	src/R5/HASHI5.C \
	src/R5/HARI5F.C \
	src/R5/HARIR5.C \
	src/R5/IWA5ROLL.C \
	src/R5/KOWASI5.C \
	src/R5/KUZURE5.C \
	src/R5/SASORI.C \
	src/R5/HASIRA5.C \
	src/R5/ET5.C \
	src/R5/BURANKO5.C \
	src/R5/DAI_RD5.C \
	src/R5/DEV52A.C \
	src/R5/SCR52A.C \
	src/R5/Z52ACT.C \
	src/R5/Z52ATBL.C

R52B_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/RIDECHK.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/R5/COLI5.C \
	src/R5/EDTBL5.C \
	src/R5/GAME5.C \
	src/R5/PLAYSP5.C \
	src/PLAYSUB.C \
	src/PLPAT6.C \
	src/SCRCHK.C \
	src/R5/ZONETBL5.C \
	src/R5/BELTSW5.C \
	src/R5/MOVIE5.C \
	src/R5/FRIEND5.C \
	src/R5/KEMUSI.C \
	src/R5/SHOOT5.C \
	src/R5/KUMO.C \
	src/R5/IWA5.C \
	src/R5/IWA5WAVE.C \
	src/R5/DENDEN.C \
	src/R5/HASHI5.C \
	src/R5/ACT51A.C \
	src/R5/COL5B.C \
	src/R5/HARI5F.C \
	src/R5/HARIR5.C \
	src/R5/KUZURE5.C \
	src/R5/KOWASI5.C \
	src/R5/HASIRA5.C \
	src/R5/SASORI.C \
	src/R5/DAI_RD5.C \
	src/R5/IWA5ROLL.C \
	src/R5/BURANKO5.C \
	src/R5/ET5.C \
	src/R5/DEV52B.C \
	src/R5/SCR52B.C \
	src/R5/Z52ACT.C \
	src/R5/Z52BTBL.C

R52C_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/RIDECHK.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/R5/ACT51A.C \
	src/R5/COLI5.C \
	src/R5/EDTBL5.C \
	src/R5/GAME5.C \
	src/R5/PLAYSP5.C \
	src/PLAYSUB.C \
	src/PLPAT6.C \
	src/SCRCHK.C \
	src/R5/ZONETBL5.C \
	src/R5/BELTSW5.C \
	src/R5/MOVIE5.C \
	src/R5/FRIEND5.C \
	src/R5/KEMUSI.C \
	src/R5/SHOOT5.C \
	src/R5/KUMO.C \
	src/R5/IWA5.C \
	src/R5/IWA5WAVE.C \
	src/R5/DENDEN.C \
	src/R5/HASHI5.C \
	src/R5/COL5C.C \
	src/R5/HARI5F.C \
	src/R5/HARIR5.C \
	src/R5/KOWASI5.C \
	src/R5/KUZURE5.C \
	src/R5/HASIRA5.C \
	src/R5/SASORI.C \
	src/R5/IWA5ROLL.C \
	src/R5/BURANKO5.C \
	src/R5/DAI_RD5.C \
	src/R5/ET5.C \
	src/R5/DEV52C.C \
	src/R5/SCR52C.C \
	src/R5/Z52ACT.C \
	src/R5/Z52CTBL.C

R52D_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/RIDECHK.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/R5/ACT51A.C \
	src/R5/COLI5.C \
	src/R5/EDTBL5.C \
	src/R5/GAME5.C \
	src/R5/PLAYSP5.C \
	src/PLAYSUB.C \
	src/PLPAT6.C \
	src/SCRCHK.C \
	src/R5/ZONETBL5.C \
	src/R5/BELTSW5.C \
	src/R5/MOVIE5.C \
	src/R5/FRIEND5.C \
	src/R5/KEMUSI.C \
	src/R5/SHOOT5.C \
	src/R5/KUMO.C \
	src/R5/IWA5.C \
	src/R5/IWA5WAVE.C \
	src/R5/DENDEN.C \
	src/R5/HASHI5.C \
	src/R5/COL5D.C \
	src/R5/HARI5F.C \
	src/R5/HARIR5.C \
	src/R5/KOWASI5.C \
	src/R5/KUZURE5.C \
	src/R5/HASIRA5.C \
	src/R5/SASORI.C \
	src/R5/IWA5ROLL.C \
	src/R5/BURANKO5.C \
	src/R5/DAI_RD5.C \
	src/R5/ET5.C \
	src/R5/DEV52D.C \
	src/R5/SCR52D.C \
	src/R5/Z52ACT.C \
	src/R5/Z52DTBL.C

R53C_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/RIDECHK.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/R5/COLI5.C \
	src/R5/PLAYSP5.C \
	src/PLAYSUB.C \
	src/PLPAT6.C \
	src/SCRCHK.C \
	src/R5/ZONETBL5.C \
	src/R5/ACT53.C \
	src/R5/COL5C.C \
	src/R5/DEV53C.C \
	src/R5/SCR53C.C \
	src/R5/Z53ACT.C \
	src/R5/Z53CTBL.C \
	src/R5/KUZURE5.C \
	src/R5/SHOOT5.C \
	src/GAME.C \
	src/R5/DAI_RD5.C \
	src/R5/BOSS_5.C \
	src/R5/EDTBL53.C

R53D_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/RIDECHK.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/R5/COLI5.C \
	src/R5/PLAYSP5.C \
	src/PLAYSUB.C \
	src/PLPAT6.C \
	src/SCRCHK.C \
	src/R5/ZONETBL5.C \
	src/R5/ACT53.C \
	src/R5/Z53ACT.C \
	src/R5/KUZURE5.C \
	src/R5/SHOOT5.C \
	src/GAME.C \
	src/R5/DAI_RD5.C \
	src/R5/COL5D.C \
	src/R5/DEV53D.C \
	src/R5/SCR53D.C \
	src/R5/Z53DTBL.C \
	src/R5/BOSS_5.C \
	src/R5/EDTBL53.C

R61A_SOURCES := \
	src/ACTION.C \
	src/ACTSET.C \
	src/DIRCOL.C \
	src/DUMMY.C \
	src/EDIT.C \
	src/EQU.C \
	src/ETC.C \
	src/FCOL.C \
	src/GAME.C \
	src/GOAL.C \
	src/IO.C \
	src/ITEM.C \
	src/PLAYSUB.C \
	src/RIDECHK.C \
	src/RING.C \
	src/SCORE.C \
	src/SCRCHK.C \
	src/SPRING.C \
	src/SUICIDE.C \
	src/ZONE.C \
	src/LOADER2.C \
	src/DLLMAIN.C \
	src/BMP.C \
	src/BLOCK.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLPAT6.C \
	src/SAVE.C \
	src/R6/ACT61A.C \
	src/R6/COLI6.C \
	src/R6/EDTBL61A.C \
	src/R6/SW6.C \
	src/R6/TOBIRA6.C \
	src/R6/TRAP_R6.C \
	src/R6/KDAI6.C \
	src/R6/BEEM6.C \
	src/R6/BLOCK6.C \
	src/R6/COL6A.C \
	src/R6/DAIR6.C \
	src/R6/EGG6.C \
	src/R6/ET6.C \
	src/R6/FRIEND6.C \
	src/R6/HACHI6.C \
	src/R6/MINOMUSI.C \
	src/R6/MOVIE6.C \
	src/R6/PISTON6.C \
	src/R6/SCR61A.C \
	src/R6/SEMI.C \
	src/R6/SHOOT6.C \
	src/R6/TOBIDAI6.C \
	src/R6/UDBLK6.C \
	src/R6/Z61ACT.C \
	src/R6/Z61ATBL.C \
	src/R6/DEV61A.C \
	src/R6/BATTA.C \
	src/R6/SEESAW6.C \
	src/PLAYSP.C \
	src/PLAYER6.C \
	src/PLCHG6.C \
	src/R6/ZONETBL6.C

R61B_SOURCES := \
	src/ACTION.C \
	src/ACTSET.C \
	src/DIRCOL.C \
	src/DUMMY.C \
	src/EDIT.C \
	src/EQU.C \
	src/ETC.C \
	src/FCOL.C \
	src/GAME.C \
	src/GOAL.C \
	src/IO.C \
	src/ITEM.C \
	src/PLAYSUB.C \
	src/RIDECHK.C \
	src/RING.C \
	src/SCORE.C \
	src/SCRCHK.C \
	src/SPRING.C \
	src/SUICIDE.C \
	src/ZONE.C \
	src/LOADER2.C \
	src/DLLMAIN.C \
	src/BMP.C \
	src/BLOCK.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLPAT6.C \
	src/SAVE.C \
	src/R6/COLI6.C \
	src/R6/SW6.C \
	src/R6/TOBIRA6.C \
	src/R6/TRAP_R6.C \
	src/R6/KDAI6.C \
	src/R6/BEEM6.C \
	src/R6/BLOCK6.C \
	src/R6/DAIR6.C \
	src/R6/EGG6.C \
	src/R6/ET6.C \
	src/R6/FRIEND6.C \
	src/R6/HACHI6.C \
	src/R6/MINOMUSI.C \
	src/R6/MOVIE6.C \
	src/R6/PISTON6.C \
	src/R6/SEMI.C \
	src/R6/SHOOT6.C \
	src/R6/TOBIDAI6.C \
	src/R6/UDBLK6.C \
	src/R6/BATTA.C \
	src/R6/SEESAW6.C \
	src/PLAYSP.C \
	src/PLAYER6.C \
	src/R6/COL6B.C \
	src/R6/SCR61B.C \
	src/R6/DEV61B.C \
	src/R6/ACT61A.C \
	src/R6/EDTBL61B.C \
	src/R6/Z61BTBL.C \
	src/R6/Z61ACT.C \
	src/PLCHG6.C \
	src/R6/ZONETBL6.C

R61C_SOURCES := \
	src/ACTION.C \
	src/ACTSET.C \
	src/DIRCOL.C \
	src/DUMMY.C \
	src/EDIT.C \
	src/EQU.C \
	src/ETC.C \
	src/FCOL.C \
	src/GAME.C \
	src/GOAL.C \
	src/IO.C \
	src/ITEM.C \
	src/PLAYSUB.C \
	src/RIDECHK.C \
	src/RING.C \
	src/SCORE.C \
	src/SCRCHK.C \
	src/SPRING.C \
	src/SUICIDE.C \
	src/ZONE.C \
	src/LOADER2.C \
	src/DLLMAIN.C \
	src/BMP.C \
	src/BLOCK.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLPAT6.C \
	src/SAVE.C \
	src/R6/ACT61A.C \
	src/R6/COLI6.C \
	src/R6/SW6.C \
	src/R6/TOBIRA6.C \
	src/R6/TRAP_R6.C \
	src/R6/KDAI6.C \
	src/R6/BEEM6.C \
	src/R6/BLOCK6.C \
	src/R6/DAIR6.C \
	src/R6/EGG6.C \
	src/R6/ET6.C \
	src/R6/FRIEND6.C \
	src/R6/HACHI6.C \
	src/R6/MINOMUSI.C \
	src/R6/MOVIE6.C \
	src/R6/PISTON6.C \
	src/R6/SEMI.C \
	src/R6/SHOOT6.C \
	src/R6/TOBIDAI6.C \
	src/R6/UDBLK6.C \
	src/R6/Z61ACT.C \
	src/R6/BATTA.C \
	src/R6/SEESAW6.C \
	src/PLAYSP.C \
	src/PLAYER6.C \
	src/R6/COL6C.C \
	src/R6/DEV61C.C \
	src/R6/SCR61C.C \
	src/R6/Z61CTBL.C \
	src/PLCHG6.C \
	src/R6/EDTBL61C.C \
	src/R6/ZONETBL6.C

R61D_SOURCES := \
	src/ACTION.C \
	src/ACTSET.C \
	src/DIRCOL.C \
	src/DUMMY.C \
	src/EDIT.C \
	src/EQU.C \
	src/ETC.C \
	src/FCOL.C \
	src/GAME.C \
	src/GOAL.C \
	src/IO.C \
	src/ITEM.C \
	src/PLAYSUB.C \
	src/RIDECHK.C \
	src/RING.C \
	src/SCORE.C \
	src/SCRCHK.C \
	src/SPRING.C \
	src/SUICIDE.C \
	src/ZONE.C \
	src/LOADER2.C \
	src/DLLMAIN.C \
	src/BMP.C \
	src/BLOCK.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLPAT6.C \
	src/SAVE.C \
	src/R6/ACT61A.C \
	src/R6/COLI6.C \
	src/R6/SW6.C \
	src/R6/TOBIRA6.C \
	src/R6/TRAP_R6.C \
	src/R6/KDAI6.C \
	src/R6/BEEM6.C \
	src/R6/BLOCK6.C \
	src/R6/DAIR6.C \
	src/R6/EGG6.C \
	src/R6/ET6.C \
	src/R6/FRIEND6.C \
	src/R6/HACHI6.C \
	src/R6/MINOMUSI.C \
	src/R6/MOVIE6.C \
	src/R6/PISTON6.C \
	src/R6/SEMI.C \
	src/R6/SHOOT6.C \
	src/R6/TOBIDAI6.C \
	src/R6/UDBLK6.C \
	src/R6/Z61ACT.C \
	src/R6/BATTA.C \
	src/R6/SEESAW6.C \
	src/PLAYSP.C \
	src/PLAYER6.C \
	src/R6/COL6D.C \
	src/R6/DEV61D.C \
	src/R6/SCR61D.C \
	src/R6/Z61DTBL.C \
	src/PLCHG6.C \
	src/R6/EDTBL61C.C \
	src/R6/ZONETBL6.C

R62A_SOURCES := \
	src/ACTION.C \
	src/ACTSET.C \
	src/DIRCOL.C \
	src/DUMMY.C \
	src/EDIT.C \
	src/EQU.C \
	src/ETC.C \
	src/FCOL.C \
	src/GAME.C \
	src/GOAL.C \
	src/IO.C \
	src/ITEM.C \
	src/PLAYSUB.C \
	src/RIDECHK.C \
	src/RING.C \
	src/SCORE.C \
	src/SCRCHK.C \
	src/SPRING.C \
	src/SUICIDE.C \
	src/ZONE.C \
	src/LOADER2.C \
	src/DLLMAIN.C \
	src/BMP.C \
	src/BLOCK.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLPAT6.C \
	src/SAVE.C \
	src/R6/COLI6.C \
	src/R6/SW6.C \
	src/R6/TOBIRA6.C \
	src/R6/TRAP_R6.C \
	src/R6/KDAI6.C \
	src/R6/BEEM6.C \
	src/R6/BLOCK6.C \
	src/R6/DAIR6.C \
	src/R6/EGG6.C \
	src/R6/ET6.C \
	src/R6/FRIEND6.C \
	src/R6/HACHI6.C \
	src/R6/MINOMUSI.C \
	src/R6/MOVIE6.C \
	src/R6/PISTON6.C \
	src/R6/SEMI.C \
	src/R6/SHOOT6.C \
	src/R6/TOBIDAI6.C \
	src/R6/UDBLK6.C \
	src/R6/BATTA.C \
	src/R6/SEESAW6.C \
	src/PLAYSP.C \
	src/PLAYER6.C \
	src/R6/EDTBL62A.C \
	src/R6/Z62ACT.C \
	src/R6/Z62ATBL.C \
	src/R6/COL6A.C \
	src/R6/TOGEBL6A.C \
	src/R6/DEV62A.C \
	src/R6/SCR62A.C \
	src/PLCHG6.C \
	src/R6/ACT62A.C \
	src/R6/ZONETBL6.C

R62B_SOURCES := \
	src/ACTION.C \
	src/ACTSET.C \
	src/DIRCOL.C \
	src/DUMMY.C \
	src/EDIT.C \
	src/EQU.C \
	src/ETC.C \
	src/FCOL.C \
	src/GAME.C \
	src/GOAL.C \
	src/IO.C \
	src/ITEM.C \
	src/PLAYSUB.C \
	src/RIDECHK.C \
	src/RING.C \
	src/SCORE.C \
	src/SCRCHK.C \
	src/SPRING.C \
	src/SUICIDE.C \
	src/ZONE.C \
	src/LOADER2.C \
	src/DLLMAIN.C \
	src/BMP.C \
	src/BLOCK.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLPAT6.C \
	src/SAVE.C \
	src/R6/COLI6.C \
	src/R6/SW6.C \
	src/R6/TOBIRA6.C \
	src/R6/TRAP_R6.C \
	src/R6/KDAI6.C \
	src/R6/BEEM6.C \
	src/R6/BLOCK6.C \
	src/R6/DAIR6.C \
	src/R6/EGG6.C \
	src/R6/ET6.C \
	src/R6/FRIEND6.C \
	src/R6/HACHI6.C \
	src/R6/MINOMUSI.C \
	src/R6/MOVIE6.C \
	src/R6/PISTON6.C \
	src/R6/SEMI.C \
	src/R6/SHOOT6.C \
	src/R6/TOBIDAI6.C \
	src/R6/UDBLK6.C \
	src/R6/BATTA.C \
	src/R6/SEESAW6.C \
	src/PLAYSP.C \
	src/PLAYER6.C \
	src/R6/ACT62A.C \
	src/R6/COL6B.C \
	src/R6/DEV62B.C \
	src/R6/EDTBL62B.C \
	src/R6/SCR62B.C \
	src/R6/Z62BTBL.C \
	src/R6/Z62ACT.C \
	src/R6/TOGEBL6B.C \
	src/PLCHG6.C \
	src/R6/ZONETBL6.C

R62C_SOURCES := \
	src/ACTION.C \
	src/ACTSET.C \
	src/DIRCOL.C \
	src/DUMMY.C \
	src/EDIT.C \
	src/EQU.C \
	src/ETC.C \
	src/FCOL.C \
	src/GAME.C \
	src/GOAL.C \
	src/IO.C \
	src/ITEM.C \
	src/PLAYSUB.C \
	src/RIDECHK.C \
	src/RING.C \
	src/SCORE.C \
	src/SCRCHK.C \
	src/SPRING.C \
	src/SUICIDE.C \
	src/ZONE.C \
	src/LOADER2.C \
	src/DLLMAIN.C \
	src/BMP.C \
	src/BLOCK.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLPAT6.C \
	src/SAVE.C \
	src/R6/COLI6.C \
	src/R6/SW6.C \
	src/R6/TOBIRA6.C \
	src/R6/TRAP_R6.C \
	src/R6/KDAI6.C \
	src/R6/BEEM6.C \
	src/R6/BLOCK6.C \
	src/R6/DAIR6.C \
	src/R6/EGG6.C \
	src/R6/ET6.C \
	src/R6/FRIEND6.C \
	src/R6/HACHI6.C \
	src/R6/MINOMUSI.C \
	src/R6/MOVIE6.C \
	src/R6/PISTON6.C \
	src/R6/SEMI.C \
	src/R6/SHOOT6.C \
	src/R6/TOBIDAI6.C \
	src/R6/UDBLK6.C \
	src/R6/BATTA.C \
	src/R6/SEESAW6.C \
	src/PLAYSP.C \
	src/PLAYER6.C \
	src/R6/ACT62A.C \
	src/R6/COL6C.C \
	src/R6/DEV62C.C \
	src/R6/EDTBL62C.C \
	src/R6/Z62ACT.C \
	src/R6/Z62CTBL.C \
	src/R6/TOGEBL6D.C \
	src/R6/SCR62C.C \
	src/PLCHG6.C \
	src/R6/ZONETBL6.C

R62D_SOURCES := \
	src/ACTION.C \
	src/ACTSET.C \
	src/DIRCOL.C \
	src/DUMMY.C \
	src/EDIT.C \
	src/EQU.C \
	src/ETC.C \
	src/FCOL.C \
	src/GAME.C \
	src/GOAL.C \
	src/IO.C \
	src/ITEM.C \
	src/PLAYSUB.C \
	src/RIDECHK.C \
	src/RING.C \
	src/SCORE.C \
	src/SCRCHK.C \
	src/SPRING.C \
	src/SUICIDE.C \
	src/ZONE.C \
	src/LOADER2.C \
	src/DLLMAIN.C \
	src/BMP.C \
	src/BLOCK.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLPAT6.C \
	src/SAVE.C \
	src/R6/COLI6.C \
	src/R6/SW6.C \
	src/R6/TOBIRA6.C \
	src/R6/TRAP_R6.C \
	src/R6/KDAI6.C \
	src/R6/BEEM6.C \
	src/R6/BLOCK6.C \
	src/R6/DAIR6.C \
	src/R6/EGG6.C \
	src/R6/ET6.C \
	src/R6/FRIEND6.C \
	src/R6/HACHI6.C \
	src/R6/MINOMUSI.C \
	src/R6/MOVIE6.C \
	src/R6/PISTON6.C \
	src/R6/SEMI.C \
	src/R6/SHOOT6.C \
	src/R6/TOBIDAI6.C \
	src/R6/UDBLK6.C \
	src/R6/BATTA.C \
	src/R6/SEESAW6.C \
	src/PLAYSP.C \
	src/PLAYER6.C \
	src/R6/COL6D.C \
	src/R6/ACT62A.C \
	src/R6/DEV62D.C \
	src/R6/EDTBL62D.C \
	src/R6/SCR62D.C \
	src/R6/TOGEBL6D.C \
	src/R6/Z62ACT.C \
	src/R6/Z62DTBL.C \
	src/PLCHG6.C \
	src/R6/ZONETBL6.C

R63C_SOURCES := \
	src/ACTION.C \
	src/ACTSET.C \
	src/DIRCOL.C \
	src/DUMMY.C \
	src/EDIT.C \
	src/EQU.C \
	src/ETC.C \
	src/FCOL.C \
	src/GAME.C \
	src/GOAL.C \
	src/IO.C \
	src/ITEM.C \
	src/PLAYSUB.C \
	src/RIDECHK.C \
	src/RING.C \
	src/SCORE.C \
	src/SCRCHK.C \
	src/SPRING.C \
	src/SUICIDE.C \
	src/ZONE.C \
	src/LOADER2.C \
	src/DLLMAIN.C \
	src/BMP.C \
	src/BLOCK.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLPAT6.C \
	src/SAVE.C \
	src/R6/COLI6.C \
	src/R6/SW6.C \
	src/R6/TOBIRA6.C \
	src/R6/TRAP_R6.C \
	src/R6/KDAI6.C \
	src/R6/BEEM6.C \
	src/R6/BLOCK6.C \
	src/R6/DAIR6.C \
	src/R6/EGG6.C \
	src/R6/ET6.C \
	src/R6/FRIEND6.C \
	src/R6/MOVIE6.C \
	src/R6/PISTON6.C \
	src/R6/SHOOT6.C \
	src/R6/TOBIDAI6.C \
	src/R6/UDBLK6.C \
	src/R6/SEESAW6.C \
	src/PLAYSP.C \
	src/PLAYER6.C \
	src/R6/COL6C.C \
	src/R6/TOGEBL6D.C \
	src/PLCHG6.C \
	src/R6/ACT63.C \
	src/R6/DEV63C.C \
	src/R6/EDTBL63C.C \
	src/R6/SCR63C.C \
	src/R6/Z63CTBL.C \
	src/R6/Z63ACT.C \
	src/R6/BOSS_6.C \
	src/R6/ZONETBL6.C

R63D_SOURCES := \
	src/ACTION.C \
	src/ACTSET.C \
	src/DIRCOL.C \
	src/DUMMY.C \
	src/EDIT.C \
	src/EQU.C \
	src/ETC.C \
	src/FCOL.C \
	src/GAME.C \
	src/GOAL.C \
	src/IO.C \
	src/ITEM.C \
	src/PLAYSUB.C \
	src/RIDECHK.C \
	src/RING.C \
	src/SCORE.C \
	src/SCRCHK.C \
	src/SPRING.C \
	src/SUICIDE.C \
	src/ZONE.C \
	src/LOADER2.C \
	src/DLLMAIN.C \
	src/BMP.C \
	src/BLOCK.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLPAT6.C \
	src/SAVE.C \
	src/R6/COLI6.C \
	src/R6/SW6.C \
	src/R6/TOBIRA6.C \
	src/R6/TRAP_R6.C \
	src/R6/KDAI6.C \
	src/R6/BEEM6.C \
	src/R6/BLOCK6.C \
	src/R6/DAIR6.C \
	src/R6/EGG6.C \
	src/R6/ET6.C \
	src/R6/FRIEND6.C \
	src/R6/MOVIE6.C \
	src/R6/PISTON6.C \
	src/R6/SHOOT6.C \
	src/R6/TOBIDAI6.C \
	src/R6/UDBLK6.C \
	src/R6/SEESAW6.C \
	src/PLAYSP.C \
	src/PLAYER6.C \
	src/R6/TOGEBL6D.C \
	src/PLCHG6.C \
	src/R6/ACT63.C \
	src/R6/Z63ACT.C \
	src/R6/BOSS_6.C \
	src/R6/COL6D.C \
	src/R6/DEV63D.C \
	src/R6/SCR63D.C \
	src/R6/EDTBL63C.C \
	src/R6/Z63DTBL.C \
	src/R6/ZONETBL6.C

R71A_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLAYSUB.C \
	src/PLPAT6.C \
	src/SCRCHK.C \
	src/GAME.C \
	src/R7/ACT71A.C \
	src/R7/CHGBAN.C \
	src/R7/CHGWALL7.C \
	src/R7/COL7A.C \
	src/R7/COLI7.C \
	src/R7/DEV71A.C \
	src/R7/EDTBL7.C \
	src/R7/PLAYSP7.C \
	src/R7/SCR71A.C \
	src/R7/Z71ACT.C \
	src/R7/Z71ATBL.C \
	src/R7/ZONETBL7.C \
	src/RIDECHK.C \
	src/R7/WALL7.C \
	src/R7/TEKKYU7.C \
	src/R7/TEKKYU7J.C \
	src/R7/SLIGHT7.C \
	src/R7/FRIEND7.C \
	src/R7/BRANKO7.C \
	src/R7/ET7.C \
	src/R7/KANABUN.C \
	src/R7/DANGO7.C \
	src/R7/MOVIE7.C \
	src/R7/KABASIRA.C \
	src/R7/HOTARU7.C

R71B_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLAYSUB.C \
	src/PLPAT6.C \
	src/SCRCHK.C \
	src/GAME.C \
	src/R7/ACT71A.C \
	src/R7/CHGBAN.C \
	src/R7/CHGWALL7.C \
	src/R7/COLI7.C \
	src/R7/EDTBL7.C \
	src/R7/PLAYSP7.C \
	src/R7/Z71ACT.C \
	src/R7/ZONETBL7.C \
	src/RIDECHK.C \
	src/R7/WALL7.C \
	src/R7/TEKKYU7.C \
	src/R7/TEKKYU7J.C \
	src/R7/SLIGHT7.C \
	src/R7/FRIEND7.C \
	src/R7/BRANKO7.C \
	src/R7/ET7.C \
	src/R7/KANABUN.C \
	src/R7/DANGO7.C \
	src/R7/MOVIE7.C \
	src/R7/COL7B.C \
	src/R7/SCR71B.C \
	src/R7/DEV71B.C \
	src/R7/Z71BTBL.C \
	src/R7/HOTARU7.C \
	src/R7/KABASIRA.C

R71C_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLAYSUB.C \
	src/PLPAT6.C \
	src/SCRCHK.C \
	src/GAME.C \
	src/R7/ACT71A.C \
	src/R7/CHGBAN.C \
	src/R7/CHGWALL7.C \
	src/R7/COLI7.C \
	src/R7/EDTBL7.C \
	src/R7/PLAYSP7.C \
	src/R7/Z71ACT.C \
	src/R7/ZONETBL7.C \
	src/RIDECHK.C \
	src/R7/WALL7.C \
	src/R7/TEKKYU7.C \
	src/R7/TEKKYU7J.C \
	src/R7/SLIGHT7.C \
	src/R7/FRIEND7.C \
	src/R7/BRANKO7.C \
	src/R7/ET7.C \
	src/R7/KANABUN.C \
	src/R7/DANGO7.C \
	src/R7/MOVIE7.C \
	src/R7/COL7C.C \
	src/R7/Z71CTBL.C \
	src/R7/SCR71C.C \
	src/R7/DEV71C.C \
	src/R7/KABASIRA.C \
	src/R7/HOTARU7.C

R71D_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLAYSUB.C \
	src/PLPAT6.C \
	src/SCRCHK.C \
	src/GAME.C \
	src/R7/ACT71A.C \
	src/R7/CHGBAN.C \
	src/R7/CHGWALL7.C \
	src/R7/COLI7.C \
	src/R7/EDTBL7.C \
	src/R7/PLAYSP7.C \
	src/R7/Z71ACT.C \
	src/R7/ZONETBL7.C \
	src/RIDECHK.C \
	src/R7/WALL7.C \
	src/R7/TEKKYU7.C \
	src/R7/TEKKYU7J.C \
	src/R7/SLIGHT7.C \
	src/R7/FRIEND7.C \
	src/R7/BRANKO7.C \
	src/R7/ET7.C \
	src/R7/KANABUN.C \
	src/R7/DANGO7.C \
	src/R7/MOVIE7.C \
	src/R7/COL7D.C \
	src/R7/DEV71D.C \
	src/R7/SCR71D.C \
	src/R7/Z71DTBL.C \
	src/R7/HOTARU7.C \
	src/R7/KABASIRA.C

R72A_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLAYSUB.C \
	src/PLPAT6.C \
	src/SCRCHK.C \
	src/GAME.C \
	src/R7/ACT71A.C \
	src/R7/CHGBAN.C \
	src/R7/CHGWALL7.C \
	src/R7/COL7A.C \
	src/R7/COLI7.C \
	src/R7/EDTBL7.C \
	src/R7/PLAYSP7.C \
	src/R7/ZONETBL7.C \
	src/RIDECHK.C \
	src/R7/WALL7.C \
	src/R7/TEKKYU7.C \
	src/R7/TEKKYU7J.C \
	src/R7/SLIGHT7.C \
	src/R7/FRIEND7.C \
	src/R7/BRANKO7.C \
	src/R7/ET7.C \
	src/R7/KANABUN.C \
	src/R7/DANGO7.C \
	src/R7/MOVIE7.C \
	src/R7/SCR72A.C \
	src/R7/Z72ACT.C \
	src/R7/DEV72A.C \
	src/R7/Z72ATBL.C \
	src/R7/HOTARU7.C \
	src/R7/KABASIRA.C

R72B_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLAYSUB.C \
	src/PLPAT6.C \
	src/SCRCHK.C \
	src/GAME.C \
	src/R7/ACT71A.C \
	src/R7/CHGBAN.C \
	src/R7/CHGWALL7.C \
	src/R7/COLI7.C \
	src/R7/EDTBL7.C \
	src/R7/PLAYSP7.C \
	src/R7/ZONETBL7.C \
	src/RIDECHK.C \
	src/R7/WALL7.C \
	src/R7/TEKKYU7.C \
	src/R7/TEKKYU7J.C \
	src/R7/SLIGHT7.C \
	src/R7/FRIEND7.C \
	src/R7/BRANKO7.C \
	src/R7/ET7.C \
	src/R7/KANABUN.C \
	src/R7/DANGO7.C \
	src/R7/MOVIE7.C \
	src/R7/Z72ACT.C \
	src/R7/COL7B.C \
	src/R7/DEV72B.C \
	src/R7/SCR72B.C \
	src/R7/Z72BTBL.C \
	src/R7/HOTARU7.C \
	src/R7/KABASIRA.C

R72C_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLAYSUB.C \
	src/PLPAT6.C \
	src/SCRCHK.C \
	src/GAME.C \
	src/R7/ACT71A.C \
	src/R7/CHGBAN.C \
	src/R7/CHGWALL7.C \
	src/R7/COLI7.C \
	src/R7/EDTBL7.C \
	src/R7/PLAYSP7.C \
	src/R7/ZONETBL7.C \
	src/RIDECHK.C \
	src/R7/WALL7.C \
	src/R7/TEKKYU7.C \
	src/R7/TEKKYU7J.C \
	src/R7/SLIGHT7.C \
	src/R7/FRIEND7.C \
	src/R7/BRANKO7.C \
	src/R7/ET7.C \
	src/R7/KANABUN.C \
	src/R7/DANGO7.C \
	src/R7/MOVIE7.C \
	src/R7/Z72ACT.C \
	src/R7/COL7C.C \
	src/R7/DEV72C.C \
	src/R7/SCR72C.C \
	src/R7/Z72CTBL.C \
	src/R7/HOTARU7.C \
	src/R7/KABASIRA.C

R72D_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLAYSUB.C \
	src/PLPAT6.C \
	src/SCRCHK.C \
	src/GAME.C \
	src/R7/ACT71A.C \
	src/R7/CHGBAN.C \
	src/R7/CHGWALL7.C \
	src/R7/COLI7.C \
	src/R7/EDTBL7.C \
	src/R7/PLAYSP7.C \
	src/R7/ZONETBL7.C \
	src/RIDECHK.C \
	src/R7/WALL7.C \
	src/R7/TEKKYU7.C \
	src/R7/TEKKYU7J.C \
	src/R7/SLIGHT7.C \
	src/R7/FRIEND7.C \
	src/R7/BRANKO7.C \
	src/R7/ET7.C \
	src/R7/KANABUN.C \
	src/R7/DANGO7.C \
	src/R7/MOVIE7.C \
	src/R7/Z72ACT.C \
	src/R7/COL7D.C \
	src/R7/DEV72D.C \
	src/R7/SCR72D.C \
	src/R7/Z72DTBL.C \
	src/R7/HOTARU7.C \
	src/R7/KABASIRA.C

R73C_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLAYSUB.C \
	src/PLPAT6.C \
	src/SCRCHK.C \
	src/GAME.C \
	src/R7/CHGBAN.C \
	src/R7/CHGWALL7.C \
	src/R7/COLI7.C \
	src/R7/PLAYSP7.C \
	src/R7/ZONETBL7.C \
	src/RIDECHK.C \
	src/R7/WALL7.C \
	src/R7/TEKKYU7.C \
	src/R7/TEKKYU7J.C \
	src/R7/FRIEND7.C \
	src/R7/BRANKO7.C \
	src/R7/MOVIE7.C \
	src/R7/SCR73C.C \
	src/R7/ACT73.C \
	src/R7/EDTBL73.C \
	src/R7/Z73ACT.C \
	src/R7/GAITOU73.C \
	src/R7/WALL73.C \
	src/R7/EMIE7.C \
	src/R7/COL7C.C \
	src/R7/Z73CTBL.C \
	src/R7/DEV73C.C \
	src/R7/HARI73.C \
	src/R7/EMIE7CG.C \
	src/R7/BOSS_7.C

R73D_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLAYSUB.C \
	src/PLPAT6.C \
	src/SCRCHK.C \
	src/GAME.C \
	src/R7/CHGBAN.C \
	src/R7/CHGWALL7.C \
	src/R7/COLI7.C \
	src/R7/PLAYSP7.C \
	src/R7/ZONETBL7.C \
	src/RIDECHK.C \
	src/R7/WALL7.C \
	src/R7/TEKKYU7.C \
	src/R7/TEKKYU7J.C \
	src/R7/FRIEND7.C \
	src/R7/BRANKO7.C \
	src/R7/MOVIE7.C \
	src/R7/ACT73.C \
	src/R7/EDTBL73.C \
	src/R7/Z73ACT.C \
	src/R7/GAITOU73.C \
	src/R7/WALL73.C \
	src/R7/EMIE7.C \
	src/R7/COL7D.C \
	src/R7/SCR73D.C \
	src/R7/DEV73D.C \
	src/R7/Z73DTBL.C \
	src/R7/HARI73.C \
	src/R7/EMIE7CG.C \
	src/R7/BOSS_7.C

R81A_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLAYSUB.C \
	src/GAME.C \
	src/RIDECHK.C \
	src/PLPAT8.C \
	src/R8/ACT81A.C \
	src/R8/COL8A.C \
	src/R8/COLI8.C \
	src/R8/DEV81A.C \
	src/R8/EDTBL81.C \
	src/PLAYSP.C \
	src/R8/SCR81A.C \
	src/R8/Z81ACT.C \
	src/R8/Z81ATBL.C \
	src/R8/ZONETBL8.C \
	src/R8/TRAP_R81.C \
	src/R8/KABUTO8.C \
	src/R8/KOMA8.C \
	src/R8/SW8.C \
	src/R8/OKUSIESO.C \
	src/R8/TOBIRA.C \
	src/R8/KONBEA.C \
	src/R8/SCARAB.C \
	src/R8/KUZURE8.C \
	src/R8/NOKOGIRI.C \
	src/R8/PROPERA8.C \
	src/R8/DAI8.C \
	src/R8/HACHI8.C \
	src/R8/ET8.C \
	src/R8/FRIEND8.C \
	src/R8/PISTON.C \
	src/R8/HAGURUMA.C \
	src/R8/SCRCHK8.C

R81B_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLAYSUB.C \
	src/GAME.C \
	src/RIDECHK.C \
	src/PLPAT8.C \
	src/R8/ACT81A.C \
	src/R8/COLI8.C \
	src/R8/EDTBL81.C \
	src/PLAYSP.C \
	src/R8/Z81ACT.C \
	src/R8/ZONETBL8.C \
	src/R8/TRAP_R81.C \
	src/R8/KABUTO8.C \
	src/R8/KOMA8.C \
	src/R8/SW8.C \
	src/R8/OKUSIESO.C \
	src/R8/TOBIRA.C \
	src/R8/KONBEA.C \
	src/R8/SCARAB.C \
	src/R8/KUZURE8.C \
	src/R8/NOKOGIRI.C \
	src/R8/PROPERA8.C \
	src/R8/DAI8.C \
	src/R8/HACHI8.C \
	src/R8/ET8.C \
	src/R8/FRIEND8.C \
	src/R8/PISTON.C \
	src/R8/HAGURUMA.C \
	src/R8/COL8B.C \
	src/R8/SCR81B.C \
	src/R8/Z81BTBL.C \
	src/R8/DEV81B.C \
	src/R8/SCRCHK8.C

R81C_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLAYSUB.C \
	src/GAME.C \
	src/RIDECHK.C \
	src/PLPAT8.C \
	src/R8/ACT81A.C \
	src/R8/COLI8.C \
	src/R8/EDTBL81.C \
	src/PLAYSP.C \
	src/R8/Z81ACT.C \
	src/R8/ZONETBL8.C \
	src/R8/TRAP_R81.C \
	src/R8/KABUTO8.C \
	src/R8/KOMA8.C \
	src/R8/SW8.C \
	src/R8/OKUSIESO.C \
	src/R8/TOBIRA.C \
	src/R8/KONBEA.C \
	src/R8/SCARAB.C \
	src/R8/KUZURE8.C \
	src/R8/NOKOGIRI.C \
	src/R8/PROPERA8.C \
	src/R8/DAI8.C \
	src/R8/HACHI8.C \
	src/R8/ET8.C \
	src/R8/FRIEND8.C \
	src/R8/PISTON.C \
	src/R8/HAGURUMA.C \
	src/R8/COL8C.C \
	src/R8/SCR81C.C \
	src/R8/Z81CTBL.C \
	src/R8/DEV81C.C \
	src/R8/SCRCHK8.C

R81D_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLAYSUB.C \
	src/GAME.C \
	src/RIDECHK.C \
	src/PLPAT8.C \
	src/R8/ACT81A.C \
	src/R8/COLI8.C \
	src/R8/EDTBL81.C \
	src/PLAYSP.C \
	src/R8/Z81ACT.C \
	src/R8/ZONETBL8.C \
	src/R8/TRAP_R81.C \
	src/R8/KABUTO8.C \
	src/R8/KOMA8.C \
	src/R8/SW8.C \
	src/R8/OKUSIESO.C \
	src/R8/TOBIRA.C \
	src/R8/KONBEA.C \
	src/R8/SCARAB.C \
	src/R8/KUZURE8.C \
	src/R8/NOKOGIRI.C \
	src/R8/PROPERA8.C \
	src/R8/DAI8.C \
	src/R8/HACHI8.C \
	src/R8/ET8.C \
	src/R8/FRIEND8.C \
	src/R8/PISTON.C \
	src/R8/HAGURUMA.C \
	src/R8/COL8D.C \
	src/R8/SCR81D.C \
	src/R8/Z81DTBL.C \
	src/R8/DEV81D.C \
	src/R8/SCRCHK8.C

R82A_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLAYSUB.C \
	src/GAME.C \
	src/RIDECHK.C \
	src/PLPAT8.C \
	src/R8/COL8A.C \
	src/R8/COLI8.C \
	src/R8/ZONETBL8.C \
	src/R8/KABUTO8.C \
	src/R8/KOMA8.C \
	src/R8/SW8.C \
	src/R8/OKUSIESO.C \
	src/R8/TOBIRA.C \
	src/R8/KUZURE8.C \
	src/R8/DAI8.C \
	src/R8/ET8.C \
	src/R8/FRIEND8.C \
	src/R8/PISTON.C \
	src/R8/HAGURUMA.C \
	src/R8/PLAYSP8.C \
	src/R8/ACT82A.C \
	src/R8/EDTBL82.C \
	src/R8/TRAP_R82.C \
	src/R8/Z82ACT.C \
	src/R8/Z82ATBL.C \
	src/R8/BEAM.C \
	src/R8/SHUT.C \
	src/R8/DEV82A.C \
	src/R8/SHOOT.C \
	src/R8/DANGO8.C \
	src/R8/LEGMECA8.C \
	src/R8/BIGBOM8.C \
	src/R8/SCR82A.C \
	src/R8/SCRCHK8.C \
	src/R8/JETTOGE8.C \
	src/R8/PIPE8.C

R82B_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLAYSUB.C \
	src/GAME.C \
	src/RIDECHK.C \
	src/PLPAT8.C \
	src/R8/COLI8.C \
	src/R8/ZONETBL8.C \
	src/R8/KABUTO8.C \
	src/R8/KOMA8.C \
	src/R8/SW8.C \
	src/R8/OKUSIESO.C \
	src/R8/TOBIRA.C \
	src/R8/KUZURE8.C \
	src/R8/DAI8.C \
	src/R8/ET8.C \
	src/R8/FRIEND8.C \
	src/R8/PISTON.C \
	src/R8/HAGURUMA.C \
	src/R8/PLAYSP8.C \
	src/R8/ACT82A.C \
	src/R8/EDTBL82.C \
	src/R8/TRAP_R82.C \
	src/R8/Z82ACT.C \
	src/R8/BEAM.C \
	src/R8/SHUT.C \
	src/R8/SHOOT.C \
	src/R8/DANGO8.C \
	src/R8/LEGMECA8.C \
	src/R8/BIGBOM8.C \
	src/R8/SCRCHK8.C \
	src/R8/JETTOGE8.C \
	src/R8/PIPE8.C \
	src/R8/COL8B.C \
	src/R8/DEV82B.C \
	src/R8/SCR82B.C \
	src/R8/Z82BTBL.C

R82C_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLAYSUB.C \
	src/GAME.C \
	src/RIDECHK.C \
	src/PLPAT8.C \
	src/R8/COLI8.C \
	src/R8/ZONETBL8.C \
	src/R8/KABUTO8.C \
	src/R8/KOMA8.C \
	src/R8/SW8.C \
	src/R8/OKUSIESO.C \
	src/R8/TOBIRA.C \
	src/R8/KUZURE8.C \
	src/R8/DAI8.C \
	src/R8/ET8.C \
	src/R8/FRIEND8.C \
	src/R8/PISTON.C \
	src/R8/HAGURUMA.C \
	src/R8/PLAYSP8.C \
	src/R8/ACT82A.C \
	src/R8/EDTBL82.C \
	src/R8/TRAP_R82.C \
	src/R8/Z82ACT.C \
	src/R8/BEAM.C \
	src/R8/SHUT.C \
	src/R8/SHOOT.C \
	src/R8/DANGO8.C \
	src/R8/LEGMECA8.C \
	src/R8/BIGBOM8.C \
	src/R8/SCRCHK8.C \
	src/R8/JETTOGE8.C \
	src/R8/PIPE8.C \
	src/R8/COL8C.C \
	src/R8/DEV82C.C \
	src/R8/SCR82C.C \
	src/R8/Z82CTBL.C

R82D_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLAYSUB.C \
	src/GAME.C \
	src/RIDECHK.C \
	src/PLPAT8.C \
	src/R8/COLI8.C \
	src/R8/ZONETBL8.C \
	src/R8/KABUTO8.C \
	src/R8/KOMA8.C \
	src/R8/SW8.C \
	src/R8/OKUSIESO.C \
	src/R8/TOBIRA.C \
	src/R8/KUZURE8.C \
	src/R8/DAI8.C \
	src/R8/ET8.C \
	src/R8/FRIEND8.C \
	src/R8/PISTON.C \
	src/R8/HAGURUMA.C \
	src/R8/PLAYSP8.C \
	src/R8/ACT82A.C \
	src/R8/EDTBL82.C \
	src/R8/TRAP_R82.C \
	src/R8/Z82ACT.C \
	src/R8/BEAM.C \
	src/R8/SHUT.C \
	src/R8/SHOOT.C \
	src/R8/DANGO8.C \
	src/R8/LEGMECA8.C \
	src/R8/BIGBOM8.C \
	src/R8/SCRCHK8.C \
	src/R8/JETTOGE8.C \
	src/R8/PIPE8.C \
	src/R8/COL8D.C \
	src/R8/DEV82D.C \
	src/R8/SCR82D.C \
	src/R8/Z82DTBL.C

R83C_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLAYSUB.C \
	src/GAME.C \
	src/RIDECHK.C \
	src/PLPAT8.C \
	src/R8/COLI8.C \
	src/R8/ZONETBL8.C \
	src/R8/COL8C.C \
	src/R8/EDTBL83.C \
	src/R8/SCR83C.C \
	src/R8/SCRCHK8.C \
	src/R8/Z83ACT.C \
	src/R8/Z83CTBL.C \
	src/R8/HAGURUMA.C \
	src/R8/BOSS_8.C \
	src/R8/PISTON.C \
	src/R8/KABUTO8.C \
	src/R8/KUZURE8.C \
	src/R8/NOKOGIRI.C \
	src/R8/TOBIRA.C \
	src/R8/JETTOGE8.C \
	src/R8/DAI8.C \
	src/R8/FRIEND8.C \
	src/R8/SW8.C \
	src/R8/KOMA8.C \
	src/R8/DEV83C.C \
	src/R8/ACT83.C \
	src/R8/PLAYSP8.C \
	src/R3/BOBIN.C \
	src/R8/SHUT.C \
	src/R8/EMIE8.C \
	src/R8/KONBEA83.C \
	src/R8/TRAP_R83.C \
	src/R8/HOTA8C.C

R83D_SOURCES := \
	src/ACTSET.C \
	src/ETC.C \
	src/FCOL.C \
	src/IO.C \
	src/SPRING.C \
	src/SCORE.C \
	src/SAVE.C \
	src/RING.C \
	src/PLAYER.C \
	src/DIRCOL.C \
	src/EDIT.C \
	src/GOAL.C \
	src/ITEM.C \
	src/ZONE.C \
	src/EQU.C \
	src/LOADER2.C \
	src/BMP.C \
	src/DUMMY.C \
	src/DLLMAIN.C \
	src/SUICIDE.C \
	src/PLCHG.C \
	src/ACTION.C \
	src/TBL0.C \
	src/TBL1.C \
	src/PLAYSUB.C \
	src/GAME.C \
	src/RIDECHK.C \
	src/PLPAT8.C \
	src/R8/COLI8.C \
	src/R8/ZONETBL8.C \
	src/R8/EDTBL83.C \
	src/R8/SCRCHK8.C \
	src/R8/Z83ACT.C \
	src/R8/HAGURUMA.C \
	src/R8/BOSS_8.C \
	src/R8/PISTON.C \
	src/R8/KABUTO8.C \
	src/R8/KUZURE8.C \
	src/R8/NOKOGIRI.C \
	src/R8/TOBIRA.C \
	src/R8/JETTOGE8.C \
	src/R8/DAI8.C \
	src/R8/FRIEND8.C \
	src/R8/SW8.C \
	src/R8/KOMA8.C \
	src/R8/ACT83.C \
	src/R8/PLAYSP8.C \
	src/R3/BOBIN.C \
	src/R8/SHUT.C \
	src/R8/TRAP_R83.C \
	src/R8/KONBEA83.C \
	src/R8/EMIE8.C \
	src/R8/COL8D.C \
	src/R8/DEV83D.C \
	src/R8/SCR83D.C \
	src/R8/Z83DTBL.C \
	src/R8/HOTA8D.C

SPECIAL_SOURCES := \
	src/SPECIAL/ACT_S.C \
	src/SPECIAL/BACT.C \
	src/SPECIAL/BMP.C \
	src/SPECIAL/COLI_S.C \
	src/SPECIAL/COMMON.C \
	src/SPECIAL/DATA_S1.C \
	src/SPECIAL/ENS.C \
	src/SPECIAL/ETC_M.C \
	src/SPECIAL/ETC_S.C \
	src/SPECIAL/FADEIN.C \
	src/SPECIAL/GAME.C \
	src/SPECIAL/KAITEN.C \
	src/SPECIAL/PLS.C \
	src/SPECIAL/SPM_EQU.C \
	src/SPECIAL/SPS_EQU.C \
	src/DLLMAIN.C \
	src/SPECIAL/SIN.C

WARP_SOURCES := \
	src/WARP/WARP.C \
	src/ACTION.C \
	src/DLLMAIN.C \
	src/EQU.C \
	src/BMP.C \
	src/IO.C

BESTTIME_SOURCES := \
	src/TITLE/BESTTIME/BESENTRY.C \
	src/TITLE/BESTTIME/BESTBMP.C \
	src/TITLE/BESTTIME/BESTDO.C \
	src/TITLE/BESTTIME/BESTGRID.C \
	src/TITLE/BESTTIME/BESTITEM.C \
	src/TITLE/BESTTIME/BESTPALT.C \
	src/TITLE/BESTTIME/BESTSPRT.C \
	src/TITLE/COMMON/HMX_OEEACTL.C

OPENING_SOURCES := \
	src/TITLE/OPENING/OPNBMP.C \
	src/TITLE/OPENING/OPNDO.C \
	src/TITLE/OPENING/OPNENTRY.C \
	src/TITLE/OPENING/OPNGRID.C \
	src/TITLE/COMMON/HMX_OEEACTL.C \
	src/TITLE/OPENING/OPNPALT.C

PLANET_SOURCES := \
	src/TITLE/PLANET/ACTM.C \
	src/TITLE/PLANET/ASCIISET.C \
	src/TITLE/PLANET/ASCMAP.C \
	src/TITLE/PLANET/CHAMOV.C \
	src/TITLE/PLANET/DATA_M.C \
	src/TITLE/PLANET/ETC.C \
	src/TITLE/PLANET/FLASH.C \
	src/TITLE/PLANET/KAITEN.C \
	src/TITLE/PLANET/LPLMAIN.C \
	src/TITLE/PLANET/PALET.C \
	src/TITLE/PLANET/PLM.C \
	src/TITLE/PLANET/PLS.C \
	src/TITLE/PLANET/SPM_EQU.C \
	src/TITLE/PLANET/SPS_EQU.C

SAVEDATA_SOURCES := \
	src/TITLE/SAVEDATA/SVDBMP.C \
	src/TITLE/SAVEDATA/SVDDO.C \
	src/TITLE/SAVEDATA/SVDEDIT.C \
	src/TITLE/SAVEDATA/SVDENTRY.C \
	src/TITLE/SAVEDATA/SVDFILE.C \
	src/TITLE/SAVEDATA/SVDGRID.C \
	src/TITLE/SAVEDATA/SVDMENU.C \
	src/TITLE/SAVEDATA/SVDPALT.C \
	src/TITLE/SAVEDATA/SVDSPRT.C \
	src/TITLE/COMMON/HMX_OEEACTL.C

SOUNDTST_SOURCES := \
	src/TITLE/SOUNDTST/SNDENTRY.C \
	src/TITLE/SOUNDTST/SNDBMP.C \
	src/TITLE/SOUNDTST/SNDDO.C \
	src/TITLE/SOUNDTST/SNDGRID.C \
	src/TITLE/SOUNDTST/SNDPALT.C \
	src/TITLE/SOUNDTST/SNDSPRT.C \
	src/TITLE/COMMON/HMX_OEEACTL.C

STAGETST_SOURCES := \
	src/TITLE/STAGETST/STGENTRY.C \
	src/TITLE/STAGETST/STGBMP.C \
	src/TITLE/STAGETST/STGDO.C \
	src/TITLE/STAGETST/STGGRID.C \
	src/TITLE/STAGETST/STGITEM.C \
	src/TITLE/STAGETST/STGPALT.C \
	src/TITLE/STAGETST/STGSPRT.C \
	src/TITLE/COMMON/HMX_OEEACTL.C

TA_SOURCES := \
	src/TITLE/TA/TA.C \
	src/TITLE/TA/FADEIN.C \
	src/TITLE/TA/TACOLOR.C \
	src/TITLE/TA/TAEACTRL.C

THANKS_SOURCES := \
	src/TITLE/THANKS/ACT.C \
	src/TITLE/THANKS/BM_M.C \
	src/TITLE/THANKS/CHRSET.C \
	src/TITLE/THANKS/DATA_SP.C \
	src/TITLE/THANKS/ETC_M.C \
	src/TITLE/THANKS/FADE.C \
	src/TITLE/THANKS/GAME.C \
	src/TITLE/THANKS/IO.C \
	src/TITLE/THANKS/SPM.C \
	src/TITLE/THANKS/SPRMOVE.C

VISUALMD_SOURCES := \
	src/TITLE/VISUALMD/VMDBMP.C \
	src/TITLE/VISUALMD/VMDDO.C \
	src/TITLE/VISUALMD/VMDENTRY.C \
	src/TITLE/VISUALMD/VMDGRID.C \
	src/TITLE/COMMON/HMX_OEEACTL.C \
	src/TITLE/VISUALMD/VMDPALT.C \
	src/TITLE/VISUALMD/VMDSPRT.C

.PHONY: all clean compare $(VARIANTS:%=compare-%)

all: $(TARGETS)

compare: $(VARIANTS:%=compare-%)

define BUILD_VARIANT
$(1)_OBJECTS := $$($(1)_SOURCES:%.C=$$(BUILD_DIR)/obj/$(1)/%.o)

$$(BUILD_DIR)/$(1).ELF: $$($(1)_OBJECTS) $$($(1)_LINKER_SCRIPT) Makefile
	@$$(MKDIR_P) $$(dir $$@)
	"$$(LD)" $$(LDFLAGS) -o $$@ $$($(1)_LINKER_SCRIPT) $$($(1)_OBJECTS)

$$(BUILD_DIR)/obj/$(1)/%.o: %.C
	@$$(MKDIR_P) $$(dir $$@)
	"$$(CC)" $$(COMPILE_ONLY) $$(CFLAGS) $$($(1)_DEFINES) $$(INCLUDES) -o $$@ $$<

compare-$(1): $$(BUILD_DIR)/$(1).ELF
	python tools/compare_elf.py $$(REFERENCE_DIR)/$(1).ELF $$(BUILD_DIR)/$(1).ELF
endef

$(foreach variant,$(VARIANTS),$(eval $(call BUILD_VARIANT,$(variant))))

clean:
	$(RM_RF) $(BUILD_DIR)/obj $(TARGETS) $(TARGETS:.ELF=.map) $(BUILD_DIR)/probe.o
