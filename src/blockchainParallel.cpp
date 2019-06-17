#include <mpi.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include "TransactionsGenerator.cpp"
#include "Miner.cpp"

using namespace std;
typedef std::chrono::system_clock Clock;

string getStrTime(chrono::time_point<Clock> chrono) {
  time_t time_tmp = Clock::to_time_t(chrono);
  string str(ctime(&time_tmp));
  str.pop_back();
  return str;
}

int main(int argc, char **argv) {

  //===== Recovery and initialization of parameters =====
  int nbTransactions = 0, nbTransactionsByBlock = 0;

  if (argc == 3) {
    nbTransactions = atoi(argv[1]);
    nbTransactionsByBlock = atoi(argv[2]);
  }
  else {
    std::cout << "Command is incomplete" << std::endl;
    exit(-1);
  }

  int nbBlocks = (int)(nbTransactions/nbTransactionsByBlock);
  int nbUsers = 10;

  //===== Initialising MPI ======
  int myRank, nProc; //Variables to know the rank of the current process and the number of processes
  //MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  MPI_Comm_size(MPI_COMM_WORLD, &nProc);

  //Data
  vector<string> transactions;
  Miner* miner = new Miner("", myRank);
  Block* block;

  //To communicate the transactions
  string stringComm;
  int tag = 0;

  //To measure the time
  chrono::time_point<std::chrono::system_clock> start, end;
  //To write infos in the file
  stringstream stream;

  //===== Proc 0 will prepare the data for all nodes
  if (myRank == 0) {
    //Generate the random transactions and save them in a file for others
    TransactionsGenerator* sim = new TransactionsGenerator(nbUsers, nbTransactions);
    transactions = sim->generateVectorTransactions();

    //We send the vector to the others procs
    stringComm = miner->serializeTransactions(transactions);
    for (int i=1; i<nProc; i++) {
      MPI_Send(&stringComm[0], stringComm.size()+1, MPI_CHAR, i, tag, MPI_COMM_WORLD);
    }
  } else {
    //We get the transactions sent by proc 0
    MPI_Status status;
    MPI_Probe(0, tag, MPI_COMM_WORLD, &status);

    int count;
    MPI_Get_count(&status, MPI_CHAR, &count);

    char buf [count];
    MPI_Recv(&buf, count, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
    string s = buf;

    transactions = miner->deserializeTransactions(s);
  }

  miner->setTransactions(transactions);


  MPI_Barrier(MPI_COMM_WORLD);
  if (myRank == 0) {
    cout << "Initialization stage done ! " << endl;
    cout << "Mining stage in process..." << endl;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  //===== Now, all nodes are ready and are in the same state =====
  while (!miner->isEmpty()) {

    start = Clock::now();
    block = miner->fillBlock(block, nbTransactionsByBlock);
    block->buildMerkleTree();

    miner->mine(block);
    miner->addBlock(block);
    //cout << "One block mined by " << myRank << " !" << endl;
    end = Clock::now();

    stream << "[" << getStrTime(end) << "]" << " ";
    stream << "Block " << block->getId() << " mined in ";
    stream << chrono::duration_cast<chrono::milliseconds> (end-start).count() << " milliseconds." << endl;
  }


  //===== There, all nodes have finished =====
  MPI_Barrier(MPI_COMM_WORLD);
  if (myRank == 0) {
    cout << "Mining stage done !" << endl;
    cout << "Now writing logs..." << endl;
  }
  MPI_Barrier(MPI_COMM_WORLD);

  auto now = Clock::now();
  std::time_t now_c = Clock::to_time_t(now);
  struct tm *parts = localtime(&now_c);

  stringstream fileName;
  fileName << "logs/";
  fileName << "log_" << parts->tm_year + 1900 << "_" << parts->tm_mon + 1 << "_" << parts->tm_mday;

  ofstream file;

  //Proc 0 will also write a file with the general informations
  if (myRank == 0) {

    //General infos
    stringstream streamGeneral;
    streamGeneral << "[nbTransactions] " << nbTransactions << endl;
    streamGeneral << "[nbBlocks] " << nbBlocks << endl;
    streamGeneral << "[nbProc] " << nProc << endl;

    //Writing them in a file
    stringstream fileName2;
    fileName2 << fileName.str() << ".txt";
    file.open (fileName2.str());
    file << streamGeneral.rdbuf();
    file.close();
  }

  //Writing personal data
  fileName << "_" << myRank << ".txt";
  file.open (fileName.str());
  file << stream.rdbuf();
  file.close();

  MPI_Finalize();

}
