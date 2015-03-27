#include <stdio.h>
#include <mpi.h>
#include <cstdlib>
#include <string.h>

#define TAG 111

void printarr(int *arr, int n)
{
	char buf[6], result[256];

	result[0] = '\0';

	for (int i = 0; i < n; i++)
	{
		snprintf(buf, sizeof(buf), "%d ", arr[i]);
		strcat(result, buf);
	}

	printf("%s\n", result);
}

int main(int argc, char** argv)
{
	int rank, size;
	MPI_Status stat;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int n = atoi(argv[1]), *vec, *resultvec, **matrix;

	if (rank == 0)
	{
		vec = new int[n]; resultvec = new int[n];
		matrix = new int*[n];

		// Заполняем матрицу и вектор
		for (int i = 0; i < n; i++)
		{
			vec[i] = rand() % 10;
			matrix[i] = new int[n];

			for (int j = 0; j < n; j++) matrix[i][j] = rand() % 10;
		}

		for (int i = 0; i < n; i++)
		{
			resultvec[i] = 0;
			for (int j = 0; j < n; j++) resultvec[i] += matrix[i][j] * vec[j];
		}

		printarr(resultvec, n);

		for (int i = 0; i < n; i++)
			delete[] matrix[i];

		delete[] vec; delete[] resultvec; delete[] matrix;
	}

	MPI_Finalize();
	return 0;
}