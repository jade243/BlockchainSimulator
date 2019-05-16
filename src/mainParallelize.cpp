#include <mpi.h>
#include <iostream>
#include "TransactionsGenerator.cpp"

using namespace std;

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
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  MPI_Comm_size(MPI_COMM_WORLD, &nProc);

  //Data
  vector<string> transactions;

  //===== Proc 0 will prepare the data for all nodes
  if (myRank == 0) {
    //Write general informations in a file
    cout << "Informations about this execution" << endl;
    cout << "nbTransactions : " << nbTransactions << endl;
    cout << "nbBlocks : " << nbBlocks << " with " << nbTransactionsByBlock << " transactions each" << endl;
    cout << "nbProc : " << nProc << endl;

    //Generate the random transactions and transmit them to others
    TransactionsGenerator* sim = new TransactionsGenerator(nbUsers, nbTransactions);
    transactions = sim->generateVectorTransactions();

    sim->printTransactions(transactions);
  }

  MPI_Finalize();

}
