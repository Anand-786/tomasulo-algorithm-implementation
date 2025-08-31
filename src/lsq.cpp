#include <iostream>
#include <vector>
#include <unordered_map>
#include <climits>
#include "agu.cpp"
#include "cache.cpp"
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
    int skip_cycle;
};

class LSQ{
    private:
        vector<LSQEntry*> lsq;
        int head;
        int tail;
        int count;
        AGU *agu;
        int size;
        bool isLoadPending;
        LSQEntry *pendingLoadEntry;
        int dataReadyCycle;
        unordered_map<int, int> memory_map;
        Cache *cache;
    public:
        LSQ(int sz, unordered_map<int, int> &mm, Cache *l1cache){
            cache = l1cache;
            size=sz;
            lsq.resize(size);
            for(int i=0;i<size;i++){
                lsq[i] = new LSQEntry();
                lsq[i]->skip_cycle = -1;
            }
            agu = new AGU();
            memory_map = mm;
            head=tail=count=0;
            isLoadPending = false;
            pendingLoadEntry = NULL;
            dataReadyCycle = 0;
        }

        int getLSQIndex(){
            return head;
        }

        int getSize(){
            return size;
        }

        int getCount(){
            return count;
        }

        LSQEntry* getLSQEntry(int index){
            return lsq[index];
        }

        void clearHeadOnCommit(){
            lsq[head]->valid=false;
            head=(head+1)%size;
            count--;
        }

        int executeEffectiveAddress(int current_cycle){
            int ptr=head;
            int glb_s_n = -1;
            bool isRawHazard = false;
            while(ptr!=tail){
                auto it=lsq[ptr];
                if(it->valid && !it->isRegValReady)
                    isRawHazard = true;
                if(it->valid && !it->addressCalculated && it->isRegValReady && (it->skip_cycle != current_cycle)){
                    agu->setImm(it->offset);
                    agu->setRegVal(it->regVal);
                    agu->calculateEffectiveAddress();
                    it->effectveAddress = agu->getEffectiveAddress();
                    it->addressCalculated = true;
                    glb_s_n = it->global_seq_num;
                    break;
                }
                ptr=(ptr+1)%size;
            }
            if(isRawHazard)
                stats->raw_hazard_stall_cycles += 1;
            return glb_s_n;
        }

        int readMemAccess(int current_cycle){
            if(isLoadPending){
                if(current_cycle == dataReadyCycle){
                    pendingLoadEntry->isDataLoaded = true;
                    pendingLoadEntry->canWriteToCDB = true;
                    isLoadPending = false;
                    dataReadyCycle = 0;
                    return pendingLoadEntry->global_seq_num;
                }
                return -1;
            }
            int ptr=head;
            while(ptr!=tail){
                auto it=lsq[ptr];
                if(!it->isLoad && !it->addressCalculated){
                    return -1;
                }
                if(it->isLoad && it->addressCalculated && !it->isDataLoaded){
                    int j=(ptr-1+size)%size;
                    int currentEA = it->effectveAddress;
                    while(j!=(head-1+size)%size && (lsq[j]->isLoad || lsq[j]->effectveAddress != currentEA))
                        j=(j-1+size)%size;
                    if(j==(head-1+size)%size){
                        //Can Load from Memory
                        it->loadedData = memory_map[currentEA];
                        stats->l1d_overall_accesses += 1;
                        stats->l1d_read_accesses += 1;
                        int penalty = cache->l1CacheSearchAndUpdate(current_cycle, currentEA, false);
                        if(penalty == 0){
                            //HIT
                            //update stats
                            stats->l1d_overall_hits += 1;
                            stats->l1d_read_hits += 1;

                            it->isDataLoaded = true;
                            it->canWriteToCDB = true;
                            return it->global_seq_num;
                        }
                        //MISS
                        //update stats
                        stats->l1d_overall_misses += 1;
                        stats->l1d_read_misses += 1;
                        stats->l1d_total_miss_penalty += penalty;

                        isLoadPending = true;
                        dataReadyCycle = current_cycle + penalty;
                        pendingLoadEntry = it;
                        return -1;
                    }
                    if(lsq[j]->isDataReady){
                        //Store to Load Forwarding
                        stats->store_to_load_forwards += 1;
                        it->loadedData = lsq[j]->dataToBeStored;
                        it->isDataLoaded = true;
                        it->canWriteToCDB = true;
                        return it->global_seq_num;
                    }
                }
                ptr=(ptr+1)%size;
            }
            return -1;
        }

        bool canBeWrittenToCDB(LSQEntry *temp){
            if(temp->isLoad)
                return temp->canWriteToCDB;
            if(temp->addressCalculated){
                if(temp->isDataReady)
                    return temp->canWriteToCDB = true;
                else    
                    stats->raw_hazard_stall_cycles += 1;
            }
            return false;
        }

        vector<int> candidateForCDBWrite(int current_cycle){
            int global_seq_num=-1;
            int ptr=head;
            while(ptr!=tail){
                auto it = lsq[ptr];
                if(!it->isWrittenToCDB && canBeWrittenToCDB(it) && (it->skip_cycle != current_cycle)){
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
            if(count>stats->lsq_peak_occupancy)
                stats->lsq_peak_occupancy = count;
            return alotted;
        }

        void markWrittenOnCDB(int index){
            lsq[index]->isWrittenToCDB = true;
        }
};