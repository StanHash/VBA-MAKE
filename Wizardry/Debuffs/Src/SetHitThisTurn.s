.thumb

origin = 0x802B422

on_hit = (0x802B44C - origin)
end    = (0x802B55E - origin)

	@ This is going in place at $02B422
	@ This sets bit 2 if BattleUnit+7C when hit

	cmp r0, #0             @ 22
	bne new_hit            @ 24

	ldr r3, [r5]           @ 26 @ load current round pointer
	ldr r1, [r3]           @ 28 @ load current round data
	mov r0, #2             @ 2A @ set the miss flag
	orr r0, r1             @ 2C
	str r0, [r3]           @ 2E @ update current round data
	b end                  @ 30 @ end!

new_hit:
	mov  r0, #0x7C         @ 32
	ldrb r1, [r6, r0]      @ 34
	mov  r2, #2            @ 36
	orr  r1, r2            @ 38
	strb r1, [r6, r0]      @ 3A
	b on_hit               @ 3C

@ mov r8, r8                     @ 3E

@ .long 0x0203A4D4               @ 40
@ .long 0x0203A608               @ 44
@ .long 0xFFF80000               @ 48
