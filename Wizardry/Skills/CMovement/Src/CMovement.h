#ifndef CMOVEMENT_INCLUDED
#define CMOVEMENT_INCLUDED

#include "gbafe.h"

// CMovCommon.c

void CMovListAdjacentTargets(int(*func)(struct Unit*));
int CMovCanUnitBeOnPosition(struct Unit* unit, int x, int y);
struct Vec2 CMovGetPushPosition(struct Unit* unit, int xPusher, int yPusher, unsigned maxAmt);
void CMovStartMuSlideAnim(struct MUProc* mu, struct Vec2 from, struct Vec2 to, unsigned duration, struct Proc* parent);
int CMovMuSlideAnimExists(void);

#endif // CMOVEMENT_INCLUDED
