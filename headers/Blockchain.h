#include <iostream>
#include "../src/Block.cpp"

using namespace std;

//To represent the blockchain itself
class Blockchain {

  private:
    Block* head; //The head of the blockchain, i.e the last mined block
    string target = "1f0696f4"; //Target for any new block in the blockchain
    int blockID = -1;

    string getString(stringstream& stream);

  public:

    //Constructors
    Blockchain();
    Blockchain(Block* head);

    //Getters and setters
    string getHashPrevBlock();    //To get the last block's hash
    string getGenesisHash();
    string getTarget();
    int getLastID();

    //Blockchain manipulation
    void addBlock(Block* block);  //To add a block
    Block* consultLastBlock();    //To get the last block

    //Serialization methods
    string serialize();
    void deserialize(string s);

    //Printing methods
    void displayChain();          //To print the blockchain in the console
    void displayShortRep();       //Print the number of blocks in the chain and the hash of the last block






};
