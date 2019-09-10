.thumb

@ Since Teq's routine was very (too?) well commented
@ I tried to put in as much as I could in here too
@ Without making it so much that it's not understandable hopefully

@ I'm basically replacing his hack
@ So logic dictates that I need to bring
@ At least around the same level of quality

@ :)

.global MGGIncreaseStats
.type   MGGIncreaseStats, %function

MGGIncreaseStats:
	@ ARGUMENTS:
	@ r0 = Battle Unit

	push {r4-r5, lr}

	@ VARIABLES:
	mov r4, r0 @ r4 = unit
	mov r5, #0 @ r5 = stat index

lop:
	@ Get current growth getter in r3

	adr r3, GrowthGetterLookup @ Get address of GrowthGetterLookup
	lsl r2, r5, #2             @ Index the lookup by stat index
	ldr r3, [r3, r2]           @ Load word at [GrowthGetterLookup + 4*StatIndex]

	@ Call growth getter

	mov r0, r4 @ arg r0 = battle unit
	mov r1, r5 @ arg r1 = stat index (for conveniance)

	bl  BXR3

	@ Get random stat increase from growth

	@ @ arg r0 = growth value (may be over 100!)

	ldr r3, =GetStatIncrease
	bl  BXR3

	@ Store stat increase in the battle unit

	@ Not actually increasing stat yet!
	@ This will be done upon saving the battle unit back into the unit

	mov  r1, #0x73    @ +0x73 in the battle struct is the start of the stat increases
	add  r1, r5       @ Add that to the current stat index
	strb r0, [r4, r1] @ Store byte!

	@ Increment stat index and loop

	add r5, #1 @ Stat index += 1

	cmp r5, #7 @ We have 7 stats (hp-pow-skl-spd-def-res-luk)
	bne lop

	@ Here we at the end of the routine
	@ Time to check if we didn't get past the caps!

	@ Getting the unit from the battle unit

	@ Because the battle unit's stats have all modifiers taken into account
	@ We need to get the regular unit to access the base stats

	ldrb r0, [r4, #0x0B] @ unit index

	ldr r3, =GetUnit
	bl  BXR3

	@ Now we call the useful level cap checking routine

	@          @ arg r0 = unit
	mov r1, r4 @ arg r1 = battle unit

	ldr r3, =CheckBattleUnitStatCaps
	bl  BXR3

	@ That's it! We at the end
	@ Time to go back to wherever we came from

	pop {r4-r5}

	pop {r3}
BXR3:	bx  r3

.align

@ Those are going to be automatically converted to a series of EA POIN
@ Thanks to lyn :)

GrowthGetterLookup:
	.word GetUnitHpGrowth  @ 0
	.word GetUnitPowGrowth @ 1
	.word GetUnitSklGrowth @ 2
	.word GetUnitSpdGrowth @ 3
	.word GetUnitDefGrowth @ 4
	.word GetUnitResGrowth @ 5
	.word GetUnitLukGrowth @ 6

	@ le compte est bon
