#!/bin/bash

## Change this to a job name you want
#SBATCH --job-name=lab3_job

## Change based on length of job and `sinfo` partitions available
#SBATCH --partition=gpu

## Request for a specific type of node
## Commented out for now, change if you need one
##SBATCH --constraint xgpe

## gpu:1 ==> any gpu. For e.g., gpu:a100-40:1 gets you one of the A100 GPU shared instances
#SBATCH --gres=gpu:1

## Must change this based on how long job will take. We are just expecting 30 seconds for now
#SBATCH --time=00:00:30

## Probably no need to change anything here
#SBATCH --ntasks=1

## May want to change this depending on how much host memory you need
## #SBATCH --mem-per-cpu=10G

## Just useful logfile names
#SBATCH --output=lab3_%j.slurmlog
#SBATCH --error=lab3_%j.slurmlog


echo "Job is running on $(hostname), started at $(date)"

# Get some output about GPU status
nvidia-smi 

# Set the nvidia compiler directory
NVCC=/usr/local/cuda/bin/nvcc

# Check that it exists and print some version info
[[ -f $NVCC ]] || { echo "ERROR: NVCC Compiler not found at $NVCC, exiting..."; exit 1; }
echo "NVCC info: $($NVCC --version)"

# Actually compile the code
echo -e "\n====> Compiling...\n"
$NVCC -arch sm_70 -O3 --std=c++17 -o sum sum.cu
echo -e "\n====> Running...\n"
./sum

echo -e "\n====> Finished running.\n"

echo -e "\nJob completed at $(date)"
