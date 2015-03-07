#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

int main(int argc, char **argv)
{
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank > 0)
	{
		MPI_Finalize();
		return 0;
	}

	int limit = pow(10, atoi(argv[1]));

	cout << "Process: " << rank << ", Limit: " << limit << endl;

	double startTime = MPI_Wtime(), S = 0;

	for (int i = 1; i < limit; i++)
		S += 1 / (1. + i);

	double endTime = MPI_Wtime();

	cout << "Result: " << S << ", Time elapsed: " << endTime - startTime << endl;

	MPI_Finalize();
	return 0;
}