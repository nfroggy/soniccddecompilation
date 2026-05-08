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

VARIANTS := R11A R11B R11C R11D R12A R12B R12C R12D R13C R13D
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
