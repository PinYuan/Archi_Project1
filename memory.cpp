#include <bits/stdc++.h>
using namespace std;

extern  bitset<32> pc;
extern  bitset<32> sp;

class InstructionMemory{
private:
    bitset<8> instMemory[1024];
    int numOfInst;
public:
    InstructionMemory(){
        FILE *fptr;
        fptr = fopen("iimage.bin" , "rb");
        if(!fptr){printf("open file error\n");return;}

        char buffer[4];
        int eatTime = 0;
        int nums = 0;

        while(fread(buffer, 4, 1,fptr)){
            if(eatTime == 0){
                string stringPc;
                for(int i=0;i<4;i++){
                    bitset<8> bs(buffer[i]);
                    stringPc += bs.to_string();
                }
                bitset<32> bPc(stringPc);
                //pc = bPc.to_ulong();
                pc = bPc;
                continue;
            }
            else if(eatTime == 1){
                string stringNums;
                for(int i=0;i<4;i++){
                    bitset<8> bs(buffer[i]);
                    stringNums += bs.to_string();
                }
                bitset<32> bNums(stringNums);
                numOfInst = bNums.to_ulong();
                continue;
            }
            else{
                for(int i=0;i<4;i++){
                    bitset<8> inst(buffer[i]);
                    instMemory[nums++] = inst;
                }
            }
            eatTime++;
        }
        fclose(fptr);
    }

    bitset<32> readInstMemory(bitset<32> readAddress){
        int readAddressInt = readAddress.to_ulong();
        string instString;

        for(int i=0;i<4;i++){
            instString += instMemory[readAddressInt+i].to_string();
        }
        bitset<32> inst(instString);
        return inst;
    }

    void outputInstMemory(){
        FILE* fptr;
        fptr = fopen("instMemOut.out", "w");
        if(!fptr){printf("open file error\n");return ;}

        char buffer[10];
        for(int i=0;i<numOfInst;i++){
            string bufferString;
            for(int j=0;j<4;j++){
                bufferString += instMemory[i*4+j].to_string();
            }
            bitset<32> inst(bufferString);
            sprintf(buffer, "0x%08X", inst);
            fwrite(buffer, 10, 1, fptr);
        }
        fclose(fptr);
    }
};

class DataMemory{
private:
    bitset<8> dataMemory[1024];
    int numOfData;
public:
    DataMemory(){
        FILE *fptr;
        fptr = fopen("dimage.bin" , "rb");
        if(!fptr){printf("open file error\n");return ;}

        numOfData = 0;
        char buffer[4];
        int eatTime = 0;
        int nums = 0;

        while(fread(buffer, 4, 1,fptr)){
            if(eatTime == 0){
                string stringSp;
                for(int i=0;i<4;i++){
                    bitset<8> bs(buffer[i]);
                    stringSp += bs.to_string();
                }
                bitset<32> bSp(stringSp);
                //sp = bSp.to_ulong();
                sp = bSp;
                continue;
            }
            else if(eatTime == 1){
                string stringNums;
                for(int i=0;i<4;i++){
                    bitset<8> bs(buffer[i]);
                    stringNums += bs.to_string();
                }
                bitset<32> bNums(stringNums);
                numOfData = bNums.to_ulong();
                continue;
            }
            else{
                for(int i=0;i<4;i++){
                    bitset<8> data(buffer[i]);
                    dataMemory[nums++] = data;
                }
            }
            eatTime++;
        }
        fclose(fptr);
    }

    bitset<32> readWriteMemory(bitset<32> address, bitset<32> writeData, bitset<1> memRead, bitset<1> memWrite){
        if(memRead[0]){
            string dataString;
            for(int i=0;i<4;i++){
                dataString += dataMemory[address.to_ulong()+i].to_string();
            }
            bitset<32> data(dataString);
            return data;
        }
        if(memWrite[0]){
            for(int i=0;i<4;i++){
                for(int j=0;j<8;j++){
                    dataMemory[address.to_ulong()+i].set(j, writeData[(3-i)*8+j]);//big endian
                }
            }
        }
    }

    voif outputDataMemory(){
        FILE* fptr;
        fptr = fopen("dataMemOut.out", "w");
        if(!fptr){printf("open file error\n");return;}

        char buffer[10];
        for(int i=0;i<numOfData;i++){
            string bufferString;
            for(int j=0;j<4;j++){
                bufferString += dataMemory[i*4+j].to_string();
            }
            bitset<32> data(bufferString);
            sprintf(buffer, "0x%08X", data);
            fwrite(buffer, 10, 1, fptr);
        }
        fclose(fptr);
    }
};


