#include <bits/stdc++.h>
#include "regFile.h"
#include "errorDetect.h"
using namespace std;

extern bitset<32> pc;
extern bitset<32> sp;
extern bitset<32> LO;
extern bitset<32> HI;
extern ErrorDetect ED;


RegisterFile::RegisterFile(){
	FILE* fptr;
    fptr = fopen("snapshot.rpt", "w");
    fclose(fptr);

    bitset<32> zero(0);
    for(int i=0;i<31;i++){
    	if(i == 29){//sp
			oldRegisters[i] = registers[i] = sp;
            continue;
		}
		registers[i] = zero;
      	oldRegisters[i] = zero;
	}
    HI = oldHI = zero;
    LO = oldLO = zero;
	oldPc = pc;
}

void RegisterFile::readWrite(bitset<5> readRegigter1, bitset<5> readRegigter2, bitset<5> writeRegigter
                             , bitset<32> writeData, bitset<1> regWrite){
     if(regWrite[0]){
		if(ED.writeToRegister0(writeRegigter) == 1)
            return;
     	registers[writeRegigter.to_ulong()] = writeData;
     }
     else{
        readData1 = registers[readRegigter1.to_ulong()];
        readData2 = registers[readRegigter2.to_ulong()];
     }
}

void RegisterFile::outputRegister(int cycle){
	FILE* fptr;
    fptr = fopen("registerOut.out", "a");
    if(!fptr){printf("open file error\n");return;}

	char buffer[20] = {'\0'};

	if(cycle == 0){//all print
		fprintf(fptr, "cycle %d\n", cycle);
        //reg
        for(int i=0;i<32;i++)
            fprintf(fptr, "$%02d: 0x%08X\n", i, registers[i]);
        //HI LO
        fprintf(fptr, "$HI: 0x%08X\n", HI);
        fprintf(fptr, "$LO: 0x%08X\n", LO);
        //pc
        fprintf(fptr, "$PC: 0x%08X\n", pc);

        fprintf(fptr, "\n\n");
	}
	else{//only print change
    	fprintf(fptr, "cycle %d\n", cycle);

        //reg check diff
        for(int i=0;i<32;i++){
            if(oldRegisters[i] != registers[i]){
                fprintf(fptr, "$%02d: 0x%08X\n", i, registers[i]);
                oldRegisters[i] = registers[i];
            }
        }
        //HI LO
        if(oldHI != HI){
            fprintf(fptr, "$HI: 0x%08X\n", HI);
            oldHI = HI;
        }
        if(oldLO != LO){
            fprintf(fptr, "$LO: 0x%08X\n", LO);
            oldLO = LO;
        }
        //pc
        if(oldPc != pc){
            fprintf(fptr, "PC: 0x%08X\n", pc);
            oldPc = pc;
        }
        fprintf(fptr, "\n\n");
	}	
	fclose(fptr);
}



