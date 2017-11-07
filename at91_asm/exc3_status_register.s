@BOARD AT91
.arm
.text
.global main

main:
STMDB R13!, {R0-R12, R14}

MOV R0, #94
MOV R1, R0, LSR #1

@S Means flags are updated by the result of the operation
ADDS R2, R0, R0 @

ADDS R2, R1, R1 @

ADDS R2, R0, R1 @

MOV R0, #0x80000000

ADD R1, R0, #0x80
MOV R2, #1

@Same here
SUBS R3, R0, R2 @

SUBS R3, R0, R1 @

@RSB Rd, R0, R1 <=> SUB Rd, R1, R0
RSBS R3, R0, R1 @

LDMIA R13!, {R0-R12, PC}