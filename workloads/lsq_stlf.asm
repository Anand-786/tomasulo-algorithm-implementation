LOOP_A:     LOAD  R3, 0(R10)
    ADDI   R4, R3, 1
    STORE R4, 0(R10)
    BEQ   R1, R0, LOOP_A