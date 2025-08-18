#include <bits/stdc++.h>
#include "instruction.cpp"
#include "rob.cpp"

using namespace std;

class CPU{
    private:
        deque<Instruction*> instructionQueue;
        int registers[32];
        ROB *rob;
        int cycle_number;
        unordered_map<int, int> memory_map;

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
            cycle_number=0;
        }
        int totalInstructions(){
            return instructionQueue.size();
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
            cout<<"\nLoaded Instruction Queue from : "<<filename<<"\nTotal Instructions = "<<totalInstructions()<<endl;
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
            cycle_number++;

            commit();
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
};