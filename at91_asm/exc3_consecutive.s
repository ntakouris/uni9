@BOARD AT91
.arm
.text
.global main

main:
STMDB R13!, {R0-R12, R14}

MOV R0, #0
LDR R1, =Stor

LOOP:

STRB R0, [R1, R0]

CMP R0, #5
ADD R0, R0, #1
BNE LOOP

LDMIA R13!, {R0-R12, PC}

.data
Stor:
.byte 0x0, 0x0, 0x0, 0x0, 0x0