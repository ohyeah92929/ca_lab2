.orig x3000
and r0, r0, #0 ; 0x5020, r0: 0
and r1, r1, r0 ; 0x5240, r1: 0
add r0, r0, #12; 0x102C, r0: 12
stw r0, r1, #10; 0x740a, mem[20]: 12
ldb r4, r0, #8; 0x2808, r4: 12
halt
target .fill x1111
.end