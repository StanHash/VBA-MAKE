	.cpu arm7tdmi
	.eabi_attribute 23, 1	@ Tag_ABI_FP_number_model
	.eabi_attribute 24, 1	@ Tag_ABI_align8_needed
	.eabi_attribute 25, 1	@ Tag_ABI_align8_preserved
	.eabi_attribute 26, 1	@ Tag_ABI_enum_size
	.eabi_attribute 30, 4	@ Tag_ABI_optimization_goals
	.eabi_attribute 34, 0	@ Tag_CPU_unaligned_access
	.eabi_attribute 18, 4	@ Tag_ABI_PCS_wchar_t
	.file	"SkillSystemCore.c"
@ GNU C11 (devkitARM release 47) version 7.1.0 (arm-none-eabi)
@	compiled by GNU C version 4.9.2, GMP version 6.0.0, MPFR version 3.1.2, MPC version 1.0.2, isl version none
@ GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
@ options passed:  -I Tools/CLib/include -imultilib thumb
@ -iprefix /home/stan/devkitPro/devkitARM/bin/../lib/gcc/arm-none-eabi/7.1.0/
@ -MMD Wizardry/SkillSystem/Internals/SkillSystemCore.d
@ -MF .MkCache/SkillSystemCore.d -MP
@ -MT Wizardry/SkillSystem/Internals/SkillSystemCore.o
@ -MT Wizardry/SkillSystem/Internals/SkillSystemCore.asm
@ -D__USES_INITFINI__ Wizardry/SkillSystem/Internals/SkillSystemCore.c
@ -mcpu=arm7tdmi -mthumb -mthumb-interwork -mtune=arm7tdmi
@ -auxbase-strip Wizardry/SkillSystem/Internals/SkillSystemCore.asm -Os
@ -Wall -fomit-frame-pointer -ffast-math -ffreestanding -fverbose-asm
@ options enabled:  -faggressive-loop-optimizations -falign-jumps
@ -falign-labels -falign-loops -fassociative-math -fauto-inc-dec
@ -fbranch-count-reg -fcaller-saves -fchkp-check-incomplete-type
@ -fchkp-check-read -fchkp-check-write -fchkp-instrument-calls
@ -fchkp-narrow-bounds -fchkp-optimize -fchkp-store-bounds
@ -fchkp-use-static-bounds -fchkp-use-static-const-bounds
@ -fchkp-use-wrappers -fcode-hoisting -fcombine-stack-adjustments -fcommon
@ -fcompare-elim -fcprop-registers -fcrossjumping -fcse-follow-jumps
@ -fcx-limited-range -fdefer-pop -fdelete-null-pointer-checks
@ -fdevirtualize -fdevirtualize-speculatively -fdwarf2-cfi-asm
@ -fearly-inlining -feliminate-unused-debug-types -fexpensive-optimizations
@ -ffinite-math-only -fforward-propagate -ffp-int-builtin-inexact
@ -ffunction-cse -fgcse -fgcse-lm -fgnu-runtime -fgnu-unique
@ -fguess-branch-probability -fhoist-adjacent-loads -fident -fif-conversion
@ -fif-conversion2 -findirect-inlining -finline -finline-atomics
@ -finline-functions -finline-functions-called-once
@ -finline-small-functions -fipa-bit-cp -fipa-cp -fipa-icf
@ -fipa-icf-functions -fipa-icf-variables -fipa-profile -fipa-pure-const
@ -fipa-reference -fipa-sra -fipa-vrp -fira-hoist-pressure
@ -fira-share-save-slots -fira-share-spill-slots
@ -fisolate-erroneous-paths-dereference -fivopts -fkeep-static-consts
@ -fleading-underscore -flifetime-dse -flra-remat -flto-odr-type-merging
@ -fmerge-constants -fmerge-debug-strings -fmove-loop-invariants
@ -fomit-frame-pointer -foptimize-sibling-calls -fpartial-inlining
@ -fpeephole -fpeephole2 -fplt -fprefetch-loop-arrays -freciprocal-math
@ -freg-struct-return -freorder-blocks -freorder-functions
@ -frerun-cse-after-loop -fsched-critical-path-heuristic
@ -fsched-dep-count-heuristic -fsched-group-heuristic -fsched-interblock
@ -fsched-last-insn-heuristic -fsched-pressure -fsched-rank-heuristic
@ -fsched-spec -fsched-spec-insn-heuristic -fsched-stalled-insns-dep
@ -fschedule-insns2 -fsection-anchors -fsemantic-interposition
@ -fshow-column -fshrink-wrap -fshrink-wrap-separate
@ -fsplit-ivs-in-unroller -fsplit-wide-types -fssa-backprop -fssa-phiopt
@ -fstdarg-opt -fstore-merging -fstrict-aliasing -fstrict-overflow
@ -fstrict-volatile-bitfields -fsync-libcalls -fthread-jumps
@ -ftoplevel-reorder -ftree-bit-ccp -ftree-builtin-call-dce -ftree-ccp
@ -ftree-ch -ftree-coalesce-vars -ftree-copy-prop -ftree-cselim -ftree-dce
@ -ftree-dominator-opts -ftree-dse -ftree-forwprop -ftree-fre
@ -ftree-loop-if-convert -ftree-loop-im -ftree-loop-ivcanon
@ -ftree-loop-optimize -ftree-parallelize-loops= -ftree-phiprop -ftree-pre
@ -ftree-pta -ftree-reassoc -ftree-scev-cprop -ftree-sink -ftree-slsr
@ -ftree-sra -ftree-switch-conversion -ftree-tail-merge -ftree-ter
@ -ftree-vrp -funit-at-a-time -funsafe-math-optimizations -fverbose-asm
@ -fzero-initialized-in-bss -mlittle-endian -mpic-data-is-text-relative
@ -msched-prolog -mthumb -mthumb-interwork -mvectorize-with-neon-quad

	.text
	.align	1
	.global	SS_GetUnitSkillList
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	SS_GetUnitSkillList, %function
SS_GetUnitSkillList:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}	@
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:57: 	unsigned personalSkill = PersonalSkillTable[unit->pCharacterData->number];
	ldr	r3, [r0]	@ tmp192, unit_38(D)->pCharacterData
	ldr	r2, .L25	@ tmp170,
	ldrb	r3, [r3, #4]	@ tmp172,
	ldrb	r2, [r2, r3]	@ _4, PersonalSkillTable
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:53: 	unsigned count = 0;
	movs	r3, #0	@ count,
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:59: 	if (personalSkill)
	cmp	r2, r3	@ _4,
	beq	.L2	@,
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:60: 		buffer[count++] = personalSkill;
	strb	r2, [r1]	@ _4, *buffer_39(D)
	adds	r3, r3, #1	@ count,
.L2:
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:64: 	unsigned classSkill = ClassSkillTable[unit->pClassData->number];
	ldr	r2, [r0, #4]	@ tmp193, unit_38(D)->pClassData
	ldr	r4, .L25+4	@ tmp174,
	ldrb	r2, [r2, #4]	@ tmp176,
	ldrb	r2, [r4, r2]	@ _8, ClassSkillTable
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:66: 	if (classSkill)
	cmp	r2, #0	@ _8,
	beq	.L3	@,
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:67: 		buffer[count++] = classSkill;
	strb	r2, [r1, r3]	@ _8, *_57
	adds	r3, r3, #1	@ count,
.L3:
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:33: 	return (unit->pCharacterData->number >= 0x46);
	ldr	r2, [r0]	@ tmp194, MEM[(const struct CharacterData * *)unit_38(D)]
	ldrb	r2, [r2, #4]	@ _43,
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:71: 	if (IsUnitGeneric(unit)) {
	cmp	r2, #69	@ _43,
	bls	.L4	@,
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:74: 		const u8* levelUpSkills = LevelUpSkillTable[unit->pClassData->number];
	ldr	r2, [r0, #4]	@ tmp195, unit_38(D)->pClassData
	ldrb	r2, [r2, #4]	@ tmp181,
	ldr	r4, .L25+8	@ tmp179,
	lsls	r2, r2, #2	@ tmp182, tmp181,
	ldr	r2, [r2, r4]	@ levelUpSkills, LevelUpSkillTable
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:76: 		if (levelUpSkills) {
	cmp	r2, #0	@ levelUpSkills,
	bne	.L5	@,
.L6:
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:99: 	buffer[count] = 0; // null-terminate list
	movs	r2, #0	@ tmp189,
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:102: }
	movs	r0, r3	@, count
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:99: 	buffer[count] = 0; // null-terminate list
	strb	r2, [r1, r3]	@ tmp189, *_20
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:102: }
	@ sp needed	@
	pop	{r4, r5}
	pop	{r1}
	bx	r1
.L9:
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:83: 				if ((level == 0xFF) || (level <= unit->level))
	cmp	r4, #255	@ _16,
	beq	.L7	@,
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:83: 				if ((level == 0xFF) || (level <= unit->level))
	ldrb	r5, [r0, #8]	@ tmp183,
	cmp	r5, r4	@ tmp183, _16
	bcc	.L8	@,
.L7:
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:84: 					buffer[count++] = levelUpSkills[1];
	ldrb	r4, [r2, #1]	@ _15, MEM[base: levelUpSkills_28, offset: 1B]
	strb	r4, [r1, r3]	@ _15, *_14
	adds	r3, r3, #1	@ count,
.L8:
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:86: 				levelUpSkills = levelUpSkills + 2;
	adds	r2, r2, #2	@ levelUpSkills,
.L5:
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:77: 			while (levelUpSkills[0]) {
	ldrb	r4, [r2]	@ _16, MEM[base: levelUpSkills_28, offset: 0B]
	cmp	r4, #0	@ _16,
	bne	.L9	@,
	b	.L6	@
.L4:
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:44: 	return gBWLDataArray[unit->pCharacterData->number].skills;
	lsls	r2, r2, #4	@ tmp185, _43,
	ldr	r0, .L25+12	@ tmp187,
	adds	r2, r2, #1	@ tmp186,
	adds	r2, r2, r0	@ ivtmp.28, tmp186, tmp187
	adds	r4, r2, #4	@ _9, ivtmp.28,
.L11:
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:95: 			if (learnedSkills[i])
	ldrb	r0, [r2]	@ _18, MEM[base: _44, offset: 0B]
	cmp	r0, #0	@ _18,
	beq	.L10	@,
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:96: 				buffer[count++] = learnedSkills[i];
	strb	r0, [r1, r3]	@ _18, *_67
	adds	r3, r3, #1	@ count,
.L10:
	adds	r2, r2, #1	@ ivtmp.28,
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:94: 		for (unsigned i = 0; i < SKILL_SAVED_COUNT; ++i)
	cmp	r4, r2	@ _9, ivtmp.28
	bne	.L11	@,
	b	.L6	@
.L26:
	.align	2
.L25:
	.word	PersonalSkillTable
	.word	ClassSkillTable
	.word	LevelUpSkillTable
	.word	gBWLDataArray
	.size	SS_GetUnitSkillList, .-SS_GetUnitSkillList
	.align	1
	.global	SS_IsSkillInList
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	SS_IsSkillInList, %function
SS_IsSkillInList:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:113: 		return FALSE;
	movs	r3, #0	@ <retval>,
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:112: 	if (skillId == SKILL_NEVER_ID)
	cmp	r1, #255	@ skillId,
	beq	.L27	@,
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:116: 		return TRUE;
	adds	r3, r3, #1	@ <retval>,
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:115: 	if (skillId == SKILL_ALWAYS_ID)
	cmp	r1, #0	@ skillId,
	beq	.L27	@,
.L29:
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:118: 	for (unsigned i = 0; list[i]; ++i)
	ldrb	r3, [r0]	@ _6, MEM[base: _13, offset: 0B]
	cmp	r3, #0	@ _6,
	bne	.L30	@,
.L27:
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:123: }
	movs	r0, r3	@, <retval>
	@ sp needed	@
	bx	lr
.L30:
	adds	r0, r0, #1	@ ivtmp.35,
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:119: 		if (skillId == list[i])
	cmp	r1, r3	@ skillId, _6
	bne	.L29	@,
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:116: 		return TRUE;
	movs	r3, #1	@ <retval>,
	b	.L27	@
	.size	SS_IsSkillInList, .-SS_IsSkillInList
	.align	1
	.global	SS_UnitHasSkill
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	SS_UnitHasSkill, %function
SS_UnitHasSkill:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, lr}	@
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:130: int SS_UnitHasSkill(const struct Unit* unit, unsigned skillId) {
	movs	r4, r1	@ skillId, skillId
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:133: 	SS_GetUnitSkillList(unit, skills);
	mov	r1, sp	@,
	bl	SS_GetUnitSkillList	@
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:135: 	return SS_IsSkillInList(skills, skillId);
	movs	r1, r4	@, skillId
	mov	r0, sp	@,
	bl	SS_IsSkillInList	@
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:136: }
	add	sp, sp, #16	@,,
	@ sp needed	@
	pop	{r4}
	pop	{r1}
	bx	r1
	.size	SS_UnitHasSkill, .-SS_UnitHasSkill
	.align	1
	.global	SS_GetSkillDescId
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	SS_GetSkillDescId, %function
SS_GetSkillDescId:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:143: 	unsigned desc = SkillDescTable[skillId];
	ldr	r3, .L39	@ tmp114,
	lsls	r0, r0, #1	@ tmp115, skillId,
	ldrh	r0, [r0, r3]	@ <retval>, SkillDescTable
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:145: 	return (desc ? desc : DefaultSkillDesc);
	cmp	r0, #0	@ <retval>,
	bne	.L36	@,
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:145: 	return (desc ? desc : DefaultSkillDesc);
	ldr	r3, .L39+4	@ tmp116,
	ldrh	r0, [r3]	@ <retval>, DefaultSkillDesc
.L36:
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:146: }
	@ sp needed	@
	bx	lr
.L40:
	.align	2
.L39:
	.word	SkillDescTable
	.word	DefaultSkillDesc
	.size	SS_GetSkillDescId, .-SS_GetSkillDescId
	.align	1
	.global	SS_UnitLearnSkill
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	SS_UnitLearnSkill, %function
SS_UnitLearnSkill:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}	@
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:33: 	return (unit->pCharacterData->number >= 0x46);
	ldr	r3, [r0]	@ MEM[(const struct CharacterData * *)unit_12(D)], MEM[(const struct CharacterData * *)unit_12(D)]
	ldrb	r3, [r3, #4]	@ _13,
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:155: 		return FALSE; // Generics can't learn skills
	movs	r0, #0	@ <retval>,
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:154: 	if (IsUnitGeneric(unit))
	cmp	r3, #69	@ _13,
	bhi	.L41	@,
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:44: 	return gBWLDataArray[unit->pCharacterData->number].skills;
	ldr	r2, .L50	@ tmp131,
	lsls	r3, r3, #4	@ tmp129, _13,
	adds	r3, r3, #1	@ tmp130,
	adds	r3, r3, r2	@ ivtmp.48, tmp130, tmp131
	movs	r2, r3	@ ivtmp.54, ivtmp.48
	adds	r0, r3, #4	@ _40, ivtmp.48,
.L43:
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:160: 		if (learnedSkills[i] == skillId)
	ldrb	r4, [r2]	@ MEM[base: _38, offset: 0B], MEM[base: _38, offset: 0B]
	cmp	r4, r1	@ MEM[base: _38, offset: 0B], skillId
	beq	.L47	@,
	adds	r2, r2, #1	@ ivtmp.54,
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:159: 	for (unsigned i = 0; i < SKILL_SAVED_COUNT; ++i)
	cmp	r2, r0	@ ivtmp.54, _40
	bne	.L43	@,
.L45:
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:164: 		if (learnedSkills[i] == 0) {
	ldrb	r0, [r3]	@ MEM[base: _4, offset: 0B], MEM[base: _4, offset: 0B]
	cmp	r0, #0	@ MEM[base: _4, offset: 0B],
	bne	.L44	@,
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:165: 			learnedSkills[i] = skillId;
	strb	r1, [r3]	@ skillId, *_4
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:166: 			return TRUE; // Sucessfully learned skill
	adds	r0, r0, #1	@ <retval>,
.L41:
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:171: }
	@ sp needed	@
	pop	{r4}
	pop	{r1}
	bx	r1
.L44:
	adds	r3, r3, #1	@ ivtmp.48,
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:163: 	for (unsigned i = 0; i < SKILL_SAVED_COUNT; ++i) {
	cmp	r3, r2	@ ivtmp.48, ivtmp.54
	bne	.L45	@,
.L47:
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:155: 		return FALSE; // Generics can't learn skills
	movs	r0, #0	@ <retval>,
	b	.L41	@
.L51:
	.align	2
.L50:
	.word	gBWLDataArray
	.size	SS_UnitLearnSkill, .-SS_UnitLearnSkill
	.align	1
	.global	SS_CanUnitLearnSkills
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	SS_CanUnitLearnSkills, %function
SS_CanUnitLearnSkills:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:33: 	return (unit->pCharacterData->number >= 0x46);
	ldr	r3, [r0]	@ MEM[(const struct CharacterData * *)unit_3(D)], MEM[(const struct CharacterData * *)unit_3(D)]
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:178: 	return !IsUnitGeneric(unit);
	movs	r2, #69	@ tmp120,
	movs	r0, #0	@ tmp121,
	ldrb	r3, [r3, #4]	@ tmp118,
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:179: }
	@ sp needed	@
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:178: 	return !IsUnitGeneric(unit);
	cmp	r2, r3	@ tmp120, tmp118
	adcs	r0, r0, r0	@ tmp119, tmp121, tmp121
@ Wizardry/SkillSystem/Internals/SkillSystemCore.c:179: }
	bx	lr
	.size	SS_CanUnitLearnSkills, .-SS_CanUnitLearnSkills
	.ident	"GCC: (devkitARM release 47) 7.1.0"
