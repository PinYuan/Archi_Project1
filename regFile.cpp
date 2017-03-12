#include <bits/stdc++.h>
using namespace std;
extern bitset<32>pc;
extern bitset<32>LO;
extern bitset<32>HI;

class RegisterFile{
private:
    bitset<32> registers[32];
    bitset<32> readData1, readData2;
	bitset<32> oldRegisters[32];
    bitset<32> oldPc, oldHI, oldLO;
public:

    RegisterFile(){
		FILE* fptr;
        fptr = fopen("snapshot.rpt", "w");
        fclose(fptr);

        bitset<32> zero(0);
        for(int i=0;i<31;i++){
            registers[i] = zero;
        	oldRegisters[i] = zero;
		}
        HI = oldHI = zero;
        LO = oldLO = zero;
		oldPc = pc;
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

    void outputRegister(int cycle){
        FILE* fptr;
        fptr = fopen("registerOut.out", "a");
        if(!fptr){printf("open file error\n");return;}

		if(cycle == 0){//all print
	        char buffer[20] = {'\0'};
    		//reg
		    for(int i=0;i<32;i++){
            	sprintf(buffer, "$%02d: 0x%08X\n", i, registers[i]);
            	fwrite(buffer, strlen(buffer), 1, fptr);
        	}
	        //HI LO
    	    sprintf(buffer, "$HI: 0x%08X\n", HI);
    	    fwrite(buffer, strlen(buffer), 1, fptr);
        	sprintf(buffer, "$LO: 0x%08X\n", LO);
        	fwrite(buffer, strlen(buffer), 1, fptr);
        	//pc
			sprintf(buffer, "PC: 0x%08X\n", pc);
            fwrite(buffer, strlen(buffer), 1, fptr);

            sprintf(buffer, "\n\n");
            fwrite(buffer, strlen(buffer), 1, fptr);
		}
		else{//only print change
            sprintf(buffer, "cycle%d\n", cycle);
            fwrite(buffer, strlen(buffer), 1, fptr);

            //reg check diff
            for(int i=0;i<32;i++){
                if(oldRegisters[i] != registers[i]){
                    sprintf(buffer, "$%02d: 0x%08X\n", i, registers[i]);
                    fwrite(buffer, strlen(buffer), 1, fptr);
                    oldRegisters[i] = registers[i];
                }
            }
            //HI LO
            if(oldHI != HI){
                sprintf(buffer, "$HI: 0x%08X\n", HI);
                fwrite(buffer, strlen(buffer), 1, fptr);
                oldHI = HI;
            }
            if(oldLO != LO){
                sprintf(buffer, "$LO: 0x%08X\n", LO);
                fwrite(buffer, strlen(buffer), 1, fptr);
                oldLO = LO;
            }
            //pc
            if(oldPc != pc){
                sprintf(buffer, "PC: 0x%08X\n", pc);
                fwrite(buffer, strlen(buffer), 1, fptr);
                oldPc = pc;
            }
            sprintf(buffer, "\n\n");
            fwrite(buffer, strlen(buffer), 1, fptr);
        }	
		fclose(fptr);
    }
};



