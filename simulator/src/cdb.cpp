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

        void setROBEntryNum(int val){
            rob_entry_number = val;
        }

        void setResult(int val){
            result = val;
        }

        int getDestinationAddress(){
            return destinationAddress;
        }

        void setDestinationAddress(int val){
            destinationAddress = val;
        }
};