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

		char s_arr[512], buf[5];
		MPI_Status stat;

		for (int i = 1; i < size - 1; i++)
		{
			darr[i] = new int[size - 1];
			MPI_Recv(darr[i], size - 1, MPI_INT, i, TAG, MPI_COMM_WORLD, &stat);

			s_arr[0] = '\0';

			for (int j = 0; j < size - 1; j++)
			{
				snprintf(buf, sizeof(buf), "%d ", darr[i][j]);
				strcat(s_arr, buf);
			}

			printf("From: %d, Array: %s\n", i, s_arr);
		}
	}
	else
	{
		int *arr = new int[size - 1];

		for (int i = 0; i < size - 1; i++)
			arr[i] = rank + i;

		MPI_Send(arr, size - 1, MPI_INT, 0, TAG, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}