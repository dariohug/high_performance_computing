#!/bin/bash
#SBATCH --job-name=cuda_scale
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --time=01:00:00
#SBATCH --mem=10G

module load nvhpc

nvcc -O3 -arch=sm_90 cpi_cuda_experiments.cu gettime.c -o cuda_scale

NUM_BLOCKS=(60 120 180 240 300 360 420 600)
NUM_THREADS=(16 32 48 64 80 96 112 128 144 160)

for B in "${NUM_BLOCKS[@]}"; do
    for T in "${NUM_THREADS[@]}"; do
        echo "Running NUM_BLOCK=$B NUM_THREAD=$T"
        srun ./cuda_scale $B $T
    done
done
