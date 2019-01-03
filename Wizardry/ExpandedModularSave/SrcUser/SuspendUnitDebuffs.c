#include "ModularSave.h"
#include "Debuffs.h"

void SUD_SaveDebuffs(void* target, unsigned size) {
	WriteAndVerifySramFast(gpDebuffTable, target, size);
}

void SUD_LoadDebuffs(void* source, unsigned size) {
	gpReadSramFast(source, gpDebuffTable, size);
}
