#!/bin/bash
#SBATCH --job-name=axpy-gpu
#SBATCH --account=uzh8
#SBATCH --constraint=gpu
#SBATCH --gres=gpu:1
#SBATCH --ntasks=1
#SBATCH --time=00:10:00

module load cray
module load nvhpc

module unload cray-libsci cray-mpich

export PE_ENV=NVIDIA

make

export ACC_NOTIFY=3  
export ACC_DEVICE_TYPE=gpu
# srun -n 1 ./blur.openacc 24 2000
srun -n 1 ./blur.openacc 26