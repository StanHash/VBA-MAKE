.thumb

@ given a skill number in r0, get the corresponding text ID
@ wait no circles, that's not what this does at all!

@ what this really is is a jump ladder for getting skill desc in stat screen help text procs
@ the *real* skill desc getter is now in SkillSystemCore.c and is named "GetSkillDescId"

.global SkillDescHelpTextLadder
.type   SkillDescHelpTextLadder, %function

SkillDescHelpTextLadder:
	@ 0
	mov r1, #0
	b .Start

	@ 1
	mov r1, #1
	b .Start

	@ 2
	mov r1, #2
	b .Start

	@ 3
	mov r1, #3
	b .Start

	@ 4
	mov r1, #4
	b .Start

	@ 5
	mov r1, #5
	b .Start

.Start:
	push {r4-r5, lr}

	sub sp, #0x10 @ make room for skill buffer

	mov r4, r0 @ HELP TEXT PROC (but which one specifically? TODO: investigate)
	mov r5, r1 @ Skill Index

	ldr r0, =gpStatScreenUnit
	ldr r0, [r0] @ arg r0 = unit

	mov r1, sp   @ arg r1 = buffer

	bl SS_GetUnitSkillList

	cmp r5, r0
	bhs .End @ Skill Slot is past end of list

	mov  r0, sp
	ldrb r0, [r0, r5]

	bl SS_GetSkillDescId

	mov  r1, r4
	add  r1, #0x4C
	strh r0, [r1]

.End:
	add sp, #0x10 @ free room for skill buffer

	pop {r4-r5}

	pop {r0}
	bx r0
