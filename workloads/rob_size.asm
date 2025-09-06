// GOAL: Overwhelm a small ROB with a large number of independent instructions.
// R1 = Loop Counter (e.g., set to 100)
// R0 = 0

LOOP:
    // First block of 8 independent instructions
    LOAD  R2, 0(R10)
    LOAD  R3, 0(R12)
    ADD   R4, R10, R11
    MUL   R5, R12, R13
    LOAD  R6, 0(R14)
    LOAD  R7, 0(R16)
    ADD   R8, R14, R15
    MUL   R9, R16, R17

    // Second block of 8 more independent instructions
    LOAD  R18, 0(R18)
    LOAD  R19, 0(R20)
    ADD   R20, R18, R19
    MUL   R21, R20, R21
    LOAD  R22, 0(R22)
    LOAD  R23, 0(R24)
    ADD   R24, R22, R23
    MUL   R25, R24, R25

    SUBI  R1, R1, 1
    BNE   R1, R_ZERO, LOOP