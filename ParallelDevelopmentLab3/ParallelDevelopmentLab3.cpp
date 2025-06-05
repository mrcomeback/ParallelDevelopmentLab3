// This program is to caculate PI using MPI
// The algorithm is based on Monte Carlo method. The Monte Carlo method randomly picks up a large number of points in a square. It only counts the ratio of pints in side the circule.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define N 1E9
#define d 1.0/N

int main(int argc, char* argv[])
{
	int rank, size, error;

	int len;
	long long int i, sum = 0, result = 0;
	char procname[MPI_MAX_PROCESSOR_NAME];

	double pi = 0.0, begin = 0.0, end = 0.0, x, y;

	error = MPI_Init(&argc, &argv);

	//Get process ID
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	//Get processes Number
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	//Get proccessor name
	MPI_Get_processor_name(procname, &len);

	//Synchronize all processes and get the begin time
	MPI_Barrier(MPI_COMM_WORLD);
	begin = MPI_Wtime();

	srand((int)time(0));
	double pr_start = MPI_Wtime();
	//Each process will caculate a part of the sum
	for (i = rank; i < N; i += size)
	{
		x = rand() / (RAND_MAX + 1.0);
		y = rand() / (RAND_MAX + 1.0);
		if (x * x + y * y < 1.0)
			result++;
	}
	double pr_end = MPI_Wtime();
	double pr_time = pr_end - pr_start;

	printf("Process %d time %lf on %s\n", rank, pr_time, procname);
	//Sum up all results
	MPI_Reduce(&result, &sum, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	//Synchronize all processes and get the end time
	MPI_Barrier(MPI_COMM_WORLD);
	end = MPI_Wtime();

	//Caculate and print PI
	if (rank == 0)
	{
		pi = 4 * d * sum;
		printf("np=%2d;    Time=%fs;    PI=%0.4f\n", size, end - begin, pi);
	}

	error = MPI_Finalize();

	return 0;
}
