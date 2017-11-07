@BOARD AT91
.arm
.text
.global main

main:
STMDB R13!, {R0-R12, R14}

MOV R0, #2
LDR R1, =Stor

LOOP:

MOV R4, R0

@An-1
LDRB R2, [R1, R4]

ADD R4, R4, #1

@An-2
LDRB R3, [R1, R4]

ADD R4, R2 , R3
STRB R4, [R1, R4]

CMP R0, #5
ADD R0, R0, #1
BNE LOOP

LDMIA R13!, {R0-R12, PC}

.data
Stor:
@First 2 fibonacci
.byte 0x1, 0x2, 0x0, 0x0, 0x0