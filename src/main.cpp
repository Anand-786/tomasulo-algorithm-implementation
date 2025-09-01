#include "logs.cpp"
#include <iostream>
#include <cstdlib>
#include "INIReader.h"
using namespace std;

void printConfig(string& filename, SimConfig *config, Cache *cache) {
    const int keyWidth = 22, valWidth = 27;
    cout << " [ CONFIGURATION ]" << endl;
    cout << " +----------------------------------------------------------+" << endl;
    
    cout << " | > L1 Cache Properties :" << setw(valWidth+7)<<" "<<"|"<< endl;
    cout << left;
    cout << " |     - " << setw(keyWidth) << "Size" << ": " << setw(valWidth) << to_string((int)pow(2, cache->getL1Size()))+" KB" << "|" << endl;
    cout << " |     - " << setw(keyWidth) << "#Sets" << ": " << setw(valWidth) << to_string(config->num_sets) << "|" << endl;
    cout << " |     - " << setw(keyWidth) << "Associativity" << ": " << setw(valWidth) << to_string(config->associativity)+"-way" << "|" << endl;
    cout << " |     - " << setw(keyWidth) << "Block Size" << ": " << setw(valWidth) << to_string(config->block_size)+" B" << "|" << endl;
    cout << " |     - " << setw(keyWidth) << "Physical Address" << ": " << setw(valWidth) << to_string(config->address_bits)+" bits" << "|" <<  endl;
    cout << " |     - " << setw(keyWidth) << "Victim Cache Access" << ": " << setw(valWidth) << to_string(config->vc_access_latency)+" cycles" <<  "|" << endl;
    cout << " |     - " << setw(keyWidth) << "Memory Fetch Latency" << ": " << setw(valWidth) << to_string(config->mem_fetch_latency)+" cycles" << "|" <<  endl;
    cout << " |     - " << setw(keyWidth) << "Writeback Latency" << ": " << setw(valWidth) << to_string(config->wb_latency)+" cycles" <<  "|" << endl;
    cout << " |                                                          |"<<endl;
    cout << " | > Simulator Properties :" <<setw(valWidth+6)<<" "<<"|"<< endl;
    cout << " |     - " << setw(keyWidth) << "Program File" << ": " << setw(valWidth) << filename <<  "|" << endl;
    cout << " |     - " << setw(keyWidth) << "#Iterations" << ": " << setw(valWidth) << to_string(config->num_iterations) <<  "|" << endl;
    cout << " |     - " << setw(keyWidth) << "ROB entries" << ": " << setw(valWidth) << to_string(config->rob_size) << "|" <<  endl;
    cout << " |     - " << setw(keyWidth) << "LSQ entries" << ": " << setw(valWidth) << to_string(config->lsq_size) <<  "|" << endl;
    cout << " |     - " << setw(keyWidth) << "Reservation Stations" << ": " << setw(valWidth) << to_string(config->num_alu_rs)+" (ALU), "+to_string(config->num_mul_rs)+" (MUL), "+to_string(config->num_div_rs)+" (DIV)" <<  "|" << endl;
    cout << " |     - " << setw(keyWidth) << "Latencies (cycles)" << ": " << setw(valWidth) << to_string(config->alu_latency)+" (ALU), "+to_string(config->mul_latency)+" (MUL), "+to_string(config->div_latency)+" (DIV)" <<  "|" << endl;

    cout << " +----------------------------------------------------------+" << endl;
    cout << endl<<endl;
}

void printStatsSummary(){
    const int keyWidth = 32;
    const int valWidth = 20;
    stringstream ss;
    ss << fixed << setprecision(2);

    cout << " [ SIMULATION SUMMARY ]" << endl;
    cout << " +----------------------------------------------------------+" << endl;
    cout << left;
    cout << " |  - " << setw(keyWidth) << "Total Execution Cycles" << ": " << setw(valWidth) << to_string(stats->total_cycles) << "|" << endl;
    cout << " |  - " << setw(keyWidth) << "Total Instructions Committed" << ": " << setw(valWidth) << to_string(stats->total_insts) << "|" << endl;
    ss << stats->ipc;
    cout << " |  - " << setw(keyWidth) << "IPC (Instructions/Cycle)" << ": " << setw(valWidth) << ss.str() << "|" << endl;
    ss.str(""); 
    ss.clear();
    ss << (stats->wallclock_time_secs * 1000);
    cout << " |  - " << setw(keyWidth) << "Wall-Clock Time" << ": " << setw(valWidth) << (ss.str() + " ms") << "|" << endl;
    ss.str(""); 
    ss.clear();
    ss << stats->l1d_amat;
    cout << " |  - " << setw(keyWidth) << "Avg. Memory Access Time[AMAT]" << ": " << setw(valWidth) << (ss.str() + " cycles") << "|" << endl;
    ss.str(""); 
    ss.clear();
    ss << (stats->l1d_overall_miss_rate * 100.0);
    cout << " |  - " << setw(keyWidth) << "L1 Cache Miss Rate" << ": " << setw(valWidth) << (ss.str() + " %") << "|" << endl;
    ss.str(""); 
    ss.clear();
    ss << stats->l1d_avg_miss_penalty;
    cout << " |  - " << setw(keyWidth) << "Avg. L1 Miss Penalty" << ": " << setw(valWidth) << (ss.str() + " cycles") << "|" << endl;
    ss.str(""); 
    ss.clear();
    ss << stats->inst_latency_avg;
    cout << " |  - " << setw(keyWidth) << "Avg. Instruction Latency" << ": " << setw(valWidth) << (ss.str() + " cycles") << "|" << endl;
    ss.str(""); 
    ss.clear();
    cout << " +----------------------------------------------------------+" << endl;
    cout << endl<<endl;
}

