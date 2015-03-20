#include <stdio.h>
#include <mpi.h>
#include <cstdlib>
#include <math.h>

#define TAG 111

int main(int argc, char** argv)
{
	int rank, size;
	MPI_Status stat;
	int limitArr[2];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0)
	{
		int limit = pow(10, atoi(argv[1]));
		int limitPerProcess = limit / (size - 1);
		int limitMod = limit % (size - 1);

		printf("Limit: %d\n", limit);

		for (int i = 1, currentLimit = 0; i < size; i++, currentLimit += limitPerProcess)
		{
			limitArr[0] = currentLimit + 1;
			limitArr[1] = currentLimit + limitPerProcess + (i == size - 1 ? limitMod : 0);

			MPI_Send(limitArr, 2, MPI_INT, i, TAG, MPI_COMM_WORLD);
		}

		double startTime = MPI_Wtime();
		double S = 0, tmp;

		for (int i = 1; i < size; i++)
		{
			MPI_Recv(&tmp, 1, MPI_DOUBLE, MPI_ANY_SOURCE, TAG, MPI_COMM_WORLD, &stat);
			S += tmp;
		}

		double elapsedTime = MPI_Wtime() - startTime;

		printf("Result: %f, time elapsed: %e\n", S, elapsedTime);
	}
	else
	{
		MPI_Recv(limitArr, 2, MPI_INT, 0, TAG, MPI_COMM_WORLD, &stat);
		double S = 0;

		for (int i = limitArr[0]; i <= limitArr[1]; i++)
			S += 1 / (1. + i);

		printf("Rank: %d, Start: %d, End: %d, S: %f\n", rank, limitArr[0], limitArr[1], S);

		MPI_Send(&S, 1, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}