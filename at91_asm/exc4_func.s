@BOARD AT91
.arm
.text
.global main

main:
STMDB R13!, {R0-R12, R14}

LDR R0, =Values
LDR R4, =Const

LDRB R5, [R4] @Z0
LDRB R6, [R4, #1] @Z1
LDRB R7, [R4, #2] @Z2

MOV R8, #0 @Counter

LOOP:

B Subrtn

AfterLoop:
ADD R0,R0, #3

CMP R8, #3
ADD R8, R8, #1

BNE LOOP

END:
LDMIA R13!, {R0-R12, PC}

Subrtn:

LDRB R1, [R0] @Ai
LDRB R2, [R0, #1] @Bi
LDRB R3, [R0, #2] @Ci

@Calculate: 5 * (Ai * Z0 + Bi * Z1 - Ci * Z2) / 64

MUL R1, R5, R1 
MUL R2, R6, R2
MUL R3, R7, R3

ADD R1, R1, R2
SUB R1, R1, R3

MOV R9, #5
MUL R1, R9, R1 

QI:
@64 = 2^6
@LSR R3, R1, #6
MOV R3, R1,LSR #6																	     

@Check if resut > [Const, #3]
LDRB R2, [R4, #3]

CMP R3, R2

BHI UpdateGreater @Break if Higher (R3 > R2)

ExitSubrtn:
B AfterLoop 
@Subrtn

UpdateGreater:
STRB R3, [R4, #3]
STRB R8, [R4, #4]
B ExitSubrtn

.data
Values:
.byte 0x02, 0x03, 0x04
.byte 0x10, 0x05, 0x06
.byte 0x0B, 0x02, 0x0D
.byte 0x01, 0x0C, 0x08

Const:
.byte 0x04, 0x07, 0x05, 0x00, 0x00
