#include <bits/stdc++.h>
using namespace std;

//Opcode assignment with constants
const int ADD=0;
const int SUB=1;
const int ADDI=2;
const int MUL=3;
const int DIV=4;
const int LOAD=5;
const int STORE=6;

//Latency array matching with the opcode index
const int latency[]={1, 1, 1, 5, 10, 2, 2};

class Instruction{
    private:
        int type;
        int dest_reg;
        int src1_reg;
        int src2_reg;
        int imm_val;
        int address;

    public:
        //Constructor with Initialisation list
        Instruction(int itype,int dest,int src1,int src2,int imm=-1,int addr=-1): 
        type(itype), dest_reg(dest), src1_reg(src1), src2_reg(src2), imm_val(imm), address(addr)
        {}

        //All Getters
        int getType() const { return type; }
        int getDestReg() const { return dest_reg; }
        int getSrc1Reg() const { return src1_reg; }
        int getSrc2Reg() const { return src2_reg; }
        int getImmVal() const { return imm_val; }
        int getAddress() const { return address; }

        //All Setters
        void setType(int val) { type = val; }
        void setDestReg(int val) { dest_reg = val; }
        void setSrc1Reg(int val) { src1_reg = val; }
        void setSrc2Reg(int val) { src2_reg = val; }
        void setImmVal(int val) { imm_val = val; }
        void setAddress(int val) { address = val; }

        int getLatency(int t){
            return latency[t];
        }

        string opcode_for_printing(int val){
            switch (val){
                case ADD:
                    return "ADD";
                case SUB:
                    return "SUB";
                case ADDI:
                    return "ADDI";
                case MUL:
                    return "MUL";
                case DIV:
                    return "DIV";
                case LOAD:
                    return "LOAD";
                case STORE:
                    return "STORE";
                default:
                    return ""; 
            }
        }
};