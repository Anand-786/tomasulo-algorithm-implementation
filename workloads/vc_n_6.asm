// --- Workload for N = 6 Conflicting Addresses ---

LOOP_6:
    LOAD  R2, 0(R10)       // Address A
    LOAD  R3, 4096(R10)    // Address B
    LOAD  R4, 8192(R10)    // Address C
    LOAD  R5, 12288(R10)   // Address D
    LOAD  R6, 16384(R10)   // Address E
    LOAD  R7, 20480(R10)   // Address F, causes more VC thrashing
    SUBI  R1, R1, 1
    BNE   R1, R0, LOOP_6