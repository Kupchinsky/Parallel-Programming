#include <stdio.h>
#include <mpi.h>
#include <cstdlib>
#include <string.h>

#define TAG 111
#define _DEBUG 1 // Для отладки

void printarr(int *arr, int n)
{
	char buf[16], result[256];

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

	// Размер матрицы, вектор, результирующий вектор, матрица, строк на процесс, остаток (лишние строки)
	int n = atoi(argv[1]), *vec, *resultvec, *matrix, linesPerProcess, linesPerProcessMod = 0;

	#if _DEBUG > 1
	printf("n = %d, size = %d\n", n, size);
	#endif

	if (rank == 0)
	{
		vec = new int[n]; resultvec = new int[n];
		matrix = new int[n * n]; // Матрица в виде одной большой строки, для более удобной передачи

		// Заполняем матрицу и вектор
		for (int i = 0; i < n; i++)
		{
			vec[i] = rand() % 10;
			for (int j = i * n, limit = (i + 1) * n; j < limit; j++) matrix[j] = rand() % 10;
		}

		#if _DEBUG > 0
		printf("matrix: ");
		printarr(matrix, n * n);

		printf("vector: ");
		printarr(vec, n);
		#endif

		linesPerProcess = n / size; // Количество строк на процесс
		linesPerProcessMod = n % size; // Лишние строки

		#if _DEBUG > 0
		printf("linesPerProcess = %d, linesPerProcessMod = %d\n", linesPerProcess, linesPerProcessMod);
		#endif

		for (int i = 1, sendMatrixSize = linesPerProcess * n; i < size; i++) // sendMatrixSize = 3 * 10 = 30
		{
			MPI_Send(&linesPerProcess, 1, MPI_INT, i, TAG, MPI_COMM_WORLD);
			MPI_Send(vec, n, MPI_INT, i, TAG, MPI_COMM_WORLD);

			MPI_Send(matrix + (sendMatrixSize * i), sendMatrixSize, MPI_INT, i, TAG, MPI_COMM_WORLD);

			#if _DEBUG > 1
			printf("i = %d, start index = %d, size = %d\n", i, sendMatrixSize * i, sendMatrixSize);
			#endif
		}
	}
	else
	{
		MPI_Recv(&linesPerProcess, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &stat);

		vec = new int[n]; resultvec = new int[linesPerProcess];
		matrix = new int[linesPerProcess * n];

		MPI_Recv(vec, n, MPI_INT, 0, TAG, MPI_COMM_WORLD, &stat);
		MPI_Recv(matrix, linesPerProcess * n, MPI_INT, 0, TAG, MPI_COMM_WORLD, &stat);

		#if _DEBUG > 1
		printarr(vec, n);
		printarr(matrix, linesPerProcess * n);
		#endif
	}

	#if _DEBUG > 0
	printf("worker: limit = %d\n", linesPerProcess + linesPerProcessMod);
	#endif

	for (int i = 0, limit = linesPerProcess + linesPerProcessMod; i < limit; i++)
	{
		#if _DEBUG > 1
		printf("j = %d, j < %d\n", i * n, (i + 1) * n);
		#endif

		resultvec[i] = 0;

		for (int j = i * n, k = 0; j < (i + 1) * n; j++, k++)
			resultvec[i] += matrix[j] * vec[k];

		#if _DEBUG > 1
		printarr(matrix + (i * n), n);
		#endif
	}

	if (rank == 0)
	{
		for (int left = size - 1; left > 0; left--)
		{
			MPI_Probe(MPI_ANY_SOURCE, TAG, MPI_COMM_WORLD, &stat);

			#if _DEBUG > 0
			printf("target index: %d, count: %d\n", linesPerProcess * stat.MPI_SOURCE + linesPerProcessMod, linesPerProcess);
			#endif

			MPI_Recv(resultvec + (linesPerProcess * stat.MPI_SOURCE + linesPerProcessMod), linesPerProcess, MPI_INT, stat.MPI_SOURCE, TAG, MPI_COMM_WORLD, &stat);
		}

		printf("result vector: ");
		printarr(resultvec, n);
	}
	else
		MPI_Send(resultvec, linesPerProcess, MPI_INT, 0, TAG, MPI_COMM_WORLD);

	delete[] vec; delete[] resultvec; delete[] matrix;

	MPI_Finalize();
	return 0;
}