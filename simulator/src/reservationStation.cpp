#include <iostream>
using namespace std;

class ReservationStation{
    private:
        bool busy;
        int opcode;
        int Vj,Vk;
        int Qj,Qk;
        int robEntryNum;

    public:
        ReservationStation(){
            busy=false;
            opcode=-1;
            robEntryNum=-1;
        }
};