#include <bits/stdc++.h>
using namespace std;

bitset<32> signExtend(bitset<16> dataIn){
    bitset<32> dataOut(dataIn.to_ulong());
    if(dataIn[15]){//negative
        for(int i=31;i>=16;i--)
            dataOut[i] = 1;
    }
    return dataOut;
}

bitset<32> unsignExtend(bitset<16> dataIn){
    bitset<32> dataOut(dataIn.to_ulong());
    return dataOut;
}

