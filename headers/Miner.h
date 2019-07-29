#include <iostream>
#include <string>
#include "../src/Blockchain.cpp"

#define MIN 0
#define MAX (int)(pow(2, 32)-1)

using namespace std;

//Handle operations for mining
class Miner {

  private:
    int id;                             //Miner's ID
    vector<string> memPool;             //List of transactions
    vector<Blockchain*> blockchains;    //Blockchains (sorted by length)

    string getString(stringstream& stream);


  public:
    //Constructors
    Miner();
    Miner(int id);

    //Getters and setters
    void getTransactionsFromFile(string fileName);
    void setTransactions(vector<string> transactions);
    Blockchain* getBlockchain(int pos);
    size_t getNbChains();
    string getHashPrevBlock();

    //Mempool maninpulation
    bool isEmpty();   //Check if the memPool is empty
    bool updateMemPool(Block* newBlock, Block* oldBlock);
    void resetBlock(Block* block); //Push the transactions back to the mempool

    //Blockchains manipulation
    void clearBlockchains();
    void addBlock(Block* block);
    void addBlock(Block* block, int pos);
    void createForkChain(Block* head);
    void sortBlockchains();
    int searchBlock(Block* block); //search block in all blockchains

    //Block manipulation
    Block* fillBlock(Block* block, int nbTransactionsByBlock);
    string getGenesisHash();
    bool mine(Block* block);

    //Serialization / deserialization of transactions
    string serializeTransactions(vector<string> transactions);
    vector<string> deserializeTransactions(string s);

    //Serialization / deserialization of blockchains
    void deserializeBlockchain(string s);
    string serializeBlockchain(int pos);

    //Communication
    bool handleReceivedBlock(Block* block, int senderRank);
    int getMessageType(string s);

    //Displaying methods
    void printInfos();
    void printAllInfos(int myRank);
    void printBlockchain(int pos);

};
