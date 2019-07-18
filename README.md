
# BlockchainSimulator

C++ code to simulate a blockchain, focusing on the mining process and some Matlab/Octave scripts to analyze results produced by the blockchain's simulation and some probabilities for attacks against blockchains.

This project is divided in several parts :
- A sequential code, in C++, to simulate mining a list of transactions
- A paralled code, in C++ using MPI library, to simulate several miners working together to mine a list of transactions
- A Matlab/Octave code to analyze the logs produced by the miners
- A Matlab/Octave code to analyze probabilities of success for selfish mining

The larger part (C++ code) is located is `src` folder and the Matlab/Octave code is located in `scripts` folder.

Some experiments were performed at University of Geneva on the Baobab cluster.

If you need indications to run the code locally, please refer to the section at the end of this file.

### Tasks list:

- [x] Selfish mining
  	- [x] Simulate the finite state machine
- [ ] Version 2
	- [x] Broadcast transactions with MPI, without using files
	- [x] Serialize blocks to send them with MPI
  	- [x] Serialize blockchains to send them with MPI
  	- [x] Add the possibility to fork chains
	- [x] Implement the algorithm for the miners to broadcast mined blocks
	- [x] Improve the logs
  - [ ] Write Matlab script to analyze the new logs
  - [ ] Improve network structure
- [x] Version 1
	- [x] Modify the code so it's compiled and ran with MPI
	- [x] Synchronize the procs so they start with the same transactions
	- [x] Implement writing logs at the end of the process
	- [x] Test the code on baobab
	- [x] Implement Matlab/Octave code to analyze the logs
- [x] Version 0
	- [x] Modelize fake transactions (represented by strings)
	- [x] Modelize the main components: blocks, block headers, the blockchain itself
	- [x] Implement a function to mine a block with SHA-256
	- [x] Implement the algorithm so the miner mines severals blocks and add them in its blockchain

## Version 2

Algorithm to handle communication of blocks between miners :

When mining a block :

```
if (miner i mines a block)
    he broadcast his block to all the network
```

When receiving a block :

```
Miner i receives a block from miner j

if (the previous hash block matches the last block of the chain)
    the miner adds it at the end of his chain and starts mining on it

else if (the block is the last one of the chain)
    the miner forgets it and mine on his last block

else if (the block appears in the chain but not in the last position)
    the miner creates a forked chain and works on the first longest he received.

else if (the block doesn't appear in the chain but the previous hash block does)
    the miner creates a forked chain and works on the first longest he received.

else (the block and the previous hash block are unknown to the miner)
    the miner ask for the whole representation of the sender's chain and mines on the longest chain he received.
```


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

To run the code, we use the script `baobabScript.sh` in the following command : `sbatch baobabScript`.

This will use any available nodes on the cluster. However, if we want to use similar nodes (with the same performance), we can use : `sbatch --constraint=V4 baobabScript.sh`
