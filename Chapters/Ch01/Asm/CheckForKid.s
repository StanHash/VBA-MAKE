.thumb

@ I don't have the original source (by Leonarth)
@ So I decompiled it from the binary
@ And cleaned it up from there.

CheckForKid:
	push {lr}

	@ Sore 0 (false) to Event Slot B

	mov r0, #0
	ldr r1, lEventSlotB
	str r0, [r1]

	@ Get current action Unit (not active unit?)

	ldr r0, lGetUnit
	mov lr, r0

	ldr  r2, lActionData
	add  r2, #0xC
	ldrb r0, [r2]

	.short 0xF800 @ bl lr+0

	@ Get rescuing unit

	add  r0, #0x1B
	ldrb r0, [r0]

	ldr r2, lGetUnit
	mov lr, r2

	.short 0xF800 @ bl lr+0

	@ Get Character index
	ldr  r0, [r0]
	ldrb r0, [r0, #4]

	cmp r0, #0x46
	beq yes_kid

	b end

yes_kid:
	@ Store 1 (true) to event slot B
	mov r2, #1
	ldr r1, lEventSlotB
	str r2, [r1]

end:
	pop {r0}
	bx r0

.align

lEventSlotB: .word 0x030004E4 @ gEventSlot + (4*0xB)
lGetUnit:    .word 0x08019430 @ GetUnit
lActionData: .word 0x0203A958 @ gActionData
