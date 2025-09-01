        ADDI R1, R0, 50
                Loop:   ADDI R2, R0, 10
                                ADD  R3, R1, R2
STORE R1, 0(R3)
                        LOAD  R4, 0(R1)
                SUB  R5, R4, R2
                                                BEQ   R15, R16, Loop