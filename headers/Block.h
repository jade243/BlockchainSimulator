#include <iostream>
#include "../src/MerkleTree.cpp"
#include "../src/BlockHeader.cpp"

using namespace std;

//To represent a block in the blockchain
class Block {

  private:
    Block* prev;                      //pointer on the previous block
    BlockHeader* blockHeader;         //block header
    int id = 0;
    string shortRep;                  //it's the 10 first bytes of the hash (to optimize visualization)
    vector<string> transactions;      //List of transactions

    string getString(stringstream& stream);

  public:
    //Constructor
    Block(string hashPrevBlock, string target);
    Block();

    //Some getters and setters
    Block* getPrev();
    void setPrev(Block* prev);
    BlockHeader* getBlockHeader();
    string getShortRep();
    int getId();
    void setId(int id);
    void computeId();

    //Methods to send and receive blocks
    string serialize();
    void deserialize(string s);

    //Manipulation related to transactions
    void addTransaction(string transaction);    //To add a transaction in the list
    void buildMerkleTree();                     //To compute the merkle tree and extract the merkle root in the block header

    //Some printing functions
    void printBlock();
    void printTransactions(int nbTransactions);
};
