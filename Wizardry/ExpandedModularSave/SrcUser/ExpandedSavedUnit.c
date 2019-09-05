#include "ModularSave.h"

#define SAVE_UNIT_COMMON \
	/* 000 */ u8 wRanks[4];           \
	/* 032 */ unsigned state    : 32; \
	/* 064 */ unsigned charId   : 8;  \
	/* 072 */ unsigned classId  : 8;  \
	/* 080 */ unsigned level    : 5;  \
	/* 085 */ unsigned xPos     : 6;  \
	/* 091 */ unsigned yPos     : 6;  \
	/* 097 */ unsigned maxHp    : 7;  \
	/* 104 */ unsigned pow      : 6;  \
	/* 110 */ unsigned skl      : 6;  \
	/* 116 */ unsigned spd      : 6;  \
	/* 122 */ unsigned def      : 6;  \
	/* 128 */ unsigned res      : 6;  \
	/* 134 */ unsigned lck      : 6;  \
	/* 140 */ unsigned conBonus : 5;  \
	/* 145 */ unsigned movBonus : 5;  \
	/* 150 */ unsigned item1    : 8;  \
	/* 158 */ unsigned item2    : 8;  \
	/* 166 */ unsigned item3    : 8;  \
	/* 174 */ unsigned item4    : 8;  \
	/* 182 */ unsigned item5    : 8;  \
	/* 190 */ unsigned itemUse1 : 6;  \
	/* 196 */ unsigned itemUse2 : 6;  \
	/* 202 */ unsigned itemUse3 : 6;  \
	/* 208 */ unsigned itemUse4 : 6;  \
	/* 214 */ unsigned itemUse5 : 6;  \
	/* 220 */ unsigned wRankBit : 8;  \
	/* 228 */ /* end */

#define PackSaveUnitCommon(apSaveUnit, apUnit) do { \
	if (!(apUnit)->pCharacterData) \
		ClearUnit(apUnit); \
	(apSaveUnit)->state    = (apUnit)->state; \
	(apSaveUnit)->charId   = (apUnit)->pCharacterData ? (apUnit)->pCharacterData->number : 0; \
	(apSaveUnit)->classId  = (apUnit)->pClassData ? (apUnit)->pClassData->number : 0; \
	(apSaveUnit)->level    = (apUnit)->level; \
	(apSaveUnit)->xPos     = (apUnit)->xPos; \
	(apSaveUnit)->yPos     = (apUnit)->yPos; \
	(apSaveUnit)->maxHp    = (apUnit)->maxHP; \
	(apSaveUnit)->pow      = (apUnit)->pow; \
	(apSaveUnit)->skl      = (apUnit)->skl; \
	(apSaveUnit)->spd      = (apUnit)->spd; \
	(apSaveUnit)->def      = (apUnit)->def; \
	(apSaveUnit)->res      = (apUnit)->res; \
	(apSaveUnit)->lck      = (apUnit)->lck; \
	(apSaveUnit)->conBonus = (apUnit)->conBonus; \
	(apSaveUnit)->movBonus = (apUnit)->movBonus; \
	(apSaveUnit)->item1    = (apUnit)->items[0] & 0xFF; \
	(apSaveUnit)->item2    = (apUnit)->items[1] & 0xFF; \
	(apSaveUnit)->item3    = (apUnit)->items[2] & 0xFF; \
	(apSaveUnit)->item4    = (apUnit)->items[3] & 0xFF; \
	(apSaveUnit)->item5    = (apUnit)->items[4] & 0xFF; \
	(apSaveUnit)->itemUse1 = (apUnit)->items[0] >> 8; \
	(apSaveUnit)->itemUse2 = (apUnit)->items[1] >> 8; \
	(apSaveUnit)->itemUse3 = (apUnit)->items[2] >> 8; \
	(apSaveUnit)->itemUse4 = (apUnit)->items[3] >> 8; \
	(apSaveUnit)->itemUse5 = (apUnit)->items[4] >> 8; \
	(apSaveUnit)->wRankBit = 0; \
	for (unsigned i = 0, w = 0; i < 8; ++i) { \
		if ((apUnit)->ranks[i]) { \
			(apSaveUnit)->wRankBit |= (1 << i); \
			(apSaveUnit)->wRanks[w++] = (apUnit)->ranks[i]; \
		} \
	} \
} while (0)

