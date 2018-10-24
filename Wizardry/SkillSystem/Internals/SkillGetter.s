.thumb

.set SkillsBuffer, 0x202b6d0 @0x202b156 @0x202a6ac

.global Skill_Getter
.type   Skill_Getter, %function

Skill_Getter:
	push {r4, lr}

	ldr r4, =SkillsBuffer

	@          @ arg r0 = unit
	mov r1, r4 @ arg r1 = buffer

	bl GetUnitSkills

	mov r1, r0 @ result r1 = count @ PLEASE DO NOT USE (kept for compatibility)
	mov r0, r4 @ result r0 = buffer

	pop {r4}

	pop {r2}
	bx r2
