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


  public:
    Miner();
    Miner(string name, int id, vector<string> memPool);
    void getTransactionsFromFile(string fileName);
    Block* fillBlock(Block* block, int nbTransactionsByBlock);
    void mine(Block* block);
    void addBlock(Block* block);
    bool isEmpty();

    int getLastID();

    void printInfos();

};
