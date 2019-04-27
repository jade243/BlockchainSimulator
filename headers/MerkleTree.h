#include <iostream>
#include "../headers/picosha2.h"

using namespace std;

class MerkleTree {

  private:
    static string hashOperation(string str); //Hash operation to build the merkle tree

  public:
    static string getMerkleRoot(vector<string> transactions); //Compute the merkle tree of the given transactions

};
