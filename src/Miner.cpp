#include "../headers/Miner.h"

//Constructors
Miner::Miner() {
  this->blockchains.push_back(new Blockchain());
}

Miner::Miner(int id) {
  this->id = id;
  this->blockchains.push_back(new Blockchain());
}

//Getters and setters
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

void Miner::setTransactions(vector<string> transactions) {
  //this->memPool.assign(transactions.begin(), transactions.end());
  this->memPool = transactions;
}

Blockchain* Miner::getBlockchain(int pos) {
  return this->blockchains.at(pos);
}

size_t Miner::getNbChains() {
  return this->blockchains.size();
}

string Miner::getHashPrevBlock() {
  return this->blockchains.at(0)->getHashPrevBlock();
}

//Mempool maninpulation
bool Miner::isEmpty() {
  return this->memPool.empty();
}

bool Miner::updateMemPool(Block* newBlock, Block* oldBlock) {
  //We push front the transactions of the old block
  for(auto const& transaction: oldBlock->getTransactions()) {
    memPool.insert(memPool.begin(), transaction);
  }

  vector<string>::iterator it;

  for (auto const& transaction: newBlock->getTransactions()) {
    it = find(memPool.begin(), memPool.end(), transaction);

    if (it != memPool.end()) {
      int index = distance(memPool.begin(), it);
      memPool.erase(memPool.begin()+index);
    }
    else {
      cout << "Element not found in myvector" << endl;
      return false;
    }
  }
  return true;

}

void Miner::resetBlock(Block* block) {
  //We push front the transactions of the old block
  for(auto const& transaction: block->getTransactions()) {
    memPool.insert(memPool.begin(), transaction);
  }
}

//Blockchains manipulation
void Miner::clearBlockchains() {
  this->blockchains.clear();
  this->blockchains.push_back(new Blockchain());
}

void Miner::addBlock(Block* block) {
  this->blockchains.at(0)->addBlock(block);
}

void Miner::addBlock(Block* block, int pos) {
  this->blockchains.at(pos)->addBlock(block);
}

void Miner::createForkChain(Block* head) {
  this->blockchains.push_back(new Blockchain(head));
}

static bool compSizes(Blockchain* a, Blockchain* b) {
  return a->getLastID() < b->getLastID();
}

void Miner::sortBlockchains() {
  sort(blockchains.begin(), blockchains.end(), compSizes);
}

int Miner::searchBlock(Block* block) {
  string hash = block->getHash();

  for (size_t i=0; i<blockchains.size(); i++) {

    Block* tmp = blockchains.at(i)->consultLastBlock();

    while (tmp != NULL) {

      if (hash.compare(tmp->getHash()) == 0) {
        //We found the block
        return i;
      }

      tmp = tmp->getPrev();

    }
  }
  return -1;
}

//Block manipulation
Block* Miner::fillBlock(Block* block, int nbTransactionsByBlock) {
  for (int i=0; i<nbTransactionsByBlock; i++) {
    if (this->memPool.size() >= 1) {
      block->addTransaction(this->memPool.at(0));
      this->memPool.erase(this->memPool.begin());
    }
  }
  return block;
}

string Miner::getGenesisHash() {
  return this->blockchains.at(0)->getGenesisHash();
}

//To mine a block
bool Miner::mine(Block* block) {

  string target = block->getBlockHeader()->target2Hex();

  //We get a random nonce each time to try to be lower than the target
  block->getBlockHeader()->setNonce(std::experimental::randint(MIN,  MAX));
  string hash = block->getBlockHeader()->hashOperation();
  //If we are lower, we're done
  if (hash <= target) {
    return true;
  }
  return false;
}

//Serialization / deserialization of transactions
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


//Serialization / deserialization of blockchains
string Miner::serializeBlockchain(int pos) {
  if ((static_cast<unsigned int>(pos) < this->blockchains.size())) {
    return this->blockchains.at(pos)->serialize();
  }
  return NULL;
}

void Miner::deserializeBlockchain(string s) {
  this->createForkChain(NULL);
  this->blockchains.at(this->blockchains.size()-1)->deserialize(s);
}

