#include "mpi.h"
#include <stdio.h>
#include "init.h"

int start_MPI(int* my_rank, int* size){
	printf("Setting MPI environment\n");
	MPI_Init(NULL,NULL);
	MPI_Comm_size(MPI_COMM_WORLD,size);
	MPI_Comm_rank(MPI_COMM_WORLD,my_rank);

	return 0;
}

int close_MPI(){
	MPI_Finalize();
	return 0;
}

int mpi_get_domain(int nx, int ny, int my_rank, int size, int* min_x, int* max_x, int* min_y, int* max_y){
    /*
    define corners or local domains
    */
    // Decompose in x direction only
    int slice_size = nx / size;
    int remainder = nx % size;
    
    // Calculate xmin and xmax for this process
    if (my_rank < remainder) {
        // First 'remainder' processes get an extra row
        *min_x = my_rank * (slice_size + 1);
        *max_x = *min_x + slice_size + 1;
    } else {
        // Remaining processes get the base slice size
        *min_x = my_rank * slice_size + remainder;
        *max_x = *min_x + slice_size;
    }
    
    // All processes handle the full y domain
    *min_y = 0;
    *max_y = ny;

    printf("I am rank %d and my domain is: xmin, xmax, ymin, ymax: %d %d %d %d\n", my_rank, *min_x, *max_x, *min_y, *max_y);
    return 0;
}

int halo_comm(params p, int my_rank, int size, double** u, double* fromLeft, double* fromRight){
	
	// Initialize fromLeft and fromRight
	for (int j = 0; j < (p.ymax - p.ymin); j++) {
		fromLeft[j] = 0; 
		fromRight[j] = 0;
	}

	// Prepare columns to send
	double* column_to_right = new double[p.ymax - p.ymin];
	for (int j = 0; j < (p.ymax - p.ymin); j++) 
		column_to_right[j] = u[p.xmax - p.xmin - 1][j]; 
	
	double* column_to_left = new double[p.ymax - p.ymin];
	for (int j = 0; j < (p.ymax - p.ymin); j++) 
		column_to_left[j] = u[0][j];

	int left_neighbor = my_rank - 1;
	int right_neighbor = my_rank + 1;

	// Exchange with right neighbor
	if (right_neighbor < size) {
		MPI_Sendrecv(column_to_right, p.ymax - p.ymin, MPI_DOUBLE, right_neighbor, 0,
					 fromRight, p.ymax - p.ymin, MPI_DOUBLE, right_neighbor, 1,
					 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	// Exchange with left neighbor
	if (left_neighbor >= 0) {
		MPI_Sendrecv(column_to_left, p.ymax - p.ymin, MPI_DOUBLE, left_neighbor, 1,
					 fromLeft, p.ymax - p.ymin, MPI_DOUBLE, left_neighbor, 0,
					 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	delete[] column_to_right;
	delete[] column_to_left;

	return 0;
}


int ALLREDUCE(double* loc_diff, double* loc_sumdiff){

	MPI_Allreduce(loc_diff, loc_sumdiff, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
	return 0;

	}

