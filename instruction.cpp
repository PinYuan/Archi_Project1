#include <bits/stdc++.h>
using namespace std;

int main()
{
    FILE *fptr;
    char buffer[4];
    fptr = fopen("iimage.bin" , "rb");
    if(!fptr){printf("open file error\n");return 0;}
    //FILE *destination = fopen("out.bin", "wb");
    int eatdata = 0;
    unsigned int PC = 0;
    unsigned int numOfIntruction = 0;
    while(fread(buffer, 4, 1,fptr)){
        //fwrite(buffer, 4, 1, destination);
        string instruction;
        for(int i=0;i<4;i++){
            bitset<8> bs(buffer[i]);
            instruction += bs.to_string();
        }
        cout<<instruction<<endl;
        if(eatdata == 0){
            bitset<32> bPC(instruction);
            PC = bPC.to_ulong();
            cout<<PC<<endl;
            continue;
        }
        else if(eatdata == 1){
            bitset<32> bnums(instruction);
            numOfIntruction = bnums.to_ulong();
            cout<<numOfIntruction<<endl;
            continue;
        }
        eatdata++;
        //judge which instruction
        string opcode = instruction.substr(0, 6);
        if(opcode == "000000"){//R
            string rs = instruction.substr(6, 5);
            string rt = instruction.substr(11, 5);
            string rd = instruction.substr(16, 5);
            string shamt = instruction.substr(21, 5);
            string func = instruction.substr(26, 6);
            if(func == "100000"){//add 20
                    printf("%-5s","add");
                    bitset<5> brd(rd);
                    bitset<5> brs(rs);
                    bitset<5> brt(rt);
                    printf("$%d, ",brd.to_ulong());
                    printf("$%d, ",brs.to_ulong());
                    printf("$%d\n",brt.to_ulong());
            }
            else if(func == "100001"){//addu 21

            }
            else if(func == "100010"){//sub 22

            }
            else if(func == "100100"){//and 24

            }
            else if(func == "100101"){//or 25

            }
            else if(func == "100110"){//xor 26

            }
            else if(func == "100111"){//nor 27

            }
            else if(func == "101000"){//nand 28

            }
            else if(func == "101010"){//slt 2A

            }
            else if(func == "000000"){//sll 00

            }
            else if(func == "000010"){//srl 02

            }
            else if(func == "000011"){//sra 03

            }
            else if(func == "000100"){//jr 08

            }
            else if(func == "010100"){//mult 18

            }
            else if(func == "011001"){//multu 19

            }
            else if(func == "011010"){//mfhi 10

            }
            else if(func == "010010"){//mflo 12

            }
        }
        else if(opcode == "000010"){//J j
            string addr = instruction.substr(6, 26);
        }
        else if(opcode == "000011"){//J jal
            string addr = instruction.substr(6, 26);
        }
        else if(opcode == "111111"){//halt

        }
        else{//I
            string rs = instruction.substr(6, 5);
            string rt = instruction.substr(11, 5);
            string immediate = instruction.substr(16, 16);
           /* if(opcode == "addi"){//addi 08

            }
            switch(opcode){
                case ""://addi 08

                    break;
                case ""://addiu 09

                    break;
                case ""://lw 23

                    break;
                case ""://lh 21

                    break;
                case ""://lhu 25

                    break;
                case ""://lb 20

                    break;
                case ""://lbu 24

                    break;
                case ""://sw 2B

                    break;
                case ""://sh 29

                    break;
                case ""://sb 28

                    break;
                case ""://lui 0F

                    break;
                case ""://andi 0C

                    break;
                case ""://ori 0D

                    break;
                case ""://nori 0E

                    break;
                case ""://slti 0A

                    break;
                case ""://beq 04

                    break;
                case ""://bne 05

                    break;
                case ""://bgtz 07

                    break;
                default:
                     break;
            }*/
        }

    }

}

