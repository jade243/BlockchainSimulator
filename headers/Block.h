#include <iostream>
#include "../src/MerkleTree.cpp"
#include "../src/BlockHeader.cpp"

using namespace std;

//To represent a block in the blockchain
class Block {

  private:
    int id = 0;                       //Block ID to locate it in the blockchain
    Block* prev;                      //Pointer on the previous block
    BlockHeader* blockHeader;         //Block header
    vector<string> transactions;      //List of transactions

    string createTarget(int difficulty);
    string getString(stringstream& stream);

  public:
    //Constructor
    Block(string hashPrevBlock, int difficulty);
    Block(string hashPrevBlock, string target);
    Block();

    //Some getters and setters
    Block* getPrev();
    void setPrev(Block* prev);
    BlockHeader* getBlockHeader();
    vector<string> getTransactions();
    int getId();
    void setId(int id);
    string getShortRep(); //The first 10 blocks after the zeros of the block hash       
    string getHash();   //The block hash

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
