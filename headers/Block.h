#include <iostream>
#include "../src/MerkleTree.cpp"

using namespace std;

//To represent a block in the blockchain
class Block {

  private:
    Block* prev;                //pointer on the previous block
    BlockHeader* blockHeader;   //block header
    string id;                  //it's the 10 first bytes of the hash (to optimize visualization)

    vector<string> transactions;//List of transactions

  public:
    //Constructor
    Block(string hashPrevBlock, string target);

    //Some getters and setters
    Block* getPrev();
    void setPrev(Block* prev);
    BlockHeader* getBlockHeader();

    //Manipulation related to transactions
    void addTransaction(string transaction);
    void buildMerkleTree();

    //Some printing functions
    void computeId();
    void printBlock();
    void printTransactions();
};
