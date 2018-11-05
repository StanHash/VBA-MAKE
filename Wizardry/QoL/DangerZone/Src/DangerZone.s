.thumb

.global DangerZoneHook
.type   DangerZoneHook, %function

PlayerPhase_Idle.handle_start = (0x0801CAEC+1)
PlayerPhase_Idle.finish       = (0x0801CB38+1)
PlayerPhase_Idle.end          = (0x0801CB64+1)

DangerZoneHook:
	@ jumpToHack from FE8U:0801CAD4
	@ Known state:
	@ r6 = E_PLAYERPHASE proc

	ldr  r1, =gKeyStatus
	ldrh r1, [r1, #0x08] @ KeyStatus.pressedKeys

	mov r0, #0x04 @ Select button

	tst r1, r0
	bne YesSelect

	mov r0, #0x08 @ Start button

	tst r1, r0
	bne YesStart

	@ Nothing here

	ldr r3, =PlayerPhase_Idle.finish
	bx  r3

YesSelect:
	@ gActiveUnit = NULL;

	ldr r1, =gActiveUnit
	mov r0, #0
	str r0, [r1]

	@ End player phase info windows

	ldr r3, =EndPlayerPhaseSideWindows
	bl  BXR3

	@ go to danger zone player phase proc label

	mov r0, r6
	mov r1, #12

	ldr r3, =GotoProcLabel
	bl  BXR3

	@ go to end of function

	ldr r3, =PlayerPhase_Idle.end
	bx  r3

YesStart:
	ldr r3, =PlayerPhase_Idle.handle_start
BXR3:	bx  r3
