#include <iostream>

using namespace std;

//To represent the blockchain itself
class Blockchain {

  private:
    Block* head; //The head of the blockchain, i.e the last mined block
    string target = "1f0696f4"; //Target for any new block in the blockchain

  public:
    Blockchain();
    void addBlock(Block* block);  //To add a block
    Block* consultLastBlock();    //To get the last block
    void displayChain();          //To print the blockchain in the console
    string getHashPrevBlock();    //To get the last block's hash
    string getTarget();


};
