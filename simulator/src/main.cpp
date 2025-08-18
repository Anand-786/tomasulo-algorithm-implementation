#include "cpu.cpp"
#include <iostream>
using namespace std;

int main(){
    cout<<"Simulator Starting..."<<endl;
    cout<<"Loading Program..."<<endl;

    CPU *cpu = new CPU();
    string filename="program.txt";
    cpu->loadProgram(filename);
    cpu->printProgram();
}