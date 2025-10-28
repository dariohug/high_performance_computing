#include "mpi.h"
#include <stdio.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes and rank of the process
    int size, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /*
    int MPI_Cart_create(MPI_Comm comm_old, int ndims, const int dims[],
    const int periods[], int reorder, MPI_Comm * comm_cart)
    
    Input Parameters
        comm_old
        input communicator (handle) 
        ndims
        number of dimensions of cartesian grid (integer) 
        dims
        integer array of size ndims specifying the number of processes in each dimension 
        periods
        logical array of size ndims specifying whether the grid is periodic (true) or not (false) in each dimension 
        reorder
        ranking may be reordered (true) or not (false) (logical) 
        
    */
    int ndims = 1;                // Onedimensional grid
    int dims[1] = {size};         // Total Number of processes in this dimension
    int periods[1] = {1};         // Periodic boundary
    int reorder = 1;              // As specified
    MPI_Comm comm_cart;           // New Cartesian communicator

    MPI_Cart_create(
        MPI_COMM_WORLD,
        ndims,
        dims,
        periods,
        reorder,
        &comm_cart
    );

    // Recompute rank based on the new communicator
    int cart_rank;
    MPI_Comm_rank(comm_cart, &cart_rank);

    // Compute left and right neighbors using MPI_Cart_shift
    int left_rank, right_rank;
    MPI_Cart_shift(comm_cart, 0, 1, &left_rank, &right_rank);

    // Step 4: Ring communication
    int send_data = cart_rank;   // Data to send
    int recv_data;               // Data to receive
    int total_sum = 0;

    double start_time = MPI_Wtime();

    for (int i = 0; i < size; i++) {
        MPI_Request requests[2];

        MPI_Irecv(&recv_data, 1, MPI_INT, left_rank, 0, comm_cart, &requests[0]);
        MPI_Isend(&send_data, 1, MPI_INT, right_rank, 0, comm_cart, &requests[1]);

        MPI_Waitall(2, requests, MPI_STATUSES_IGNORE);

        send_data = recv_data;
        total_sum += recv_data;
    }

    double elapsed = MPI_Wtime() - start_time;

    printf("Process %d (cart rank %d): left=%d, right=%d, sum=%d, time=%f sec\n",
           my_rank, cart_rank, left_rank, right_rank, total_sum, elapsed);

    MPI_Finalize();
    return 0;
}
