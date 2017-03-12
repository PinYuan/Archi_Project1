#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

class InstructionMemory{
private:
    bitset<8> instMemory[1024];
    int numOfInst;
public:
    InstructionMemory();

    bitset<32> readInstMemory(bitset<32> readAddress);

    void outputInstMemory();
};

class DataMemory{
private:
    bitset<8> dataMemory[1024];
    int numOfData;
public:
    DataMemory();

    bitset<32> readWriteMemory(bitset<32> address, bitset<32> writeData, bitset<1> memRead, bitset<1> memWrite);

    void outputDataMemory();
};

#endif // MEMORY_H_INCLUDED

