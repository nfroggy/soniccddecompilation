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

VARIANTS := R11A R11B R11C R11D R12A R12B R12C R12D R13C R13D R31A R31B R31C R31D R32A R32B R32C R32D R33C R33D R41A R41B R41C R41D R42A R42B R42C R42D R43C R43D
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
