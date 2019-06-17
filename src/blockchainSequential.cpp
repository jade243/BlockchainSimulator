#include <iostream>
#include "../headers/picosha2.h"
#include "Blockchain.cpp"
#include "TransactionsGenerator.cpp"
#include <experimental/random>

#define MIN 0
#define MAX (int)(pow(2, 32)-1)
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

void fillBlock(Block* block, vector<string> transactions) {

  for (int i=0; i<TX_PER_BLOCK; i++) {
    if (transactions.size() >= 1) {
      block->addTransaction(transactions.at(0));
      transactions.erase(transactions.begin());
    }
  }
}

int main()
{
    int nbTransactions = 100;
    int nbUsers = 10;
    double nbBlocks = floor(nbTransactions / TX_PER_BLOCK);

    TransactionsGenerator* sim = new TransactionsGenerator(nbUsers, nbTransactions);
    vector<string> transactions = sim->generateVectorTransactions();

    Blockchain blockchain;
    Block* block;

    for (int i=0; i<nbBlocks; i++) {

      //Create a new block
      block = new Block(blockchain.getHashPrevBlock(), blockchain.getTarget());

      //Build the merkle tree
      fillBlock(block, transactions);
      block->buildMerkleTree();

      //Mine and add the block to the block chain
      mine(block);
      blockchain.addBlock(block);
    }

    blockchain.displayChain();

    return 0;
}
