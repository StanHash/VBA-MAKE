
#include "CMovement.h"

/*!
 * Builds target list via checking units adjacent to the active unit
 * \param func returns non-zero if the unit should be added as target
 */
void CMovListAdjacentTargets(int(*func)(struct Unit*))
{
	static const s8 offLut[] = { +1, 0, -1, 0, +1 };

	const int x = gActiveUnit->xPos;
	const int y = gActiveUnit->yPos;

	InitTargets(x, y);

	for (unsigned i = 0; i < 4; ++i)
	{
		const int xOff = offLut[i];
		const int yOff = offLut[i+1];

		const int uid = gMapUnit[y + yOff][x + xOff];

		if (!uid)
			continue;

		struct Unit* const unit = GetUnit(uid);

		if (!UNIT_IS_VALID(unit))
			continue;

		if (!func(unit))
			continue;

		AddTarget(x + xOff, y + yOff, uid, 0);
	}
}

/*!
 * Checks whether the given unit could legally stand on the given coordinates
 * \param unit the unit to check for
 * \param x the x map position
 * \param y the y map position
 * \return non-zero of the position is legal for that unit
 */
int CMovCanUnitBeOnPosition(struct Unit* unit, int x, int y)
{
	if (x < 0 || y < 0)
		return FALSE; // position out of bounds

	if (x >= gMapSize.x || y >= gMapSize.y)
		return FALSE; // position out of bounds

	if (gMapUnit[y][x])
		return FALSE; // a unit is occupying this position

	if (gMapHidden[y][x] & 1)
		return FALSE; // a hidden unit is occupying this position

	return CanUnitCrossTerrain(unit, gMapTerrain[y][x]);
}

/*!
 * Gets 'push position' for the unit being pushed (pusing source needs to be adjacent)
 * \param unit the unit being pushed
 * \param xPusher the x map position of the pushing source
 * \param yPusher the y map position of the pushing source
 * \param maxAmt the maximum amount of tiles the unit can be pushed
 * \return the coordinates the unit would be pushed to
 */
struct Vec2 CMovGetPushPosition(struct Unit* unit, int xPusher, int yPusher, unsigned maxAmt)
{
	struct Vec2 result = { unit->xPos, unit->yPos };

	const int xDir = unit->xPos - xPusher;
	const int yDir = unit->yPos - yPusher;

	for (unsigned i = 0; i < maxAmt; ++i)
	{
		if (!CMovCanUnitBeOnPosition(unit, result.x + xDir, result.y + yDir))
			break;

		result.x += xDir;
		result.y += yDir;
	}

	return result;
}

struct MuSlideAnimProc
{
	/* 00 */ PROC_HEADER;

	/* 2C */ struct MUProc* mu;
	/* 30 */ struct Vec2 from, to;
	/* 38 */ unsigned cnt;
	/* 3C */ unsigned max;
};

static
void MuSlideAnim_OnLoop(struct MuSlideAnimProc* proc)
{
	proc->cnt++;

	const int xDisplay = proc->from.x + Div((proc->to.x - proc->from.x) * proc->cnt, proc->max);
	const int yDisplay = proc->from.y + Div((proc->to.y - proc->from.y) * proc->cnt, proc->max);

	MU_SetDisplayPosition(proc->mu, xDisplay, yDisplay);

	if (proc->cnt == proc->max)
		BREAK_PROC(proc);
}

static const
struct ProcInstruction sMuSlideAnimProc[] =
{
	PROC_SET_NAME("Stan:MuSlideAnim"),
	PROC_LOOP_ROUTINE(MuSlideAnim_OnLoop),

	PROC_END
};

void CMovStartMuSlideAnim(struct MUProc* mu, struct Vec2 from, struct Vec2 to, unsigned duration, struct Proc* parent)
{
	struct MuSlideAnimProc* proc = parent
		? START_PROC_BLOCKING(sMuSlideAnimProc, parent)
		: START_PROC(sMuSlideAnimProc, ROOT_PROC_3);

	proc->mu = mu;

	proc->from = from;
	proc->to   = to;

	proc->cnt = 0;
	proc->max = duration;
}

int CMovMuSlideAnimExists(void)
{
	return ProcFind(sMuSlideAnimProc)
		? TRUE
		: FALSE;
}
