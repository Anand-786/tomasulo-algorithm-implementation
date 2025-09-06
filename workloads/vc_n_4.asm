// --- Workload for N = 4 Conflicting Addresses ---

LOOP_4:
    LOAD  R2, 0(R10)       // Address A
    LOAD  R3, 4096(R10)    // Address B
    LOAD  R4, 8192(R10)    // Address C
    LOAD  R5, 12288(R10)   // Address D, conflicts with A, B, C
    SUBI  R1, R1, 1
    BNE   R1, R0, LOOP_4