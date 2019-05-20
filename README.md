# BlockchainSimulator

C++ code to simulate a blockchain, focusing on the mining process.

If you need indications to run the code locally, please refer to the section at the end of this file.

TODO :

* Implement writing the logs for the miners
* Start version 2

## Version 1

In this version, we are using MPI to parallelize the code. When the program starts, the processus 0 initiliaze the data (generates random transactions and save them in a file). Then, all nodes try to mine the transactions by blocks on their own and they write a log file with all interesting informations about the execution.

In this version, all nodes have their own representation of the blockchain so in the next version, we'll add features to synchronize the blockchain.

![MiningParallelizedExample](../master/figures/MiningParallelizedExample.png)

## Version 0

This is a first draft of the project : a sequential code where we modelize mining a few blocks with random transactions.

In the next version, we'll parallelize this code.

Here an example of 5-blocks mining :

![MiningExample](../master/figures/MiningExample.png)

Transactions are represented in a simplified way, they are generated automatically and saved in a file :

![TransactionsList](../master/figures/TransactionsList.png)

## Running the program locally

To compile the code, you can the makefile and the command : `make`. It will compile for both parallel and sequential codes.

### Sequential code

To execute the program, you can use the following command (for Ubuntu):

```
./blockchainSequential
```

### Parallelized code

First, you need a MPI compiler

* On Ubuntu 16+, you can use `sudo apt install mpi-default-dev build-essential`
* On windows, have a look to Microsoft MPI
* Mac OS: Open-mpi or Mpich

Then, you need to compile the code and run the code with the following commands :

```
make
mpirun -np 10 ./blockchainParallel 100 10
```

(MPI run command : `mpirun -np <nbProcs> ./blockchainParallel <nbTransactions> <nbBlocks>`)
