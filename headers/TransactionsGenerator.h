#include <iostream>
#include <experimental/random>
#include <sstream>
#include <string>
#include <fstream>

using namespace std;

//Generate a file with random transactions
class TransactionsGenerator {

  private:
    string fileName;
    int nbUsers;
    int nbTransactions;
    int minTransfer = 1;
    int maxTransfer = 20;

    int chooseRandomUser(int previousChoice);

  public:
    TransactionsGenerator(string fileName, int nbUsers, int nbTransactions);
    TransactionsGenerator(int nbUsers, int nbTransactions);
    void generateTransactions();
    vector<string> generateVectorTransactions();
    void printTransactions(vector<string> transactions);

};
