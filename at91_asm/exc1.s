@BOARD AT91
@Defaults for instructions as a size target is word (unless specified to move halfword or byte)
.arm
.text
.global main

main:

@Store register state to restore upon the end of the program
@! = Refresh the register afterwards
STMDB R13!, {R0-R12, R14}

MOV R0, #0x20

@LSL = Left logical shift by #N
MOV R1, R0, LSL #2

@MVN = MOV and Invert content
MVN R2, R1, LSL #1

@Loads Values' label contents into R3 (first word only)
LDR R3, =Values

@[] = The value (memory address) of R3
@#N = Adds to the content before moving to target (R4) 

@Loads contents of the memory address contained in R3 plus 4 into R4
LDR R4, [R3], #4

@B = Move whole byte instead
LDRB R5, [R3], #2
@SH = Load register signed halfword (immediate offset)
LDRSH R6, [R3], #2

LDR R3, =Stack

@,refresh R3
STMIA R3!, {R0-R2, R4-R6}
@LDM Starts from some address and progressively moves data onto the target registers
@DB Means that the memory accessed is going to be decremented by 4 before access (in this case just for R3)
LDMDB R3!, {R0-R2}
LDMDB R3!, {R4-R6}

@Restore register state because program terminated
LDMIA R13!, {R0-R12, PC}


.data
Values:
.word 0xCAFEBABA
.word 0x82345678

Stack:
.word 0,0,0,0
.word 0,0,0,0