.thumb

.global SkillLearnerHook
.type   SkillLearnerHook, %function

ReturnLocation = 0x8076020+1

.macro BLX_R3 to
	ldr r3, =\to
	bl BXR3
.endm

SkillLearnerHook:
	@ This first part is for the "Display Weapon Rank Gained" hack (Teq)
	@ it writes weapon exp to ram, next to where the weapon type would be

	BLX_R3 GetBattleNewWExp

	ldr  r1, =gPopupItem
	strb r0, [r1, #0x2]

	@ Okay thanks Teq, now back to skills

	@ Check if level increased

	ldr  r0, [r4]        @ unit
	ldrb r1, [r0, #0x08] @ level

	mov  r2, #0x70
	ldrb r2, [r0, r2] @ previous level

	cmp r1, r2
	beq .NoNewSkill

	@ Get level up skill for current level

	@ arg r0 = unit
	@ arg r1 = level

	bl SS_UnitGetLevelUpSkill

	cmp r0, #0
	beq .NoNewSkill

	@ Try learn that skill

	mov r1, r0   @ arg r1 = skill
	ldr r0, [r4] @ arg r0 = unit

	bl SkillAdderLegacy

	cmp r0, #0
	beq .NoNewSkill

	@ Store skill id in the popup proc

	mov  r1, #0x4E
	strb r0, [r5, r1]

.NoNewSkill:
	@ Restore replaced stuff

	ldr r0, [r4]

	BLX_R3 DidWeaponBreak

	ldr r3, =ReturnLocation
BXR3:	bx  r3
