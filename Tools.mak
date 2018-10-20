# making sure devkitARM exists and is set up
ifeq ($(strip $(DEVKITARM)),)
  $(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

# including devkitARM tool definitions
include $(DEVKITARM)/base_tools

# OS-specific tool calls (EA)
ifeq ($(OS),Windows_NT)
  export EA := $(realpath .)/Tools/EventAssembler/Core.exe
else
  export EA := mono $(realpath .)/Tools/EventAssembler/Core.exe
endif

# Making sure we are using python 3
ifeq ($(shell python -c 'import sys; print(int(sys.version_info[0] > 2))'),1)
  export PYTHON3 := python
else
  export PYTHON3 := python3
endif

# additional tools
export PARSEFILE         := $(realpath .)/Tools/ParseFile
export PORTRAITFORMATTER := $(realpath .)/Tools/PortraitFormatter
export GBAGFX            := $(realpath .)/Tools/gbagfx
export PORTRAIT_PROCESS  := $(PYTHON3) $(realpath .)/Tools/py/portrait-process.py
export TEXT_PROCESS      := $(PYTHON3) $(realpath .)/Tools/py/text-process.py
export C2EA              := $(PYTHON3) $(realpath .)/Tools/py/NMM2CSV/c2ea.py
export TMX2EA            := $(PYTHON3) $(realpath .)/Tools/py/TMX2EA/tmx2ea.py

# unused
# export LYN               := $(realpath .)/Tools/lyn

PREPROCESS_MESSAGE = @echo "$(notdir $<) => $(notdir $@)"