#!/bin/bash
#SBATCH --job-name=cuda_pi
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --time=01:00:00
#SBATCH --mem=10G

module load nvhpc

nvcc -O3 -arch=sm_90 cpi_cuda.cu gettime.c -o cuda_acc
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

srun ./cuda_acc
