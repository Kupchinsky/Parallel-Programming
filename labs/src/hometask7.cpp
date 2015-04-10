#include <stdio.h>
#include <mpi.h>
#include <sstream>

using namespace std;

int main(int argc, char** argv)
{
	int rank, size, n;

	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &size );

	if (rank == 0)
		//n = atoi(argv[1]);
		scanf("%d", &n);

	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int *A = new int[n], *B = new int[size * n];

	if (rank == 0)
	{
		for (int i = 0; i < size * n; i++)
			B[i] = i;
	}

	MPI_Scatter(B, n, MPI_INT, A, n, MPI_INT, 0, MPI_COMM_WORLD);

	// Выводим строку
	string buf;

	for (int i = 0; i < n; i++)
	{
		stringstream ss;
		ss << A[i];
		buf += ss.str() + " ";
	}

	printf("Rank: %d, Arr: %s\n", rank, buf.c_str());

	if (rank == 0)
	{
		// Дополнительно выводим всю матрицу
		buf = "";

		for (int i = 0; i < size * n; i++)
		{
			if (i % n == 0)
				buf += "\n";

			stringstream ss;
			ss << B[i];
			buf += ss.str() + " ";
		}

		printf("Source Arr:%s\n", buf.c_str());
	}

	MPI_Finalize();
	return 0;
}