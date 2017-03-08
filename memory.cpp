#include <bits/stdc++.h>
using namespace std;

char instructionMemory[1024];
char dataMemory[1024];
extern unsigned int pc;
extern unsigned int sp;

string readInstructionMemory(int read_Address){
    string instruction;
    for(int i=0;i<4;i++){
        instruction += instructionMemory[read_Address+i];
    }
    return instruction;
}

int initialInstructionMemory(){
    FILE *fptr;
    char buffer[4];
    fptr = fopen("iimage.bin" , "rb");
    if(!fptr){printf("open file error\n");return 0;}

    unsigned int numOfInstruction;
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
            pc = bPc.to_ulong();
            continue;
        }
        else if(eatTime == 1){
            string stringNums;
            for(int i=0;i<4;i++){
                bitset<8> bs(buffer[i]);
                stringNums += bs.to_string();
            }
            bitset<32> bNums(stringNums);
            numOfInstruction = bNums.to_ulong();
            continue;
        }
        else{
            for(int i=0;i<4;i++){
                instructionMemory[nums++] = buffer[i];
            }
        }
        eatTime++;
    }
    fclose(fptr);
}

int DataMemory(int address, int writeData, bool memRead, bool memWrite){

}

int initialDataMemory(){
    FILE *fptr;
    char buffer[4];
    fptr = fopen("dimage.bin" , "rb");
    if(!fptr){printf("open file error\n");return 0;}

    unsigned int numOfData;
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
            sp = bSp.to_ulong();
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
                dataMemory[nums++] = buffer[i];
            }
        }
        eatTime++;
    }
    fclose(fptr);
}
