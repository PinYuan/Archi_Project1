single_cycle:simulator.o ALU.o errorDetect.o instruction.o memory.o regFile.o signExtend.o 
	g++ -std=c++11 -o single_cycle simulator.o ALU.o errorDetect.o instruction.o memory.o regFile.o signExtend.o
simulator.o:
	g++ -std=c++11 -c simulator.cpp
ALU.o:
	g++ -std=c++11 -c ALU.cpp
errorDetect.o:
	g++ -std=c++11 -c errorDetect.cpp
instruction.o:
	g++ -std=c++11 -c instruction.cpp
memory.o:
	g++ -std=c++11 -c memory.cpp
regFile.o:
	g++ -std=c++11 -c regFile.cpp
signExtend.o:
	g++ -std=c++11 -c signExtend.cpp
clean:
	rm -f mysimulator simulator.o ALU.o errorDetect.o instruction.o memory.o regFile.o signExtend.o
 
