.cpu cortex-m0
.text
.syntax unified
.thumb

.type slayer, %function
.global slayer


secand:
	// secand (uint8_t a0, uint8_t b0, uint8_t a1, uint8_t b1, uint8_t m);
	// a0 = r1
	// b0 = r2
	// a1 = r3
	// b1 = r6
	// m  = r7

	MOV  r4, sp		// Move dummy value into r4 before loading new value
	MOV  r4, r2		// Step 1
	ANDS r4, r1		// Step 2
	EORS r4, r7		// Step 3
	MOV  r5, sp		// Move dummy value into r5 before loading new value
	MOV  r5, r1		// Step 4
	ANDS r5, r6		// Step 5
	EORS r4, r5		// Step 6
	MOV  r5, sp	// Move dummy value into r5 before loading new value
	MOV  r5, r3		// Step 7
	ANDS r5, r2		// Step 8
	EORS r4, r5		// Step 9
	MOV  r5, sp		// Move dummy value into r5 before loading new value
	MOV  r5, r3		// Step 10
	ANDS r5, r6		// Step 11
	EORS r4, r5		// Step 12

	BX lr


secor:
	// secor (uint8_t a0, uint8_t b0, uint8_t a1, uint8_t b1, uint8_t m)
	// a0 = r1
	// b0 = r2
	// a1 = r3
	// b1 = r0
	// m  = r7

	MOV  r4, r7		// Step 1
	EORS r4, r0		// Step 2
	MOV  r5, r2		// Step 3
	ORRS r5, r3		// Step 4
	EORS r5, r4		// Step 5
	MOV  r4, sp		// Move dummy value into r4 before loading new value
	MOV  r4, r2		// Step 6
	ANDS r4, r1		// Step 7
	MOV  r6, sp		// Move dummy value into r6 before loading new value
	MOV  r6, r1		// Step 8
	ORRS r6, r0		// Step 9
	EORS r4, r6		// Step 10
	EORS r4, r5		// Step 11

	BX lr



