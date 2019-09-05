# making sure devkitARM exists and is set up
ifeq ($(strip $(DEVKITARM)),)
  $(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

# including devkitARM tool definitions
include $(DEVKITARM)/base_tools

# OS-specific tool calls (EA)
ifeq ($(OS),Windows_NT)
  export NLEA := $(realpath .)/Tools/EventAssembler/Core.exe
  export CCEA := $(realpath .)/Tools/EventAssembler/ColorzCore.exe
else
  export NLEA := mono $(realpath .)/Tools/EventAssembler/Core.exe
  export CCEA := mono $(realpath .)/Tools/EventAssembler/ColorzCore.exe
endif

# Default EA is NL!EA
export EA := $(NLEA)

# NL!EA is used for dependency generation.
# ColorzCore is used for the "real" pass.

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
export PORTRAIT_PROCESS  := $(PYTHON3) $(realpath .)/Tools/Py/portrait-process.py
export TEXT_PROCESS      := $(PYTHON3) $(realpath .)/Tools/Py/text-process-classic.py --force-refresh
export C2EA              := $(PYTHON3) $(realpath .)/Tools/Py/NMM2CSV/c2ea.py
export TMX2EA            := $(PYTHON3) $(realpath .)/Tools/Py/TMX2EA/tmx2ea.py
export LYN               := $(realpath .)/Tools/lyn

NOTIFY_PROCESS = @echo "$(notdir $<) => $(notdir $@)"
