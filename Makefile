.SUFFIXES:
.PHONY: hack clean

include Tools.mak

# Common cache directory
# Used to generate dependency files in
CACHE_DIR := .MkCache
$(shell mkdir -p $(CACHE_DIR) > /dev/null)

# ===============
# = MAIN TARGET =
# ===============

# When making, we want the dependency file to be regenerated
# So we use this phony rule to delete it and call make recursively
# To make the target ROM (which depends on said dependency file)

hack:
	@rm -f $(EVENT_DEPENDS)
	@$(MAKE) $(ROM_TARGET)

# =================
# = THE BUILDFILE =
# =================

# ROMs
ROM_SOURCE    := FE8U.gba
ROM_TARGET    := VBAR.gba

# EA Input File
EVENT_MAIN    := Main.event

# EA Secondary outupt files
EVENT_DEPENDS := $(CACHE_DIR)/Main.d
EVENT_SYMBOLS := VBAR.sym.event

# Make the target ROM from events and source ROMs
# Depends on the dependency file so we ensure it being generated
# (The depency file will list all other files the ROM depends on)
$(ROM_TARGET): $(EVENT_MAIN) $(EVENT_DEPENDS) $(ROM_SOURCE)
	@echo Building $(ROM_TARGET).
	@cp -f $(ROM_SOURCE) $(ROM_TARGET)
	@$(CCEA) A FE8 -output:$(ROM_TARGET) -input:$(EVENT_MAIN) --nocash-sym || (rm $(ROM_TARGET) && false)

# Dependency making rule
# The sed invocation is to convert windows dir separators ('\') to unix ('/', so make doesn't break)
$(EVENT_DEPENDS):
	@echo Refreshing dependencies.
	@$(EA) A FE8 -output $(ROM_TARGET) -input $(EVENT_MAIN) -MM -MG -MT $(EVENT_DEPENDS) -MF $(EVENT_DEPENDS)
	@sed -i s/\\\\/\\//g $(EVENT_DEPENDS)

# =============
# = PORTRAITS =
# =============

PORTRAIT_LIST      := Spritans/PortraitList.txt
PORTRAIT_INSTALLER := Spritans/Portraits.event

# Make the portrait installer
$(PORTRAIT_INSTALLER): $(PORTRAIT_LIST)
	$(NOTIFY_PROCESS)
	@$(PORTRAIT_PROCESS) $< $@

# Convert a png to portrait components
%_mug.dmp %_palette.dmp %_frames.dmp %_minimug.dmp: %.png
	$(NOTIFY_PROCESS)
	@$(PORTRAITFORMATTER) $<

# ========
# = TEXT =
# ========

