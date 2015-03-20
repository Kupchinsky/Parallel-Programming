#include <stdio.h>
#include <cstdlib>
#include <mpi.h>
#include <string.h>

#define TAG 111

int main(int argc, char** argv)
{
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0)
	{
		int **darr = new int*[size - 1];

		for (int i = 0; i < size - 1; i++)
		{
			darr[i] = new int[size - 1];

			for (int j = 0; j < size - 1; j++)
				darr[i][j] = i + j + 1;

			MPI_Send(darr[i], size - 1, MPI_INT, i + 1, TAG, MPI_COMM_WORLD);
		}
	}
	else
	{
		int *arr = new int[size - 1];

		MPI_Status stat;
		MPI_Recv(arr, size - 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &stat);

		char *s_arr = new char[512], buf[5];

		for (int i = 0; i < size - 1; i++)
		{
			snprintf(buf, sizeof(buf), "%d ", arr[i]);
			strcat(s_arr, buf);
		}

		printf("Rank: %d, Array (%d): %s\n", rank, size - 1, s_arr);
	}

	MPI_Finalize();
	return 0;
}