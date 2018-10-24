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

static int IsUnitGeneric(const struct Unit* unit) {
	return (unit->pCharacterData->number >= 0x46);
}

int GetUnitSkills(const struct Unit* unit, uint8_t buffer[]) {
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

		const u8* learnedSkills = gBWLDataArray[unit->pCharacterData->number].skills;

		for (unsigned i = 0; i < 4; ++i)
			if (learnedSkills[i])
				buffer[count++] = learnedSkills[i];
	}

	buffer[count++] = 0; // null-terminate list

	return count;
}
