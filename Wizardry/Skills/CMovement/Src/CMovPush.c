
#include "CMovement.h"

extern const u16 gShoveHelpMsg;
extern const u8  gShoveActId;

enum
{
	PUSH_BACK_OFFSET    = 4,
	PUSH_PUSH_OFFSET    = 8,

	PUSH_BACK_DURATION  = 4,
	PUSH_PUSH_DURATION  = 4,
	PUSH_SLIDE_DURATION = 8,
};

static
struct Vec2 ShoveGetTargetPosition(struct Unit* unit)
{
	return CMovGetPushPosition(unit, gActiveUnit->xPos, gActiveUnit->yPos, 1);
}

static
void ShoveSelectOnInit(struct TargetSelectionProc* proc)
{
	BmMapFill(gMapMovement, -1);
	BmMapFill(gMapRange,     0);

	StartBottomHelpText(
		(Proc*) proc,
		GetStringFromIndex(gShoveHelpMsg));
}

static
void ShoveSelectOnEnd(struct TargetSelectionProc* proc)
{
	BmMapFill(gMapMovement, -1);
	BmMapFill(gMapRange,     0);

	// Apparently onSwitchOut isn't called on end? bad

	// Disable MLV
	HideMoveRangeGraphics();
}

static
void ShoveSelectOnSwitchIn(struct TargetSelectionProc* proc, struct TargetEntry* target)
{
	// Change facing
	ChangeActiveUnitFacing(target->x, target->y);

	// Set map display
	struct Vec2 pos = ShoveGetTargetPosition(GetUnit(target->unitIndex));
	gMapMovement[pos.y][pos.x] = 0;

	// Enable MLV
	DisplayMoveRangeGraphics(MLV_MOVE_BLUE);
}

static
void ShoveSelectOnSwitchOut(struct TargetSelectionProc* proc, struct TargetEntry* target)
{
	// Clear map display
	struct Vec2 pos = ShoveGetTargetPosition(GetUnit(target->unitIndex));
	gMapMovement[pos.y][pos.x] = -1;

	// Disable MLV
	HideMoveRangeGraphics();
}

static
int ShoveSelectOnSelect(struct TargetSelectionProc* proc, struct TargetEntry* target)
{
	struct Vec2 pos = ShoveGetTargetPosition(GetUnit(target->unitIndex));

	gActionData.unitActionType = gShoveActId;

	gActionData.targetIndex = target->unitIndex;
	gActionData.xOther = pos.x;
	gActionData.yOther = pos.y;

	return TSE_DISABLE | TSE_END | TSE_PLAY_BEEP | TSE_PLAY_BOOP | TSE_CLEAR_GFX;
}

static const
struct TargetSelectionDefinition sShoveSelect =
{
	.onInit      = ShoveSelectOnInit,
	.onEnd       = ShoveSelectOnEnd,
	.onSwitchIn  = ShoveSelectOnSwitchIn,
	.onSwitchOut = ShoveSelectOnSwitchOut,
	.onAPress    = ShoveSelectOnSelect,
	.onBPress    = GenericSelection_BackToUM_CamWait,
};

static
int ShoveTargetCheck(struct Unit* unit)
{
	struct Vec2 pos = CMovGetPushPosition(unit, gActiveUnit->xPos, gActiveUnit->yPos, 1);

	if (pos.x == unit->xPos && pos.y == unit->yPos)
		return FALSE; // Cannot push unit

	return TRUE;
}

static
int ShoveListTargets(void)
{
	CMovListAdjacentTargets(ShoveTargetCheck);
	return GetTargetListSize();
}

struct ShoveAnimProc
{
	/* 00 */ PROC_HEADER;
	/* 2C */ unsigned cnt;
	/* 30 */ struct Unit* target;
	/* 34 */ struct Vec2 direction;
	/* 3C */ struct Vec2 from;
};

static
struct Vec2 ShoveAnimGetActiveNeutPos(struct ShoveAnimProc* proc)
{
	struct Vec2 const result =
	{
		gActiveUnit->xPos*16,
		gActiveUnit->yPos*16,
	};

	return result;
}

static
struct Vec2 ShoveAnimGetActiveBackPos(struct ShoveAnimProc* proc)
{
	struct Vec2 const result =
	{
		gActiveUnit->xPos*16 - proc->direction.x*PUSH_BACK_OFFSET,
		gActiveUnit->yPos*16 - proc->direction.y*PUSH_BACK_OFFSET,
	};

