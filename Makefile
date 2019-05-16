EXEC = blockchain
CC = mpiCC

all: $(EXEC)

blockchain : ./src/blockchainParallelize.cpp
	$(CC) ./src/blockchainParallelize.cpp -o blockchain -std=c++14


clean :
	rm -rf $(EXEC)
