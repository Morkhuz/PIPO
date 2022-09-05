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
	// address of a1 = r3
	// address of b1 = r6
	// m  = r7

	MOV  r4, sp			// Move dummy value into r4
	MOV  r4, r2			// Step 1			// r4 = b0
	ANDS r4, r1			// Step 2			// r4 = b0 & a0
	MOV  r5, sp			// Move dummy value into r5
	LDRB r5, [r7]		// load value of m into r5
	EORS r5, r4			// Step 3			// r5 = (b0 & a0) xor m
	MOV r4, r5								// r4 = (b0 & a0) xor m
	MOV  r5, sp			// Move dummy value into r5
	PUSH {r2}			// push b0 onto stack, before loading b1 into register
	MOV  r2, sp			// Move dummy value into r2
	MOV  r5, r1			// Step 4			// r5 = a0
	LDRB r7, [r6]		// load value of b1 into r7
	ANDS r5, r7			// Step 5			// r5 = a0 & b1
	MOV  r7, sp			// Move dummy value into r7
	POP  {r2}			// get b0 back into r2
	EORS r4, r5			// Step 6			// r4 = (b0 & a0) xor m xor (a0 & b1)
	MOV  r5, sp			// Move dummy value into r5
	PUSH {r1}			// Push a0 onto stack, before loading a1 into register
	MOV  r1, sp			// Move dummy value into r1
	LDRB r5, [r3]		// Step 7			// r5 = a1
	ANDS r5, r2			// Step 8			// r5 = a1 & b0
	EORS r4, r5			// Step 9			// r4 = (b0 & a0) xor m xor (a0 & b1) xor (a1 & b0)
	MOV  r5, sp			// Move dummy value into r5 before loading new value
	PUSH {r2}			// push b0 onto stack, before loading b1 into register
	LDRB r5, [r3]		// Step 10			// r5 = a1
	LDRB r7, [r6]		// load value of b1 into r7
	ANDS r5, r7			// Step 11			// r5 = a1 & b1
	MOV  r7, sp			// Move dummy value into r7
	EORS r4, r5			// Step 12			// r4 = (b0 & a0) xor m xor (a0 & b1) xor (a1 & b0) xor (a1 & b1)
	MOV  r5, sp			// Move dummy value into r5
	MOV  r6, sp			// Move dummy value into r6
	POP  {r2}			// get b0 back into r2
	MOV  r2, sp			// Move dummy value into r2
	POP  {r1}			// get a0 back into r1
	MOV  r1, sp			// Move dummy value into r1

	BX lr


secor:
	// secor (uint8_t a0, uint8_t b0, uint8_t a1, uint8_t b1, uint8_t m)
	// a0 = r1
	// b0 = r2
	// address a1 = r3
	// address b1 = r0
	// m  = r7

	MOV  r4, r7		// Step 1				// r4 = c
	PUSH {r2}		// push b0 onto the stack
	MOV  r2, sp		// Move dummy value into r2
	LDRB r7, [r0]	// load value of b1 into r7
	EORS r7, r4		// Step 2				// r7 = c xor b1
	MOV r4, r7								// r4 = c xor b1
	MOV  r7, sp		// Move dummy value into r7
	POP  {r2}		// Load b0 into r2
	MOV  r5, r2		// Step 3				// r5 = b0
	PUSH {r1}		// push a0 onto the stack
	MOV  r1, sp		// Move dummy value into r1
	LDRB r7, [r3]	// Load a1 into r7
	ORRS r5, r7		// Step 4				// r5 = b0 or a1
	MOV  r7, sp		// Move dummy value into r7
	POP  {r1}		// Move a0 into r1
	EORS r5, r4		// Step 5				// r5 = (bo or a1) xor ( c xor b1)
	MOV  r4, sp		// Move dummy value into r4
	MOV  r4, r2		// Step 6				// r4 = b0
	ANDS r4, r1		// Step 7				// r4 = b0 & a0
	MOV  r6, r1		// Step 8				// r6 = a0
	PUSH {r2}		// Push b0 onto the stack
	MOV  r2, sp		// Move dummy value into r2
	LDRB r7, [r0]	// Load b1 into r7
	ORRS r6, r7		// Step 9				// r6 = a0 or b1
	MOV  r7, sp		// Move dummy value into r7
	EORS r4, r6		// Step 10				// r4 = (b0 & a0) xor (a0 or b1)
	MOV  r6, sp		// Move dummy value into r6
	EORS r4, r5		// Step 11				// r4 = (b0 & a0) xor (a0 or b1) xor (bo or a1) xor c xor b1
	MOV  r5, sp		// Move dummy value into r5
	POP  {r2}		// Move b0 into r2
	MOV  r2, sp		// Move dummy value into r2
	MOV  r1, sp		// Move dummy value into r1
	BX lr



