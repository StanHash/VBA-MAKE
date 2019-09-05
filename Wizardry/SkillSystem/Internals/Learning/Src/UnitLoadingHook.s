.thumb

.global UnitLoadingHook
.type   UnitLoadingHook, %function

ReturnLocation = 0x8017EFC+1

.macro BLX_R3 to
	ldr r3, =\to
	bl BXR3
.endm

UnitLoadingHook:
	push {r4-r7, lr}

	mov r5, r0 @ var r5 = unit

	bl SS_UnitAutolevelSkills

	@ Restore replaced code

	mov r0, r5 @ arg r0 = unit

	BLX_R3 GetROMUnitSupportCount

	ldr r3, =ReturnLocation
BXR3:	bx  r3
