
	.thumb

	@ NihilTester - given ram data in r0 and skill number in r1, changes r1 to 0xFF before passing it to SkillTester when:
	@ the skill is a skill that Nihil nullifies/negates (so if in the NegatedSkills list)
	@ the skill is being tested in a battle or whilst targeting an enemy
	@ the skill to be tested belongs to either the attacker or the defender (so they are in a battle/targeting)
	@ skill 0 is always true, skill FF is always false.
	@ I probably was really wasteful with the registers but oh well I did not think about that until I had tested a billion skills and I lazy

	.global NihilTester
	.type   NihilTester, %function

NihilTester:
	push {r4-r7, lr}

	sub sp, #0x10 @ make room for skill list (0x10 is defined max skill buffer size)

	@ Check for special skill ids 0 and 0xFF
	@ To get that out of the way

	cmp r1, #0
	beq .ReturnTrue

	cmp r1, #0xFF
	beq .ReturnFalse

	@ Save parameters to safe registers

	mov  r7, r0 @ Unit
	mov  r6, r1 @ SkillId

	@ =================
	@ = PART I: NIHIL =
	@ =================

	@ Check if in battle computations

	ldr  r4, =gBattleStats @ pre-battle data pointer, gonna check if a target has been selected or the fight has started (0x02 if targeting someone, 0x01 if battle started)
	ldrb r4, [r4]

	cmp r4, #0x01
	blo .NihilEnd @ if 0, not in combat, Nihil does nothing

	cmp r4, #0x02
	bhi .NihilEnd @ if 4 also not in combat so Nihil does nothing

	@ Check whether the unit is the attacker or defender

	ldrb r0, [r7, #0x0B]

	ldr  r4, =gBattleActor
	ldrb r4, [r4, #0x0B]

	cmp r0, r4 @check if the skill to test is from the attacker (compares allegiance byte of attacker and unit whose skill is being tested)
	beq .NihilIsAttacker

	ldr  r4, =gBattleTarget
	ldrb r4, [r4, #0x0B]

	cmp r0, r4 @check if the skill to test is from the defender (compares allegiance byte of defender and unit whose skill is being tested)
	beq .NihilIsDefender

	b .NihilEnd @ if neither, skill is never negated, I think this last condition is met when checking for nearby auras and such

.NihilIsAttacker:
	ldr r0, =gBattleTarget @ prepare to get defender skills
	@ ldr r5, =gBattleActor @ and to set lethality to 0 if we find nihil

	b .NihilCheckOther

.NihilIsDefender:
	ldr r0, =gBattleActor @ prepare to get attacker skills
	@ ldr r5, =gBattleTarget @ and to set lethality to 0 if we find nihil

.NihilCheckOther:
	@ Check if Nihil is in opposing unit skill list

	@             @ arg r0 = unit

	adr  r1, lNihilId
	ldrb r1, [r1] @ arg r1 = skill id

	bl SS_UnitHasSkill

	cmp r0, #0
	beq .NihilEnd

	@ Check if the checked skill is in the nihil list

	ldr r0, =NegatedSkills @ arg r0 = list
	mov r1, r6             @ arg r1 = skill id

	bl SS_IsSkillInList

	@ If the skill is in the negated list, this unit never has it
	@ No matter whether it is in its skill list or not

	cmp r0, #0
	bne .ReturnFalse

.NihilEnd:
	@ ===========================
	@ = PART II: CATCH THEM ALL =
	@ ===========================

	@ Check if Catch Them All is in the unit skill list

	mov r0, r7    @ arg r0 = unit

	adr  r1, lCatchThemAllId
	ldrb r1, [r1] @ arg r1 = skill id

	bl SS_UnitHasSkill

	@ If the unit has catch them all, return true

	cmp r0, #0
	bne .ReturnTrue

	@ Check for the skill the standard way

	mov r0, r7 @ arg r0 = unit
	mov r1, r6 @ arg r1 = skill id

	bl SS_UnitHasSkill

	b .End

.ReturnTrue:
	mov r0, #1
	b .End

.ReturnFalse:
	mov r0, #0

.End:
	add sp, #0x10 @ restore the space used for skill buffer

	pop {r4-r7}

	pop {r1}
	bx r1

	.align

lCatchThemAllId:
	.byte CatchEmAllID

	.align

lNihilId:
	.byte NihilID
