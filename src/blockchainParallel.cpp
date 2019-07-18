#include <mpi.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include "TransactionsGenerator.cpp"
#include "Miner.cpp"
#include "tests.cpp"

using namespace std;
typedef std::chrono::system_clock Clock;

string getStrTime(chrono::time_point<Clock> chrono) {
  time_t time_tmp = Clock::to_time_t(chrono);
  string str(ctime(&time_tmp));
  str.pop_back();
  return str;
}

int main(int argc, char **argv) {

  //===== Personal parameters =====
  bool isTesting = false;

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
  int difficulty = 3;

  //===== Initialising MPI ======
  int myRank, nProc; //Variables to know the rank of the current process and the number of processes
  //MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  MPI_Comm_size(MPI_COMM_WORLD, &nProc);
  MPI_Request request;

  //Data
  vector<string> transactions;
  Miner* miner = new Miner("", myRank);
  Block* block;

  //To communicate the transactions
  string stringComm;
  int tag = 0;

  //To measure the time
  chrono::time_point<std::chrono::system_clock> start, time;
  start = Clock::now(); //We start a chronometer
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
    time = Clock::now();
    stream << "[" << getStrTime(time) << "]" << " ";
    stream << "Initialization done in ";
    stream << chrono::duration_cast<chrono::milliseconds> (time-start).count() << " milliseconds." << endl;
  }

  MPI_Barrier(MPI_COMM_WORLD);
  //===== Now, all nodes are ready and are in the same state =====


  //TEST to send blocks between blocks
  if (isTesting) {
    if (myRank == 0) {cout << endl << "===== We test sending a block to all the network =====" << endl;}
    testSendingBlocks(miner, nbTransactionsByBlock, myRank, nProc, tag);
    MPI_Barrier(MPI_COMM_WORLD);

    if (myRank == 0) {cout << endl << "===== We test sending a blockchain to all the network =====" << endl;}
    testSendingBlockchain(miner, nbTransactionsByBlock, myRank, nProc, tag);
    MPI_Barrier(MPI_COMM_WORLD);

    if (myRank == 0) {cout << endl << "===== We test sending an empty blockchain to all the network =====" << endl;}
    testSendingEmptyBlockchain(miner, myRank, nProc, tag);
    MPI_Barrier(MPI_COMM_WORLD);

    if (myRank == 0) {cout << endl << "===== We test sending a block or a blockchain randomly =====" << endl;}
    testSendingBlockOrBlockchain(miner, nbTransactionsByBlock, myRank, nProc, tag);
    MPI_Barrier(MPI_COMM_WORLD);

  } else {
    bool finishBlock = false;
    bool isMining = false;

    //The miners will need to empty their memPool
    while (!miner->isEmpty()) {

      //Miners construct a block
      block = new Block(miner->getHashPrevBlock(), difficulty);
      block = miner->fillBlock(block, nbTransactionsByBlock);
      block->buildMerkleTree();

      finishBlock = false;
      isMining = false;

      while (!finishBlock) {

        //Miners try to mine their own block
        isMining = miner->mine(block);

        if (isMining) { //If they succeed in doing so, ...

          //They add the block to their longest blockchain
          miner->addBlock(block);

          //We write the logs
          time = Clock::now();
          stream << "[" << getStrTime(time) << "]" << " ";
          stream << "Block mined in ";
          stream << chrono::duration_cast<chrono::milliseconds> (time-start).count() << " milliseconds." << endl;

          //They broadcast the block to all the network
          string s = block->serialize();
          cout << "I'm proc " << myRank << " and I've sent a block " << block->getShortRep() << endl;
          for (int i=0; i<nProc; i++) {
            if (i != myRank) {
              MPI_Isend(&s[0], s.size()+1, MPI_CHAR, i, tag, MPI_COMM_WORLD, &request);
            }
          }

          finishBlock = true;

        }

        //If they haven't mined their block, they check communication over the network

        //They check if they have received a message
        MPI_Status status;
        int flag;
        MPI_Iprobe(MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &flag, &status);

        //If yes, we know it's a string so they extract it
        if (flag) {
          //We extract the length of the string
          int count;
          MPI_Get_count(&status, MPI_CHAR, &count);

          int senderRank = status.MPI_SOURCE;

          //and the string itself
          char buf [count];
          MPI_Irecv(&buf, count, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &request);
          std::string s = buf;

          int messageType = miner->getMessageType(s);

          if (messageType == 0) {       //The miner receives a blockchain
            cout << "I'm proc " << myRank << " and I've received a blockchain" << endl;
            miner->deserializeBlockchain(s);

            miner->resetBlock(block);
            finishBlock = true;

            miner->sortBlockchains();

          }
          else if (messageType == 1) {  //The miner receives a block
            Block* receivedBlock = new Block();
            receivedBlock->deserialize(s);

            cout << "I'm proc " << myRank << " and I've received a block " << receivedBlock->getShortRep() << endl;

            finishBlock = miner->handleReceivedBlock(receivedBlock, senderRank);
            if (finishBlock)
              miner->updateMemPool(receivedBlock, block);
          }
          else if (messageType == 2) {  //The miner receives a request for a blockchain
            Block* receivedBlock = new Block();
            receivedBlock->deserialize(s);

            int posBlockchain = miner->searchBlock(receivedBlock);
            if (posBlockchain != -1) {
              string s = miner->serializeBlockchain(posBlockchain);
              cout << "I've sent a blockchain to " << senderRank << endl;
              MPI_Isend(&s[0], s.size()+1, MPI_CHAR, senderRank, tag, MPI_COMM_WORLD, &request);
            }
          }
        }
      }

    }


    //===== There, all nodes have finished =====
    MPI_Barrier(MPI_COMM_WORLD);
    miner->printAllInfos(myRank);
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


  }





  MPI_Finalize();

}
