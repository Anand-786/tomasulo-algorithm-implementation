#include <iostream>
#include <vector>
#include "reservationStation.cpp"

using namespace std;

class FunctionalUnit{
    private:
        vector<ReservationStation*> reservationStations;
        unordered_set<int> freeRS;
        bool occupied;
        int latency;
        bool completed;
        int cyclesNeeded;
        int result;
        int rob_entry_num;
        int global_seq_num;

    public:
        FunctionalUnit(int num_rs, int maxLatency){
            reservationStations.resize(num_rs);
            for(int i=0;i<num_rs;i++)
                reservationStations[i]=new ReservationStation();
            for(int i=0;i<num_rs;i++)
                freeRS.insert(i);
            occupied = false;
            latency = maxLatency;
            completed = false;
            cyclesNeeded = result = rob_entry_num = global_seq_num = -1;
        }

        int getResult(){
            return result;
        }

        int getRobEntryNum(){
            return rob_entry_num;
        }

        void advanceCycle(){
            cyclesNeeded--;
        }

        bool isCompleted(){
            return completed;
        }

        void markIfCompleted(){
            if(!cyclesNeeded)
                completed = true;
        }

        int getGlobalSeqNum(){
            return global_seq_num;
        }

        void setGlobalSeqNum(int val){
            global_seq_num = val;
        }

        void setOccupied(bool val){
            occupied = val;
        }

        void markIncomplete(){
            completed = false;
        }

        int getOccupiedSlotCount(){
            return ((reservationStations.size()) - (freeRS.size()));
        }

        bool getOccupied(){
            return occupied;
        }

        vector<ReservationStation*> getRS(){
            return reservationStations;
        }

        int executeIfPossible(int current_cycle){
            if(occupied){
                advanceCycle();
                markIfCompleted();
                return -1;
            }
            //assign a new instruction to execute
            ReservationStation *temp = NULL;
            int temp_index=-1,index=0;
            int min_glb_seq_num=INT_MAX;
            bool isRawHazard = false;
            for(auto it: reservationStations){
                if(it->isBusy() && it->isReadyForExecution() && (it->getGlobalSeqNum()<min_glb_seq_num) && (it->getSkipCycle()!=current_cycle)){
                    min_glb_seq_num = it->getGlobalSeqNum();
                    temp = it;
                    temp_index = index;
                }

                if(it->isBusy() && !it->isReadyForExecution())
                    isRawHazard = true;
                index++;
            }
            if(isRawHazard)
                stats->raw_hazard_stall_cycles += 1;
            if(!temp){
                return -1;
            }
            
            setOccupied(true);
            markIncomplete();
            cyclesNeeded = latency-1;
            markIfCompleted();
            switch (temp->getOpcode()){
                case ADD:
                    result = temp->getVj() + temp->getVk();
                    break;
                case SUB:
                    result = temp->getVj() - temp->getVk();
                    break;
                case ADDI:
                    result = temp->getVj() + temp->getVk();
                    break;
                case MUL:
                    result = temp->getVj() * temp->getVk();
                    break;
                case DIV:
                    result = temp->getVj() / temp->getVk();
                    break;
            }
            rob_entry_num = temp->getRobEntryNum();
            global_seq_num = temp->getGlobalSeqNum();

            //mark RS as empty
            reservationStations[temp_index]->setBusy(false);
            freeRS.insert(temp_index);
            return global_seq_num;
        }

        bool freeRSAvailable(){
            return !freeRS.empty();
        }

        ReservationStation* issueInRS(int op, int Qj, int Qk, int Vj, int Vk, int rob_entry, int glb_seq_num){
            int freeRSIndex = *freeRS.begin();
            freeRS.erase(freeRSIndex);
            reservationStations[freeRSIndex]->setBusy(true);
            reservationStations[freeRSIndex]->setOpcode(op);
            reservationStations[freeRSIndex]->setQj(Qj);
            reservationStations[freeRSIndex]->setQk(Qk);
            reservationStations[freeRSIndex]->setVj(Vj);
            reservationStations[freeRSIndex]->setVk(Vk);
            reservationStations[freeRSIndex]->setROBEntryNum(rob_entry);
            reservationStations[freeRSIndex]->setGlobalSeqNum(glb_seq_num);
            return reservationStations[freeRSIndex];
        }
};