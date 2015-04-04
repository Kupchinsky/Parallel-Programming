#include <stdio.h>
#include <mpi.h>
#include <cstdlib>
#include <string>
#include <time.h>
#include <sstream>

#define BCAST_TAG 1111

void MPI_MyBcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
{
	int rank, size;
	MPI_Status stat;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	// Не выполняемся при 1 процессе
	if (size == 1)
		return;

	int currentSend, currentRecv, last = (root != 0 ? root : size) - 1, log = 1;

	while (true)
	{
		for (int i = 0; i < log; i++)
		{
			currentSend = root + i;
			currentRecv = log + currentSend;

			if (currentSend >= size)
				currentSend -= size;
			if (currentRecv >= size)
				currentRecv -= size;

			if (rank == currentSend)
				MPI_Send(buffer, count, datatype, currentRecv, BCAST_TAG, comm);
			else if (rank == currentRecv)
				MPI_Recv(buffer, count, datatype, currentSend, BCAST_TAG, comm, &stat);

			if (currentRecv == last) // Последний процесс
				return;
		}

		log *= 2;
	}
}

int main(int argc, char** argv)
{
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	double time_original = 0, time_my = 0;
	int n = 200, *arr = new int[n], target = size / 2;

	if (target == rank)
	{
		for (int i = 0; i < n; i++)
			arr[i] = i + i;
	}

	for(int i = 0; i < 5; i++)
	{
		// Original
		MPI_Barrier(MPI_COMM_WORLD);

		if (rank == 0)
			time_original = MPI_Wtime();

		MPI_Bcast(arr, n, MPI_INT, target, MPI_COMM_WORLD);

		if (rank == 0)
			time_original = MPI_Wtime() - time_original;

		// My
		MPI_Barrier(MPI_COMM_WORLD);

		if (rank == 0)
			time_my = MPI_Wtime();

		MPI_MyBcast(arr, n, MPI_INT, target, MPI_COMM_WORLD);

		if (rank == 0)
		{
			time_my = MPI_Wtime() - time_my;
			printf("My: %.9lf; Original: %.9lf\n", time_my, time_original);
		}
	}

	MPI_Finalize();
	return 0;
}