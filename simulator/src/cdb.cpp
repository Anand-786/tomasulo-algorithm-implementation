#include <iostream>
using namespace std;

class CDB{
    private:
        int rob_entry_number;
        int result;
        int destinationAddress;
    
    public:
        int getROBEntryNum(){
            return rob_entry_number;
        }

        int getResult(){
            return result;
        }

        int setROBEntryNum(int val){
            rob_entry_number = val;
        }

        int setResult(int val){
            result = val;
        }

        int getDestinationAddress(){
            return destinationAddress;
        }

        void setDestinationAddress(int val){
            destinationAddress = val;
        }
};