#include "../headers/Blockchain.h"

Blockchain::Blockchain() {
  head = NULL;
}

//To add a block in the blockchain
void Blockchain::addBlock(Block* block) {
  if (head == NULL) {       //If it's the first block...
    head = block;
  }
  else {                    //If not, ...
    block->setPrev(head);
    head = block;
  }
}

//To get the last block
Block* Blockchain::consultLastBlock() {
  return this->head;
}

//To get the last block's hash
string Blockchain::getHashPrevBlock() {
  string hash;
  if (head == NULL) { //In case, the blockchain is empty, ...
    string block0 = "This is the first block";
    picosha2::hash256_hex_string(block0, hash);
  }
  else { //Otherwise, we return the last hash
    hash = head->getBlockHeader()->hashOperation();
  }
  return hash;
}

//To print the block chain in the console
void Blockchain::displayChain() {
  if (head != NULL) {
    string hashPrevBlock;
    Block* tmp = head;
    while (tmp != NULL) {
      tmp->printBlock();
      hashPrevBlock = tmp->getBlockHeader()->getHashPrevBlock();
      cout << "\t^" << "\t" << "prev : " << hashPrevBlock << endl <<
              "\t|" << "\t" << "target : " << tmp->getBlockHeader()->target2Hex() << endl <<
              "\t|" << endl <<
              "\t|" << endl;
      tmp = tmp->getPrev();
    }
    cout << "Block 0 : " << hashPrevBlock.substr(0, 10) << "..." << endl;
  }
  else {
    cout << "The chain is empty" << endl;
  }
}
