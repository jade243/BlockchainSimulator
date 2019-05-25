#include "../headers/MerkleTree.h"

string MerkleTree::getMerkleRoot(vector<string> transactions) {
  //Vector which will contain the hashes to build the merkle root
  vector<string> hashes;
  vector<string> tmpHashes;

  //First, we push the hashes of the transactions
  for (unsigned int i=0; i<transactions.size(); i++) {
    hashes.push_back(hashOperation(transactions.at(i)));
  }

  //We compute the tree until we get one hash (the merkle root)
  while (hashes.size() != 1) {
    unsigned int i = 0;
    while (i+1 < hashes.size()) {
      tmpHashes.push_back(hashOperation(hashes.at(i) + hashes.at(i+1)));
      i = i+2;
    }
    if (hashes.size() % 2 != 0) {
      tmpHashes.push_back(hashes.at(i));
    }
    hashes.swap(tmpHashes);
    tmpHashes.clear();
  }

  //We return the merkle root
  return hashes.at(0);
}

//Performs the hash operation (same than mining)
//The operation used is SHA256(SHA256(blockheader))
string MerkleTree::hashOperation(string str) {
  string hash1;
  picosha2::hash256_hex_string(str, hash1);

  string hash2;
  picosha2::hash256_hex_string(hash1, hash2);

  return hash2;
}
