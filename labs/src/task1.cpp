#include <stdio.h>
#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	printf0("Process: %d, Process Count: %d\n", rank, size);

	cout << "lol" << endl;

	MPI_Finalize();
	return 0;
};