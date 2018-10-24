.thumb

@adds a skill to the given unit's list of skills. Returns 0 if unit already has 4 skills
@r0 is unit in ram

.global SkillAdderLegacy
.type   SkillAdderLegacy, %function

SkillAdderLegacy:
	push {r4, lr}

	mov r4, r1

	@ arg r0 = unit
	@ arg r1 = skill

	bl SS_UnitLearnSkill

	cmp r0, #0 @ SKILL_LEARN_SUCCESS
	beq .ReturnTrue

	cmp r0, #3 @ SKILL_LEARN_ERR_NO_ROOM
	bne .ReturnFalse

	@ Write learned skill data to 202BCDE
	@ This is to eventually call the skill removal menu

	@ r1 = (0x8000 | Skill Index) (top bit set to denote it is indeed a skill to be learned)
	mov  r0, #0x80
	lsl  r0, #8
	orr  r0, r4

	@ store
	ldr  r1, =0x202BCDE @ pExtraItemOrSkill, used by vanilla for when it tries to send an item to convoy
	strh r0, [r1]

.ReturnTrue:
	mov r0, #1
	b .End

.ReturnFalse:
	mov r0, #0

.End:
	pop {r4}

	pop {r1}
	bx r1
