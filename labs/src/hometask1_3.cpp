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

	char *processorName = new char[MPI_MAX_PROCESSOR_NAME];
	int length;

	MPI_Get_processor_name(processorName, &length);

	cout << "MPI_Get_processor_name returned: " << processorName << endl;

	MPI_Finalize();
	return 0;
}