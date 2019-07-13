#include "../headers/Miner.h"

//Constructors

Miner::Miner() {
  this->blockchains.push_back(new Blockchain());
}

Miner::Miner(string name, int id) {
  this->name = name;
  this->id = id;
  this->blockchains.push_back(new Blockchain());
}

//Manipulation methods
void Miner::getTransactionsFromFile(string fileName) {
  ifstream file;
  file.open (fileName);

  string line;
  if (file.is_open())
  {
    while (getline (file, line)) {
      this->memPool.push_back(line);
    }
    file.close();
  }
}

void Miner::createForkChain() {
  this->blockchains.push_back(new Blockchain());
}

void Miner::setTransactions(vector<string> transactions) {
  //this->memPool.assign(transactions.begin(), transactions.end());
  this->memPool = transactions;
}

Block* Miner::fillBlock(Block* block, int nbTransactionsByBlock) {

  block = new Block(this->blockchains.at(0)->getHashPrevBlock(), this->blockchains.at(0)->getTarget());

  for (int i=0; i<nbTransactionsByBlock; i++) {
    if (this->memPool.size() >= 1) {
      block->addTransaction(this->memPool.at(0));
      this->memPool.erase(this->memPool.begin());
    }
  }
  return block;
}

//To mine a block
bool Miner::mine(Block* block) {

  string target = block->getBlockHeader()->target2Hex();

  //We get a random nonce each time to try to be lower than the target
  block->getBlockHeader()->setNonce(std::experimental::randint(MIN,  MAX));
  string hash = block->getBlockHeader()->hashOperation();
  //If we are lower, we're done
  if (hash <= target) {
    block->computeId();
    return true;
  }
  return false;
}

void Miner::addBlock(Block* block) {
  this->blockchains.at(0)->addBlock(block);
}

bool Miner::isEmpty() {
  return this->memPool.empty();
}

//Displaying methods
void Miner::printInfos() {
  cout << "I'm the proc " << this->name << " with id " << this->id << ". My first 5 transactions are : "<< endl;
  if (this->memPool.size() >= 5) {
    for(size_t i=0; i<5; ++i)
      cout << this->memPool[i] << endl;
  } else {
    for(size_t i=0; i<this->memPool.size(); ++i)
      cout << this->memPool[i] << endl;
  }
}

void Miner::printBlockchain(int pos) {
  if ((static_cast<unsigned int>(pos) < this->blockchains.size())) {
    this->blockchains.at(pos)->displayShortRep();
  }
}

//Methods for serialization / deserialization to send objects
string Miner::serializeTransactions(vector<string> transactions) {
  stringstream stream;
  for(size_t i=0; i<transactions.size()-1; ++i)
      stream << transactions[i] << endl;
  stream << transactions[transactions.size()-1];
  return stream.str();
}


vector<string> Miner::deserializeTransactions(string s) {
  stringstream stream;
  stream.str(s);
  vector<string> transactions;

  while (!stream.eof()) {
    transactions.push_back(getString(stream));
  }
  return transactions;
}

string Miner::getString(stringstream& stream) {
  int size = 1024;
  char line[size];
  stream.getline(line, size);
  string line_tmp = line;
  return line_tmp;
}

void Miner::clearBlockchains() {
  this->blockchains.clear();
  this->blockchains.push_back(new Blockchain());
}

string Miner::serializeBlockchain(int pos) {
  if ((static_cast<unsigned int>(pos) < this->blockchains.size())) {
    return this->blockchains.at(pos)->serialize();
  }
  return NULL;
}

void Miner::deserializeBlockchain(string s) {
  this->createForkChain();
  this->blockchains.at(this->blockchains.size()-1)->deserialize(s);
}

bool Miner::isBlock(string s) {
  stringstream stream;
  stream.str(s);

  string line = getString(stream);
  return line == "1";

}
