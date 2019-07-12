#include <experimental/random>

void testSendingBlocks(Miner* miner, int nbTransactionsByBlock, int myRank, int nProc, int tag) {

    Block* block = new Block();
    MPI_Request request;

    bool isMining = false;
    block = miner->fillBlock(block, nbTransactionsByBlock);
    block->buildMerkleTree();

    int sender;
    if (myRank == 0) {
      sender = std::experimental::randint(0,  nProc-1);
      cout << "We randomly choose a sender : Proc n°" << sender << endl;
    }
    MPI_Bcast(&sender, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    while(true) {
      if (myRank == sender) {
        isMining = miner->mine(block);
      }

      if (isMining == true) {
        string s = block->serialize();
        cout << "I'm proc " << myRank << " and I've sent a block" << endl;
        block->printTransactions(1);
        cout << endl;

        for (int i=0; i<nProc; i++) {
         if (i != myRank) {
           MPI_Isend(&s[0], s.size()+1, MPI_CHAR, i, tag, MPI_COMM_WORLD, &request);
         }
        }
        break;
      } else {
        //They check if they have received a message
        MPI_Status status;
        int flag;
        MPI_Iprobe(MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &flag, &status);

        //If yes, we know it's a string so they extract it
        if (flag) {
          //We extract the length of the string
          int count;
          MPI_Get_count(&status, MPI_CHAR, &count);

          //and the string itself
          char buf [count];
          MPI_Irecv(&buf, count, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &request);
          std::string s = buf;

          block->deserialize(s);

          cout << "I'm proc " << myRank << " and I've received a block" << endl;
          block->printTransactions(1);
          cout << endl;
          break;
        }
      }
    }
}


void testSendingBlockchain(Miner* miner, int nbTransactionsByBlock, int myRank, int nProc, int tag) {
  Block* block = new Block();
  MPI_Request request;

  bool isMining = false;
  block = miner->fillBlock(block, nbTransactionsByBlock);
  block->buildMerkleTree();

  int sender;
  if (myRank == 0) {
    sender = std::experimental::randint(0,  nProc-1);
    cout << "We randomly choose a sender : Proc n°" << sender << endl;
  }
  MPI_Bcast(&sender, 1, MPI_INT, 0, MPI_COMM_WORLD);

  MPI_Barrier(MPI_COMM_WORLD);

  while(true) {
    if (myRank == sender) {
      isMining = miner->mine(block);
    }

    if (isMining == true) {
      miner->addBlock(block);
      string s = miner->serializeBlockchain(0);
      cout << "I'm proc " << myRank << " and I've sent my blockchain" << endl;
      miner->printBlockchain(0);
      cout << endl;

      for (int i=0; i<nProc; i++) {
       if (i != myRank) {
         MPI_Isend(&s[0], s.size()+1, MPI_CHAR, i, tag, MPI_COMM_WORLD, &request);
       }
      }
      break;
    } else {
      //They check if they have received a message
      MPI_Status status;
      int flag;
      MPI_Iprobe(MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &flag, &status);

      //If yes, we know it's a string so they extract it
      if (flag) {
        //We extract the length of the string
        int count;
        MPI_Get_count(&status, MPI_CHAR, &count);

        //and the string itself
        char buf [count];
        MPI_Irecv(&buf, count, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &request);
        std::string s = buf;

        miner->deserializeBlockchain(s);

        cout << "I'm proc " << myRank << " and I've received a blockchain" << endl;
        miner->printBlockchain(0);
        miner->printBlockchain(1);
        cout << endl;

        break;
      }
    }
  }
}

void testSendingEmptyBlockchain(Miner* miner, int nbTransactionsByBlock, int myRank, int nProc, int tag) {
  MPI_Request request;

  miner->clearBlockchains();

  int sender;
  if (myRank == 0) {
    sender = std::experimental::randint(0,  nProc-1);
    cout << "We randomly choose a sender : Proc n°" << sender << endl;
  }
  MPI_Bcast(&sender, 1, MPI_INT, 0, MPI_COMM_WORLD);

  MPI_Barrier(MPI_COMM_WORLD);

  while(true) {

    if (myRank == sender) {
      string s = miner->serializeBlockchain(0);
      cout << "I'm proc " << myRank << " and I've sent my empty blockchain" << endl;
      miner->printBlockchain(0);
      cout << endl;

      for (int i=0; i<nProc; i++) {
       if (i != myRank) {
         MPI_Isend(&s[0], s.size()+1, MPI_CHAR, i, tag, MPI_COMM_WORLD, &request);
       }
      }
      break;
    } else {
      //They check if they have received a message
      MPI_Status status;
      int flag;
      MPI_Iprobe(MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &flag, &status);

      //If yes, we know it's a string so they extract it
      if (flag) {
        //We extract the length of the string
        int count;
        MPI_Get_count(&status, MPI_CHAR, &count);

        //and the string itself
        char buf [count];
        MPI_Irecv(&buf, count, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &request);
        std::string s = buf;

        miner->deserializeBlockchain(s);

        cout << "I'm proc " << myRank << " and I've received a blockchain" << endl;
        miner->printBlockchain(0);
        miner->printBlockchain(1);
        cout << endl;

        break;
      }
    }
  }
}
