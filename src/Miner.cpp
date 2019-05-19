#include "../headers/Miner.h"

//Constructors

Miner::Miner() {}

Miner::Miner(string name, int id, vector<string> memPool) {
  this->name = name;
  this->id = id;
  this->memPool = memPool;
}

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
