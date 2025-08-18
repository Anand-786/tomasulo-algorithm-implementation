#include <iostream>
#include <vector>
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
        AGU *agu;
        int size;
    public:
        LSQ(int sz){
            size=sz;
            lsq.resize(size);
            for(int i=0;i<size;i++)
                lsq[i] = new LSQEntry();
            agu = new AGU();
        }

        void executeEffectiveAddress(){
            for(auto it: lsq){
                if(it->isLoad && it->isRegValReady){
                    agu->setImm(it->offset);
                    agu->setRegVal(it->regVal);
                    agu->calculateEffectiveAddress();
                    it->effectveAddress = agu->getEffectiveAddress();
                    it->addressCalculated = true;
                    break;
                }
            }
        }
};