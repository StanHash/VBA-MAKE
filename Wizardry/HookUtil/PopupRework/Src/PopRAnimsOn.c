#include "PopupRework.h"

// TODO: add to libgbafe {

extern const u8 gGfxBattlePopup[];
extern const u16 gPalBattlePopup[];
extern const u8 gTSA_BattlePopup[];
extern const u8 gGfxPopupTextBG[];
extern const u32 gPopupIconFrameData[];

extern u8 gSomeTSABuffer[];

extern struct FontData gSomeFontStruct;

void MakeBattlePopupTileMapFromTSA(u16* bgOut, u16 size) __attribute__((long_call));
void SomeBattlePlaySound_8071990(int id, int volume) __attribute__((long_call));

// }

struct PopupReworkAnimsOnProc {
	struct PopupReworkProc popr;
	struct AnimationInterpreter* iconAis;
};

static void PopR_AnimsOnAddIcon(struct PopupReworkProc* proc, unsigned iconId, unsigned xOffset);
static void PopR_AnimsOnDraw(struct PopupReworkAnimsOnProc* proc);
static void PopR_AnimsOnWait(struct PopupReworkAnimsOnProc* proc);
static void PopR_AnimsOnCleanup(struct PopupReworkAnimsOnProc* proc);

static const struct ProcInstruction sProc_PopRAnimsOnPopup[] = {
	PROC_SET_DESTRUCTOR(PopR_AnimsOnCleanup),

	PROC_SLEEP(0),

	PROC_CALL_ROUTINE(PopR_AnimsOnDraw),
	PROC_LOOP_ROUTINE(PopR_AnimsOnWait),

	PROC_END
};

static struct PopupReworkProc* PopR_StartAnimsOnPopup(const u32* definition, unsigned time, struct Proc* parent) {
	struct PopupReworkAnimsOnProc* proc = (struct PopupReworkAnimsOnProc*) StartBlockingProc(
		sProc_PopRAnimsOnPopup, parent
	);

	proc->popr.pop.pDefinition = definition;
	proc->popr.pop.iconPalId   = 0x12;

	if (time == 0)
		proc->popr.pop.clock = -1;
	else
		proc->popr.pop.clock = time;

	proc->popr.addIcon = PopR_AnimsOnAddIcon;
	proc->iconAis       = NULL;

	return (struct PopupReworkProc*) (proc);
}

static void PopR_AnimsOnAddIcon(struct PopupReworkProc* proc, unsigned iconId, unsigned xOffset) {
	struct PopupReworkAnimsOnProc* const pproc = (struct PopupReworkAnimsOnProc*) proc;

	if (pproc->iconAis)
		AIS_Free(pproc->iconAis);

	pproc->iconAis = AIS_Create(gPopupIconFrameData, 150);

	pproc->iconAis->oam2base  = 0x2440;
	pproc->iconAis->xPosition = proc->pop.xTileReal + 0x10 + xOffset;
	pproc->iconAis->yPosition = proc->pop.yTileReal + 0x08;

	LoadIconObjectGraphics(iconId, 0x40);
}

