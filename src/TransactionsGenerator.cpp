#include "../headers/TransactionsGenerator.h"

//Constructor to set the main parameters
TransactionsGenerator::TransactionsGenerator(string fileName, int nbUsers, int nbTransactions) {
  this->fileName = fileName;
  this->nbUsers = nbUsers;
  this->nbTransactions = nbTransactions;
}

//Constructor to set the main parameters
TransactionsGenerator::TransactionsGenerator(int nbUsers, int nbTransactions) {
  this->nbUsers = nbUsers;
  this->nbTransactions = nbTransactions;
}

//Generate random transactions and save them to a file
void TransactionsGenerator::generateTransactions() {

  stringstream stream;

  //To compute amount for each user and initial amount needed
  int users[this->nbUsers] = {0};
  int initialAmount[this->nbUsers] = {1};

  //We generate transactions
  for (int i=0; i<this->nbTransactions-this->nbUsers; i++) {
    //We choose two different users
    int payer = chooseRandomUser(-1);
    int payee = chooseRandomUser(payer);
    //and a random amount
    int amount = std::experimental::randint(this->minTransfer,  this->maxTransfer);

    //We check if the payer has enough money, if not we increase his initial amount
    if (users[payer-1] < amount) {
      initialAmount[payer-1] += (amount - users[payer-1]);
      users[payer-1] += (amount - users[payer-1]);
    }
    users[payer-1] -= amount;
    users[payee-1] += amount;

    //We append the transaction to the stream
    stream << "User " << payer << " gives " << amount << " bitcoin(s) to " << payee << ".\n";
  }

  //We save the transactions to the file
  ofstream file;
  file.open (this->fileName);

  //We start by saving the initial amounts
  for (int i=0; i<this->nbUsers; i++) {
    file << "User " << i+1 << " starts with " << initialAmount[i] << " bitcoin(s).\n";
  }

  //and then the transactions
  file << stream.rdbuf();

  file.close();
}

//Generate random transactions and save them to a file
vector<string> TransactionsGenerator::generateVectorTransactions() {

  vector<string> transactions;
  stringstream stream;

  //To compute amount for each user and initial amount needed
  int users[this->nbUsers] = {0};
  int initialAmount[this->nbUsers] = {1};

  //We generate transactions
  for (int i=0; i<this->nbTransactions-this->nbUsers; i++) {
    //We choose two different users
    int payer = chooseRandomUser(-1);
    int payee = chooseRandomUser(payer);
    //and a random amount
    int amount = std::experimental::randint(this->minTransfer,  this->maxTransfer);

    //We check if the payer has enough money, if not we increase his initial amount
    if (users[payer-1] < amount) {
      initialAmount[payer-1] += (amount - users[payer-1]);
      users[payer-1] += (amount - users[payer-1]);
    }
    users[payer-1] -= amount;
    users[payee-1] += amount;

    //We add the transaction to the vector
    stream << "User " << payer << " gives " << amount << " bitcoin(s) to " << payee << ".\n";
    transactions.push_back(stream.str());
    stream.str("");
  }

  vector<string> coinbases;

  //We start by saving the initial amounts
  for (int i=0; i<this->nbUsers; i++) {
    stream << "User " << i+1 << " starts with " << initialAmount[i] << " bitcoin(s).\n";
    coinbases.push_back(stream.str());
    stream.str("");
  }

  coinbases.insert(coinbases.end(), transactions.begin(), transactions.end());
  return coinbases;
}

//Function to choose a different payee than the payer
int TransactionsGenerator::chooseRandomUser(int previousChoice) {
  int user = std::experimental::randint(1,  this->nbUsers);
  while (user == previousChoice) {
    user = std::experimental::randint(1,  this->nbUsers);
  }
  return user;
}

void TransactionsGenerator::printTransactions(vector<string> transactions) {
  for (auto&& transaction: transactions) {
    cout << transaction << endl;
  }
}
