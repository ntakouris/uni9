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
AfterLoop:

CMP R8, #3
ADD R8, R8, #1

BNE LOOP

END:
LDMIA R13!, {R0-R12, PC}

Subrtn:

MOV R4, #6 @i
LDRB R5, [R2, R4] @Bi (b6 initial)

Calc:
CMP R4, #0

SUB R4, R4, #1
LDRB R6, [R2, R6] @Ai

MUL R5, R0, R5
ADD R5, R5, R6

BNE Calc

Result: @CHECK R5 HERE

B AfterLoop
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