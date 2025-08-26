#include "cpu.cpp"
#include <fstream>
#include <iomanip>

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
        map<int, instructionLog*> *instructionLogs;
        ofstream trace;

    public:
        Logs(FunctionalUnit *alu_fu, FunctionalUnit *mul_fu, FunctionalUnit *div_fu, LSQ *loadstorequeue, ROB *reorderbuffer, int *registersarray, 
            int *rsiarray,  unordered_map<int,int> *mem_map, map<int, instructionLog*> *iLogs){
            this->ALU_FU = alu_fu;
            this->MUL_FU = mul_fu;
            this->DIV_FU = div_fu;
            this->lsq = loadstorequeue;
            this->registers = registersarray;
            this->rsi = rsiarray;
            this->rob = reorderbuffer;
            this->memory_map = mem_map;
            this->instructionLogs = iLogs;
            trace.open("../results/trace.log");
            if(trace.is_open()) 
                cout<<"Trace file created.\n";
            else
                cout<<"Trace file creation failed!\n";
        }

        //print final cycle table
        void printTable(){
            cout<<"\nFinal Instruction Status Table : "<<endl<<endl;
            cout << left << setw(25) << "Instruction" << "| "
                    << setw(7) << "Issue" << "| "
                    << setw(11) << "ExecStart" << "| "
                    << setw(9) << "ReadMem" << "| "
                    << setw(10) << "WriteCDB" << "| "
                    << setw(8) << "Commit" << "\n";
                cout << setfill('-') << setw(80) << "" << setfill(' ') << "\n";
                for (auto const& [key, val] : *instructionLogs) {
                    cout << left << setw(25) << val->instruction << "| "
                        << setw(7) << (val->issueCycle == 0 ? "-" : to_string(val->issueCycle)) << "| "
                        << setw(11) << (val->executeStartCycle == 0 ? "-" : to_string(val->executeStartCycle)) << "| "
                        << setw(9) << (val->memAccessCycle == 0 ? "-" : to_string(val->memAccessCycle)) << "| "
                        << setw(10) << (val->writeCDBCycle == 0 ? "-" : to_string(val->writeCDBCycle)) << "| "
                        << setw(8) << (val->commitCycle == 0 ? "-" : to_string(val->commitCycle)) << "\n";
                }
            cout << endl;
        }

        //helper function for getting string opcode
        string opcodeString(int val){
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

        // Helper function to print the reservation station table for any FU
        void print_fu_reservations(ofstream& trace, const string& title, FunctionalUnit* fu) {
            trace<<title<<"\n";
            trace<<left
                <<setw(6)<<"Busy"<<"| "
                <<setw(8)<<"Op"<<"| "
                <<setw(8)<<"Qj"<<"| "
                <<setw(8)<<"Qk"<<"| "
                <<setw(8)<<"Vj"<<"| "
                <<setw(8)<<"Vk"<<"| "
                <<setw(6)<<"Dest"<<"\n";
            trace<<setfill('-')<<setw(65)<<""<<setfill(' ')<<"\n"; // Divider line

            for (const auto& rs : fu->getRS()) {
                if (rs->isBusy()) { // Only print valid entries
                    trace<<left
                        <<setw(6)<<"Yes"<<"| "
                        <<setw(8)<<opcodeString(rs->getOpcode())<< "| "
                        // Use ternary operator to print '-' for invalid values
                        <<setw(8)<<(rs->getQj() != -1 ? "ROB " + to_string(rs->getQj()) : "-")<<"| "
                        <<setw(8)<<(rs->getQk() != -1 ? "ROB " + to_string(rs->getQk()) : "-")<<"| "
                        <<setw(8)<<(rs->getQj() == -1 ? to_string(rs->getVj()) : "-")<<"| "
                        <<setw(8)<<(rs->getQk() == -1 ? to_string(rs->getVk()) : "-")<<"| "
                        <<setw(6)<<"ROB " + to_string(rs->getRobEntryNum())<<"\n";
                }
            }
            trace << "\n";
        }

        void addCycleLogs(int current_cycle){
            if(trace.is_open()){
                // --- Cycle Header ---
                int totalwidth = 200;
                string msg = " Cycle "+to_string(current_cycle)+" ";
                int remwidth = totalwidth - msg.length();
                int leftpad = remwidth/2;
                int rightpad = remwidth - leftpad;
                trace<<string(leftpad, '-')<<msg<<string(rightpad, '-')<<"\n\n";

                // --- Instruction Status ---
                trace << "Instruction Status\n";
                trace << left << setw(25) << "Instruction" << "| "
                    << setw(7) << "Issue" << "| "
                    << setw(11) << "ExecStart" << "| "
                    << setw(9) << "ReadMem" << "| "
                    << setw(10) << "WriteCDB" << "| "
                    << setw(8) << "Commit" << "\n";
                trace << setfill('-') << setw(80) << "" << setfill(' ') << "\n";
                for (auto const& [key, val] : *instructionLogs) {
                    trace << left << setw(25) << val->instruction << "| "
                        << setw(7) << (val->issueCycle == 0 ? "-" : to_string(val->issueCycle)) << "| "
                        << setw(11) << (val->executeStartCycle == 0 ? "-" : to_string(val->executeStartCycle)) << "| "
                        << setw(9) << (val->memAccessCycle == 0 ? "-" : to_string(val->memAccessCycle)) << "| "
                        << setw(10) << (val->writeCDBCycle == 0 ? "-" : to_string(val->writeCDBCycle)) << "| "
                        << setw(8) << (val->commitCycle == 0 ? "-" : to_string(val->commitCycle)) << "\n";
                }
                trace << "\n";

                // --- Reservation Stations ---
                print_fu_reservations(trace, "Integer-ALU Reservation Stations", ALU_FU);
                print_fu_reservations(trace, "Multiplier Reservation Stations", MUL_FU);
                print_fu_reservations(trace, "Divider Reservation Stations", DIV_FU);

                // --- Reorder Buffer (ROB) ---
                trace << "Reorder Buffer [ROB : (Head => Tail) OR (Oldest => Newest)]\n";
                trace << left
                    << setw(8) << "Entry" << "| "
                    << setw(8) << "Type" << "| "
                    << setw(10) << "Dest" << "| "
                    << setw(10) << "Value" << "| "
                    << setw(6) << "Done" << "\n";
                trace << setfill('-') << setw(50) << "" << setfill(' ') << "\n";
                int head = rob->getRobIndex();
                int size = rob->getTotalSize();
                for (int i = 0; i < rob->getCount(); i++) {
                    RobEntry* reply = rob->getROBEntry(head);
                    trace << left
                        << setw(8) << "ROB " + to_string(head) << "| "
                        << setw(8) << opcodeString(reply->opcode) << "| "
                        << setw(10) << (reply->opcode == 6 ? "Mem[":"R") + (reply->dest == -1 ?"-":to_string(reply->dest)) + (reply->opcode == 6 ? "]":"") << "| "
                        << setw(10) << (reply->isReady ? to_string(reply->result) : "-") << "| "
                        << setw(6) << (reply->isReady ? "Yes" : "No") << "\n";
                    
                    head = (head+1)%size;
                }
                trace << "\n";

                // --- Load/Store Queue (LSQ) ---
                trace << "Load/Store Queue [LSQ : (Head => Tail) OR (Oldest => Newest)]\n";
                trace << left
                    << setw(8) << "Entry" << "| "
                    << setw(8) << "Type" << "| "
                    << setw(6) << "EA" << "| "
                    << setw(10) << "Value" << "| "
                    << setw(10) << "Dest" << "| "
                    << setw(9) << "Offset" << "| "
                    << setw(18) << "Base Reg Value" << "| "
                    << setw(8) << "Ready" << "| "
                    << setw(12) << "CDB Write" << "\n";
                trace << setfill('-') << setw(105) << "" << setfill(' ') << "\n";
                head = lsq->getLSQIndex();
                size = lsq->getSize();
                for (int i = 0; i < lsq->getCount(); i++) {
                    LSQEntry *reply = lsq->getLSQEntry(head);
                    if (reply->valid) {
                        trace << left
                            <<setw(8)<<"LSQ " + to_string(head)<<"| "
                            <<setw(8)<<(reply->isLoad ? "LOAD" : "STORE" ) << "| "
                            <<setw(6)<<(reply->addressCalculated ? to_string(reply->effectveAddress) : "-")<<"| "
                            <<setw(10)<<(reply->isLoad ? (reply->isDataLoaded ? to_string(reply->loadedData):"-") : 
                                            (reply->isDataReady ? to_string(reply->dataToBeStored):"-"))<<"| "
                            <<setw(10)<<"ROB " + to_string(reply->rob_entry_num) <<"| "
                            <<setw(9)<< to_string(reply->offset) <<"| "
                            <<setw(18)<< (reply->isRegValReady ? to_string(reply->regVal):"-") <<"| "
                            <<setw(8)<<(reply->canWriteToCDB ? "YES":"NO")<<"| "
                            <<setw(12)<< (reply->isWrittenToCDB ? "DONE":"-") << "\n";
                    }
                    head = (head+1)%size;
                }
                trace << "\n";

                // --- Register Status ---
                trace << "Register File & Status\n";
                for(int i = 0; i < 32; ++i) trace << " R" << left << setw(6) << i << "|"; trace << "\n";
                trace << setfill('-') << setw(288) << "" << setfill(' ') << "\n";
                for(int i = 0; i < 32; ++i) trace << " " << left << setw(7) << (rsi[i] == -1 ? "Ready" : "ROB "+to_string(rsi[i])) << "|"; trace << "\n";
                for(int i = 0; i < 32; ++i) trace << " " << left << setw(7) << (rsi[i] == -1 ? to_string(registers[i]) : "-") << "|"; trace << "\n\n";

                // --- Memory ---
                trace << "Memory State\n";
                bool empty = true;
                for (auto const& [address, value] : *memory_map) {
                    empty = false;
                    trace << "M[" << address << "] : " << value << "\n";
                }
                trace<<(empty ? "-":"")<<"\n";
                trace<<endl<<endl<<endl;
            }
            else{
                cout<<"trace.log file unexpectedly closed!\n";
            }
        }
};