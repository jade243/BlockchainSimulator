#include "../headers/Block.h"

//Constructor
Block::Block(string hashPrevBlock, int difficulty) {
  prev = NULL;
  blockHeader = new BlockHeader(hashPrevBlock, createTarget(difficulty));
}

Block::Block(string hashPrevBlock, string target) {
  prev = NULL;
  blockHeader = new BlockHeader(hashPrevBlock, target);
}

Block::Block() {
  prev = NULL;
}

string Block::createTarget(int difficulty) {
  string bits = "0696f4";

  int decimalExponent = 64 - 6 - difficulty;
  stringstream ss;
  ss << hex << decimalExponent;
  string exponent ( ss.str() );

  return exponent + bits;
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

vector<string> Block::getTransactions() {
  return this->transactions;
}

string Block::getShortRep() {
  return this->shortRep;
}

int Block::getId() {
  return this->id;
}

void Block::setId(int id) {
  this->id = id;
}

void Block::computeId() {
  //We extract the 10 first bytes of the bloch header's hash
  this->shortRep = blockHeader->hashOperation().substr(0, 10);
}

string Block::getHash() {
  return blockHeader->hashOperation();
}


//Methods to send and receive blocks
string Block::serialize() {
  stringstream stream;
  stream << true << endl;
  stream << blockHeader->getVersion() << endl;
  stream << blockHeader->getHashPrevBlock() << endl;
  stream << blockHeader->getHashMerkleRoot() << endl;
  stream << blockHeader->getTime() << endl;
  stream << blockHeader->getTarget() << endl;
  stream << blockHeader->getNonce() << endl;
  stream << id << endl;
  for(size_t i=0; i<this->transactions.size()-1; ++i)
      stream << this->transactions[i] << endl;
  stream << this->transactions[this->transactions.size()-1];
  return stream.str();
}

string Block::getString(stringstream& stream) {
  int size = 1024;
  char line[size];
  stream.getline(line, size);
  string line_tmp = line;
  return line_tmp;
}

void Block::deserialize(string s) {

  stringstream stream;
  stream.str(s);

  string messageType = getString(stream);
  if (messageType == "1" || messageType == "2") {
    string version = getString(stream);
    string hashPrevBlock = getString(stream);
    string hashMerkleRoot = getString(stream);
    string time = getString(stream);
    string target = getString(stream);
    string nonce = getString(stream);
    BlockHeader* blockHeader = new BlockHeader(version, hashPrevBlock, hashMerkleRoot, time, target, nonce);
    this->blockHeader = blockHeader;

    this->id = stoi(getString(stream));

    transactions.clear();
    while (!stream.eof()) {
      this->transactions.push_back(getString(stream));
    }

    this->computeId();
  } else {
    cout << "The string doesn't start by 1 or 2" << endl;
  }
}

//Manipulation related to transactions
void Block::addTransaction(string transaction) {
  this->transactions.push_back(transaction);
}

void Block::buildMerkleTree() {
  this->getBlockHeader()->setHashMerkleRoot(MerkleTree::getMerkleRoot(this->transactions));
}

//Some printing functions
void Block::printBlock() {
  cout << "Block id : " << this->shortRep << "..." << endl;
}

void Block::printTransactions(int nbTransactions) {
  cout << "There are " << this->transactions.size() << " transactions." << endl;
  if (static_cast<unsigned int>(nbTransactions) <= this->transactions.size()) {
    for (int i=0; i<nbTransactions; i++) {
      cout << this->transactions.at(i) << endl;
    }
  } else {
    for (unsigned int i=0; i<this->transactions.size(); i++) {
      cout << this->transactions.at(i) << endl;
    }
  }
}
