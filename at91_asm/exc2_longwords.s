@BOARD AT91
.text
.global main

main: 
STMDB R13!, {R0-R12, R14}


@Initialize counter
MOV R0, #0

@Push label addresses into registers
LDR R4, =arrayA
LDR R5, =arrayB
LDR R6, =arrayC

LOOP:
@Load contents of lines of arrays
LDR R1, [R4, R0]
LDR R2, [R5, R0]

@Add with carry on status register
ADC R3, R1, R2

CMP R0, #14

@Increment counter (only takes effect if jump happens afterwards)
ADD R0, R0, #4

@If not done, increment counter (R3) and jump back
BNE LOOP

@Save addition result into ArrayC
STR R3, [R6]

LDMIA R13!, {R0-R12, PC}

@Arrays A & B will be added together line-by line
@Array C stores the results
@All Array lenghts are 16 bytes longs
.data
arrayA:
.byte 0x20, 0x7F, 0xFF, 0x39, 0x16, 0x6F, 0x30, 0xB, 0x57, 0x2D, 0x72, 0x2D, 0x42, 0x17, 0x86, 0xA8

arrayB:
.byte 0x13, 0x1, 0x12, 0x59, 0x5A, 0x70, 0x59, 0x20, 0x17, 0x62, 0x43, 0x53, 0x92, 0x8C, 0xC8, 0x43

arrayC:
.byte 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
