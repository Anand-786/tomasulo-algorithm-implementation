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

    public:
        FunctionalUnit(int num_rs, int maxLatency){
            reservationStations.resize(num_rs);
            for(int i=0;i<num_rs;i++)
                reservationStations[i]=new ReservationStation();
            occupied = false;
            latency = maxLatency;
            completed = false;
            cyclesNeeded = -1;
        }
};