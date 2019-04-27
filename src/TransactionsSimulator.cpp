#include "../headers/TransactionsSimulator.h"

TransactionsSimulator::TransactionsSimulator(string fileName, int nbUsers, int nbTransactions) {
  this->fileName = fileName;
  this->nbUsers = nbUsers;
  this->nbTransactions = nbTransactions;
}

int TransactionsSimulator::chooseRandomUser(int previousChoice) {
  int user = std::experimental::randint(1,  this->nbUsers);
  while (user == previousChoice) {
    user = std::experimental::randint(1,  this->nbUsers);
  }
  return user;
}

void TransactionsSimulator::generateTransactions() {

  stringstream stream;
  int users[this->nbUsers] = {0};
  int initialAmount[this->nbUsers] = {0};

  for (int i=0; i<this->nbTransactions-this->nbUsers; i++) {
    int payer = chooseRandomUser(-1);
    int payee = chooseRandomUser(payer);
    int amount = std::experimental::randint(this->minTransfer,  this->maxTransfer);

    if (users[payer-1] < amount) {
      initialAmount[payer-1] += (amount - users[payer-1]);
      users[payer-1] += (amount - users[payer-1]);
    }
    users[payer-1] -= amount;
    users[payee-1] += amount;

    stream << "User " << payer << " gives " << amount << " bitcoin(s) to " << payee << ".\n";
  }

  ofstream file;
  file.open (this->fileName);
  for (int i=0; i<this->nbUsers; i++) {
    if (initialAmount[i] != 0) 
      file << "User " << i+1 << " starts with " << initialAmount[i] << " bitcoin(s).\n";
  }
  file << stream.rdbuf();

  file.close();
}


void TransactionsSimulator::setMinTransfer(int min) {
  this->minTransfer = min;
}

void TransactionsSimulator::setMaxTransfer(int max) {
  this->maxTransfer = max;
}
