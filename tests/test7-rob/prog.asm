LOOP:   LOAD  R2, 0(R10)
        LOAD  R3, 0(R12)
        ADD   R4, R10, R11
        MUL   R5, R12, R13
        LOAD  R6, 0(R14)
        LOAD  R7, 0(R16)
        ADD   R8, R14, R15
        MUL   R9, R16, R17
        SUBI  R1, R1, 1
        BEQ   R1, R0, LOOP