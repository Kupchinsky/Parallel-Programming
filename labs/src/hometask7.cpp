#include <stdio.h>
#include <mpi.h>
#include <sstream>

using namespace std;

int** splitpointers(int *arr, int n, int split_size)
{
	int size = n / split_size, **result = new int*[size];

	for (int i = 0; i < size; i++)
		result[i] = arr + (split_size * i);

	return result;
}

int main(int argc, char** argv)
{
	int rank, size, n;

	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &size );

	if (rank == 0)
		n = atoi(argv[1]);
		//scanf("%d", &n);

	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int *A = new int[n], *singleB, **B;

	if (rank == 0)
	{
		// Заполняем матрицу
		singleB = new int[size * n];
		B = splitpointers(singleB, size * n, n);

		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < n; j++)
				B[i][j] = i + j;
		}
	}

	MPI_Scatter(B[0], n, MPI_INT, A, n, MPI_INT, 0, MPI_COMM_WORLD);

	// Выводим строку
	stringstream ss;

	for (int i = 0; i < n; i++)
		ss << A[i] << " ";

	printf("Rank: %d, Arr: %s\n", rank, ss.str().c_str());

	if (rank == 0)
	{
		// Дополнительно выводим всю матрицу
		stringstream ss2;

		for (int i = 0; i < size; i++)
		{
			ss2 << "(" << i << ") ";

			for (int j = 0; j < n; j++)
				ss2 << B[i][j] << " ";

			ss2 << endl;
		}

		printf("Source Arr:\n%s\n", ss2.str().c_str());
	}

	MPI_Finalize();
	return 0;
}