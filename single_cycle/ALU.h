#ifndef ALU_H_INCLUDED
#define ALU_H_INCLUDED
#include <bits/stdc++.h>
using namespace std;

class ALU{
public:
    bitset<32> ALUResult;
    bitset<1> zero;

    bitset<32> ALUoperater(bitset<32> data1, bitset<32> data2, bitset<4> ALUControl);

    bitset<4> ALUcontrol(bitset<2> ALUop, bitset<6> func);
};

#endif // ALU_H_INCLUDED

