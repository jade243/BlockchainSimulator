#include <iostream>
#include <ctime>

using namespace std;

//To represent a block header
class BlockHeader {


  private:

    //Attributes
    string version = "20000000";  //Version of the block (from Bloc #573513 of Bitcoin blockchain)
    string hashPrevBlock;         //hash of the previous block
    string hashMerkleRoot;        //merkle root
    string time;                  //time
    string target;                //expected target for the hash
    string nonce;                 //random number

    //Methods
    string int2Hex(int number);

  public:
    BlockHeader(string hashPrevBlock, string target);

    //Some getters and setters
    string getNonce();
    void setNonce(long unsigned int nonce);
    string getTarget();
    string getHashPrevBlock();
    void setTarget(string target);
    void setHashMerkleRoot(string merkleRoot);

    string hashOperation();   //To perform the hash operation of the block header
    string toString();        //To create one string with all the fields

    string target2Hex();      //To transform the target

    //Some printing functions
    void printSizes();
    void printBlockHeader();


};
