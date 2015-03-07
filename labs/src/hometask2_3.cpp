#include <stdio.h>
#include <mpi.h>

#define TAG 111
#define SIZE 3

int main(int argc, char **argv)
{
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0)
	{
		MPI_Status status;
		int arr_size = (size * SIZE) - SIZE;
		int *arr = new int[arr_size];

		for (int i = 1, k = 0; i < size; i++, k += SIZE)
			MPI_Recv(arr + k, SIZE, MPI_INT, i, TAG, MPI_COMM_WORLD, &status);

		printf("Process %d received array: ", rank);

		for (int i = 0; i < arr_size; i++)
			printf("%d, ", arr[i]);

		printf("\n");
	}
	else
	{
		int *arr = new int[SIZE];

		for (int i = 0; i < SIZE; i++)
			arr[i] = rank;

		MPI_Send(arr, SIZE, MPI_INT, 0, TAG, MPI_COMM_WORLD);
		printf("Process %d sent data\n", rank);
	}

	MPI_Finalize();
	return 0;
}
