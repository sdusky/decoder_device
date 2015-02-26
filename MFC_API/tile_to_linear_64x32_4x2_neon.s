	.arch armv7-a
	.text
	.global tile_to_linear_64x32_4x2_neon
	.type	tile_to_linear_64x32_4x2_neon, %function
tile_to_linear_64x32_4x2_neon:
	.fnstart
	@r0		p_linear_addr
	@r1		p_tiled_addr
	@r2		x_size
	@r3		y_size
	@r4		i
	@r5		j
	@r6		nn(tiled_addr)
	@r7		mm(linear_addr)
	@r8		temp( trans_addr, loop counter, load buffer )
	@r9		x_size_temp
	@r10	y_size_temp
	@r11	#64, #8

	STMFD		sp!, {r4-r12,r14}

	MOV		r9, r2, ASR #4
	MOV		r9, r9, LSL #4
	MOV		r10, r3, ASR #4
	MOV		r10, r10, LSL #4

	MOV		r11, #64

TILE_TO_LINEAR_64x32_4x2_NEON_MULTIPLE16:
	MOV		r4, #0
TILE_TO_LINEAR_64x32_4x2_NEON_LOOP_MULTIPLE16_OUTER:

	MOV		r5, #0
TILE_TO_LINEAR_64x32_4x2_NEON_LOOP_MULTIPLE16_INNER:
	STMFD		sp!, {r0-r3, ip}
	MOV		r0, r2
	MOV		r1, r3
	MOV		r2, r5
	MOV		r3, r4
	BL		tile_4x2_read_asm
	MOV		r8, r0
	LDMFD		sp!, {r0-r3, ip}

	ADD		r6, r1, r8
	MUL		r7, r2, r4
	ADD		r7, r7, r0
	ADD		r7, r7, r5

	VLD1.8		{Q0}, [r6], r11
	VLD1.8		{Q1}, [r6], r11
	VLD1.8		{Q2}, [r6], r11
	VLD1.8		{Q3}, [r6], r11
	VLD1.8		{Q4}, [r6], r11
	VLD1.8		{Q5}, [r6], r11
	VLD1.8		{Q6}, [r6], r11
	VLD1.8		{Q7}, [r6], r11
	VLD1.8		{Q8}, [r6], r11
	VLD1.8		{Q9}, [r6], r11
	VLD1.8		{Q10}, [r6], r11
	VLD1.8		{Q11}, [r6], r11
	VLD1.8		{Q12}, [r6], r11
	VLD1.8		{Q13}, [r6], r11
	VLD1.8		{Q14}, [r6], r11
	VLD1.8		{Q15}, [r6]

	VST1.8		{Q0}, [r7], r2
	VST1.8		{Q1}, [r7], r2
	VST1.8		{Q2}, [r7], r2
	VST1.8		{Q3}, [r7], r2
	VST1.8		{Q4}, [r7], r2
	VST1.8		{Q5}, [r7], r2
	VST1.8		{Q6}, [r7], r2
	VST1.8		{Q7}, [r7], r2
	VST1.8		{Q8}, [r7], r2
	VST1.8		{Q9}, [r7], r2
	VST1.8		{Q10}, [r7], r2
	VST1.8		{Q11}, [r7], r2
	VST1.8		{Q12}, [r7], r2
	VST1.8		{Q13}, [r7], r2
	VST1.8		{Q14}, [r7], r2
	VST1.8		{Q15}, [r7]

	ADD		r5, r5, #16
	CMP		r5, r9
	BLT		TILE_TO_LINEAR_64x32_4x2_NEON_LOOP_MULTIPLE16_INNER

	ADD		r4, r4, #16
	CMP		r4, r10
	BLT		TILE_TO_LINEAR_64x32_4x2_NEON_LOOP_MULTIPLE16_OUTER

	CMP		r10, r3
	BGE		TILE_TO_LINEAR_64x32_4x2_NEON_REMAINDER_X

TILE_TO_LINEAR_64x32_4x2_NEON_REMAINDER_Y:
	MOV		r11, #8

	MOV		r4, r10
TILE_TO_LINEAR_64x32_4x2_NEON_LOOP_REMAINDER_Y_OUTER:

	MOV		r5, #0
