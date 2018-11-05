#include "gbafe.h"

// TODO: add to libgbafe

void PlayerPhase_CancelAction(struct Proc* proc) __attribute__((long_call));
void RunCharacterEvents(u8 fromChar, u8 toChar) __attribute__((long_call));

int ActionTalk(struct Proc* proc) __attribute__((long_call));
int ActionSupport(struct Proc* proc) __attribute__((long_call));

int NewTalkAction(struct Proc* proc) {
	gGameState.partialActionTaken |= 0x80;
	PlayerPhase_CancelAction(proc);

	/*

	RunCharacterEvents(
		GetUnit(gActionData.subjectIndex)->pCharacterData->number,
		GetUnit(gActionData.targetIndex)->pCharacterData->number
	);

	// */

	return ActionTalk(proc);
}

int NewSupportAction(struct Proc* proc) {
	gGameState.partialActionTaken |= 0x80;
	PlayerPhase_CancelAction(proc);

	return ActionSupport(proc);
}
