#include "../headers/Block.h"

//Constructors
Block::Block() {
  prev = NULL;
}

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

//Some printing functions
void Block::computeId() {
  //We extract the 10 first bytes of the bloch header's hash
  id = blockHeader->hashOperation().substr(0, 10);
}

void Block::printBlock() {
  cout << "Block id : " << this->id << "..." << endl;
}
