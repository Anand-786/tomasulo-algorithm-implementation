#include "logs.cpp"
#include <iostream>
#include <cstdlib>
#include "INIReader.h"
using namespace std;

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

    return config;
}

int main(){
    string config_path = "../config/config.ini";
    SimConfig *config = loadConfig(config_path);
    CPU *cpu = new CPU(config);
    string filename="program.txt";
    cpu->loadProgram(filename);
    Logs *log = new Logs(cpu->getAluFU(), cpu->getMulFU(), cpu->getDivFU(), cpu->getLSQ(), cpu->getROB(), cpu->getRegisters(),
                        cpu->getRSI(), cpu->getMemoryMap(), cpu->getInstructionLogs());

    while(cpu->continueSimulation()){
        cpu->nextCycle();
        log->addCycleLogs(cpu->getCurrentCycle());
    }

    cout<<"\nSimulation Completed.\n";
    log->printTable();

    cout<<"\nCycles Consumed : "<<cpu->getCurrentCycle()<<" clock cycles\n\n";
    cout<<"To see Cycle-by-Cycle Logs check : results/trace.log\n\n";
    createInstructionStatusFile(log->getILogs());
    //calling gantt chart python script
    system("python3 ../results/gantt.py");

    return 0;
}