TILE_TO_LINEAR_64x32_4x2_NEON_LOOP_REMAINDER_Y_INNER:
	STMFD		sp!, {r0-r3, ip}
	MOV		r0, r2
	MOV		r1, r3
	MOV		r2, r5
	MOV		r3, r4
	BL		tile_4x2_read_asm
	MOV		r8, r0
	LDMFD		sp!, {r0-r3, ip}

	ADD		r6, r1, r8
	MUL		r7, r2, r4
	ADD		r7, r7, r0
	ADD		r7, r7, r5
	MOV		r8, #8

	VLD1.8		{D0}, [r6], r11
	VLD1.8		{D1}, [r6], r11
	VST1.8		{D0}, [r7], r11
	VST1.8		{D1}, [r7], r11

	ADD		r5, r5, #16
	CMP		r5, r9
	BLT		TILE_TO_LINEAR_64x32_4x2_NEON_LOOP_REMAINDER_Y_INNER

	ADD		r4, r4, #1
	CMP		r4, r3
	BLT		TILE_TO_LINEAR_64x32_4x2_NEON_LOOP_REMAINDER_Y_OUTER

TILE_TO_LINEAR_64x32_4x2_NEON_REMAINDER_X:
	CMP		r9, r2
	BGE		TILE_TO_LINEAR_64x32_4x2_NEON_RESTORE_REG

	MOV		r4, #0
TILE_TO_LINEAR_64x32_4x2_NEON_LOOP_REMAINDER_X_OUTER:

	MOV		r5, r9
TILE_TO_LINEAR_64x32_4x2_NEON_LOOP_REMAINDER_X_INNER:
	STMFD		sp!, {r0-r3, ip}
	MOV		r0, r2
	MOV		r1, r3
	MOV		r2, r5
	MOV		r3, r4
	BL		tile_4x2_read_asm
	MOV		r8, r0
	LDMFD		sp!, {r0-r3, ip}

	AND		r6, r5, #3
	ADD		r6, r6, r8
	ADD		r6, r1, r6
	MUL		r7, r2, r4
	ADD		r7, r7, r0
	ADD		r7, r7, r5

	LDR		r8, [r6]
	STR		r8, [r7]

	ADD		r5, r5, #4
	CMP		r5, r2
	BLT		TILE_TO_LINEAR_64x32_4x2_NEON_LOOP_REMAINDER_X_INNER

	ADD		r4, r4, #1
	CMP		r4, r3
	BLT		TILE_TO_LINEAR_64x32_4x2_NEON_LOOP_REMAINDER_X_OUTER

TILE_TO_LINEAR_64x32_4x2_NEON_RESTORE_REG:
	LDMFD		sp!, {r4-r12,r15}
	.fnend


	.arch armv7-a
	.text
	.global tile_to_linear_64x32_4x2_uv_neon
	.type	tile_to_linear_64x32_4x2_uv_neon, %function
tile_to_linear_64x32_4x2_uv_neon:
	.fnstart
	@r0		p_linear_addr_u			p0
	@r1		p_tiled_addr			p1
	@r2		x_size				p2
	@r3		y_size				p3
	@r4		i
	@r5		j
	@r6		nn(tiled_addr)
	@r7		mm(linear_addr)
	@r8		temp1
	@r9		x_size_temp
	@r10		y_size_temp
	@r11		p_linear_addr_v
	@r12		temp2

	STMFD		sp!, {r4-r12,r14}

	MOV		r8, r2, ASR #1
	MUL		r8, r8, r3

	MOV		r9, r2, ASR #4
	MOV		r9, r9, LSL #4
	MOV		r10, r3, ASR #4
	MOV		r10, r10, LSL #4

	ADD		r11, r0, r8

TILE_TO_LINEAR_64x32_4x2_UV_NEON_MULTIPLE16:
	MOV		r4, #0
TILE_TO_LINEAR_64x32_4x2_UV_NEON_LOOP_MULTIPLE16_OUTER:

	MOV		r5, #0
