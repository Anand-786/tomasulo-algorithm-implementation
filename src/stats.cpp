#include <iostream>
#include <iomanip>
using ll = long long;
using d = double;
using namespace std;

struct Stats{
    //Core Performance
    ll total_cycles = 0;                    //counter
    ll total_insts = 0;                     //counter
    d ipc = 0;                              //derived : total_instr/total_cycles
    d wallclock_time_secs = 0;              //derived : endtime - startime (milliseconds)

    //Instruction Level
    ll op_class_int_alu = 0;                //No. of ALU type instr
    ll op_class_int_mul = 0;                //No. of MUL type instr
    ll op_class_int_div = 0;                //No. of DIV type instr
    ll op_class_mem_read = 0;               //No. of Load type instr
    ll op_class_mem_write = 0;              //No. of Store type instr
    ll total_instr_latency = 0;             //counter
    d inst_latency_avg = 0;                 //Average time taken for 1 instr : Sum(Commit - Issue)/total_instr
    ll inst_latency_max = 0;                //Max time taken for 1 instr


    //Pipeline Analysis
    ll total_rob_occupancy= 0 ;             //counter
    d rob_avg_occupancy = 0;                //Sum(occupancy in each cycle)/total_cycles
    ll rob_peak_occupancy = 0;              //counter
    ll total_lsq_occupancy = 0;             //counter
    d lsq_avg_occupancy = 0;                //Sum(occupancy in each cycle)/total_cycles
    ll lsq_peak_occupancy = 0;              //counter
    ll total_rs_occupancy = 0;              //counter
    d rs_avg_occupancy = 0;                 //Sum(occupancy in each cycle)/total_cycles
    ll total_alu_occupancy = 0;             //counter
    d fu_alu_utilization = 0;               //Sum(occupancy in each cycle)/total_cycles
    ll total_mul_occupancy = 0;             //counter
    d fu_mul_utilization = 0;               //Sum(occupancy in each cycle)/total_cycles
    ll total_div_occupancy = 0;             //counter
    d fu_div_utilization = 0;               //Sum(occupancy in each cycle)/total_cycles

    //Dependency and Hazards
    ll raw_hazard_stall_cycles = 0;         //counter
    ll store_to_load_forwards = 0;          //counter
    ll cdb_total_broadcasts = 0;            //counter

    //L1 Cache
    ll l1d_overall_accesses = 0;            //counter
    ll l1d_overall_hits = 0;                //counter
    ll l1d_overall_misses = 0;              //counter
    d l1d_overall_hit_rate = 0;             //derived : overall_hits/total_access
    d l1d_overall_miss_rate = 0;            //derived : overall_misses/total_access
    ll l1d_read_accesses = 0;               //counter
    ll l1d_read_hits = 0;                   //counter
    ll l1d_read_misses = 0;                 //counter
    ll l1d_write_accesses = 0;              //counter
    ll l1d_write_hits = 0;                  //counter
    ll l1d_write_misses = 0;                //counter
    ll l1d_total_miss_penalty = 0;          //counter
    d l1d_avg_miss_penalty = 0;             //derived : total_miss_penalty/total_misses
    d l1d_amat = 0;                         //derived : hit_time + miss_rate * avg_miss_penalty
};

Stats *stats = new Stats();

class Statistics{
    public:
};