void printFileLocation(){
    cout << " [ SIMULATION OUTPUT FILES ]" << endl;
    cout << " +----------------------------------------------------------+" << endl;
    cout << left;
    string trace_line = "> Detailed trace log:      results/trace.log";
    cout << " |  " << setw(56) << trace_line << "|" << endl;
    string stats_line = "> Full statistics report:  results/stats.txt";
    cout << " |  " << setw(56) << stats_line << "|" << endl;
    cout << " +----------------------------------------------------------+" << endl;
    cout << endl;
}

void createInstructionStatusFile(map<int, instructionLog*> *instructionLogs){
    ofstream istatus;
    istatus.open("instruction_status.txt");
    if(istatus.is_open()){
            istatus << left << setw(25) << "Instruction" << "| "
                    << setw(7) << "Issue" << "| "
                    << setw(11) << "ExecStart" << "| "
                    << setw(9) << "ReadMem" << "| "
                    << setw(10) << "WriteCDB" << "| "
                    << setw(8) << "Commit" << "\n";
                istatus << setfill('-') << setw(80) << "" << setfill(' ') << "\n";
                for (auto const& [key, val] : *instructionLogs) {
                    istatus << left << setw(25) << val->instruction << "| "
                        << setw(7) << (val->issueCycle == 0 ? "-" : to_string(val->issueCycle)) << "| "
                        << setw(11) << (val->executeStartCycle == 0 ? "-" : to_string(val->executeStartCycle)) << "| "
                        << setw(9) << (val->memAccessCycle == 0 ? "-" : to_string(val->memAccessCycle)) << "| "
                        << setw(10) << (val->writeCDBCycle == 0 ? "-" : to_string(val->writeCDBCycle)) << "| "
                        << setw(8) << (val->commitCycle == 0 ? "-" : to_string(val->commitCycle)) << endl;
                }
            istatus.close();
    }
    else{
        cout<<"Could not create instruction_status file for Gantt chart!"<<endl;
    }
}

//Loads Config.ini to our SimConfig Struct
SimConfig* loadConfig(string &config_path){
    INIReader reader(config_path);
    if(reader.ParseError() < 0){
        cout<<"Could not load Configuration!\n\nExiting...";
        exit(1);
    }

    SimConfig *config = new SimConfig();
    config->rob_size = reader.GetInteger("Core", "rob_size", 16);
    config->lsq_size = reader.GetInteger("Core", "lsq_size", 8);
    config->num_alu_rs = reader.GetInteger("ReservationStations", "num_alu_rs", 8);
    config->num_mul_rs = reader.GetInteger("ReservationStations", "num_mul_rs", 4);
    config->num_div_rs = reader.GetInteger("ReservationStations", "num_div_rs", 2);
    config->alu_latency = reader.GetInteger("Latencies", "alu_latency", 1);
    config->mul_latency = reader.GetInteger("Latencies", "mul_latency", 5);
    config->div_latency = reader.GetInteger("Latencies", "div_latency", 10);
    config->num_sets = reader.GetInteger("Cache", "num_sets", 64);
    config->associativity = reader.GetInteger("Cache", "associativity", 8);
    config->block_size = reader.GetInteger("Cache", "block_size", 64);
    config->address_bits = reader.GetInteger("Cache", "address_bits", 32);
    config->mem_fetch_latency = reader.GetInteger("Cache", "mem_fetch_latency", 50);
    config->wb_latency = reader.GetInteger("Cache", "wb_latency", 10);
    config->vc_access_latency = reader.GetInteger("Cache", "vc_access_latency", 3);
    config->filepath = reader.GetString("Simulation", "program_file_path", "program.asm");
    config->num_iterations = reader.GetInteger("Simulation", "num_iterations", 2);
    return config;
}

