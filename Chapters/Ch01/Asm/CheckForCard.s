.thumb

@ I don't have the original source (by Leonarth)
@ So I decompiled it from the binary
@ And cleaned it up from there.

CheckForCard:
	push {lr}

	@ Sore 0 (false) to Event Slot B

	movs r0, #0
	ldr  r1, lEventSlotB
	str  r0, [r1]

	@ Get current action Unit (not active unit?)

	ldr r0, lGetUnit
	mov lr, r0

	ldr  r2, lActionData
	add  r2, #0xC
	ldrb r0, [r2]

	.short 0xF800 @ bl lr+0

	@ r0 = &Unit.item[0]
	add  r0, #0x1E

	ldrb r2, [r0]

	cmp  r2, #0x72
	beq  found_card

	@ r0 = &Unit.item[1]
	add  r0, #2
	ldrb r2, [r0]

	cmp  r2, #0x72
	beq  found_card

	@ r0 = &Unit.item[2]
	add  r0, #2
	ldrb r2, [r0]

	cmp  r2, #0x72
	beq  found_card

	@ r0 = &Unit.item[3]
	add  r0, #2
	ldrb r2, [r0]

	cmp  r2, #0x72
	beq  found_card

	@ r0 = &Unit.item[4]
	add  r0, #2
	ldrb r2, [r0]

	cmp  r2, #0x72
	beq  found_card

	add  r0, #2

	b end

found_card:
	@ Remove item from inventory
	mov  r2, #0
	strh r2, [r0]
	mov  r2, #1

	@ Store 1 (true) to event slot B
	ldr r1, lEventSlotB
	str r2, [r1]

end:
	pop {r0}
	bx r0

.align

lEventSlotB: .word 0x030004E4 @ gEventSlot + (4*0xB)
lGetUnit:    .word 0x08019430 @ GetUnit
lActionData: .word 0x0203A958 @ gActionData
