#include <iostream>
#include <iomanip>
#include <fstream>
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
    private:
        ofstream stats_file;
    public:
        void updateDerivedStats(){
            stats->ipc = (double)(stats->total_insts)/(stats->total_cycles);
            stats->wallclock_time_secs /= 1000;
            stats->inst_latency_avg = (double)(stats->total_instr_latency)/(stats->total_insts);  
            stats->rob_avg_occupancy = (double)(stats->total_rob_occupancy)/(stats->total_cycles);
            stats->lsq_avg_occupancy = (double)(stats->total_lsq_occupancy)/(stats->total_cycles);
            stats->rs_avg_occupancy = (double)(stats->total_rs_occupancy)/(stats->total_cycles);
            stats->fu_alu_utilization = (double)(stats->total_alu_occupancy)/(stats->total_cycles);
            stats->fu_mul_utilization = (double)(stats->total_mul_occupancy)/(stats->total_cycles);
            stats->fu_div_utilization = (double)(stats->total_div_occupancy)/(stats->total_cycles);
            stats->l1d_overall_hit_rate = (double)(stats->l1d_overall_hits)/(stats->l1d_overall_accesses);
            stats->l1d_overall_miss_rate = (double)(stats->l1d_overall_misses)/(stats->l1d_overall_accesses);
            stats->l1d_avg_miss_penalty = (double)(stats->l1d_total_miss_penalty)/(stats->l1d_overall_misses);
            stats->l1d_amat = 1 + (stats->l1d_overall_miss_rate)*(stats->l1d_avg_miss_penalty);
        }
        void dumpStats(){
            stats_file.open("../results/stats.txt");
            if(stats_file.is_open()){
                updateDerivedStats();

                const int KEY_WIDTH = 35;

                stats_file << fixed << setprecision(4);

                stats_file << "# --- Core Performance ---" << "\n";
                stats_file << left << setw(KEY_WIDTH) << "sim.total_cycles" << "= " << stats->total_cycles << "\n";
                stats_file << left << setw(KEY_WIDTH) << "sim.total_insts" << "= " << stats->total_insts << "\n";
                stats_file << left << setw(KEY_WIDTH) << "sim.ipc" << "= " << stats->ipc << "\n";
                stats_file << left << setw(KEY_WIDTH) << "sim.wallclock_time_secs" << "= " << stats->wallclock_time_secs << "\n";
                stats_file << "\n";

                stats_file << "# --- Instruction Level ---" << "\n";
                stats_file << left << setw(KEY_WIDTH) << "commit.op_class.IntAlu" << "= " << stats->op_class_int_alu << "\n";
                stats_file << left << setw(KEY_WIDTH) << "commit.op_class.IntMul" << "= " << stats->op_class_int_mul << "\n";
                stats_file << left << setw(KEY_WIDTH) << "commit.op_class.IntDiv" << "= " << stats->op_class_int_div << "\n";
                stats_file << left << setw(KEY_WIDTH) << "commit.op_class.MemRead" << "= " << stats->op_class_mem_read << "\n";
                stats_file << left << setw(KEY_WIDTH) << "commit.op_class.MemWrite" << "= " << stats->op_class_mem_write << "\n";
                stats_file << left << setw(KEY_WIDTH) << "commit.inst_latency.avg" << "= " << stats->inst_latency_avg << "\n";
                stats_file << left << setw(KEY_WIDTH) << "commit.inst_latency.max" << "= " << stats->inst_latency_max << "\n";
                stats_file << "\n";

                stats_file << "# --- Pipeline Analysis ---" << "\n";
                stats_file << left << setw(KEY_WIDTH) << "rob.avg_occupancy" << "= " << stats->rob_avg_occupancy << "\n";
                stats_file << left << setw(KEY_WIDTH) << "rob.peak_occupancy" << "= " << stats->rob_peak_occupancy << "\n";
                stats_file << left << setw(KEY_WIDTH) << "lsq.avg_occupancy" << "= " << stats->lsq_avg_occupancy << "\n";
                stats_file << left << setw(KEY_WIDTH) << "lsq.peak_occupancy" << "= " << stats->lsq_peak_occupancy << "\n";
                stats_file << left << setw(KEY_WIDTH) << "rs.avg_occupancy" << "= " << stats->rs_avg_occupancy << "\n";
                stats_file << left << setw(KEY_WIDTH) << "fu.alu_utilization" << "= " << stats->fu_alu_utilization << "\n";
                stats_file << left << setw(KEY_WIDTH) << "fu.mul_utilization" << "= " << stats->fu_mul_utilization << "\n";
                stats_file << left << setw(KEY_WIDTH) << "fu.div_utilization" << "= " << stats->fu_div_utilization << "\n";
                stats_file << "\n";

                stats_file << "# --- Dependency and Hazards ---" << "\n";
                stats_file << left << setw(KEY_WIDTH) << "rs.raw_hazard_stall_cycles" << "= " << stats->raw_hazard_stall_cycles << "\n";
                stats_file << left << setw(KEY_WIDTH) << "lsq.store_to_load_forwards" << "= " << stats->store_to_load_forwards << "\n";
                stats_file << left << setw(KEY_WIDTH) << "cdb.total_broadcasts" << "= " << stats->cdb_total_broadcasts << "\n";
                stats_file << "\n";

                stats_file << "# --- L1 Data Cache ---" << "\n";
                stats_file << left << setw(KEY_WIDTH) << "l1d.overall_accesses" << "= " << stats->l1d_overall_accesses << "\n";
                stats_file << left << setw(KEY_WIDTH) << "l1d.overall_hits" << "= " << stats->l1d_overall_hits << "\n";
                stats_file << left << setw(KEY_WIDTH) << "l1d.overall_misses" << "= " << stats->l1d_overall_misses << "\n";
                stats_file << left << setw(KEY_WIDTH) << "l1d.overall_hit_rate" << "= " << stats->l1d_overall_hit_rate << "\n";
                stats_file << left << setw(KEY_WIDTH) << "l1d.overall_miss_rate" << "= " << stats->l1d_overall_miss_rate << "\n";
                stats_file << left << setw(KEY_WIDTH) << "l1d.read_accesses" << "= " << stats->l1d_read_accesses << "\n";
                stats_file << left << setw(KEY_WIDTH) << "l1d.read_hits" << "= " << stats->l1d_read_hits << "\n";
                stats_file << left << setw(KEY_WIDTH) << "l1d.read_misses" << "= " << stats->l1d_read_misses << "\n";
                stats_file << left << setw(KEY_WIDTH) << "l1d.write_accesses" << "= " << stats->l1d_write_accesses << "\n";
                stats_file << left << setw(KEY_WIDTH) << "l1d.write_hits" << "= " << stats->l1d_write_hits << "\n";
                stats_file << left << setw(KEY_WIDTH) << "l1d.write_misses" << "= " << stats->l1d_write_misses << "\n";
                stats_file << left << setw(KEY_WIDTH) << "l1d.avg_miss_penalty" << "= " << stats->l1d_avg_miss_penalty << "\n";
                stats_file << left << setw(KEY_WIDTH) << "l1d.amat" << "= " << stats->l1d_amat << "\n";
                stats_file<<endl;
            }
            else{
                cout<<" Could not create stats.txt file!"<<endl;
                return;
            }
        }
};