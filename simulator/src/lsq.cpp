#include <iostream>
#include <vector>
#include <unordered_map>
#include "agu.cpp"
using namespace std;

struct LSQEntry{
    bool valid;
    bool isLoad;
    int global_seq_num;
    int rob_entry_num;
    int effectveAddress;
    bool addressCalculated;
    int offset;
    int regVal;
    int reg_rob_entry;
    bool isRegValReady;
    int loadedData;
    bool isDataLoaded;
    int dataToBeStored;
    int data_rob_entry;
    bool isDataReady;
    bool canWriteToCDB;
    bool isWrittenToCDB;
};

class LSQ{
    private:
        vector<LSQEntry*> lsq;
        int head;
        int tail;
        int count;
        AGU *agu;
        int size;
        unordered_map<int, int> memory_map;
    public:
        LSQ(int sz, unordered_map<int, int> &mm){
            size=sz;
            lsq.resize(size);
            for(int i=0;i<size;i++)
                lsq[i] = new LSQEntry();
            agu = new AGU();
            memory_map = mm;
            head=tail=count=0;
        }

        void clearHeadOnCommit(){
            lsq[head]->valid=false;
            head=(head+1)%size;
        }

        void executeEffectiveAddress(){
            int ptr=head;
            while(ptr!=tail){
                auto it=lsq[ptr];
                if(it->valid && !it->addressCalculated && it->isRegValReady){
                    agu->setImm(it->offset);
                    agu->setRegVal(it->regVal);
                    agu->calculateEffectiveAddress();
                    it->effectveAddress = agu->getEffectiveAddress();
                    it->addressCalculated = true;
                    break;
                }
                ptr=(ptr+1)%size;
            }
        }

        void readMemAccess(){
            int ptr=head;
            while(ptr!=tail){
                auto it=lsq[ptr];
                if(!it->isLoad && !it->addressCalculated){
                    return;
                }
                if(it->isLoad && it->addressCalculated){
                    int j=(ptr-1+size)%size;
                    int currentEA = it->effectveAddress;
                    while(j!=(head-1+size)%size && (lsq[j]->isLoad || lsq[j]->effectveAddress != currentEA))
                        j=(j-1+size)%size;
                    if(j==(head-1+size)%size){
                        it->loadedData = memory_map[currentEA];
                        it->isDataLoaded = true;
                        it->canWriteToCDB = true;
                        return;
                    }
                    if(lsq[j]->isDataReady){
                        it->loadedData = lsq[j]->dataToBeStored;
                        it->isDataLoaded = true;
                        it->canWriteToCDB = true;
                        return;
                    }
                }
                ptr=(ptr+1)%size;
            }
        }

        bool canBeWrittenToCDB(LSQEntry *temp){
            if(temp->isLoad)
                return temp->canWriteToCDB;
            if(temp->addressCalculated && temp->isDataReady)
                return temp->canWriteToCDB = true;
            return false;
        }

        vector<int> candidateForCDBWrite(){
            int global_seq_num=-1;
            int ptr=head;
            while(ptr!=tail){
                auto it = lsq[ptr];
                if(!it->isWrittenToCDB && canBeWrittenToCDB(it)){
                    if(it->isLoad)
                        return {it->global_seq_num, -1, it->loadedData, it->rob_entry_num, ptr};
                    else{
                        return {it->global_seq_num, it->effectveAddress, it->dataToBeStored, it->rob_entry_num, ptr};
                    }
                }
                ptr=(ptr+1)%size;
            }
            return {INT_MAX, -1, -1, -1, -1};
        }

        bool isFull(){
            return (count==size);
        }

        LSQEntry* issueInLSQ(bool isL, int glb_seq_num, int rob_entry, int off, int reg_val, int reg_rob,
                        bool is_reg_ready, int dataStore, int data_rob, bool is_data_ready){
            lsq[tail]->valid = true;
            lsq[tail]->isLoad = isL;
            lsq[tail]->global_seq_num = glb_seq_num;
            lsq[tail]->rob_entry_num = rob_entry;
            lsq[tail]->addressCalculated = false;
            lsq[tail]->offset = off;
            lsq[tail]->reg_rob_entry = reg_rob;
            lsq[tail]->regVal = reg_val;
            lsq[tail]->isRegValReady = is_reg_ready;
            lsq[tail]->isDataLoaded = false;
            lsq[tail]->dataToBeStored = dataStore;
            lsq[tail]->data_rob_entry = data_rob;
            lsq[tail]->isDataReady = is_data_ready;
            lsq[tail]->canWriteToCDB = false;
            lsq[tail]->isWrittenToCDB = false;

            LSQEntry *alotted = lsq[tail];
            tail = (tail+1)%size;
            count++;
            return alotted;
        }

        void markWrittenOnCDB(int index){
            lsq[index]->isWrittenToCDB = true;
        }
};