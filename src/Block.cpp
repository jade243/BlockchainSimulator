#include "../headers/Block.h"

//Constructor
Block::Block(string hashPrevBlock, string target) {
  prev = NULL;
  blockHeader = new BlockHeader(hashPrevBlock, target);
}

//Some getters and setters
Block* Block::getPrev() {
  return this->prev;
}

void Block::setPrev(Block* prev) {
  this->prev = prev;
}

BlockHeader* Block::getBlockHeader() {
  return this->blockHeader;
}

//Manipulation related to transactions
void Block::addTransaction(string transaction) {
  this->transactions.push_back(transaction);
}

void Block::buildMerkleTree() {
  this->getBlockHeader()->setHashMerkleRoot(MerkleTree::getMerkleRoot(this->transactions));
}

//Some printing functions
void Block::computeId() {
  //We extract the 10 first bytes of the bloch header's hash
  id = blockHeader->hashOperation().substr(0, 10);
}

void Block::printBlock() {
  cout << "Block id : " << this->id << "..." << endl;
}

void Block::printTransactions() {
  cout << "There are " << this->transactions.size() << " transactions." << endl;
  for (int i=0; i<this->transactions.size(); i++) {
    cout << this->transactions.at(i) << endl;
  }
}
