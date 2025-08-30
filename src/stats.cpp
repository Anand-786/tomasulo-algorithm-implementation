#include <iostream>
#include <iomanip>
using ll = long long;
using d = double;
using namespace std;

struct Stats{
    //Core Performance
    ll total_cycles = 0;
    ll total_insts = 0;
    d ipc = 0;
    d wallclock_time_secs = 0;

    //Instruction Level
    ll op_class_int_alu = 0;
    ll op_class_int_mul = 0;
    ll op_class_int_div = 0;
    ll op_class_mem_read = 0;
    ll op_class_mem_write = 0;
    d inst_latency_avg = 0;
    ll inst_latency_max = 0;


    //Pipeline Analysis
    d rob_avg_occupancy = 0;
    ll rob_peak_occupancy = 0;
    d lsq_avg_occupancy = 0;
    ll lsq_peak_occupancy = 0;
    d rs_avg_occupancy = 0;
    d fu_alu_utilization = 0;
    d fu_mul_utilization = 0;
    d fu_div_utilization = 0;

    //Dependency and Hazards
    ll raw_hazard_stall_cycles = 0;
    ll store_to_load_forwards = 0;
    ll cdb_total_broadcasts = 0;
    ll cdb_broadcast_contention = 0;

    //L1 Cache
    ll l1d_overall_accesses = 0;
    ll l1d_overall_hits = 0;
    ll l1d_overall_misses = 0;
    d l1d_overall_hit_rate = 0;
    d l1d_overall_miss_rate = 0;
    ll l1d_read_accesses = 0;
    ll l1d_read_hits = 0;
    ll l1d_read_misses = 0;
    ll l1d_write_accesses = 0;
    ll l1d_write_hits = 0;
    ll l1d_write_misses = 0;
    ll l1d_misses_on_clean = 0;
    ll l1d_misses_on_dirty = 0;
    d l1d_avg_miss_penalty = 0;
    d l1d_amat = 0;
    ll l1d_total_evictions = 0;
};

Stats *stats = new Stats();

class Statistics{
    public:
};