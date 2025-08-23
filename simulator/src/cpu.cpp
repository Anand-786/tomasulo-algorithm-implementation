#include <bits/stdc++.h>
#include "instruction.cpp"
#include "rob.cpp"
#include "functionalUnit.cpp"
#include "lsq.cpp"
#include "cdb.cpp"

using namespace std;

class CPU{
    private:
        queue<Instruction*> instructionQueue;
        int totalInstructions;
        int registers[32];
        int rsi[32];
        ROB *rob;
        LSQ *lsq;
        CDB *cdb;
        int current_cycle;
        unordered_map<int, int> memory_map;
        unordered_map<int, vector<ReservationStation*>> waitingRS;
        unordered_map<int, vector<LSQEntry*>> waitingLS; 
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
            rob = new ROB(20);
            lsq = new LSQ(10, memory_map);
            cdb = new CDB();
            current_cycle=0;
            ALU_FU = new FunctionalUnit(4, latency[ADD]);
            MUL_FU = new FunctionalUnit(4, latency[MUL]);
            DIV_FU = new FunctionalUnit(4, latency[DIV]);
            ADDR_FU = new FunctionalUnit(0, latency[ADDI]);
            for(int i=0;i<32;i++)
                rsi[i]=-1;
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
                            instructionQueue.push(instr);
                        }
                        else{
                            src2=stoi(instruction[3].substr(1));
                            Instruction *instr = new Instruction(opcode,dest,src1,src2,-1,-1,global_seq_num);
                            instructionQueue.push(instr);
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
                            instructionQueue.push(instr);
                        }
                        else{
                            Instruction *instr = new Instruction(opcode,-1,first_reg,reg,imm,-1,global_seq_num);
                            instructionQueue.push(instr);
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
            queue<Instruction*> temp=instructionQueue;
            while(!temp.empty()){
                Instruction *instr = temp.front();
                temp.pop();
                int type=instr->getType(), dest=instr->getDestReg(), src1=instr->getSrc1Reg(), src2=instr->getSrc2Reg();
                int imm=instr->getImmVal(), addr=instr->getAddress(), global_seq_num=instr->getGlobal_Seq_Num();
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
                if(global_seq_num!=-1)
                    cout<<"SEQ num: "<<global_seq_num<<"  ";
                cout<<"\n\n";
            }
            cout<<endl;
        }

        bool continueSimulation(){
            return !(instructionQueue.empty() && rob->isEmpty());
        }

        void nextCycle(){
            current_cycle++;

            cout<<"Cycel no : "<<current_cycle<<endl;
            commit();
            writeCDB();
            memAccess();
            execute();
            issue();

            cout<<endl;
        }

        void commit(){
            if(!rob->isReady())
                return;
            
            if(rob->isRegisterWrite())
                registers[rob->getDest()] = rob->getResult();
            else
                memory_map[rob->getDest()] = rob->getResult();

            //free the lsq head
            if(rob->isLoadStoreInstr())
                lsq->clearHeadOnCommit();

            rob->setIsReady(false);
            rob->incrementHead();
        }

        void writeCDB(){
            //part 1 : arbitration
            int result_val,result_dest_rob_entry_num, result_memory_address=-1;
            int arbitration = totalInstructions;
            bool isCDBWritePosssible = false;
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
            pair<pair<int, int>, pair<int, int>> lsq_candidate = lsq->candidateForCDBWrite();
            if(lsq_candidate.first.first < arbitration){
                arbitration = lsq_candidate.first.first;
                result_val = lsq_candidate.second.first;
                result_dest_rob_entry_num = lsq_candidate.second.second;
                result_memory_address = lsq_candidate.first.second;
                isCDBWritePosssible =true;
            }

            //part 2 : clearing
            FunctionalUnit *clear = NULL;
            if(ALU_FU->isCompleted() && arbitration == ALU_FU->getGlobalSeqNum()){
                result_val = ALU_FU->getResult();
                result_dest_rob_entry_num = ALU_FU->getRobEntryNum();
                clear = ALU_FU;
                isCDBWritePosssible =true;
            }
            else if(MUL_FU->isCompleted() && arbitration == MUL_FU->getGlobalSeqNum()){
                result_val = ALU_FU->getResult();
                result_dest_rob_entry_num = ALU_FU->getRobEntryNum();
                clear = MUL_FU;
                isCDBWritePosssible =true;
            }
            else if(DIV_FU->isCompleted() && arbitration == DIV_FU->getGlobalSeqNum()){
                result_val = ALU_FU->getResult();
                result_dest_rob_entry_num = ALU_FU->getRobEntryNum();
                clear = DIV_FU;
                isCDBWritePosssible =true;
            }
            if(clear){
                clear->setOccupied(false);
                clear->markIncomplete();
            }
            if(!isCDBWritePosssible)
                return;

            //part 3 : load values on CDB
            cdb->setResult(result_val);
            cdb->setROBEntryNum(result_dest_rob_entry_num);
            cdb->setDestinationAddress(result_memory_address);

            //part 4 : updating all Waiting Reservation Stations
            if(waitingRS.find(cdb->getROBEntryNum()) != waitingRS.end()){
                for(auto it: waitingRS[cdb->getROBEntryNum()]){
                    if(it->getQj() == cdb->getROBEntryNum()){
                        it->setVj(cdb->getResult());
                        it->setQj(-1);
                    }
                    if(it->getQk() == cdb->getROBEntryNum()){
                        it->setVk(cdb->getResult());
                        it->setQk(-1);
                    }
                }
            }

            //part 5 : updating all Waiting Load and Stores
            if(waitingLS.find(cdb->getROBEntryNum()) != waitingLS.end()){
                for(auto it: waitingLS[cdb->getROBEntryNum()]){
                    if(it->reg_rob_entry == cdb->getROBEntryNum()){
                        it->regVal = cdb->getResult();
                        it->isRegValReady = true;
                    }
                    else{
                        it->dataToBeStored = cdb->getResult();
                        it->isDataReady = true;
                    }
                }
            }

            //part 6 : updating ROB
            rob->cdbWrite(cdb->getROBEntryNum(), cdb->getResult(), cdb->getDestinationAddress());
        }

        void memAccess(){
            lsq->readMemAccess();
        }

        void execute(){
            lsq->executeEffectiveAddress();
            ALU_FU->executeIfPossible();
            MUL_FU->executeIfPossible();
            DIV_FU->executeIfPossible();
        }

        void issue(){
            Instruction *instrToBeIssued = instructionQueue.front();
            //check 2 cond: 1. RS/LSQ free slot    2. ROB free slot
            bool rsAvailable=false,robAvailable=false, lsqavailable=false;
            robAvailable = !(rob->isFull());
            if(!robAvailable)
                return;
            if(instrToBeIssued->getType() <= DIV){
                switch (instrToBeIssued->getType()){
                    case ADD:
                    case SUB:
                    case ADDI:
                        rsAvailable = ALU_FU->freeRSAvailable();
                        break;
                    case MUL:
                        rsAvailable = MUL_FU->freeRSAvailable();
                        break;
                    case DIV:
                        rsAvailable = DIV_FU->freeRSAvailable();
                        break;
                }
                if(!rsAvailable)
                    return;
                
                //We can issue now
                //part 1: ROB entry
                int robSlot = rob->issueROBSlot(instrToBeIssued->getType(), instrToBeIssued->getDestReg(), instrToBeIssued->getGlobal_Seq_Num());

                //part 2: RS entry
                int op = instrToBeIssued->getType();
                int glb_seq_num = instrToBeIssued->getGlobal_Seq_Num();
                int Qj,Qk,Vj,Vk;
                bool isSrc1Waiting = false, isSrc2Waiting = false;
                int src1Rob, src2Rob; 
                if(rsi[instrToBeIssued->getSrc1Reg()]==-1){
                    Qj = -1;
                    Vj = registers[instrToBeIssued->getSrc1Reg()];
                }
                else{
                    Qj = rsi[instrToBeIssued->getSrc1Reg()];
                    Vj = 0;
                    isSrc1Waiting = true;
                    src1Rob = Qj;
                }

                if(rsi[instrToBeIssued->getSrc2Reg()]==-1){
                    Qk = -1;
                    Vk = registers[instrToBeIssued->getSrc2Reg()];
                }
                else{
                    Qk = rsi[instrToBeIssued->getSrc2Reg()];
                    Vk = 0;
                    isSrc2Waiting = true;
                    src2Rob = Qk;
                }

                //update RSI for dest register
                rsi[instrToBeIssued->getDestReg()] = robSlot;

                ReservationStation *alotted;
                switch (instrToBeIssued->getType()){
                    case ADD:
                    case SUB:
                    case ADDI:
                        alotted = ALU_FU->issueInRS(op, Qj, Qk, Vj, Vk, robSlot, glb_seq_num);
                        break;
                    case MUL:
                        alotted = MUL_FU->issueInRS(op, Qj, Qk, Vj, Vk, robSlot, glb_seq_num);
                        break;
                    case DIV:
                        alotted = DIV_FU->issueInRS(op, Qj, Qk, Vj, Vk, robSlot, glb_seq_num);
                        break;
                }
                if(isSrc1Waiting)
                    waitingRS[src1Rob].push_back(alotted);
                if(isSrc2Waiting && (src2Rob != src1Rob))
                    waitingRS[src2Rob].push_back(alotted);

                instructionQueue.pop();
                return;
            }
            else{
                lsqavailable = !(lsq->isFull());
                if(!lsqavailable)
                    return;
                
                //We can issue now
                //part 1: get rob slot
                int robSlot = rob->issueROBSlot(instrToBeIssued->getType(), instrToBeIssued->getDestReg(), instrToBeIssued->getGlobal_Seq_Num());

                //part 2: LSQ entry
                int op = instrToBeIssued->getType();
                int glb_seq_num = instrToBeIssued->getGlobal_Seq_Num();
                int off = instrToBeIssued->getImmVal();
                int reg_val,reg_rob, dataStore, data_rob;
                bool is_reg_ready, is_data_ready;
                if(op == LOAD){
                    if(rsi[instrToBeIssued->getSrc1Reg()] == -1){
                        reg_val = registers[instrToBeIssued->getSrc1Reg()];
                        reg_rob = -1;
                        is_reg_ready = true;
                    }
                    else{
                        reg_val = 0;
                        reg_rob = rsi[instrToBeIssued->getSrc1Reg()];
                        is_reg_ready = false;
                    }

                    //update rsi for dest register
                    rsi[instrToBeIssued->getDestReg()] = robSlot;
                }
                else{
                    if(rsi[instrToBeIssued->getSrc2Reg()] == -1){
                        reg_val = registers[instrToBeIssued->getSrc2Reg()];
                        reg_rob = -1;
                        is_reg_ready = true;
                    }
                    else{
                        reg_val = 0;
                        reg_rob = rsi[instrToBeIssued->getSrc2Reg()];
                        is_reg_ready = false;
                    }

                    if(rsi[instrToBeIssued->getSrc1Reg()] == -1){
                        dataStore = registers[instrToBeIssued->getSrc1Reg()];
                        data_rob = -1;
                        is_data_ready = true;
                    }
                    else{
                        dataStore = 0;
                        data_rob = rsi[instrToBeIssued->getSrc1Reg()];
                        is_data_ready = false;
                    }
                }

                LSQEntry *alotted = lsq->issueInLSQ((op==LOAD)?true:false, glb_seq_num, robSlot, off, reg_val, reg_rob, is_reg_ready, dataStore, data_rob, is_data_ready);

                if(!is_reg_ready)
                    waitingLS[reg_rob].push_back(alotted);
                if((op == STORE) && !is_data_ready)
                    waitingLS[data_rob].push_back(alotted);

                instructionQueue.pop();
                return;
            } 
        }
};