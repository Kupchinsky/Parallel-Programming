#include <stdio.h>
#include <mpi.h>

#define TAG 111
#define SIZE 20

int main(int argc, char **argv)
{
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0)
	{
		int *arr = new int[SIZE];

		for (int i = 0; i < SIZE; i++)
			arr[i] = i;

		for (int i = 1, k = 0; i < size; i++, k += 2)
		{
			MPI_Send(arr + k, 2, MPI_INT, i, TAG, MPI_COMM_WORLD);
			printf("Process %d sent data [%d, %d] to %d\n", rank, arr[k], arr[k + 1], i);
		}
	}
	else
	{
		MPI_Status status;
		int *arr = new int[2];

		MPI_Recv(arr, 2, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);
		printf("Process %d received data [%d, %d]\n", rank, arr[0], arr[1]);
	}

	MPI_Finalize();
	return 0;
}
