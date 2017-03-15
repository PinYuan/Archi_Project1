#ifndef ERRORDETECT_H_INCLUDED
#define ERRORDETECT_H_INCLUDED
#include <bits/stdc++.h>
using namespace std;

class ErrorDetect{
private:
    int mult;//mult(u)
    int mf;//mfhi mflo
    int cycle;
public:
    ErrorDetect();
    void setCycle(int cycle);
    int writeToRegister0(bitset<5> writeToReg);
    int numberOverflow(bitset<32> data1, bitset<32> data2, bitset<32> output);
    void overwriteHILORegister(bitset<6> func);
    int memoryAddressOverflow(bitset<6> opCode, bitset<32> address);
    int dataMisaligned(bitset<6> opCode, bitset<32> address);
};

#endif // ERRORDETECT_H_INCLUDED
