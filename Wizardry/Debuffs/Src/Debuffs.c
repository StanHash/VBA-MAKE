#include "Debuffs.h"

// ===============
// = DEBUFF CORE =
// ===============

static int IsUnitInUnitArray(struct Unit* unit) {
	ptrdiff_t diff = (unit - gUnitArray);
	return (diff >= 0) && (diff < 137);
}

struct DebuffEntry* Debuff_GetEntryForUnit(struct Unit* unit) {
	if (unit->pCharacterData && IsUnitInUnitArray(unit))
		return gpDebuffTable + (unit - gUnitArray);

	return NULL;
}

void Debuff_ClearAll(void) {
	CpuFill32(0, gpDebuffTable, DEBUFF_TABLE_SIZE * sizeof(struct DebuffEntry));
}

void Debuff_Copy(struct Unit* source, struct Unit* target) {
	struct DebuffEntry* srcDebuff = Debuff_GetEntryForUnit(source);
	struct DebuffEntry* dstDebuff = Debuff_GetEntryForUnit(target);

	if (srcDebuff && dstDebuff)
		memcpy(dstDebuff, srcDebuff, sizeof(struct DebuffEntry));
}

void Debuff_Clear(struct Unit* unit) {
	struct DebuffEntry* debuff = Debuff_GetEntryForUnit(unit);

	if (debuff)
		memset(debuff, 0, sizeof(struct DebuffEntry));
}

void Debuff_TurnUpdate(struct Unit* unit) {
	struct DebuffEntry* debuff = Debuff_GetEntryForUnit(unit);

	if (debuff) {
		#define DEBUFF_DEPLETE(aEntry, statName) do { \
			if (((aEntry)->statName ## Debuff) > 0) \
				((aEntry)->statName ## Debuff)--; \
		} while (0)

		DEBUFF_DEPLETE(debuff, str);
		DEBUFF_DEPLETE(debuff, mag);
		DEBUFF_DEPLETE(debuff, skl);
		DEBUFF_DEPLETE(debuff, spd);
		DEBUFF_DEPLETE(debuff, def);
		DEBUFF_DEPLETE(debuff, res);
		DEBUFF_DEPLETE(debuff, luk);

		#undef DEBUFF_DEPLETE

		debuff->rallyBits = 0;
	}
}

// =========================
// = DEBUFF STAT MODIFIERS =
// =========================

#define DECL_DEBUFF_MODIFY(StatName, statName) \
int MDebuff_Modify ## StatName(int stat, struct Unit* unit) { \
	struct DebuffEntry* debuff = Debuff_GetEntryForUnit(unit); \
	if (debuff) \
		stat = stat - debuff->statName ## Debuff; \
	return stat; \
}

DECL_DEBUFF_MODIFY(Str, str)
DECL_DEBUFF_MODIFY(Mag, mag)
DECL_DEBUFF_MODIFY(Skl, skl)
DECL_DEBUFF_MODIFY(Spd, spd)
DECL_DEBUFF_MODIFY(Def, def)
DECL_DEBUFF_MODIFY(Res, res)
DECL_DEBUFF_MODIFY(Lck, luk)

#undef DECL_DEBUFF_MODIFY

#define DELC_DEBUFF_RALLY_MODIFY(aBonus, StatName, STAT_NAME) \
int MDebuff_RallyModify ## StatName(int stat, struct Unit* unit) { \
	struct DebuffEntry* debuff = Debuff_GetEntryForUnit(unit); \
	if (debuff && (debuff->rallyBits & DEBUFF_RALLY_ ## STAT_NAME)) \
		stat = stat + (aBonus); \
	return stat; \
}

DELC_DEBUFF_RALLY_MODIFY(4, Str,      STR)
DELC_DEBUFF_RALLY_MODIFY(4, Mag,      MAG)
DELC_DEBUFF_RALLY_MODIFY(4, Skl,      SKL)
DELC_DEBUFF_RALLY_MODIFY(4, Spd,      SPD)
DELC_DEBUFF_RALLY_MODIFY(4, Def,      DEF)
DELC_DEBUFF_RALLY_MODIFY(4, Res,      RES)
DELC_DEBUFF_RALLY_MODIFY(8, Lck,      LUK)
DELC_DEBUFF_RALLY_MODIFY(1, Mov,      MOV)
DELC_DEBUFF_RALLY_MODIFY(2, Spectrum, SPC)

#undef DELC_DEBUFF_RALLY_MODIFY
