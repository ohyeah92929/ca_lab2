.orig x3000
	add r0, r0, #1 ; 				r0 = 1
	add r1, r1, #2 ; 				r1 = 2
	add r2, r1, r0 ; r2 = 3
	brp positive
	xor r0, r0, r0 ; r0 = 0
positive add r3, r2, #-7; 			r3 = -4
	brn negative
	xor r1, r1, r1 ; r1 = 0
negative add r4, r3, #4 ; 			r4 = 0
	brz zero
	xor r2, r2, r2 ; r2 = 0
zero lshf r2, r2, #10 ; r2 = 3 << 10 = 0x0C00 (1100.0000.0000)
	add r2, r2, #7 ; r2 = 0x0C07
	lea r5, testStb ; 				r5 = 0x3028
	ldw r6, r5, #0 ;    			r6 = 0x3050
	stb r2, r6, #0 ; 				mem[0x3050][0] = 07
	rshfa r2, r2, #8 ; 				r2 = 0x0C
	stb r2, r6, #1 ;				mem[0x3050][1] = 0x0C
	halt
testStb	.fill x3050
.end
