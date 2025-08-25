#include "cpu.cpp"

class Logs{
    private:
        FunctionalUnit *ALU_FU;
        FunctionalUnit *MUL_FU;
        FunctionalUnit *DIV_FU;
        LSQ *lsq;
        ROB *rob;
        int *registers;
        int *rsi;
        unordered_map<int,int> *memory_map;

    public:
        Logs(){}
};