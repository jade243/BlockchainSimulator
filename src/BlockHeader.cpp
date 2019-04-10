#include "../headers/BlockHeader.h"
#include "../headers/picosha2.h"
#include <experimental/random>

using namespace std;

//First constructor to generate an almost random BlockHeader (to do some test)
BlockHeader::BlockHeader() {
  long unsigned int start = 0, end = (long unsigned int)(pow(2, 32)-1) ;

  long unsigned int version_int = std::experimental::randint(start,  end);
  version = int2Hex(version_int);

  string prevBlock = "This was the previous block of the blockchain";
  picosha2::hash256_hex_string(prevBlock, hashPrevBlock);

  string merkleRoot = "This is the merkle root";
  picosha2::hash256_hex_string(merkleRoot, hashMerkleRoot);

  time_t time_int = std::time(0);
  time = int2Hex((int)(time_int));

  long unsigned int target_int = std::experimental::randint(start,  end);
  target = int2Hex(target_int);

  long unsigned int nonce_int = std::experimental::randint(start,  end);
  nonce = int2Hex(nonce_int);
}

//Second constructor to generate a new block header
//The version is random for now
BlockHeader::BlockHeader(string hashPrevBlock, string target) {
  long unsigned int start = 0, end = (long unsigned int)(pow(2, 32)-1) ;

  long unsigned int version_int = std::experimental::randint(start,  end);
  version = int2Hex(version_int);

  this->hashPrevBlock = hashPrevBlock;

  string merkleRoot = "This is the merkle root";
  picosha2::hash256_hex_string(merkleRoot, hashMerkleRoot);

  time_t time_int = std::time(0);
  time = int2Hex((int)(time_int));

  this->target = target;

  long unsigned int nonce_int = std::experimental::randint(start,  end);
  nonce = int2Hex(nonce_int);
}

//Function to convert a decimal number in hexadecimal
string BlockHeader::int2Hex(int number) {
  stringstream stream;
  stream << hex << number;
  string result( stream.str() );
  return result;
}

//To print the size of each field
void BlockHeader::printSizes() {
  //We use strings to represent hex numbers, so the size depends on the length
  //of each string
  cout << "Size of version\t\t" << (version.length()*4) << " bits" << endl;
  cout << "Size of hashPrevBlock\t" << (hashPrevBlock.length()*4) << " bits" << endl;
  cout << "Size of hashMerkleRoot\t" << (hashPrevBlock.length()*4) << " bits" << endl;
  cout << "Size of time\t\t" << (time.length()*4) << " bits" << endl;
  cout << "Size of target\t\t" << (target.length()*4) << " bits" << endl;
  cout << "Size of nonce\t\t" << (nonce.length()*4) << " bits" << endl;
}

//Getters and setters

string BlockHeader::getNonce() {
  return this->nonce;
}

void BlockHeader::setNonce(long unsigned int nonce) {
  this->nonce = int2Hex(nonce);
}

string BlockHeader::getTarget() {
  return this->target;
}

string BlockHeader::getHashPrevBlock() {
  return this->hashPrevBlock;
}

void BlockHeader::setTarget(string target) {
  this->target = target;
}

//Performs the hash operation on the block header
//The operation used is SHA256(SHA256(blockheader))
string BlockHeader::hashOperation() {
  string input = this->toString();

  string hash1;
  picosha2::hash256_hex_string(input, hash1);

  string hash2;
  picosha2::hash256_hex_string(hash1, hash2);

  return hash2;
}

//Transforms the target in the same format than an output of the hash function
string BlockHeader::target2Hex() {
  //The target field is composed of 4 bytes : 0xh1 h2 h3 h4 h5 h6 h7 h8
  //The first byte (h1 h2) represents the number of zeros (it's called the exponent)
  //Then, we replace the 3 first bytes of this serie of zeros by the 3 other
  //bytes (called the coefficient) of the target field : h3 h4 h5 h6 h7 h8 0 0 0 0 ...

  //We extract the exponent and the coefficient
  string exponent = target.substr(0, 2);
  string coeff = target.substr(2, target.length()-2);

  //We compute the number of zeros needed
  stringstream stream;
  int nbBytes;
  stream << exponent;
  stream >> hex >> nbBytes;

  //We construct the final target so it has a length of 64 to match the length
  //of the hash function
  string headTarget(64-nbBytes*2, '0');
  string finalTarget(nbBytes*2-coeff.length(), '0');
  finalTarget = headTarget + coeff + finalTarget;
  return finalTarget;
}

//To create one string with all the fields so we can hash the block header
string BlockHeader::toString() {
  stringstream stream;
  stream << version << hashPrevBlock << hashMerkleRoot << time << target << nonce;
  return stream.str();
}
