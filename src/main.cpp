#include "logs.cpp"
#include <iostream>
#include <cstdlib>
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

int main(){
    CPU *cpu = new CPU();
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