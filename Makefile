EXEC = blockchainParallel blockchainSequential
OBJ = block blockchain blockHeader merkleTree miner transactionsGenerator
CC = mpiCC
CPP = g++
OPT = -c

all: $(EXEC) $(OBJ)

blockchainSequential : ./src/blockchainSequential.cpp
	$(CPP) ./src/blockchainSequential.cpp -o blockchainSequential

blockchainParallel : ./src/blockchainParallel.cpp
	$(CC) ./src/blockchainParallel.cpp -o blockchainParallel -std=c++14

block : ./src/Block.cpp
	$(CPP) $(OPT) ./src/Block.cpp -o ./objs/Block.o

blockchain : ./src/Blockchain.cpp
	$(CPP) $(OPT) ./src/Blockchain.cpp -o ./objs/Blockchain.o

blockHeader : ./src/BlockHeader.cpp
	$(CPP) $(OPT) ./src/BlockHeader.cpp -o ./objs/BlockHeader.o

merkleTree : ./src/MerkleTree.cpp
	$(CPP) $(OPT) ./src/MerkleTree.cpp -o ./objs/MerkleTree.o

miner : ./src/Miner.cpp
	$(CPP) $(OPT) ./src/Miner.cpp -o ./objs/Miner.o

transactionsGenerator : ./src/TransactionsGenerator.cpp
	$(CPP) $(OPT) ./src/TransactionsGenerator.cpp -o ./objs/TransactionsGenerator.o

clean :
	rm -rf $(EXEC)
