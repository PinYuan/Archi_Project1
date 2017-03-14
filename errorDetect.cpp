#include <bits/stdc++.h>
#include "ErrorDetect.h"
using namespace std;

ErrorDetect::ErrorDetect(){
    mult = mf = 0;
    FILE* fptr;
    fopen("error_dump.rpt", "w");
    fclose(fptr);
}
void ErrorDetect::writeToRegister0(bitset<5> writeToReg, int cycle){
    if(writeToReg == 0){
        FILE* fptr;
        fopen("error_dump.rpt", "a");
        fprintf(fptr , "In cycle %d: Write $0 Error\n", cycle);
        fclose(fptr);
    }
}
void ErrorDetect::numberOverflow(bitset<32> data1, bitset<32> data2, bitset<32> output, int cycle){
    if(data1[31]==data2[31] && data1[31] != output[31]){
        FILE* fptr;
        fopen("error_dump.rpt", "a");
        fprintf(fptr , "In cycle %d: Number Overflow\n", cycle);
        fclose(fptr);
    }
}
void ErrorDetect::overwriteHILORegister(bitset<6> func, int cycle){//every time call mult(u) and mfhi(lo)
    if(func == 24 || func == 25){//mult(u)
        mult++;
        if(mf != mult-1){
            FILE* fptr;
            fopen("error_dump.rpt", "a");
            fprintf(fptr , "In cycle %d: Overwrite HI-LO registers\n", cycle);
            fclose(fptr);
        }
    }
    else if(func == 16 || func ==18){
        mf++;
    }
}
int ErrorDetect::memoryAddressOverflow(bitset<32> address, int cycle){
    int halt = 0;
    if(address.to_ulong() > 1023){
        halt = 1;
        FILE* fptr;
        fopen("error_dump.rpt", "a");
        fprintf(fptr , "In cycle %d: Address Overflow\n", cycle);
        fclose(fptr);
    }
    return halt;
}
int ErrorDetect::dataMisaligned(bitset<6> opCode, bitset<32> address, int cycle){
    int halt = 0;
    if(opCode == 35 || opCode == 43){//word lw sw
        if(address.to_ulong()%4 != 0)
            halt = 1;
    }
    else if(opCode == 33 || opCode == 37 || opCode == 41){//half lh lhu sh
        if(address.to_ulong()%2 != 0)
            halt = 1;
    }
    if(halt){
        FILE* fptr;
        fopen("error_dump.rpt", "a");
        fprintf(fptr , "In cycle %d: Misalignment Error\n", cycle);
        fclose(fptr);
    }
    return halt;
}

