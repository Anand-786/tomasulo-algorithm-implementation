// --- Workload for N = 5 Conflicting Addresses ---

LOOP_5:
    LOAD  R2, 0(R10)       // Address A
    LOAD  R3, 4096(R10)    // Address B
    LOAD  R4, 8192(R10)    // Address C
    LOAD  R5, 12288(R10)   // Address D
    LOAD  R6, 16384(R10)   // Address E, causes VC thrash
    SUBI  R1, R1, 1
    BNE   R1, R0, LOOP_5