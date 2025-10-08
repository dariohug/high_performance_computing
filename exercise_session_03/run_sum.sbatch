#!/bin/bash
#SBATCH --job-name=sum_job          # Job name
#SBATCH --output=sum_output.txt  # Output file (%j = job ID)
#SBATCH --error=sum_error.txt    # Error file
#SBATCH --ntasks=1                  # Number of tasks (processes)
#SBATCH --cpus-per-task=8           # CPU cores per task
#SBATCH --time=00:10:00             # Max runtime (HH:MM:SS)
#SBATCH --mem=8G                    # Memory for the job
#SBATCH --partition=normal          # Partition/queue

# Load compiler module
module load gcc-native/12.3

# Compile your program
cc -O3 -fopenmp -o sum sum.c get_time.c -lm

# Run your program
./sum
