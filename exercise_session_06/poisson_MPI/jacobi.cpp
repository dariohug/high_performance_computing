#include "jacobi.h"
#include <math.h>
#include "mpi_module.h"
#include "mpi.h"

/**
 * @brief      Computes norm of the difference between two matrices
 *
 * @param[in]  p     Parameters
 * @param      mat1  The matrix 1
 * @param      mat2  The matrix 2
 *
 * @return     Returns \sqrt{\sum(mat1_{ij} - mat2_{ij})^2/(nx*ny)}
 */
double norm_diff(params p, double** mat1, double** mat2, int my_rank){

    double local_sum = 0., diff = 0.;
    for (int i = p.xmin; i < p.xmax; i++){
        for (int j = p.ymin; j < p.ymax; j++){
            int idx = i - p.xmin;
            int idy = j - p.ymin;
            diff = mat1[idx][idy] - mat2[idx][idy];
            local_sum += diff * diff;
        }
    }
    
    double global_sum = 0.;
    MPI_Allreduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    
    double ret = sqrt(global_sum / (p.nx * p.ny));
    return ret;
}

/**
 * @brief      Compute a Jacobi iteration to solve Poisson equation. This function updates
 *             u_new and u_old in-place.
 *
 * @param[in]  p      Parameters
 * @param      u_new  The new solution
 * @param      u_old  The old solution
 * @param      f      The source term
 */
void jacobi_step(params p, double** u_new, double** u_old, double** f, int my_rank, int size){
    double dx = 1.0/((double)p.nx - 1);
    double dy = 1.0/((double)p.ny - 1);

    double* fromLeft = new double[p.ymax - p.ymin]; 
    double* fromRight = new double[p.ymax - p.ymin];

    halo_comm(p, my_rank, size, u_new, fromLeft, fromRight); 

    for (int i=p.xmin; i<p.xmax; i++){
        for (int j=p.ymin; j<p.ymax; j++){
            u_old[i - p.xmin][j - p.ymin] = u_new[i - p.xmin][j - p.ymin];
        }
    }

    for (int i=p.xmin; i<p.xmax; i++){
        if (i==0 || i==p.nx-1) continue;
        for (int j=p.ymin; j<p.ymax; j++){
            if (j==0 || j==p.ny-1) continue;
            int idx = i-p.xmin;
            int idy = j-p.ymin;
            
            double left = (i == p.xmin) ? fromLeft[idy] : u_old[idx-1][idy];
            double right = (i == p.xmax-1) ? fromRight[idy] : u_old[idx+1][idy];
            
            u_new[idx][idy] = 0.25*(left + right + u_old[idx][idy-1] + u_old[idx][idy+1] - dx*dy*f[idx][idy]);
        }
    }
    
    delete[] fromLeft;
    delete[] fromRight;
    
    if (p.nx!=p.ny) printf("In function jacobi_step (jacobi.cpp l.26): nx != ny, check jacobi updates\n");
}
