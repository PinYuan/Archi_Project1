#ifndef ERRORDETECT_H_INCLUDED
#define ERRORDETECT_H_INCLUDED
#include <bits/stdc++.h>
using namespace std;

class ErrorDetect{
private:
    int mult;//mult(u)
    int mf;//mfhi mflo
public:
    ErrorDetect();
    void writeToRegister0(bitset<5> writeToReg, int cycle);
    void numberOverflow(bitset<32> data1, bitset<32> data2, bitset<32> output, int cycle);
    void overwriteHILORegister(bitset<6> func, int cycle);
    int memoryAddressOverflow(bitset<32> address, int cycle);
    int dataMisaligned(bitset<6> opCode, bitset<32> address, int cycle);
};

#endif // ERRORDETECT_H_INCLUDED

