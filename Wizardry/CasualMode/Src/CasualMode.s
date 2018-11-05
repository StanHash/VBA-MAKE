.thumb

.global CasualModeHook
.type   CasualModeHook, %function

CasualModeHook:
	@ We're hooking from inside an existing function ("KillUnit")
	@ All we need to do is to "kill" the unit in r4
	@ And pop/return (the stack frame only contains the return location)
	@ (og func: FE8U:080183FC, hook is FE8U:08018418)

	push {r4}

	mov r4, r2 @ var r4 = unit

	bl CasualCheck

	@ Load unit state

	ldr r3, [r4, #0x0C]

	@ unset dead bit? Apparently required for pair up compat??
	@ circles what kind of black magic is this again

	mov r2, #4
	bic r3, r2

	@ if r0 == 0: r0 = 0 else: r0 = 1

	neg r1, r0  @ r1 = -r0
	orr r0, r1  @ r0 = r0 | -r0 (top bit is zero if neither r0 or -r0 is negative, so if r0 = 0)
	lsr r0, #31 @ only get top (sign) bit

	mov r1, #1     @ r1 = 1

	mov r2, #1     @ r2 = 1
	add r1, r0, #2 @ r1 = 2 + CasualMode
	lsl r2, r1     @ r2 = (1 << (2 + CasualMode)) = US_DEAD or US_NOT_DEPLOYED

	@ Update unit state

	orr r3, r1 @ US_HIDDEN
	orr r3, r2 @ US_DEAD or US_NOT_DEPLOYED

	str r3, [r4, #0x0C]

	@ go to end if casual mode
	@ only clear supports if not casual mode

	cmp r0, #0
	bne end

	mov r0, r4 @ arg r0 = Unit

	ldr r3, =ClearUnitSupports
	bl BXR3

end:
	pop {r4}

	pop {r0}
	bx  r0

.pool
.align

CasualCheck:
	@ Simply check if the casual mode eid is set

	adr  r0, lEidCasualMode
	ldrb r0, [r0]

	ldr r3, =CheckEventId
BXR3:	bx  r3

.align

lEidCasualMode:
	.byte eidCasualMode