slayer: 			// slayer (uint8_t *X, uint8_t *m, uint8_t *T, uint8_t c);
// Preserve registers ( R4 - R7, LR )
	PUSH {r4, r5, r6, r7, lr}
						// r0 = X, r1 = m, r2 = T, r3 = c
	PUSH {r2}			// Push address of T on stack
	MOV r8, r0			// Save address of X in r8
	MOV r9, r3			// r9 = c
	MOV r4, r1			// r4 = address of m[0]
	MOV r10, r4			// r10  = address of m[0]
	ADDS r4, #1			// r4 = address of m[1]
	MOV r11, r4			// r11 = address of m[1]
	ADDS r4, #1			// r4 = address of m[2]
	MOV r12, r4			// r12 = address of m[2]
	MOV  r4, sp			// Move dummy value into r4
	MOV  r5, sp			// Move dummy value into r5
	MOV  r6, sp			// Move dummy value into r6
	MOV  r7, sp			// Move dummy value into r7

// Step 4
	LDRB r1, [r0, #7]	// r1 = X[7]
	LDRB r2, [r0, #6]	// r2 = X[6]
	MOV  r3, r10		// r3 = address of m0
	MOV  r6, r11		// r6 = address of m1
	MOV  r7, r12		// r7 = address of m2
	BL secand			// secand(X[7], X[6], m0, m1, m2)

// Step 5
	LDRB r1, [r0, #5]	// r1 = X[5]
	EORS r1, r4			// r1 = X[5] xor temp
	MOV r4, sp			// move dummy value into r4
	STRB r1, [r0, #5]	// Store new Value of X[5]
	MOV r1, sp			// Move dummy value into r1

// Step 6
	LDRB r1, [r0, #3]	// r1 = X[3]
	LDRB r2, [r0, #5]	// r2 = X[5]
	MOV r3, r12			// r3 = address of m2
	MOV r6, r10			// r6 = address of m0
	MOV r7, r11			// r7 = address of m1
	BL secand			// secand(X[3], X[5], m2, m0, m1)

// Step 7
	LDRB r1, [r0, #4]	// r1 = X[4]
	EORS r1, r4			// r1 = X[4] xor temp
	MOV r4, sp			// move dummy value into r4
	STRB r1, [r0, #4]	// Store new Value of X[4]

// Step 8
	LDRB r2, [r0, #7]	// r2 = X[7]
	EORS r2, r1			// r2 = X[7] xor X[4]
	STRB r2, [r0, #7]	// Store new Value of X[7]
	MOV r1, sp			// move dummy value into r1
	MOV r2, sp			// move dummy value into r2

// Step 9
	LDRB r1, [r0, #6]	// r1 = X[6]
	LDRB r2, [r0, #3]	// r2 = X[3]
	EORS r1, r2			// r1 = X[6] xor X[3]
	STRB r1, [r0, #6]	// Store new Value of X[6]
	MOV r1, sp			// move dummy value into r1
	MOV r2, sp			// move dummy value into r2

// Step 10
	LDRB r1, [r0, #5]	// r1 = X[5]
	LDRB r2, [r0, #4]	// r2 = X[4]
	MOV r3, r10			// r3 = address of m0
	MOV r0, r12			// r0 = address of m2
	MOV r7, r9			// r7 = c
	BL secor			// secor(X[5], X[4], m0, m2, c)
	MOV r0, r8

// Step 11
	LDRB r1, [r0, #3]	// r1 = X[3]
	EORS r1, r4			// r1 = X[3] xor temp
	MOV r4, sp			// move dummy value into r4
	STRB r1, [r0, #3]	// Store new Value of X[3]
	MOV r1, sp			// move dummy value into r1

// Step 12
	LDRB r1, [r0, #5]	// r1 = X[5]
	LDRB r2, [r0, #7]	// r2 = X[7]
	EORS r1, r2			// r1 = X[5] xor X[7]
	STRB r1, [r0, #5]	// Store new Value of X[5]
	MOV r2, sp			// move dummy value into r2

// Step 13
	//LDRB r1, [r0, #5]	// r1 is still  X[5]
	LDRB r2, [r0, #6]	// r2 = X[6]
	MOV r3, r12			// r3 = address of m2
	MOV r6, r10			// r6 = address of m0
	MOV r7, r11			// r7 = address of m1
	BL secand			// secand(X[5], X[6], m2, m0, m1)

// Step 14
	LDRB r1, [r0, #4]	// r1 = X[4]
	EORS r1, r4			// r1 = X[4] xor temp
	MOV r4, sp			// move dummy value into r4
	STRB r1, [r0, #4]	// Store new Value of X[4]
	MOV r1, sp			// move dummy value into r1

// Step 15
	LDRB r1, [r0, #1]	// r1 = X[1]
	LDRB r2, [r0, #0]	// r2 = X[0]
	MOV r3, r11			// r3 = address of m1
	MOV r6, r10			// r6 = address of m0
	MOV r7, r12			// r7 = address of m2
	BL secand			// secand(X[1], X[0], m1, m0, m2)

// Step 16
	LDRB r1, [r0, #2]	// r1 = X[2]
	EORS r1, r4			// r1 = X[2] xor temp
	MOV r4, sp			// move dummy value into r4
	STRB r1, [r0, #2]	// Store new Value of X[2]
	MOV r1, sp			// move dummy value into r1

// Step 17
	LDRB r1, [r0, #1]	// r1 = X[1]
	LDRB r2, [r0, #2]	// r2 = X[2]
	//MOV r3, r11		// r3  is still address of m1
	MOV r0, r10			// r0 = address of m0
	MOV r7, r9			// r7 = c
	BL secor			// secor(X[1], X[2], m1, m0, c)
	MOV r0, r8

// Step 18
	LDRB r1, [r0]		// r1 = X[0]
	EORS r1, r4			// r1 = X[0] xor temp
	MOV  r4, sp			// move dummy value into r4
	STRB r1, [r0]		// Store new Value of X[0]
	MOV r1, sp			// move dummy value into r1

// Step 19
	LDRB r1, [r0, #2]	// r1 = X[2]
	LDRB r2, [r0, #0]	// r2 = X[0]
	MOV r3, r10			// r3 = address of m0
	MOV r0, r12			// r0 = address of m2
	MOV r7, r9			// r7 is still c
	BL secor			// secor(X[2], X[0], m0, m2, c)
	MOV r0, r8

// Step 20
	LDRB r1, [r0, #1]	// r1 = X[1]
	EORS r1, r4			// r1 = X[1] xor temp
	MOV  r4, sp			// move dummy value into r4
	STRB r1, [r0, #1]	// Store new Value of X[1]
	MOV r1, sp			// move dummy value into r1

// Step 21
	LDRB r1, [r0, #2]	// r1 = X[2]
	MVNS r2, r1			// r2 = ~X[2]
	STRB r2, [r0, #2]	// Store ( only last byte) of new Value of X[2]
	MOV r1, sp			// move dummy value into r1
	MOV r2, sp			// move dummy value into r2

// Step 22
	LDRB r1, [r0, #7]	// r1 = X[7]
	LDRB r2, [r0, #1]	// r2 = X[1]
	EORS r1, r2			// r1 = X[7] xor X[1]
	STRB r1, [r0, #7]	// Store new Value of X[7]
	MOV r1, sp			// move dummy value into r1
	MOV r2, sp			// move dummy value into r2

// Step 23
	LDRB r1, [r0, #3]	// r1 = X[3]
	LDRB r2, [r0, #2]	// r2 = X[2]
	EORS r1, r2			// r1 = X[3] xor X[2]
	STRB r1, [r0, #3]	// Store new Value of X[3]
	MOV r1, sp			// move dummy value into r1
	MOV r2, sp			// move dummy value into r2

// Step 24
	LDRB r1, [r0, #4]	// r1 = X[4]
	LDRB r2, [r0, #0]	// r2 = X[0]
	EORS r1, r2			// r1 = X[4] xor X[0]
	STRB r1, [r0, #4]	// Store new Value of X[4]
	MOV r1, sp			// move dummy value into r1
	MOV r2, sp			// move dummy value into r2

// Step 25 - 27
	POP {r7}			// pop address of T from stack into r7
	LDRB r2, [r0, #7]	// r2 = X[7]
	STRB r2, [r7]		// T[0] = X[7]
	MOV r2, sp			// move dummy value into r2

	LDRB r2, [r0, #3]	// r2 = X[3]
	STRB r2, [r7, #1]	// T[1] = X[3]
	MOV r2, sp			// move dummy value into r2

	LDRB r2, [r0, #4]	// r2 = X[4]
	STRB r2, [r7, #2]	// T[2] = X[4]
	MOV r2, sp			// move dummy value into r2

// Step 28
	LDRB r1, [r0, #5]	// r1 = X[5]
	LDRB r2, [r7]		// r2 = T[0]
	PUSH {r7}			/// push address of T to stack
	MOV r3, r12			// r3 = address of m2
	MOV r6, r10			// r6 = address of m0
	MOV r7, r11			// r7 = address of m1
	BL secand			// secand(X[5], T[0], m2, m0, m1)

// Step 29
	LDRB r1, [r0, #6]	// r1 = X[6]
	EORS r1, r4			// r1 = X[6] xor temp
	MOV  r4, sp			// move dummy value into r4
	STRB r1, [r0, #6]	// Store new Value of X[6]


// Step 30
	POP {r7}			// pop address of T from stack into r7
	LDRB r2, [r7]		// r2 = T[0]
	EORS r2, r1			// r2 = T[0] xor X[6]
	STRB r2, [r7]		// Store new Value of T[0]
	MOV r2, sp			// move dummy value into r2

// Step 31
	LDRB r1, [r7, #2]	// r1 = T[2]
	LDRB r2, [r7, #1]	// r2 = T[1]
	PUSH {r7}			/// push address of T to stack
	MOV r3, r11			// r3 = address of m1
	MOV r0, r12			// r0 = address of m2
	MOV r7, r9			// r7 = c
	BL secor			// secor(T[2], T[1], m1, m2, c)
	MOV r0, r8

// Step 32
	LDRB r1, [r0, #6]	// r1 = X[6]
	EORS r1, r4			// r1 = X[6] xor temp
	MOV  r4, sp			// move dummy value into r4
	STRB r1, [r0, #6]	// Store new Value of X[6]
	MOV r1, sp			// move dummy value into r1

// Step 33
	POP {r7}			// pop address of T from stack into r7
	MOV  r3, r10		// r3 = address of m0
	LDRB r1, [r3]		// r1 = m0
	LDRB r2, [r0, #5]	// r2 = X[5]
	EORS r1, r2			// r1 = m0 xor X[5]
	MOV  r2, r1			// r2 = r1 = m0 xor X[5]
	MOV  r1, sp			// move dummy value into r1
	LDRB r1, [r7, #1]	// r1 = T[1]
	EORS r1, r2			// r1 = T[1] xor X[5] xor m0
	STRB r1, [r7, #1]	// Store new Value of T[1]
	MOV r1, sp			// move dummy value into r1
	MOV r2, sp			// move dummy value into r2

// Step 34
	LDRB r1, [r0, #6]	// r1 = X[6]
	LDRB r2, [r7, #2]	// r2 = T[2]
	PUSH {r7}			/// push address of T to stack
	//MOV r3, r10		// r3 is still address of m0
	MOV r0, r11			// r0 = address of m1
	MOV r7, r9			// r7 = c
	BL secor			// secor(X[6], T[2], m0, m1, c)
	MOV r0, r8

// Step 35
	LDRB r1, [r0, #5]	// r1 = X[5]
	EORS r1, r4			// r1 = X[5] xor temp
	MOV  r4, sp			// move dummy value into r4
	STRB r1, [r0, #5]	// Store new Value of X[5]
	MOV r1, sp			// move dummy value into r1

// Step 36
	POP {r7}			// pop address of T from stack into r7
	LDRB r1, [r7, #1]	// r1 = T[1]
	LDRB r2, [r7]		// r2 = T[0]
	PUSH {r7}			/// push address of T to stack
	//MOV r3, r10		// r3 is still address of m0
	MOV r6, r11			// r6 = address of m1
	MOV r7, r12			// r7 = address of m2
	BL secand			// secand(T[1], T[0], m0, m1, m2)

// Step 37
	POP {r7}			// pop address of T from stack into r7
	LDRB r1, [r7, #2]	// r1 = T[2]
	EORS r1, r4			// r1 = T[2] xor temp
	MOV  r4, sp			// move dummy value into r4
	STRB r1, [r7, #2]	// Store new Value of T[2]
	MOV r1, sp			// move dummy value into r1

// Step 38
	LDRB r1, [r0, #2]	// r1 = X[2]
	LDRB r2, [r7]		// r2 = T[0]
	EORS r1, r2			// r1 = X[2] xor T[0]
	STRB r1, [r0, #2]	// Store new Value of X[2]
	MOV r1, sp			// move dummy value into r1
	MOV r2, sp			// move dummy value into r2

// Step 39
	LDRB r1, [r0, #1]	// r1 = X[1]
	LDRB r2, [r7, #2]	// r2 = T[2]
	EORS r1, r2			// r1 = X[1] xor T[2]
	STRB r1, [r7]		// Store new Value of T[0]
	MOV r1, sp			// move dummy value into r1
	MOV r2, sp			// move dummy value into r2

// Step 40
	LDRB r1, [r0]		// r1 = X[0]
	LDRB r2, [r7, #1]	// r2 = T[1]
	EORS r1, r2			// r1 = X[0] xor T[1]
	STRB r1, [r0, #1]	// Store new Value of X[0]
	MOV r1, sp			// move dummy value into r1
	MOV r2, sp			// move dummy value into r2

// Step 41
	LDRB r1, [r0, #7]	// r1 = X[7]
	MOV  r3, r11		// r3 = address of m1
	LDRB r2, [r3]		// r2 = m1
	EORS r2, r1			// r2 = m1 xor X[7]
	STRB r2, [r0]		// Store new Value of X[0]
	MOV r1, sp			// move dummy value into r1
	MOV r2, sp			// move dummy value into r2

// Step 42
	LDRB r1, [r7]		// r1 = T[0]
	STRB r1, [r0, #7] 	// Store new Value of X[7]
	MOV r1, sp			// move dummy value into r1

// Step 43
	LDRB r1, [r0, #3]	// r1 = X[3]
	STRB r1, [r7, #1] 	// Store new Value of T[1]
	MOV r1, sp			// move dummy value into r1

// Step 44
	LDRB r1, [r0, #6]	// r1 = X[6]
	STRB r1, [r0, #3] 	// Store new Value of X[3]
	MOV r1, sp			// move dummy value into r1

// Step 45
	LDRB r1, [r7, #1]	// r1 = T[1]
	STRB r1, [r0, #6] 	// Store new Value of X[6]
	MOV r1, sp			// move dummy value into r1

// Step 46
	LDRB r1, [r0, #4]	// r1 = X[4]
	STRB r1, [r7, #2] 	// Store new Value of T[2]
	MOV r1, sp			// move dummy value into r1

// Step 47
	LDRB r1, [r0, #5]	// r1 = X[5]
	STRB r1, [r0, #4] 	// Store new Value of X[4]
	MOV r1, sp			// move dummy value into r1

// Step 48
	LDRB r1, [r7, #2]	// r1 = T[2]
	STRB r1, [r0, #5] 	// Store new Value of X[5]
	MOV r1, sp			// move dummy value into r1


	POP {r4, r5, r6, r7}// restore r4 - r8
	POP {r0}			// restore lr into r0
	MOV lr, r0			// lr = old address
	BX lr
