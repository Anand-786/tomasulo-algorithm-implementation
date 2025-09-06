// --- Workload for N = 2 Conflicting Addresses ---
// R10 is the base register, assumed to be 0
// R1 is the loop counter
// R0 is the zero register for comparison

LOOP_2:
    LOAD  R2, 0(R10)       // Access address A
    LOAD  R3, 4096(R10)    // Access address B (A + 4096), conflicts with A
    SUBI  R1, R1, 1
    BNE   R1, R0, LOOP_2