#include "SkillSystem.h"
#include "PopupRework.h"

struct SSLearningState {
	/* ASSUMED ALLOCATED SIZE: 10 */

	/* 00 */ u8 skillLearnedUnitId[4];
	/* 04 */ u8 skillLearnedId[4];
	/* 08 */ s8 skillForgetSlot[4];

	/* 0C */ unsigned nextSlotIndex : 2; // range 0..3
};

extern struct SSLearningState* const gpSSLearningState;

// =============================
// = SKILL ICON POPR COMPONENT =
// =============================

static int  SS_PopRSkillIcon_GetLength(struct PopupReworkProc* proc, u32 argument);
static void SS_PopRSkillIcon_Display(struct PopupReworkProc* proc, struct TextHandle* text, u32 argument);

const struct PopupComponentType gSS_PopRComponent_SkillIcon = {
	.getLength = SS_PopRSkillIcon_GetLength,
	.display   = SS_PopRSkillIcon_Display,
};

static int SS_PopRSkillIcon_GetLength(struct PopupReworkProc* proc, u32 argument) {
	return 16;
}

static void SS_PopRSkillIcon_Display(struct PopupReworkProc* proc, struct TextHandle* text, u32 argument) {
	proc->addIcon(proc, 0x100 | gPopupItem, Text_GetXCursor(text));
	Text_Advance(text, 16);

	LoadIconPalette(0, proc->pop.iconPalId);
}

// ===========================
// = BATTLE UNIT INIT/DEINIT =
// ===========================

void SSLearn_OnBattleUnitInit(struct BattleUnit* bu, struct Unit* unit) {
	// Because unsigned overflow is well defined in C, even for bitfields
	// We can use gpSSLearningState->nextSlotIndex to interate on the 4
	// Valid indices in a loopy manner

	unsigned i = gpSSLearningState->nextSlotIndex++;

	gpSSLearningState->skillLearnedUnitId[i] = unit->index;
	gpSSLearningState->skillLearnedId[i]     = 0;
	gpSSLearningState->skillForgetSlot[i]    = (-1);
}

void SSLearn_OnBattleUnitDeinit(struct Unit* unit, struct BattleUnit* bu) {
	for (unsigned i = 0; i < 4; ++i) {
		if (gpSSLearningState->skillLearnedUnitId[i] != unit->index)
			continue;

		if (gpSSLearningState->skillForgetSlot[i] >= 0)
			SS_UnitForgetSkillSlot(unit, gpSSLearningState->skillForgetSlot[i]);

		if (gpSSLearningState->skillLearnedId[i]) {
			int slot = SS_UnitGetFreeSkillSlot(unit);

			if (slot >= 0)
				SS_UnitSetSkillSlot(unit, slot, gpSSLearningState->skillLearnedId[i]);
		}

		gpSSLearningState->skillLearnedUnitId[i] = 0;

		break;
	}
}

// ==============
// = ModLU Hook =
// ==============

void SSLearn_UnitLevelUp(struct BattleUnit* bu) {
	if (!SS_UnitCanLearnSkills(&bu->unit))
		return;

	for (unsigned i = 0; i < 4; ++i) {
		if (gpSSLearningState->skillLearnedUnitId[i] != bu->unit.index)
			continue;

		gpSSLearningState->skillLearnedId[i] = 50;//SS_UnitGetLevelUpSkill(&bu->unit, bu->unit.level);
		break;
	}
}

// =============
// = PopR Hook =
// =============

int SS_PopR_InitSkillLearning(void) {
	for (unsigned i = 0; i < 4; ++i) {
		if (!gpSSLearningState->skillLearnedId[i])
			continue;

		if (gSubjectBattleStruct.unit.index == gpSSLearningState->skillLearnedUnitId[i]) {
			SetPopupItem(gpSSLearningState->skillLearnedId[i]);
			gpSSLearningState->skillLearnedId[i] = 0;

			return TRUE;
		}

		if (gTargetBattleStruct.unit.index == gpSSLearningState->skillLearnedUnitId[i]) {
			SetPopupItem(gpSSLearningState->skillLearnedId[i]);
			gpSSLearningState->skillLearnedId[i] = 0;

			return TRUE;
		}
	}

	return FALSE;
}
