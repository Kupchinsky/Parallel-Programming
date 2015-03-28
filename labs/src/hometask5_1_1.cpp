#include <stdio.h>
#include <mpi.h>
#include <cstdlib>
#include <string>
#include <sstream>

#define TAG 111

using namespace std;

void printarr(int *arr, int n)
{
	string result;

	for (int i = 0; i < n; i++)
	{
		stringstream ss;
		ss << arr[i];

		result += ss.str() + " ";
	}

	printf("%s\n", result.c_str());
}

int main(int argc, char** argv)
{
	int rank, size;
	MPI_Status stat;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int n = atoi(argv[1]), *vec, *resultvec, **matrix;
	double starttime;

	if (rank == 0)
	{
		vec = new int[n]; resultvec = new int[n];
		matrix = new int*[n];

		// Заполняем матрицу и вектор
		for (int i = 0; i < n; i++)
		{
			vec[i] = rand() % 1000;
			matrix[i] = new int[n];

			for (int j = 0; j < n; j++) matrix[i][j] = rand() % 1000;
		}

		starttime = MPI_Wtime();

		// Считаем
		for (int i = 0; i < n; i++)
		{
			resultvec[i] = 0;
			for (int j = 0; j < n; j++)
				resultvec[i] += matrix[i][j] * vec[j];
		}

		double endtime = MPI_Wtime() - starttime;

		//printf("result vector: ");
		//printarr(resultvec, n);
		printf("elapsed time: %e\n", endtime);

		for (int i = 0; i < n; i++)
			delete[] matrix[i];

		delete[] vec; delete[] resultvec; delete[] matrix;
	}

	MPI_Finalize();
	return 0;
}