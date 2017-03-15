#include <bits/stdc++.h>
#include "ALU.h"
#include "errorDetect.h"
#include "instruction.h"
#include "memory.h"
#include "regFile.h"
#include "signExtend.h"

using namespace std;
bitset<32> pc;
bitset<32> initialSp;
bitset<32> HI;
bitset<32> LO;
ErrorDetect ED;

int main()
{
    InstructionMemory IM;
    DataMemory DM;
    RegisterFile RF;
    ALU ALU1;

    int cycle = 0;
    bitset<32> initialPc = pc;

    //control
    bitset<2> ALUop;

    RF.outputRegister(cycle);
    //DM.outputDataMemory();
    while(1){
        cycle++;
        ED.setCycle(cycle);
        int offsetPc = pc.to_ulong() - initialPc.to_ulong();

        Instruction instruction(IM.readInstMemory(bitset<32>(offsetPc)));//Instruction instruction(IM.readInstMemory(pc));
        //print instruction
        cout << "cycle" << cycle << endl;
        cout << "instruction: " << instruction.completeInst << ", type: " << instruction.type << "  " << instruction.func << endl;

        if(instruction.type == 'S'){//halt
            break;
        }
        else if(instruction.type == 'J'){
            if(instruction.opCode == 3){//ra = pc + 4
                int pcReturn = pc.to_ulong() + 4;
                RF.readWrite(0, 0, 31, pcReturn, 1);//reg write
            }

            int nextPcInt = pc.to_ulong() +4;
            bitset<32> nextPc(nextPcInt);
            for(int i=28;i<32;i++)
                pc.set(i, nextPc[i]);
            for(int i=2;i<28;i++)
                pc.set(i, instruction.targetAddr[i-2]);
            pc.set(0, 0);
            pc.set(1, 0);
        }
        else if(instruction.type == 'I'){
            if(instruction.opCode == 4 || instruction.opCode == 5 || instruction.opCode == 7){//beq bne bgtz
                //ALU 01 for sub
                ALUop.set(1, 0);
                ALUop.set(0, 1);

                bool satisfy;
                bitset<32> signExtenedImme = signExtend(instruction.immediate);
                RF.readWrite(instruction.regRs, instruction.regRt, 0, 0, 0);//reg read
                ALU1.ALUoperater(RF.readData1, RF.readData2,  ALU1.ALUcontrol(ALUop, 0));

                ///error detect
                //- - +  => - + -, + - - => + + +
                if(RF.readData1[31] == 1 && RF.readData2[31] == 0)//error when ans is +
                    ED.numberOverflow(-1, -1, ALU1.ALUResult);
                else if(RF.readData1[31] == 0 && RF.readData2[31] == 1)//error when ans is -
                    ED.numberOverflow(1, 1, ALU1.ALUResult);

                if(instruction.opCode == 4)
                    if(ALU1.zero == 1)
                        satisfy = true;
                    else
                        satisfy = false;
                else if(instruction.opCode == 5)
                    if(~ALU1.zero == 1)
                        satisfy = true;
                    else
                        satisfy = false;
                else if(instruction.opCode == 7)
                    if(~ALU1.ALUResult[31] == 1)
                        satisfy = true;
                    else
                        satisfy = false;

                if(satisfy){
                    int nextPc = pc.to_ulong() + 4 + (signExtenedImme<<2).to_ulong();
                    pc = bitset<32>(nextPc);
                }
                else{
                    int nextPc = pc.to_ulong() + 4;
                    pc = bitset<32>(nextPc);
                }
            }
            else{
                if(instruction.opCode == 35 || instruction.opCode == 33 || instruction.opCode == 37 || instruction.opCode == 32 || instruction.opCode ==  36){//load
                    //ALU 00 for add
                    ALUop.set(1, 0);
                    ALUop.set(0, 0);

                    bitset<32> signExtenedImme = signExtend(instruction.immediate);
                    RF.readWrite(instruction.regRs, instruction.regRt, 0, 0, 0);//reg read
                    ALU1.ALUoperater(signExtenedImme, RF.readData1, ALU1.ALUcontrol(ALUop, 0));//$s + C(un/signed)

                    ///error detect
                    ED.writeToRegister0(instruction.regRt);
                    //+ + +, - + -
                    if((RF.readData1[31] == 0 && RF.readData2[31] == 0) || (RF.readData1[31] == 1 && RF.readData2[31] == 1))
                        ED.numberOverflow(RF.readData1, RF.readData2, ALU1.ALUResult);
                    int halt = 0;
                    if(ED.memoryAddressOverflow(instruction.opCode, ALU1.ALUResult))
                        halt = 1;
                    if(ED.dataMisaligned(instruction.opCode, ALU1.ALUResult))
                        halt = 1;
                    if(halt == 1)
                        break;

                    bitset<32> writeToReg = DM.readWriteMemory(ALU1.ALUResult, 0, 1, 0);//mem read
                    bitset<32> realWriteToReg(0);
                    if(instruction.opCode == 35){//lw
                        realWriteToReg = writeToReg;
                    }
                    else if(instruction.opCode == 33 || instruction.opCode == 37){//lh  lhu
                        if(instruction.opCode == 33 && writeToReg[31] == 1)
                            realWriteToReg = bitset<32>(4294967295);//"0xFFFFFFFF"
                        for(int i=0;i<16;i++)
                            realWriteToReg.set(i, writeToReg[i+16]);
                    }
                    else if(instruction.opCode == 32 || instruction.opCode == 36){//lb  lbu
                        if(instruction.opCode == 32 && writeToReg[31] == 1)
                            realWriteToReg = bitset<32>(4294967295);//"0xFFFFFFFF"
                        for(int i=0;i<8;i++)
                            realWriteToReg.set(i, writeToReg[i+24]);
                    }
                    RF.readWrite(0, 0, instruction.regRt, realWriteToReg, 1);
                }
                else if(instruction.opCode == 43 || instruction.opCode == 41 || instruction.opCode ==  40){//store
                    //ALU 00 for add
                    ALUop.set(1, 0);
                    ALUop.set(0, 0);

                    bitset<32> signExtenedImme = signExtend(instruction.immediate);
                    RF.readWrite(instruction.regRs, instruction.regRt, 0, 0, 0);
                    ALU1.ALUoperater(signExtenedImme, RF.readData1, ALU1.ALUcontrol(ALUop, 0));//$s + C(signed)

                    ///error detect
                    //+ + +, - + -
                    if((RF.readData1[31] == 0 && RF.readData2[31] == 0) || (RF.readData1[31] == 1 && RF.readData2[31] == 1))
                        ED.numberOverflow(RF.readData1, RF.readData2, ALU1.ALUResult);
                    int halt = 0;
                    if(ED.memoryAddressOverflow(instruction.opCode, ALU1.ALUResult))
                        halt = 1;
                    if(ED.dataMisaligned(instruction.opCode, ALU1.ALUResult))
                        halt = 1;
                    if(halt == 1)
                        break;

                    if(instruction.opCode == 43){//sw
                        DM.readWriteMemory(ALU1.ALUResult, RF.readData2, 0, 1);//mem write
                    }
                    else if(instruction.opCode == 41){//sh
                        bitset<32> writeToMem = RF.readData2<<16;
                        bitset<32> originalMem = DM.readWriteMemory(ALU1.ALUResult, 0, 1, 0);//mem read
                        for(int i=0;i<16;i++)
                            writeToMem.set(i, originalMem[i]);
                        DM.readWriteMemory(ALU1.ALUResult, writeToMem, 0, 1);//mem write
                    }
                    else if(instruction.opCode == 40){//sb
                        bitset<32> writeToMem = RF.readData2<<24;
                        bitset<32> originalMem = DM.readWriteMemory(ALU1.ALUResult, 0, 1, 0);//mem read
                        for(int i=0;i<24;i++)
                            writeToMem.set(i, originalMem[i]);
                        DM.readWriteMemory(ALU1.ALUResult, writeToMem, 0, 1);//mem write
                    }
                }
                else if(instruction.opCode == 8 || instruction.opCode == 9){//addi addiu
                    RF.readWrite(instruction.regRs, instruction.regRt, 0, 0, 0);//reg read
                    ALUop.set(1, 0);
                    ALUop.set(0, 0);
                    if(instruction.opCode == 8)
                        ALU1.ALUoperater(RF.readData1, signExtend(instruction.immediate),  ALU1.ALUcontrol(ALUop, 0));
                    else if(instruction.opCode == 9)
                        ALU1.ALUoperater(RF.readData1, unsignExtend(instruction.immediate),  ALU1.ALUcontrol(ALUop, 0));
                    RF.readWrite(0, 0, instruction.regRt, ALU1.ALUResult, 1);//reg write

                    ///error detect
                    ED.writeToRegister0(instruction.regRt);
                    if(instruction.opCode == 8)
                        ED.numberOverflow(RF.readData1, signExtend(instruction.immediate), ALU1.ALUResult);
                }
                else if(instruction.opCode == 15 || instruction.opCode == 12 || instruction.opCode == 13 || instruction.opCode == 14 || instruction.opCode == 10){
                    RF.readWrite(instruction.regRs, instruction.regRt, 0, 0, 0);//reg read
                    //directly assign alu control
                    if(instruction.opCode == 15)
                        ALU1.ALUoperater(RF.readData2, 16, 8);//shift <<
                    else if(instruction.opCode == 12)
                        ALU1.ALUoperater(RF.readData1, unsignExtend(instruction.immediate), 2);//andi
                    else if(instruction.opCode == 13)
                        ALU1.ALUoperater(RF.readData1, unsignExtend(instruction.immediate), 3);//ori
                    else if(instruction.opCode == 14)
                        ALU1.ALUoperater(RF.readData1, unsignExtend(instruction.immediate), 5);//nori
                    else if(instruction.opCode == 10)
                        ALU1.ALUoperater(RF.readData1, signExtend(instruction.immediate), 7);//slti
                    RF.readWrite(0, 0, instruction.regRt, ALU1.ALUResult, 1);//reg write
                }
                else{//unrecognized instruction
                    printf("illegal instruction found at 0x%X\n", pc);
                    break;
                }
                //pc = pc + 4
                int pcInt = pc.to_ulong() + 4;
                pc = bitset<32>(pcInt);
            }
        }
        else if(instruction.type == 'R'){
            //ALU 10 for need to reference func field
            ALUop.set(1, 1);
            ALUop.set(0, 0);

            if(instruction.func == 32 || instruction.func == 33 || instruction.func == 34){//add addu sub
                RF.readWrite(instruction.regRs, instruction.regRt, 0, 0, 0);//reg read
                ALU1.ALUoperater(RF.readData1, RF.readData2,  ALU1.ALUcontrol(ALUop, instruction.func));
                RF.readWrite(0, 0, instruction.regRd, ALU1.ALUResult, 1);//reg write
                ///error detect
                ED.writeToRegister0(instruction.regRd);
                if(instruction.func == 32)
                    ED.numberOverflow(RF.readData1, RF.readData2, ALU1.ALUResult);
                else if(instruction.func == 34)
                    //- - +  => - + -, + - - => + + +
                    if(RF.readData1[31] == 1 && RF.readData2[31] == 0)//error when ans is +
                        ED.numberOverflow(-1, -1, ALU1.ALUResult);
                    else if(RF.readData1[31] == 0 && RF.readData2[31] == 1)//error when ans is -
                        ED.numberOverflow(1, 1, ALU1.ALUResult);
            }
            else if(instruction.func == 36 || instruction.func == 37 || instruction.func == 38 || instruction.func == 39 || instruction.func == 40 || instruction.func == 42){//and or xor nor nand slt
                RF.readWrite(instruction.regRs, instruction.regRt, 0, 0, 0);//reg read
                ALU1.ALUoperater(RF.readData1, RF.readData2,  ALU1.ALUcontrol(ALUop, instruction.func));
                RF.readWrite(0, 0, instruction.regRd, ALU1.ALUResult, 1);//reg write
            }
            else if(instruction.func == 0 || instruction.func == 2 || instruction.func == 3){//sll srl sra
                RF.readWrite(instruction.regRs, instruction.regRt, 0, 0, 0);//reg read
                bitset<32> shift(instruction.shamt.to_ulong());
                ALU1.ALUoperater(RF.readData2, shift,  ALU1.ALUcontrol(ALUop, instruction.func));
                RF.readWrite(0, 0, instruction.regRd, ALU1.ALUResult, 1);//reg write

                ///error detect
                //NOP sll $0, $0, 0=0x00000000 skip
                if(instruction.func != 0 || instruction.regRd != 0 || instruction.regRt != 0 || instruction.immediate == 0){
                    ED.writeToRegister0(instruction.regRd);
                }
            }
            else if(instruction.func == 8){//jr
                RF.readWrite(instruction.regRs, instruction.regRt, 0, 0, 0);//reg read
                pc = RF.readData1;
            }
            else if(instruction.func == 24 || instruction.func == 25){//mult multu
                RF.readWrite(instruction.regRs, instruction.regRt, 0, 0, 0);//reg read
                ///error detect
                ALU1.ALUoperater(RF.readData1, RF.readData2,  ALU1.ALUcontrol(ALUop, instruction.func));//HI LO write
                ED.overwriteHILORegister(instruction.func);
            }
            else if(instruction.func == 16){//mfhi
                RF.readWrite(0, 0, instruction.regRd, HI, 1);//reg write
                ///error detect
                ED.writeToRegister0(instruction.regRd);
                ED.overwriteHILORegister(instruction.func);
            }
            else if(instruction.func == 18){//mflo
                RF.readWrite(0, 0, instruction.regRd,LO, 1);//reg write
                ///error detect
                ED.writeToRegister0(instruction.regRd);
                ED.overwriteHILORegister(instruction.func);
            }
            else{//unrecognized instruction
                printf("illegal instruction found at 0x%X\n", pc);
                break;
            }
            //pc = pc + 4
            if(instruction.func != 8){//skip jr
                int pcInt = pc.to_ulong() + 4;
                pc = bitset<32>(pcInt);
            }
        }
        RF.outputRegister(cycle);
    }

}
