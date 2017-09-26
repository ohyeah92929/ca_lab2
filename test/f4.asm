.orig x3000
and r0, r0, #0 ; 0x5020, r0: 0
and r1, r1, r0 ; 0x5240, r1: 0
lea r4, bye; 0xe104, r4: x300c(0011000000001010)
jmp r4; 0xc100
add r0, r0, #12; 0x102C
add r0, r0, #12; 0x102C
bye add r0, r0, #12; 0x102C, r0: 12
add r1, r0, r0; 0x1200, r1: 24
rshfl r5, r4, 1; 0xd711, r5: x1805(0001100000000101)
not r6, r4; 0x9d3f, r6: xcff5(1100111111110101)
rshfa r2, r6, 1; 0xd5b1 r2: xe7fa (1110011111111010)
xor r3, r2, r6; 0x9686, r3: x280f(0010100000001111)
halt
lab .fill x2222;
.end