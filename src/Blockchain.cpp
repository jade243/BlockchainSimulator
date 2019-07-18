#include "../headers/Blockchain.h"

//Constructors
Blockchain::Blockchain() {
  this->head = NULL;
}

Blockchain::Blockchain(Block* head) {
  this->head = head;
}

//Getters and setters
//To get the last block's hash
string Blockchain::getHashPrevBlock() {
  string hash;
  if (head == NULL) { //In case, the blockchain is empty, ...
    hash = getGenesisHash();
  }
  else { //Otherwise, we return the last hash
    hash = head->getBlockHeader()->hashOperation();
  }
  return hash;
}

string Blockchain::getGenesisHash() {
  string hash;
  string block0 = "This is the first block";
  picosha2::hash256_hex_string(block0, hash);
  return hash;
}

//To get the actual target
string Blockchain::getTarget() {
  return this->target;
}

int Blockchain::getLastID() {
  return this->blockID;
}

//Blockchain manipulation
//To add a block in the blockchain
void Blockchain::addBlock(Block* block) {
  this->blockID += 1;
  block->setId(this->blockID+1);
  if (head == NULL) {       //If it's the first block...
    block->setPrev(NULL);
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

//Serialization Methods
string Blockchain::serialize() {
  stringstream stream;
  stream << false << endl; //To indicate we send the whole chain

  if (head != NULL) {
    Block* tmp = head;

    while (tmp != NULL) {
      stream << "new" << endl;
      stream << tmp->serialize() << endl;
      tmp = tmp->getPrev();
    }
  } else {
    stream << -1 << endl; //To indicate the chain is empty
  }
  return stream.str();
}

string Blockchain::getString(stringstream& stream) {
  int size = 1024;
  char line[size];
  stream.getline(line, size);
  string line_tmp = line;
  return line_tmp;
}

void Blockchain::deserialize(string s) {
  stringstream stream;
  stream.str(s);
  head = NULL;
  blockID = -1;

  stringstream stream_tmp;

  string line = getString(stream);
  if (line == "0") {
    line = getString(stream); // We remove the first "new"
    Block* block = new Block();

    if (line != "-1") {
      while (!stream.eof()) {
        string line = getString(stream);
        stream_tmp.str("");

        while (line != "new" && !stream.eof()) {
          stream_tmp << line << endl;
          line = getString(stream);
        }

        block = new Block();
        block->deserialize(stream_tmp.str());
        this->addBlock(block);
      }
    }
  }
}


//Printing methods
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

void Blockchain::displayShortRep() {
  if (head != NULL) {
    int count = 0;
    Block* tmp = head;
    while (tmp != NULL) {
      count += 1;
      tmp = tmp->getPrev();
    }
    cout << "There are " << count << " blocks in the chain and the first one is " << head->getShortRep() << endl;

  } else {
    cout << "The blockchain is empty" << endl;
  }
}
