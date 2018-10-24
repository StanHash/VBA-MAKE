#include "SkillSystem.h"

extern const u8 PersonalSkillTable[];
extern const u8 ClassSkillTable[];

extern const u16 SkillDescTable[];
extern const u16 DefaultSkillDesc;

/*!
 * \param unit the unit who's skills to get
 * \param buffer the buffer in which to write null-terminated skill list
 * \return number of skills written to buffer
 */
int SS_GetUnitSkillList(const struct Unit* unit, u8 buffer[SKILL_BUFFER_SIZE]) {
	unsigned count = 0;

	// TODO: The idea is that this will eventually be modular
	// And call functions from an arbitrary list

	// Get personal skill

	unsigned personalSkill = PersonalSkillTable[unit->pCharacterData->number];

	if (personalSkill)
		buffer[count++] = personalSkill;

	// Get class skill

	unsigned classSkill = ClassSkillTable[unit->pClassData->number];

	if (classSkill)
		buffer[count++] = classSkill;

	// Get learned skills
	count += SS_GetUnitLearnedSkillList(unit, buffer + count);

	// getting the learning skill list already adds a null-terminator
	// buffer[count] = 0; // null-terminate list

	return count;
}

/*!
 * \param list skill list to search in. This can be any length.
 * \param skillId skill id to check for
 * \return non-zero if skill is listed
 *
 * note: this checks for special skill ids 0 and 0xFF and returns accordingly
 */
int SS_IsSkillInList(const u8* list, unsigned skillId) {
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
	u8 skills[SKILL_BUFFER_SIZE];

	SS_GetUnitSkillList(unit, skills);

	return SS_IsSkillInList(skills, skillId);
}

/*!
 * \param skillId skill id
 * \return text id refering to skill description
 */
int SS_GetSkillDescId(int skillId) {
	unsigned desc = SkillDescTable[skillId];

	return (desc ? desc : DefaultSkillDesc);
}
