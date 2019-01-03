.thumb 

.global DebuffInitializeHook
.type   DebuffInitializeHook, %function

DebuffInitializeHook:
	@ r0 = gDebuffTable
	ldr r0, =gpDebuffTable
	ldr r0, [r0]

	ldrb r1, [r5, #0x0B] @ Unit index
	lsl  r1, #3          @ *sizeof(DebuffTableEntry) (8)
	add  r0, r1

	@ Clear out all 8 bytes
	mov r1, #0
	str r1, [r0]
	str r1, [r0, #0x4]

	@ Code that we replaced to jump here
	ldrb r0, [r6, #3]      @Level
	lsr  r0, r0, #3
	strb r0, [r5, #0x8]
	mov r1, r5
	add r1, #0x10          @Hidden statuses (e.g. Afa's drops)
	mov r2, r5
	bx lr
