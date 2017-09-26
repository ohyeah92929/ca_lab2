.orig x3000
and r0, r0, #0 ; 0x5020, r0: 0
add r0, r0, #13; 0x102d, r0: d
lshf r2, r0, #2; , r2: 0x0034(0000000000110100)
rshfl r3, r2, #1; , r3: 0x001a					(0000000000011010)
not r4, r3; 0x98ff, r4: 0xffe5(1111111111100101)
rshfa r5, r4, #1;  r5: 0xfff2 					(1111111111110010)
xor r6, r5, r3;     r6: xffe8(1111111111101000
halt
lab .fill x2222;
.end
