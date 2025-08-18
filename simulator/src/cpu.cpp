#include <bits/stdc++.h>
#include "instruction.cpp"
#include "rob.cpp"
#include "functionalUnit.cpp"

using namespace std;

class CPU{
    private:
        deque<Instruction*> instructionQueue;
        int totalInstructions;
        int registers[32];
        ROB *rob;
        int current_cycle;
        unordered_map<int, int> memory_map;
        FunctionalUnit *ALU_FU;
        FunctionalUnit *MUL_FU;
        FunctionalUnit *DIV_FU;
        FunctionalUnit *ADDR_FU;


        vector<string> parseInstruction(string &line){
            string opcode;
            istringstream iss(line);
            iss>>opcode;
            string rest;
            getline(iss, rest);
            string token;
            istringstream ss(rest);
            vector<string> instruction;
            instruction.push_back(opcode);
            while (getline(ss, token, ',')) {
                token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());
                if (!token.empty()) 
                    instruction.push_back(token);
            }
            return instruction;
        }

    public:
        CPU(){
            rob=new ROB(20);
            current_cycle=0;
            ALU_FU = new FunctionalUnit(4, latency[ADD]);
            MUL_FU = new FunctionalUnit(4, latency[MUL]);
            DIV_FU = new FunctionalUnit(4, latency[DIV]);
            ADDR_FU = new FunctionalUnit(0, latency[ADDI]);
        }
        int totalInstructions(){
            return totalInstructions;
        }

        void loadProgram(string &filename){
            ifstream file(filename);
            if (!file.is_open()) {
                cerr<<"Error: Could not open file: "<<filename<<endl;
                return;
            }
            string line;
            int global_seq_num=0;
            while (getline(file, line)) {
                if (!line.empty() && line[0] != '#'){
                    global_seq_num++;
                    vector<string> instruction = parseInstruction(line);
                    if(instruction.size()>3){
                        int opcode;
                        string op=instruction[0];
                        if(op=="ADD")
                            opcode=ADD;
                        else if(op=="SUB")
                            opcode=SUB;
                        else if(op=="MUL")
                            opcode=MUL;
                        else if(op=="DIV")
                            opcode=DIV;
                        else if(op=="ADDI")
                            opcode=ADDI;
                        else{
                            cerr<<"Instruction type not defined :"<<op<<endl;
                            return;
                        }
                        int dest=stoi(instruction[1].substr(1)),src1=stoi(instruction[2].substr(1)),src2;
                        if(opcode==ADDI){
                            src2=stoi(instruction[3]);
                            Instruction *instr = new Instruction(opcode,dest,src1,-1,src2,-1,global_seq_num);
                            instructionQueue.push_back(instr);
                        }
                        else{
                            src2=stoi(instruction[3].substr(1));
                            Instruction *instr = new Instruction(opcode,dest,src1,src2,-1,-1,global_seq_num);
                            instructionQueue.push_back(instr);
                        }
                    }
                    else{
                        int opcode;
                        string op=instruction[0];
                        if(op=="LOAD")
                            opcode=LOAD;
                        else if(op=="STORE")
                            opcode=STORE;
                        else{
                            cerr<<"Instruction Type not supoorted :"<<op<<endl;
                            return;
                        }
                        int first_reg=stoi(instruction[1].substr(1));
                        int imm,reg,j=0;
                        string im,r;
                        while(instruction[2][j]!='(')
                            im.push_back(instruction[2][j++]);
                        j+=2;
                        while(j!=')')
                            r.push_back(instruction[2][j++]);
                        imm=stoi(im);
                        reg=stoi(r);
                        if(opcode==LOAD){
                            Instruction *instr = new Instruction(opcode,first_reg,reg,-1,imm,-1,global_seq_num);
                            instructionQueue.push_back(instr);
                        }
                        else{
                            Instruction *instr = new Instruction(opcode,-1,first_reg,reg,imm,-1,global_seq_num);
                            instructionQueue.push_back(instr);
                        }
                    }
                }
            }
            file.close();
            totalInstructions = instructionQueue.size();
            cout<<"\nLoaded Instruction Queue from : "<<filename<<"\nTotal Instructions = "<<totalInstructions<<endl;
        }

        void printProgram(){
            cout<<"\nGiven Assembly Program :\n"<<endl;
            deque<Instruction*> temp=instructionQueue;
            while(!temp.empty()){
                Instruction *instr = temp.front();
                temp.pop_front();
                int type=instr->getType(), dest=instr->getDestReg(), src1=instr->getSrc1Reg(), src2=instr->getSrc2Reg();
                int imm=instr->getImmVal(), addr=instr->getAddress();
                cout<<"Op: "<<instr->opcode_for_printing(type)<<"   ";
                if(dest!=-1)
                    cout<<"Dest: R"<<dest<<"   ";
                if(src1!=-1)
                    cout<<"Src1: R"<<src1<<"   ";
                if(src2!=-1)
                    cout<<"Src2: R"<<src2<<"   ";
                if(imm!=-1)
                    cout<<"Imm: "<<imm<<"  ";
                if(addr!=-1)
                    cout<<"Address: "<<addr<<"  ";
                cout<<"\n\n";
            }
            cout<<endl;
        }

        bool continueSimulation(){
            return !(instructionQueue.empty() && rob->isEmpty());
        }

        void nextCycle(){
            current_cycle++;

            commit();
            writeCDB();
        }

        void commit(){
            if(!rob->isReady())
                return;
            
            if(rob->isRegisterType())
                registers[rob->getDest()] = rob->getResult();
            else
                memory_map[rob->getDest()] = rob->getResult();

            rob->setIsReady(false);
            rob->incrementHead();
        }

        void writeCDB(){
            int result_val,result_dest_rob_entry_num;
            int arbitration = totalInstructions;

            if(ALU_FU->isCompleted()){
                int temp = ALU_FU->getGlobalSeqNum();
                if(temp < arbitration)
                    arbitration = temp;
            }
            if(MUL_FU->isCompleted()){
                int temp = MUL_FU->getGlobalSeqNum();
                if(temp < arbitration)
                    arbitration = temp;
            }
            if(DIV_FU->isCompleted()){
                int temp = DIV_FU->getGlobalSeqNum();
                if(temp < arbitration)
                    arbitration = temp;
            }
            FunctionalUnit *clear;
            if(arbitration == ALU_FU->getGlobalSeqNum())
                clear = ALU_FU;
            else if(arbitration == MUL_FU->getGlobalSeqNum())
                clear = MUL_FU;
            else if(arbitration == DIV_FU->getGlobalSeqNum())
                clear = DIV_FU;
            else{
                return;
            }

            clear->setOccupied(false);
            clear->markIncomplete();
        }
};