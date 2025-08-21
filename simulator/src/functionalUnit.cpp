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

        bool markIfCompleted(){
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

        void executeIfPossible(){
            if(occupied){
                advanceCycle();
                markIfCompleted();
                return;
            }
            //assign a new instruction to execute
            ReservationStation *temp = NULL;
            int temp_index=-1,index=0;
            int min_glb_seq_num=INT_MAX;
            for(auto it: reservationStations){
                if(it->isBusy() && it->isReadyForExecution() && (it->getGlobalSeqNum()<min_glb_seq_num)){
                    min_glb_seq_num = it->getGlobalSeqNum();
                    temp = it;
                    temp_index = index;
                }
                index++;
            }
            if(!temp)
                return;
            
            setOccupied(true);
            markIncomplete();
            cyclesNeeded = latency-1;
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
            temp->setBusy(false);
            freeRS.insert(temp_index);
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