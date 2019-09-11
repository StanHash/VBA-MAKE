
#include "SkillSystem.h"
#include "PopupRework.h"

extern const u8* const ClassSkillLists[];
extern const u8* const CharacterSkillLists[];

// =======================
// = SKILL LEARNING CORE =
// =======================

/*!
 * \param unit unit
 * \param level level
 * \param classId class to check skills for
 * \return skill they would learn (zero if none)
 */
unsigned SS_GetLevelUpSkill(unsigned level, unsigned charId, unsigned classId)
{
	const u8* charSkills = CharacterSkillLists[charId];

	if (charSkills)
	{
		while (charSkills[0])
		{
			unsigned skLvl = charSkills[0];
			unsigned klass = charSkills[1];

			if ((klass == 0) || (klass == classId))
				if (skLvl == level)
					return charSkills[2];

			charSkills = charSkills + 3;
		}
	}

	const u8* classSkills = ClassSkillLists[classId];

	if (classSkills)
	{
		level = Mod(level, 20);

		while (classSkills[0])
		{
			if (level == classSkills[0])
				return classSkills[1];

			classSkills = classSkills + 2;
		}
	}

	return 0;
}

/*!
 * \param unit unit
 * \param level level
 * \return skill they would learn (zero if none)
 */
unsigned SS_UnitGetLevelUpSkill(struct Unit* unit)
{
	return SS_GetLevelUpSkill(
		(UNIT_CATTRIBUTES(unit) & CA_PROMOTED)
			? 20 + unit->level
			: unit->level,
		unit->pCharacterData->number,
		unit->pClassData->number);
}

/*!
 * \param unit unit to autolevel skills
 */
void SS_UnitAutolevelSkills(struct Unit* unit)
{
	SS_ClearUnitSkills(unit);

	const u8* charSkills = CharacterSkillLists[unit->pCharacterData->number];

	if (charSkills)
	{
		unsigned unitLevel = (UNIT_CATTRIBUTES(unit) & CA_PROMOTED)
			? 20 + unit->level
			: unit->level;

		unsigned unitClass = unit->pCharacterData->number;

		while (charSkills[0])
		{
			unsigned level = charSkills[0];
			unsigned klass = charSkills[1];

			if ((klass == 0) || (klass == unitClass))
				if ((level == 0xFF) || (level <= unitLevel))
					SS_UnitLearnSkill(unit, charSkills[2]);

			charSkills = charSkills + 3;
		}
	}

	const u8* classSkills = ClassSkillLists[unit->pClassData->number];

	if (classSkills)
	{
		while (classSkills[0])
		{
			unsigned level = classSkills[0];

			// Skill is learned if learning level is lower than unit level,
			// Or learning level is 0xFF (used to give promoted generics unpromoted classes' skills)

			if ((level == 0xFF) || (level <= unit->level))
				SS_UnitLearnSkill(unit, classSkills[1]);

			classSkills = classSkills + 2;
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

// ==============
// = ModLU Hook =
// ==============

void SSLearn_UnitLevelUp(struct BattleUnit* bu)
{
	BU_SKILL_LEARNED(bu) = SS_UnitGetLevelUpSkill(&bu->unit);
}

void SSLearn_UnitPromote(struct Unit* unit, u8 classId)
{
	// TODO: figure out how to do skill forgetting on promotion here

	unsigned learn = SS_GetLevelUpSkill(21, unit->pCharacterData->number, classId);

	if (learn)
		SS_UnitLearnSkill(unit, learn);
}

void SSLearn_UnitGainDiff(struct BattleUnit* bu, struct Unit* unit)
{
	u8* newSkills = SS_GetUnitSkillArray(&bu->unit);
	u8* oldSkills = SS_GetUnitSkillArray(unit);

	for (unsigned i = 0; i < SKILL_SAVED_COUNT; ++i)
	{
		if (newSkills[i] != oldSkills[i])
		{
			BU_SKILL_LEARNED(bu) = newSkills[i];
			return;
		}
	}
}

// =============
// = PopR Hook =
// =============

int SS_PopR_InitSkillLearning(void)
{
	if (BU_SKILL_LEARNED(&gBattleActor))
	{
		SetPopupItem(BU_SKILL_LEARNED(&gBattleActor));
		BU_SKILL_LEARNED(&gBattleActor) = 0;

		return TRUE;
	}
	else if (BU_SKILL_LEARNED(&gBattleTarget))
	{
		SetPopupItem(BU_SKILL_LEARNED(&gBattleTarget));
		BU_SKILL_LEARNED(&gBattleTarget) = 0;

		return TRUE;
	}

	return FALSE;
}
