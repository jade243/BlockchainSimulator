#include <iostream>
#include <string>
#include "../src/Blockchain.cpp"

using namespace std;

//Handle operations for mining
class Miner {

  private:
    string name;
    int id;
    vector<string> memPool;
    Blockchain blockchain;


  public:
    Miner();
    Miner(string name, int id, vector<string> memPool);
    void getTransactionsFromFile(string fileName);

    void printInfos();

};
