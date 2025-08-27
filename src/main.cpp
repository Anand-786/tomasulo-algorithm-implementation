#include "logs.cpp"
#include <iostream>
#include <cstdlib>
using namespace std;

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
    
    //calling gantt chart python script
    system("python3 ../results/gantt.py");

    return 0;
}