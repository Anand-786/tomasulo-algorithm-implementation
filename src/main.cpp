#include "logs.cpp"
#include <iostream>
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

    cout<<"Simulation Completed.\n";
    cout<<"Cycles Consumed : "<<cpu->getCurrentCycle()<<"\n";
    cout<<"To see Cycle-by-Cycle Logs check : results/trace.log\n";
}