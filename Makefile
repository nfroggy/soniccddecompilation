TARGET := build/R11A.ELF
REFERENCE := R11A.ELF
BUILD_DIR := build
LINKER_SCRIPT ?= linker.lcf
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

DEFINES := -DR11A -DEDIT_DATA_ENTRY_COUNT=52
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

R11A_SOURCES := \
	src/DLLMAIN.C \
	src/ACTION.C \
	src/ACTSET.C \
	src/EQU.C \
	src/DAI_K.C \
	src/DAI_RD1.C \
	src/DIRCOL.C \
	src/DUMMY.C \
	src/EDIT.C \
	src/EMIE1.C \
	src/EMIE1CG.C \
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
	src/ZONETBL1.C \
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

OBJECTS := $(R11A_SOURCES:%.C=$(BUILD_DIR)/obj/%.o)

.PHONY: all clean compare

all: $(TARGET)

$(TARGET): $(OBJECTS) $(LINKER_SCRIPT) Makefile
	@$(MKDIR_P) $(dir $@)
	"$(LD)" $(LDFLAGS) -o $@ $(LINKER_SCRIPT) $(OBJECTS)

$(BUILD_DIR)/obj/%.o: %.C
	@$(MKDIR_P) $(dir $@)
	"$(CC)" $(COMPILE_ONLY) $(CFLAGS) $(DEFINES) $(INCLUDES) -o $@ $<

compare: $(TARGET)
	python tools/compare_elf.py $(REFERENCE) $(TARGET)

clean:
	$(RM_RF) $(BUILD_DIR)/obj $(TARGET) $(TARGET:.ELF=.map) $(BUILD_DIR)/probe.o
