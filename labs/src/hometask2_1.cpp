#include <stdio.h>
#include <mpi.h>

#define TAG 111

void sendData(int& send, int& rank, int& sendRank)
{
	MPI_Send(&send, 1, MPI_INT, sendRank, TAG, MPI_COMM_WORLD);

	printf("Process %d sent data to %d\n", rank, sendRank);
}

void recvData(int& send, int& rank, int& recvRank)
{
	MPI_Status status;
	MPI_Recv(&send, 1, MPI_INT, recvRank, TAG, MPI_COMM_WORLD, &status);

	printf("Process %d received data from %d\n", rank, recvRank);
}

int main(int argc, char **argv)
{
	int rank, size, send = 1;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int sendRank = (rank == 0 ? 1 : (rank == size - 1 ? 0 : rank + 1));
	int recvRank = (rank == 0 ? size : rank) - 1;

	if (rank == 0)
	{
		sendData(send, rank, sendRank);
		recvData(send, rank, recvRank);
	}
	else
	{
		recvData(send, rank, recvRank);
		sendData(send, rank, sendRank);
	}

	MPI_Finalize();
	return 0;
}