#define UnpackSaveUnitCommon(apSaveUnit, apUnit) do { \
	(apUnit)->state          = (apSaveUnit)->state; \
	(apUnit)->pCharacterData = GetCharacterData((apSaveUnit)->charId); \
	(apUnit)->pClassData     = GetClassData((apSaveUnit)->classId); \
	(apUnit)->level          = (apSaveUnit)->level; \
	(apUnit)->xPos           = ((apSaveUnit)->xPos != ((1 << 6)-1)) ? (apSaveUnit)->xPos : -1; \
	(apUnit)->yPos           = ((apSaveUnit)->yPos != ((1 << 6)-1)) ? (apSaveUnit)->yPos : -1; \
	(apUnit)->maxHP          = (apSaveUnit)->maxHp; \
	(apUnit)->pow            = (apSaveUnit)->pow; \
	(apUnit)->skl            = (apSaveUnit)->skl; \
	(apUnit)->spd            = (apSaveUnit)->spd; \
	(apUnit)->def            = (apSaveUnit)->def; \
	(apUnit)->res            = (apSaveUnit)->res; \
	(apUnit)->lck            = (apSaveUnit)->lck; \
	(apUnit)->conBonus       = (apSaveUnit)->conBonus; \
	(apUnit)->movBonus       = (apSaveUnit)->movBonus; \
	(apUnit)->items[0]       = (apSaveUnit)->item1 | ((apSaveUnit)->itemUse1 << 8); \
	(apUnit)->items[1]       = (apSaveUnit)->item2 | ((apSaveUnit)->itemUse2 << 8); \
	(apUnit)->items[2]       = (apSaveUnit)->item3 | ((apSaveUnit)->itemUse3 << 8); \
	(apUnit)->items[3]       = (apSaveUnit)->item4 | ((apSaveUnit)->itemUse4 << 8); \
	(apUnit)->items[4]       = (apSaveUnit)->item5 | ((apSaveUnit)->itemUse5 << 8); \
	for (unsigned i = 0, w = 0; i < 8; ++i) \
		if (((apSaveUnit)->wRankBit & (1 << i))) \
			(apUnit)->ranks[i] = (apSaveUnit)->wRanks[w++]; \
	SetUnitHP((apUnit), GetUnitMaxHP((apUnit))); \
} while (0)

#define SAVE_UNIT_PLAYER_COMMON \
	/* 000 */ u8 supports[6];   \
	/* 048 */ SAVE_UNIT_COMMON; \
	/* 276 */ unsigned exp : 7; \
	/* 283 */ /* end */

#define PackSaveUnitPlayerCommon(apSaveUnit, apUnit) do { \
	PackSaveUnitCommon(apSaveUnit, apUnit); \
	for (unsigned i = 0; i < 6; ++i) \
		(apSaveUnit)->supports[i] = (apUnit)->supports[i]; \
	(apSaveUnit)->exp = (apUnit)->exp; \
} while (0)

#define UnpackSaveUnitPlayerCommon(apSaveUnit, apUnit) do { \
	UnpackSaveUnitCommon(apSaveUnit, apUnit); \
	for (unsigned i = 0; i < 6; ++i) \
		(apUnit)->supports[i] = (apSaveUnit)->supports[i]; \
	(apUnit)->exp = ((apSaveUnit)->exp != ((1 << 7)-1)) ? (apSaveUnit)->exp : -1; \
} while (0)

#define SAVE_UNIT_SUSPEND_COMMON(SAVE_UNIT_BASE) \
	/* 000 */ SAVE_UNIT_BASE;          \
	/* +00 */ unsigned curHp      : 7; \
	/* +07 */ unsigned rescueUnit : 8; \
	/* +15 */ unsigned ballistaId : 7; \
	/* +22 */ unsigned statusId   : 4; \
	/* +26 */ unsigned statusDura : 4; \
	/* +30 */ unsigned torchDura  : 4; \
	/* +34 */ unsigned purwDura   : 4; \
	/* +38 */ /* end */

#define PackSaveUnitSuspendExtra(apSaveUnit, apUnit) do { \
	(apSaveUnit)->curHp      = (apUnit)->curHP; \
	(apSaveUnit)->rescueUnit = (apUnit)->rescueOtherUnit; \
	(apSaveUnit)->ballistaId = (apUnit)->ballistaIndex; \
	(apSaveUnit)->statusId   = (apUnit)->statusIndex; \
	(apSaveUnit)->statusDura = (apUnit)->statusDuration; \
	(apSaveUnit)->torchDura  = (apUnit)->torchDuration; \
	(apSaveUnit)->purwDura   = (apUnit)->barrierDuration; \
} while (0)

