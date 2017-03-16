#ifndef INSTRUCTION_H_INCLUDED
#define INSTRUCTION_H_INCLUDED
#include <bits/stdc++.h>
using namespace std;

class Instruction{
public:
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

    Instruction(bitset<32> inst);
};

#endif // INSTRUCTION_H_INCLUDED

