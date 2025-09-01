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
    int mem_fetch_latency;
    int wb_latency;
    int vc_access_latency; 
    int victim_cache_size;

    //Simulation
    string filepath;
    int num_iterations;
    bool victim_cache_enabled;
    bool trace_file_enabled;
    bool gantt_chart_enabled;
    bool instruction_table_display;
};