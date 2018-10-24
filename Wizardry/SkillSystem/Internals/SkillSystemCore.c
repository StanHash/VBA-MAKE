#include "gbafe.h"

struct SkillsBWL {
	/* 00 */ u8 _pad00;
	/* 01 */ u8 skills[4];
	/* 05 */ u8 _pad05[0x10 - 0x05];
};

extern struct SkillsBWL gBWLDataArray[];

extern const u8 PersonalSkillTable[];
extern const u8 ClassSkillTable[];
extern const u8* const LevelUpSkillTable[];

extern const u16 SkillDescTable[];
extern const u16 DefaultSkillDesc;

enum {
	SKILL_SAVED_COUNT = 4, 
	SKILL_BUFFER_SIZE = 0x10
};

enum {
	SKILL_ALWAYS_ID = 0x00, // units always have this skill
	SKILL_NEVER_ID  = 0xFF, // units never have this skill
};

/*!
 * \param unit unit to check for
 * \return non-zero if the given unit is to be considered "generic"
 */
static int IsUnitGeneric(const struct Unit* unit) {
	return (unit->pCharacterData->number >= 0x46);
}

/*!
 * \param unit unit
 * \return pointer to read-write skill buffer (size is at least SKILL_SAVED_COUNT)
 */
static u8* GetUnitSavedSkillsBuffer(const struct Unit* unit) {
	if (IsUnitGeneric(unit))
		return NULL;

	return gBWLDataArray[unit->pCharacterData->number].skills;
}

/*!
 * \param unit the unit who's skills to get
 * \param buffer the buffer in which to write null-terminated skill list
 * \return number of skills written to buffer
 */
int SS_GetUnitSkillList(const struct Unit* unit, uint8_t buffer[SKILL_BUFFER_SIZE]) {
	unsigned count = 0;

	// Get personal skill

	unsigned personalSkill = PersonalSkillTable[unit->pCharacterData->number];

	if (personalSkill)
		buffer[count++] = personalSkill;

	// Get class skill

	unsigned classSkill = ClassSkillTable[unit->pClassData->number];

	if (classSkill)
		buffer[count++] = classSkill;

	// Get learned skills

	if (IsUnitGeneric(unit)) {
		// For generic units, compute learned skills from class and level

		const u8* levelUpSkills = LevelUpSkillTable[unit->pClassData->number];

		if (levelUpSkills) {
			while (levelUpSkills[0]) {
				unsigned level = levelUpSkills[0];

				// Skill is learned if learning level is lower than unit level,
				// Or learning level is 0xFF (used to give promoted generics unpromoted classes' skills)

				if ((level == 0xFF) || (level <= unit->level))
					buffer[count++] = levelUpSkills[1];

				levelUpSkills = levelUpSkills + 2;
			}
		}
	} else {
		// For non-generic units, load learned skills from their bwl entry

		const u8* learnedSkills = GetUnitSavedSkillsBuffer(unit);

		for (unsigned i = 0; i < SKILL_SAVED_COUNT; ++i)
			if (learnedSkills[i])
				buffer[count++] = learnedSkills[i];
	}

	buffer[count] = 0; // null-terminate list

	return count;
}

/*!
 * \param list skill list to search in. This can be any length.
 * \param skillId skill id to check for
 * \return non-zero if skill is listed
 *
 * note: this checks for special skill ids 0 and 0xFF and returns accordingly
 */
int SS_IsSkillInList(const uint8_t* list, unsigned skillId) {
	if (skillId == SKILL_NEVER_ID)
		return FALSE;

	if (skillId == SKILL_ALWAYS_ID)
		return TRUE;

	for (unsigned i = 0; list[i]; ++i)
		if (skillId == list[i])
			return TRUE;

	return FALSE;
}

/*!
 * \param unit the unit to check whether it has the skill
 * \param skillId skill id to check for
 * \return non-zero if unit has the skill
 */
int SS_UnitHasSkill(const struct Unit* unit, unsigned skillId) {
	uint8_t skills[SKILL_BUFFER_SIZE];

	SS_GetUnitSkillList(unit, skills);

	return SS_IsSkillInList(skills, skillId);
}

/*!
 * \param  skillId skill id
 * \return text id refering to skill description
 */
int SS_GetSkillDescId(int skillId) {
	unsigned desc = SkillDescTable[skillId];

	return (desc ? desc : DefaultSkillDesc);
}

enum { // SS_UnitLearnSkill error codes
	SKILL_LEARN_SUCCESS           = 0,

	SKILL_LEARN_ERR_GENERIC       = 1,
	SKILL_LEARN_ERR_ALREADY_KNOWN = 2,
	SKILL_LEARN_ERR_NO_ROOM       = 3,
};

/*!
 * \param unit the unit who to learn the skill to
 * \param skillId skill id
 * \return error code (or zero on sucess)
 */
int SS_UnitLearnSkill(struct Unit* unit, unsigned skillId) {
	// Check if unit is generic (which can't learn skills)

	if (IsUnitGeneric(unit))
		return SKILL_LEARN_ERR_GENERIC;

	u8* learnedSkills = GetUnitSavedSkillsBuffer(unit);

	// Check if unit already knows the skill

	for (unsigned i = 0; i < SKILL_SAVED_COUNT; ++i)
		if (learnedSkills[i] == skillId)
			return SKILL_LEARN_ERR_ALREADY_KNOWN;

	// Find room in the list to add the skill

	for (unsigned i = 0; i < SKILL_SAVED_COUNT; ++i) {
		if (learnedSkills[i] == 0) {
			learnedSkills[i] = skillId;
			return SKILL_LEARN_SUCCESS;
		}
	}

	// We didn't find room :(

	return SKILL_LEARN_ERR_NO_ROOM;
}

/*!
 * \param unit unit
 * \param level level
 * \return skill they would learn (zero if none)
 */
int SS_UnitGetLevelUpSkill(const struct Unit* unit, unsigned level) {
	const u8* levelUpSkills = LevelUpSkillTable[unit->pClassData->number];

	if (levelUpSkills) {
		while (levelUpSkills[0]) {
			if (level == levelUpSkills[0])
				return levelUpSkills[1];

			levelUpSkills = levelUpSkills + 2;
		}
	}

	return 0;
}

/*!
 * \param unit unit
 * \return non-zero if the given unit is allowed to learn skills (doesn't check whether it has room for any tho)
 */
int SS_CanUnitLearnSkills(struct Unit* unit) {
	return !IsUnitGeneric(unit);
}
