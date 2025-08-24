#include "cpu.cpp"
#include <iostream>
using namespace std;

int main(){
    cout<<"Simulator Starting...\n"<<endl;
    cout<<"Loading Program..."<<endl;

    CPU *cpu = new CPU();
    string filename="program.txt";
    cpu->loadProgram(filename);
    cpu->printProgram();

    cout<<"Simultaion Starts...\n\n";

    while(cpu->continueSimulation()){
        cpu->nextCycle();
    }

    cout<<"Simulation Ended."<<endl;
}