#include <bits/stdc++.h>
#include "ALU.h"
using namespace std;
extern bitset<32> HI;
extern bitset<32> LO;


bitset<32> ALU::ALUoperater(bitset<32> data1, bitset<32> data2, bitset<4> ALUControl){
    int zeroInt;
    if(ALUControl == 0){//add
        int ALUResultInt = data1.to_ulong() + data2.to_ulong();
        ALUResult = bitset<32>(ALUResultInt);
    }
    else if(ALUControl == 1){//sub
        int ALUResultInt = data1.to_ulong() - data2.to_ulong();
        ALUResult = bitset<32>(ALUResultInt);
    }
    else if(ALUControl == 2){//and
        data1 &= data2;
        ALUResult = data1;
    }
    else if(ALUControl == 3){//or
        data1 |= data2;
        ALUResult = data1;
    }
    else if(ALUControl == 4){//xor
        data1 ^= data2;
        ALUResult = data1;
    }
    else if(ALUControl == 5){//nor
        data1 |= data2;
        data1 = ~data1;
        ALUResult = data1;
    }
    else if(ALUControl == 6){//nand
        data1 &= data2;
        data1 = ~data1;
        ALUResult = data1;
    }
    else if(ALUControl == 7){//slt
        int ALUResultInt;
        if(data1[31] == 0 && data2[31] == 0)//+ +
            ALUResultInt =  data1.to_ulong() < data2.to_ulong();
        else if(data1[31] == 1 && data2[31] == 0)//- +
            ALUResultInt = 1;
        else if(data1[31] == 0 && data2[31] == 1)//+ -
            ALUResultInt = 0;
        else
            ALUResultInt = data1.to_ulong() < data2.to_ulong();
        ALUResult = bitset<32>(ALUResultInt);
    }
    else if(ALUControl == 8){//sll
        int shiftTime = data2.to_ulong();
        ALUResult = data1 << shiftTime;
    }
    else if(ALUControl == 9){//srl
        int shiftTime = data2.to_ulong();
        ALUResult = data1 >> shiftTime;
    }
    else if(ALUControl == 10){//sra
        int neg = data1[31]?1:0;
        int shiftTime = data2.to_ulong();
        data1 >>= shiftTime;
        if(neg){
            for(int i=0;i<shiftTime;i++)
                data1.set(31-i,1);
        }
        ALUResult = data1;
    }
	else if(ALUControl == 11 || ALUControl == 12){//mult multu
        bitset<64> ALUResultInt64(0);
        if(ALUControl == 11){//mult
            //turn - => +
            bitset<32> absData1(0);
            bitset<32> absData2(0);
            if(data1[31] == 1)
                absData1 = bitset<32>(data1.flip().to_ulong()+1);
            if(data2[31] == 1)
                absData2 = bitset<32>(data2.flip().to_ulong()+1);
            for(int i=0;i<32;i++){
                if(absData2[i] == 1){
                    bitset<64> temp = bitset<64>(absData1.to_ulong());
                    temp <<= i;
                    ALUResultInt64 = bitset<64>(ALUResultInt64.to_ullong()+temp.to_ullong());
                }
            }
            if(data1[31] ^ data2[31])
                ALUResultInt64 = bitset<64>(ALUResultInt64.flip().to_ullong()+1);
        }
        else{//multu
            for(int i=0;i<32;i++){
                if(data2[i] == 1){
                    bitset<64> temp = bitset<64>(data1.to_ulong());
                    temp <<= i;
                    ALUResultInt64 = bitset<64>(ALUResultInt64.to_ullong()+temp.to_ullong());
                }
            }
        }
        for(int i=0;i<32;i++)
            HI.set(i, ALUResultInt64[i+32]);
        for(int i=0;i<32;i++)
            LO.set(i, ALUResultInt64[i]);
        if(ALUResultInt64 == 0)
            ALUResult = bitset<32>(0);
    }
    zero = (ALUResult == 0)?bitset<1>(1):bitset<1>(0);
    return ALUResult;
}

bitset<4> ALU::ALUcontrol(bitset<2> ALUop, bitset<6> func){
    bitset<4> ALUControl(0);
    ALUControl.set(3, ALUop[1]&(!func[5]));

    ALUControl.set(2, (ALUop[1]&func[5]&func[2]&func[1])|
                      (ALUop[1]&func[5]&func[3]&(!func[2]))|
                      (ALUop[1]&(!func[5])&func[3]&func[0]));

    ALUControl.set(1, (ALUop[1]&func[5]&func[2]&(!func[1]))|
                      (ALUop[1]&func[5]&func[3])|
                      (ALUop[1]&(!func[5])&(!func[4])&func[0])|
                      (ALUop[1]&func[4]&(!func[0])));

    int set0 = 0;
    if((func == 34) || (func == 37) || (func == 39) || (func == 42) || (func == 2) || (func == 24)){
        set0 = 1;
    }
    ALUControl.set(0, (ALUop[0])|set0);
    return ALUControl;
}

