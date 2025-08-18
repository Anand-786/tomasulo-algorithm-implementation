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
};