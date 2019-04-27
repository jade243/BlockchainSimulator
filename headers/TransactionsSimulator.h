#include <iostream>
#include <experimental/random>

using namespace std;

class TransactionsSimulator {

  private:
    string fileName;
    int nbUsers;
    int nbTransactions;
    int minTransfer = 1;
    int maxTransfer = 20;

    int chooseRandomUser(int previousChoice);

  public:
    TransactionsSimulator(string fileName, int nbUsers, int nbTransactions);
    void generateTransactions();

    void setMinTransfer(int min);
    void setMaxTransfer(int max);

};
