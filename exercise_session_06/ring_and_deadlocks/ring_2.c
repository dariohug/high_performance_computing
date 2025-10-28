#include "mpi.h"
#include <stdio.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes and rank of the process
    int size, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int my_sum = 0;

    int send_rank = my_rank;  // Send    buffer
    int recv_rank = 0;        // Receive buffer

    // Compute the ranks of left/right neighbours 
    int left_rank, right_rank;
    MPI_Request requests[2];
    
    // Compute the ranks of left/right neighbours
    left_rank = (my_rank - 1 + size) % size;
    right_rank = (my_rank + 1) % size;

    double start_time = MPI_Wtime();

    // // Loop over the number of processes
    // for (int i = 0; i < size; i++) {
    //     // MPI_Recv(&recv_rank, 1, MPI_INT, left_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    //     // MPI_Ssend(&send_rank, 1, MPI_INT, right_rank, 0, MPI_COMM_WORLD);
    //     MPI_Irecv(&recv_rank, 1, MPI_INT, left_rank, 0, MPI_COMM_WORLD, &requests[1]);
    //     MPI_Isend(&send_rank, 1, MPI_INT, right_rank, 0, MPI_COMM_WORLD, &requests[0]);
    //     MPI_Waitall(2, requests, MPI_STATUSES_IGNORE);
        
    //     send_rank = recv_rank;
        
    //     my_sum += recv_rank;
    // }

    double elapsed = MPI_Wtime() - start_time;

    printf("I am processor %d out of %d, my left neighbor is %d, the sum is %d and it took me %f seconds\n", my_rank, size, left_rank, my_sum, elapsed);

    // Finalize the MPI environment.
    MPI_Finalize();
}
