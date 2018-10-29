#ifndef FE8_SKILL_SYSTEM_H
#define FE8_SKILL_SYSTEM_H

#include "gbafe.h"

enum { // Skill System constants
	SKILL_SAVED_COUNT = 4,
	SKILL_BUFFER_SIZE = 0x10
};

enum { // Special skill ids
	SKILL_ALWAYS_ID = 0x00, // units always have this skill
	SKILL_NEVER_ID  = 0xFF, // units never have this skill
};

enum { // skill learning error codes
	SKILL_LEARN_SUCCESS           = 0, // only used for functions without other results

	SKILL_LEARN_ERR_GENERIC       = -1,
	SKILL_LEARN_ERR_ALREADY_KNOWN = -2,
	SKILL_LEARN_ERR_NO_ROOM       = -3,
};

#pragma long_calls

// in SkillSystemCore

int SS_GetUnitSkillList(const struct Unit* unit, u8 buffer[SKILL_BUFFER_SIZE]);
int SS_IsSkillInList(const u8* list, unsigned skillId);
int SS_UnitHasSkill(const struct Unit* unit, unsigned skillId);
int SS_GetSkillDescId(int skillId);

// in SkillSystemLearning

int SS_GenerateUnitInitialLearnedSkillList(const struct Unit* unit, u8 buffer[]);
int SS_GetUnitLearnedSkillList(const struct Unit* unit, u8 buffer[]);
int SS_UnitCanLearnSkills(const struct Unit* unit);
int SS_UnitGetFreeSkillSlot(const struct Unit* unit);
int SS_UnitForgetSkillSlot(struct Unit* unit, unsigned slot);
int SS_UnitSetSkillSlot(struct Unit* unit, unsigned slot, unsigned skillId);
int SS_UnitLearnSkill(struct Unit* unit, unsigned skillId);
int SS_UnitGetLevelUpSkill(const struct Unit* unit, unsigned level);

#pragma long_calls_off

#endif // FE8_SKILL_SYSTEM_H