#define UnpackSaveUnitSuspendExtra(apSaveUnit, apUnit) do { \
	(apUnit)->curHP           = (apSaveUnit)->curHp; \
	(apUnit)->rescueOtherUnit = (apSaveUnit)->rescueUnit; \
	(apUnit)->ballistaIndex   = (apSaveUnit)->ballistaId; \
	(apUnit)->statusIndex     = (apSaveUnit)->statusId; \
	(apUnit)->statusDuration  = (apSaveUnit)->statusDura; \
	(apUnit)->torchDuration   = (apSaveUnit)->torchDura; \
	(apUnit)->barrierDuration = (apSaveUnit)->purwDura; \
} while (0)

struct GameSaveUnit {
	SAVE_UNIT_PLAYER_COMMON;
} __attribute__((packed, aligned(4)));

static void PackGameSaveUnit(struct GameSaveUnit* saveUnit, struct Unit* unit) {
	PackSaveUnitPlayerCommon(saveUnit, unit);
}

static void UnpackGameSaveUnit(struct GameSaveUnit* saveUnit, struct Unit* unit) {
	UnpackSaveUnitPlayerCommon(saveUnit, unit);
}

struct SuspendSavePlayerUnit {
	/* 000 */ SAVE_UNIT_SUSPEND_COMMON(SAVE_UNIT_PLAYER_COMMON);
	/* +00 */ unsigned supportGainedBits : 8;
	/* +08 */ /* end */
} __attribute__((packed, aligned(4)));

static void PackSuspendSavePlayerUnit(struct SuspendSavePlayerUnit* saveUnit, struct Unit* unit) {
	PackSaveUnitPlayerCommon(saveUnit, unit);
	PackSaveUnitSuspendExtra(saveUnit, unit);

	saveUnit->supportGainedBits = unit->supportBits;
}

static void UnpackSuspendSavePlayerUnit(struct SuspendSavePlayerUnit* saveUnit, struct Unit* unit) {
	UnpackSaveUnitPlayerCommon(saveUnit, unit);
	UnpackSaveUnitSuspendExtra(saveUnit, unit);

	unit->supportBits = saveUnit->supportGainedBits;
}

struct SuspendSaveNonPlayerUnit {
	/* 000 */ SAVE_UNIT_SUSPEND_COMMON(SAVE_UNIT_COMMON);
	/* +00 */ unsigned ai1    : 8;
	/* +08 */ unsigned ai2    : 8;
	/* +16 */ unsigned ai1Cur : 8;
	/* +24 */ unsigned ai2Cur : 8;
	/* +32 */ unsigned aiConf : 16; // ai 3 & 4
	/* +48 */ unsigned aiFlag : 8; // field A in Unit
	/* +56 */ /* end */
} __attribute__((packed, aligned(4)));

static void PackSuspendSaveNonPlayerUnit(struct SuspendSaveNonPlayerUnit* saveUnit, struct Unit* unit) {
	PackSaveUnitCommon(saveUnit, unit);
	PackSaveUnitSuspendExtra(saveUnit, unit);

	saveUnit->ai1    = unit->ai1;
	saveUnit->ai2    = unit->ai2;
	saveUnit->ai1Cur = unit->ai1data;
	saveUnit->ai2Cur = unit->ai2data;
	saveUnit->aiConf = unit->ai3And4;
	saveUnit->aiFlag = unit->aiFlag;
}

static void UnpackSuspendSaveNonPlayerUnit(struct SuspendSaveNonPlayerUnit* saveUnit, struct Unit* unit) {
	UnpackSaveUnitCommon(saveUnit, unit);
	UnpackSaveUnitSuspendExtra(saveUnit, unit);

	unit->ai1     = saveUnit->ai1;
	unit->ai2     = saveUnit->ai2;
	unit->ai1data = saveUnit->ai1Cur;
	unit->ai2data = saveUnit->ai2Cur;
	unit->ai3And4 = saveUnit->aiConf;
	unit->aiFlag  = saveUnit->aiFlag;

	// Initialize unit supports
	// Since this is a non-player unit, save supports doesn't matter?

	if (unit->pCharacterData)
		InitUnitSupports(unit);
}

void ESU_SaveGameUnits(void* target, unsigned size) {
	unsigned count = Div(size, sizeof(struct GameSaveUnit));

	for (unsigned i = 0; i < count; ++i) {
		struct Unit* unit = GetUnit(UA_BLUE + i+1);

		struct GameSaveUnit saveUnit;
		memset(&saveUnit, 0, sizeof(saveUnit));

		PackGameSaveUnit(&saveUnit, unit);
		WriteAndVerifySramFast(&saveUnit, target + i*sizeof(saveUnit), sizeof(saveUnit));
	}
}

