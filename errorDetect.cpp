#include <bits/stdc++.h>
#include "errorDetect.h"
using namespace std;

ErrorDetect::ErrorDetect(){
    cycle = 0;
    mult = mf = 0;
    FILE* fptr;
    fptr = fopen("error_dump.rpt", "w");
    fclose(fptr);
}
void ErrorDetect::setCycle(int cycle){
    this->cycle = cycle;
}
int ErrorDetect::writeToRegister0(bitset<5> writeToReg){
    if(writeToReg == 0){
        FILE* fptr;
        fptr = fopen("error_dump.rpt", "a");
        char buffer[35];fprintf(fptr, "In cycle %d: Write $0 Error\n", cycle);
        fclose(fptr);
        return 1;
    }
    else return 0;
}
int ErrorDetect::numberOverflow(bitset<32> data1, bitset<32> data2, bitset<32> output){
    if((data1[31] ^ data2[31] == 0) && (data1[31] != output[31])){
        FILE* fptr;
        fptr = fopen("error_dump.rpt", "a");
        fprintf(fptr , "In cycle %d: Number Overflow\n", cycle);
        fclose(fptr);
        return 1;
    }
    else return 0;
}
void ErrorDetect::overwriteHILORegister(bitset<6> func){//every time call mult(u) and mfhi(lo)
    if(func == 24 || func == 25){//mult(u)
        mult++;
        if(mult != 1){
            if(mf == 0){
                FILE* fptr;
                fptr = fopen("error_dump.rpt", "a");
                fprintf(fptr , "In cycle %d: Overwrite HI-LO registers\n", cycle);
                fclose(fptr);
            }
            else mf = 0;
        }
    }
    else if(func == 16 || func ==18){
        mf++;
    }
}
int ErrorDetect::memoryAddressOverflow(bitset<6> opCode, bitset<32> address){
    int halt = 0;
    bitset<32> maxAddress(0);
    if(opCode == 35 || opCode == 43)//word
        maxAddress = bitset<32> (address.to_ulong()+3);
    else if(opCode == 33 || opCode ==37 || opCode == 41)//half
        maxAddress = bitset<32> (address.to_ulong()+1);
    else//byte
        maxAddress = address;
    if(address.to_ulong() > 1023 || maxAddress.to_ulong() > 1023){
        halt = 1;
        FILE* fptr;
        fptr = fopen("error_dump.rpt", "a");
        fprintf(fptr , "In cycle %d: Address Overflow\n", cycle);
        fclose(fptr);
    }
    return halt;
}
int ErrorDetect::dataMisaligned(bitset<6> opCode, bitset<32> address){
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
        fptr = fopen("error_dump.rpt", "a");
        fprintf(fptr , "In cycle %d: Misalignment Error\n", cycle);
        fclose(fptr);
    }
    return halt;
}
