#ifndef DEBUFFS_H
#define DEBUFFS_H

#include "gbafe.h"

enum {
	DEBUFF_TABLE_SIZE = 137 // one for each Unit in the unit array
};

/* Old setup
 0-2: Debuffs, 4 bits each (str/skl/spd/def/res/luk)
 3: Rallys (bit 7 = rally move, bit 8 = rally spectrum)
 4: Str/Skl Silver Debuff (5 bits), bit 6 = half magic?, bit 7 = half strength, HO bit = Hexing Rod
 5: Magic
 6: LO Nibble = Dual Guard Meter?
 7: unused3
*/

struct DebuffEntry {
	/* 000 */ unsigned strDebuff : 4;
	/* 004 */ unsigned magDebuff : 4;
	/* 008 */ unsigned sklDebuff : 4;
	/* 012 */ unsigned spdDebuff : 4;
	/* 016 */ unsigned defDebuff : 4;
	/* 020 */ unsigned resDebuff : 4;
	/* 024 */ unsigned lukDebuff : 4;
	/* 028 */ unsigned rallyBits : 9;
	/* 037 */ /* end */
} __attribute__((packed, aligned(4)));

_Static_assert(sizeof(struct DebuffEntry) == 8, "Size of debuff entry is expected to be 8!");

enum {
	DEBUFF_RALLY_STR = (1 << 0),
	DEBUFF_RALLY_MAG = (1 << 1),
	DEBUFF_RALLY_SKL = (1 << 2),
	DEBUFF_RALLY_SPD = (1 << 3),
	DEBUFF_RALLY_DEF = (1 << 4),
	DEBUFF_RALLY_RES = (1 << 5),
	DEBUFF_RALLY_LUK = (1 << 6),
	DEBUFF_RALLY_MOV = (1 << 7),
	DEBUFF_RALLY_SPC = (1 << 8),
};

extern struct DebuffEntry* const gpDebuffTable;

#endif // DEBUFFS_H
