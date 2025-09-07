LOOP:   LOAD  R2, 0(R10)
    LOAD  R3, 0(R12)
    ADD   R4, R10, R11
    MUL   R5, R12, R13
    LOAD  R6, 0(R14)
    LOAD  R7, 0(R16)
    ADD   R8, R14, R15
    MUL   R9, R16, R17
    LOAD  R18, 0(R18)
    LOAD  R19, 0(R20)
    ADD   R20, R18, R19
    MUL   R21, R20, R21
    LOAD  R22, 0(R22)
    LOAD  R23, 0(R24)
    ADD   R24, R22, R23
    MUL   R25, R24, R25
    BEQ   R1, R0, LOOP