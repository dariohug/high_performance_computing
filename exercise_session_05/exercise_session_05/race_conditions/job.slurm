#!/bin/bash
#SBATCH --job-name=race_conditions
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --time=01:00:00
#SBATCH --mem=10G

module load gcc-native/12.3
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

srun ./parallel

