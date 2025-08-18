#include <iostream>
#include <vector>
using namespace std;

struct RobEntry{
    int opcode;
    int dest;
    int result;
    bool exception;
    int global_seq_num;
    bool isReady;
};

class ROB{
    private:
        vector<RobEntry*> rob;
        int head;
        int tail;
        int count;
        int size;
    public:
        ROB(int maxSize){
            size=maxSize;
            rob.resize(size);
            head=tail=0;
            count=0;
        }

        bool isFull(){
            return (count==size);
        }

        bool isEmpty(){
            return (count==0);
        }

        bool isReady(){
            return rob[head]->isReady;
        }

        void setIsReady(bool val){
            rob[head]->isReady = val;
        }

        bool isRegisterType(){
            int op=rob[head]->opcode;
            if(op<=4)
                return true;
            return false;
        }

        int getDest(){
            return rob[head]->dest;
        }

        int getResult(){
            return rob[head]->result;
        }

        void incrementHead(){
            head = (head+1)%size;
        }
};