void ESU_LoadGameUnits(void* source, unsigned size) {
	unsigned count = Div(size, sizeof(struct GameSaveUnit));

	for (unsigned i = 0; i < count; ++i) {
		struct Unit* unit = GetUnit(UA_BLUE + i+1);

		struct GameSaveUnit saveUnit;
		memset(&saveUnit, 0, sizeof(saveUnit));

		gpReadSramFast(source + i*sizeof(saveUnit), &saveUnit, sizeof(saveUnit));
		UnpackGameSaveUnit(&saveUnit, unit);
	}
}

void ESU_SaveSuspendPlayerUnits(void* target, unsigned size) {
	unsigned count = Div(size, sizeof(struct SuspendSavePlayerUnit));

	for (unsigned i = 0; i < count; ++i) {
		struct Unit* unit = GetUnit(UA_BLUE + i+1);

		struct SuspendSavePlayerUnit saveUnit;
		memset(&saveUnit, 0, sizeof(saveUnit));

		PackSuspendSavePlayerUnit(&saveUnit, unit);
		WriteAndVerifySramFast(&saveUnit, target + i*sizeof(saveUnit), sizeof(saveUnit));
	}
}

void ESU_LoadSuspendPlayerUnits(void* source, unsigned size) {
	unsigned count = Div(size, sizeof(struct SuspendSavePlayerUnit));

	for (unsigned i = 0; i < count; ++i) {
		struct Unit* unit = GetUnit(UA_BLUE + i+1);

		struct SuspendSavePlayerUnit saveUnit;
		memset(&saveUnit, 0, sizeof(saveUnit));

		gpReadSramFast(source + i*sizeof(saveUnit), &saveUnit, sizeof(saveUnit));
		UnpackSuspendSavePlayerUnit(&saveUnit, unit);
	}
}

void ESU_SaveSuspendGreenUnits(void* target, unsigned size) {
	unsigned count = Div(size, sizeof(struct SuspendSaveNonPlayerUnit));

	for (unsigned i = 0; i < count; ++i) {
		struct Unit* unit = GetUnit(UA_GREEN + i+1);

		struct SuspendSaveNonPlayerUnit saveUnit;
		memset(&saveUnit, 0, sizeof(saveUnit));

		PackSuspendSaveNonPlayerUnit(&saveUnit, unit);
		WriteAndVerifySramFast(&saveUnit, target + i*sizeof(saveUnit), sizeof(saveUnit));
	}
}

void ESU_LoadSuspendGreenUnits(void* source, unsigned size) {
	unsigned count = Div(size, sizeof(struct SuspendSaveNonPlayerUnit));

	for (unsigned i = 0; i < count; ++i) {
		struct Unit* unit = GetUnit(UA_GREEN + i+1);

		struct SuspendSaveNonPlayerUnit saveUnit;
		memset(&saveUnit, 0, sizeof(saveUnit));

		gpReadSramFast(source + i*sizeof(saveUnit), &saveUnit, sizeof(saveUnit));
		UnpackSuspendSaveNonPlayerUnit(&saveUnit, unit);
	}
}

void ESU_SaveSuspendEnemyUnits(void* target, unsigned size) {
	unsigned count = Div(size, sizeof(struct SuspendSaveNonPlayerUnit));

	for (unsigned i = 0; i < count; ++i) {
		struct Unit* unit = GetUnit(UA_RED + i+1);

		struct SuspendSaveNonPlayerUnit saveUnit;
		memset(&saveUnit, 0, sizeof(saveUnit));

		PackSuspendSaveNonPlayerUnit(&saveUnit, unit);
		WriteAndVerifySramFast(&saveUnit, target + i*sizeof(saveUnit), sizeof(saveUnit));
	}
}

void ESU_LoadSuspendEnemyUnits(void* source, unsigned size) {
	unsigned count = Div(size, sizeof(struct SuspendSaveNonPlayerUnit));

	for (unsigned i = 0; i < count; ++i) {
		struct Unit* unit = GetUnit(UA_RED + i+1);

		struct SuspendSaveNonPlayerUnit saveUnit;
		memset(&saveUnit, 0, sizeof(saveUnit));

		gpReadSramFast(source + i*sizeof(saveUnit), &saveUnit, sizeof(saveUnit));
		UnpackSuspendSaveNonPlayerUnit(&saveUnit, unit);
	}
}

/*

// I use those in addition to generating asm to know of the real size of each structs

const u32 SIZE_OF_GAME_SAVE_UNIT = sizeof(struct GameSaveUnit);
const u32 SIZE_OF_SUSPEND_SAVE_PLAYER_UNIT = sizeof(struct SuspendSavePlayerUnit);
const u32 SIZE_OF_SUSPEND_SAVE_NON_PLAYER_UNIT = sizeof(struct SuspendSaveNonPlayerUnit);

*/
