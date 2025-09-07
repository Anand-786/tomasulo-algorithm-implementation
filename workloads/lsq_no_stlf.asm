// GOAL: Perform independent loads and stores WITH NO SPATIAL LOCALITY.
// R10 is our base pointer, assumed to be 0 initially.
// R1  = Loop Counter

LOOP_B_REVISED:
    // The STORE targets the address in R10.
    STORE R2, 0(R10)

    // The LOAD targets an address far away from the STORE's address.
    LOAD  R3, 10000(R10)

    // Move to the next block of memory for the next iteration.
    // The stride (128) is > the cache block size (64), guaranteeing a miss.
    ADDI  R10, R10, 128
    SUBI  R1, R1, 1
    BNE   R1, R0, LOOP_B_REVISED