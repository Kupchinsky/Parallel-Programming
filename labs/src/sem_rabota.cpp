#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <mpi.h>
#include <limits.h>

using namespace std;

#define vector_box vector<long>
#define map_boxes map< vector_box*, long> // Камни, Вес (не используется)

#define TAG 111
#define PARALLEL

#ifdef PARALLEL
	#define out(x) cout << "Rank: " << rank << ", " << x << endl
#else
	#define out(x) cout << x << endl
#endif

void addbox(map_boxes& boxes)
{
    boxes[new vector_box()] = 0;
}

#ifdef PARALLEL
	long* merge(long *v1, unsigned int n1, long *v2, unsigned int n2)
	{
		long *result = new long[n1 + n2], i = 0, j = 0;

		for (unsigned int k = 0; k < n1 + n2; k++)
		{
			if (i >= n1)
			{
				result[k] = v2[j];
				j++;
			}
			else if (j >= n2)
			{
				result[k] = v1[i];
				i++;
			}
			else if (v1[i] < v2[j])
			{
				result[k] = v1[i];
				i++;
			}
			else
			{
				result[k] = v2[j];
				j++;
			}
		}

		return result;
	}
#endif

int main(int argc, char *argv[])
{
	int rank, size;
	double start_time;

	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &size );

    vector<long> st;
    map_boxes boxes;

#ifndef PARALLEL
	if (rank != 0)
	{
		MPI_Finalize();
		return 0;
	}
#endif

#ifdef PARALLEL
	MPI_Status stat;

	if (rank == 0)
	{
#endif

		out("LONG_MAX = " << LONG_MAX);

		start_time = MPI_Wtime();
		out("Start time got");

		// Ящики
		for (unsigned int i = 0; i < 10; i++)
			addbox(boxes);

		srand(time(NULL));
		out("Inserting...");

		// Камни
		for (unsigned int i = 1; i <= 100000000; i++)
			st.push_back(i * 10);

#ifdef PARALLEL

		// Рассылка части на 2-й процесс
		out("Sending...");

		unsigned int part_count = st.size() / 2;

		MPI_Send(&part_count, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD);
		MPI_Send(&(*st.begin()), part_count, MPI_LONG, 1, TAG, MPI_COMM_WORLD);

		// Удаляем вторую часть
		st.erase(st.begin(), st.begin() + part_count);
	}
	else
	{
		// Принятие второй части
		out("Receiving...");

		unsigned int part_count;
		MPI_Recv(&part_count, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &stat);

		long *received_array = new long[part_count];
		MPI_Recv(received_array, part_count, MPI_LONG, 0, TAG, MPI_COMM_WORLD, &stat);

		// Отправляем её в вектор
		st.assign(received_array, received_array + part_count);
		delete[] received_array;
	}
#endif

    out("Sorting...");
    sort(st.begin(), st.end(), [] (long i, long j) -> bool
    {
        return i < j;
    });

#ifdef PARALLEL
	if (rank == 0)
	{
		// Принимаем отсортированный массив с 2-го процесса
		out("Receiving...");

		unsigned int part_count;
		MPI_Recv(&part_count, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD, &stat);

		long *received_array = new long[part_count];
		MPI_Recv(received_array, part_count, MPI_LONG, 1, TAG, MPI_COMM_WORLD, &stat);

		// Объединяем в один
		out("Mergeing...");

		long *merged_array = merge(received_array, part_count, &(*st.begin()), st.size()), all_count = part_count + st.size();
		delete[] received_array;

		st.clear();
		st.assign(merged_array, merged_array + all_count);
		delete[] merged_array;
#endif

		out("Running...");

		while(st.size())
		{
			map_boxes::iterator box = boxes.begin();

			long min_boxweight = box->second;
			vector_box *min_box = box->first;

			for (; box != boxes.end(); box++)
			{
				if (box->second < min_boxweight)
				{
					min_boxweight = box->second;
					min_box = box->first;
				}
			}

			min_box->push_back(st.back());
			boxes[min_box] += st.back();
			st.pop_back();
		}

		out("Elapsed time: " << MPI_Wtime() - start_time);
		out("Result:");

		for (map_boxes::iterator box = boxes.begin(); box != boxes.end(); box++)
		{
			delete box->first;
			out("Weight: " << box->second);
		}

#ifdef PARALLEL
	}
	else
	{
		// Отправка второй отсортированной части
		out("Sending...");

		unsigned int part_count = st.size();

		MPI_Send(&part_count, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD);
		MPI_Send(&(*st.begin()), part_count, MPI_LONG, 0, TAG, MPI_COMM_WORLD);
	}
#endif

	MPI_Finalize();
    return 0;
}
