#!/bin/bash

## This is a Slurm job script for Lab 2: mm-seq.cpp

#SBATCH --job-name=lab2-mmseq
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --partition=xs-4114
#SBATCH --mem=1gb
#SBATCH --time=00:03:00
#SBATCH --output=lab2_mmseq_%j.slurmlog
#SBATCH --error=lab2_mmseq_%j.slurmlog
#SBATCH --mail-type=NONE

# Check that one argument was passed (for matrix size)
if [ ! "$#" -eq 1 ]
then
  echo "Expecting 1 argument (<matrix size>), got $#"
  exit 1
fi

echo "Running job: $SLURM_JOB_NAME!"
echo "We are running on $(hostname)"
echo "Job started at $(date)"
echo "Arguments to your executable: $@"

# Compile your code in case you forgot
echo "Compiling..."
srun g++ -o mm-seq mm-seq.cpp
# Runs your script with the arguments you passed in
echo "Running..."
srun perf stat ./mm-seq $@

echo "Job ended at $(date)"