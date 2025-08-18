#include <iostream>
#include <vector>
#include "reservationStation.cpp"

using namespace std;

class FunctionalUnit{
    private:
        vector<ReservationStation*> reservationStations;
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
            occupied = false;
            latency = maxLatency;
            completed = false;
            cyclesNeeded = result = rob_entry_num = global_seq_num = -1;
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
};