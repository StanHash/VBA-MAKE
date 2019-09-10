
	.thumb

	.global DebuffClearHook
	.type   DebuffClearHook, %function

DebuffClearHook:
	ldr r3, =Debuff_ClearAll
	bl  BXR3

	@ restore replaced stuff

	ldr r3, =RefreshEntityBmMaps
	bl  BXR3

	ldr r3, =SMS_UpdateFromGameData
	bl  BXR3

	pop {r4-r5}

	pop {r3}
BXR3:	bx  r3
