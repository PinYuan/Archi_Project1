#include <bits/stdc++.h>
#include "regFile.h"
using namespace std;

extern bitset<32> pc;
extern bitset<32> initialSp;
extern bitset<32> LO;
extern bitset<32> HI;


RegisterFile::RegisterFile(){
	FILE* fptr;
    fptr = fopen("snapshot.rpt", "w");
    fclose(fptr);

    bitset<32> zero(0);
    for(int i=0;i<31;i++){
    	if(i == 29){//sp
			oldRegisters[i] = registers[i] = initialSp;
            continue;
		}
		registers[i] = zero;
      	oldRegisters[i] = zero;
	}
    HI = oldHI = zero;
    LO = oldLO = zero;
	oldPc = pc;
}

void RegisterFile::readWrite(bitset<5> readRegister1, bitset<5> readRegister2, bitset<5> writeRegister
                             , bitset<32> writeData, bitset<1> regWrite){
     if(regWrite[0]){
		if(writeRegister == 0)
            return;
     	registers[writeRegister.to_ulong()] = writeData;
     }
     else{
        readData1 = registers[readRegister1.to_ulong()];
        readData2 = registers[readRegister2.to_ulong()];
     }
}

void RegisterFile::outputRegister(int cycle){
	FILE* fptr;
    fptr = fopen("snapshot.rpt", "a");
    if(!fptr){printf("open file error\n");return;}


	if(cycle == 0){//all print
		fprintf(fptr, "cycle %d\n", cycle);
        //reg
        for(int i=0;i<32;i++)
            fprintf(fptr, "$%02d: 0x%08X\n", i, (unsigned int)registers[i].to_ulong());
        //HI LO
        fprintf(fptr, "$HI: 0x%08X\n", (unsigned int)HI.to_ulong());
        fprintf(fptr, "$LO: 0x%08X\n", (unsigned int)LO.to_ulong());
        //pc
        fprintf(fptr, "PC: 0x%08X\n", (unsigned int)pc.to_ulong());

        fprintf(fptr, "\n\n");
	}
	else{//only print change
    	fprintf(fptr, "cycle %d\n", cycle);

        //reg check diff
        for(int i=0;i<32;i++){
            if(oldRegisters[i] != registers[i]){
                fprintf(fptr, "$%02d: 0x%08X\n", i, (unsigned int)registers[i].to_ulong());
                oldRegisters[i] = registers[i];
            }
        }
        //HI LO
        if(oldHI != HI){
            fprintf(fptr, "$HI: 0x%08X\n", (unsigned int)HI.to_ulong());
            oldHI = HI;
        }
        if(oldLO != LO){
            fprintf(fptr, "$LO: 0x%08X\n", (unsigned int)LO.to_ulong());
            oldLO = LO;
        }
        //pc
        if(oldPc != pc){
            fprintf(fptr, "PC: 0x%08X\n", (unsigned int)pc.to_ulong());
            oldPc = pc;
        }
        fprintf(fptr, "\n\n");
	}	
	fclose(fptr);
}