TILE_TO_LINEAR_64x32_4x2_UV_NEON_LOOP_MULTIPLE16_INNER:
	STMFD		sp!, {r0-r3, ip}
	MOV		r0, r2
	MOV		r1, r3
	MOV		r2, r5
	MOV		r3, r4
	BL		tile_4x2_read_asm
	MOV		r8, r0
	LDMFD		sp!, {r0-r3, ip}

	ADD		r6, r1, r8
	MOV		r8, #64

	VLD2.8		{D0, D1}, [r6], r8
	VLD2.8		{D2, D3}, [r6], r8
	VLD2.8		{D4, D5}, [r6], r8
	VLD2.8		{D6, D7}, [r6], r8
	VLD2.8		{D8, D9}, [r6], r8
	VLD2.8		{D10, D11}, [r6], r8
	VLD2.8		{D12, D13}, [r6], r8
	VLD2.8		{D14, D15}, [r6], r8
	VLD2.8		{D16, D17}, [r6], r8
	VLD2.8		{D18, D19}, [r6], r8
	VLD2.8		{D20, D21}, [r6], r8
	VLD2.8		{D22, D23}, [r6], r8
	VLD2.8		{D24, D25}, [r6], r8
	VLD2.8		{D26, D27}, [r6], r8
	VLD2.8		{D28, D29}, [r6], r8
	VLD2.8		{D30, D31}, [r6]

	MOV		r8, r2, ASR #1
	MUL		r12, r4, r8
	ADD		r7, r12, r5, ASR #1
	ADD		r7, r7, r0

	VST1.8		{D0}, [r7], r8
	VST1.8		{D2}, [r7], r8
	VST1.8		{D4}, [r7], r8
	VST1.8		{D6}, [r7], r8
	VST1.8		{D8}, [r7], r8
	VST1.8		{D10}, [r7], r8
	VST1.8		{D12}, [r7], r8
	VST1.8		{D14}, [r7], r8
	VST1.8		{D16}, [r7], r8
	VST1.8		{D18}, [r7], r8
	VST1.8		{D20}, [r7], r8
	VST1.8		{D22}, [r7], r8
	VST1.8		{D24}, [r7], r8
	VST1.8		{D26}, [r7], r8
	VST1.8		{D28}, [r7], r8
	VST1.8		{D30}, [r7], r8

	ADD		r7, r12, r5, ASR #1
	ADD		r7, r7, r11

	VST1.8		{D1}, [r7], r8
	VST1.8		{D3}, [r7], r8
	VST1.8		{D5}, [r7], r8
	VST1.8		{D7}, [r7], r8
	VST1.8		{D9}, [r7], r8
	VST1.8		{D11}, [r7], r8
	VST1.8		{D13}, [r7], r8
	VST1.8		{D15}, [r7], r8
	VST1.8		{D17}, [r7], r8
	VST1.8		{D19}, [r7], r8
	VST1.8		{D21}, [r7], r8
	VST1.8		{D23}, [r7], r8
	VST1.8		{D25}, [r7], r8
	VST1.8		{D27}, [r7], r8
	VST1.8		{D29}, [r7], r8
	VST1.8		{D31}, [r7], r8

	ADD		r5, r5, #16
	CMP		r5, r9
	BLT		TILE_TO_LINEAR_64x32_4x2_UV_NEON_LOOP_MULTIPLE16_INNER

	ADD		r4, r4, #16
	CMP		r4, r10
	BLT		TILE_TO_LINEAR_64x32_4x2_UV_NEON_LOOP_MULTIPLE16_OUTER

	CMP		r10, r3
	BGE		TILE_TO_LINEAR_64x32_4x2_UV_NEON_REMAINDER_X

TILE_TO_LINEAR_64x32_4x2_UV_NEON_REMAINDER_Y:

	MOV		r4, r10
TILE_TO_LINEAR_64x32_4x2_UV_NEON_LOOP_REMAINDER_Y_OUTER:

	MOV		r5, #0
TILE_TO_LINEAR_64x32_4x2_UV_NEON_LOOP_REMAINDER_Y_INNER:
	STMFD		sp!, {r0-r3, ip}
	MOV		r0, r2
	MOV		r1, r3
	MOV		r2, r5
	MOV		r3, r4
	BL		tile_4x2_read_asm
	MOV		r8, r0
	LDMFD		sp!, {r0-r3, ip}

	ADD		r6, r1, r8

	VLD2.8		{D0, D1}, [r6]

	MOV		r8, r2, ASR #1
	MUL		r12, r4, r8
	ADD		r7, r12, r5, ASR #1
	ADD		r7, r7, r0

	VST1.8		{D0}, [r7]

	ADD		r7, r12, r5, ASR #1
	ADD		r7, r7, r11

	VST1.8		{D1}, [r7]

	ADD		r5, r5, #16
	CMP		r5, r9
	BLT		TILE_TO_LINEAR_64x32_4x2_UV_NEON_LOOP_REMAINDER_Y_INNER

	ADD		r4, r4, #1
	CMP		r4, r3
	BLT		TILE_TO_LINEAR_64x32_4x2_UV_NEON_LOOP_REMAINDER_Y_OUTER

