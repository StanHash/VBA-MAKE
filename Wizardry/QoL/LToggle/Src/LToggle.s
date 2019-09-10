.thumb

.global LToggleHook
.type   LToggleHook, %function

LToggleHook:
	ldr  r1, =gKeyState
	ldrh r1, [r1, #4]

	@ r2 = 0x200 (L Button Mask)
	mov r2, #0x20
	lsl r2, #4

	tst r1, r2
	beq Return

	cmp r0, #1
	beq Anim

	mov r0, #1
	b Return

Anim:
	mov r0, #3

Return:
	pop {r1}
	bx r1
