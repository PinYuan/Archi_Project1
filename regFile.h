#ifndef REGFILE_H_INCLUDED
#define REGFILE_H_INCLUDED
#include <bits/stdc++.h>
using namespace std;

class RegisterFile{
private:
    bitset<32> registers[32];
    bitset<32> oldRegisters[32];
    bitset<32> oldPc, oldHI, oldLO;
public:
    bitset<32> readData1, readData2;
    
	RegisterFile();

    void readWrite(bitset<5> readRegister1, bitset<5> readRegister2, bitset<5> writeRegister
                   , bitset<32> writeData, bitset<1> regWrite);

    void outputRegister(int cycle);
};

#endif // REGFILE_H_INCLUDED