	return result;
}

static
struct Vec2 ShoveAnimGetActivePushPos(struct ShoveAnimProc* proc)
{
	struct Vec2 const result =
	{
		gActiveUnit->xPos*16 + proc->direction.x*PUSH_PUSH_OFFSET,
		gActiveUnit->yPos*16 + proc->direction.y*PUSH_PUSH_OFFSET,
	};

	return result;
}

static
struct Vec2 ShoveAnimGetTargetFromPos(struct ShoveAnimProc* proc)
{
	struct Vec2 const result =
	{
		proc->from.x*16,
		proc->from.y*16,
	};

	return result;
}

static
struct Vec2 ShoveAnimGetTargetToPos(struct ShoveAnimProc* proc)
{
	struct Vec2 const result =
	{
		proc->target->xPos*16,
		proc->target->yPos*16,
	};

	return result;
}

static
void ShoveAnim_OnInit(struct ShoveAnimProc* proc)
{
	proc->cnt = 0;

	proc->target = GetUnit(gActionData.targetIndex);

	proc->from.x = proc->target->xPos;
	proc->from.y = proc->target->yPos;

	proc->direction.x = proc->target->xPos - gActiveUnit->xPos;
	proc->direction.y = proc->target->yPos - gActiveUnit->yPos;
}

static
void ShoveAnim_OnInitBack(struct ShoveAnimProc* proc)
{
	CMovStartMuSlideAnim(
		MU_GetByUnit(gActiveUnit),
		ShoveAnimGetActiveNeutPos(proc),
		ShoveAnimGetActiveBackPos(proc),
		PUSH_BACK_DURATION,
		(struct Proc*) proc);
}

static
void ShoveAnim_OnInitPush(struct ShoveAnimProc* proc)
{
	CMovStartMuSlideAnim(
		MU_GetByUnit(gActiveUnit),
		ShoveAnimGetActiveBackPos(proc),
		ShoveAnimGetActivePushPos(proc),
		PUSH_PUSH_DURATION,
		(struct Proc*) proc);
}

static
void ShoveAnim_OnInitSlide(struct ShoveAnimProc* proc)
{
	HideUnitSMS(proc->target);

	proc->target->xPos = gActionData.xOther;
	proc->target->yPos = gActionData.yOther;

	UnitFinalizeMovement(proc->target);

	struct MUProc* mu = MU_Create(proc->target);

	MU_SetFacing(mu,
		GetFacingDirection(
			proc->target->xPos, proc->target->yPos,
			gActiveUnit->xPos,  gActiveUnit->yPos));

	CMovStartMuSlideAnim(
		mu,
		ShoveAnimGetTargetFromPos(proc),
		ShoveAnimGetTargetToPos(proc),
		PUSH_SLIDE_DURATION,
		(struct Proc*) proc);

	CMovStartMuSlideAnim(
		MU_GetByUnit(gActiveUnit),
		ShoveAnimGetActivePushPos(proc),
		ShoveAnimGetActiveNeutPos(proc),
		PUSH_SLIDE_DURATION,
		(struct Proc*) proc);
}

static const
struct ProcInstruction sShoveAnimProc[] =
{
	PROC_SET_NAME("Stan:ShoveAnim"),

	PROC_CALL_ROUTINE(ShoveAnim_OnInit),

	PROC_CALL_ROUTINE(ShoveAnim_OnInitBack),
	PROC_YIELD,

	PROC_CALL_ROUTINE(ShoveAnim_OnInitPush),
	PROC_YIELD,

	PROC_CALL_ROUTINE(ShoveAnim_OnInitSlide),
	PROC_YIELD,

	PROC_END
};

// ===============================
// = INTERFACE WITH THE EXTERIOR =
// ===============================

#include "SkillSystem.h"

extern const u8 gShoveSkillId;

s8 CmdShoveIsAvailable(void)
{
	return (SS_UnitHasSkill(gActiveUnit, gShoveSkillId) && ShoveListTargets() > 0)
		? MCA_USABLE
		: MCA_NONUSABLE;
}

void CmdShoveEffect(void)
{
	ShoveListTargets();
	StartTargetSelection(&sShoveSelect);
}

int ActPush(struct Proc* proc)
{
	ProcStartBlocking(sShoveAnimProc, proc);
	return 0;
}
