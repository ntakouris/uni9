@BOARD AT91
.text
.global main

main: 
STMDB {R0-R3}


@Initialize counter
MOV R0, #0x1

LOOP:
@Load contents of lines of arrays
LDRB R1, [=ArrayA, R0]
LDRB R2, [=ArrayB, R0]

ADD R3, R1, R2

@Save addition result into ArrayC
STRB R3, [=ArrayC, R0]

CMP R0, #0x81  @8 times 16 + 1 = 129

@Increment counter (only takes effect if jump happens afterwards)
ADD R0, R0, #0x8

@If not done, increment counter (R3) and jump back
BEQ LOOP @Jump to loop


LDMIA {R0-R3, PC}

@Arrays A & B will be added together line-by line
@Array C stores the results
@All Array lenghts are 16 bytes longs
.data
ArrayA:
.byte 0x1F, 0x7F, 0xFF, 0x39, 0x16, 0x6F, 0x30, 0xB, 0x57, 0x2D, 0x72, 0x2D, 0x42, 0x17, 0x86, 0xA8

.ArrayB:
.byte 0x13, 0x1, 0x12, 0x59, 0x5A, 0x70, 0x59, 0x20, 0x17, 0x62, 0x43, 0x53, 0x92, 0x8C, 0xC8, 0x43

.ArrayC
.byte 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
