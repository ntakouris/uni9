@BOARD AT91
.text
.global main

main: 
STMDB R13!, {R0-R12, R14}

START:
LDR R0, =Array
MOV R1, #20

BL Sort 
BL Check

END:
LDMIA R13!, {R0-R12, PC}

Sort:
STMDB R13!, {R2-R6}

MOV R7, #0 @Index Tracker

OuterLoop:
LDRB R3, [R0, R7] @Current Minimum Element
MOV R4, R7 @Current Minimum Element Index
MOV R2, R7 @Counter
GreaterLoop:
ADD R2, R2, #1

LDRB R5, [R0, R2] @Next Element
MOV R6, R2 @Next Element Index

CMP R5, R3 @Find Greater

BHI GreaterLoopEnd
@These execute only if R3 > R5 (find max element)
MOV R3, R5
MOV R4, R6 
GreaterLoopEnd:

CMP R2, R1
BNE GreaterLoop

LDRB R5, [R0, R7] 

STRB R5, [R0, R4]
STRB R3, [R0, R7]

ADD R7, R7, #1
CMP R7, R1
BNE OuterLoop

LDMIA R13!, {R2-R6}
MOV PC, LR

Check:
STMDB R13!, {R2-R6}

LDR R2, =Correct
MOV R3, #0
MOV R6, #0x0000

CL:
LDRB R4, [R0, R3]
ADD R3, R3, #1
LDRB R5, [R0, R3]

CMP R4, R5
@If R4 > R5 - Wrong
STRHI R6, [R2]

CMP R3, #20
BNE CL

LDMIA R13!, {R2-R6}
MOV PC, LR

.data
Array:
.byte 0x02, 0x01, 0x05, 0x04, 0x03, 0x06, 0x09 , 0x08, 0x07, 0x0A, 0x0C, 0x0B, 0x0F, 0x0E, 0x0D, 0xFF, 0x20, 0x2F, 0x1F

Correct:
.word 0xFFFF
