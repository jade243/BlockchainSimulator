# BlockchainSimulator

C++ code to simulate a blockchain, focusing on the mining process.

If you need indications to run the code locally, please refer to the section at the end of this file.

TODO :

* Start version 2

## Version 1

In this version, we are using MPI to parallelize the code. When the program starts, the processus 0 initiliaze the data (generates random transactions and save them in a file). Then, all nodes try to mine the transactions by blocks on their own and they write a log file with all interesting informations about the execution.

In this version, all nodes have their own representation of the blockchain so in the next version, we'll add features to synchronize the blockchain.

![MiningParallelizedExample](../master/figures/MiningParallelizedExample.png)

![Log](../master/figures/logs.png)

In the folder scripts, there are scripts, written for Matlab or Octave, to visualize and analyze the data. For example time_graph.m :

![TimeGraph](../master/figures/time_graph.png)

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

Then, you need to compile and run the code with the following commands :

```
make
mpirun -np 10 ./blockchainParallel 100 10
```

(MPI run command : `mpirun -np <nbProcs> ./blockchainParallel <nbTransactions> <nbBlocks>`)

## Running the program on baobab cluster

To truly parallelize the code, the computations are performed at University of Geneva on the Baobab cluster. To run the code, we use the script `baobabScript.sh` in the following command : `sbatch baobabScript`.

This will use any available nodes on the cluster. However, if we want to use similar nodes (with the same performance), we can use : `sbatch --constraint=V4 baobabScript.sh`
