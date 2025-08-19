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
    int regVal_rob_entry;
    bool isRegValReady;
    int loadedData;
    bool isDataLoaded;
    int dataToBeStored;
    bool isDataReady;
    bool canWriteToCDB;
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

        void executeEffectiveAddress(){
            int ptr=head;
            while(ptr!=tail){
                auto it=lsq[ptr];
                if(it->isLoad && it->isRegValReady){
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

        int candidateForCDBWrite(){
            int global_seq_num=-1;
            for(auto it: lsq){
                if(it->canWriteToCDB){
                    return it->global_seq_num;
                }
            }
        }
};