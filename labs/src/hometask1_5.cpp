#include <stdio.h>
#include <mpi.h>
#include <iostream>

using namespace std;

#define N 100

int main(int argc, char **argv)
{
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int *a = new int[N], S = 0;

	for (int i = 0; i < N; i++)
	{
		a[i] = i;

		if (i < rank + 1)
			S += a[i];
	}

	printf("Process: %d, Result: %d\n", rank, S);

	delete[] a;

	MPI_Finalize();
	return 0;
}