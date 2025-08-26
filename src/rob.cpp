#include <iostream>
#include <vector>
using namespace std;

struct RobEntry{
    int opcode;
    int dest;
    int result;
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
            for(int i=0;i<size;i++){
                rob[i] = new RobEntry();
                rob[i]->isReady = false;
            }
            head=tail=0;
            count=0;
        }

        int getTotalSize(){
            return size;
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

        bool isRegisterWrite(){
            int op=rob[head]->opcode;
            if(op<=5)
                return true;
            return false;
        }

        int getDest(){
            return rob[head]->dest;
        }

        int getSize(){
            return count;
        }

        int getResult(){
            return rob[head]->result;
        }

        int getRobIndex(){
            return head;
        }

        bool isValueAfterBroadcastReady(int index){
            return rob[index]->isReady;
        }

        int getReadyResultAfterBroadcast(int index){
            return rob[index]->result;
        }

        void incrementHead(){
            head = (head+1)%size;
            count--;
        }

        bool isLoadStoreInstr(){
            int op=rob[head]->opcode;
            return (op>=5);
        }

        int getCount(){
            return count;
        }

        RobEntry* getROBEntry(int index){
            return rob[index];
        }

        void cdbWrite(int rob_entry_num, int res, int destinationAddress){
            rob[rob_entry_num]->result = res;
            rob[rob_entry_num]->isReady = true;
            if((rob[rob_entry_num]->opcode == STORE) && (destinationAddress != -1))
                rob[rob_entry_num]->dest = destinationAddress;
        }

        int issueROBSlot(int op, int destination, int glb_seq_num){
            rob[tail]->opcode = op;
            rob[tail]->dest = destination;
            rob[tail]->global_seq_num = glb_seq_num;
            rob[tail]->isReady = false;
            int slot = tail;
            tail=(tail+1)%size;
            count++;
            return slot;
        }
};