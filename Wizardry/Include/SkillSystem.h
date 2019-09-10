#ifndef FE8_SKILL_SYSTEM_H
#define FE8_SKILL_SYSTEM_H

#include "gbafe.h"

enum
{
	// Skill System constants

	SKILL_SAVED_COUNT = 6,
};

enum
{
	// Special skill ids

	SKILL_ALWAYS_ID = 0x00, // units always have this skill
	SKILL_NEVER_ID  = 0xFF, // units never have this skill
};

enum
{
	// skill learning error codes

	SKILL_LEARN_SUCCESS           = 0, // only used for functions without other results

	SKILL_LEARN_ERR_ALREADY_KNOWN = -2,
	SKILL_LEARN_ERR_NO_ROOM       = -3,
};

// Skill id and forgot skill slot id occupy what were previously padding bytes in the battle unit

#define BU_SKILL_LEARNED(bu)     (*((u8*) (((void*) (bu)) + 0x58)))
#define BU_SKILL_FORGET_SLOT(bu) (*((s8*) (((void*) (bu)) + 0x7F)))

// in SkillSystemCore

u8* SS_GetUnitSkillArray(struct Unit* unit);
void SS_ClearUnitSkills(struct Unit* unit);
int SS_CountUnitSkills(struct Unit* unit);
int SS_UnitHasSkill(struct Unit* unit, unsigned sid);
int SS_UnitGetFreeSkillSlot(struct Unit* unit);
void SS_UnitForgetSkillSlot(struct Unit* unit, unsigned slot);
void SS_UnitSetSkillSlot(struct Unit* unit, unsigned slot, unsigned sid);
int SS_UnitLearnSkill(struct Unit* unit, unsigned skillId);
int SS_GetSkillDescId(unsigned sid);

// in SSLearningLevelUp

unsigned SS_UnitGetLevelUpSkill(struct Unit* unit);
void SS_UnitAutolevelSkills(struct Unit* unit);

#endif // FE8_SKILL_SYSTEM_H
