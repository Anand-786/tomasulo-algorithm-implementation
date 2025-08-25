#include <iostream>
using namespace std;

class AGU{
    private:
        int imm;
        int reg_val;
        int effectiveAddress;

    public:
        void setImm(int val){
            imm = val;
        }
        void setRegVal(int val){
            reg_val = val;
        }
        void calculateEffectiveAddress(){
            effectiveAddress = imm + reg_val;
        }
        int getEffectiveAddress(){
            return effectiveAddress; 
        }
};