int main(){
    string ascii_art_banner = R"(
    $$$$$$$$\  $$$$$$\  $$\      $$\  $$$$$$\   $$$$$$\  $$\   $$\ $$\       $$$$$$\         $$$$$$\  $$$$$$\ $$\      $$\ $$\   $$\ $$\        $$$$$$\ $$$$$$$$\  $$$$$$\  $$$$$$$\  
    \__$$  __|$$  __$$\ $$$\    $$$ |$$  __$$\ $$  __$$\ $$ |  $$ |$$ |     $$  __$$\       $$  __$$\ \_$$  _|$$$\    $$$ |$$ |  $$ |$$ |      $$  __$$\\__$$  __|$$  __$$\ $$  __$$\ 
       $$ |   $$ /  $$ |$$$$\  $$$$ |$$ /  $$ |$$ /  \__|$$ |  $$ |$$ |     $$ /  $$ |      $$ /  \__|  $$ |  $$$$\  $$$$ |$$ |  $$ |$$ |      $$ /  $$ |  $$ |   $$ /  $$ |$$ |  $$ |
       $$ |   $$ |  $$ |$$\$$\$$ $$ |$$$$$$$$ |\$$$$$$\  $$ |  $$ |$$ |     $$ |  $$ |      \$$$$$$\    $$ |  $$\$$\$$ $$ |$$ |  $$ |$$ |      $$$$$$$$ |  $$ |   $$ |  $$ |$$$$$$$  |
       $$ |   $$ |  $$ |$$ \$$$  $$ |$$  __$$ | \____$$\ $$ |  $$ |$$ |     $$ |  $$ |       \____$$\   $$ |  $$ \$$$  $$ |$$ |  $$ |$$ |      $$  __$$ |  $$ |   $$ |  $$ |$$  __$$< 
       $$ |   $$ |  $$ |$$ |\$  /$$ |$$ |  $$ |$$\   $$ |$$ |  $$ |$$ |     $$ |  $$ |      $$\   $$ |  $$ |  $$ |\$  /$$ |$$ |  $$ |$$ |      $$ |  $$ |  $$ |   $$ |  $$ |$$ |  $$ |
       $$ |    $$$$$$  |$$ | \_/ $$ |$$ |  $$ |\$$$$$$  |\$$$$$$  |$$$$$$$$\ $$$$$$  |      \$$$$$$  |$$$$$$\ $$ | \_/ $$ |\$$$$$$  |$$$$$$$$\ $$ |  $$ |  $$ |    $$$$$$  |$$ |  $$ |
       \__|    \______/ \__|     \__|\__|  \__| \______/  \______/ \________|\______/        \______/ \______|\__|     \__| \______/ \________|\__|  \__|  \__|    \______/ \__|  \__|
                                                                                                                                                                                  
                                                                                                                                                                                  
                                                                                                                                                                                  
    )";

    cout<<ascii_art_banner<<endl;
    string config_path = "../config/config.ini";
    SimConfig *config = loadConfig(config_path);
    Cache *cache = new Cache(config->num_sets, config->associativity, config->block_size, 
                            config->address_bits, config->mem_fetch_latency, config->wb_latency, config->vc_access_latency);
    CPU *cpu = new CPU(config, cache);
    string filename = config->filepath;
    cpu->loadProgram(filename);
    Logs *log = new Logs(cpu->getAluFU(), cpu->getMulFU(), cpu->getDivFU(), cpu->getLSQ(), cpu->getROB(), cpu->getRegisters(),
                        cpu->getRSI(), cpu->getMemoryMap(), cpu->getInstructionLogs());

    printConfig(filename, config, cache);
    
    cout<<" [ INFO ] Executing Program..."<<endl;

    auto start = chrono::high_resolution_clock::now();

    while(cpu->continueSimulation()){
        cpu->nextCycle();
        log->addCycleLogs(cpu->getCurrentCycle());
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    stats->wallclock_time_secs = duration.count();
    stats->total_cycles = cpu->getCurrentCycle();

    cout<<" [ DONE ] Simulation Completed Successfully."<<endl<<endl;

    log->printTable();
    cout<<endl;

    //dump stats in stats.txt file
    Statistics *statsdump = new Statistics();
    statsdump->dumpStats();

    printStatsSummary();

    printFileLocation();

    //calling gantt chart python script
    createInstructionStatusFile(log->getILogs());
    system("python3 ../results/gantt.py");

    return 0;
}