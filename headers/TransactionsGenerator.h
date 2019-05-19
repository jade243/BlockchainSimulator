#include <iostream>
#include <experimental/random>
#include <sstream>
#include <string>
#include <fstream>

using namespace std;

//Generate a file or a vector with random transactions
class TransactionsGenerator {

  private:
    int nbUsers;
    int nbTransactions;
    int minTransfer = 1;
    int maxTransfer = 20;

    int chooseRandomUser(int previousChoice);

  public:
    TransactionsGenerator(int nbUsers, int nbTransactions);
    void generateTransactions(string fileName);
    vector<string> generateVectorTransactions();

    void printTransactions(vector<string> transactions);

};