//Communication
//Methods when receiving a block
bool Miner::handleReceivedBlock(Block* block, int senderRank) {
  //We extract the hash of the previous block from which the block was built
  string hashPrev = block->getBlockHeader()->getHashPrevBlock();
  string hash = block->getHash();

  int tag = 0;
  MPI_Request request;

  bool newBlockToLongestChain = false;

  //cout << "Pass 2" << endl;

  bool foundBlock = false;
  //We check if it's part of one of our fork chains
  for (size_t i=0; i<blockchains.size(); i++) {

    Block* tmp = blockchains.at(i)->consultLastBlock();

    //We check if it's the next block (we won't have to create forks then)
    if (tmp == NULL || hashPrev.compare(tmp->getHash()) == 0) {
      //cout << "I add a block in the blockchain n°" << i << endl;
      addBlock(block, i);
      //updateMemPool(block, oldBlock);
      if (i == 0)
        newBlockToLongestChain = true;
      foundBlock = true;
    }
    //We check if it's the same block than the last
    else if (tmp != NULL && hash.compare(tmp->getHash()) == 0) {
      //cout << "I know this block already" << endl;
      foundBlock = true;
    }
    //We check if it's part of the chain
    else {
      //cout << "Pass 3" << endl;
      while (tmp != NULL) {
        tmp = tmp->getPrev();

        if (hashPrev.compare(tmp->getHash()) == 0) {    //We check if it's the next block
          //cout << "I will fork a chain" << endl;
          createForkChain(tmp);
          addBlock(block, getNbChains());

          //We send back our version of the blockchain
          string s = serializeBlockchain(i);
          cout << "I've sent a blockchain to " << senderRank << endl;
          MPI_Isend(&s[0], s.size()+1, MPI_CHAR, senderRank, tag, MPI_COMM_WORLD, &request);

          foundBlock = true;
          break;
        }
        else if (hash.compare(tmp->getHash()) == 0) {   //We check if it's the same block
          string s = serializeBlockchain(i);
          cout << "I've sent a blockchain to " << senderRank << endl;
          MPI_Isend(&s[0], s.size()+1, MPI_CHAR, senderRank, tag, MPI_COMM_WORLD, &request);

          foundBlock = true;
          break;
        }
      }

    }
  }

  if (!foundBlock) {
    if (hashPrev.compare(getGenesisHash()) == 0) {
      //cout << "Created new blockchain from 0" << endl;
      createForkChain(block);
    }
    else {
      stringstream stream;
      stream << 2 << endl;

      string s = block->serialize();
      stream << s << endl;

      string toSend = stream.str();
      cout << "I've sent a request for a blockchain to " << senderRank << endl;
      MPI_Isend(&toSend[0], toSend.size()+1, MPI_CHAR, senderRank, tag, MPI_COMM_WORLD, &request);
    }
  }

  Blockchain* oldBegin = blockchains.at(0);
  sort(blockchains.begin(), blockchains.end(), compSizes);

  return (oldBegin != blockchains.at(0) || newBlockToLongestChain);
}

int Miner::getMessageType(string s) {
  stringstream stream;
  stream.str(s);

  string line = getString(stream);
  return stoi(line);
}

//Displaying methods
void Miner::printInfos() {
  cout << "I'm the proc " << this->id << ". My first 5 transactions are : "<< endl;
  if (this->memPool.size() >= 5) {
    for(size_t i=0; i<5; ++i)
      cout << this->memPool[i] << endl;
  } else {
    for(size_t i=0; i<this->memPool.size(); ++i)
      cout << this->memPool[i] << endl;
  }
}

void Miner::printAllInfos(int myRank) {
  cout << "The miner " << myRank << " has " << blockchains.size() << " forks." << endl;
  for (size_t i=0; i<blockchains.size(); i++) {
    cout << "Blockchain n°" << i << endl;
    blockchains.at(i)->displayShortRep();
    cout << endl << endl;
  }
}

void Miner::printBlockchain(int pos) {
  if ((static_cast<unsigned int>(pos) < this->blockchains.size())) {
    this->blockchains.at(pos)->displayShortRep();
  }
}

//Private methods
string Miner::getString(stringstream& stream) {
  int size = 1024;
  char line[size];
  stream.getline(line, size);
  string line_tmp = line;
  return line_tmp;
}
