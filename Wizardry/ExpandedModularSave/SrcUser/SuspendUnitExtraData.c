#include "ModularSave.h"

extern void* const gpSuspendExtraDataRAM;

void SUE_SaveExtraData(void* target, unsigned size) {
	WriteAndVerifySramFast(gpSuspendExtraDataRAM, target, size);
}

void SUE_LoadExtraData(void* source, unsigned size) {
	gpReadSramFast(source, gpSuspendExtraDataRAM, size);
}
