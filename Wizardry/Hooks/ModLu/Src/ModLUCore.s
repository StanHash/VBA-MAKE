.thumb

.global MLUCheckForLevelUp
.type   MLUCheckForLevelUp, %function

.global MLUStandardGetMaxLevel
.type   MLUStandardGetMaxLevel, %function

.global MLUStandardStatGains
.type   MLUStandardStatGains, %function

@ Global symbols it needs to work:
@ - gpGetUnitMaxLevel (pointer to max level getter)
@ - gLevelUpFuncList (null-terminated pointer list containing all routines called on level up)

@ Unit struct field offset definitions
Unit.pCharData  = 0x00 @ word
Unit.pClassData = 0x04 @ word
Unit.level      = 0x08 @ byte
Unit.exp        = 0x09 @ byte
Unit.aiFlags    = 0x0A @ byte
Unit.index      = 0x0B @ byte
Unit.state      = 0x0C @ word
@ ...

@ BattleUnit struct field offset definitions
BattleUnit.expGained = 0x6E
@ ... (am lazy)

MLUCheckForLevelUp:
	@ LynJump from 0802BA4C
	@ Which is in the middle of CheckForLevelUp

	@ KNOWN STATE:
	@	stack frame is 0x30 bytes (0x10 + 8 pushes)
	@	r7 = bu (BattleUnit)
	@	r3 = exp amount (over 100!)
	@	r4-r6 and r8-r10 are free

	@ Store new exp value

	sub  r3, #100
	strb r3, [r7, #Unit.exp]

	@ r4 = extra exp value (we may need it later)
	mov r4, r3

	@ Increment level
	@ r5 = incremented level

	ldrb r0, [r7, #Unit.level]
	add  r5, r0, #1
	strb r5, [r7, #Unit.level]

	@ Get max level

	mov r0, r7 @ arg r0 = unit

	ldr r3, =gpGetUnitMaxLevel
	ldr r3, [r3]

	bl BXR3

	@ Compare level to max level

	cmp r0, r5
	bne MLUCheckForLevelUp.skip_cap_level

	@ Here our unit has reached its maximum level

	@ Update exp gained to not contain the extra bit

	mov  r1, #BattleUnit.expGained
	ldrb r0, [r7, r1]
	sub  r0, r4 @ expGained - (expTotal - 100)
	strb r0, [r7, r1]

	@ Set exp to max

	mov  r0, #0xFF
	strb r0, [r7, #Unit.exp]

MLUCheckForLevelUp.skip_cap_level:
	@ Here is where it gets interesting :)

	@ r6 = (MLUCheckForLevelUp.loop+1)
	@ we gonna put that into lr instead of using bl
	@ gotta love them micro optimizations

	adr r6, MLUCheckForLevelUp.loop
	add r6, #1

	@ r4 = list iterator

	ldr r4, =gLevelUpFuncList

MLUCheckForLevelUp.loop:
	@ r3 = *it++
	ldmia r4!, {r3}

	@ End the routine if we reached list terminator

	cmp r3, #0
	beq MLUCheckForLevelUp.end

	@ "call" routine with r0 = bu pointer
	@ lr = MLUCheckForLevelUp.loop (with thumb bit)

	mov r0, r7

	mov lr, r6
BXR3:	bx  r3

MLUCheckForLevelUp.end:
	add sp, #0x10

	pop {r3-r5}
	mov r8,  r3
	mov r9,  r4
	mov r10, r5

	pop {r4-r7}

	pop {r0}
	bx  r0

.pool
.align

MLUStandardGetMaxLevel:
	ldr r1, [r0, #Unit.pCharData]
	ldr r2, [r0, #Unit.pClassData]

	ldr r1, [r1, #0x28]
	ldr r2, [r2, #0x28]

	orr r1, r2

	@ r0 = 0x80000 (CA_MAXLEVEL10 or something idr)
	mov r0, #1
	lsl r0, #19

	tst r1, r0
	bne MLUStandardGetMaxLevel.level10

	mov r0, #20
	bx  lr

MLUStandardGetMaxLevel.level10:
	mov r0, #10
	bx  lr

.pool
.align

MLUStandardStatGains:
	@ this is just a wrapper around the second part of vanilla CheckForLevelUp
	@ aka the part that does the stat gains

	push {r4-r7, lr}

	mov  r7, r10
	mov  r6, r9
	mov  r5, r8
	push {r5-r7}

	sub  sp, sp, #0x10

	mov r7, r0

	ldr r3, =(0x0802BA8A+1)
	bx  r3
