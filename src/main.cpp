#include <iostream>
#include "../headers/picosha2.h"
#include "BlockHeader.cpp"
#include "Block.cpp"
#include "Blockchain.cpp"
#include <experimental/random>

#define MIN 0
#define MAX (int)(pow(2, 32)-1)

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

int main()
{
    //We define the target (for now it stays the same during all the execution)
    //and the number of blocks we want to generate
    string target = "1f0696f4";
    int nbBlocks = 5;

    //We create the blockchain
    Blockchain blockchain;
    Block* block;

    //We mine the blocks and insert them in the blockchain
    for (int i=0; i<nbBlocks; i++) {
      block = new Block(blockchain.getHashPrevBlock(), target);
      mine(block);
      blockchain.addBlock(block);
    }

    //We display the blockchain
    blockchain.displayChain();

    return 0;
}
