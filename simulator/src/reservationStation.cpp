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
        bool isReadyForExecution(){
            if(Qj==-1 && Qk==-1)
                return true;
            return false;
        }
};