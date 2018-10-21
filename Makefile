.SUFFIXES:
.PHONY: hack hack_recursive all clean

include Tools.mak

# Cache directory
CACHE_DIR := .MkCache
$(shell mkdir -p $(CACHE_DIR) > /dev/null)

# EA Input File
EVENT_MAIN    := Main.event

# EA Secondary outupt files
EVENT_DEPENDS := $(CACHE_DIR)/Main.d
EVENT_SYMBOLS := VBAR.sym.event

# ROMs
ROM_SOURCE    := FE8U.gba
ROM_TARGET    := VBAR.gba

# Clean-only stuff
ifeq ($(MAKECMDGOALS),clean)
  # NMMs and generated events
  NMMS := $(shell find -type f -name '*.nmm')
  TABLE_EVENTS := $(NMMS:.nmm=.event)

  # TMXs and generated files
  TMXS := $(shell find -type f -name '*.tmx')
  MAP_GENERATED := $(TMXS:.tmx=.event) $(TMXS:.tmx=_data.dmp)

  # All files that are going to be cleaned
  ALL_TO_CLEAN := $(EVENT_DEPENDS) $(ROM_TARGET) $(EVENT_SYMBOLS) $(TABLE_EVENTS) $(MAP_GENERATED)
endif

# Variable listing all text files in the writans directory
# The text installer depends on them (in case there was any change)
# (Too lazy to code a dependency thingy for that too)
WRITANS_ALL_TEXT := $(wildcard Writans/Text/*.txt)

# ------------------
# PHONY TARGET RULES

hack:
	@rm -f $(EVENT_DEPENDS)
	@$(MAKE) hack_recursive

hack_recursive: $(ROM_TARGET);

clean:
	@rm -f  $(ALL_TO_CLEAN)
	@rm -rf $(CACHE_DIR)
	@rm -rf Writans/Text/.TextEntries Writans/Text.event Writans/TextDefinitions.event
	@echo all clean!

# -------------------
# ACTUAL TARGET RULES

$(ROM_TARGET): $(EVENT_MAIN) $(EVENT_DEPENDS) $(ROM_SOURCE)
	@echo Building $(ROM_TARGET).
	@cp -f $(ROM_SOURCE) $(ROM_TARGET)
	@$(CCEA) A FE8 -output:$(ROM_TARGET) -input:$(EVENT_MAIN) || (rm $(ROM_TARGET) && false)

$(EVENT_DEPENDS):
	@echo Refreshing dependencies.
	@$(EA) A FE8 -output $(ROM_TARGET) -input $(EVENT_MAIN) -MM -MG -MT $(EVENT_DEPENDS) -MF $(EVENT_DEPENDS)
	@sed -i s/\\\\/\\//g $(EVENT_DEPENDS)

# -------------------
# SPECIAL FILES RULES

Spritans/Portraits.event: Spritans/PortraitList.txt
	$(PORTRAIT_PROCESS) $< $@

Writans/Text.event Writans/TextDefinitions.event: $(WRITANS_ALL_TEXT)
	$(TEXT_PROCESS) Writans/Text/TextMain.txt Writans/Text.event Writans/TextDefinitions.event

# -------------
# PATTERN RULES

# PNG to 4bpp rule
%.4bpp: %.png
	$(PREPROCESS_MESSAGE)
	@Tools/Png2Dmp $< -o $@
#	@$(GBAGFX) $< $@

# PNG to gbapal rule
%.gbapal: %.png
	$(PREPROCESS_MESSAGE)
#	@Tools/Png2Dmp $< -o $@ --palette-only
	@$(GBAGFX) $< $@

# Anything to lz rule
%.lz: %
	$(PREPROCESS_MESSAGE)
	@$(GBAGFX) $< $@

# Formatted text to insertable binary
# Nulling output because it's annoying
%.fetxt.bin: %.fetxt
	$(PREPROCESS_MESSAGE)
	@$(PARSEFILE) $< -o $@ > /dev/null

# PNG to insertable portrait rule
%_mug.dmp %_palette.dmp %_frames.dmp %_minimug.dmp: %.png
	$(PREPROCESS_MESSAGE)
	@$(PORTRAITFORMATTER) $<

# CSV+NMM to event
%.event: %.csv %.nmm
	$(PREPROCESS_MESSAGE)
	@echo | $(C2EA) -csv $*.csv -nmm $*.nmm -out $*.event $(ROM_SOURCE)

# TMX to event + dmp
%.event %_data.dmp: %.tmx
	$(PREPROCESS_MESSAGE)
	@echo | $(TMX2EA) $<

# OBJ to event
%.lyn.event: %.o
	$(PREPROCESS_MESSAGE)
	@$(LYN) $< > $@

# OBJ to event
%.lyn.event: %.elf
	$(PREPROCESS_MESSAGE)
	@$(LYN) $< > $@

# --------------------
# INCLUDE DEPENDENCIES

ifneq ($(MAKECMDGOALS),clean)
  -include $(EVENT_DEPENDS)
endif
