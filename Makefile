EXEC = blockchainParallel blockchainSequential
CC = mpiCC
CPP = g++
OPT = -c -Wall

all: $(EXEC)

blockchainSequential : ./src/*.cpp
	$(CPP) $(OPT) ./src/blockchainSequential.cpp -o blockchainSequential

blockchainParallel : ./src/*.cpp
	$(CC) $(OPT) ./src/blockchainParallel.cpp -o blockchainParallel -std=c++14

clean :
	rm -rf $(EXEC)
