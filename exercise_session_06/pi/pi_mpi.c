#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

int main(int argc, char** argv) {
    // Leibniz Formula: 
    // Pi/4 = sum, n = 0 -> inf ((-1)^n)/2n + 1

    // MPI idea: Split the work into chunks and compute these together in the end 

    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const long long N = 1000000000LL;
    long long base = N / size;
    long long remainder = N % size; // Since it not always works 
    long long start = rank * base + (rank < remainder ? rank : remainder);
    long long count = base + (rank < remainder ? 1 : 0);

    double local_sum = 0.0;
    // Reduce the amount of compitations 
    int sign = (start % 2 == 0) ? 1 : -1;

    for (long long i = start; i < start + count; ++i) {
        local_sum += (double)sign / (double)(2.0 * i + 1.0);
        sign = -sign;
    }

    double global_sum = 0.0;
    // Count the local sums together
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double pi_est = global_sum * 4.0;
        printf("number of processes: %d\n", size);
        printf("π ≈ %.15f\n", pi_est);
    }

    MPI_Finalize();
    return 0;
}
