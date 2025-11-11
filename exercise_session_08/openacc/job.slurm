#!/bin/bash
#SBATCH --job-name=openacc
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --time=01:00:00
#SBATCH --mem=10G

module load nvhpc

nvc -O3 -acc -gpu=cc90 cpi_openacc.c gettime.c -o cpi_acc
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

srun ./cpi_acc
