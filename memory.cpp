#include <bits/stdc++.h>
#include "memory.h"
#include "errorDetect.h"
using namespace std;

extern  bitset<32> pc;
extern  bitset<32> initialSp;

InstructionMemory::InstructionMemory(){
	FILE *fptr;
	fptr = fopen("iimage.bin" , "rb");
	if(!fptr){printf("open file error\n");return;}

	char buffer[4];
	int eatTime = 0;
	int nums = 0;
	
	bitset<8> zero(0);
    for(int i=0;i<1024;i++)
        instMemory[i] = zero;
	
	while(fread(buffer, 4, 1,fptr)){
		if(eatTime == 0){
			string stringPc;
			for(int i=0;i<4;i++){
				bitset<8> bs(buffer[i]);
				stringPc += bs.to_string();
			}
			pc = bitset<32> (stringPc);
		}
		else if(eatTime == 1){
			string stringNums;
			for(int i=0;i<4;i++){
				bitset<8> bs(buffer[i]);
				stringNums += bs.to_string();
			}
			bitset<32> bNums(stringNums);
			numOfInst = bNums.to_ulong();
		}
		else{
			for(int i=0;i<4;i++){
				bitset<8> inst(buffer[i]);
                int programCounter = pc.to_ulong() + nums;
                instMemory[programCounter] = inst;
                nums++;
			}
		}
		eatTime++;
		if(eatTime == numOfInst+2)break;
	}
	fclose(fptr);
}

bitset<32> InstructionMemory::readInstMemory(bitset<32> readAddress){
	int readAddressInt = readAddress.to_ulong();
	string instString;

	for(int i=0;i<4;i++){
		instString += instMemory[readAddressInt+i].to_string();
	}
	bitset<32> inst(instString);
	return inst;
}

void InstructionMemory::outputInstMemory(){
	FILE* fptr;
	fptr = fopen("instMemOut.out", "w");
	if(!fptr){printf("open file error\n");return ;}

	char buffer[20] = {'\0'};
	for(int i=0;i<numOfInst;i++){
		string bufferString;
		for(int j=0;j<4;j++){
		bufferString += instMemory[i*4+j].to_string();
		}
		bitset<32> inst(bufferString);
		sprintf(buffer, "0x%08X\n", (unsigned int)inst.to_ulong());
		fwrite(buffer, strlen(buffer), 1, fptr);
	}
	fclose(fptr);
}

DataMemory::DataMemory(){
	FILE *fptr;
	fptr = fopen("dimage.bin" , "rb");
	if(!fptr){printf("open file error\n");return ;}

	numOfData = 0;
	char buffer[4];
	int eatTime = 0;
	int nums = 0;

	bitset<8> zero(0);
    for(int i=0;i<1024;i++)
        dataMemory[i] = zero;

	while(fread(buffer, 4, 1,fptr)){
		if(eatTime == 0){
			string stringSp;
			for(int i=0;i<4;i++){
				bitset<8> bs(buffer[i]);
				stringSp += bs.to_string();
			}
			initialSp = bitset<32> (stringSp);
		}
		else if(eatTime == 1){
			string stringNums;
			for(int i=0;i<4;i++){
				bitset<8> bs(buffer[i]);
				stringNums += bs.to_string();
			}
			bitset<32> bNums(stringNums);
			numOfData = bNums.to_ulong();
		}
		else{
			for(int i=0;i<4;i++){
				bitset<8> data(buffer[i]);
				dataMemory[nums++] = data;
			}
			if(nums == 1024)break;
		}
		eatTime++;
		if(eatTime == numOfData+2)break;
	}
	fclose(fptr);
}

bitset<32> DataMemory::readWriteMemory(bitset<32> address, bitset<32> writeData, bitset<1> memRead, bitset<1> memWrite){
	if(memRead == 1){
		string dataString;
		for(int i=0;i<4;i++){
			dataString += dataMemory[address.to_ulong()+i].to_string();
		}
		bitset<32> data(dataString);
		return data;
	}
	if(memWrite == 1){
	for(int i=0;i<4;i++){
			for(int j=0;j<8;j++){
				dataMemory[address.to_ulong()+i].set(j, writeData[(3-i)*8+j]);//big endian
			}
		}
	}
}

void DataMemory::outputDataMemory(){
	FILE* fptr;
	fptr = fopen("dataMemOut.out", "w");
	if(!fptr){printf("open file error\n");return;}

	char buffer[20] = {'\0'};
	for(int i=0;i<numOfData;i++){
		string bufferString;
		for(int j=0;j<4;j++){
			bufferString += dataMemory[i*4+j].to_string();
		}
		bitset<32> data(bufferString);
		sprintf(buffer, "0x%08X\n", (unsigned int)data.to_ulong());
		fwrite(buffer, strlen(buffer), 1, fptr);
	}
	fclose(fptr);
}


