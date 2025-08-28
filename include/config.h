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
};