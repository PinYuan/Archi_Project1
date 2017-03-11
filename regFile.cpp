#include <bits/stdc++.h>
using namespace std;

class RegigterFile{
private:
    bitset<32> registers[32];
    bitset<32> HI;
    bitset<32> LO;
public:
    bitset<32> readData1, readData2;

    RegigterFile(){
        bitset<32> zero(0);
        for(int i=0;i<31;i++){
            registers[i] = zero;
        }
        HI = zero;
        LO = zero;
    }

    void readWrite(bitset<5> readRegigter1, bitset<5> readRegigter2, bitset<5> writeRegigter
                   , bitset<32> writeData, bitset<1> regWrite){
        if(regWrite[0]){
            registers[writeRegigter.to_ulong()] = writeData;
        }
        else{
            readData1 = registers[readRegigter1.to_ulong()];
            readData2 = registers[readRegigter2.to_ulong()];
        }
    }

    //TO DO write HI LO

    void outputRegister(){
        FILE* fptr;
        fptr = fopen("registerOut.out", "w");
        if(!fptr){printf("open file error\n");return;}

        char buffer[20] = {'\0'};
        for(int i=0;i<32;i++){
            string bufferString;
            bufferString = registers[i].to_string();

            bitset<32> reg(bufferString);
            sprintf(buffer, "$%02d: 0x%08X\n", i, reg);
            fwrite(buffer, strlen(buffer), 1, fptr);
        }

        //HI LO
        sprintf(buffer, "$HI: 0x%08X\n", HI);
        fwrite(buffer, strlen(buffer), 1, fptr);
        sprintf(buffer, "$LO: 0x%08X\n", LO);
        fwrite(buffer, strlen(buffer), 1, fptr);
        fclose(fptr);
    }
};



