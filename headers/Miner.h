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
    vector<Blockchain*> blockchains;
    //Blockchain* blockchain = new Blockchain();

    string getString(stringstream& stream);


  public:
    Miner();
    Miner(string name, int id);
    void getTransactionsFromFile(string fileName);
    void setTransactions(vector<string> transactions);
    Block* fillBlock(Block* block, int nbTransactionsByBlock);
    bool mine(Block* block);
    void addBlock(Block* block);
    void addBlock(Block* block, int pos);
    bool isEmpty();
    void clearBlockchains();

    string getGenesisHash();
    bool updateMemPool(Block* newBlock, Block* oldBlock);

    void createForkChain(Block* head);
    int lastIndex();
    string serializeTransactions(vector<string> transactions);
    vector<string> deserializeTransactions(string s);

    void deserializeBlockchain(string s);
    string serializeBlockchain(int pos);

    bool handleReceivedBlock(Block* block);
    string getHashPrevBlock();

    int getMessageType(string s);

    int getLastID();

    void printInfos();
    void printAllInfos(int myRank);
    void printBlockchain(int pos);

};
