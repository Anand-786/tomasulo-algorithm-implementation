#include <iostream>
using namespace std;

class ReservationStation{
    private:
        bool busy;
        int opcode;
        int Vj,Vk;
        int Qj,Qk;
        int robEntryNum;
        int global_seq_num;

    public:
        ReservationStation(){
            busy=false;
            opcode=-1;
            robEntryNum=-1;
            global_seq_num=-1;
        }

        bool isBusy(){
            return busy;
        }
        void setBusy(bool val){
            busy = val;
        }
        int getOpcode(){
            return opcode;
        }
        void setOpcode(int val){
            opcode = val;
        }
        int getVj(){ 
            return Vj;
        }
        int getQj(){
            return Qj;
        }
        int getVk(){
            return Vk;
        }
        int getQk(){
            return Qk;
        }
        void setVj(int val){
            Vj = val;
        }
        void setQj(int val){
            Qj = val;
        }
        void setVk(int val){
            Vk = val;
        }
        void setQk(int val){
            Qk = val;
        }
        void setROBEntryNum(int val){
            robEntryNum = val;
        }
        void setGlobalSeqNum(int val){
            global_seq_num = val;
        }
        int getGlobalSeqNum(){
            return global_seq_num;
        }
        int getRobEntryNum(){
            return robEntryNum;
        }
        bool isReadyForExecution(){
            if(Qj==-1 && Qk==-1)
                return true;
            return false;
        }
};