#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <iomanip>
#include <limits>

using namespace std;

int main(int argc, char **argv)
{
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	double mpiTick = MPI_Wtick();
	cout << "Single MPI tick is " << setprecision(9) << fixed << mpiTick << " seconds" << endl;

	MPI_Finalize();
	return 0;
}