
 .ORIG x3000
 AND R2,R2,#0 ; R2 is counter, initialize to 0
 LEA R3,PTR ; R3 is pointer to characters
 LDW R3,R3,#0
 TRAP x23 ; R0 gets character input
 LDB R1,R3,#0 ; R1 gets the next character, x3008
 ;
 ; Test character for end of file
 ;
 TEST ADD R4,R1,#-4 ; Test for EOT, x3
 BRz OUTPUT ; If done, prepare the output
 ;
 ; Test character for match. If a match, increment count.
 ;
 NOT R1,R1
 ADD R1,R1,R0 ; If match, R1 = xFFFF, x3010
 NOT R1,R1 ; If match, R1 = x0000
 BRnp GETCHAR ; If no match, do not increment
 ADD R2,R2,#1
 ;
 ; Get next character from the file
 ;
 GETCHAR ADD R3,R3,#1 ; Increment the pointer
 LDB R1,R3,#0 ; R1 gets the next character to test
 BRnzp TEST
 ;
 ; Output the count.
 ;
 OUTPUT LEA R0,ASCII ; Load the ASCII template
 LDW R0,R0,#0 ; x3020
 ADD R0,R0,R2 ; Convert binary to ASCII
 TRAP x21 ; ASCII code in R0 is displayed
 TRAP x25 ; Halt machine
 ;
 ; Storage for pointer and ASCII template
 ;
 ASCII .FILL x0030
 PTR .FILL x4000
 .END