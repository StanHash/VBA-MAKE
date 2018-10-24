#include "SkillSystem.h"

struct SkillsBWL {
	/* 00 */ u8 _pad00;
	/* 01 */ u8 skills[4];
	/* 05 */ u8 _pad05[0x10 - 0x05];
};

extern struct SkillsBWL gBWLDataArray[];

extern const u8* const LevelUpSkillTable[];

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
static u8* GetUnitSavedSkillsArray(const struct Unit* unit) {
	if (IsUnitGeneric(unit))
		return NULL;

	return gBWLDataArray[unit->pCharacterData->number].skills;
}

/*!
 * \param unit unit
 * \param buffer where to write null-terminated skill list to
 * \return number of skills in list (not counting the terminating zero)
 */
int SS_GenerateUnitInitialLearnedSkillList(const struct Unit* unit, u8 buffer[]) {
	unsigned count = 0;

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

	buffer[count] = 0;

	return count;
}

/*!
 * \param unit unit
 * \param buffer buffer in which to write null-terminated skill list
 * \return number of skills in list (not counting the terminating zero)
 */
int SS_GetUnitLearnedSkillList(const struct Unit* unit, u8 buffer[]) {
	// For generic units, compute learned skills as if they were just loaded

	if (IsUnitGeneric(unit))
		return SS_GenerateUnitInitialLearnedSkillList(unit, buffer);

	// For non-generic units, load learned skills from their bwl entry

	unsigned count = 0;

	const u8* learnedSkills = GetUnitSavedSkillsArray(unit);

	for (unsigned i = 0; i < SKILL_SAVED_COUNT; ++i)
		if (learnedSkills[i])
			buffer[count++] = learnedSkills[i];

	buffer[count] = 0;
	return count;
}

/*!
 * \param unit unit
 */
void SS_InitializeUnitLearnedSkills(struct Unit* unit) {
	// If unit is generic, no initialization needed

	if (IsUnitGeneric(unit))
		return;

	// Generate initial skill list

	u8 buffer[SKILL_BUFFER_SIZE];

	unsigned count = SS_GenerateUnitInitialLearnedSkillList(unit, buffer);

	// Cap count if too much

	if (count > SKILL_SAVED_COUNT)
		count = SKILL_SAVED_COUNT;

	// Get saved array

	u8* learnedList = GetUnitSavedSkillsArray(unit);

	// Add initial skills to learned list

	for (unsigned i = 0; i < count; ++i)
		learnedList[i] = buffer[i];

	// Fill the rest with zero

	for (unsigned i = count; i < SKILL_SAVED_COUNT; ++i)
		learnedList[i] = 0;
}

/*!
 * \param unit the unit who to learn the skill to
 * \param skillId skill id
 * \return error code (or zero on sucess)
 */
int SS_UnitLearnSkill(struct Unit* unit, unsigned skillId) {
	// Check if unit is generic (which can't learn skills)

	if (IsUnitGeneric(unit))
		return SKILL_LEARN_ERR_GENERIC;

	u8* learnedSkills = GetUnitSavedSkillsArray(unit);

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
