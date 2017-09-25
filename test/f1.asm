
.orig x3000
	add r0, r0, #2
	add r1, r0, #8
	lea r3, target
	ldb r4, r3, #0
	halt
target .fill x4321
.end
