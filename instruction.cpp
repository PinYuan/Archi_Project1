#include <bits/stdc++.h>
using namespace std;

class Instruction{
private:
    bitset<32> completeInst;
    bitset<6> opCode;
    bitset<5> regRs;
    bitset<5> regRt;
    bitset<5> regRd;
    bitset<5> shamt;
    bitset<6> func;
    bitset<16> immediate;
    bitset<26> targetAddr;
    char type;
public:
    Instruction(bitset<32> inst){
        completeInst = inst;
        for(int i=0;i<6;i++)
            opCode.set(i, instruction[26+i]);
        for(int i=0;i<5;i++)
            regRs.set(i, instruction[21+i]);
        for(int i=0;i<5;i++)
            regRt.set(i, instruction[16+i]);
        for(int i=0;i<5;i++)
            regRd.set(i, instruction[11+i]);
        for(int i=0;i<5;i++)
            shamt.set(i, instruction[6+i]);
        for(int i=0;i<5;i++)
            func.set(i, instruction[i]);
        for(int i=0;i<16;i++)
            immediate.set(i, instruction[16+i]);
        for(int i=0;i<26;i++)
            targetAddr.set(i, instruction[i]);
        if(opCode == 63)
            type = 'S';
        else if(opCode == 2 || opCode == 3)
            type = 'J';
        else if(opCode == 0)
            type = 'R';
        else
            type = 'I';
    }
};

