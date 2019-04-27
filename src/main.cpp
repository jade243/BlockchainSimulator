#include <iostream>
#include "../headers/picosha2.h"
#include "BlockHeader.cpp"
#include "Block.cpp"
#include "Blockchain.cpp"
#include "TransactionsGenerator.cpp"
#include <experimental/random>

#define MIN 0
#define MAX (int)(pow(2, 32)-1)
#define MEMPOOL "transactionsList.txt"
#define TX_PER_BLOCK 10

using namespace std;

//To mine a block
void mine(Block* block) {

  string target = block->getBlockHeader()->target2Hex();
  while (true) {
    //We get a random nonce each time to try to be lower than the target
    block->getBlockHeader()->setNonce(std::experimental::randint(MIN,  MAX));
    string hash = block->getBlockHeader()->hashOperation();
    //If we are lower, we're done
    if (hash <= target) {
      cout << "Block mined !!!" << endl;
      block->computeId();
      break;
    }
  }
}

void fillBlock(Block* block) {

  ofstream newMemPool;
  newMemPool.open("tmpMemPool.txt");
  ifstream memPool;
  memPool.open(MEMPOOL);

  string line;

  for (int i=0; i<TX_PER_BLOCK; i++) {
    if (getline(memPool, line))
      block->addTransaction(line);
  }

  while (getline(memPool, line))
    newMemPool << line << endl;

  memPool.close();
  newMemPool.close();
  remove(MEMPOOL);
  rename("tmpMemPool.txt", MEMPOOL);

}

int main()
{
    int nbTransactions = 100;
    int nbUsers = 10;
    double nbBlocks = floor(nbTransactions / TX_PER_BLOCK);

    TransactionsGenerator* sim = new TransactionsGenerator(MEMPOOL, nbUsers, nbTransactions);
    sim->generateTransactions();

    Blockchain blockchain;
    Block* block;

    for (int i=0; i<nbBlocks; i++) {

      //Create a new block
      block = new Block(blockchain.getHashPrevBlock(), blockchain.getTarget());

      //Build the merkle tree
      fillBlock(block);
      block->buildMerkleTree();

      //Mine and add the block to the block chain
      mine(block);
      blockchain.addBlock(block);
    }

    blockchain.displayChain();

    return 0;
}
