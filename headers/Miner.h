#include <iostream>
#include <string>
#include "../src/Blockchain.cpp"

#define MIN 0
#define MAX (int)(pow(2, 32)-1)

using namespace std;

//Handle operations for mining
class Miner {

  private:
    string name;
    int id;
    vector<string> memPool;
    Blockchain* blockchain = new Blockchain();

    string getString(stringstream& stream);


  public:
    Miner();
    Miner(string name, int id);
    void getTransactionsFromFile(string fileName);
    void setTransactions(vector<string> transactions);
    Block* fillBlock(Block* block, int nbTransactionsByBlock);
    bool mine(Block* block);
    void addBlock(Block* block);
    bool isEmpty();

    string serializeTransactions(vector<string> transactions);
    vector<string> deserializeTransactions(string s);

    int getLastID();

    void printInfos();

};