slayer: 			// slayer (uint8_t *X, uint8_t *m, uint8_t *T, uint8_t c);
	// Preserve registers ( R4 - R7, LR )
	PUSH {r4, r5, r6, r7, lr}
						// r0 = X, r1 = m, r2 = T, r3 = c
	PUSH {r2}			// Push address of T on stack
	MOV r8, r0			// Save address of X in r8
	MOV r9, r3			// r9 = c
	LDRB r4, [r1]
	MOV r10, r4			// r10  = m[0]
	LDRB r4, [r1, #1]
	MOV r11, r4			// r11 = m[1]
	LDRB r4, [r1, #2]
	MOV r12, r4			// r12 = m[2]

// Step 4
	LDRB r1, [r0, #7]	// r1 = X[7]
	LDRB r2, [r0, #6]	// r2 = X[6]
	MOV r3, r10			// r3 = m0
	MOV r6, r11			// r6 = m1
	MOV r7, r12			// r7 = m2
	BL secand			// secand(X[7], X[6], m0, m1, m2)

// Step 5
	LDRB r1, [r0, #5]	// r1 = X[5]
	EORS r1, r4			// r1 = X[5] xor temp
	STRB r1, [r0, #5]	// Store new Value of X[5]

// Step 6
	LDRB r1, [r0, #3]	// r1 = X[3]
	MOV r2, sp			// Move dummy value into r1 --> was masked with m1 before and now value masked with m1 gets loaded
	LDRB r2, [r0, #5]	// r2 = X[5]
	MOV r3, r12			// r3 = m2
	MOV r6, r10			// r6 = m0
	MOV r7, r11			// r7 = m1
	BL secand			// secand(X[3], X[5], m2, m0, m1)

// Step 7
	LDRB r1, [r0, #4]	// r1 = X[4]
	EORS r1, r4			// r1 = X[4] xor temp
	STRB r1, [r0, #4]	// Store new Value of X[4]

// Step 8
	LDRB r2, [r0, #7]	// r2 = X[7]
	EORS r2, r1			// r2 = X[7] xor X[4]
	STRB r2, [r0, #7]	// Store new Value of X[7]

// Step 9
	LDRB r1, [r0, #6]	// r1 = X[6]
	LDRB r2, [r0, #3]	// r2 = X[3]
	EORS r1, r2			// r1 = X[6] xor X[3]
	STRB r1, [r0, #6]	// Store new Value of X[6]

// Step 10
	LDRB r1, [r0, #5]	// r1 = X[5]
	MOV r2, sp			// Move dummy value into r1 --> was masked with m2 before and now value masked with m2 gets loaded
	LDRB r2, [r0, #4]	// r2 = X[4]
	MOV r3, r10			// r3 = m0
	MOV r0, r12			// r0 = m2
	MOV r7, r9			// r7 = c
	BL secor			// secor(X[5], X[4], m0, m2, c)
	MOV r0, r8

// Step 11
	LDRB r1, [r0, #3]	// r1 = X[3]
	EORS r1, r4			// r1 = X[3] xor temp
	STRB r1, [r0, #3]	// Store new Value of X[3]

// Step 12
	LDRB r1, [r0, #5]	// r1 = X[5]
	LDRB r2, [r0, #7]	// r2 = X[7]
	EORS r1, r2			// r1 = X[5] xor X[7]
	STRB r1, [r0, #5]	// Store new Value of X[5]

// Step 13
	//LDRB r1, [r0, #5]	// r1 is still  X[5]
	LDRB r2, [r0, #6]	// r2 = X[6]
	MOV r3, r12			// r3 = m2
	MOV r6, r10			// r6 = m0
	MOV r7, r11			// r7 = m1
	BL secand			// secand(X[5], X[6], m2, m0, m1)

// Step 14
	MOV  r1, sp			// Move dummy value into r1 --> was masked with m2 before and now value masked with m2 gets loaded
	LDRB r1, [r0, #4]	// r1 = X[4]
	EORS r1, r4			// r1 = X[4] xor temp
	STRB r1, [r0, #4]	// Store new Value of X[4]

// Step 15
	LDRB r1, [r0, #1]	// r1 = X[1]
	MOV  r2, sp			// Move dummy value into r2 --> was masked with m0 before and now value masked with m0 gets loaded
	LDRB r2, [r0, #0]	// r2 = X[0]
	MOV r3, r11			// r3 = m1
	//MOV r6, r10		// r6 is still m0
	MOV r7, r12			// r7 = m2
	BL secand			// secand(X[1], X[0], m1, m0, m2)

// Step 16
	MOV  r1, sp			// Move dummy value into r1 --> was masked with m1 before and now value masked with m1 gets loaded
	LDRB r1, [r0, #2]	// r1 = X[2]
	EORS r1, r4			// r1 = X[2] xor temp
	STRB r1, [r0, #2]	// Store new Value of X[2]

// Step 17
	LDRB r1, [r0, #1]	// r1 = X[1]
	MOV  r2, sp			// Move dummy value into r2 --> was masked with m0 before and now value masked with m0 gets loaded
	LDRB r2, [r0, #2]	// r2 = X[2]
	//MOV r3, r11		// r3  is still m1
	MOV r0, r10			// r0 = m0
	MOV r7, r9			// r7 = c
	BL secor			// secor(X[1], X[2], m1, m0, c)
	MOV r0, r8

// Step 18
	LDRB r1, [r0]		// r1 = X[0]
	EORS r1, r4			// r1 = X[0] xor temp
	STRB r1, [r0]		// Store new Value of X[0]

// Step 19
	LDRB r1, [r0, #2]	// r1 = X[2]
	LDRB r2, [r0, #0]	// r2 = X[0]
	MOV r3, r10			// r3 = m0
	MOV r0, r12			// r0 = m2
	//MOV r7, r9		// r7 is still c
	BL secor			// secor(X[2], X[0], m0, m2, c)
	MOV r0, r8

// Step 20
	LDRB r1, [r0, #1]	// r1 = X[1]
	EORS r1, r4			// r1 = X[1] xor temp
	STRB r1, [r0, #1]	// Store new Value of X[1]

// Step 21
	LDRB r1, [r0, #2]	// r1 = X[2]
	MVNS r2, r1			// r2 = ~X[2]
	STRB r2, [r0, #2]	// Store ( only last byte) of new Value of X[2]

// Step 22
	LDRB r1, [r0, #7]	// r1 = X[7]
	LDRB r2, [r0, #1]	// r2 = X[1]
	EORS r1, r2			// r1 = X[7] xor X[1]
	STRB r1, [r0, #7]	// Store new Value of X[7]

// Step 23
	LDRB r1, [r0, #3]	// r1 = X[3]
	LDRB r2, [r0, #2]	// r2 = X[2]
	EORS r1, r2			// r1 = X[3] xor X[2]
	STRB r1, [r0, #3]	// Store new Value of X[3]

// Step 24
	LDRB r1, [r0, #4]	// r1 = X[4]
	LDRB r2, [r0, #0]	// r2 = X[0]
	EORS r1, r2			// r1 = X[4] xor X[0]
	STRB r1, [r0, #4]	// Store new Value of X[4]

// Step 25 - 27
	POP {r7}			// pop address of T from stack into r7
	LDRB r2, [r0, #7]	// r2 = X[7]
	STRB r2, [r7]		// T[0] = X[7]

	LDRB r2, [r0, #3]	// r2 = X[3]
	STRB r2, [r7, #1]	// T[1] = X[3]

	LDRB r2, [r0, #4]	// r2 = X[4]
	STRB r2, [r7, #2]	// T[2] = X[4]

// Step 28
	LDRB r1, [r0, #5]	// r1 = X[5]
	LDRB r2, [r7]		// r2 = T[0]
	PUSH {r7}			/// push address of T to stack
	MOV r3, r12			// r3 = m2
	MOV r6, r10			// r6 = m0
	MOV r7, r11			// r7 = m1
	BL secand			// secand(X[5], T[0], m2, m0, m1)

// Step 29
	LDRB r1, [r0, #6]	// r1 = X[6]
	EORS r1, r4			// r1 = X[6] xor temp
	STRB r1, [r0, #6]	// Store new Value of X[6]

// Step 30
	POP {r7}			// pop address of T from stack into r7
	//LDRB r2, [r7]		// r2 = T[0] already loaded in Step 28
	EORS r2, r1			// r2 = T[0] xor X[6]
	STRB r2, [r7]		// Store new Value of T[0]

// Step 31
	LDRB r1, [r7, #2]	// r1 = T[2]
	LDRB r2, [r7, #1]	// r2 = T[1]
	PUSH {r7}			/// push address of T to stack
	MOV r3, r11			// r3 = m1
	MOV r0, r12			// r0 = m2
	MOV r7, r9			// r7 = c
	BL secor			// secor(T[2], T[1], m1, m2, c)
	MOV r0, r8

// Step 32
	LDRB r1, [r0, #6]	// r1 = X[6]
	EORS r1, r4			// r1 = X[6] xor temp
	STRB r1, [r0, #6]	// Store new Value of X[6]

// Step 33
	POP {r7}			// pop address of T from stack into r7
	MOV  r1, sp			// Move dummy value into r1 --> was m0 before and now value masked with m0 gets loaded
	MOV r1, r10			// r1 = m0
	MOV  r2, sp			// Move dummy value into r2 --> was masked with m2 before and now value masked with m2 gets loaded
	LDRB r2, [r0, #5]	// r2 = X[5]
	EORS r2, r1			// r2 = X[5] xor m0
	LDRB r1, [r7, #1]	// r1 = T[1]
	EORS r1, r2			// r1 = T[1] xor X[5] xor m0
	STRB r1, [r7, #1]	// Store new Value of T[1]

// Step 34
	MOV  r1, sp			// Move dummy value into r1 --> was masked with m0 before and now value masked with m0 gets loaded
	LDRB r1, [r0, #6]	// r1 = X[6]
	MOV  r2, sp			// Move dummy value into r2 --> was masked with m1 before and now value masked with m1 gets loaded
	LDRB r2, [r7, #2]	// r2 = T[2]
	PUSH {r7}			/// push address of T to stack
	MOV r3, r10			// r3 = m0
	MOV r0, r11			// r0 = m1
	MOV r7, r9			// r7 = c
	BL secor			// secor(X[6], T[2], m0, m1, c)
	MOV r0, r8

// Step 35
	LDRB r1, [r0, #5]	// r1 = X[5]
	EORS r1, r4			// r1 = X[5] xor temp
	STRB r1, [r0, #5]	// Store new Value of X[5]

// Step 36
	POP {r7}			// pop address of T from stack into r7
	LDRB r1, [r7, #1]	// r1 = T[1]
	MOV  r2, sp			// Move dummy value into r2 --> was masked with m1 before and now value masked with m1 gets loaded
	LDRB r2, [r7]		// r2 = T[0]
	PUSH {r7}			/// push address of T to stack
	//MOV r3, r10			// r3 is still m0
	MOV r6, r11			// r6 = m1
	MOV r7, r12			// r7 = m2
	BL secand			// secand(T[1], T[0], m0, m1, m2)

// Step 37
	POP {r7}			// pop address of T from stack into r7
	LDRB r1, [r7, #2]	// r1 = T[2]
	EORS r1, r4			// r1 = T[2] xor temp
	STRB r1, [r7, #2]	// Store new Value of T[2]

// Step 38
	MOV  r1, sp			// Move dummy value into r1 --> was masked with m0 before and now value masked with m0 gets loaded
	LDRB r1, [r0, #2]	// r1 = X[2]
	MOV  r2, sp			// Move dummy value into r2 --> was masked with m1 before and now value masked with m1 gets loaded
	LDRB r2, [r7]		// r2 = T[0]
	EORS r1, r2			// r1 = X[2] xor T[0]
	STRB r1, [r0, #2]	// Store new Value of X[2]

// Step 39
	MOV  r1, sp			// Move dummy value into r1 --> was masked with m2 before and now value masked with m2 gets loaded
	LDRB r1, [r0, #1]	// r1 = X[1]
	LDRB r2, [r7, #2]	// r2 = T[2]
	EORS r1, r2			// r1 = X[1] xor T[2]
	STRB r1, [r7]		// Store new Value of T[0]

// Step 40
	LDRB r1, [r0]		// r1 = X[0]
	MOV  r2, sp			// Move dummy value into r2 --> was masked with m0 before and now value masked with m0 gets loaded
	LDRB r2, [r7, #1]	// r2 = T[1]
	EORS r1, r2			// r1 = X[0] xor T[1]
	STRB r1, [r0, #1]	// Store new Value of X[0]

// Step 41
	LDRB r1, [r0, #7]	// r1 = X[7]
	MOV  r2, r11		// r2 = m1
	EORS r1, r2			// r1 = X[7] xor m1
	STRB r1, [r0]		// Store new Value of X[0]

// Step 42
	LDRB r1, [r7]		// r1 = T[0]
	STRB r1, [r0, #7] 	// Store new Value of X[7]

// Step 43
	LDRB r1, [r0, #3]	// r1 = X[3]
	STRB r1, [r7, #1] 	// Store new Value of T[1]

// Step 44
	LDRB r1, [r0, #6]	// r1 = X[6]
	STRB r1, [r0, #3] 	// Store new Value of X[3]

// Step 45
	LDRB r1, [r7, #1]	// r1 = T[1]
	STRB r1, [r0, #6] 	// Store new Value of X[6]

// Step 46
	LDRB r1, [r0, #4]	// r1 = X[4]
	STRB r1, [r7, #2] 	// Store new Value of T[2]

// Step 47
	MOV  r1, sp			// Move dummy value into r1 --> was masked with m1 before and now value masked with m1 gets loaded
	LDRB r1, [r0, #5]	// r1 = X[5]
	STRB r1, [r0, #4] 	// Store new Value of X[4]

// Step 48
	MOV  r1, sp			// Move dummy value into r1 --> was masked with m1 before and now value masked with m1 gets loaded
	LDRB r1, [r7, #2]	// r1 = T[2]
	STRB r1, [r0, #5] 	// Store new Value of X[5]


	POP {r4, r5, r6, r7}// restore r4 - r8
	POP {r0}			// restore lr into r0
	MOV lr, r0			// lr = old address
	BX lr
