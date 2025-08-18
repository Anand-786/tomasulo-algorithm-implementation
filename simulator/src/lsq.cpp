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
};