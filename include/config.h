#pragma once
#include <string>

struct SimConfig{
    //core
    int rob_size;
    int lsq_size;

    //ReservationStations
    int num_alu_rs;
    int num_mul_rs;
    int num_div_rs;

    //Latencies
    int alu_latency;
    int mul_latency;
    int div_latency;

    //Cache
    int num_sets;
    int associativity;
    int block_size;
    int address_bits;
    int miss_penalty_no_replacement;
    int miss_penalty_with_replacement;
};