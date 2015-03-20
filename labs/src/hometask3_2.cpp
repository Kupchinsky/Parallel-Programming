#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	int n = 100000000;
	double a = 0, b = 0.00000001, starttime = MPI_Wtime();

	for (int i = 0; i < n; i++)
		a += b;

	double endtime = MPI_Wtime();

	printf("MFlops: %.3f\n", 1 / ((endtime - starttime) / n) / 1000000);

	MPI_Finalize();
	return 0;
}