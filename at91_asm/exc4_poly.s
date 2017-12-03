@BOARD AT91
.arm
.text
.global main

main:
STMDB R13!, {R0-R12, R14}

LDR R1, =Values
LDR R2, =Const
MOV R3, #0 @Result

MOV R8, #0 @Counter

LOOP:

LDR R0, [R1, R8]

BL Subrtn

CMP R8, #6
ADD R8, R8, #1

BNE LOOP

END:
LDMIA R13!, {R0-R12, PC}

Subrtn:
STMDB R13!, {R4, R5, R6}

MOV R5, R8

CMP R8 , #0
MOV R6, #1
BEQ Powend @If power is 0

POW: @R6 = R0 ^ R8
MUL R6, R6, R0

CMP R5, #1
SUB R5, R5, #1
BNE POW

POWEND:

LDRB R4, [R2, R8] @Ai

MUL R6, R6, R4 @Ai * X ^ i
@Add to result
ADD R3, R3, R6

LDMIA R13!, {R4, R5, R6}
MOV PC, LR 
@Subrtn

.data
Values:
.word 0x10
.word 0x50A
.word 0xCDCA
.word 0x80AB

Const:
.byte 0x04, 0x07, 0x05
.byte 0x20, 0x1A, 0x12, 0x06