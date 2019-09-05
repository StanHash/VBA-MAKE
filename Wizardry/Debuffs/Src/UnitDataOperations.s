
	.thumb

	.global DebuffClearUnit
	.type   DebuffClearUnit, %function

	.global DebuffCopyUnit
	.type   DebuffCopyUnit, %function

DebuffClearUnit:
	push {r4-r5, lr}

	mov  r4, r0 @ var r4 = unit

	ldrb r5, [r4, #0x0B] @ var r5 = unit id

	@             @ arg r0 = dest
	mov r1, #0    @ arg r1 = value
	mov r2, #0x48 @ arg r2 = size

	ldr r3, =memset
	bl  BXR3

	@ restore unit id
	strb r5, [r4, #0x0B]

	mov r0, r4 @ arg r0 = unit

	bl Debuff_Clear

	pop {r4-r5}

	pop {r3}
BXR3:	bx  r3

	.pool
	.align

DebuffCopyUnit:
	push {r4-r6, lr}

	mov  r4, r0 @ var r4 = source
	mov  r5, r1 @ var r5 = target

	ldrb r6, [r4, #0x0B] @ var r5 = target id

	mov r1, r4    @ arg r1 = source
	mov r0, r5    @ arg r0 = target
	mov r2, #0x48 @ arg r2 = size

	ldr r3, =memcpy
	bl  BXR3

	@ restoring target id
	strb r6, [r5, #0x0B]

	mov r0, r4 @ arg r0 = source unit
	mov r1, r5 @ arg r1 = target unit

	bl Debuff_Copy

	pop {r4-r6}

	pop {r0}
	bx  r0

	.pool
	.align
