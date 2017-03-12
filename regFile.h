#ifndef REGFILE_H_INCLUDED
#define REGFILE_H_INCLUDED

class RegisterFile{
private:
    bitset<32> registers[32];
    bitset<32> readData1, readData2;
    bitset<32> oldRegisters[32];
    bitset<32> oldPc, oldHI, oldLO;
public:
    RegisterFile();

    void readWrite(bitset<5> readRegigter1, bitset<5> readRegigter2, bitset<5> writeRegigter
                   , bitset<32> writeData, bitset<1> regWrite);

    void outputRegister(int cycle);
};

#endif // REGFILE_H_INCLUDED

