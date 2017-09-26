.orig x3000
and r0, r0, #0 ; 0x5020, r0: 0
add r0, r0, #13; 0x102d, r0: d
lshfl r2, r0, 2; , r2: 0x0030(0000000000110000)
rshfl r3, r2, 1; , r3: 0x0018(0000000000011000)
not r4, r3; 0x98ff, r4: 0xffe7(1111111111100111)
rshfa r5, r4, 1;  r5: 0xfff3 (1111111111110011)
xor r6, r5, r3;     r6: x280f(1111111111101011)
halt
lab .fill x2222;
.end