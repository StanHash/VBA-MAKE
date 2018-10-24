.thumb

SkillsBuffer = 0x202B6D0

.global SkillGetterLegacy
.type   SkillGetterLegacy, %function

SkillGetterLegacy:
	push {r4, lr}

	ldr r4, =SkillsBuffer

	@          @ arg r0 = unit
	mov r1, r4 @ arg r1 = buffer

	bl SS_GetUnitSkillList

	mov r1, r0 @ result r1 = count @ PLEASE DO NOT USE (kept for compatibility)
	mov r0, r4 @ result r0 = buffer

	pop {r4}

	pop {r2}
	bx r2
