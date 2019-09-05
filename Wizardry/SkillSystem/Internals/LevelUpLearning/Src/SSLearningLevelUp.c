
#include "SkillSystem.h"
#include "PopupRework.h"

extern const u8* const LevelUpSkillTable[];

// Skill id and forgot skill slot id occupy what were previously padding bytes in the battle unit

#define BU_SKILL_LEARNED(bu)     (*((u8*) (((void*) (bu)) + 0x58)))
#define BU_SKILL_FORGET_SLOT(bu) (*((s8*) (((void*) (bu)) + 0x7F)))

// =======================
// = SKILL LEARNING CORE =
// =======================

/*!
 * \param unit unit
 * \param level level
 * \return skill they would learn (zero if none)
 */
unsigned SS_UnitGetLevelUpSkill(struct Unit* unit, unsigned level)
{
	const u8* levelUpSkills = LevelUpSkillTable[unit->pClassData->number];

	if (levelUpSkills)
	{
		while (levelUpSkills[0])
		{
			if (level == levelUpSkills[0])
				return levelUpSkills[1];

			levelUpSkills = levelUpSkills + 2;
		}
	}

	return 0;
}

/*!
 * \param unit unit to autolevel skills
 */
void SS_UnitAutolevelSkills(struct Unit* unit)
{
	SS_ClearUnitSkills(unit);

	const u8* levelUpSkills = LevelUpSkillTable[unit->pClassData->number];

	if (levelUpSkills)
	{
		while (levelUpSkills[0])
		{
			unsigned level = levelUpSkills[0];

			// Skill is learned if learning level is lower than unit level,
			// Or learning level is 0xFF (used to give promoted generics unpromoted classes' skills)

			if ((level == 0xFF) || (level <= unit->level))
				SS_UnitLearnSkill(unit, levelUpSkills[1]);

			levelUpSkills = levelUpSkills + 2;
		}
	}
}

// =============================
// = SKILL ICON POPR COMPONENT =
// =============================

static int  SS_PopRSkillIcon_GetLength(struct PopupReworkProc* proc, u32 argument);
static void SS_PopRSkillIcon_Display(struct PopupReworkProc* proc, struct TextHandle* text, u32 argument);

const struct PopupComponentType gSS_PopRComponent_SkillIcon =
{
	.getLength = SS_PopRSkillIcon_GetLength,
	.display   = SS_PopRSkillIcon_Display,
};

static int SS_PopRSkillIcon_GetLength(struct PopupReworkProc* proc, u32 argument)
{
	return 16;
}

static void SS_PopRSkillIcon_Display(struct PopupReworkProc* proc, struct TextHandle* text, u32 argument)
{
	proc->addIcon(proc, 0x100 | gPopupItem, Text_GetXCursor(text));
	Text_Advance(text, 16);

	LoadIconPalette(0, proc->pop.iconPalId);
}

// ===========================
// = BATTLE UNIT INIT/DEINIT =
// ===========================

void SSLearn_OnBattleUnitInit(struct BattleUnit* bu, struct Unit* unit)
{
	BU_SKILL_LEARNED(bu) = 0;
	BU_SKILL_FORGET_SLOT(bu) = (-1);
}

void SSLearn_OnBattleUnitDeinit(struct Unit* unit, struct BattleUnit* bu)
{
	if (BU_SKILL_FORGET_SLOT(bu) >= 0)
		SS_UnitForgetSkillSlot(unit, BU_SKILL_FORGET_SLOT(bu));

	if (BU_SKILL_LEARNED(bu))
	{
		int slot = SS_UnitGetFreeSkillSlot(unit);

		if (slot >= 0)
			SS_UnitSetSkillSlot(unit, slot, BU_SKILL_LEARNED(bu));
	}
}

// ==============
// = ModLU Hook =
// ==============

void SSLearn_UnitLevelUp(struct BattleUnit* bu)
{
	BU_SKILL_LEARNED(bu) = SS_UnitGetLevelUpSkill(&bu->unit, bu->unit.level);
}

// =============
// = PopR Hook =
// =============

int SS_PopR_InitSkillLearning(void)
{
	if (BU_SKILL_LEARNED(&gActiveBattleUnit))
	{
		SetPopupItem(BU_SKILL_LEARNED(&gActiveBattleUnit));
		BU_SKILL_LEARNED(&gActiveBattleUnit) = 0;

		return TRUE;
	}
	else if (BU_SKILL_LEARNED(&gTargetBattleUnit))
	{
		SetPopupItem(BU_SKILL_LEARNED(&gTargetBattleUnit));
		BU_SKILL_LEARNED(&gTargetBattleUnit) = 0;

		return TRUE;
	}

	return FALSE;
}