TILE_TO_LINEAR_64x32_4x2_UV_NEON_REMAINDER_X:
	CMP		r9, r2
	BGE		TILE_TO_LINEAR_64x32_4x2_UV_NEON_RESTORE_REG

	MOV		r4, #0
TILE_TO_LINEAR_64x32_4x2_UV_NEON_LOOP_REMAINDER_X_OUTER:

	MOV		r5, r9
TILE_TO_LINEAR_64x32_4x2_UV_NEON_LOOP_REMAINDER_X_INNER:
	STMFD		sp!, {r0-r3, ip}
	MOV		r0, r2
	MOV		r1, r3
	MOV		r2, r5
	MOV		r3, r4
	BL		tile_4x2_read_asm
	MOV		r8, r0
	LDMFD		sp!, {r0-r3, ip}

	AND		r6, r5, #3
	ADD		r6, r6, r8
	ADD		r6, r1, r6

	LDRH		r10, [r6]

	MOV		r8, r2, ASR #1
	MUL		r12, r4, r8
	ADD		r7, r12, r5, ASR #1
	ADD		r7, r7, r0

	STRB		r10, [r7]

	ADD		r7, r12, r5, ASR #1
	ADD		r7, r7, r11

	MOV		r10, r10, ASR #8
	STRB		r10, [r7]

	ADD		r5, r5, #2
	CMP		r5, r2
	BLT		TILE_TO_LINEAR_64x32_4x2_UV_NEON_LOOP_REMAINDER_X_INNER

	ADD		r4, r4, #1
	CMP		r4, r3
	BLT		TILE_TO_LINEAR_64x32_4x2_UV_NEON_LOOP_REMAINDER_X_OUTER

TILE_TO_LINEAR_64x32_4x2_UV_NEON_RESTORE_REG:
	LDMFD		sp!, {r4-r12,r15}		@ restore registers
	.fnend


	.arch armv7-a
	.text
	.align	2
	.global	tile_4x2_read_asm
	.type	tile_4x2_read_asm, %function
tile_4x2_read_asm:
	.fnstart
.LFB0:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	add	ip, r3, #32
	sub	r0, r0, #1
	cmp	r1, ip
	cmple	r3, r1
	mov	ip, r2, asr #2
	mov	r0, r0, asr #7
	stmfd	sp!, {r4, r5, lr}
	.save {r4, r5, lr}
.LCFI0:
	add	r0, r0, #1
	bge	.L2
	sub	r1, r1, #1
	tst	r1, #32
	bne	.L2
	tst	r3, #32
	bne	.L2
	mov	r4, r2, asr #7
	and	r1, r3, #31
	eor	r4, r4, r3, asr #5
	ubfx	r3, r3, #6, #8
	tst	r4, #1
	ubfx	r4, r2, #8, #6
	and	ip, ip, #15
	mov	r2, r2, asr #6
	mla	r3, r0, r3, r4
	orr	r1, ip, r1, asl #4
	b	.L9
.L2:
	mov	r2, ip, asr #5
	and	r4, r3, #31
	eor	r1, r2, r3, asr #5
	and	r5, r2, #127
	ubfx	r3, r3, #6, #8
	tst	r1, #1
	and	r1, ip, #15
	mov	r2, ip, asr #4
	mla	r3, r0, r3, r5
	orr	r1, r1, r4, asl #4
.L9:
	andne	r2, r2, #1
	andeq	r2, r2, #1
	orrne	r2, r2, #2
	mov	r1, r1, asl #2
	orr	r3, r1, r3, asl #13
	orr	r0, r3, r2, asl #11
	ldmfd	sp!, {r4, r5, pc}
.LFE0:
	.fnend
	.size	tile_4x2_read_asm, .-tile_4x2_read_asm
	.ident	"GCC: (Sourcery G++ Lite 2009q3-67) 4.4.1"
	.section	.note.GNU-stack,"",%progbits


