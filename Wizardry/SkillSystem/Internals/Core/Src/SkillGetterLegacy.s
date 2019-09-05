
	.thumb

	SkillsBuffer = 0x202B6D0

	.global SkillGetterLegacy
	.type   SkillGetterLegacy, %function

SkillGetterLegacy:
	push {r4, lr}

	mov r4, r0 @ var r4 = unit
	@          @ arg r0 = unit

	bl SS_GetUnitSkillArray

	mov r3, r0 @ tmp r3 = unit skill array
	mov r0, r4 @ arg r0 = unit
	mov r4, r3 @ var r4 = unit skill array

	bl SS_CountUnitSkills

	mov r1, r0 @ var r1 = count (result)
	           @ var r0 = counter

	ldr r3, =SkillsBuffer

lop:
	sub r0, #1
	blt finish

	ldrb r2, [r4, r0] @ load from array[i]
	strb r2, [r3, r0] @ save to buffer[i]

	b lop

finish:
	mov  r0, #0
	strb r0, [r3, r1] @ buffer[count] = 0

	           @ result r1 = count @ PLEASE DO NOT USE (kept for compatibility)
	mov r0, r3 @ result r0 = buffer

	pop {r4}

	pop {r2}
	bx r2
