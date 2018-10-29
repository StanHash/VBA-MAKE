.thumb

.global GetUnitStatGrowthVanilla
.type   GetUnitStatGrowthVanilla, %function

.global GetUnitBaseGrowth
.type   GetUnitBaseGrowth, %function

.global GetUnitMetisTomeGrowthBonus
.type   GetUnitMetisTomeGrowthBonus, %function

GetUnitStatGrowthVanilla:
	@ arguments:
	@ r0 = battle unit
	@ r1 = stat index

	@ returns:
	@ r0 = total growth

	push {r4-r5, lr}

	mov r4, r0 @ r4 = battle unit

	@ @ arg r0 = battle unit
	@ @ arg r1 = stat index

	bl GetUnitBaseGrowth

	mov r5, r0 @ r5 = base growth

	mov r0, r4 @ arg r0 = battle unit

	bl GetUnitMetisTomeGrowthBonus

	add r0, r5 @ r0 = base growth + metis tome growth bonus

	pop {r4-r5}

	pop {r3}
	bx  r3

.align

GetUnitBaseGrowth:
	@ arguments:
	@ r0 = battle unit
	@ r1 = stat index

	@ returns:
	@ r0 = base character growth

	ldr  r2, [r0, #0] @ get char data

	add  r1, #0x1C    @ get growth offset from stat index
	ldrb r0, [r2, r1] @ load stat growth

	bx   lr

.align

GetUnitMetisTomeGrowthBonus:
	@ arguments:
	@ r0 = battle unit

	@ returns:
	@ r0 = growth bonus

	ldr r1, [r0, #0xC] @ get state bits

	mov r2, #1
	lsl r2, #13 @ r2 = 0x2000 = metis tome flag

	tst r1, r2
	beq return_zero

	mov r0, #5
	bx  lr

return_zero:
	mov r0, #0
	bx  lr