static void PopR_AnimsOnDraw(struct PopupReworkAnimsOnProc* proc) {
	static const unsigned BOX_GFX_TILE  = 0x100;
	static const unsigned TEXT_GFX_TILE = 0x108;
	static const unsigned COMMON_PAL    = 1;
	static void* const    BOX_GFX_VRAM  = (void*)(VRAM + BOX_GFX_TILE * 0x20);
	static void* const    TEXT_GFX_VRAM = (void*)(VRAM + TEXT_GFX_TILE * 0x20);

	proc->popr.pop.xGfxSize = PopR_GetLength(&proc->popr);

	unsigned xTileSize    = (proc->popr.pop.xGfxSize + 7) / 8;
	unsigned xStartOffset = (xTileSize*8 - proc->popr.pop.xGfxSize)/2;

	proc->popr.pop.xTileReal = ((240 - (xTileSize+4)*8) / 2);
	proc->popr.pop.yTileReal = 48;

	Font_InitForUI(&gSomeFontStruct, TEXT_GFX_VRAM, TEXT_GFX_TILE, COMMON_PAL);
	Font_SetDraw1DTileNoClear();

	struct TextHandle text;
	Text_InitClear(&text, xTileSize);

	CopyToPaletteBuffer(gPalBattlePopup, COMMON_PAL * 0x20, 0x20);

	Decompress(gGfxBattlePopup, BOX_GFX_VRAM);
	Decompress(gGfxPopupTextBG, TEXT_GFX_VRAM);

	Text_SetXCursor(&text, xStartOffset);

	PopR_DisplayComponents(&proc->popr, &text);

	Decompress(gTSA_BattlePopup, gSomeTSABuffer);
	MakeBattlePopupTileMapFromTSA(gBg1MapBuffer, xTileSize);

	SetBgPosition(1, -proc->popr.pop.xTileReal, -proc->popr.pop.yTileReal);

	EnableBgSyncByIndex(1);

	SetDefaultColorEffects();

	SomeBattlePlaySound_8071990(proc->popr.pop.soundId, 0x100);
}

static void PopR_AnimsOnWait(struct PopupReworkAnimsOnProc* proc) {
	if (proc->popr.pop.clock < 0) {
		if (gKeyStatus.pressedKeys & A_BUTTON)
			BreakProcLoop((struct Proc*) (proc));
	} else {
		if (proc->popr.pop.clock == 0)
			BreakProcLoop((struct Proc*) (proc));
		else
			proc->popr.pop.clock--;
	}
}

static void PopR_AnimsOnCleanup(struct PopupReworkAnimsOnProc* proc) {
	if (proc->iconAis)
		AIS_Free(proc->iconAis);

	FillBgMap(gBg1MapBuffer, 0);
	EnableBgSyncByIndex(1);
}

struct AnimsOnWrapperProc {
	/* 00 */ PROC_HEADER;

	/* 2C */ const struct BattlePopupType* itPop;
};

static void PopR_AnimsOnWrapperLoop(struct AnimsOnWrapperProc* proc);
static void PopR_AnimsOnWrapperCleanup(struct AnimsOnWrapperProc* proc);

static const struct ProcInstruction sProc_PopR_AnimsOnWrapper[] = {
	PROC_SET_DESTRUCTOR(PopR_AnimsOnWrapperCleanup),
	PROC_SLEEP(10),

PROC_LABEL(0),
	PROC_LOOP_ROUTINE(PopR_AnimsOnWrapperLoop),
	PROC_SLEEP(8),

	PROC_GOTO(0),

PROC_LABEL(1),
	PROC_END
};

void PopR_StartBattlePopups(void) {
	struct AnimsOnWrapperProc* proc = (struct AnimsOnWrapperProc*) StartProc(sProc_PopR_AnimsOnWrapper, ROOT_PROC_3);

	gpPopup6C = (struct Proc*) (proc);
	gBoolPopupEnded = FALSE;

	if (gSomethingRelatedToAnimAndDistance == 4)
		// Promoting!
		proc->itPop = gPromotionPopupTable;

	else
		// Not Promoting!
		proc->itPop = gBattlePopupTable;

	Sound_SetSongVolume(0x80);
}

static void PopR_AnimsOnWrapperLoop(struct AnimsOnWrapperProc* proc) {
	const struct BattlePopupType type = *proc->itPop++;

	if (!type.tryInit) {
		GotoProcLabel((struct Proc*) (proc), 1);
		return;
	}

	if (!type.tryInit()) {
		GotoProcLabel((struct Proc*) (proc), 0);
		return;
	}

	PopR_StartAnimsOnPopup(type.definition, type.time, (struct Proc*) (proc));
	BreakProcLoop((struct Proc*) (proc));
}

static void PopR_AnimsOnWrapperCleanup(struct AnimsOnWrapperProc* proc) {
	gBoolPopupEnded = TRUE;
	Sound_SetSongVolume(0x100);
}
