#!/bin/bash
#SBATCH --job-name=sum_omp
#SBATCH --output=sum_omp_all.txt
#SBATCH --error=sum_omp_error.txt
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=16        # Maximum threads you want to test
#SBATCH --time=00:20:00
#SBATCH --mem=8G
#SBATCH --partition=normal

module load gcc-native/12.3

# Compile the program with OpenMP
cc -O3 -fopenmp -o sum_omp sum.c get_time.c -lm

# List of thread counts to test
THREADS=(1 2 4 8 16)

# Loop over thread counts
for NTHREADS in "${THREADS[@]}"; do
    echo "=====================================" >> sum_omp_all.txt
    echo "Running with $NTHREADS threads" >> sum_omp_all.txt
    export OMP_NUM_THREADS=$NTHREADS
    ./sum_omp >> sum_omp_all.txt
done

