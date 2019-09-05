#include "SkillSystem.h"

extern const u16 SkillDescTable[];
extern const u16 DefaultSkillDesc;

/*!
 * \param unit unit
 * \return pointer to read-write skill array of (size is at least SKILL_SAVED_COUNT)
 */
u8* SS_GetUnitSkillArray(struct Unit* unit)
{
	return unit->supports;
}

/*!
 * \param unit unit
 */
void SS_ClearUnitSkills(struct Unit* unit)
{
	memset(SS_GetUnitSkillArray(unit), 0, SKILL_SAVED_COUNT);
}

/*!
 * \param unit the unit who's skill count to get
 * \return number of skills the unit current has
 */
int SS_CountUnitSkills(struct Unit* unit)
{
	unsigned count = 0;
	const u8* list = SS_GetUnitSkillArray(unit);

	while (count < SKILL_SAVED_COUNT)
	{
		if (!list[count])
			break;

		count++;
	}

	return count;
}

/*!
 * \param unit the unit to check whether it has the skill
 * \param sid skill id to check for
 * \return non-zero if unit has the skill
 */
int SS_UnitHasSkill(struct Unit* unit, unsigned sid)
{
	if (sid == SKILL_NEVER_ID)
		return FALSE;

	if (sid == SKILL_ALWAYS_ID)
		return TRUE;

	const u8* list = SS_GetUnitSkillArray(unit);

	for (unsigned i = 0; i < SKILL_SAVED_COUNT; ++i)
	{
		if (sid == list[i])
			return TRUE;
	}

	return FALSE;
}

/*!
 * \param unit unit
 * \return error code (slot id on success)
 */
int SS_UnitGetFreeSkillSlot(struct Unit* unit)
{
	u8* learnedSkills = SS_GetUnitSkillArray(unit);

	for (unsigned i = 0; i < SKILL_SAVED_COUNT; ++i)
		if (!learnedSkills[i])
			return i;

	return SKILL_LEARN_ERR_NO_ROOM;
}

/*!
 * \param unit unit
 * \param slot the slot in which the skill to forget resides
 */
void SS_UnitForgetSkillSlot(struct Unit* unit, unsigned slot)
{
	u8* learnedSkills = SS_GetUnitSkillArray(unit);

	for (unsigned i = slot; i < (SKILL_SAVED_COUNT-1); ++i)
		learnedSkills[i] = learnedSkills[i+1];

	learnedSkills[SKILL_SAVED_COUNT-1] = 0;
}

/*!
 * \param unit unit
 * \param slot skill slot
 * \param sid skill id to put in slot
 */
void SS_UnitSetSkillSlot(struct Unit* unit, unsigned slot, unsigned sid)
{
	SS_GetUnitSkillArray(unit)[slot] = sid;
}

/*!
 * \param unit the unit who to learn the skill to
 * \param sid skill id
 * \return error code (zero/SKILL_LEARN_SUCCESS on success)
 */
int SS_UnitLearnSkill(struct Unit* unit, unsigned sid)
{
	if (SS_UnitHasSkill(unit, sid))
		return SKILL_LEARN_ERR_ALREADY_KNOWN;

	int slot = SS_UnitGetFreeSkillSlot(unit);

	if (slot < 0)
		return slot; // probably SKILL_LEARN_ERR_NO_ROOM

	SS_UnitSetSkillSlot(unit, slot, sid);

	return SKILL_LEARN_SUCCESS;
}

/*!
 * \param sid skill id
 * \return text id refering to skill description
 */
int SS_GetSkillDescId(unsigned sid)
{
	unsigned desc = SkillDescTable[sid];
	return (desc ? desc : DefaultSkillDesc);
}
