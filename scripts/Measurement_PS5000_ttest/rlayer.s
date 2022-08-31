.cpu cortex-m0
.text
.syntax unified
.thumb

.type rlayer, %function
.global rlayer


rlayer:
	// r0 is starting address of X
	MOVS r5, 0xFF			// set r5 to 0xFF for ANDS

	// R-Layer of X[1]
	LDRB r1, [r0, #1]		// r1 = X[1]
	LSRS r2, r1, #1			// r2 = r1 >> 1
	LSLS r3, r1, #7			// r3 = r1 << 7
	ORRS r2, r3	 			// r2 = r2 OR r3
	STRB r2, [r0, #1]		// r2 = X[1] <<< 7

	// R-Layer of X[2]
	LDRB r1, [r0, #2]		// r1 = X[2]
	LSRS r2, r1, #4			// r2 = r1 >> 4
	LSLS r3, r1, #4			// r3 = r1 << 4
	ORRS r2, r3				// r2 = r2 OR r3
	STRB r2, [r0, #2]		// r2 = X[2] <<< 4

	// R-Layer of X[3]
	LDRB r1, [r0, #3]		// r1 = X[3]
	LSRS r2, r1, #5			// r2 = r1 >> 5
	LSLS r3, r1, #3			// r3 = r1 << 3
	ORRS r2, r3				// r2 = r2 OR r3
	STRB r2, [r0, #3]		// r2 = X[3] <<< 3


	// R-Layer of X[4]
	LDRB r1, [r0, #4]		// r1 = X[4]
	LSRS r2, r1, #2			// r2 = r1 >> 2
	LSLS r3, r1, #6			// r3 = r1 << 6
	ORRS r2, r3				// r2 = r2 OR r3
	STRB r2, [r0, #4]		// r2 = X[4] <<< 6


	// R-Layer of X[5]
	MOV r1, sp				// Move dummy value into r1
	LDRB r1, [r0, #5]		// r1 = X[5]
	LSRS r2, r1, #3			// r2 = r1 >> 3
	LSLS r3, r1, #5			// r3 = r1 << 5
	ORRS r2, r3				// r2 = r2 OR r3
	STRB r2, [r0, #5]		// r2 = X[5] <<< 5

	// R-Layer of X[6]
	LDRB r1, [r0, #6]		// r1 = X[6]
	LSRS r2, r1, #7			// r2 = r1 >> 7
	LSLS r3, r1, #1			// r3 = r1 << 1
	ORRS r2, r3				// r2 = r2 OR r3
	STRB r2, [r0, #6]		// r2 = X[6] <<< 1

	// R-Layer of X[7]
	LDRB r1, [r0, #7]		// r1 = X[7]
	LSRS r2, r1, #6			// r2 = r1 >> 6
	LSLS r3, r1, #2			// r3 = r1 << 2
	ORRS r2, r3				// r2 = r2 OR r3
	STRB r2, [r0, #7]		// r2 = X[7] <<< 2

	BX lr