# Variable listing all text files in the writans directory
# The text installer depends on them (in case there was any change)
# (Too lazy to code a dependency thingy for that too)
WRITANS_ALL_TEXT    := $(wildcard Writans/Text/*.txt)

# Main input text file
WRITANS_TEXT_MAIN   := Writans/Text/TextMain.txt

# textprocess outputs
WRITANS_INSTALLER   := Writans/Text.event
WRITANS_DEFINITIONS := Writans/TextDefinitions.event

# Make text installer and definitions from text
$(WRITANS_INSTALLER) $(WRITANS_DEFINITIONS): $(WRITANS_TEXT_MAIN) $(WRITANS_ALL_TEXT)
	$(NOTIFY_PROCESS)
	@$(TEXT_PROCESS) $(WRITANS_TEXT_MAIN) --installer $(WRITANS_INSTALLER) --definitions $(WRITANS_DEFINITIONS) --parser-exe $(PARSEFILE)

# ==========
# = TABLES =
# ==========

# Convert CSV+NMM to event
%.event: %.csv %.nmm
	$(NOTIFY_PROCESS)
	@echo | $(C2EA) -csv $*.csv -nmm $*.nmm -out $*.event $(ROM_SOURCE) > /dev/null

# ========
# = MAPS =
# ========

# TMX to event + dmp
%.event %_data.dmp: %.tmx
	$(NOTIFY_PROCESS)
	@echo | $(TMX2EA) $< > /dev/null

# ==========================
# = GRAPHICS & COMPRESSION =
# ==========================

# PNG to 4bpp rule
%.4bpp: %.png
	$(NOTIFY_PROCESS)
	@Tools/Png2Dmp $< -o $@
#	@$(GBAGFX) $< $@

# PNG to gbapal rule
%.gbapal: %.png
	$(NOTIFY_PROCESS)
#	png2dmp doesn't seem to support palette output with --to-stdout
#	@Tools/Png2Dmp $< -o $@ --palette-only
	@$(GBAGFX) $< $@

# Anything to lz rule
%.lz: %
	$(NOTIFY_PROCESS)
	@$(GBAGFX) $< $@

# ==================
# = OBJECTS & DMPS =
# ==================

LYN_REFERENCE := Tools/CLib/reference/FE8U-20190316.o

# OBJ to event
%.lyn.event: %.o $(LYN_REFERENCE)
	$(NOTIFY_PROCESS)
	@$(LYN) $< $(LYN_REFERENCE) > $@

# OBJ to event (alt)
# TODO: we shouldn't need this
%.lyn.event: %.elf $(LYN_REFERENCE)
	$(NOTIFY_PROCESS)
	@$(LYN) $< $(LYN_REFERENCE) > $@

# OBJ to DMP rule
%.dmp: %.o
	$(NOTIFY_PROCESS)
	@$(OBJCOPY) -S $< -O binary $@

# ========================
# = ASSEMBLY/COMPILATION =
# ========================

# Setting C/ASM include directories up (there is none yet)
INCLUDE_DIRS := Tools/CLib/include Wizardry/Include
INCFLAGS     := $(foreach dir, $(INCLUDE_DIRS), -I "$(dir)")

# setting up compilation flags
ARCH    := -mcpu=arm7tdmi -mthumb -mthumb-interwork
CFLAGS  := $(ARCH) $(INCFLAGS) -Wall -Os -mtune=arm7tdmi -ffreestanding -mlong-calls
ASFLAGS := $(ARCH) $(INCFLAGS)

# defining dependency flags
CDEPFLAGS = -MMD -MT "$*.o" -MT "$*.asm" -MF "$(CACHE_DIR)/$(notdir $*).d" -MP
SDEPFLAGS = --MD "$(CACHE_DIR)/$(notdir $*).d"

# ASM to OBJ rule
%.o: %.s
	$(NOTIFY_PROCESS)
	@$(AS) $(ASFLAGS) $(SDEPFLAGS) -I $(dir $<) $< -o $@ $(ERROR_FILTER)

# C to ASM rule
# I would be fine with generating an intermediate .s file but this breaks dependencies
%.o: %.c
	$(NOTIFY_PROCESS)
	@$(CC) $(CFLAGS) $(CDEPFLAGS) -g -c $< -o $@ $(ERROR_FILTER)

# C to ASM rule
%.asm: %.c
	$(NOTIFY_PROCESS)
	@$(CC) $(CFLAGS) $(CDEPFLAGS) -S $< -o $@ -fverbose-asm $(ERROR_FILTER)

# Avoid make deleting objects it thinks it doesn't need anymore
# Without this make may fail to detect some files as being up to date
.PRECIOUS: %.o;

# ==============
# = MAKE CLEAN =
# ==============

ifeq ($(MAKECMDGOALS),clean)

  # NMMs and generated events
  NMMS := $(shell find -type f -name '*.nmm')
  TABLE_EVENTS := $(NMMS:.nmm=.event)

  # TMXs and generated files
  TMXS := $(shell find -type f -name '*.tmx')
  MAP_GENERATED := $(TMXS:.tmx=.event) $(TMXS:.tmx=_data.dmp)

  # Portraits and generated files
  PORTRAITS := $(wildcard Spritans/Portraits/*.png)

  PORTRAIT_GENERATED := \
    $(PORTRAITS:.png=_mug.dmp) $(PORTRAITS:.png=_palette.dmp) \
    $(PORTRAITS:.png=_frames.dmp) $(PORTRAITS:.png=_minimug.dmp)

  # ASM/C and generated files
  CFILES := $(shell find -type f -name '*.c')
  SFILES := $(shell find -type f -name '*.s')

  ASM_C_GENERATED := $(CFILES:.c=.o) $(SFILES:.s=.o) $(CFILES:.c=.asm)
  ASM_C_GENERATED += $(ASM_C_GENERATED:.o=.dmp) $(ASM_C_GENERATED:.o=.lyn.event)

  # All files that are going to be cleaned
  ALL_TO_CLEAN := \
    $(EVENT_DEPENDS) \
    $(ROM_TARGET) \
    $(EVENT_SYMBOLS) \
    $(TABLE_EVENTS) \
    $(MAP_GENERATED) \
    $(WRITANS_INSTALLER) \
    $(WRITANS_DEFINITIONS) \
    $(PORTRAIT_INSTALLER) \
    $(PORTRAIT_GENERATED) \
    $(ASM_C_GENERATED)

endif

clean:
	@rm -f  $(ALL_TO_CLEAN)
	@rm -rf $(CACHE_DIR)
	@rm -rf Writans/Text/.TextEntries

	@echo all clean!

# ========================
# = INCLUDE DEPENDENCIES =
# ========================

ifneq ($(MAKECMDGOALS),clean)
  -include $(EVENT_DEPENDS)
  -include $(wildcard $(CACHE_DIR)/*.d)
endif
